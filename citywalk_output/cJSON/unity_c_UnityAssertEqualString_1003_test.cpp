/*
Step 1 candidates (core dependencies identified from UnityAssertEqualString and <FOCAL_CLASS_DEP>):
- Unity, Unity.CurrentTestFailed
- UnityAssertEqualString (the focal method)
- UnityPrintExpectedAndActualStrings
- UnityTestResultsFailBegin
- UnityAddMsgIfSpecified
- UNITY_FAIL_AND_BAIL
- RETURN_IF_FAIL_OR_IGNORE
- msg (parameter)
- lineNumber (parameter)
- expected / actual (parameters)
- The null-pointer handling branch (expected && actual vs else)
- The for-loop comparison logic (character-wise comparison)
- The fallback for both pointers being null
The tests below target equal strings, both-null case, and failure cases (one or both non-equal).
*/

extern "C" {
#include <stddef.h>
#include <cstdint>
#include <unity.h>

}


// Note: This test suite uses Unity's own test harness to exercise the focal C function
// UnityAssertEqualString directly. The test suite is written in C++11 style but relies on
// the C Unity framework semantics (no GTest). The Unity test runner will handle bailouts
// from failing assertions via setjmp/longjmp, so we avoid terminating the test process manually.

/*
Test 1: Equal non-null strings should pass.
- expected = "hello"
- actual   = "hello"
- msg      = empty
- lineNumber = current test line
Expected outcome: Unity.CurrentTestFailed remains 0 and no bail occurs.
*/
void test_UnityAssertEqualString_Pass_EqualNonNull(void)
{
    // Arrange: prepare inputs
    const char* expected = "hello";
    const char* actual   = "hello";
    const char* msg      = "";
    UNITY_LINE_TYPE line = (UNITY_LINE_TYPE)__LINE__;

    // Act: invoke the focal method
    UnityAssertEqualString(expected, actual, msg, line);

    // Assert: the global Unity state should reflect a passing assertion
    // If UnityAssertEqualString failed, Unity would bail and RUN_TEST would catch it.
    // Here we simply ensure no bail occurred by design (no crash, no fail flag set).
    TEST_ASSERT_EQUAL_UINT(0, Unity.CurrentTestFailed);
}

/*
Test 2: Both pointers null should pass (both null is considered equal by the implementation).
- expected = NULL
- actual   = NULL
- msg      = empty
- lineNumber = current test line
Expected outcome: Unity.CurrentTestFailed remains 0.
*/
void test_UnityAssertEqualString_Pass_BothNull(void)
{
    const char* expected = nullptr;
    const char* actual   = nullptr;
    const char* msg      = "";
    UNITY_LINE_TYPE line = (UNITY_LINE_TYPE)__LINE__;

    UnityAssertEqualString(expected, actual, msg, line);

    TEST_ASSERT_EQUAL_UINT(0, Unity.CurrentTestFailed);
}

/*
Test 3: One pointer null and the other non-null should fail.
- expected = "abc"
- actual   = NULL
- msg      = "expected non-null, actual null"
- lineNumber = current test line
Expected outcome: Unity.CurrentTestFailed becomes 1 and the test is reported as failed by Unity.
*/
void test_UnityAssertEqualString_Fail_ExpectedNonNull_ActualNull(void)
{
    const char* expected = "abc";
    const char* actual   = nullptr;
    const char* msg      = "expected non-null, actual null";
    UNITY_LINE_TYPE line = (UNITY_LINE_TYPE)__LINE__;

    UnityAssertEqualString(expected, actual, msg, line);

    TEST_ASSERT_NOT_EQUAL(0, Unity.CurrentTestFailed);
}

/*
Test 4: Non-null strings with different content should fail.
- expected = "abc"
- actual   = "abd"
- msg      = "contents differ"
- lineNumber = current test line
Expected outcome: Unity.CurrentTestFailed becomes 1 and Unity reports failure for string mismatch.
*/
void test_UnityAssertEqualString_Fail_DifferentContent(void)
{
    const char* expected = "abc";
    const char* actual   = "abd";
    const char* msg      = "contents differ";
    UNITY_LINE_TYPE line = (UNITY_LINE_TYPE)__LINE__;

    UnityAssertEqualString(expected, actual, msg, line);

    TEST_ASSERT_NOT_EQUAL(0, Unity.CurrentTestFailed);
}

int main(void)
{
    // Initialize Unity test framework
    UnityBegin("unity_assert_equal_string_tests.c");

    // Run the tests
    RUN_TEST(test_UnityAssertEqualString_Pass_EqualNonNull, __LINE__);
    RUN_TEST(test_UnityAssertEqualString_Pass_BothNull, __LINE__);
    RUN_TEST(test_UnityAssertEqualString_Fail_ExpectedNonNull_ActualNull, __LINE__);
    RUN_TEST(test_UnityAssertEqualString_Fail_DifferentContent, __LINE__);

    // Close Unity and return result
    return UnityEnd();
}