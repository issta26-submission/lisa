// Test suite for focal method: testHEX16sWithinDeltaAndCustomMessage
// This suite extends the existing Unity-based tests without using GTest.
// It demonstrates additional coverage around HEX16 within-delta assertions
// and exercises the focal test via a wrapper as well as direct edge cases.

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Bring in Unity C framework with C linkage for compatibility with C++
extern "C" {
}

// Forward declaration of the focal test function defined in testunity.c
// This is a test function provided by the focal file and uses Unity macros internally.
extern void testHEX16sWithinDeltaAndCustomMessage(void);

// Wrapper to allow running the focal test as a separate Unity test in C++
void testHEX16sWithinDeltaAndCustomMessageWrapper(void)
{
    // This wrapper delegates to the focal test which contains three assertions
    // that verify HEX16 within-delta behavior with a custom message.
    testHEX16sWithinDeltaAndCustomMessage();
}

// Direct edge-case test leveraging Unity macros to validate HEX16 within-delta
void testHEX16sWithinDeltaEdgeCasesDirect(void)
{
    // Purpose: verify within-delta passes for a boundary case (delta = 1)
    // expected = 0x7FFF, actual = 0x8000 -> difference is exactly 1
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(1, 0x7FFF, 0x8000, "Direct edge within delta.");

    // Additional within-delta case to ensure typical internal delta behavior works
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(2, 0x1234, 0x1236, "Direct edge within delta with higher delta.");

    // If we want to cover another within-edge scenario, uncomment or add more as needed.
}

// Negative edge-case test to ensure the within-delta assertion fails appropriately
void testHEX16sWithinDeltaEdgeCasesDirectNotWithin(void)
{
    // Purpose: validate that a value outside the delta correctly aborts the test
    // delta = 1, expected 0x7FFF, actual 0x8001 -> difference is 2 (not within)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(1, 0x7FFF, 0x8001, "Direct edge not within delta should fail.");
    VERIFY_FAILS_END
}

// Minimal test runner invoking the above tests
int main(void)
{
    // Initialize the Unity test framework
    UnityBegin("test_hex16_within_delta_suite.cpp");

    // Run: focal function wrapper
    RUN_TEST(testHEX16sWithinDeltaAndCustomMessageWrapper); // executes focal test via wrapper

    // Run: direct edge-case tests
    RUN_TEST(testHEX16sWithinDeltaEdgeCasesDirect);
    RUN_TEST(testHEX16sWithinDeltaEdgeCasesDirectNotWithin);

    // Finalize and return test results
    return UnityEnd();
}