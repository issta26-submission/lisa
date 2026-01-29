// This is a lightweight C++11 test suite for the focal function:
// u64 sqlite3LogEstToInt(LogEst x) implemented below.
// The tests exercise various branches of the function to ensure correctness.
// They do not rely on any external testing framework (GTest, etc.).
// Explanatory comments describe the intent of each test case.

#include <math.h>
#include <vector>
#include <string>
#include <limits>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Step 1: Minimal, self-contained type aliases to mirror the focal code's types.
using u64 = uint64_t;
using LogEst = uint64_t;

// Step 2: Provide the focal function under test.
// This mirrors the logic shown in <FOCAL_METHOD>.
static const u64 LARGEST_INT64 = static_cast<u64>(std::numeric_limits<int64_t>::max());

u64 sqlite3LogEstToInt(LogEst x) {
  u64 n;
  n = x % 10;
  x /= 10;
  if (n >= 5) n -= 2;
  else if (n >= 1) n -= 1;
  if (x > 60) return (u64)LARGEST_INT64;
  return x >= 3 ? (n + 8) << (x - 3) : (n + 8) >> (3 - x);
}

// Step 3: A tiny, self-contained test harness with non-terminating assertions.
// We implement a minimal assert-like mechanism that reports failures but continues.
static int g_total = 0;
static int g_failed = 0;

template <typename T, typename U>
static void report_assertion(const T& actual, const U& expected, const std::string& msg) {
  ++g_total;
  if (!(actual == expected)) {
    ++g_failed;
    std::cerr << "[ASSERT FAIL] " << msg
              << " | got: " << actual << ", expected: " << expected << "\n";
  } else {
    // Optional: uncomment to see passing tests
    // std::cout << "[ASSERT PASS] " << msg << "\n";
  }
}

static void runTestSet() {
  // Helper to run a single test with a descriptive message.
  auto run = [](const std::string& name, LogEst input, u64 expected) {
    u64 got = sqlite3LogEstToInt(input);
    report_assertion(got, expected, name);
  };

  // Step 3, refined tests that cover true/false branches and boundary conditions.

  // Test 1: input = 0
  // Rationale: x%10 = 0 -> n = 0; x = 0; no branch triggers;
  // result should be (0+8)>>(3-0) = 8 >> 3 = 1
  run("case_zero_input", 0ULL, 1ULL);

  // Test 2: input where n>=1 but <5 (n decreases by 1)
  // Example: input = 11 -> n=1 -> n becomes 0; x=1; x>60? no; x>=3? no;
  // result = (0+8)>>(3-1) = 8>>2 = 2
  run("n_ge_1_case", 11ULL, 2ULL);

  // Test 3: input where n>=5 (n decreases by 2)
  // Example: input = 15 -> n=5 -> n becomes 3; x=1; result = (3+8)>>(3-1) = 11>>2 = 2
  run("n_ge_5_case", 15ULL, 2ULL);

  // Test 4: boundary with small x but x>=3 after division
  // Example: input = 12 -> n=2 -> n becomes 1; x=1; result = (1+8)>>(3-1) = 9>>2 = 2
  run("case_x_after_div_has_three_or_more", 12ULL, 2ULL);

  // Test 5: boundary producing a left shift
  // Example: input = 60 -> n=0 -> x=6; result = (0+8)<<(6-3) = 8<<3 = 64
  run("case_left_shift_by_x_minus_3", 60ULL, 64ULL);

  // Test 6: another left-shift boundary with x=61 (after division x=6)
  // n becomes 0; result should also be 8<<3 = 64
  run("case_left_shift_with_x_61", 61ULL, 64ULL);

  // Test 7: large x that triggers a large left shift
  // Example: input = 100 -> n=0; x=10; result = (0+8)<<(10-3) = 8<<7 = 1024
  run("case_large_left_shift", 100ULL, 1024ULL);

  // Test 8: trigger the overflow sentinel by modifying x so that x after division > 60
  // Example: input = 610 -> n=0; x=61; x > 60 -> LARGEST_INT64
  run("case_overflow_sentinel", 610ULL, LARGEST_INT64);
}

int main() {
  // Explain what we are testing at the top of the run.
  std::cout << "Running tests for sqlite3LogEstToInt (C-style logic reimplemented in C++11).\n";

  // Execute the test set.
  runTestSet();

  // Summary (non-terminating; continues after failures)
  std::cout << "\nTest summary:\n";
  std::cout << "Total assertions attempted: " << g_total << "\n";
  std::cout << "Failures: " << g_failed << "\n";

  if (g_failed == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << "Some tests failed. See details above.\n";
  }

  // Return non-zero if any test failed to signal error in CI scripts, while still printing results.
  return g_failed ? 1 : 0;
}