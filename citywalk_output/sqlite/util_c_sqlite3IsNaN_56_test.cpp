// C++ unit test for the focal C function: sqlite3IsNaN
// This test suite exercises NaN vs non-NaN behavior by invoking the
// external C function declared as: extern "C" int sqlite3IsNaN(double x);

#include <math.h>
#include <cmath>
#include <limits>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


extern "C" int sqlite3IsNaN(double x); // Declaration of the focal method (from util.c)

/*
  Domain-friendly lightweight test scaffold:
  - Non-terminating assertions: do not exit on failure; accumulate results.
  - Use numeric_limits to generate NaN, Infinity, etc.
  - Test both NaN and non-NaN inputs to verify correct classification.
*/

static int g_failures = 0;

// Simple non-terminating assertion helper
static void expect(bool cond, const char* desc) {
  if (!cond) {
    std::cerr << "TEST FAILED: " << desc << std::endl;
    ++g_failures;
  }
  // Do nothing on success to keep test output concise
}

// Test 1: NaN input should be detected as NaN (non-zero rc)
static void test_nan_input_is_nan() {
  double nan = std::numeric_limits<double>::quiet_NaN();
  int rc = sqlite3IsNaN(nan);
  expect(rc != 0, "sqlite3IsNaN should return non-zero for NaN input");
}

// Test 2: A normal finite value should not be detected as NaN (rc == 0)
static void test_non_nan_input_not_nan() {
  double v = 123.456;
  int rc = sqlite3IsNaN(v);
  expect(rc == 0, "sqlite3IsNaN should return 0 for non-NaN input");
}

// Test 3: Positive infinity should not be classified as NaN
static void test_infinity_not_nan() {
  double inf = std::numeric_limits<double>::infinity();
  int rc = sqlite3IsNaN(inf);
  expect(rc == 0, "sqlite3IsNaN should return 0 for +Infinity");
}

// Test 4: Negative infinity should not be classified as NaN
static void test_negative_infinity_not_nan() {
  double minf = -std::numeric_limits<double>::infinity();
  int rc = sqlite3IsNaN(minf);
  expect(rc == 0, "sqlite3IsNaN should return 0 for -Infinity");
}

// Test 5: NaN produced by an arithmetic operation (0.0/0.0) is detected as NaN
static void test_nan_via_operation_detected() {
  double n = 0.0 / 0.0; // Produces NaN
  int rc = sqlite3IsNaN(n);
  expect(rc != 0, "sqlite3IsNaN should detect NaN produced by 0.0/0.0");
}

int main() {
  // Run all tests
  test_nan_input_is_nan();
  test_non_nan_input_not_nan();
  test_infinity_not_nan();
  test_negative_infinity_not_nan();
  test_nan_via_operation_detected();

  // Report summary
  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}