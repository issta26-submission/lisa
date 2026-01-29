// This C++11 test wrapper exercises the focal Unity-based test method
//: testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits
// It follows the project guidelines: no Google Test, uses Unity-style macros,
// and provides explanatory comments for each unit test.
// Step 1: Candidate Keywords extracted from the focal method
// - testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits
// - TEST_ASSERT_INT8_WITHIN
// - delta = 5
// - values: 0x123 (int8 cast), 0xF23 (int8 cast)

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// The focal test is defined in testunity.c as a C function.
// We declare it here for linking with the C/C++ test wrapper.
extern "C" void testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits(void);

/*
  Test wrapper 1: Run the focal test function directly.
  This exercises the exact focal unit test as it is implemented in testunity.c
  and validates that its internal assertions pass under the Unity framework.
*/
static void testFocalMethodWrapped(void)
{
    // Call the focal test directly
    testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits();
}

/*
  Test wrapper 2: Re-implement the same assertion logic directly in C++ using Unity.
  This provides additional coverage for the positive path where the int8 values
  are within the specified delta (after casting to int8).
*/
static void testDirectInt8WithinDeltaPass(void)
{
    // Within delta: cast to int8 to simulate Unity's int8 comparison
    { TEST_ASSERT_INT8_WITHIN(5, (int8_t)0x123, (int8_t)0xF23); }
}

/*
  Test wrapper 3: Ensure the assertion properly fails when the delta is exceeded.
  We wrap the assertion with EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to catch
  the abort and keep the test suite running (non-terminating assertion style).
*/
static void testDirectInt8WithinDeltaFail(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN(5, (int8_t)0x7F, (int8_t)0x80);
    VERIFY_FAILS_END
}

/*
  Step 3: Main runner
  - Executes three tests:
    1) The focal method under test (as implemented in testunity.c)
    2) A direct, explicit positive case exercising the same assertion
    3) A direct negative case ensuring proper failure behavior
*/
int main(void)
{
    UNITY_BEGIN("testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_suite");

    // Test 1: Run the focal method
    RUN_TEST(testFocalMethodWrapped);

    // Test 2: Direct positive path equivalent assertion
    RUN_TEST(testDirectInt8WithinDeltaPass);

    // Test 3: Direct negative path to verify failure handling
    RUN_TEST(testDirectInt8WithinDeltaFail);

    return UNITY_END();
}