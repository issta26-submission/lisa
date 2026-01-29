// Test suite for the focal method fromHLFtoFLT in cmsalpha.c
// This test is designed to be compiled alongside cmsalpha.c in a C++11 project
// without using GoogleTest. It uses simple, non-terminating assertions and
// reports a summary at the end.
// Notes:
// - When CMS_NO_HALF_SUPPORT is NOT defined, the test validates half-to-float conversion.
// - When CMS_NO_HALF_SUPPORT is defined, the test validates that dst remains unchanged
//   (the function takes no action in this path).
// - The test relies only on standard library facilities and a single extern "C" symbol
//   declaration for fromHLFtoFLT.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cmath>


// Declaration of the focal method fromHLFtoFLT implemented in cmsalpha.c
extern "C" void fromHLFtoFLT(void* dst, const void* src);

namespace cmsunit {
    // Global non-terminating failure counter
    int g_failures = 0;

    // Simple non-terminating assertion wrapper: records a failure but continues
    void record_failure(const char* msg) {
        std::cerr << "FAIL: " << msg << std::endl;
        ++g_failures;
    }

    // Simple pass message
    void record_pass(const char* msg) {
        std::cout << "PASS: " << msg << std::endl;
    }

    // Retrieve total failures
    int getFailures() {
        return g_failures;
    }
}

// Test: When CMS_NO_HALF_SUPPORT is NOT defined, fromHLFtoFLT should convert a 16-bit
// half-precision value to a 32-bit float in dst.
#ifndef CMS_NO_HALF_SUPPORT
void test_fromHLFtoFLT_converts_half_to_float() {
    // Arrange
    float dst = -12345.0f;
    uint16_t src = 0x3C00; // IEEE 754 half for 1.0f

    // Act
    fromHLFtoFLT(&dst, &src);

    // Assert: allow a small tolerance due to conversion nuances
    float expected = 1.0f;
    float tol = 1e-6f;
    if (std::fabs(dst - expected) > tol) {
        cmsunit::record_failure("fromHLFtoFLT did not convert 0x3C00 to approximately 1.0f");
    } else {
        cmsunit::record_pass("fromHLFtoFLT correctly converted 0x3C00 to 1.0f");
    }
}
#endif

// Additional conversion check: another representative value to increase coverage
#ifndef CMS_NO_HALF_SUPPORT
void test_fromHLFtoFLT_converts_half_to_float_two() {
    // Arrange
    float dst = -9999.0f;
    uint16_t src = 0x4000; // IEEE 754 half for 2.0f

    // Act
    fromHLFtoFLT(&dst, &src);

    // Assert
    float expected = 2.0f;
    float tol = 1e-6f;
    if (std::fabs(dst - expected) > tol) {
        cmsunit::record_failure("fromHLFtoFLT did not convert 0x4000 to approximately 2.0f");
    } else {
        cmsunit::record_pass("fromHLFtoFLT correctly converted 0x4000 to 2.0f");
    }
}
#endif

// Test: When CMS_NO_HALF_SUPPORT is defined, the focal method should perform no action
// other than suppressing unused-parameter warnings. dst should remain unchanged.
#ifdef CMS_NO_HALF_SUPPORT
void test_fromHLFtoFLT_no_op_when_no_half_support() {
    // Arrange
    float dst = 3.14159f;
    uint16_t src = 0x3C00; // Value that would convert to 1.0 if half support were present

    // Act
    fromHLFtoFLT(&dst, &src);

    // Assert: dst should be unchanged
    float expected = 3.14159f;
    if (dst != expected) {
        cmsunit::record_failure("fromHLFtoFLT altered dst when CMS_NO_HALF_SUPPORT is defined (no-op expected)");
    } else {
        cmsunit::record_pass("fromHLFtoFLT performed no operation as expected under CMS_NO_HALF_SUPPORT");
    }
}
#endif

int main() {
    // Run appropriate tests based on compile-time macro
#ifndef CMS_NO_HALF_SUPPORT
    // Tests to verify true path (half-to-float conversion)
    test_fromHLFtoFLT_converts_half_to_float();
    test_fromHLFtoFLT_converts_half_to_float_two();
#else
    // Tests to verify false path (no conversion performed when half support is disabled)
    test_fromHLFtoFLT_no_op_when_no_half_support();
#endif

    // Summary
    int failures = cmsunit::getFailures();
    if (failures == 0) {
        std::cout << "All tests completed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
        return 1;
    }
}