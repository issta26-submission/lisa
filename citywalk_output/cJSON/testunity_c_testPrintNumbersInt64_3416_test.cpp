// test_print_numbers_int64_suite.cpp
// Purpose:
//   Provide a focused test harness for the focal method testPrintNumbersInt64
//   defined in testunity.c. This harness delegates the test execution to the
//   focal test so that all its internal branches (determined by compile-time
//   macros) are exercised in a single Unity test invocation.
//
// Key notes transmitted by Step 1-3:
//   - The focal method uses compile-time switches:
//       UNITY_SUPPORT_64 controls whether 64-bit support is compiled in.
//       USING_OUTPUT_SPY controls whether output spying is used (actual assertions).
//   - Depending on those macros, testPrintNumbersInt64 will either
//       - Execute real assertions against printed numbers (when both macros are defined),
//       - Be ignored (when 64-bit is supported but no output spy is provided), or
//       - Be ignored (when 64-bit support itself is not compiled in).
//   - This wrapper ensures we exercise the focal method in a valid Unity test context
//     without modifying its internal implementation or private static helpers.
//   - No GTest is used; Unity framework is assumed to be available in the project.
//   - The test is written for C++11 compatibility, but relies on the existing C Unity test setup.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Focal method declared in testunity.c
// The actual implementation is in the provided focal source; this declares
// the entry point so the wrapper can invoke it within Unity's test harness.
void testPrintNumbersInt64(void);

#ifdef __cplusplus
}
#endif

// Wrapper test: delegates directly to the focal testPrintNumbersInt64.
// This ensures the test runs within Unity's test lifecycle and will respect
// the compile-time branches defined in the focal method.
// Explanations for behavior based on compile-time macros:
// - If UNITY_SUPPORT_64 is not defined: the focal test will compile to TEST_IGNORE().
// - If UNITY_SUPPORT_64 is defined but USING_OUTPUT_SPY is not: the focal test will
//   compile to TEST_IGNORE() (no actual numeric assertions will run).
// - If both UNITY_SUPPORT_64 and USING_OUTPUT_SPY are defined: the focal test will
//   perform actual TEST_ASSERT_EQUAL_PRINT_NUMBERS() calls for the specified values.
void testWrapper_testPrintNumbersInt64(void)
{
    // Direct delegation to the focal method under test.
    // The focal method itself contains all branch coverage logic via preprocessor
    // guards, so this single wrapper suffices to exercise all configured paths.
    testPrintNumbersInt64();
}

// Optional: If you later want to explicitly verify 64-bit branch behavior, you can
// uncomment and implement additional wrappers here that map to testPrintNumbersInt64
// under distinct macros. However, the current single-wrapper approach relies on
// the focal method's own compile-time branches to cover all scenarios.

int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the wrapper test which, in turn, runs the focal test.
    // Depending on compile-time macros, this will exercise the real assertions
    // (when both UNITY_SUPPORT_64 and USING_OUTPUT_SPY are defined) or will
    // be ignored (when the spy is not defined or 64-bit support is absent).
    RUN_TEST(testWrapper_testPrintNumbersInt64);

    // Finalize Unity test run and return the status
    return UNITY_END();
}