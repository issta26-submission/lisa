// Comprehensive unit tests for sqlite3Fts5ExprRowid
// This test suite targets the focal method:
//   i64 sqlite3Fts5ExprRowid(Fts5Expr *p) { return p->pRoot->iRowid; }
// It relies on the library's public/internal types provided via fts5Int.h and fts5parse.h.
// The tests are written in C++11 without GoogleTest and use a lightweight assertion approach.

#include <fts5Int.h>
#include <cstdio>
#include <climits>
#include <fts5parse.h>
#include <stdio.h>
#include <cstdint>


extern "C" {
  // Import necessary dependencies to ensure ABI compatibility with the focal function.
  // These headers provide the Fts5Expr and related structures as used by sqlite3Fts5ExprRowid.
  #include "fts5Int.h"
  #include "fts5parse.h"

  // Prototype for the focal method (in C linkage to match the library's implementation)
  i64 sqlite3Fts5ExprRowid(Fts5Expr *p);
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper macro for reporting test results without terminating execution
#define EXPECT_EQ(a, b) do { \
  ++g_total_tests; \
  if ((a) != (b)) { \
    printf("ASSERT FAILED at %s:%d: %s == %lld, but got %lld\n", __FILE__, __LINE__, #a, (long long)(b), (long long)(a)); \
    ++g_failed_tests; \
  } \
} while (0)

// Test 1: Basic positive iRowid value
// Rationale: Validate that sqlite3Fts5ExprRowid returns the iRowid stored in the root for a typical positive value.
static bool test_rowid_basic_positive() {
  // Arrange
  Fts5ExprRoot root;
  root.iRowid = 1234567890123LL;  // typical positive value

  Fts5Expr expr;
  expr.pRoot = &root;

  // Act
  i64 result = sqlite3Fts5ExprRowid(&expr);

  // Assert
  EXPECT_EQ(result, root.iRowid);
  return g_failed_tests == 0;
}

// Test 2: Zero iRowid
// Rationale: Ensure that a zero value is preserved and returned correctly.
static bool test_rowid_zero() {
  // Arrange
  Fts5ExprRoot root;
  root.iRowid = 0LL;

  Fts5Expr expr;
  expr.pRoot = &root;

  // Act
  i64 result = sqlite3Fts5ExprRowid(&expr);

  // Assert
  EXPECT_EQ(result, root.iRowid);
  return g_failed_tests == 0;
}

// Test 3: Negative iRowid
// Rationale: Confirm that negative iRowid values are handled and returned accurately.
static bool test_rowid_negative() {
  // Arrange
  Fts5ExprRoot root;
  root.iRowid = -987654321LL;

  Fts5Expr expr;
  expr.pRoot = &root;

  // Act
  i64 result = sqlite3Fts5ExprRowid(&expr);

  // Assert
  EXPECT_EQ(result, root.iRowid);
  return g_failed_tests == 0;
}

// Test 4: Maximum 64-bit iRowid
// Rationale: Check behavior at the upper bound of i64 to ensure no overflow or UB.
static bool test_rowid_maximum() {
  // Arrange
  Fts5ExprRoot root;
  root.iRowid = LLONG_MAX;  // 9223372036854775807

  Fts5Expr expr;
  expr.pRoot = &root;

  // Act
  i64 result = sqlite3Fts5ExprRowid(&expr);

  // Assert
  EXPECT_EQ(result, root.iRowid);
  return g_failed_tests == 0;
}

// Entry point for running all tests
int main() {
  printf("Starting sqlite3Fts5ExprRowid unit tests (C++11, no GTest)\n");

  // Run tests with explanatory comments above each call
  // Test 1
  {
    // Explanatory: Basic positive iRowid value
    bool ok = test_rowid_basic_positive();
    if (!ok) printf("Test 1 (basic positive iRowid) FAILED.\n");
  }

  // Test 2
  {
    // Explanatory: iRowid == 0
    bool ok = test_rowid_zero();
    if (!ok) printf("Test 2 (iRowid = 0) FAILED.\n");
  }

  // Test 3
  {
    // Explanatory: Negative iRowid
    bool ok = test_rowid_negative();
    if (!ok) printf("Test 3 (negative iRowid) FAILED.\n");
  }

  // Test 4
  {
    // Explanatory: Maximum i64 value
    bool ok = test_rowid_maximum();
    if (!ok) printf("Test 4 (maximum iRowid) FAILED.\n");
  }

  // Summary
  printf("sqlite3Fts5ExprRowid tests completed. Total: %d, Failed: %d\n",
         g_total_tests, g_failed_tests);

  // Return non-zero if any test failed
  return g_failed_tests ? 1 : 0;
}