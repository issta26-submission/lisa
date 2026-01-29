/*
Step 1: Program Understanding
- Focused function: sqlite3Int64ToText(i64 v, char *zOut)
- Purpose: Convert a 64-bit signed integer (i64) to its decimal string representation in zOut.
- Key behaviors:
  - Handles negative numbers, including the smallest int64 value.
  - Uses a local buffer zTemp[22], writes digits backwards, optionally a '-' for negatives.
  - Returns the length of the produced string (excluding the null terminator) and ensures zOut is null-terminated.
  - Works without relying on any C++ STL; the test will verify both content and length.
- Dependencies: Requires i64 type and a C implementation of sqlite3Int64ToText. We assume i64 = long long and link against the C implementation.

Step 2: Unit Test Generation
- Candidate Keywords (core components to cover and validate):
  - v == 0
  - v > 0 (various digit lengths)
  - v < 0 (negative values)
  - v == SMALLEST_INT64 (minimum 64-bit value)
  - v == MAX_int64 (maximum 64-bit value)
  - Boundary conditions and length correctness
  - Null-termination behavior (zOut[len] == '\\0')
  - Consistency with standard formatting via snprintf/("%lld") for expected string
- Class Dependencies: The focal class file lists many helper functions; for our test we only directly rely on the sqlite3Int64ToText signature. We declare a C prototype to call into the implementation. We do not mock internal static helpers; we test end-to-end behavior.

Step 3: Test Case Refinement
- Domain knowledge: Use 64-bit edge cases (min, max), typical positive values, zero, negative values. Validate both content and returned length. Use non-terminating assertions (do not exit on first failure) and accumulate results. Ensure cross-language linkage by using extern "C" for the function.

Note: This test suite is written in C++11, without using GTest. It uses a tiny in-house assertion style to accumulate failures and prints diagnostics to standard error. It assumes the environment provides sqlite3Int64ToText with C linkage.

Now the test code:

*/
#include <math.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <limits>
#include <iostream>


// Domain-specific: define i64/u64 aliases solid enough for cross-translation-unit use.
// We assume 64-bit long long on the target platform; this matches typical sqlite3_int64 typedefs.
typedef long long i64;
typedef unsigned long long u64;

// Declare the focal C function with C linkage.
// The actual implementation is in util.c (as part of the project under test).
extern "C" int sqlite3Int64ToText(i64 v, char *zOut);

// Lightweight test harness (non-terminating assertions)
static int total_tests = 0;
static int failed_tests = 0;

// Helper: convert a C++ long long to a std::string using "%lld" formatting.
// This serves as the expected value for comparison.
static std::string to_string_expected(i64 v) {
  char buf[64];
  std::snprintf(buf, sizeof(buf), "%lld", (long long)v);
  return std::string(buf);
}

// Helper: run a single test case and report result
static void run_case(const char* name, i64 v) {
  char zOut[64];
  int len = sqlite3Int64ToText(v, zOut);
  std::string got(zOut, len);
  std::string exp = to_string_expected(v);

  bool ok = (got == exp) && (len == (int)exp.size()) && (zOut[len] == '\0');
  if(!ok) {
    std::cerr << "Test FAILED: " << name
              << " | v=" << v
              << " | got='" << got << "'"
              << " | exp='" << exp << "'"
              << " | len=" << len
              << " | zOut[len]='" << (zOut[len] == '\0' ? "\\0" : zOut[len]) << "'\n";
    ++failed_tests;
  } else {
    std::cout << "Test PASSED: " << name << "\n";
  }
  ++total_tests;
}

int main() {
  // Step 2 / Step 3: Generate a comprehensive set of test cases for sqlite3Int64ToText.

  // Basic case: zero
  // Purpose: Ensure "0" is produced and length is 1, with proper termination.
  run_case("zero", 0);

  // Small positive number
  // Purpose: Validate standard positive conversion without sign.
  run_case("positive_small", 123456789);

  // Small negative number
  // Purpose: Validate negative sign handling for non-min value.
  run_case("negative_small", -123456789);

  // Large positive value (multi-digit, still within 64-bit)
  // Purpose: Validate multiple digits handling.
  run_case("positive_large", 9223372036854775807LL); // LLONG_MAX

  // Boundary: minimum 64-bit value
  // Purpose: Ensure correct representation of the most negative number (-2^63)
  i64 minv = std::numeric_limits<long long>::min();
  run_case("min_int64", minv);

  // Boundary: a large negative number (not min) to exercise the minus path
  run_case("negative_large", -987654321012345LL);

  // Extra large 18-digit positive
  run_case("positive_1e18", 1000000000000000000LL);

  // Random large positive
  run_case("positive_random", 123456789012345LL);

  // Final summary
  std::cerr << "Total tests: " << total_tests << ", Failures: " << failed_tests << "\n";

  // Return non-zero if any test failed to signal an overall test run failure.
  return (failed_tests == 0) ? 0 : 1;
}