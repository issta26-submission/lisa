/*
Unit test suite for the focal method:
cmsINLINE cmsUInt8Number _cmsQuickSaturateByte(cmsFloat64Number d)

Summary of functionality:
- Adds 0.5 to d
- If the adjusted value <= 0, returns 0
- If the adjusted value >= 255, returns 255
- Otherwise returns floor(adjusted_value) cast to cmsUInt8Number via _cmsQuickFloorWord(d)

This test suite is implemented in plain C++11 (no Google Test) and uses a small
in-house assertion mechanism that continues execution after failures to maximize
code coverage. It imports dependencies via the project header(s) and calls the
function directly from main as required when gtest is not allowed.
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>


// Step 1: Import necessary dependencies with correct paths.
// We bring in lcms internal declarations so cmsUInt8Number, cmsFloat64Number and
// _cmsQuickSaturateByte (and its helper _cmsQuickFloorWord) are visible.
// The tests rely on the behaviour defined in the focal method above.
extern "C" {
}

// Lightweight test harness (no external testing framework)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple assertion macro that records failures but continues execution.
// It prints helpful information to stdout for debugging.
#define ASSERT_UINT8_EQ(actual, expected) do { \
    ++g_total_tests; \
    if (static_cast<uint8_t>(actual) != static_cast<uint8_t>(expected)) { \
        printf("Test failure at %s:%d: expected %u, got %u\n", __FILE__, __LINE__, \
               static_cast<unsigned>(expected), static_cast<unsigned>(static_cast<uint8_t>(actual))); \
        ++g_failed_tests; \
    } \
} while (0)

// Step 3: Test Cases

// Test 1: Negative numbers should saturate to 0 after adding 0.5 and checks.
// Covers the d <= 0 branch after the addition.
static void test_negative_and_zero_boundaries() {
    // d = -1.0 -> -1.0 + 0.5 = -0.5 <= 0  -> 0
    cmsUInt8Number r1 = _cmsQuickSaturateByte(-1.0);
    ASSERT_UINT8_EQ(r1, 0);

    // d = -0.5 -> -0.5 + 0.5 = 0.0 <= 0  -> 0
    cmsUInt8Number r2 = _cmsQuickSaturateByte(-0.5);
    ASSERT_UINT8_EQ(r2, 0);

    // d = -0.0001 -> -0.0001 + 0.5 = 0.4999 > 0 and < 255 -> floor(0.4999) = 0
    cmsUInt8Number r3 = _cmsQuickSaturateByte(-0.0001);
    ASSERT_UINT8_EQ(r3, 0);
}

// Test 2: Values near zero and small positives to exercise floor path.
// Covers the else-branch where only floor is applied after adding 0.5.
static void test_small_positive_values() {
    // d = 0.0 -> 0.0 + 0.5 = 0.5; floor(0.5) = 0
    cmsUInt8Number r1 = _cmsQuickSaturateByte(0.0);
    ASSERT_UINT8_EQ(r1, 0);

    // d = 0.6 -> 0.6 + 0.5 = 1.1; floor(1.1) = 1
    cmsUInt8Number r2 = _cmsQuickSaturateByte(0.6);
    ASSERT_UINT8_EQ(r2, 1);
}

// Test 3: Boundary and saturation checks around 254.5 to ensure correct clamping.
// - d = 254.4 -> -> 254.4 + 0.5 = 254.9 -> floor(254.9) = 254
// - d = 254.5 -> -> 254.5 + 0.5 = 255.0 >= 255 -> saturation to 255
// - d = 255.0 -> -> 255.0 + 0.5 = 255.5 >= 255 -> saturation to 255
static void test_upper_boundaries_and_saturation() {
    // d = 254.4 -> expect 254
    cmsUInt8Number r1 = _cmsQuickSaturateByte(254.4);
    ASSERT_UINT8_EQ(r1, 254);

    // d = 254.5 -> saturate to 255
    cmsUInt8Number r2 = _cmsQuickSaturateByte(254.5);
    ASSERT_UINT8_EQ(r2, 255);

    // d = 255.0 -> saturate to 255
    cmsUInt8Number r3 = _cmsQuickSaturateByte(255.0);
    ASSERT_UINT8_EQ(r3, 255);

    // d = 1000.0 -> saturate to 255
    cmsUInt8Number r4 = _cmsQuickSaturateByte(1000.0);
    ASSERT_UINT8_EQ(r4, 255);
}

// Step 2: Additional test to ensure some mid-range values behave as floor(d+0.5)
static void test_mid_range_values() {
    // d = 0.4 -> 0.4 + 0.5 = 0.9; floor(0.9) = 0
    cmsUInt8Number r1 = _cmsQuickSaturateByte(0.4);
    ASSERT_UINT8_EQ(r1, 0);

    // d = 0.7 -> 0.7 + 0.5 = 1.2; floor(1.2) = 1
    cmsUInt8Number r2 = _cmsQuickSaturateByte(0.7);
    ASSERT_UINT8_EQ(r2, 1);
}

int main() {
    // Run all tests in sequence
    test_negative_and_zero_boundaries();
    test_small_positive_values();
    test_upper_boundaries_and_saturation();
    test_mid_range_values();

    // Summary
    printf("CMS Quick Saturate Byte tests completed. Ran %d tests, %d failures.\n",
           g_total_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}