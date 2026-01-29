// test_equal_ushort_suite.cpp
// A focused Unity-based test suite for the focal method testEqualUShorts
// and to enhance coverage via an additional independent test.
// This file is written for C++11 compilation, using the Unity C unit-test framework.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Step 1: Program Understanding (embedded as comments)
// - Focus: testEqualUShorts in testunity.c validates equality checks for unsigned short values.
// - Key components observed: unsigned short v0, v1; pointers p0, p1; TEST_ASSERT_EQUAL_UINT checks.
// - Candidate Keywords (core dependents to mirror/test behavior):
//   - v0, v1, p0, p1, *p0, *p1, 19467, 1837, 2987
//   - TEST_ASSERT_EQUAL_UINT macro
//   - unsigned short type and pointer semantics
//   - direct value comparisons vs. pointer dereferenced comparisons

// Step 2: Unit Test Generation (test suite composition)
// - We'll reuse the focal test by invoking testEqualUShorts (extern "C" style to accommodate C linkage from C testfile).
// - We add an additional independent test (testDirectUShortEqualityTwoWays) to increase coverage
//   by reproducing similar semantics with direct variables and pointers in a separate test function.

// Forward declaration: bring the focal test function from testunity.c into this test file.
// Ensure C linkage to avoid name mangling issues when compiled under C++.
#ifdef __cplusplus
extern "C" {
#endif
void testEqualUShorts(void);
#ifdef __cplusplus
}
#endif

// Additional test to improve coverage for unsigned short equality scenarios.
// Declared with C linkage to ensure Unity can invoke it correctly from the C test runner.
extern "C" void testDirectUShortEqualityTwoWays(void);

// Implementation of the additional test
extern "C" void testDirectUShortEqualityTwoWays(void)
{
    // Purpose: Independently exercise the same kind of equality checks as testEqualUShorts,
    // using direct values and pointer dereferencing to verify:
    // - value-to-value equality
    // - pointer dereferenced equality
    unsigned short v0 = 19467;
    unsigned short v1 = 19467;
    unsigned short *p0 = &v0;
    unsigned short *p1 = &v1;

    // Basic sanity checks with literals
    TEST_ASSERT_EQUAL_UINT(1837, 1837);
    TEST_ASSERT_EQUAL_UINT(2987, 2987);

    // Equality via variables
    TEST_ASSERT_EQUAL_UINT(v0, v1);

    // Equality via literal against a variable
    TEST_ASSERT_EQUAL_UINT(19467, v1);
    TEST_ASSERT_EQUAL_UINT(v0, 19467);

    // Pointer-based equality
    TEST_ASSERT_EQUAL_UINT(*p0, v1);
    TEST_ASSERT_EQUAL_UINT(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT(*p0, 19467);
}

// Step 3: Test Case Refinement
// - A minimal main runner that executes the focal test and the additional coverage test.
// - This maintains compatibility with the Unity framework and enables compilation under C++11.

int main(void)
{
    UNITY_BEGIN();
    // Execute the focal test as defined in testunity.c
    RUN_TEST(testEqualUShorts);
    // Execute the additional, self-contained test to broaden coverage of ushort equality scenarios
    RUN_TEST(testDirectUShortEqualityTwoWays);
    return UNITY_END();
}