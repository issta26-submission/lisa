// test_less_than_hex16.cpp
// A focused test suite for the focal method testLessThanHEX16
// This uses the Unity C unit testing framework in a C++11 environment.
// The tests are designed to exercise true and false branches of the HEX16
// less-than predicate, including pointer-based parameter passing.

#include <unity.h>
#include <cstdio>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Import Unity C test framework
extern "C" {
}

// Forward declarations for Unity's setUp/tearDown if needed
void setUp(void);
void tearDown(void);

// No setup required for these tests
void setUp(void)  { /* intentionally empty */ }
void tearDown(void) { /* intentionally empty */ }

/*
 * Test: Positive case coverage for TEST_ASSERT_LESS_THAN_HEX16 with 16-bit values.
 * This verifies that when the first argument is indeed less than the second,
 * all four variants (direct values and via pointers) pass without aborting.
 */
static void testLessThanHEX16_PositiveCase(void)
{
    uint16_t v0 = 0x0000; // smaller value
    uint16_t v1 = 0xFFFF; // larger value
    uint16_t *p0 = &v0;
    uint16_t *p1 = &v1;

    // Direct comparison: 0x0000 < 0xFFFF -> true
    TEST_ASSERT_LESS_THAN_HEX16(v0, v1);

    // Pointer indirect comparisons
    TEST_ASSERT_LESS_THAN_HEX16(*p0, v1);
    TEST_ASSERT_LESS_THAN_HEX16(v0, *p1);
    TEST_ASSERT_LESS_THAN_HEX16(*p0, *p1);
}

/*
 * Test: Negative case coverage for TEST_ASSERT_LESS_THAN_HEX16.
 * This ensures that a false predicate (first >= second) triggers a test abort.
 * The test uses Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END wrappers to
 * catch the abort and allow the test suite to continue.
 */
static void testLessThanHEX16_NegativeCase_Abort(void)
{
    uint16_t v0 = 0xFFFF; // larger value
    uint16_t v1 = 0x0000; // smaller value
    uint16_t *p0 = &v0;
    uint16_t *p1 = &v1;

    // Expect the predicate to fail (since v0 is not < v1)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX16(v0, v1);
    VERIFY_FAILS_END
}

/*
 * Note:
 * - The tests intentionally exercise both true and false branches of the
 *   underlying predicate: v0 < v1 should pass, while v0 < v1 when false
 *   should abort the test case.
 * - Pointer-based variants ensure that the macro correctly handles both
 *   direct values and dereferenced pointers, covering address vs. content
 *   comparisons as appropriate for Unity macros.
 */

// Unity-based test harness entry points
int main(void)
{
    // Begin Unity test run
    UNITY_BEGIN();

    // Run focal tests for testLessThanHEX16
    RUN_TEST(testLessThanHEX16_PositiveCase);
    RUN_TEST(testLessThanHEX16_NegativeCase_Abort);

    // End Unity test run
    return UNITY_END();
}