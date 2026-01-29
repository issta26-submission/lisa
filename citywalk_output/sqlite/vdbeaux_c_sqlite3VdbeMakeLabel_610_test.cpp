/*
Step 1 & 2: Program Understanding and Test Plan Sketch embedded as comments
- Focal method: int sqlite3VdbeMakeLabel(Parse *pParse) { return --pParse->nLabel; }
- Core behavior: Decrements the nLabel field inside the provided Parse object and returns the new value.
- Key dependent component (Candidate Keywords): Parse, nLabel, sqlite3VdbeMakeLabel, decrement, pointer, return value.
- Testing goal: Verify that:
  1) The function decrements nLabel by exactly 1.
  2) The returned value equals the updated nLabel.
  3) Multiple consecutive calls update state cumulatively as expected.
- Class/Dependencies note (as per guidance): We mock a minimal Parse struct containing nLabel to drive sqlite3VdbeMakeLabel. This mirrors the dependent field used by the focal method.
- Domain knowledge considerations: No private members or private access; tests are standalone and use C++11 features only. We avoid GTest and rely on a tiny, non-terminating assertion style. Tests are invoked from main.

Note: This test harness assumes that the symbol sqlite3VdbeMakeLabel(Parse*) is available at link time (from the library under test) and that the Parse object used by the library places an int nLabel at the offset expected by the focal implementation. The test defines a local Parse struct with an int nLabel member to drive the function call. If the real Parse layout differs, the test may need adjustment to match the actual layout.

The test suite is intentionally small but structured to be easily extended for higher coverage if needed.
*/

#include <cstdio>
#include <cinttypes>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <limits>


// Step 2: Declare a minimal Parse type that matches the focal function's usage in tests.
// We define only the member that sqlite3VdbeMakeLabel touches to drive behavior.
struct Parse {
  int nLabel;
};

// Step 2: Forward declare the focal function from the library under test.
// The actual implementation resides in vdbeaux.c within the project under test.
extern "C" int sqlite3VdbeMakeLabel(Parse *pParse);

// Helper: Lightweight, non-terminating test assertion utilities.
// The tests should continue executing after a failed condition, and report failures.
static bool g_anyFailure = false;

#define EXPECT_EQ_INT(actual, expected, desc) do { \
  if ((actual) != (expected)) { \
    printf("  [FAIL] %s: expected %d, got %d\n", (desc), (expected), (actual)); \
    g_anyFailure = true; \
  } \
} while (0)

#define EXPECT_TRUE(cond, desc) do { \
  if (!(cond)) { \
    printf("  [FAIL] %s: condition is false\n", (desc)); \
    g_anyFailure = true; \
  } \
} while (0)

/*
Test 1: Basic decrement behavior
- Setup: nLabel starts at a positive value (e.g., 5).
- Action: Call sqlite3VdbeMakeLabel once.
- Expectation: Returned value is 4 and pParse->nLabel becomes 4.
*/
static bool test_makeLabel_basic() {
  Parse p{5}; // initial state
  int ret = sqlite3VdbeMakeLabel(&p);
  bool ok = true;
  EXPECT_EQ_INT(ret, 4, "return value after first decrement (5 -> 4)");
  EXPECT_EQ_INT(p.nLabel, 4, "Parse.nLabel after first decrement (5 -> 4)");
  return ok && !g_anyFailure;
}

/*
Test 2: Decrement from zero
- Setup: nLabel starts at 0.
- Action: Call sqlite3VdbeMakeLabel once.
- Expectation: Returned value is -1 and nLabel becomes -1 (C-style signed wrap behavior on decrement).
*/
static bool test_makeLabel_zero() {
  Parse p{0};
  int ret = sqlite3VdbeMakeLabel(&p);
  bool ok = true;
  EXPECT_EQ_INT(ret, -1, "return value when starting from 0 (0 -> -1)");
  EXPECT_EQ_INT(p.nLabel, -1, "Parse.nLabel after decrement from 0 to -1");
  return ok && !g_anyFailure;
}

/*
Test 3: Multiple consecutive decrements
- Setup: nLabel starts at 2.
- Action: Call sqlite3VdbeMakeLabel twice.
- Expectations:
  - After first call: ret == 1, nLabel == 1
  - After second call: ret == 0, nLabel == 0
*/
static bool test_makeLabel_multiple() {
  Parse p{2};
  int r1 = sqlite3VdbeMakeLabel(&p);
  int r2 = sqlite3VdbeMakeLabel(&p);
  bool ok = true;
  EXPECT_EQ_INT(r1, 1, "first decrement from 2 should yield 1");
  EXPECT_EQ_INT(p.nLabel, 1, "Parse.nLabel after first decrement should be 1");
  EXPECT_EQ_INT(r2, 0, "second decrement from 1 should yield 0");
  EXPECT_EQ_INT(p.nLabel, 0, "Parse.nLabel after second decrement should be 0");
  return ok && !g_anyFailure;
}

// Entry point: Run all tests and print a concise summary.
// Tests are invoked from main, following the no-GTest constraint.
int main() {
  printf("Starting unit tests for sqlite3VdbeMakeLabel(Parse *pParse)\n");

  // Reset global failure state for clean per-test reporting
  auto resetFailures = []() { g_anyFailure = false; };

  int totalTests = 0;
  int failedTests = 0;

  // Test 1
  resetFailures();
  totalTests++;
  printf("Test 1: test_makeLabel_basic\n");
  bool t1 = test_makeLabel_basic();
  if (t1) printf("  Result: PASSED\n"); else { printf("  Result: FAILED\n"); failedTests++; }
  // Note: test_makeLabel_basic uses its own local assertions via EXPECT_EQ_INT.

  // Test 2
  resetFailures();
  totalTests++;
  printf("Test 2: test_makeLabel_zero\n");
  bool t2 = test_makeLabel_zero();
  if (t2) printf("  Result: PASSED\n"); else { printf("  Result: FAILED\n"); failedTests++; }

  // Test 3
  resetFailures();
  totalTests++;
  printf("Test 3: test_makeLabel_multiple\n");
  bool t3 = test_makeLabel_multiple();
  if (t3) printf("  Result: PASSED\n"); else { printf("  Result: FAILED\n"); failedTests++; }

  // Summary
  int passedTests = totalTests - failedTests;
  printf("Summary: %d passed, %d failed (out of %d tests)\n", passedTests, failedTests, totalTests);

  // Return non-zero if any test failed to signal abnormal termination in scripts
  return (failedTests > 0) ? 1 : 0;
}