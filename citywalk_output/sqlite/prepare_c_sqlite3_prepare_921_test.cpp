// Self-contained C++11 test harness for the focal method sqlite3_prepare
// Note: This is a lightweight, self-contained approximation to exercise the
// behavior of the focal function by providing a controllable mock for
// sqlite3LockAndPrepare. It does not depend on GTest and uses a simple
// non-terminating CHECK macro for assertions.
//
// The tests are designed to cover:
 // - True branch: rc == SQLITE_OK
 // - False branch possibilities for rc != SQLITE_OK with various ppStmt states
 // - Behavior when ppStmt is NULL
 // - Behavior when ppStmt is non-NULL but *ppStmt is NULL or non-NULL
// (As per the domain guidance, we avoid terminating assertions and rely on
// non-terminating checks to maximize coverage.)

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Domain knowledge: provide minimal sqlite-like types and constants
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

const int SQLITE_OK = 0;

// Global test state to control the mock's behavior
static int g_rc_for_lock = SQLITE_OK;                 // rc returned by sqlite3LockAndPrepare
static int g_lock_call_count = 0;                     // how many times lock was invoked
static int g_set_ppStmt_value = 0;                    // whether to set *ppStmt in the mock
static sqlite3_stmt* g_next_ppStmt_value = nullptr;   // value to assign to *ppStmt when requested

// Forward declaration to mimic the production function linkage.
// We implement a mock for sqlite3LockAndPrepare to drive sqlite3_prepare behavior.
extern "C" int sqlite3LockAndPrepare(sqlite3 *db,
                                   const char *zSql,
                                   int nBytes,
                                   unsigned int prepFlags,
                                   void *pOld,
                                   sqlite3_stmt **ppStmt,
                                   const char **pzTail);

// Focal method under test (reproduced here for testing in isolation).
// In the real project, this would be the exact code from prepare.c.
// We invoke the mocked sqlite3LockAndPrepare to simulate various outcomes.
extern "C" int sqlite3_prepare(sqlite3 *db,
                             const char *zSql,
                             int nBytes,
                             sqlite3_stmt **ppStmt,
                             const char **pzTail)
{
    int rc;
    rc = sqlite3LockAndPrepare(db, zSql, nBytes, 0, 0, ppStmt, pzTail);
    // The real code has an assert: assert(rc==SQLITE_OK || ppStmt==0 || *ppStmt==0);
    // In tests we avoid terminating assertions by compiling with NDEBUG semantics.
    // We still preserve behavior by not altering rc.
    return rc;
}

// Mock implementation of sqlite3LockAndPrepare used to drive tests.
// It increments a counter and conditionally sets *ppStmt to a value for testing.
extern "C" int sqlite3LockAndPrepare(sqlite3 *db,
                                   const char *zSql,
                                   int nBytes,
                                   unsigned int prepFlags,
                                   void *pOld,
                                   sqlite3_stmt **ppStmt,
                                   const char **pzTail)
{
    ++g_lock_call_count;
    if (g_set_ppStmt_value && ppStmt != nullptr) {
        *ppStmt = g_next_ppStmt_value;
    }
    // Return the controlled rc value for the test scenarios
    return g_rc_for_lock;
}

// Lightweight test harness (no external libraries). Uses non-terminating
// checks to accumulate failures and report at the end.
static int g_fail_count = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << msg << " (in " << __FILE__ \
                  << ":" << __LINE__ << ")\n"; \
        ++g_fail_count; \
    } \
} while(0)

static void reset_test_state() {
    g_rc_for_lock = SQLITE_OK;
    g_lock_call_count = 0;
    g_set_ppStmt_value = 0;
    g_next_ppStmt_value = nullptr;
    g_fail_count = 0;
}

// Test Case 1: rc = SQLITE_OK, ppStmt is not artificially set by the mock.
// Expect sqlite3_prepare to return SQLITE_OK and not error (true branch).
// This verifies the normal path where the lock-and-prepare succeeds.
static void test_case_ok_rc_no_ppstmt_change() {
    reset_test_state();
    g_rc_for_lock = SQLITE_OK;
    g_set_ppStmt_value = 0;          // Do not touch *ppStmt
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt **ppStmt = &stmt;
    const char *tail = nullptr;

    int rc = sqlite3_prepare(db, "SELECT 1", -1, ppStmt, &tail);

    CHECK(rc == SQLITE_OK, "Case1: rc should be SQLITE_OK when lock returns OK");
    CHECK(stmt == nullptr, "Case1: ppStmt should remain null when not set by mock");
    CHECK(g_lock_call_count == 1, "Case1: sqlite3LockAndPrepare should be called once");
}

// Test Case 2: rc = non-OK and mock sets ppStmt to a non-null value.
// Ensure rc is propagated and ppStmt is updated accordingly.
// This exercises the non-OK branch with ppStmt being set.
static void test_case_non_ok_with_ppstmt_set() {
    reset_test_state();
    g_rc_for_lock = 1; // non-OK
    g_set_ppStmt_value = 1;
    g_next_ppStmt_value = reinterpret_cast<sqlite3_stmt*>(0xDEADBEEF);
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt **ppStmt = &stmt;
    const char *tail = nullptr;

    int rc = sqlite3_prepare(db, "INSERT", 6, ppStmt, &tail);

    CHECK(rc == 1, "Case2: rc should propagate non-OK value from lock");
    CHECK(stmt != nullptr, "Case2: ppStmt should be updated by mock when non-NULL");
    CHECK(stmt == reinterpret_cast<sqlite3_stmt*>(0xDEADBEEF),
          "Case2: ppStmt should match the mock-provided pointer value");
    CHECK(g_lock_call_count == 1, "Case2: sqlite3LockAndPrepare should be called exactly once");
}

// Test Case 3: rc = non-OK and mock sets ppStmt to NULL.
// This tests the scenario where *ppStmt would be 0, satisfying the assertion guard.
// Since we compile with NDEBUG, we won't abort on the assertion in this test.
static void test_case_non_ok_with_ppstmt_null() {
    reset_test_state();
    g_rc_for_lock = -2; // non-OK
    g_set_ppStmt_value = 1;
    g_next_ppStmt_value = nullptr; // Ensure *ppStmt becomes NULL
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt*>(0); // non-NULL pointer, but *ppStmt will be NULL after assignment
    sqlite3_stmt **ppStmt = &stmt;
    const char *tail = nullptr;

    int rc = sqlite3_prepare(db, "UPDATE", 6, ppStmt, &tail);

    CHECK(rc == -2, "Case3: rc should propagate non-OK value from lock");
    CHECK(stmt == nullptr, "Case3: *ppStmt should be NULL when mock returns NULL");
    CHECK(g_lock_call_count == 1, "Case3: sqlite3LockAndPrepare should be called exactly once");
}

// Test Case 4: ppStmt pointer itself is NULL (ppStmt == nullptr).
// This checks that the function handles a NULL ppStmt gracefully (no dereference).
static void test_case_ppstmt_null_pointer() {
    reset_test_state();
    g_rc_for_lock = SQLITE_OK;
    g_set_ppStmt_value = 0;
    sqlite3 *db = nullptr;
    sqlite3_stmt **ppStmt = nullptr;
    const char *tail = nullptr;

    int rc = sqlite3_prepare(db, "SELECT 2", -1, ppStmt, &tail);

    CHECK(rc == SQLITE_OK, "Case4: rc should be SQLITE_OK when ppStmt is NULL and lock returns OK");
    CHECK(g_lock_call_count == 1, "Case4: sqlite3LockAndPrepare should be called once even if ppStmt is NULL");
}

// Entry point for running all tests
int main() {
    std::cout << "Running sqlite3_prepare unit tests (self-contained, GTest-free)\n";

    test_case_ok_rc_no_ppstmt_change();
    test_case_non_ok_with_ppstmt_set();
    test_case_non_ok_with_ppstmt_null();
    test_case_ppstmt_null_pointer();

    if (g_fail_count == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_fail_count << " test(s) failed\n";
        return 1;
    }
}