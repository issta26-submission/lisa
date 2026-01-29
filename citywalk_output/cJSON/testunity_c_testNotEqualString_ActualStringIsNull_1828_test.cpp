/*
  Test Suite: NotEqualString with Actual String NULL (focal method)
  Objective:
  - Provide a focused C++11 test suite that targets the focal Unity test:
      testNotEqualString_ActualStringIsNull located in testunity.c
  - Use Unity-style assertions and macros to exercise the abort path when the actual
    string is NULL, and also cover a couple of non-abort cases to increase coverage.

  Note:
  - We avoid GTest as requested.
  - This test suite is designed to be compiled with Unity's library (unity.c/.h) available.
  - The suite imports Unity dependencies via unity.h and provides minimal stubs
    for spying-related helper functions used by the macros in the Unity test harness.

  Candidate Keywords (Step 1 insights):
  - TEST_ASSERT_EQUAL_STRING, TEST_ASSERT_EQUAL_STRING_LEN
  - NULL, "foo", "actual" string
  - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - startPutcharSpy, endPutcharSpy, getBufferPutcharSpy (Unity I/O spying hooks)
  - UnityBegin, RUN_TEST, UnityEnd
  - Non-terminating assertions vs. failing assertions (to exercise branches)
  - extern "C" linkage for cross-language compatibility (C tests in C++)
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Declarations of test functions to allow cross-language linkage with Unity harness
// (needed because we implement tests in C++ but Unity expects C-style entry points)
void testNotEqualString_ActualStringIsNull_ShouldAbort(void);
void testNotEqualStringLen_ActualStringIsNull_ShouldAbort(void);
void testEqualStrings_Pass(void);
void testEqualStrings_BothNull_Pass(void);

#ifdef __cplusplus
}
#endif

// Minimal stubs for spying hooks used by Unity's EXPECT_ABORT_BEGIN macros
// The real implementation exists in the provided Unity harness in test unity
void startPutcharSpy(void) { /* no-op stub for test harness */ }
void endPutcharSpy(void) { /* no-op stub for test harness */ }
char* getBufferPutcharSpy(void) { return nullptr; }

// Test 1: Focal equivalent - Actual string is NULL; expect an abort (failure)
void testNotEqualString_ActualStringIsNull_ShouldAbort(void)
{
    // This reproduces the focal behavior:
    // TEST_ASSERT_EQUAL_STRING("foo", NULL);
    // Expect the assertion to fail and Unity to abort the test as designed.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("foo", NULL);
    VERIFY_FAILS_END
}

// Test 2: Len variant of focal case - Actual is NULL; expect an abort (failure)
void testNotEqualStringLen_ActualStringIsNull_ShouldAbort(void)
{
    // Reproduces the focal length-variant usage:
    // TEST_ASSERT_EQUAL_STRING_LEN("foo", NULL, 1);
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foo", NULL, 1);
    VERIFY_FAILS_END
}

// Test 3: Happy path to increase coverage for non-abort case (equal strings)
void testEqualStrings_Pass(void)
{
    // This covers the non-abort path where both strings are equal
    TEST_ASSERT_EQUAL_STRING("foo", "foo");
}

// Test 4: Both NULL strings should pass (null-vs-null should be considered equal)
void testEqualStrings_BothNull_Pass(void)
{
    // This confirms that NULL vs NULL is treated as equal by the macro
    TEST_ASSERT_EQUAL_STRING(NULL, NULL);
}

// Main runner for the test suite (Step 3 guidelines)
int main(void)
{
    UNITY_BEGIN("testNotEqualStringActualStringIsNull_suite.cpp");

    // Execute tests
    RUN_TEST(testNotEqualString_ActualStringIsNull_ShouldAbort);
    RUN_TEST(testNotEqualStringLen_ActualStringIsNull_ShouldAbort);
    RUN_TEST(testEqualStrings_Pass);
    RUN_TEST(testEqualStrings_BothNull_Pass);

    return UNITY_END();
}