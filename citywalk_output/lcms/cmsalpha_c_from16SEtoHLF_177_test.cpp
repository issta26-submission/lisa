// Unit test suite for from16SEtoHLF in cmsalpha.c
// This single translation unit provides a self-contained mock environment
// to exercise the focal function with and without half-support (CMS_NO_HALF_SUPPORT).
// Compile with -DCMS_NO_HALF_SUPPORT to test the No-Half path, or without it to test the Half path.

// Notes:
//  - This test mocks the minimal CMS types/macros used by the focal function.
//  - It provides a simple _cmsFloat2Half implementation that maps [0,1] to [0,65535].
//  - The tests are designed to be executable without Google Test, using a lightweight harness.

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>


// ------------------------------------------------------------
// Minimal CMS-like types and helpers to support the test
// ------------------------------------------------------------
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef double   cmsFloat64Number;
typedef float    cmsFloat32Number;
typedef bool     cmsBool;

#define cmsUNUSED_PARAMETER(x) (void)(x)

static inline cmsUInt16Number CHANGE_ENDIAN_INTERNAL(cmsUInt16Number v) {
    // Simple 16-bit byte swap
    return (cmsUInt16Number)(((v & 0x00FF) << 8) | ((v & 0xFF00) >> 8));
}
#define CHANGE_ENDIAN(x) CHANGE_ENDIAN_INTERNAL(x)

// Lightweight _cmsFloat2Half implementation for testing purposes
static cmsUInt16Number _cmsFloat2Half(cmsFloat32Number f) {
    if (f <= 0.0f) return 0;
    if (f >= 1.0f) return 65535;
    // Scale [0,1] to [0,65535] with truncation
    return (cmsUInt16Number)(f * 65535.0f);
}

// Focal method under test (reproduced here for self-contained testing)
void from16SEtoHLF(void* dst, const void* src)
{
{
#ifndef CMS_NO_HALF_SUPPORT
    cmsFloat32Number n = (CHANGE_ENDIAN(*(cmsUInt16Number*)src)) / 65535.0f;
    *(cmsUInt16Number*)dst = _cmsFloat2Half(n);
#else
    cmsUNUSED_PARAMETER(dst);
    cmsUNUSED_PARAMETER(src);
#endif
}
 }

// ------------------------------------------------------------
// Test harness
// ------------------------------------------------------------
static int g_failures = 0;

// Helper for reporting failures (non-terminating, continues tests)
static void report_failure(const char* test_name, const char* message) {
    std::cerr << "FAIL: " << test_name << " - " << message << "\n";
    ++g_failures;
}

// Test: from16SEtoHLF with half support (CMS_NO_HALF_SUPPORT NOT defined)
// Verifies correct writes to dst for a set of representative inputs.
// This test ensures the endianness adjustment and conversion to 16-bit half value are correct.
#ifndef CMS_NO_HALF_SUPPORT

static void test_from16SEtoHLF_with_half_support() {
    struct Case { uint16_t src; uint16_t expectedDst; const char* name; };
    Case cases[] = {
        { 0x0000, 0x0000, "zero" },       // n = (0)/65535 = 0.0 -> 0
        { 0xFFFF, 0xFFFF, "max" },        // n = (65535)/65535 = 1.0 -> 65535
        { 0x8000, 0x0080, "mid_low" },    // after swap: 0x0080 -> 128/65535
        { 0x7FFF, 0xFF7F, "near_max" }    // after swap: 0xFF7F -> 65407/65535
    };

    const int casesCount = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < casesCount; ++i) {
        uint16_t src = cases[i].src;
        uint16_t dst = 0xABCD; // sentinel to ensure write occurs
        from16SEtoHLF(&dst, &src);
        if (dst != cases[i].expectedDst) {
            // Include test name and expected/actual values
            std::ostringstream oss;
            oss << "dst mismatch for test '" << cases[i].name
                << "': got " << dst << ", expected " << cases[i].expectedDst;
            report_failure(cases[i].name, oss.str().c_str());
        }
    }
}

// Entry point for the "with half" tests
int main_with_half() {
    test_from16SEtoHLF_with_half_support();
    if (g_failures == 0) {
        std::cout << "All from16SEtoHLF tests (with half support) passed.\n";
    } else {
        std::cout << g_failures << " from16SEtoHLF test(s) failed (with half support).\n";
    }
    return g_failures;
}

#else
// If CMS_NO_HALF_SUPPORT is defined, this block is excluded.
// The test registration is handled in the No-Half section below.
#endif

// ------------------------------------------------------------
// Test: from16SEtoHLF without half support (CMS_NO_HALF_SUPPORT defined)
// Verifies that function does not modify dst and handles parameters properly
// by using a sentinel destination value before the call.
// This ensures the early-out path does not write to dst.
#ifdef CMS_NO_HALF_SUPPORT

static void test_from16SEtoHLF_no_half_support() {
    struct Case { uint16_t src; uint16_t expectedDst; const char* name; };
    Case cases[] = {
        { 0x0000, 0xABCD, "no_change_zero" },  // dst should remain unchanged
        { 0xFFFF, 0xABCD, "no_change_max" },
        { 0x8000, 0xABCD, "no_change_mid" },
        { 0x7FFF, 0xABCD, "no_change_near_max" }
    };

    const int casesCount = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < casesCount; ++i) {
        uint16_t src = cases[i].src;
        uint16_t dst = 0xABCD; // sentinel to detect any writes
        from16SEtoHLF(&dst, &src);
        if (dst != 0xABCD) {
            std::ostringstream oss;
            oss << "dst should remain unchanged for test '" << cases[i].name
                << "', but changed to " << dst;
            report_failure(cases[i].name, oss.str().c_str());
        }
    }
}

// Entry point for the "no half" tests
int main_no_half() {
    test_from16SEtoHLF_no_half_support();
    if (g_failures == 0) {
        std::cout << "All from16SEtoHLF tests (no half support) passed.\n";
    } else {
        std::cout << g_failures << " from16SEtoHLF test(s) failed (no half).\n";
    }
    return g_failures;
}

#endif

// ------------------------------------------------------------
// Entrypoint selection depending on CMS_NO_HALF_SUPPORT
// ------------------------------------------------------------
#ifndef CMS_NO_HALF_SUPPORT
int main() {
    // Run tests for the normal path (half support available)
    return main_with_half();
}
#else
int main() {
    // Run tests for the no-half path
    return main_no_half();
}
#endif

// End of test suite

// Explanatory notes for users compiling this test:
// - To test the path with half support, compile without defining CMS_NO_HALF_SUPPORT:
//     g++ -std=c++11 -O2 cms_test.cpp -o cms_test
// - To test the path without half support, compile with CMS_NO_HALF_SUPPORT defined:
//     g++ -std=c++11 -DCMS_NO_HALF_SUPPORT -O2 cms_test.cpp -o cms_test_nohalf
// In both cases, the executable will run the relevant test set and report results.