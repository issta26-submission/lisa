// C++11 test harness for sqlite3_value_text16le
// This test suite mocks the internal helper sqlite3ValueText to verify
// how sqlite3_value_text16le delegates to it, without requiring a full SQLite build.
// It uses plain C-style testing (no GoogleTest) as requested.

#include <cstdio>
#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Forward declare the opaque SQLite value type to keep tests self-contained.
extern "C" {
  typedef struct sqlite3_value sqlite3_value;
  // Declaration of the focal function under test.
  const void *sqlite3_value_text16le(sqlite3_value *pVal);
  // The focal code calls this internal helper:
  // const void *sqlite3ValueText(sqlite3_value *pVal, int enc);
  // We will provide a mock definition for it in this test to control behavior.
  const void *sqlite3ValueText(sqlite3_value *pVal, int enc);
}

// Global encoding constant used by the mock to verify proper encoding propagation.
#ifndef SQLITE_UTF16LE
#define SQLITE_UTF16LE 4  // Commonly used value; adjusted here if needed by environment
#endif

// Sentinels used by the internal mock to differentiate inputs.
// They are actual pointer values cast from integers for test determinism.
static sqlite3_value* VAL1 = (sqlite3_value*) (void*)0x1;
static sqlite3_value* VAL2 = (sqlite3_value*) (void*)0x2;
static sqlite3_value* VAL3 = (sqlite3_value*) (void*)0x3;

// A flag to verify that the encoding argument passed to sqlite3ValueText is exactly SQLITE_UTF16LE.
static bool g_wrong_enc_called = false;

// Mock implementation of sqlite3ValueText to intercept calls from sqlite3_value_text16le.
// This mock is linked with the test binary and will override the real symbol if linked appropriately.
extern "C" const void *sqlite3ValueText(sqlite3_value *pVal, int enc) {
  // Record if an encoding other than SQLITE_UTF16LE is ever observed.
  if (enc != SQLITE_UTF16LE) {
    g_wrong_enc_called = true;
  }
  // Return deterministic pointers depending on the input value to enable precise assertions.
  if (pVal == VAL1) {
    return (const void*)0xABC;
  } else if (pVal == VAL2) {
    return nullptr;
  } else if (pVal == VAL3) {
    return (const void*)0xDEF;
  }
  // Default path for any other input.
  static const char defaultText[] = "default";
  return (const void*)defaultText;
}

// Lightweight test harness (no GTest). Each test returns true on success, false on failure.
static bool test_value_text16le_returns_pointer_for_VAL1() {
  // Reset trajectory-tracking.
  g_wrong_enc_called = false;

  const void *p = sqlite3_value_text16le(VAL1);

  // Expected: the mock returns 0xABC for VAL1, and encoding observed should be correct.
  if (p != (const void*)0xABC) {
    std::cerr << "[FAIL] test_value_text16le_returns_pointer_for_VAL1: unexpected pointer " 
              << p << " (expected 0xABC)" << std::endl;
    return false;
  }
  if (g_wrong_enc_called) {
    std::cerr << "[FAIL] test_value_text16le_returns_pointer_for_VAL1: encoding argument not SQLITE_UTF16LE" 
              << std::endl;
    return false;
  }
  std::cout << "[PASS] test_value_text16le_returns_pointer_for_VAL1" << std::endl;
  return true;
}

static bool test_value_text16le_returns_null_for_VAL2() {
  g_wrong_enc_called = false;

  const void *p = sqlite3_value_text16le(VAL2);

  if (p != nullptr) {
    std::cerr << "[FAIL] test_value_text16le_returns_null_for_VAL2: expected null pointer, got "
              << p << std::endl;
    return false;
  }
  if (g_wrong_enc_called) {
    std::cerr << "[FAIL] test_value_text16le_returns_null_for_VAL2: encoding argument not SQLITE_UTF16LE" 
              << std::endl;
    return false;
  }
  std::cout << "[PASS] test_value_text16le_returns_null_for_VAL2" << std::endl;
  return true;
}

static bool test_value_text16le_returns_pointer_for_VAL3() {
  g_wrong_enc_called = false;

  const void *p = sqlite3_value_text16le(VAL3);

  if (p != (const void*)0xDEF) {
    std::cerr << "[FAIL] test_value_text16le_returns_pointer_for_VAL3: unexpected pointer " 
              << p << " (expected 0xDEF)" << std::endl;
    return false;
  }
  if (g_wrong_enc_called) {
    std::cerr << "[FAIL] test_value_text16le_returns_pointer_for_VAL3: encoding argument not SQLITE_UTF16LE" 
              << std::endl;
    return false;
  }
  std::cout << "[PASS] test_value_text16le_returns_pointer_for_VAL3" << std::endl;
  return true;
}

// Entry point for running tests.
int main() {
  int total = 0;
  int passed = 0;

  // Run the suite of tests
  if (test_value_text16le_returns_pointer_for_VAL1()) ++passed;
  ++total;

  if (test_value_text16le_returns_null_for_VAL2()) ++passed;
  ++total;

  if (test_value_text16le_returns_pointer_for_VAL3()) ++passed;
  ++total;

  // Summary
  std::cout << "Tests passed: " << passed << "/" << total << std::endl;

  // Return non-zero if any test failed (simple CI-friendly exit code)
  return (passed == total) ? 0 : 1;
}

// Notes:
// - This test suite intentionally mocks sqlite3ValueText to exercise sqlite3_value_text16le's
//   dependency on the encoding parameter and its delegation behavior.
// - We rely on the shared symbol sqlite3ValueText being overridable by providing our mock.
// - The tests use only the standard library and sqlite3 headers/types where available.
// - The test demonstrates non-terminating assertions by returning failure status instead of exiting
//   on the first error, allowing multiple tests to run in a single invocation.