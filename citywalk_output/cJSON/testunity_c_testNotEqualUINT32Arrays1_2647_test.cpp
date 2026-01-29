// This file provides a focused C++11 compatible test suite for the focal Unity-based
// test method testNotEqualUINT32Arrays1 (located in testunity.c as provided).
// Note: We are using the Unity C test framework in a C++11 compatible file.
// The suite exercises variations of not-equal UINT32 arrays to validate the expected
// failure behavior when TEST_ASSERT_EQUAL_UINT32_ARRAY is invoked.
//
// Key dependencies and keywords (mapped from the focal method analysis):
// - UNITY_UINT32: 32-bit unsigned integer type used in the arrays
// - TEST_ASSERT_EQUAL_UINT32_ARRAY: assertion under test that should fail for unequal arrays
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: mechanism to expect a test abort (failure) without
//   stopping the whole test suite
// - p0, p1: test arrays of type UNITY_UINT32
// - 4: the length used in the focal method (array length for the test)
// These keywords are the core components the tests rely on to exercise the focal method behavior.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Length used by the focal test: 4 elements per array
static const unsigned int NOT_EQUAL_LEN = 4;

/*
 * Test variant 1: Original focal test scenario
 * Two UINT32 arrays differ in the last element. This should trigger a failure
 * when TEST_ASSERT_EQUAL_UINT32_ARRAY is evaluated.
 */
void testNotEqualUINT32Arrays1_VariantOriginal(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65131u};

    // Expect the test to abort due to inequality, and verify that the failure is handled
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, NOT_EQUAL_LEN);
    VERIFY_FAILS_END
}

/*
 * Test variant 2: Mismatch at index 2 (third element)
 * Change occurs earlier than the last element to ensure the assertion detects
 * non-equality at a different position.
 */
void testNotEqualUINT32Arrays1_VariantMismatchIndex2(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 986, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, NOT_EQUAL_LEN);
    VERIFY_FAILS_END
}

/*
 * Test variant 3: Mismatch at index 1 (second element)
 * Verifies the assertion detects inequality at an earlier position in the array.
 */
void testNotEqualUINT32Arrays1_VariantMismatchIndex1(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 7, 987, 65132u};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, NOT_EQUAL_LEN);
    VERIFY_FAILS_END
}

/*
 * Test variant 4: All elements differ
 * Ensures the macro handles multiple inequalities within the same array pair.
 */
void testNotEqualUINT32Arrays1_VariantAllDifferent(void)
{
    UNITY_UINT32 p0[] = {9, 10, 11, 12};
    UNITY_UINT32 p1[] = {0xFFFFFFFFu, 0, 0, 0};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, NOT_EQUAL_LEN);
    VERIFY_FAILS_END
}

// Minimal Unity-based test runner (acts as the main) to execute the tests without GTest.
int main(void)
{
    UNITY_BEGIN();

    // Register and run all test variants for the focal method
    RUN_TEST(testNotEqualUINT32Arrays1_VariantOriginal);
    RUN_TEST(testNotEqualUINT32Arrays1_VariantMismatchIndex2);
    RUN_TEST(testNotEqualUINT32Arrays1_VariantMismatchIndex1);
    RUN_TEST(testNotEqualUINT32Arrays1_VariantAllDifferent);

    return UNITY_END();
}