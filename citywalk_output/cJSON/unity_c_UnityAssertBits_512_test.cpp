/*
    Test suite for UnityAssertBits in unity.c

    Step 1 (Conceptual): UnityAssertBits depends on mask/expected/actual bitwise
    logic and on Unity's test harness (fail/bail, print helpers, and optional
    message). Key dependent components identified:
      - mask, expected, actual: bitwise evaluation (mask & expected) vs (mask & actual)
      - RETURN_IF_FAIL_OR_IGNORE, UnityTestResultsFailBegin, UnityPrint, UnityPrintMask
      - UnityStrExpected, UnityStrWas, UnityAddMsgIfSpecified, UNITY_FAIL_AND_BAIL
      - lineNumber (for reporting the failure location)

    Step 2 (Test generation): We exercise both branches (pass and fail) of the
    predicate (mask & expected) != (mask & actual) and also cover edge cases
    (zero mask, negative values).

    Step 3 (Refinement): Tests cover:
      - Pass case where masked bits match
      - Fail case where masked bits differ
      - Edge case with zero mask (always pass)
      - Negative values handling (masking with negative masks)

    Implementation notes:
      - The test harness uses the Unity framework. Tests are written in C++11
        but bind to the C Unity API via extern "C".
      - We rely on Unity's test runner to catch failures (RUN_TEST, UnityEnd, etc.).
      - Each test includes explanatory comments describing purpose and expected outcome.
*/

#include <stddef.h>
#include <cstdint>
#include <unity.h>


extern "C" {
}

// Forward declare tests (no need for extern "C" here since these are C-linkage through Unity)
void test_UnityAssertBits_ShouldPass_WhenBitsMatch(void);
void test_UnityAssertBits_ShouldFail_WhenBitsMismatch(void);
void test_UnityAssertBits_ZeroMask_ShouldAlwaysPass(void);
void test_UnityAssertBits_NegativeMaskAndValues_ShouldPass(void);
void test_UnityAssertBits_NegativeMaskMismatchEdgeCase(void);

/* 
   Set up and tearDown are optional for these tests; included for completeness.
*/
void setUp(void)    { /* Not needed for UnityAssertBits tests; present for compatibility */ }
void tearDown(void) { /* Not needed for UnityAssertBits tests; present for compatibility */ }

/*
   Test 1: Pass case where (mask & expected) == (mask & actual)
   - This ensures the function does not print failures or bail when masked bits match.
*/
void test_UnityAssertBits_ShouldPass_WhenBitsMatch(void)
{
    // mask selects some bits; expected and actual have identical values in those bits.
    // No failure should be produced; test should pass.
    const int mask = 0x3F;       // sample 6-bit mask
    const int expected = 0x15;     // 0b010101
    const int actual   = 0x15;     // identical in masked region
    const char* msg = NULL;
    const int lineNumber = 10;

    UnityAssertBits(mask, expected, actual, msg, lineNumber);
}

/*
   Test 2: Fail case where (mask & expected) != (mask & actual)
   - This tests the true failure path of UnityAssertBits and ensures
   - UnityTestResultsFailBegin, UnityPrint, UnityPrintMask, and UnityAddMsgIfSpecified
   - are exercised and UNITY_FAIL_AND_BAIL is invoked.
   - This test is expected to fail the current Unity test case; the Unity harness will record a failure and continue.
*/
void test_UnityAssertBits_ShouldFail_WhenBitsMismatch(void)
{
    // mask selects a region where expected and actual differ
    const int mask = 0xFF;        // 8-bit mask
    const int expected = 0x5A;      // 0b01011010
    const int actual   = 0x5B;      // 0b01011011 (differs in LSB within mask)
    const char* msg = "Bits do not match under mask";
    const int lineNumber = 20;

    UnityAssertBits(mask, expected, actual, msg, lineNumber);
}

/*
   Test 3: Edge case with zero mask
   - By design, (mask & anything) == (mask & anything2) for mask == 0
   - This should always pass; ensures no spurious failures when mask is zero.
*/
void test_UnityAssertBits_ZeroMask_ShouldAlwaysPass(void)
{
    const int mask = 0x0;
    const int expected = 0xDEADBEEF;
    const int actual   = 0xCAFEBABE;
    const char* msg = NULL;
    const int lineNumber = 30;

    UnityAssertBits(mask, expected, actual, msg, lineNumber);
}

/*
   Test 4: Negative mask and values - pass scenario
   - Verifies that negative bit patterns are handled correctly by masking.
*/
void test_UnityAssertBits_NegativeMaskAndValues_ShouldPass(void)
{
    const int mask = -1;      // all bits selected
    const int expected = -1;
    const int actual   = -1;
    const char* msg = NULL;
    const int lineNumber = 40;

    UnityAssertBits(mask, expected, actual, msg, lineNumber);
}

/*
   Test 5: Negative mask with a mismatch edge-case
   - A mismatch on a negative mask boundary to ensure correct comparison behavior.
*/
void test_UnityAssertBits_NegativeMaskMismatchEdgeCase(void)
{
    const int mask = -1;      // all bits active
    const int expected = -1;
    const int actual   = 0;     // difference on all bits
    const char* msg = "Negative mask mismatch edge case";
    const int lineNumber = 50;

    UnityAssertBits(mask, expected, actual, msg, lineNumber);
}

/*
   Step 2: Test suite runner
   - We invoke a minimal set of tests using Unity's runner.
*/
int main(void)
{
    UnityBegin("unity_assertbits_tests");

    // Test cases to cover the true and false branches of the predicate
    RUN_TEST(test_UnityAssertBits_ShouldPass_WhenBitsMatch);
    RUN_TEST(test_UnityAssertBits_ShouldFail_WhenBitsMismatch);
    RUN_TEST(test_UnityAssertBits_ZeroMask_ShouldAlwaysPass);
    RUN_TEST(test_UnityAssertBits_NegativeMaskAndValues_ShouldPass);
    RUN_TEST(test_UnityAssertBits_NegativeMaskMismatchEdgeCase);

    return UnityEnd();
}