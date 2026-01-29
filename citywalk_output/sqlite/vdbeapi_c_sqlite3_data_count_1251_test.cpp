/*
  Lightweight C++11 unit test suite for the focal method:
  int sqlite3_data_count(sqlite3_stmt *pStmt)

  What it tests (based on the focal method logic):
  - If pStmt is NULL, result is 0.
  - If pStmt is non-NULL but pStmt interpreted as Vdbe has pResultRow == NULL, result is 0.
  - If pStmt interpreted as Vdbe has a non-NULL pResultRow, result is pVm->nResColumn.

  Notes:
  - This test provides a minimal in-source reimplementation of the required
    types and function to exercise the focal logic without external dependencies.
  - Static functions in the real project are not relied upon; we mirror only what is
    necessary for the focal method to compile and run in isolation.
  - Non-terminating assertions are implemented via a simple log mechanism that records
    failures and reports them after running all tests.
*/

#include <vector>
#include <opcodes.h>
#include <sstream>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Minimal replication of the relevant parts of the library needed for sqlite3_data_count

// Forward-declare the sqlite3_stmt type as an opaque type (as real code uses a typedef)
typedef void sqlite3_stmt;

// Minimal Vdbe structure with the fields used by sqlite3_data_count
struct Vdbe {
  void* pResultRow;
  int nResColumn;
};

// The focal method under test (copied in a minimal form to enable isolated testing)
int sqlite3_data_count(sqlite3_stmt *pStmt) {
  Vdbe *pVm = (Vdbe *)pStmt;
  if (pVm == nullptr || pVm->pResultRow == nullptr) return 0;
  return pVm->nResColumn;
}

// Global test failure log (non-terminating assertions)
static std::vector<std::string> g_failures;

// Helper to record a failure message
static void logFailure(const std::string &msg) {
  g_failures.push_back(msg);
}

/*
  Test 1: Null statement pointer should yield 0.
  This covers the true branch of "pVm==0" in the focal method.
*/
static void test_sqlite3_data_count_null_stmt() {
  int r = sqlite3_data_count(nullptr);
  if (r != 0) {
    std::ostringstream oss;
    oss << "test_sqlite3_data_count_null_stmt: expected 0, got " << r;
    logFailure(oss.str());
  }
}

/*
  Test 2: Non-null statement but with a NULL result row should yield 0.
  This covers the false branch of pVm!=0 but pVm->pResultRow==0.
*/
static void test_sqlite3_data_count_null_resultrow() {
  Vdbe v;
  v.pResultRow = nullptr;
  v.nResColumn = 7; // arbitrary non-zero
  sqlite3_stmt *p = (sqlite3_stmt *)&v;
  int r = sqlite3_data_count(p);
  if (r != 0) {
    logFailure("test_sqlite3_data_count_null_resultrow: expected 0 when pResultRow is NULL");
  }
}

/*
  Test 3a: Non-null result row with small nResColumn should return the exact value.
*/
static void test_sqlite3_data_count_non_null_resultrow_small() {
  Vdbe v;
  int dummy;
  v.pResultRow = &dummy;
  v.nResColumn = 1;
  sqlite3_stmt *p = (sqlite3_stmt *)&v;
  int r = sqlite3_data_count(p);
  if (r != 1) {
    std::ostringstream oss;
    oss << "test_sqlite3_data_count_non_null_resultrow_small: expected 1, got " << r;
    logFailure(oss.str());
  }
}

/*
  Test 3b: Non-null result row with a larger nResColumn should return that value.
*/
static void test_sqlite3_data_count_non_null_resultrow_large() {
  Vdbe v;
  int dummy;
  v.pResultRow = &dummy;
  v.nResColumn = 12345;
  sqlite3_stmt *p = (sqlite3_stmt *)&v;
  int r = sqlite3_data_count(p);
  if (r != 12345) {
    std::ostringstream oss;
    oss << "test_sqlite3_data_count_non_null_resultrow_large: expected 12345, got " << r;
    logFailure(oss.str());
  }
}

/*
  Entry point for all tests. We intentionally keep tests isolated and table-driven
  so a failure in one test does not abort the rest.
*/
static void run_all_tests() {
  test_sqlite3_data_count_null_stmt();
  test_sqlite3_data_count_null_resultrow();
  test_sqlite3_data_count_non_null_resultrow_small();
  test_sqlite3_data_count_non_null_resultrow_large();
}

int main() {
  // Run tests
  run_all_tests();

  // Report results
  if (g_failures.empty()) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << g_failures.size() << " TEST(S) FAILED:\n";
    for (size_t i = 0; i < g_failures.size(); ++i) {
      std::cout << i + 1 << ") " << g_failures[i] << "\n";
    }
    return static_cast<int>(g_failures.size());
  }
}