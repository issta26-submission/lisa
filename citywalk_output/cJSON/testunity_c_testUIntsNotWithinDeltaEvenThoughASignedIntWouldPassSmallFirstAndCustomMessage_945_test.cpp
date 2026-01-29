// Step 1 - Candidate Keywords (for reference)
// - Focal predicate: TEST_ASSERT_UINT_WITHIN_MESSAGE
// - Delta semantics with unsigned wrap: 5, 1, -1
// - Negative value handling via unsigned conversion
// - Custom message propagation: "Custom Message."
// - Aborting behavior capture: EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
// - Output capture hooks: startPutcharSpy, endPutcharSpy, getBufferPutcharSpy
// - Domain: Unity C unit testing macros, unsigned integer comparisons, failure message inspection

// Step 2 & 3 - Expanded test suite for the focal behavior
// Note: This file provides additional tests to exercise the focal scenario
// testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage
// in a self-contained manner. It appends tests that (a) verify the failure case with a custom
// message, (b) verify the failure output includes the custom message, and (c) provide a
// passing variant to cover the "within delta" path as a reference.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declarations for Unity's I/O spy helpers, assumed to be defined in the Unity test setup.
// If already declared elsewhere, these externs may be omitted or adjusted accordingly.
extern void startPutcharSpy(void);
extern void endPutcharSpy(void);
extern char* getBufferPutcharSpy(void);

// 1) Failing case: small-first (actual = -1 cast to unsigned), with a custom message
// This directly mirrors the focal test but is kept as an independent test to boost coverage.
// Expected: Test should abort, and the custom message should be part of the failure output.
void testUIntsNotWithinDeltaSmallFirstAndCustomMessage_Extended(void)
{
    // Candidate Case: failure path with custom message
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END
}

// 2) Failing case output check: small-first with custom message (output captured)
void testUIntsNotWithinDeltaSmallFirstAndCustomMessage_Output_Extended(void)
{
    // Capture the output produced during the abort to verify the custom message is printed
    startPutcharSpy();
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END
    endPutcharSpy();

    char* buffer = getBufferPutcharSpy();
    TEST_ASSERT_NOT_NULL(buffer);

    // Verify that the custom message string appears in the captured output
    // This confirms that the message is propagated as part of the failure output
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Custom Message."));
}

// 3) Passing variant to ensure the "within delta" path is covered for the same macro with a custom message
// This ensures that when values are within the delta, the test passes and executes the body without abort.
void testUIntsWithinDeltaSmallFirstAndCustomMessage_Passes_Extended(void)
{
    // Within delta case should pass; no abort should occur
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 1, 3, "Custom Message.");
}

// 4) Failing case: big-first (actual = -1 as unsigned, expected = 1), with a custom message
// This covers the alternate ordering of arguments to exercise the same wrap behavior.
void testUIntsNotWithinDeltaBigFirstAndCustomMessage_Extended(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
    VERIFY_FAILS_END
}

// 5) Failing case output check: big-first with custom message (output captured)
void testUIntsNotWithinDeltaBigFirstAndCustomMessage_Output_Extended(void)
{
    startPutcharSpy();
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
    VERIFY_FAILS_END
    endPutcharSpy();

    char* buffer = getBufferPutcharSpy();
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Custom Message."));
}

// 6) Optional: a self-contained additional within-delta test variant with a custom message
void testUIntsWithinDeltaSmallFirstAndCustomMessage_SimplePass(void)
{
    // Simple within-delta case to increase coverage of the passing branch
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, 1, 4, "Custom Message.");
}

// Note:
// - These tests are designed to be compiled alongside the existing Unity test suite used by testunity.c.
// - If a custom test runner is used, ensure that RUN_TEST or equivalent is wired to pick up these tests.
// - If the test environment uses a single Unity main (e.g., via unity.c), add these tests to the
//   same compilation unit or create an appropriate runner to execute them.
// - The tests that check for failure use the same abort-capturing pattern as the focal test,
//   and the output checks rely on the putchar spy utilities provided by the test harness
//   (startPutcharSpy/endPutcharSpy/getBufferPutcharSpy). Ensure these hooks are implemented
//   in the Unity test framework in your environment.