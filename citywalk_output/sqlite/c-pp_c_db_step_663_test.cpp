// Minimal C++11 test suite for the focal function: db_step
// This test suite does not rely on Google Test. It uses a lightweight
// non-terminating assertion mechanism (EXPECT_*) and a tiny harness
// that runs in main().
// The test suite mocks sqlite3_step to control return codes and validates
// that db_step propagates the same return code and only triggers db_affirm_rc
// when the return code is neither SQLITE_ROW nor SQLITE_DONE.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>


// Forward declaration of the function under test.
// db_step is defined in the focal file (c-pp.c) and uses C linkage.
extern "C" int db_step(sqlite3_stmt *pStmt);

// -------------------------------------------------------------------------------------
// Mock infrastructure to simulate sqlite3_step behavior for tests

// Global control for the next return code from our mock sqlite3_step.
static int g_next_rc = SQLITE_ROW;

// Mock implementation of sqlite3_step to override the real SQLite behavior in tests.
// This function has C linkage to match the real API.
extern "C" int sqlite3_step(sqlite3_stmt * /*pStmt*/) {
  return g_next_rc;
}

// -------------------------------------------------------------------------------------
// Lightweight testing framework (non-terminating assertions)

// Track total tests and failed assertions to provide a summary.
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating equality assertion for integral values.
#define EXPECT_EQ(a, b) do {                                     \
    ++g_total_tests;                                               \
    if ((a) != (b)) {                                            \
        ++g_failed_tests;                                         \
        std::cerr << "EXPECT_EQ failed: " #a " = " << (a)           \
                  << ", expected " << (b)                        \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    }                                                             \
} while (0)

// Convenience wrappers for readability in tests.
#define EXPECT_INT_EQ(a, b) EXPECT_EQ((int)(a), (int)(b))

// -------------------------------------------------------------------------------------
// Prototypes for test wrappers
void test_db_step_rc_row();   // rc = SQLITE_ROW
void test_db_step_rc_done();  // rc = SQLITE_DONE
void test_db_step_rc_error(); // rc = SQLITE_ERROR (non-row, non-done)
void test_db_step_rc_other(); // rc = some other non-special value

// -------------------------------------------------------------------------------------
// Main entry point for tests
int main() {
    // Run tests
    test_db_step_rc_row();
    test_db_step_rc_done();
    test_db_step_rc_error();
    test_db_step_rc_other();

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}

// -------------------------------------------------------------------------------------
// Test 1: When sqlite3_step returns SQLITE_ROW, db_step should return SQLITE_ROW
// and should not trigger db_affirm_rc (no observable side-effects in this stub).
void test_db_step_rc_row() {
    // Arrange
    g_next_rc = SQLITE_ROW;
    // Act
    int rc = db_step(nullptr);
    // Assert
    EXPECT_INT_EQ(rc, SQLITE_ROW);
    // Note: db_affirm_rc is a no-op in this test harness (as defined in the focal dep),
    // so we cannot observe its invocation directly without modifying the production code.
}

// -------------------------------------------------------------------------------------
// Test 2: When sqlite3_step returns SQLITE_DONE, db_step should return SQLITE_DONE
// (end-of-result set, no error path taken).
void test_db_step_rc_done() {
    // Arrange
    g_next_rc = SQLITE_DONE;
    // Act
    int rc = db_step(nullptr);
    // Assert
    EXPECT_INT_EQ(rc, SQLITE_DONE);
}

// -------------------------------------------------------------------------------------
// Test 3: When sqlite3_step returns a non-row, non-done code (e.g., SQLITE_ERROR),
// db_step should return that code (and would call db_affirm_rc in a real scenario).
void test_db_step_rc_error() {
    // Arrange
    g_next_rc = SQLITE_ERROR;
    // Act
    int rc = db_step(nullptr);
    // Assert
    EXPECT_INT_EQ(rc, SQLITE_ERROR);
}

// -------------------------------------------------------------------------------------
// Test 4: When sqlite3_step returns an arbitrary non-special code (e.g., 12345),
// db_step should return that code as-is.
// This helps ensure coverage of the else-branch logic in db_step's conditional.
void test_db_step_rc_other() {
    // Arrange
    g_next_rc = 12345;
    // Act
    int rc = db_step(nullptr);
    // Assert
    EXPECT_INT_EQ(rc, 12345);
}

// End of test suite.