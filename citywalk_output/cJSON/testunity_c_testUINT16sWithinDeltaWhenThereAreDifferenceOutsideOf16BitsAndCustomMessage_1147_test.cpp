/*
Candidate Keywords (core dependencies for the focal method):
- TEST_ASSERT_UINT16_WITHIN_MESSAGE
- 5 (delta)
- 0x54321 and 0x44321 (values used to exercise 16-bit wrap behavior)
- "Custom Message." (custom message variant)
- UINT16 data type and related Unity macros
This test suite focuses on the focal testUINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage
and provides an additional secondary test to exercise the same assertion in a separate test case.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// The focal test under testunity.c
void testUINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage(void);
#ifdef __cplusplus
}
#endif

// Local duplicate to further exercise the same assertion using a separate test case.
// This helps increase coverage by executing a second, independent test path
// that uses the same core condition (wrap-around delta with 0x54321 vs 0x44321).
static void testUINT16sWithinDelta_CustomMessageAlternative(void)
{
    // Recreate the focal assertion in a separate test to ensure independent
    // execution and reporting within Unity's framework.
    // This uses the same values as the focal test to validate the wrap behavior.
    TEST_ASSERT_UINT16_WITHIN_MESSAGE(5, 0x54321, 0x44321, "Custom Message.");
}

// Main entry point for the standalone test suite (C++11 compatible).
// This file is intended to be compiled and linked with the existing Unity framework
// and testunity.c so that RUN_TEST can invoke the focal test defined there.
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("test_suite_uint16_within_delta_custom_message.cpp");

    // Run the focal test from testunity.c
    RUN_TEST(testUINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage);

    // Run an additional, independent test that duplicates the focal condition
    // to ensure the assertion path is exercised in a separate test case.
    RUN_TEST(testUINT16sWithinDelta_CustomMessageAlternative);

    // Return the test run results
    return UnityEnd();
}

/*
Notes:
- This test suite relies on the existing Unity test environment used by testunity.c.
- The focal test verifies that within delta logic for 16-bit comparisons handles
  differences that exceed 16 bits by wrapping and still being within the specified delta.
- The additional test function testUINT16sWithinDelta_CustomMessageAlternative duplicates
  the focal case to provide a separate, independently executed path for the same scenario.
- No private methods or private state access is required; tests rely solely on public
  Unity macros and the focal test function defined in testunity.c.
*/