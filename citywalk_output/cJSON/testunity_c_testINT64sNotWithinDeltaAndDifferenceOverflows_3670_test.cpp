// A focused Unity-based test suite in C++11 for the focal method behavior
// testINT64sNotWithinDeltaAndDifferenceOverflows
// This test exercises the 64-bit overflow path when performing
// TEST_ASSERT_INT64_WITHIN with values that cause signed 64-bit overflow in the delta computation.
//
// Notes:
// - The test is guarded with UNITY_SUPPORT_64 to avoid compile-time issues when 64-bit support is absent.
// - When 64-bit is not supported, the test should be ignored (TEST_IGNORE()).
// - The test uses 64-bit literals cast to int64_t to ensure correct typing across compilers.

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


extern "C" {
}

// Lightweight setup/teardown to satisfy Unity's expectations
static void setUp(void)  { /* no setup required for this test */ }
static void tearDown(void) { /* no teardown required for this test */ }

// Test 1: Directly verify that within-delta int64 comparison overflows are handled as a failure
// This mirrors the focal function's inner logic for 64-bit environments.
// If UNITY_SUPPORT_64 is defined, this should abort (fail) as the delta calculation overflows.
// If UNITY_SUPPORT_64 is not defined, the test is ignored.
void testINT64sNotWithinDeltaAndDifferenceOverflows_Direct(void)
{
#ifndef UNITY_SUPPORT_64
    // 64-bit support is not available; skip this test gracefully.
    TEST_IGNORE();
#else
    // Expect the comparison to abort due to overflow in delta calculation.
    EXPECT_ABORT_BEGIN
    // Use explicit 64-bit literals and cast to ensure proper typing.
    int64_t expected = (int64_t)0x8000000000000000ULL; // LLONG_MIN
    int64_t actual   = (int64_t)0x7FFFFFFFFFFFFFFFULL; // LLONG_MAX
    TEST_ASSERT_INT64_WITHIN(1, expected, actual);
    VERIFY_FAILS_END
#endif
}

// Test 2: Guard test for environments without 64-bit support (placeholder/coverage)
 // This test ensures the suite compiles in both configurations.
// If 64-bit is not supported, it should be ignored here as well.
// If 64-bit is supported, this test serves as a placeholder to ensure coverage wiring.
void testINT64sNotWithinDeltaAndDifferenceOverflows_NotSupported(void)
{
#ifndef UNITY_SUPPORT_64
    // 64-bit support is not available; explicitly mark as ignored.
    TEST_IGNORE();
#else
    // 64-bit is available; provide a non-operational placeholder to keep suite structure intact.
    TEST_IGNORE_MESSAGE("64-bit path present; this test acts as a placeholder for coverage completion.");
#endif
}

// Entry point to run the tests
int main(void)
{
    UNITY_BEGIN();

    // Register tests
    RUN_TEST(testINT64sNotWithinDeltaAndDifferenceOverflows_Direct);
    RUN_TEST(testINT64sNotWithinDeltaAndDifferenceOverflows_NotSupported);

    return UNITY_END();
}