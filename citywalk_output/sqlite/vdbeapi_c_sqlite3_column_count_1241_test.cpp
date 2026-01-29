// Lightweight C++11 unit tests for sqlite3_column_count (focus: vdbeapi.c)
// No GoogleTest/GMock used. Tests are executed from main().
// This test suite relies on internal SQLite headers to align with actual
// Vdbe structure layout (N.B.: static/internal helpers are not invoked).

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <opcodes.h>


// Include internal SQLite headers to align with the focal method's dependencies.
// These headers define the Vdbe structure and sqlite3_stmt type used by sqlite3_column_count.

// Simple non-terminating test assertions.
// They log failures but allow the test suite to continue running.
static int g_test_failures = 0;

#define EXPECT_EQ(actual, expected)                                         \
  do {                                                                        \
    if ((actual) != (expected)) {                                           \
      std::cerr << "[TEST_FAIL] " << __FUNCTION__ << ":" << __LINE__        \
                << "  Expected: " << (expected)                               \
                << "  Actual: " << (actual) << std::endl;                 \
      ++g_test_failures;                                                    \
    }                                                                       \
  } while (0)

#define EXPECT_TRUE(condition)                                                 \
  do {                                                                         \
    if (!(condition)) {                                                        \
      std::cerr << "[TEST_FAIL] " << __FUNCTION__ << ":" << __LINE__            \
                << "  Condition is false: " << #condition << std::endl;      \
      ++g_test_failures;                                                       \
    }                                                                        \
  } while (0)


// Step 2 - Test Suite for sqlite3_column_count
// Focus: verify correct behavior for null pointer and non-null Vdbe pointer cases.
// sqlite3_column_count is defined as:
//   Vdbe *pVm = (Vdbe *)pStmt;
//   if( pVm==0 ) return 0;
//   return pVm->nResColumn;
static void test_sqlite3_column_count_null_ptr(void) {
    // True branch: pStmt is NULL -> should return 0
    int res = sqlite3_column_count(nullptr);
    EXPECT_EQ(res, 0);
}

// Create a Vdbe instance with a known nResColumn value and pass it as sqlite3_stmt*.
// The function will cast to Vdbe* and read nResColumn.
static void test_sqlite3_column_count_non_null_with_value(void) {
    Vdbe v;
    v.nResColumn = 7; // arbitrary non-zero value to verify correct read
    int res = sqlite3_column_count((sqlite3_stmt *)&v);
    EXPECT_EQ(res, 7);
}

// Edge case: non-null Vdbe with nResColumn = 0 should still return 0 (no crash, proper value).
static void test_sqlite3_column_count_non_null_with_zero(void) {
    Vdbe v;
    v.nResColumn = 0;
    int res = sqlite3_column_count((sqlite3_stmt *)&v);
    EXPECT_EQ(res, 0);
}


// Step 3 - Test Runner (non-terminating assertions; call tests from main)
int main() {
    // Run all tests
    test_sqlite3_column_count_null_ptr();
    test_sqlite3_column_count_non_null_with_value();
    test_sqlite3_column_count_non_null_with_zero();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[TEST_SUMMARY] ALL tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[TEST_SUMMARY] " << g_test_failures
                  << " test(s) failed." << std::endl;
        return g_test_failures;
    }
}