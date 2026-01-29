// This test module targets the focal method/testHEX8sWithinDelta present in testunity.c.
// It uses the Unity framework (Unity.h) to exercise HEX8 within-delta assertions.
// The tests are written in C++, conforming to a C/C++ mixed project setup (no GTest).
// Notes for reviewers:
// - We focus on exercising true and false branches of the within-delta predicate.
// - We rely on existing Unity macros (e.g., TEST_ASSERT_HEX8_WITHIN, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END).
// - The tests are self-contained and can be compiled alongside the current Unity test suite.
// - Static members and file-scope statics from the focal file are not redefined here; we only invoke the assertion macros.

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
}
#endif

// Lightweight setup/teardown to satisfy Unity's expectations when running standalone tests.
void setUp(void)   {}
void tearDown(void) {}

// Candidate Keywords extracted from the focal method (Step 1):
// - HEX8 within-delta assertion: TEST_ASSERT_HEX8_WITHIN
// - Delta (tolerance) parameter
// - Expected and actual HEX8 values
// - Failure assertion wrappers: EXPECT_ABORT_BEGIN / VERIFY_FAILS_END

// Test 1: Basic positive case coverage
// This mirrors the focal method's first three assertions which should all pass.
// We combine them into a single test to ensure multiple passes in sequence.
void testHEX8sWithinDelta_BasicPasses(void)
{
    // True branches: each within-delta check should pass without aborting.
    // 1) 254 within delta 1 of 255
    TEST_ASSERT_HEX8_WITHIN(1, 254, 255);
    // 2) 251 within delta 5 of 255
    TEST_ASSERT_HEX8_WITHIN(5, 251, 255);
    // 3) 1 within delta 5 of 4
    TEST_ASSERT_HEX8_WITHIN(5, 1, 4);
}

// Test 2: Boundary condition where difference equals delta
// Verifies that the predicate treats the edge case as within tolerance.
void testHEX8sWithinDelta_BoundaryEqualsDelta(void)
{
    // 0xFE and 0xFF differ by exactly 1, which is within delta of 1.
    TEST_ASSERT_HEX8_WITHIN(1, 0xFE, 0xFF);

    // Additional boundary: identical values should pass with any delta (including 0)
    TEST_ASSERT_HEX8_WITHIN(0, 0xAB, 0xAB);
}

// Test 3: Negative path coverage (assertion should fail when outside delta)
// We expect Unity to abort the current test when the condition is not met.
// This ensures the negative (false) branch is exercised.
void testHEX8sWithinDelta_NotWithinDelta_ShouldFail(void)
{
    // Use a delta that is too small to cover the difference (0xFF vs 0x00 -> diff = 255)
    // We wrap with EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to capture the failure without stopping the test runner.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(1, 0xFF, 0x00);
    VERIFY_FAILS_END
}

// Optional: Additional negative test to ensure the macro handles large deltas gracefully
// by still requiring the absolute difference to be <= delta.
void testHEX8sWithinDelta_NotWithinDelta_LargeDifference(void)
{
    // delta too small for large difference; expect failure
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(10, 0xA0, 0xFF); // diff = 31, within 10 is false -> should fail
    VERIFY_FAILS_END
}

// Main runner for this focused test suite.
// This can be compiled and linked with Unity to run only these tests.
// If the project uses a shared runner, these RUN_TEST calls can be integrated there instead.
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testHEX8sWithinDelta_BasicPasses);
    RUN_TEST(testHEX8sWithinDelta_BoundaryEqualsDelta);
    RUN_TEST(testHEX8sWithinDelta_NotWithinDelta_ShouldFail);
    RUN_TEST(testHEX8sWithinDelta_NotWithinDelta_LargeDifference);

    return UNITY_END();
}