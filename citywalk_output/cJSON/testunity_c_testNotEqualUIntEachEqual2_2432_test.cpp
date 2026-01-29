/***************
 * Test Suite: Focused coverage for the focal method testNotEqualUIntEachEqual2
 * Context: Uses the Unity testing framework (C) from testunity.c
 * Approach: Provide a dedicated C++11 runner that invokes the focal test
 *           and an additional positive-path test to exercise the true branch
 *           of TEST_ASSERT_EACH_EQUAL_UINT without aborting.
 * Note: The focal test is defined in testunity.c as:
 *       void testNotEqualUIntEachEqual2(void) { unsigned int p0[] = {987, 8, 987, 987}; EXPECT_ABORT_BEGIN TEST_ASSERT_EACH_EQUAL_UINT(987, p0, 4); VERIFY_FAILS_END }
 ***************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declaration of the focal test from testunity.c
void testNotEqualUIntEachEqual2(void);
#ifdef __cplusplus
}
#endif

// Positive-path test to exercise the true branch of TEST_ASSERT_EACH_EQUAL_UINT
// This test ensures that when all elements are equal to the expected value,
// TEST_ASSERT_EACH_EQUAL_UINT behaves as a non-aborting assertion (i.e., test passes).
void test_EachEqualUInt_Passes(void)
{
    // Candidate Keywords/Setup:
    // - Array of unsigned int with all elements equal to the expected value (987)
    // - Macro under test: TEST_ASSERT_EACH_EQUAL_UINT(expected, array, length)
    // - Purpose: validate the "all match" path (true branch) of the macro
    unsigned int p0[] = {987, 987, 987, 987};
    // This should pass without aborting since all elements match the expected value
    TEST_ASSERT_EACH_EQUAL_UINT(987, p0, 4);
}

// Minor helper: ensure a clear description when the suite is run
int main(void)
{
    // Name of the suite for reporting purposes
    UnityBegin("testNotEqualUIntEachEqual2_suite.cpp");

    // Validate the focal negative-case behavior (the original testNotEqualUIntEachEqual2)
    // This test is expected to abort (fail), and Unity will catch the abort and continue
    RUN_TEST(testNotEqualUIntEachEqual2);

    // Validate the positive-path behavior of TEST_ASSERT_EACH_EQUAL_UINT
    // This ensures the true-branch of the macro is exercised without abort
    RUN_TEST(test_EachEqualUInt_Passes);

    return UnityEnd();
}