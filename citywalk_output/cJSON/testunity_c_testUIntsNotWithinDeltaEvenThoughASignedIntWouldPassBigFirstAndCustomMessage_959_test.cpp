/************************************************************
 * Test Suite: Focused coverage for testUIntsNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage
 * Context: This suite extends the existing Unity-based tests in testunity.c
 *          by adding targeted cases that exercise the signed-vs-unsigned behavior
 *          when asserting unsigned within-delta relationships.
 * 
 * Assumptions:
 *  - The project uses the Unity framework (unity.h) and the focal test
 *    function resides in testunity.c with the macro-based tests.
 *  - The new tests are designed to be picked up by the existing Unity test runner
 *    (i.e., they follow the same TEST naming/style conventions so that RUN_TEST
 *    in the Unity harness will discover them automatically).
 * 
 * Domain Knowledge Summary (for test design):
 *  - We must exercise unsigned arithmetic behavior when a signed value is cast to unsigned.
 *  - The focal test uses TEST_ASSERT_UINT_WITHIN_MESSAGE(delta, expected, actual, "Custom Message.")
 *  - A negative 'expected' (e.g., -1) is cast to unsigned (e.g., 0xFFFFFFFF), causing
 *    large absolute differences that are outside typical deltas (e.g., 5).
 *  - We should verify behavior with various message values: non-null, NULL, and empty strings.
 *
 * Candidate Keywords (from Step 1):
 *  - UINT, WITHIN, DELTA, CUSTOM_MESSAGE, SIGNED_VS_UNSIGNED, BIG_FIRST, WITHIN, NOT_WITHIN
 *  - TEST_ASSERT_UINT_WITHIN_MESSAGE, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
 *  - NegativeAsUnsigned, BoundaryEdge, MessageVariants
 ************************************************************/

/* Include Unity framework header to ensure macros are available.
   The Unity harness (unity.c) should be compiled in the test build. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


/* ------------------------------------------------------------------
   New test cases for the focal scenario:
     - testUIntsNotWithinDeltaBigFirstAndCustomMessageNull
     - testUIntsNotWithinDeltaBigFirstAndCustomMessageEmpty
   These mirror the pattern of the existing focal test and validate that
   when a signed negative value is cast to unsigned, the delta check fails,
   even though a signed interpretation would yield a small difference.
   ------------------------------------------------------------------ */

/* Test: Negative expected value (-1) cast to unsigned; actual=1; Custom Message = NULL */
void testUIntsNotWithinDeltaBigFirstAndCustomMessageNull(void)
{
    // This should abort because (unsigned)(-1) vs 1 are far apart (> delta)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, -1, 1, NULL);
    VERIFY_FAILS_END
    // Explanatory: Using NULL as custom message verifies that the framework handles
    // NULL gracefully and still records the failure as expected.
}

/* Test: Negative expected value (-1) cast to unsigned; actual=1; Custom Message = "" (empty string) */
void testUIntsNotWithinDeltaBigFirstAndCustomMessageEmpty(void)
{
    // This should abort for the same reason as above; message is an empty string
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT_WITHIN_MESSAGE(5, -1, 1, "");
    VERIFY_FAILS_END
    // Explanatory: Empty message should not alter the failure semantics; ensures
    // message pointer handling does not affect the abort condition.
}

/* Note to test runners:
   These tests are designed to be discovered by the existing Unity harness.
   If you maintain separate test registration, ensure both tests are added to
   the test runner (e.g., RUN_TEST(testUIntsNotWithinDeltaBigFirstAndCustomMessageNull);
   RUN_TEST(testUIntsNotWithinDeltaBigFirstAndCustomMessageEmpty);). */

/* End of focused test additions for the focal method under test. */

/* 
   On top of these tests, you may also consider adding:
     - A variant with actual=UINT_MAX and expected=-2 to check delta edge
     - A variant with a non-NULL arbitrary custom message to confirm message display
   However, the above two tests specifically target the NULL vs empty string cases,
   which can often be a source of NULL-pointer or zero-length string handling bugs
   within test frameworks.
*/