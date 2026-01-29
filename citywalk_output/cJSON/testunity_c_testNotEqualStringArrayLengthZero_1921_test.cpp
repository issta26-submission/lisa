// NotEqualStringArrayLengthZeroSuite.cpp
// A lightweight C++11 test harness that targets the focal testNotEqualStringArrayLengthZero
// from testunity.c (Unity-based unit tests).
// This suite uses Unity (no GTest) and is compiled as C++11-compatible code.
// It includes additional tests to exercise both passing and aborting paths for
// string-array equality with zero length.

#include <cstddef>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Ensure Unity's C API is accessible from C++ code
extern "C" {
}

// Forward declaration of the focal test (defined in testunity.c)
extern "C" void testNotEqualStringArrayLengthZero(void);

// Lightweight setUp/tearDown for Unity
extern "C" void setUp(void) { /* no setup required for these tests */ }
extern "C" void tearDown(void) { /* no teardown required for these tests */ }

/*
 * Test 1: Pass case for zero-length string array comparison.
 * 
 * Rationale:
 * - The focal test uses a zero-length comparison (length = 0).
 * - When expStrings and testStrings are compatible for length 0, TEST_ASSERT_EQUAL_STRING_ARRAY
 *   should not access the arrays and should pass. This verifies the non-aborting path for
 *   the zero-length scenario.
 */
void testNotEqualStringArrayLengthZero_Pass(void)
{
    const char *testStrings[] = { NULL };  // actual strings (single NULL entry in this test)
    const char **expStrings = NULL;        // expected strings (NULL pointer)
    
    // Expect no abort and the assertion to pass for length 0
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 0);
}

/*
 * Test 2: Abort path replication for the focal zero-length test.
 * 
 * Rationale:
 * - The original focal test encloses the assertion in EXPECT_ABORT_BEGIN/VERIFY_FAILS_END,
 *   indicating the test should abort on failure. Reproducing this path confirms that
 *   the Unity harness correctly captures and reports the abort when the condition is not met.
 */
void testNotEqualStringArrayLengthZero_AbortInTest(void)
{
    const char *testStrings[] = { NULL };
    const char **expStrings = NULL;
    
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 0);
    VERIFY_FAILS_END
}

/*
 * Test 3: Reuse the focal test to verify the abort path directly from the original
 *         test function in testunity.c. This ensures the exact same behavior is exercised.
 * 
 * Rationale:
 * - By invoking the focal test function within the Unity runner, we exercise the same
 *   abort scenario as the original test, providing redundancy for coverage.
 * - This test simply delegates; it documents and confirms the exact focal behavior.
 */
void testNotEqualStringArrayLengthZero_InvokeFocal(void)
{
    // The focal test is designed to abort; wrap in EXPECT_ABORT to confirm behavior.
    EXPECT_ABORT_BEGIN
    testNotEqualStringArrayLengthZero();
    VERIFY_FAILS_END
}

// Main driver to run the tests.
// This assumes testunity.c (Unity-based tests) is linked in the build.
int main(void)
{
    UNITY_BEGIN();

    // Run the focal test via the Unity test harness
    RUN_TEST(testNotEqualStringArrayLengthZero);

    // Additional tests to exercise both success and abort scenarios
    RUN_TEST(testNotEqualStringArrayLengthZero_Pass);
    RUN_TEST(testNotEqualStringArrayLengthZero_AbortInTest);
    RUN_TEST(testNotEqualStringArrayLengthZero_InvokeFocal);

    return UNITY_END();
}