// Test suite for cmsUInt16Number ab2Fix4(cmsFloat64Number ab)
// Focus: verify saturating behavior of (ab + 128.0) * 257.0 and clamping to [0, 65535]
// This test relies on the implementation in cmspcs.c and lcms2_internal.h.
// Compile by linking with cmspcs.c (and any required lcms sources).

#include <lcms2_internal.h>
#include <iostream>
#include <string>


// Declaration with C linkage to match the focal function in cmspcs.c
extern "C" cmsUInt16Number ab2Fix4(cmsFloat64Number ab);

// Simple, non-terminating test harness (no GTest/GMock; just plain C++11)
static int g_failures = 0;

static void report(const std::string& test_name, cmsUInt16Number got, cmsUInt16Number expected, bool pass)
{
    if (pass) {
        std::cout << "[PASS] " << test_name << " | got: " << got << std::endl;
    } else {
        std::cout << "[FAIL] " << test_name << " | expected: " << expected << ", got: " << got << std::endl;
        ++g_failures;
    }
}

static void run_test(const std::string& name, cmsFloat64Number input, cmsUInt16Number expected)
{
    cmsUInt16Number result = ab2Fix4(input);
    bool pass = (result == expected);
    report(name, result, expected, pass);
}

int main()
{
    // Test 1: Input well below -128 -> saturates to 0
    // ab = -200 -> (ab+128) * 257 = (-72) * 257 < 0 -> saturate to 0
    run_test("ab2Fix4_negative_below_min", -200.0, 0);

    // Test 2: Input exactly -128 -> (0) * 257 = 0 -> saturate to 0
    run_test("ab2Fix4_at_min_boundary", -128.0, 0);

    // Test 3: Input just above min boundary: -127 -> (1) * 257 = 257
    run_test("ab2Fix4_min_plus_one", -127.0, 257);

    // Test 4: Moderate positive value within range: 0 -> (128) * 257 = 32896
    run_test("ab2Fix4_zero_input", 0.0, 32896);

    // Test 5: Boundary that exactly fills max (65535) with integer ab:
    // ab = 127 -> (255) * 257 = 65535
    run_test("ab2Fix4_boundary_to_max", 127.0, 65535);

    // Test 6: Positive input that would exceed max: ab = 128 -> (256) * 257 = 65792 -> saturate to 65535
    run_test("ab2Fix4_exceeding_max", 128.0, 65535);

    // Test 7: Large positive input far above max: ab = 1000 -> saturate to 65535
    run_test("ab2Fix4_large_positive", 1000.0, 65535);

    // Test 8: Negative input well below min: ab = -256 -> saturate to 0
    run_test("ab2Fix4_large_negative", -256.0, 0);

    // Test 9: Positive value that also saturates: ab = 256 -> (384) * 257 = 98688 -> saturate to 65535
    run_test("ab2Fix4_another_saturation", 256.0, 65535);

    // Summary
    if (g_failures == 0) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[SUMMARY] Failures: " << g_failures << std::endl;
        return 1;
    }
}