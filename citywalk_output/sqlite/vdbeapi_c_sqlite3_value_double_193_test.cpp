// C++11 test suite for the focal method: sqlite3_value_double
// Intent: verify sqlite3_value_double delegates to sqlite3VdbeRealValue
// with a Mem-like underlying object. This test suite is self-contained:
// it provides minimal stubs for the required types and functions.

#include <opcodes.h>
#include <cmath>
#include <string>
#include <limits>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Minimal stubs to model the environment of the focal function

// Opaque sqlite3_value type (test harness uses Mem as the underlying storage via cast)
struct sqlite3_value { int dummy; };

// Mem structure used by the real SQLite engine to store numeric values.
// We mimic only what is necessary for sqlite3_value_double to work in tests.
struct Mem {
  double r; // represents the real value stored in a Mem cell
};

// Forward declaration of the real-value retriever used by sqlite3_value_double
// In the real codebase, this is a static or internal function. Here we provide
// a lightweight implementation to enable testing the focal method.
static double sqlite3VdbeRealValue(Mem* pVal) {
  // In the real engine, this would handle various MEM types and conversions.
  // For our test harness, we simply return the stored double.
  return (pVal != nullptr) ? pVal->r : 0.0;
}

// The focal method under test (reproduced inline for a self-contained test file)
double sqlite3_value_double(sqlite3_value *pVal){
  return sqlite3VdbeRealValue((Mem*)pVal);
}

// Helper: compare doubles with tolerance; handle NaN/Inf gracefully.
// Returns true when values are "equal" for test purposes.
static bool double_equal(double a, double b, double tol = 1e-12) {
  if (std::isnan(a) && std::isnan(b)) return true;
  if (!std::isfinite(a) && !std::isfinite(b)) {
    // both infinities: check sign
    return (a == b);
  }
  return std::fabs(a - b) <= tol;
}

// Test 1: Basic positive double value
// Expects basic propagation of a normal double through the wrapper.
static bool test_basic_positive_double() {
  Mem m;
  m.r = 3.5; // arbitrary normal double
  sqlite3_value *p = (sqlite3_value*)&m;
  double out = sqlite3_value_double(p);
  bool ok = double_equal(out, 3.5);
  if (!ok) std::cerr << "[FAIL] test_basic_positive_double: expected 3.5, got " << out << "\n";
  return ok;
}

// Test 2: Basic negative double value
// Ensures negative numbers are preserved through the wrapper.
static bool test_basic_negative_double() {
  Mem m;
  m.r = -123.456;
  sqlite3_value *p = (sqlite3_value*)&m;
  double out = sqlite3_value_double(p);
  bool ok = double_equal(out, -123.456);
  if (!ok) std::cerr << "[FAIL] test_basic_negative_double: expected -123.456, got " << out << "\n";
  return ok;
}

// Test 3: NaN propagation
// NaN should be returned as NaN by the wrapper (we check isnan on the result).
static bool test_nan_double() {
  Mem m;
  m.r = std::numeric_limits<double>::quiet_NaN();
  sqlite3_value *p = (sqlite3_value*)&m;
  double out = sqlite3_value_double(p);
  bool ok = std::isnan(out);
  if (!ok) std::cerr << "[FAIL] test_nan_double: expected NaN, got " << out << "\n";
  return ok;
}

// Test 4: Positive infinity propagation
// Ensures +Inf is preserved through the wrapper.
static bool test_pos_infinity_double() {
  Mem m;
  m.r = std::numeric_limits<double>::infinity();
  sqlite3_value *p = (sqlite3_value*)&m;
  double out = sqlite3_value_double(p);
  bool ok = std::isinf(out) && out > 0;
  if (!ok) std::cerr << "[FAIL] test_pos_infinity_double: expected +Inf, got " << out << "\n";
  return ok;
}

// Test 5: Negative infinity propagation
// Ensures -Inf is preserved through the wrapper.
static bool test_neg_infinity_double() {
  Mem m;
  m.r = -std::numeric_limits<double>::infinity();
  sqlite3_value *p = (sqlite3_value*)&m;
  double out = sqlite3_value_double(p);
  bool ok = std::isinf(out) && out < 0;
  if (!ok) std::cerr << "[FAIL] test_neg_infinity_double: expected -Inf, got " << out << "\n";
  return ok;
}

// Simple test runner
int main() {
  int total = 5;
  int passed = 0;

  // Run tests with explanatory comments above each case.
  // Each test returns true on success; failures are reported to stderr.
  if (test_basic_positive_double()) {
    ++passed;
  }

  if (test_basic_negative_double()) {
    ++passed;
  }

  if (test_nan_double()) {
    ++passed;
  }

  if (test_pos_infinity_double()) {
    ++passed;
  }

  if (test_neg_infinity_double()) {
    ++passed;
  }

  // Summary
  if (passed == total) {
    std::cout << "[SUMMARY] ALL TESTS PASSED (" << passed << "/" << total << ")\n";
  } else {
    std::cout << "[SUMMARY] SOME TESTS FAILED (" << passed << "/" << total << ")\n";
  }

  return (passed == total) ? 0 : 1;
}