/*
Unit Test Suite for sqlite3_libversion_number (C function)
- Step 1: Understand focal method and extract candidate keywords
  Candidate Keywords: sqlite3_libversion_number, SQLITE_VERSION_NUMBER (version macro),
  the concept of version information retrieval from the SQLite core.

- Step 2 & 3: Generate and refine a compact, executable test suite without GTest.
  Design:
  - Lightweight, non-terminating assertions (EXPECT_*) implemented in plain C++11.
  - Tests run from main() in a single translation unit.
  - Use C linkage for the focal function: extern "C" int sqlite3_libversion_number(void);
  - Tests verify basic correctness properties (non-negative, consistent value across calls).
  - Tests are placed inside a dedicated namespace to respect "correct namespace" guidance.

Notes:
- The test assumes the production code (containing sqlite3_libversion_number) is linked into the final binary.
- We avoid any private/static internal details; we only exercise the public-facing function under test.

Compilation environment:
- C++11 compliant compiler
- No GTest or external test frameworks
- Minimal standard library usage
*/

#include <iostream>
#include <sqliteInt.h>


// Declare the focal function with C linkage so we can call it from C++ code.
// This matches the production code's symbol and calling convention.
extern "C" int sqlite3_libversion_number(void);

// Lightweight test harness and test cases implemented in a dedicated namespace.
namespace sqlite_tests {

  // Expose a simple non-terminating assertion mechanism.
  static int g_total_tests = 0;
  static int g_failed_tests = 0;

  // Expectation macros: do not abort on failure; record and report.
  #define EXPECT_TRUE(cond) do { \
      ++g_total_tests; \
      if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_TRUE failed: " #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      } \
  } while(0)

  #define EXPECT_EQ(a, b) do { \
      ++g_total_tests; \
      if(!((a) == (b))) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b \
                  << " ( " << (a) << " != " << (b) << " )" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      } \
  } while(0)

  // Helper: reset counters before running a fresh suite
  inline void reset_counters() {
    g_total_tests = 0;
    g_failed_tests = 0;
  }

  // Test 1: The function should return a non-negative integer.
  // Rationale: Version numbers are non-negative; this covers a basic sanity check.
  void Test_ReturnsNonNegative() {
    int v = sqlite3_libversion_number();
    EXPECT_TRUE(v >= 0);
  }

  // Test 2: The function should return a stable value across successive calls.
  // Rationale: The API should be idempotent for a single call; repeated calls yield the same value.
  void Test_VersionNumberIsStableAcrossCalls() {
    int v1 = sqlite3_libversion_number();
    int v2 = sqlite3_libversion_number();
    EXPECT_EQ(v1, v2);
  }

  // Run all tests and report results.
  inline void RunAll() {
    reset_counters();

    // Unit Test A: Non-negativity
    // Step 3: Covered by Test_ReturnsNonNegative
    Test_ReturnsNonNegative();

    // Unit Test B: Consistency across calls
    // Step 3: Covered by Test_VersionNumberIsStableAcrossCalls
    Test_VersionNumberIsStableAcrossCalls();

    // Summary
    std::cout << "SQLite libversion_number tests: "
              << (g_total_tests - g_failed_tests) << " passed, "
              << g_failed_tests << " failed." << std::endl;
  }

  // Expose exit code for main()
  inline int ExitCode() {
    return g_failed_tests;
  }

} // namespace sqlite_tests

int main() {
  // Run the focused test suite for sqlite3_libversion_number
  sqlite_tests::RunAll();

  // Return non-zero if any test failed
  return sqlite_tests::ExitCode();
}