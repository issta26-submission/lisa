// test_focal_suite.cpp
// A dedicated test suite to exercise the focal Unity test: 
// testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage
// and surrounding scenarios for 8-bit integer within-delta checks.
// This suite uses Unity (C) in a C++11 project without GTest.

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Forward declaration of the focal test function located in testunity.c.
// It is a C function; disable name mangling for C linkage.
extern "C" void testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void);

// Wrapper test that directly invokes the focal test function.
// This ensures the focal test is executed as part of this separate test suite.
void test_integration_focal_wrapped(void)
{
    // Call the focal test directly. Any assertion failures inside will be
    // reported by Unity as a failure of this wrapper test.
    testINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage();
}

// Additional tests to improve coverage around 8-bit within-delta behavior.

// Test that an 8-bit within-delta assertion passes when difference is within delta,
// using a case that exercises edge values like negative numbers (sign handling).
void test_Int8WithinDeltaEdgeCasePass(void)
{
    // delta = 6, values are -128 and -122.
    // Casting to int8_t ensures 8-bit behavior is exercised.
    TEST_ASSERT_INT8_WITHIN_MESSAGE(6, (int8_t) -128, (int8_t) -122, "Edge case pass: within delta with negative values.");
}

// Test that an 8-bit within-delta assertion fails when difference exceeds delta.
// Using clearly outside the 8-bit delta range with distinct low-level values.
void test_Int8WithinDeltaEdgeCaseFail(void)
{
    // delta = 5, values are 0x10 and 0x20 (16 and 32). Difference is 16 > 5.
    // Expect the assertion to fail and Unity to report a failure.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN(5, (int8_t)0x10, (int8_t)0x20);
    VERIFY_FAILS_END
}

// Main runner for this test suite.
// It executes:
// - the focal test via a wrapper
// - a passing edge-case test
// - a failing edge-case test
int main(void)
{
    UnityBegin("test_focal_suite.cpp");

    // Run the focal test wrapper to ensure the focal path is exercised.
    RUN_TEST(test_integration_focal_wrapped);

    // Run additional coverage tests for 8-bit within-delta behavior.
    RUN_TEST(test_Int8WithinDeltaEdgeCasePass);
    RUN_TEST(test_Int8WithinDeltaEdgeCaseFail);

    return UnityEnd();
}