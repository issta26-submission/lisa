// Self-contained unit-test suite for sqlite3_prepare16 wrapper
// This test reproduces the focal method behavior in a minimal, isolated environment
// without relying on GTest. It verifies the wrapper's interaction with the underlying
// sqlite3Prepare16 implementation (provided here as a static function) and covers
// simple, non-aborting execution paths.
//
// Notes:
// - We simulate a minimal sqlite3 and sqlite3_stmt types.
// - The sqlite3Prepare16 function (static) determines rc based on a simple string in zSql.
// - The wrapper sqlite3_prepare16 delegates to sqlite3Prepare16 and then asserts the
//   OR-branch rc==SQLITE_OK || ppStmt==0 || *ppStmt==0 to avoid aborts during tests.
// - Tail pointer (pzTail) is updated by the static implementation to verify data flow.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cassert>


// Domain constants (matching SQLite semantics for the test subset)
static const int SQLITE_OK = 0;
static const int SQLITE_ERROR = 1;

// Lightweight stand-ins for SQLite types
struct sqlite3 {};
struct sqlite3_stmt {};

// Sentinel value used to verify pzTail propagation
static const void* const TAIL_SENTINEL = reinterpret_cast<const void*>(0xDEADC0DE);

// Forward declaration of the static (internal) implementation
static int sqlite3Prepare16(sqlite3 *db,
                          const void *zSql,
                          int nBytes,
                          unsigned int prepFlags,
                          sqlite3_stmt **ppStmt,
                          const void **pzTail);

// Wrapper under test: sqlite3_prepare16
int sqlite3_prepare16(
  sqlite3 *db,              
  const void *zSql,         
  int nBytes,               
  sqlite3_stmt **ppStmt,    
  const void **pzTail       
){
  int rc;
  rc = sqlite3Prepare16(db, zSql, nBytes, 0, ppStmt, pzTail);
  // Non-terminating assertion path: avoid aborts during testing
  assert( rc==SQLITE_OK || ppStmt==0 || *ppStmt==0 );  /* VERIFY: F13021 */
  return rc;
}

// Static implementation used by the wrapper; this is where test behavior is simulated
static int sqlite3Prepare16(sqlite3 *db,
                          const void *zSql,
                          int nBytes,
                          unsigned int prepFlags,
                          sqlite3_stmt **ppStmt,
                          const void **pzTail)
{
  // Basic input validation
  if (zSql == nullptr) {
    // Simulate an error for null SQL input
    if (pzTail != nullptr) *pzTail = TAIL_SENTINEL;
    if (ppStmt != nullptr) *ppStmt = nullptr;
    return SQLITE_ERROR;
  }

  // Interpret zSql content as a small test "command"
  const char* sqlStr = static_cast<const char*>(zSql);
  bool isOk = true;
  if (std::strcmp(sqlStr, "OK") == 0) {
    isOk = true;
  } else if (std::strcmp(sqlStr, "ERR") == 0) {
    isOk = false;
  } else {
    // Default to OK for other inputs
    isOk = true;
  }

  // Propagate a sentinel tail to verify data flow
  if (pzTail != nullptr) {
    *pzTail = TAIL_SENTINEL;
  }

  // Populate a statement handle if operation is OK
  if (ppStmt != nullptr) {
    if (isOk) {
      *ppStmt = new sqlite3_stmt();
    } else {
      *ppStmt = nullptr;
    }
  }

  return isOk ? SQLITE_OK : SQLITE_ERROR;
}

// Simple test harness (no GTest). Each test returns true on pass, false on failure.
// Tests also print diagnostic information to stdout.
namespace {

int g_failures = 0;

// Helper to report a single failing test with context
#define TEST_FAIL(msg) do { \
  std::cerr << "TEST FAILURE: " << msg << "\n"; \
  ++g_failures; \
} while (0)

bool test_ok_path_with_stmt_and_tail() {
  // Test the true-branch path: rc == SQLITE_OK, with a non-null ppStmt and tail propagation
  sqlite3 db;
  sqlite3_stmt* stmt = nullptr;
  const void* tail = nullptr;

  const char* sql = "OK";
  int rc = sqlite3_prepare16(&db, static_cast<const void*>(sql),
                             static_cast<int>(std::strlen(sql)),
                             &stmt, &tail);

  bool passed = (rc == SQLITE_OK) &&
                (stmt != nullptr) &&
                (tail == TAIL_SENTINEL);

  if (!passed) {
    TEST_FAIL("test_ok_path_with_stmt_and_tail: expected rc=SQLITE_OK, stmt non-null, tail sentinel.");
  }

  if (stmt) {
    delete stmt;
  }
  return passed;
}

bool test_error_path_with_nonnull_ppStmt() {
  // Test the false-branch path: rc != SQLITE_OK and ppStmt is provided.
  // The internal implementation sets *ppStmt to nullptr so the assertion in the wrapper passes.
  sqlite3 db;
  sqlite3_stmt* stmt = reinterpret_cast<sqlite3_stmt*>(0x1); // non-null initial value
  const void* tail = nullptr;

  const char* sql = "ERR";
  int rc = sqlite3_prepare16(&db, static_cast<const void*>(sql),
                             static_cast<int>(std::strlen(sql)),
                             &stmt, &tail);

  bool passed = (rc == SQLITE_ERROR) &&
                (stmt == nullptr) &&                 // underlying impl clears non-null stmt on error
                (tail == TAIL_SENTINEL);

  if (!passed) {
    TEST_FAIL("test_error_path_with_nonnull_ppStmt: expected rc=SQLITE_ERROR, stmt cleared, tail sentinel.");
  }
  return passed;
}

bool test_error_path_with_null_ppStmt() {
  // Test rc != SQLITE_OK path when ppStmt is null (should bypass *ppStmt dereference in assert)
  sqlite3 db;
  const void* tail = nullptr;

  const char* sql = "ERR";
  int rc = sqlite3_prepare16(&db, static_cast<const void*>(sql),
                             static_cast<int>(std::strlen(sql)),
                             /*ppStmt*/ nullptr, &tail);

  bool passed = (rc == SQLITE_ERROR) && (tail == TAIL_SENTINEL);

  if (!passed) {
    TEST_FAIL("test_error_path_with_null_ppStmt: expected rc=SQLITE_ERROR and tail sentinel when ppStmt is null.");
  }
  return passed;
}

} // anonymous namespace

int main() {
  std::cout << "Starting sqlite3_prepare16 wrapper tests (non-GTest, C++11).\n";

  bool t1 = test_ok_path_with_stmt_and_tail();
  bool t2 = test_error_path_with_nonnull_ppStmt();
  bool t3 = test_error_path_with_null_ppStmt();

  if (t1 && t2 && t3 && g_failures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cout << "Tests completed with failures: " << g_failures << "\n";
    return g_failures;
  }
}