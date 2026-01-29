// This test suite targets the UnityAssertEqualStringLen function
// located in unity.c. It is built against the Unity C framework (non-GTest).
// The tests are written in C++11-compatible style, but rely on the C Unity API.
// Note: To fully cover all code paths, some failure paths would terminate the test
// via UNITY_FAIL_AND_BAIL. Here, we focus on non-failing paths to ensure executable tests
// and to exercise the successful branches of the function.
// Additional failure-path tests can be added using Unity's test harness with
// setjmp/longjmp wrappers if longjmp behavior needs to be intercepted.

#include <stddef.h>
#include <unity.h>


// Include the Unity framework headers
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declarations for test functions to ensure C linkage (needed for C++ compilation)
#ifdef __cplusplus
extern "C" {
#endif
void test_UnityAssertEqualStringLen_PassExactMatch(void);
void test_UnityAssertEqualStringLen_PassZeroLength(void);
void test_UnityAssertEqualStringLen_BothNullPass(void);
#ifdef __cplusplus
}
#endif

// The test runner uses Unity's RUN_TEST macro. We provide a minimal main to execute the tests.
int main(void)
{
    // Begin the Unity test run for the target Unity implementation (unity.c).
    UnityBegin("unity.c_UnityAssertEqualStringLen_tests");

    // Execute test cases
    // Each test is annotated with comments explaining the purpose and the scenario tested.
    RUN_TEST(test_UnityAssertEqualStringLen_PassExactMatch); // Case: non-null pointers, identical content, exact length match
    RUN_TEST(test_UnityAssertEqualStringLen_PassZeroLength);  // Case: length == 0, should pass regardless of string content
    RUN_TEST(test_UnityAssertEqualStringLen_BothNullPass);    // Case: both pointers NULL, should pass

    // End the test run and return the result
    return UnityEnd();
}

// Test 1: Passing scenario with exact content match
// Purpose: Ensure that UnityAssertEqualStringLen does not flag a failure when expected and actual
//          strings are non-null, identical, and the specified length matches the string content.
void test_UnityAssertEqualStringLen_PassExactMatch(void)
{
    const char* expected = "Hello";
    const char* actual   = "Hello";
    // length equals the string length for "Hello" (5)
    const UNITY_UINT32 length = 5;

    // This call should not trigger a test failure.
    UnityAssertEqualStringLen(expected, actual, length, NULL, __LINE__);

    // Verify that no failure was recorded for this test.
    // We use a non-terminating assertion here to confirm the internal state.
    TEST_ASSERT_EQUAL_INT(0, Unity.CurrentTestFailed);
}

// Test 2: Passing scenario when length is zero
// Purpose: Verify that when length is zero, the function returns without marking a failure,
//          even if the pointers are non-null and content differs. The loop body will not execute.
void test_UnityAssertEqualStringLen_PassZeroLength(void)
{
    const char* expected = "ABC";
    const char* actual   = "XYZ";
    const UNITY_UINT32 length = 0; // zero-length should bypass the comparison loop

    // This call should pass since length is zero.
    UnityAssertEqualStringLen(expected, actual, length, NULL, __LINE__);

    // Confirm no failure was recorded for this test.
    TEST_ASSERT_EQUAL_INT(0, Unity.CurrentTestFailed);
}

// Test 3: Passing scenario when both pointers are NULL
// Purpose: Ensure that when both expected and actual are NULL, the function does not flag a failure.
void test_UnityAssertEqualStringLen_BothNullPass(void)
{
    const char* expected = NULL;
    const char* actual   = NULL;
    const UNITY_UINT32 length = 5; // arbitrary length; pointers are NULL so the else branch is taken

    // This call should pass since both pointers are NULL and equal.
    UnityAssertEqualStringLen(expected, actual, length, NULL, __LINE__);

    // Verify that no failure was recorded for this test.
    TEST_ASSERT_EQUAL_INT(0, Unity.CurrentTestFailed);
}