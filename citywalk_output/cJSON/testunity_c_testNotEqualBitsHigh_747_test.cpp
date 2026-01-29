/*
Candidate Keywords and planning notes:
- TEST_ASSERT_BITS_HIGH: Unity assertion under testNotEqualBitsHigh
- 32-bit values: uint32_t (UNITY_UINT32 equivalent usage in test)
- 0xFF55AA00, 0x55550000: known failing high-bits scenario
- 0xABCD1234, 0xABCD0001: passing high-bits scenario (same high 16 bits)
- EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: capture expected aborts for negative tests
- UnityBegin / RUN_TEST / UnityEnd: test harness orchestration
- Purpose: provide comprehensive coverage for testNotEqualBitsHigh by including both a failing case (assertion aborts) and a passing case (no abort)
- No GTest: pure Unity-based tests, self-contained main
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Optional: provide a minimal setup/teardown (not required for these tests)
void setUp(void) {}
void tearDown(void) {}

/*
  Test: testNotEqualBitsHigh_FailCase
  Purpose:
  - Demonstrates that testNotEqualBitsHigh correctly detects non-equal high bits
  - Uses inputs where the high 16 bits differ, so TEST_ASSERT_BITS_HIGH should fail and abort
  Behavior:
  - Expected to abort, captured by EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
*/
void testNotEqualBitsHigh_FailCase(void)
{
    uint32_t v0 = 0xFF55AA00;
    uint32_t v1 = 0x55550000;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_HIGH(v0, v1);
    VERIFY_FAILS_END
}

/*
  Test: testNotEqualBitsHigh_PassCase
  Purpose:
  - Verifies the non-aborting path when high bits are equal
  - High 16 bits of both values are 0xABCD, lower bits may differ
  Behavior:
  - Should pass without abort
*/
void testNotEqualBitsHigh_PassCase(void)
{
    uint32_t v0 = 0xABCD1234;
    uint32_t v1 = 0xABCD0001;
    // This should pass since the high bits (0xABCD) are equal
    TEST_ASSERT_BITS_HIGH(v0, v1);
}

/* Additional test to explicitly ensure another fail case (alternative inputs)
   This reinforces that differing high bits cause aborts consistently. */
void testNotEqualBitsHigh_AnotherFailCase(void)
{
    uint32_t v0 = 0x12345678;
    uint32_t v1 = 0x92345678; // differing in high nibble (0x1 vs 0x9)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_HIGH(v0, v1);
    VERIFY_FAILS_END
}

/* Minimal test runner for the two primary scenarios (pass and fail) */
int main(void)
{
    UnityBegin("testNotEqualBitsHigh_suite.c");

    // Failing case: should abort
    RUN_TEST(testNotEqualBitsHigh_FailCase, __LINE__);

    // Passing case: should not abort
    RUN_TEST(testNotEqualBitsHigh_PassCase, __LINE__);

    // Additional fail case to demonstrate robustness (non-deterministic environments)
    RUN_TEST(testNotEqualBitsHigh_AnotherFailCase, __LINE__);

    return UnityEnd();
}