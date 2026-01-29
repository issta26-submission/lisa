// Comprehensive test suite for the focal behavior of TEST_ASSERT_EACH_EQUAL_INT
// focusing on the specific scenario of testNotEqualIntEachEqual1.
// This test suite is written in C++11-compatible style, but uses the Unity C unit testing framework.
// It exercises various edge cases around the NOT-EQUAL condition for an array of ints.

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

// Prototypes for the test variants to be invoked by Unity's test runner
void testNotEqualIntEachEqual1_VariantFirstMismatch(void);
void testNotEqualIntEachEqual1_VariantMiddleMismatch(void);
void testNotEqualIntEachEqual1_VariantLastMismatch(void);
void testNotEqualIntEachEqual1_NullActual(void);
void testNotEqualIntEachEqual1_LengthZero(void);

// ---------------------------------------------------------------------------------
// Test Variants
// Each test uses the focal macro TEST_ASSERT_EACH_EQUAL_INT to verify that all
// elements in an array are equal to an expected value. The test intentionally
// expects a failure (abort) to validate the NOT-EQUAL path is correctly detected.
// Comments explain what scenario is being exercised.
// ---------------------------------------------------------------------------------

// Variant 1: Mismatch at the first element
// Expected: Abort, since p0[0] != 1
void testNotEqualIntEachEqual1_VariantFirstMismatch(void)
{
    // Array begins with a value different from the expected (0 instead of 1)
    int p0[] = {0, 1, 1, 1};
    // This should abort because not all elements equal to 1
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 4);
    VERIFY_FAILS_END
}

// Variant 2: Mismatch in the middle element
// Expected: Abort, since p0[2] != 1
void testNotEqualIntEachEqual1_VariantMiddleMismatch(void)
{
    int p0[] = {1, 1, 2, 1};
    // Abort expected due to a mismatch at index 2
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 4);
    VERIFY_FAILS_END
}

// Variant 3: Mismatch at the last element
// Expected: Abort, since p0[3] != 1
void testNotEqualIntEachEqual1_VariantLastMismatch(void)
{
    int p0[] = {1, 1, 1, 2};
    // Abort expected due to a mismatch at index 3
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 4);
    VERIFY_FAILS_END
}

// Variant 4: Null actual pointer with non-zero length
// Existing tests cover this pattern, but we add another variant to strengthen coverage
void testNotEqualIntEachEqual1_NullActual(void)
{
    int* p0 = NULL;
    // Abort expected because the actual pointer is NULL but length is non-zero
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(1, p0, 4);
    VERIFY_FAILS_END
}

// Variant 5: Length zero with a non-trivial expected value
// This ensures the function behaves consistently when length is zero.
void testNotEqualIntEachEqual1_LengthZero(void)
{
    int p0[] = {1, 2, 3, 4};
    // Abort expected when length is zero, even if the array has contents
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(0, p0, 0);
    VERIFY_FAILS_END
}

// ---------------------------------------------------------------------------------
// Test harness with a minimal main to run the test suite
// ---------------------------------------------------------------------------------

int main(void)
{
    UnityBegin("test_not_equal_int_each_equal_suite.cpp");

    // Register tests
    RUN_TEST(testNotEqualIntEachEqual1_VariantFirstMismatch, __LINE__);
    RUN_TEST(testNotEqualIntEachEqual1_VariantMiddleMismatch, __LINE__);
    RUN_TEST(testNotEqualIntEachEqual1_VariantLastMismatch, __LINE__);
    RUN_TEST(testNotEqualIntEachEqual1_NullActual, __LINE__);
    RUN_TEST(testNotEqualIntEachEqual1_LengthZero, __LINE__);

    return UnityEnd();
}