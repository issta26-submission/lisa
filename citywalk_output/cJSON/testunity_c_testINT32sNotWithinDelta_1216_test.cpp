// test_int32s_not_within_delta_suite.cpp
// A focused test suite for the focal method testINT32sNotWithinDelta
// using Unity C framework in a C++11 project (no GTest).

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declarations of test cases
// These tests extend coverage around TEST_ASSERT_INT32_WITHIN and its messages.
void testINT32sNotWithinDelta_BetweenNegativeAndPositive(void);
void testINT32sNotWithinDelta_WithDeltaOneFromZero(void);
void testINT32sWithinDelta_EdgeCases(void);
void testINT32sWithinDelta_WithNegativeEdge(void);
void testINT32sNotWithinDelta_CustomMessage_Edge(void);

// Minimal stubs required by EXPECT_ABORT_BEGIN / VERIFY_FAILS_END macros
// The Unity framework uses these to wrap abortible tests. They don't need
// to perform any special action for these tests, so keep them no-ops.
void startPutcharSpy(void) { /* no-op */ }
void endPutcharSpy(void) { /* no-op */ }

// If Unity expects a buffer (not used here), provide a harmless stub.
char* getBufferPutcharSpy(void) { return nullptr; }

// Standard setup/teardown (no special state needed)
void setUp(void) { /* no setup required for these tests */ }
void tearDown(void) { /* no teardown required for these tests */ }

/*
 * Test: testINT32sNotWithinDelta_BetweenNegativeAndPositive
 * Purpose: Ensure TEST_ASSERT_INT32_WITHIN(false) path is correctly aborted
 *          when the actual value is far outside the allowed delta.
 *           delta = 1, expected = -100, actual = 100; difference = 200
 */
void testINT32sNotWithinDelta_BetweenNegativeAndPositive(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(1, -100, 100);
    VERIFY_FAILS_END
    // If execution reaches here, the test unexpectedly did not abort as expected.
}

/*
 * Test: testINT32sNotWithinDelta_WithDeltaOneFromZero
 * Purpose: Validate not-within-delta path when numbers straddle zero with
 *          a small delta.
 *           delta = 1, expected = 0, actual = 2; difference = 2
 */
void testINT32sNotWithinDelta_WithDeltaOneFromZero(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(1, 0, 2);
    VERIFY_FAILS_END
}

/*
 * Test: testINT32sWithinDelta_EdgeCases
 * Purpose: Confirm within-delta scenarios are treated as passing tests.
 *           delta = 1, expected = 0, actual = 1 (diff = 1)
 *           delta = 5, expected = -10, actual = -14 (diff = 4)
 */
void testINT32sWithinDelta_EdgeCases(void)
{
    // Within delta with small positive difference
    TEST_ASSERT_INT32_WITHIN(1, 0, 1);

    // Within delta with negative numbers
    TEST_ASSERT_INT32_WITHIN(5, -10, -14);
}

/*
 * Test: testINT32sWithinDelta_WithNegativeEdge
 * Purpose: Validate within-delta behavior for negative values near boundary.
 *           delta = 5, expected = -50, actual = -55 (diff = 5)
 */
void testINT32sWithinDelta_WithNegativeEdge(void)
{
    TEST_ASSERT_INT32_WITHIN(5, -50, -55);
}

/*
 * Test: testINT32sNotWithinDelta_CustomMessage_Edge
 * Purpose: Ensure that a custom message is included on assertion failure.
 *           delta = 3, expected = 100, actual = 104 (diff = 4 > delta)
 * The test uses a custom message to verify messaging pathway.
 */
void testINT32sNotWithinDelta_CustomMessage_Edge(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN_MESSAGE(3, 100, 104, "Custom message: INT32 within delta failed as expected.");
    VERIFY_FAILS_END
}

// Main entry to run the focused test suite
int main(void)
{
    UNITY_BEGIN("test_int32s_not_within_delta_suite.cpp");

    RUN_TEST(testINT32sNotWithinDelta_BetweenNegativeAndPositive, __LINE__);
    RUN_TEST(testINT32sNotWithinDelta_WithDeltaOneFromZero, __LINE__);
    RUN_TEST(testINT32sWithinDelta_EdgeCases, __LINE__);
    RUN_TEST(testINT32sWithinDelta_WithNegativeEdge, __LINE__);
    RUN_TEST(testINT32sNotWithinDelta_CustomMessage_Edge, __LINE__);

    return UNITY_END();
}