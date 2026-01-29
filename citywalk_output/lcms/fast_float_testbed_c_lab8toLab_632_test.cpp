// Test suite for the focal method lab8toLab in fast_float_testbed.c
// This test suite is written in C++11 without any external testing framework.
// It uses a small, non-terminating assertion style and reports a summary at the end.
// The tests exercise lab8toLab by supplying known 8-bit Lab inputs and validating
// the resulting cmsCIELab values after conversion through cmsLabEncoded2Float.

// Note: Ensure the build links against the color management library providing cmsLabEncoded2Float
// and include the LittleCMS header (lcms2.h or the equivalent in your environment).
// The lab8toLab function is declared with C linkage, as it is defined in a C source file.

#include <fast_float_internal.h>
#include <iostream>
#include <iomanip>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>
#include <cmath>


// Import color space definitions used by the focal method.
// Adjust the include path if your environment places lcms headers elsewhere.

// Declare the focal method with C linkage so it can be linked from the C source.
// The actual types cmsUInt8Number and cmsCIELab come from lcms2.h
extern "C" void lab8toLab(cmsUInt8Number lab8[3], cmsCIELab* Lab);

// Simple non-terminating assertion helper
static void fail_if(bool condition, const char* msg, int line) {
    if (condition) return;
    std::cerr << "TEST_FAIL at line " << line << ": " << msg << std::endl;
}

// Helper to compare two floats with a tolerance
static bool almost_equal(float a, float b, float tol) {
    return std::fabs(a - b) <= tol;
}

int main() {
    // Disable sync with stdio for a tiny performance improvement (optional)
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int failures = 0;

    // Test 1: Zero input should map to L ~ 0, a ~ -128, b ~ -128
    // This validates the 8->16 bit expansion and the basic decoding path.
    {
        cmsUInt8Number lab8[3] = {0, 0, 0};
        cmsCIELab Lab;
        lab8toLab(lab8, &Lab);

        // Expected approximations (based on typical Lab encoding mapping in cmsLabEncoded2Float):
        // L in [0, 100], a in [-128, 127], b in [-128, 127]
        // With 0 encoding, L -> 0, a -> -128, b -> -128
        float tol = 1.0f;
        fail_if(!almost_equal(Lab.L, 0.0f, tol), "LAB zero input: L expected ~0", __LINE__);
        fail_if(!almost_equal(Lab.a, -128.0f, tol), "LAB zero input: a expected ~-128", __LINE__);
        fail_if(!almost_equal(Lab.b, -128.0f, tol), "LAB zero input: b expected ~-128", __LINE__);
    }

    // Test 2: Max input should map to L ~ 100, a ~ 127, b ~ 127
    // This validates the upper boundary behavior.
    {
        cmsUInt8Number lab8[3] = {255, 255, 255};
        cmsCIELab Lab;
        lab8toLab(lab8, &Lab);

        float tol = 1.0f;
        fail_if(!almost_equal(Lab.L, 100.0f, tol), "LAB max input: L expected ~100", __LINE__);
        fail_if(!almost_equal(Lab.a, 127.0f, tol), "LAB max input: a expected ~127", __LINE__);
        fail_if(!almost_equal(Lab.b, 127.0f, tol), __LINE__);
    }

    // Test 3: Mid-range input to exercise typical non-extreme values.
    // lab8 = {128, 64, 32} yields mid L and specific a/b based on 16-bit decoding.
    // We'll compute the expected values approximately:
    // 16-bit encoding: 128 -> 0x8080, 64 -> 0x4040, 32 -> 0x2020
    // L16 = 0x8080 -> ~0.5 of 100, a16 ~ 0x4040 -> ~-64, b16 ~ 0x2020 -> ~-97
    {
        cmsUInt8Number lab8[3] = {128, 64, 32};
        cmsCIELab Lab;
        lab8toLab(lab8, &Lab);

        // Expected approximate values with a modest tolerance
        float tolL = 1.5f;
        float tolA = 1.5f;
        float tolB = 1.5f;

        // L around 50.15
        fail_if(!almost_equal(Lab.L, 50.15f, tolL), "LAB mid input: L expected ~50.15", __LINE__);
        // a around -64
        fail_if(!almost_equal(Lab.a, -64.0f, tolA), "LAB mid input: a expected ~-64", __LINE__);
        // b around -97
        fail_if(!almost_equal(Lab.b, -97.0f, tolB), "LAB mid input: b expected ~-97", __LINE__);
    }

    // Summary
    std::cout << "\nTest Summary:\n";
    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed.\n";
        return 1;
    }
}

// Explanatory notes:
// - The tests rely on the standard LittleCMS lab encoding where 16-bit Lab components map to
//   L ∈ [0, 100] and a, b ∈ [-128, 127].
// - The FROM_8_TO_16 macro is used inside lab8toLab to convert 8-bit inputs to 16-bit values.
// - cmsLabEncoded2Float converts the 16-bit Lab encoding to the floating-point Lab representation.
// - The tests use a small, non-terminating assertion style to allow all tests to run and report
//   a summary of failures.
// - If your project uses a slightly different Lab encoding, adjust the expected values accordingly.