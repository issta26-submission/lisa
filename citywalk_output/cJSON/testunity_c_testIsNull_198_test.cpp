/*******************************************************
 * Test Suite: Focused tests for the focal method
 * testIsNull (located in testunity.c)
 *
 * This suite is designed to be compiled under C++11
 * without GTest. It uses the Unity framework macros
 * to exercise the canonical "is null" and "is not null"
 * checks performed by testIsNull.
 *
 * Step 1 (Conceptual): Candidate Keywords
 * - ptr1 (char*), ptr2 (const char*): the two pointers used
 * - NULL: zero pointer sentinel
 * - "hello": non-null string literal
 * - TEST_ASSERT_NULL, TEST_ASSERT_NOT_NULL: focal assertions
 * - The two branches: NULL vs non-NULL pointers
 *
 * Step 2 (Unit Test Generation): The tests below explicitly
 * cover:
 * - Positive path: ptr1 == NULL and ptr2 != NULL
 * - Negative path variant A: ptr2 == NULL (NOT_NULL should fail)
 * - Negative path variant B: ptr1 != NULL (NULL should fail)
 *
 * Step 3 (Test Case Refinement): Tests are written to be
 * non-terminating where possible and to use Unity's
 * EXPECT_ABORT_BEGIN/VERIFY_FAILS_END for expected-fail cases.
 *******************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// If testIsNull were to be invoked directly as a test, declare it here:
// extern void testIsNull(void);

#ifdef __cplusplus
}
#endif

// Test 1: Positive path - ensure a NULL pointer is Null and a non-null
// pointer is Not-Null, mirroring the focal method's intended behavior.
static void testIsNull_PositivePath_Reimplementation(void)
{
    // Re-implement the focal path to validate the assertions directly
    {
        char* ptr1 = NULL;
        const char* ptr2 = "hello";
        TEST_ASSERT_NULL(ptr1);     // ptr1 should be NULL
        TEST_ASSERT_NOT_NULL(ptr2); // ptr2 should be non-NULL
    }
}

// Test 2: Negative path variant - ptr2 is NULL, so TEST_ASSERT_NOT_NULL should fail
static void testIsNull_NonNullPtr2_ShouldFail(void)
{
    const char* ptr2 = NULL;
    char* ptr1 = NULL;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NULL(ptr1);     // This should pass
    TEST_ASSERT_NOT_NULL(ptr2); // This should fail (ptr2 is NULL)
    VERIFY_FAILS_END
}

// Test 3: Negative path variant - ptr1 is non-NULL, so TEST_ASSERT_NULL should fail
static void testIsNull_Ptr1_ShouldFail(void)
{
    char* ptr1 = (char*) "hello"; // non-NULL (note: string literal cast for C++)
    const char* ptr2 = "hello";
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_NULL(ptr1); // This should fail (ptr1 is non-NULL)
    VERIFY_FAILS_END
}

// Main runner for this test suite
int main(void)
{
    UnityBegin("test_isnull_suite.cpp");

    RUN_TEST(testIsNull_PositivePath_Reimplementation);
    RUN_TEST(testIsNull_NonNullPtr2_ShouldFail);
    RUN_TEST(testIsNull_Ptr1_ShouldFail);

    return UnityEnd();
}