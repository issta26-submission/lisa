// test_equal_int_arrays_suite.cpp
// A focused C++11 compatible test suite for the focal method testEqualIntArrays
// leveraging the existing Unity C unit-testing framework without GTest.
// This suite imports the focal test by declaration and also adds complementary
// tests to improve coverage of the TEST_ASSERT_EQUAL_INT_ARRAY macro.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test (part of the Unity-based test suite in testunity.c)
void testEqualIntArrays(void);
#ifdef __cplusplus
}
#endif

// Additional test helpers and scenarios to increase coverage of array-equality checks.

void setUp(void) {
    // Optional per-test setup (left empty intentionally)
}

void tearDown(void) {
    // Optional per-test teardown (left empty intentionally)
}

// 1) Directly exercise the macro with two identical arrays of the same length.
// This mirrors the happy-path contained within the focal test but in a separate test
// to ensure the macro behaves correctly when used independently.
void testDirectEqualityUsingMacro(void) {
    // Two identical arrays; should pass for length 4
    int a[] = {7, 14, 21, -3};
    int b[] = {7, 14, 21, -3};
    // True branch: identical content
    TEST_ASSERT_EQUAL_INT_ARRAY(a, b, 4);
}

// 2) Null pointer pair with non-zero length should be treated as a valid comparison
// if both are NULL on the tested length (as seen in the focal test).
void testDirectEqualityWithNulls(void) {
    // Both pointers are NULL; length 1 (as in focal test)
    TEST_ASSERT_EQUAL_INT_ARRAY(NULL, NULL, 1);
}

// 3) Negative path: arrays differ within the compared length should fail gracefully.
// Use Unity's expected abort mechanism to verify that the macro detects the mismatch
// without terminating the whole test runner.
void testDirectInequalityWithDifferentElements(void) {
    int a[] = {1, 2, 3, 4};
    int b[] = {1, 2, 3, 5}; // difference at the last element
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT_ARRAY(a, b, 4);
    VERIFY_FAILS_END
}

// 4) Length-based equality: ensure that only the first N elements are compared.
// If length is smaller than actual array sizes but contents match in the prefix, it should pass.
void testDirectEqualityWithSmallerLength(void) {
    int a[] = {1, 2, 3, 4};
    int b[] = {1, 2, 3, 999}; // last element differs, but we only compare first 3
    TEST_ASSERT_EQUAL_INT_ARRAY(a, b, 3);
}

// 5) Zero-length comparison should be a no-op and pass regardless of array contents.
// This mirrors the behavior observed in the focal test where length is zero in some cases.
void testDirectLengthZeroEquality(void) {
    int a[] = {1, 2, 3};
    int b[] = {9, 9, 9};
    TEST_ASSERT_EQUAL_INT_ARRAY(a, b, 0);
}

// 6) Coverage: ensure the focal test can be run within this suite.
// This validates that the integration point (calling the focal test) is functional.
void testRunFocalTestDirectly(void) {
    // This invokes the focal test as-is; its internal assertions will be checked.
    // If the focal test ever fails, Unity will report accordingly.
    testEqualIntArrays();
}

// Main function: register all tests with the Unity harness.
// We explicitly run the focal test (testEqualIntArrays) along with our own
// supplementary tests to maximize coverage of the array-equality pathway.
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testRunFocalTestDirectly);        // Execute the focal method under test
    RUN_TEST(testDirectEqualityUsingMacro);    // Additional direct macro usage
    RUN_TEST(testDirectEqualityWithNulls);      // Null pointer handling
    RUN_TEST(testDirectInequalityWithDifferentElements); // Detects mismatch inside length
    RUN_TEST(testDirectEqualityWithSmallerLength); // Prefix-based equality path
    RUN_TEST(testDirectLengthZeroEquality);      // Zero-length path
    return UNITY_END();
}