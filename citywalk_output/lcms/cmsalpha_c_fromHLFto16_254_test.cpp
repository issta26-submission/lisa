// File: test_fromHLFto16_with_half.cpp
// Purpose: Unit tests for fromHLFto16 when CMS_NO_HALF_SUPPORT is NOT defined.
// This exercises the true branch of the conditional: convert a 16-bit half to float,
// scale by 65535, saturate, and store as 16-bit unsigned value.
// Tests chosen to cover typical edge cases: 0.0, +1.0, and -1.0 in half-precision.
// The test uses a lightweight, non-terminating assertion style by printing PASS/FAIL.
// Compile: g++ -std=c++11 test_fromHLFto16_with_half.cpp cmsalpha.c -DCMS_NO_HALF_SUPPORT -lm -o test_with_half
// (Note: cmsalpha.c is expected to be built with CMS_NO_HALF_SUPPORT not defined in this test.)
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Declaration of the focal C function fromHLFto16 with C linkage.
extern "C" void fromHLFto16(void* dst, const void* src);

// Simple non-terminating assertion helper
static bool assert_eq_uint16(const char* test_name, uint16_t got, uint16_t expected) {
    bool ok = (got == expected);
    std::cout << std::setw(40) << std::left << test_name
              << " -> " << (ok ? "PASS" : "FAIL")
              << " | expected: " << std::setw(5) << expected
              << "  actual: " << std::setw(5) << got
              << std::endl;
    return ok;
}

int main() {
    // Candidate keywords mapping:
    // - fromHLFto16: core conversion from 16-bit half to 16-bit unsigned value
    // - _cmsHalf2Float: used inside the function to convert half -> float
    // - _cmsQuickSaturateWord: used inside the function to clamp to [0, 65535]
    // - CMS_NO_HALF_SUPPORT: compile-time switch; here we test the path where it is NOT defined

    bool all_passed = true;

    // Test 1: half = +0.0 -> dst should be 0
    {
        uint16_t src = 0x0000; // half +0.0
        uint16_t dst = 0;      // destination buffer
        fromHLFto16(&dst, &src);
        // Expected: 0
        all_passed &= assert_eq_uint16("fromHLFto16: half +0.0 -> 0", dst, 0);
    }

    // Test 2: half = +1.0  -> dst should be 65535
    {
        uint16_t src = 0x3C00; // half +1.0
        uint16_t dst = 0;
        fromHLFto16(&dst, &src);
        // Expected: 65535
        all_passed &= assert_eq_uint16("fromHLFto16: half +1.0 -> 65535", dst, 65535);
    }

    // Test 3: half = -1.0  -> dst should saturate to 0 (negative maps to 0)
    {
        uint16_t src = 0xBC00; // half -1.0
        uint16_t dst = 0;
        fromHLFto16(&dst, &src);
        // Expected: 0
        all_passed &= assert_eq_uint16("fromHLFto16: half -1.0 -> 0", dst, 0);
    }

    // Summary
    if (all_passed) {
        std::cout << "\nAll tests PASSED for fromHLFto16_with_half." << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests FAILED for fromHLFto16_with_half." << std::endl;
        return 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////

// File: test_fromHLFto16_without_half.cpp
// Purpose: Unit tests for fromHLFto16 when CMS_NO_HALF_SUPPORT IS defined.
// This exercises the false branch of the conditional: parameters are unused.
// We verify that the function does not modify dst in this configuration.
// Compile: g++ -std=c++11 test_fromHLFto16_without_half.cpp cmsalpha.c -DCMS_NO_HALF_SUPPORT -lm -o test_without_half
// ( cmsalpha.c is expected to be built with CMS_NO_HALF_SUPPORT defined in this test.)

// Declaration of the focal C function fromHLFto16 with C linkage.
extern "C" void fromHLFto16(void* dst, const void* src);

// Simple non-terminating assertion helper
static bool assert_unchanged_uint16(const char* test_name, uint16_t before, uint16_t after) {
    bool ok = (before == after);
    std::cout << std::setw(40) << std::left << test_name
              << " -> " << (ok ? "PASS" : "FAIL")
              << " | expected unchanged: " << std::setw(5) << before
              << "  actual: " << std::setw(5) << after
              << std::endl;
    return ok;
}

int main() {
    // In this build, the function should be a no-op with respect to dst.
    bool all_passed = true;

    // Test 1: dst preset to a value should remain unchanged regardless of src
    {
        uint16_t src = 0x0000; // content is irrelevant when CMS_NO_HALF_SUPPORT is defined
        uint16_t dst = 0xABCD; // initial known value
        fromHLFto16(&dst, &src);
        all_passed &= assert_unchanged_uint16("fromHLFto16:no-half -> dst unchanged", 0xABCD, dst);
    }

    // Test 2: another prefill value to ensure no modification occurs
    {
        uint16_t src = 0x3C00; // arbitrary
        uint16_t dst = 0x1234; // initial known value
        fromHLFto16(&dst, &src);
        all_passed &= assert_unchanged_uint16("fromHLFto16:no-half -> dst unchanged (2)", 0x1234, dst);
    }

    if (all_passed) {
        std::cout << "\nAll tests PASSED for fromHLFto16_without_half." << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests FAILED for fromHLFto16_without_half." << std::endl;
        return 1;
    }
}