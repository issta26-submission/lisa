// Test suite for the focal method: testHEX8sWithinDeltaAndCustomMessage
// Step 1 (Conceptual): Candidate Keywords derived from the focal method
// - HEX8 within delta checks (TEST_ASSERT_HEX8_WITHIN_MESSAGE macro)
// - Custom message handling in assertions
// - 8-bit hex comparisons (254 vs 255, 251 vs 255, 1 vs 4 with delta 5)
// - Interaction with Unity test framework (setup/teardown, assertion macros)
// The following code provides a small Unity-based runner in C++11 that executes
// the focal test along with a few related HEX8-within-delta tests to increase coverage.

#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>



#ifdef __cplusplus
extern "C" {
#endif
// Forward declarations of the test functions located in testunity.c (C file).
// They must be linked together with this test suite.
void testHEX8sWithinDeltaAndCustomMessage(void);
void testHEX8sWithinDelta(void);
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits(void);
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void);
void testHEX8sNotWithinDelta(void);
void testHEX8sNotWithinDeltaAndCustomMessage(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Begin Unity test run. The test file name helps identify log output.
    UnityBegin("testunity.c (HEX8 within delta focused tests)");

    // Focal test under scrutiny.
    // Purpose: Verify that three HEX8 within-delta checks pass and emit a custom message.
    // This corresponds directly to the provided focal method.
    // Expected behavior: All assertions succeed, test passes.
    printf("Running focal test: testHEX8sWithinDeltaAndCustomMessage\n");
    RUN_TEST(testHEX8sWithinDeltaAndCustomMessage);

    // Additional HEX8 within-delta tests to expand coverage and exercise related paths.
    // These tests exercise within-delta scenarios without custom messages.
    printf("Running HEX8 within-delta test without custom message: testHEX8sWithinDelta\n");
    RUN_TEST(testHEX8sWithinDelta);

    printf("Running HEX8 within-delta with out-of-range delta path: testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits\n");
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits);

    printf("Running HEX8 within-delta with out-of-range delta and custom message: testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage\n");
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage);

    // Negative test scenarios to ensure failures are captured (wrapped to avoid terminating the test suite).
    printf("Running HEX8 not within delta: testHEX8sNotWithinDelta\n");
    RUN_TEST(testHEX8sNotWithinDelta);

    printf("Running HEX8 not within delta with custom message: testHEX8sNotWithinDeltaAndCustomMessage\n");
    RUN_TEST(testHEX8sNotWithinDeltaAndCustomMessage);

    // End Unity test run and report results.
    return UnityEnd();
}