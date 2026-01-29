/*
 * Test suite for the focal method: testUINT16sWithinDelta
 * Focus: validate the UINT16 within-delta assertions via Unity framework.
 *
 * Step 1 - Candidate Keywords (derived from focal method)
 * - TEST_ASSERT_UINT16_WITHIN
 * - TEST_ASSERT_UINT16_WITHIN_MESSAGE
 * - delta concept (within given tolerance)
 * - UINT16 data type (uint16_t)
 * - specific sample values: 1, 5000, 5001, 4996, 5005
 * - the notion of "within delta" for unsigned 16-bit comparisons
 *
 * Step 2/3 - We provide a compact, executable Unity-based test suite
 * focusing on the UINT16 within-delta behavior, with comments explaining each test.
 * Note: We avoid tests that intentionally abort (to keep all tests executable
 * in a single run). We include pass-path tests and a couple of edge-case
 * scenarios that remain non-aborting in this suite.
 *
 * This file is written to compile under C++11 as requested, while leveraging
 * the Unity C unit-testing framework (unity.h).
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// If there are project-specific setup/teardown constraints, add them here.
void setUp(void) { /* Nothing to initialize for these tests */ }
void tearDown(void) { /* Nothing to clean up for these tests */ }

/*
 * Test 1: Basic coverage of UINT16 within-delta for three cases
 * This mirrors the focal method's intent and ensures the core behavior is correct.
 */
static void testUINT16sWithinDelta_Basic(void)
{
    // 5000 and 5001 are within delta 1
    TEST_ASSERT_UINT16_WITHIN(1, 5000, 5001);

    // 5000 and 4996 are within delta 5 (diff = 4)
    TEST_ASSERT_UINT16_WITHIN(5, 5000, 4996);

    // 5000 and 5005 are within delta 5 (diff = 5)
    TEST_ASSERT_UINT16_WITHIN(5, 5000, 5005);
}

/*
 * Test 2: UINT16 within-delta with custom message
 * Verifies that the variant with an explicit message still passes when values are within delta.
 */
static void testUINT16sWithinDelta_WithCustomMessage_Basic(void)
{
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(1, 5000, 5001, "Custom: within delta 1");
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(5, 5000, 4996, "Custom: within delta 5");
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(5, 5000, 5005, "Custom: within delta 5");
}

/*
 * Test 3: Large delta passes existing difference
 * When delta is large (e.g., 65535), even big differences should pass.
 */
static void testUINT16sWithinDelta_BigDelta_Pass(void)
{
    // Large delta should accept a modest difference
    TEST_ASSERT_UINT16_WITHIN(65535, 0, 5000);
}

/*
 * Test 4: Zero delta passes exact equality
 * Delta = 0 should accept only exact matches.
 * We test a clear pass case here.
 */
static void testUINT16sWithinDelta_ZeroDelta_Pass(void)
{
    TEST_ASSERT_UINT16_WITHIN(0, 5000, 5000);
}

/* Main runner to execute the focused tests above. */
int main(void)
{
    UNITY_BEGIN();

    // Execute tests with explanatory comments
    RUN_TEST(testUINT16sWithinDelta_Basic);
    RUN_TEST(testUINT16sWithinDelta_WithCustomMessage_Basic);
    RUN_TEST(testUINT16sWithinDelta_BigDelta_Pass);
    RUN_TEST(testUINT16sWithinDelta_ZeroDelta_Pass);

    return UNITY_END();
}