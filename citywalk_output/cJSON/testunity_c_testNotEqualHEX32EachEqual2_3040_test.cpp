/*
 * Expanded test suite for the focal testNotEqualHEX32EachEqual2 (Unity style C tests)
 *
 * Context:
 * - The focal method uses Unity's TEST_ASSERT_EACH_EQUAL_HEX32 to verify that all
 *   elements in an array are equal to a given HEX32 value.
 * - The existing testNotEqualHEX32EachEqual2 in testunity.c asserts that a mismatch
 *   should cause an abort (i.e., the test should fail).
 *
 * Objective:
 * - Extend coverage for the same focal concept by providing additional test variants
 *   that exercise different input configurations and preserve the non-GTest (Unity)
 *   testing approach suitable for C/C++11 projects.
 * - Keep tests aligned with the existing Unity-based style and macros.
 *
 * Important notes for integration:
 * - These tests rely on the Unity framework macros used in testunity.c, including
 *   EXPECT_ABORT_BEGIN and VERIFY_FAILS_END, and should be linked with the same test
 *   harness (i.e., compiled together with testunity.c or a compatible Unity setup).
 * - The tests below are designed to be added to the existing suite, not to replace
 *   the original focal test.
 * - They follow the pattern of the focal test style and include explanatory comments.
 *
 * Usage:
 * - Compile with Unity (as in the existing testunity.c environment).
 * - The tests can be picked up by the existing test runner (RUN_TEST) or invoked
 *   directly via Unity's test runner, depending on the project setup.
 *
 * Note on static/global state:
 * - These tests do not rely on static state across tests; each test initializes its
 *   own local data.
 *
 * This file is compatible with C/C++11 compilers and does not require GTest.
 */

// Include core Unity header
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


/* 
 * Test: Additional variant where the array differs from the expected HEX32 value
 *         early in the sequence (abort is expected).
 * Mirrors the structure of the focal testNotEqualHEX32EachEqual2 but with a
 * different first element to ensure the inequality path is exercised.
 */
void testNotEqualHEX32EachEqual2_VariantDifferentFirstElement(void)
{
    // Arrange: a 4-element array with a mismatch at index 0
    UNITY_UINT32 p0[] = {1, 987, 987, 987};

    // Act + Assert: the comparison should abort because not all elements equal 987
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
    VERIFY_FAILS_END
}

/* 
 * Test: Another variant where the mismatch occurs in a later position
 * Ensures the not-all-equal path is detected beyond the first element.
 */
void testNotEqualHEX32EachEqual2_VariantMismatchLater(void)
{
    // Arrange: a 4-element array with a mismatch at index 2
    UNITY_UINT32 p0[] = {987, 987, 986, 987};

    // Act + Assert: abort expected since one element differs from 987
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
    VERIFY_FAILS_END
}

/* 
 * Test: Positive path - all elements are equal to the expected HEX32 value
 * This confirms that the "not equal" path is not erroneously triggered when
 * all elements match.
 */
void testNotEqualHEX32EachEqual2_AllElementsEqualShouldPass(void)
{
    // Arrange: all elements equal to 987
    UNITY_UINT32 p0[] = {987, 987, 987, 987};

    // Act: should pass since all elements equal the expected value
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
}

/* 
 * Test: NULL pointer input handling (should abort as per the contract of the macro)
 * This ensures the focal macro handles invalid pointer input by aborting.
 * Note: This test relies on the abort-capturing macros (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
 * and should be run within the same Unity environment as the focal test.
 */
void testNotEqualHEX32EachEqual2_NULLPointer_ShouldAbort(void)
{
    // Arrange: null pointer with a valid length
    UNITY_UINT32* p0 = NULL;

    // Act + Assert: abort expected due to NULL pointer input
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
    VERIFY_FAILS_END
}

/* 
 * Note:
 * - If you prefer a non-abort variant to strengthen coverage in environments where
 *   abort-capturing macros are unavailable, you can instead place a test using
 *   a valid, all-equal array (see testNotEqualHEX32EachEqual2_AllElementsEqualShouldPass)
 *   as shown above to verify the positive path.
 *
 * - To integrate these tests into your existing testunity.c workflow:
 *   1) Add function declarations to the appropriate test file or header.
 *   2) Ensure the test runner (main or equivalent) invokes RUN_TEST on these new tests.
 *   3) Build with the same Unity setup used by testunity.c (same includes and macros).
 */