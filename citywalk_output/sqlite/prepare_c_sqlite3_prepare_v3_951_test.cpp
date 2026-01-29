// Minimal, self-contained unit test suite for the focal method sqlite3_prepare_v3
// Implemented in a single translation unit to avoid external dependencies.
// This test suite mocks the dependent function sqlite3LockAndPrepare and
// stubs the required SQLite types/constants to exercise the core logic of
// sqlite3_prepare_v3 as defined in the provided focal method.

// Domain/Step notes (embedded as comments):
// - Step 1: Analyze focal method: sqlite3_prepare_v3 delegates to sqlite3LockAndPrepare
//           with flags = SQLITE_PREPARE_SAVESQL | (prepFlags & SQLITE_PREPARE_MASK)
//           and then asserts rc==SQLITE_OK || ppStmt==0 || *ppStmt==0.
// - Step 2: Generate tests emphasizing the interaction with prepFlags masking,
//           rc values (OK vs error), and pointer-out parameters ppStmt and pzTail.
// - Step 3: Refine tests to cover true/false branches of masking and tail pointer.
// - All tests avoid terminating assertions by using a small non-terminating test harness.

#include <vector>
#include <sstream>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


//------------------------------------------------------------------------------
// Minimal, self-contained SQLite-like type definitions and constants
//------------------------------------------------------------------------------

struct sqlite3;       // opaque handle (mock)
struct sqlite3_stmt;  // opaque statement (mock)

static const int SQLITE_OK = 0;
static const unsigned int SQLITE_PREPARE_SAVESQL = 0x01;
static const unsigned int SQLITE_PREPARE_MASK   = 0xFF;

//------------------------------------------------------------------------------
// Global state used by the mocked dependency and test assertions
//------------------------------------------------------------------------------

static int g_rc_to_return = SQLITE_OK;             // rc value that sqlite3LockAndPrepare returns
static bool g_ppStmt_must_be_nonnull = false;      // whether to set *ppStmt to a non-null dummy
static const char* g_tail_str = nullptr;           // value assigned to *pzTail by mock
static unsigned int g_last_passed_prep_flags = 0;  // value observed by the mock for prepFlags

static int g_call_count_lock_prepare = 0;          // how many times mock invoked

//------------------------------------------------------------------------------
// Mocked dependency: sqlite3LockAndPrepare
// It records parameters, optionally sets *ppStmt, returns a configurable rc.
//------------------------------------------------------------------------------

int sqlite3LockAndPrepare(sqlite3 *db,
                          const char *zSql,
                          int nBytes,
                          unsigned int prepFlags,
                          void *pOld,
                          sqlite3_stmt **ppStmt,
                          const char **pzTail)
{
    (void)db; (void)zSql; (void)nBytes; (void)pOld;
    g_last_passed_prep_flags = prepFlags;
    ++g_call_count_lock_prepare;

    if (ppStmt) {
        if (g_ppStmt_must_be_nonnull) {
            static sqlite3_stmt dummy;
            *ppStmt = &dummy;
        } else {
            *ppStmt = nullptr;
        }
    }

    if (pzTail) {
        *pzTail = g_tail_str;
    }

    return g_rc_to_return;
}

//------------------------------------------------------------------------------
// Focal method under test: sqlite3_prepare_v3
// The implementation mirrors the provided source exactly, enabling isolated testing.
//------------------------------------------------------------------------------

int sqlite3_prepare_v3(
  sqlite3 *db,              /* Database handle. */
  const char *zSql,         /* UTF-8 encoded SQL statement. */
  int nBytes,               /* Length of zSql in bytes. */
  unsigned int prepFlags,   /* Zero or more SQLITE_PREPARE_* flags */
  sqlite3_stmt **ppStmt,    /* OUT: A pointer to the prepared statement */
  const char **pzTail       /* OUT: End of parsed string */
){
  int rc;
  rc = sqlite3LockAndPrepare(db, zSql, nBytes,
                 SQLITE_PREPARE_SAVESQL | (prepFlags & SQLITE_PREPARE_MASK),
                 0, ppStmt, pzTail);
  assert( rc==SQLITE_OK || ppStmt==0 || *ppStmt==0 );
  return rc;
}

//------------------------------------------------------------------------------
// Lightweight non-terminating test harness (no GTest/GMock)
// Provides EXPECT-like macros, reports failures, and runs tests from main.
//------------------------------------------------------------------------------

static std::vector<std::string> g_failures;

static void log_failure(const std::string& s) {
    g_failures.push_back(s);
}

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { \
        std::ostringstream oss; oss << "EXPECT_TRUE failed: " << (msg); log_failure(oss.str()); } } while(0)

#define EXPECT_EQ_INT(a, b, msg) \
    do { int _a = (int)(a); int _b = (int)(b); \
         if(_a != _b) { \
             std::ostringstream oss; \
             oss << "EXPECT_EQ_INT failed: " << (msg) \
                 << " (expected " << _b << ", got " << _a << ")"; \
             log_failure(oss.str()); \
         } } while(0)

#define EXPECT_PTR_EQ(a, b, msg) \
    do { void* _a = (void*)(a); void* _b = (void*)(b); \
         if(_a != _b) { \
             std::ostringstream oss; \
             oss << "EXPECT_PTR_EQ failed: " << (msg) \
                 << " (expected " << _b << ", got " << _a << ")"; \
             log_failure(oss.str()); \
         } } while(0)

#define EXPECT_STR_EQ(a, b, msg) \
    do { const char* _a = (a); const char* _b = (b); \
         if(_a != _b && (!(_a && _b) || std::string(_a) != std::string(_b))) { \
             std::ostringstream oss; \
             oss << "EXPECT_STR_EQ failed: " << (msg) \
                 << " (expected \"" << (_b ? _b : "NULL") << "\", got \"" \
                 << (_a ? _a : "NULL") << "\")"; \
             log_failure(oss.str()); \
         } } while(0)

static void run_all_tests();

// Test Case 1: rc = SQLITE_OK, prepFlags contains a masked value; ensure flags passed to mock match masked result
// and that zTail is propagated, ppStmt is null when configured.
static void test_case1_ok_masked_flags_and_tail() {
    // Arrange
    g_rc_to_return = SQLITE_OK;
    g_ppStmt_must_be_nonnull = false;
    g_tail_str = "TAIL1";

    // Act
    sqlite3 db;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    int rc = sqlite3_prepare_v3(&db, "SELECT 1", 8, 0xAA, &stmt, &tail);

    // Assert
    EXPECT_EQ_INT(rc, SQLITE_OK, "Case1: rc should be SQLITE_OK");
    unsigned int expected_flags = SQLITE_PREPARE_SAVESQL | (0xAA & SQLITE_PREPARE_MASK);
    EXPECT_EQ_INT((int)g_last_passed_prep_flags, (int)expected_flags,
                  "Case1: prepFlags should be masked and OR'ed with SAVESQL");
    EXPECT_PTR_EQ(stmt, nullptr, "Case1: ppStmt should be null when mock sets it to null");
    EXPECT_PTR_EQ(tail, g_tail_str, "Case1: pzTail should reflect the provided tail string");
}

// Test Case 2: rc = non-OK, prepFlags masked; ensure assertion path is kept valid by having ppStmt be null
// and tail left as NULL (as not provided).
static void test_case2_error_rc_masked_flags() {
    // Arrange
    g_rc_to_return = -1;                 // non-OK
    g_ppStmt_must_be_nonnull = false;    // ensure *ppStmt will be set to nullptr by mock
    g_tail_str = nullptr;

    // Act
    sqlite3 db;
    sqlite3_stmt* stmt = reinterpret_cast<sqlite3_stmt*>(0x1); // non-null local dummy to verify pointer handling
    const char* tail = "SENTINEL"; // value won't be used if the mock sets to nullptr
    int rc = sqlite3_prepare_v3(&db, "SELECT 1", 9, 0xDEADBEEF, &stmt, &tail);

    // Assert
    EXPECT_EQ_INT(rc, -1, "Case2: rc should be non-OK as configured");
    unsigned int expected_flags = SQLITE_PREPARE_SAVESQL | (0xDEADBEEF & SQLITE_PREPARE_MASK);
    EXPECT_EQ_INT((int)g_last_passed_prep_flags, (int)expected_flags,
                  "Case2: prepFlags should be masked correctly even on error");
    EXPECT_PTR_EQ(stmt, nullptr, "Case2: ppStmt should be nullptr due to test config");
    EXPECT_PTR_EQ(tail, nullptr, "Case2: pzTail should be nullptr when not provided");
}

// Test Case 3: rc = SQLITE_OK with ppStmt forced non-null by mock; ensures non-null ppStmt does not affect OK branch.
// Also verifies tail propagation and masking works for a different prepFlags.
static void test_case3_ok_with_nonnull_ppstmt() {
    // Arrange
    g_rc_to_return = SQLITE_OK;
    g_ppStmt_must_be_nonnull = true;
    g_tail_str = "TAIL3";

    // Act
    sqlite3 db;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    int rc = sqlite3_prepare_v3(&db, "SELECT 1", 9, 0x23, &stmt, &tail);

    // Assert
    EXPECT_EQ_INT(rc, SQLITE_OK, "Case3: rc should be SQLITE_OK");
    unsigned int expected_flags = SQLITE_PREPARE_SAVESQL | (0x23 & SQLITE_PREPARE_MASK);
    EXPECT_EQ_INT((int)g_last_passed_prep_flags, (int)expected_flags,
                  "Case3: prepFlags masked correctly with non-null ppStmt");
    EXPECT_TRUE(stmt != nullptr, "Case3: ppStmt should be non-null when mock sets it");
    EXPECT_PTR_EQ(tail, g_tail_str, "Case3: pzTail should reflect the provided tail string");
}

// Run all tests and report results
static void run_all_tests() {
    test_case1_ok_masked_flags_and_tail();
    test_case2_error_rc_masked_flags();
    test_case3_ok_with_nonnull_ppstmt();

    if(g_failures.empty()){
        std::cout << "All tests passed: sqlite3_prepare_v3 focal method.\n";
    } else {
        std::cout << g_failures.size() << " test failures:\n";
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "  [" << i+1 << "] " << g_failures[i] << "\n";
        }
    }
}

//------------------------------------------------------------------------------
// Main entry point
//------------------------------------------------------------------------------

int main() {
    // Run the test suite
    run_all_tests();
    return (int)g_failures.size();
}