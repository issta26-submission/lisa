/***************************************************************
 * Test Suite: Focused tests around the focal method
 *              testNotEqualMemoryEachEqual3 (located in testunity.c)
 *
 * Rationale:
 * - The focal method uses Unity's TEST_ASSERT_EACH_EQUAL_MEMORY
 *   to verify that multiple memory blocks match an expected block.
 * - The specific testNotEqualMemoryEachEqual3 scenario intentionally
 *   mismatches one of the blocks, causing the test to abort.
 *
 * This standalone C++11 compatible test suite:
 * - Exposes the focal test via extern "C" so it can be linked with
 *   the existing testunity.c (which defines the focal test).
 * - Adds an additional pass-path test that exercises a matching
 *   memory scenario (three blocks equal to the expected block).
 * - Uses Unity's RUN_TEST mechanism to drive tests from main.
 *
 * Note:
 * - This file should be compiled and linked together with testunity.c.
 * - No GTest is used; Unity is the test framework as in the original project.
 ***************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// The focal test under testNotEqualMemoryEachEqual3 is defined in testunity.c.
// We declare it here so we can reference it from the Unity test runner.
void testNotEqualMemoryEachEqual3(void);
#ifdef __cplusplus
}
#endif

// Forward declaration for our additional pass-path test (not in testunity.c)
void testNotEqualMemoryEachEqual3_Pass(void);

// Setup/Teardown hooks (required by Unity)
void setUp(void) {}
void tearDown(void) {}

/*
 * Test 1: Replicate the focal scenario and exercise the exact failing case.
 * This test is the focal method itself as defined in testunity.c and is
 * expected to abort/fail due to the deliberate memory mismatch.
 * The behavior is exercised by RUN_TEST(testNotEqualMemoryEachEqual3) in main.
 * (No extra code required here since the focal test contains its own
 * EXPECT_ABORT_BEGIN/VERIFY_FAILS_END wrappers.)
 */

// Test 2: A pass-path variant for the same memory-equality scenario.
// This validates the positive path where the three blocks are exactly equal.
void testNotEqualMemoryEachEqual3_Pass(void)
{
    // Three consecutive blocks of 4 ints each, all equal to the first block.
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2, 1, 8, 987, -2, 1, 8, 987, -2};

    // This should pass without aborting.
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int)*4, 3);
}

/*
 * Main driver:
 * - Runs the focal test from testunity.c to ensure the exact negative case
 *   is exercised (abort occurs as expected).
 * - Runs our additional pass-path test to ensure the underlying macro works
 *   for a correct three-block scenario.
 */
int main(void)
{
    UNITY_BEGIN("test_not_equal_memory_each_equal3_suite_cpp.cpp");

    // Run the focal test defined in testunity.c (intentionally fails)
    RUN_TEST(testNotEqualMemoryEachEqual3);

    // Run a complementary test that should pass
    RUN_TEST(testNotEqualMemoryEachEqual3_Pass);

    return UNITY_END();
}