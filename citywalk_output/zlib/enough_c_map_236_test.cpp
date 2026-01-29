/*
Step 1 - Program Understanding and Candidate Keywords
- Core function under test: map(int syms, int left, int len)
- Dependency: a global-ish context object g with a member max (i.e., g.max)
- Arithmetic formula being exercised:
  result = (((syms-1)>>1) * ((syms-2)>>1) + (left>>1) - 1) * (g.max - 1) + (len - 1)
- Candidate keywords representing dependencies and operators:
  - syms, left, len (inputs)
  - g.max (static/global dependency affecting the scale)
  - bit shifts (>>1), integer arithmetic, size_t / unsigned arithmetic
  - multiplication by (g.max - 1)
  - additive terms and final + (len - 1)
- The test suite will validate the numeric correctness of map for several representative inputs and different g.max values.

Step 2 - Unit Test Generation
- Provide a minimal C++11-compatible environment that mirrors the focal dependencies
- Implement a global-ish context with a member max to feed into map
- Create explicit test cases with manually computed expected results
- Use a lightweight, non-terminating assertion mechanism (collects failures and reports at the end)

Step 3 - Test Case Refinement
- Coverage includes:
  - Different g.max values (1, 2, 5, 3, 10)
  - Boundary-ish input values for syms (2, 4, 5, 6)
  - Non-negative left and len values to avoid undefined shifts
- Each test case includes explanatory comments

Note: The provided focal method is reproduced here in a test-friendly form to allow independent compilation under C++11 without GTest or external frameworks.

The code below provides the test harness and test cases.

*/

// Minimal, self-contained test harness and reproduction of the focal map function.

#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <cstddef>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>


// Dependency reproduction: a global-like context used by map
struct GContext {
    int max;
};

// Static context instance to mirror file-scoped/global usage
static GContext g;

// Focal method reproduced for testing (inline in original, but here we provide as a function)
inline size_t map(int syms, int left, int len) {
    return ((size_t)((syms - 1) >> 1) * ((syms - 2) >> 1) +
            (left >> 1) - 1) * (g.max - 1) +
           len - 1;
}

// Lightweight test harness
static int failures = 0;
static void report_failure(const char* context, size_t observed, size_t expected) {
    std::cerr << "Failure in " << context
              << ": expected " << expected
              << ", got " << observed << std::endl;
    ++failures;
}

// Helper to perform an assertion without aborting the test run
static void expect_eq(size_t observed, size_t expected, const char* context) {
    if (observed != expected) {
        report_failure(context, observed, expected);
    }
}

// Test Case 1
// Description: Basic scenario where g.max == 2 to validate the base scale factor (g.max - 1) == 1
// Inputs: syms=2, left=2, len=1
// Expected (manual arithmetic):
//   a = ((2-1) >> 1) = 0
//   b = ((2-2) >> 1) = 0
//   ab = 0
//   d = (2 >> 1) - 1 = 0
//   e = 0 + 0 = 0
//   f = 0 * (2-1) = 0
//   result = 0 + (1-1) = 0
static void test_case_basic_g2_syms2_left2_len1() {
    g.max = 2;
    size_t res = map(2, 2, 1);
    size_t expected = 0;
    expect_eq(res, expected, "test_case_basic_g2_syms2_left2_len1");
}

// Test Case 2
// Description: Mixed inputs with g.max=5 to exercise non-zero scale factor
// Inputs: syms=4, left=10, len=3
// Manual arithmetic:
//   a = ((4-1)>>1) = 3>>1 = 1
//   b = ((4-2)>>1) = 2>>1 = 1
//   ab = 1
//   d = (10>>1) - 1 = 5 - 1 = 4
//   e = 1 + 4 = 5
//   f = 5 * (5-1) = 20
//   result = 20 + (3-1) = 22
static void test_case_mixed_g5_syms4_left10_len3() {
    g.max = 5;
    size_t res = map(4, 10, 3);
    size_t expected = 22;
    expect_eq(res, expected, "test_case_mixed_g5_syms4_left10_len3");
}

// Test Case 3
// Description: Edge case where g.max == 1 (scale factor becomes zero, result should be len-1)
// Inputs: syms=3, left=8, len=2
// Manual arithmetic:
//   a = ((3-1)>>1) = 2>>1 = 1
//   b = ((3-2)>>1) = 1>>1 = 0
//   ab = 0
//   d = (8>>1) - 1 = 4 - 1 = 3
//   e = 0 + 3 = 3
//   f = 3 * (1-1) = 0
//   result = 0 + (2-1) = 1
static void test_case_g1_syms3_left8_len2() {
    g.max = 1;
    size_t res = map(3, 8, 2);
    size_t expected = 1;
    expect_eq(res, expected, "test_case_g1_syms3_left8_len2");
}

// Test Case 4
// Description: Another mixed input with g.max=3 to check mid-range scaling
// Inputs: syms=5, left=7, len=4
// Manual arithmetic:
//   a = ((5-1)>>1) = 4>>1 = 2
//   b = ((5-2)>>1) = 3>>1 = 1
//   ab = 2
//   d = (7>>1) - 1 = 3 - 1 = 2
//   e = 2 + 2 = 4
//   f = 4 * (3-1) = 8
//   result = 8 + (4-1) = 11
static void test_case_g3_syms5_left7_len4() {
    g.max = 3;
    size_t res = map(5, 7, 4);
    size_t expected = 11;
    expect_eq(res, expected, "test_case_g3_syms5_left7_len4");
}

// Test Case 5
// Description: Largeish inputs to exercise stability with bigger max value
// Inputs: syms=6, left=3, len=6
// Manual arithmetic:
//   a = ((6-1)>>1) = 5>>1 = 2
//   b = ((6-2)>>1) = 4>>1 = 2
//   ab = 4
//   d = (3>>1) - 1 = 1 - 1 = 0
//   e = 4 + 0 = 4
//   f = 4 * (10-1) = 36
//   result = 36 + (6-1) = 41
static void test_case_large_max_syms6_left3_len6() {
    g.max = 10;
    size_t res = map(6, 3, 6);
    size_t expected = 41;
    expect_eq(res, expected, "test_case_large_max_syms6_left3_len6");
}

// Main entry: run all tests and report summary
int main() {
    // Run all test cases
    test_case_basic_g2_syms2_left2_len1();
    test_case_mixed_g5_syms4_left10_len3();     // Note: corrected name in call to match function defined above
    // The above line uses a function name that doesn't match; to keep consistency, call the proper functions:
    // We will directly invoke the correctly named functions instead.
    test_case_mixed_g5_syms4_left10_len3();       // ensure function exists
    test_case_g1_syms3_left8_len2();
    test_case_g3_syms5_left7_len4();
    test_case_large_max_syms6_left3_len6();

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed.\n";
        return failures;
    }
}

// Note on test case wiring:
// The test_case_mixed_g5_syms4_left10_len3 function name is used to represent Test Case 2.
// We added an additional call in main to ensure both Test Case 2 and its alias are executed.
// This file is self-contained and does not rely on external testing frameworks.