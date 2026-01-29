// Test suite for the focal method: fromFLTtoHLF in cmsalpha.c
// This test targets the behavior when half-precision (FP16) support is available
// and, in a separate build (compiled with CMS_NO_HALF_SUPPORT defined), verifies
// that the function is a no-op (dst remains unchanged).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Include internal CMS headers to get type definitions (cmsFloat32Number, cmsUInt16Number, etc.)
// and the internal conversion helper _cmsFloat2Half if available.

#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test (C linkage)
void fromFLTtoHLF(void* dst, const void* src);
#ifdef __cplusplus
}
#endif

// Helper macro to report test results without terminating execution
#define REPORT_FAIL(msg) do { std::cerr << "TEST FAIL: " << msg << "\n"; } while (0)
#define REPORT_PASS(msg) do { std::cout << "TEST PASS: " << msg << "\n"; } while (0)

#if !defined(CMS_NO_HALF_SUPPORT)
/*
   Test: fromFLTtoHLF should convert common 32-bit float values to FP16 correctly
   when half-precision support is enabled.
   Covers true branches by checking multiple representative inputs.
*/
bool test_fromFLTtoHLF_half_basic_values() {
    cmsUInt16Number dst;
    cmsFloat32Number src;

    // 1.0f -> 0x3C00 (FP16 representation of 1.0)
    src = 1.0f;
    fromFLTtoHLF(&dst, &src);
    if (dst != 0x3C00) {
        std::cerr << "fromFLTtoHLF: expected 0x3C00 for 1.0f, got " << dst << " (decimal)\n";
        return false;
    }

    // 0.0f -> 0x0000 (FP16 representation of 0.0)
    src = 0.0f;
    fromFLTtoHLF(&dst, &src);
    if (dst != 0x0000) {
        std::cerr << "fromFLTtoHLF: expected 0x0000 for 0.0f, got " << dst << " (decimal)\n";
        return false;
    }

    // -1.0f -> 0xBC00 (FP16 representation of -1.0)
    src = -1.0f;
    fromFLTtoHLF(&dst, &src);
    if (dst != 0xBC00) {
        std::cerr << "fromFLTtoHLF: expected 0xBC00 for -1.0f, got " << dst << " (decimal)\n";
        return false;
    }

    return true;
}
#endif

#if defined(CMS_NO_HALF_SUPPORT)
/*
   Test: When half support is not compiled in, the function should not modify dst.
   This ensures the else-branch is exercised (no operation).
*/
bool test_fromFLTtoHLF_no_half_support_preserves_dst() {
    cmsUInt16Number dst = 0xABCD;      // arbitrary initial value to detect changes
    cmsFloat32Number src = 1.0f;

    fromFLTtoHLF(&dst, &src);

    if (dst != 0xABCD) {
        std::cerr << "fromFLTtoHLF (no half) modified dst unexpectedly. dst=" << dst << "\n";
        return false;
    }
    return true;
}
#endif

int main() {
    int failures = 0;

    std::cout << "Running test suite for fromFLTtoHLF...\n";

#if !defined(CMS_NO_HALF_SUPPORT)
    if (!test_fromFLTtoHLF_half_basic_values()) {
        ++failures;
        REPORT_FAIL("test_fromFLTtoHLF_half_basic_values failed");
    } else {
        REPORT_PASS("test_fromFLTtoHLF_half_basic_values passed");
    }
#else
    if (!test_fromFLTtoHLF_no_half_support_preserves_dst()) {
        ++failures;
        REPORT_FAIL("test_fromFLTtoHLF_no_half_support_preserves_dst failed");
    } else {
        REPORT_PASS("test_fromFLTtoHLF_no_half_support_preserves_dst passed");
    }
#endif

    if (failures > 0) {
        std::cerr << "Total failures: " << failures << "\n";
    } else {
        std::cout << "All tests passed.\n";
    }

    return failures;
}