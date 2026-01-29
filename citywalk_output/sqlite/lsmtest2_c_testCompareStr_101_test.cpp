/*
Step 1 - Program Understanding and Candidate Keywords
Candidate Keywords representing the focal method's core dependencies:
- testCompareStr
- z1, z2 (input strings)
- pRc (pointer to result code)
- strcmp (string comparison)
- testPrintError (error reporting)
- test_failed (test framework hook)
- rc (local result code)
- true/false branches of outer if (*pRc == 0) and inner if (strcmp(z1, z2))
- lsmtest2.c context (unit under test)
- z1, z2 equality/inequality scenarios
- External C linkage for C functions when invoked from C++
- extern "C" usage for testCompareStr, testPrintError, test_failed
Step 2 - Unit Test Generation
We focus on testCompareStr behavior by exercising:
- Case A: *pRc == 0 and z1 != z2 -> rc becomes 1 (inner branch true)
- Case B: *pRc == 0 and z1 == z2 -> rc remains 0 (inner branch false)
- Case C: *pRc != 0 -> rc remains unchanged (outer branch false)
Step 3 - Test Case Refinement
Test suite uses a minimal C++11-friendly harness without GoogleTest. It relies on:
- extern "C" declarations to bind to the C function testCompareStr
- no modification of private/internal state (as per constraints)
- non-terminating assertions (collect and report results)
- simple main() to run all cases and print a summary
The tests are designed to maximize code execution paths and branch coverage for the focal predicate logic.
*/

#include <iostream>
#include <cstring>
#include <lsmtest.h>


// External C interfaces from the focal codebase
extern "C" void testCompareStr(const char *z1, const char *z2, int *pRc);
extern "C" void testPrintError(const char *fmt, ...);
extern "C" void test_failed();

// Simple test harness state (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Lightweight assertion helper (non-terminating)
static void t_assert(bool condition, const char *desc) {
  ++g_total;
  if (!condition) {
    ++g_failed;
    std::cerr << "ASSERT FAILED: " << desc << std::endl;
  }
}

// Test Case 1: z1 and z2 are different, *pRc is initially 0
// Expect: rc becomes 1 (inner branch executed)
static void testCase_UnequalStrings_OuterRcZero() {
  const char *z1 = "alpha";
  const char *z2 = "beta";
  int rc = 0;
  testCompareStr(z1, z2, &rc);
  t_assert(rc == 1, "Case A: unequal strings with *pRc==0 should set rc to 1");
}

// Test Case 2: z1 and z2 are equal, *pRc is initially 0
// Expect: rc remains 0 (inner branch not taken)
static void testCase_EqualStrings_OuterRcZero() {
  const char *z1 = "gamma";
  const char *z2 = "gamma";
  int rc = 0;
  testCompareStr(z1, z2, &rc);
  t_assert(rc == 0, "Case B: equal strings with *pRc==0 should leave rc as 0");
}

// Test Case 3: *pRc is non-zero
// Expect: rc remains unchanged regardless of z1/z2 content
static void testCase_NonZeroOuterRc() {
  const char *z1 = "delta";
  const char *z2 = "epsilon";
  int rc = 5; // non-zero initial value
  testCompareStr(z1, z2, &rc);
  t_assert(rc == 5, "Case C: non-zero *pRc should leave rc unchanged");
}

int main() {
  // Execute all test cases
  testCase_UnequalStrings_OuterRcZero();
  testCase_EqualStrings_OuterRcZero();
  testCase_NonZeroOuterRc();

  // Summary
  std::cout << "Test Summary: total=" << g_total << ", failed=" << g_failed << std::endl;
  // Non-zero exit code indicates at least one failure for quick CI feedback
  return (g_failed != 0) ? 1 : 0;
}