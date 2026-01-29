/*
Step 1 - Program Understanding (Candidate Keywords)
- Focused method: testPrintNumbersUInt64 (a Unity test function in testunity.c)
- Key macros that govern behavior:
  - UNITY_SUPPORT_64: enables 64-bit support path
  - USING_OUTPUT_SPY: enables output spying to verify printed output
  - TEST_IGNORE / TEST_IGNORE_MESSAGE: skip paths when unsupported
  - TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS: used inside the focal test to validate printed numbers
  - startPutcharSpy / endPutcharSpy / getBufferPutcharSpy: helpers to capture and inspect printed output
- Core dependencies (based on <FOCAL_CLASS_DEP> contents):
  - External C interfaces: testPrintNumbersUInt64(void)
  - Spy controls: startPutcharSpy(), endPutcharSpy(), getBufferPutcharSpy()
  - Output verification: strstr-based checks on captured buffer
- Purpose: Validate that testPrintNumbersUInt64 correctly prints 64-bit unsigned integers when 64-bit support and output spy are available.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test is declared with C linkage
void testPrintNumbersUInt64(void);
#ifdef __cplusplus
}
#endif

// Interop for the output spy (only available when 64-bit support and spy are enabled)
#if defined(UNITY_SUPPORT_64) && defined(USING_OUTPUT_SPY)
extern "C" void startPutcharSpy(void);
extern "C" void endPutcharSpy(void);
extern "C" char* getBufferPutcharSpy(void);
#endif

/*
Test 1: Verify that testPrintNumbersUInt64 outputs the expected 64-bit unsigned numbers
- Preconditions:
  - UNITY_SUPPORT_64 and USING_OUTPUT_SPY must be defined to enable the meaningful path.
- Approach:
  - Start the putchar spy, invoke the focal test, stop the spy, and fetch the captured buffer.
  - Assert that the buffer contains all four expected decimal representations in any order/position.
- Rationale:
  - Ensures that the 64-bit path prints the correct values: 0, 70000000000, 9223372036854775808, and 18446744073709551615.
*/
#if defined(UNITY_SUPPORT_64) && defined(USING_OUTPUT_SPY)
static void testPrintNumbersUInt64_OutputSpy64(void)
{
    // Start capturing printed output
    startPutcharSpy();

    // Execute the focal unit test
    testPrintNumbersUInt64();

    // Stop capturing output and fetch buffer
    endPutcharSpy();

    char* buffer = getBufferPutcharSpy();
    TEST_ASSERT_NOT_NULL(buffer);

    // Validate presence of all expected numbers in the captured output
    // We rely on substring searches to tolerate formatting/spaces/newlines
    TEST_ASSERT_NOT_NULL(strstr(buffer, "0"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "70000000000"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "9223372036854775808"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "18446744073709551615"));
}
#endif

/*
Test 2: Wrapper test to integrate with Unity's RUN_TEST mechanism
- Purpose:
  - Provide a test entry that, when the build enables 64-bit support with an output spy, calls the detailed test above.
- Behavior:
  - If 64-bit support or spy is not enabled, this test will be ignored gracefully via Unity's TEST_IGNORE_MESSAGE.
- Rationale:
  - Maintains compatibility with a test runner that discovers tests via function naming conventions and RUN_TEST macros.
*/
static void testPrintNumbersUInt64_Wrapper(void)
{
#if defined(UNITY_SUPPORT_64) && defined(USING_OUTPUT_SPY)
    testPrintNumbersUInt64_OutputSpy64();
#else
    // If the environment is not configured for 64-bit and output spy, skip gracefully
    TEST_IGNORE_MESSAGE("Skipping testPrintNumbersUInt64: UNITY_SUPPORT_64 or USING_OUTPUT_SPY not defined.");
#endif
}

/* 
Note for integration:
- The Unity harness typically requires the test wrapper to be registered with RUN_TEST in a runner.
- If your build system automatically collects all functions named test* or provides a central runner, ensure to add:
    RUN_TEST(testPrintNumbersUInt64_Wrapper);
- If your project uses an alternative test discovery mechanism, adapt accordingly.
*/

/*
Step 3 - Domain Knowledge Refinement (Comments)
- This suite ensures coverage of the 64-bit path by exercising the only test that prints unsigned 64-bit numbers when 64-bit support is present.
- We rely on the output spy to non-intrusively validate printed values without terminating the test run.
- Static members and private internals are not accessed directly; the test uses the public focal method and the provided spy interface.
- All tests use non-terminating assertions (i.e., they rely on Unity's assertion macros and do not call TEST_FAIL directly except via the standard failure paths inside testPrintNumbersUInt64 itself).
- If a project uses a different namespace or C linkage constraints, adapt the extern "C" blocks accordingly.
- This file is designed to compile as part of a C/C++ Unity-based test suite (C++11 compatible). It does not provide a main() since that is typically supplied by testunity.c.
*/