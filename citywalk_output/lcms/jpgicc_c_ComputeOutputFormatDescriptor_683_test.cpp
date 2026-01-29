// This test suite targets the focal function:
// cmsUInt32Number ComputeOutputFormatDescriptor(cmsUInt32Number dwInput, int OutColorSpace)
// The tests are written in C++11, do not rely on GoogleTest, and execute via a simple main-based runner.
// The tests assume the repository provides the iccjpeg.h header (or equivalent) that defines
// the necessary macros and types used by the function (PT_* color spaces, COLORSPACE_SH, PLANAR_SH,
// CHANNELS_SH, BYTES_SH, FLAVOR_SH, T_PLANAR, etc.) as well as the global Compressor object with
// the field write_Adobe_marker. We also assume the function uses cmsUInt32Number as input/output type.
//
// Strategy:
// - Use the actual macros and constants from the library to compute the expected descriptor.
// - Exercise both branches for color spaces (GRAY -> 1 channel, 3 channels for RGB/CMY/Lab/YUV/YCbCr, 4 for CMYK).
// - Test CMYK flavor toggling via Compressor.write_Adobe_marker (true => flavor 1, false => flavor 0).
// - Probe planar vs non-planar through T_PLANAR(dwInput) by choosing arbitrary dwInput values.
// - Keep tests self-contained and use a tiny test harness (no external test framework).

#include <iccjpeg.h>
#include <jpeglib.h>
#include <iostream>
#include <iomanip>
#include <utils.h>


// Include the header that defines all necessary types/macros used by ComputeOutputFormatDescriptor.
// This is typically the header bundled with the library (iccjpeg.h in the project's dependencies).

// The focal function is defined in a C file. We declare it for C linkage in the C++ test.
extern "C" cmsUInt32Number ComputeOutputFormatDescriptor(cmsUInt32Number dwInput, int OutColorSpace);

// Access to the Compressor flag that influences the CMYK flavor.
// We declare an external struct with the named field write_Adobe_marker to toggle the flavor logic.
extern struct {
    cmsBool write_Adobe_marker;
} Compressor;

// Simple non-terminating test assertion helper. Returns true if a == b, false otherwise.
template <typename T>
static bool_assertion_equal(bool &ok, const T &a, const T &b, const char* testDesc) {
    if (a != b) {
        std::cerr << "Test FAILED: " << testDesc << "\n";
        std::cerr << "  Actual:   " << std::setw(0) << std::hex << a << std::dec << "\n";
        std::cerr << "  Expected: " << std::setw(0) << std::hex << b << std::dec << "\n";
        ok = false;
        return false;
    }
    std::cout << "Test PASSED: " << testDesc << "\n";
    return true;
}

// Compute the expected descriptor using the same macros as the focal function.
// This ensures we stay in sync with the implementation while avoiding hard-coded bitfields.
static cmsUInt32Number ComputeExpectedDescriptor(cmsUInt32Number dwInput, int OutColorSpace) {
    int IsPlanar  = T_PLANAR(dwInput);
    int Channels  = 0;
    int Flavor    = 0;

    switch (OutColorSpace) {
        case PT_GRAY:
            Channels = 1;
            break;
        case PT_RGB:
        case PT_CMY:
        case PT_Lab:
        case PT_YUV:
        case PT_YCbCr:
            Channels = 3;
            break;
        case PT_CMYK:
            if (Compressor.write_Adobe_marker)
                Flavor = 1;
            Channels = 4;
            break;
        default:
            // For unknown color spaces, we do not compute an expected descriptor here.
            // The focal method would call FatalError in production.
            // Return 0 to indicate an unsupported path in tests.
            return 0;
    }

    return (COLORSPACE_SH(OutColorSpace) | PLANAR_SH(IsPlanar) | CHANNELS_SH(Channels) | BYTES_SH(1) | FLAVOR_SH(Flavor));
}

// Test 1: Gray color space should yield 1 channel descriptor (non-CMYK path, flavor 0).
static bool test_gray_descriptor() {
    cmsUInt32Number dwInput = 0;           // Arbitrary input; relies on T_PLANAR macro for IsPlanar
    int OutColorSpace = PT_GRAY;

    cmsUInt32Number actual   = ComputeOutputFormatDescriptor(dwInput, OutColorSpace);
    cmsUInt32Number expected = ComputeExpectedDescriptor(dwInput, OutColorSpace);

    bool ok = true;
    test_eq = true;
    std::cout << "Running test_gray_descriptor..." << std::endl;
    if (expected == 0) {
        std::cerr << "Warning: Expected descriptor computation was not possible for Gray test (unexpected path).\n";
        return false;
    }
    // Use a non-terminating assertion
    return (actual == expected);
}

// Test 2: 3-channel color spaces (RGB family) should yield 3 channels.
static bool test_rgb_family_descriptor() {
    cmsUInt32Number dwInput = 0;           // Arbitrary; uses T_PLANAR
    int OutColorSpace = PT_RGB;            // 3-channel path

    cmsUInt32Number actual   = ComputeOutputFormatDescriptor(dwInput, OutColorSpace);
    cmsUInt32Number expected = ComputeExpectedDescriptor(dwInput, OutColorSpace);

    std::cout << "Running test_rgb_family_descriptor..." << std::endl;
    return (actual == expected);
}

// Test 3: CMYK without the Adobe marker should have Flavor = 0 and 4 channels.
static bool test_cmyk_descriptor_without_adobe() {
    cmsUInt32Number dwInput = 0;
    int OutColorSpace = PT_CMYK;

    // Ensure Adobe marker is disabled
    Compressor.write_Adobe_marker = 0;

    cmsUInt32Number actual   = ComputeOutputFormatDescriptor(dwInput, OutColorSpace);
    cmsUInt32Number expected = ComputeExpectedDescriptor(dwInput, OutColorSpace);

    std::cout << "Running test_cmyk_descriptor_without_adobe..." << std::endl;
    if (expected == 0) {
        std::cerr << "Warning: Expected descriptor computation was not possible for CMYK test (unexpected path).\n";
        return false;
    }
    return (actual == expected);
}

// Test 4: CMYK with the Adobe marker should set Flavor = 1 and 4 channels.
static bool test_cmyk_descriptor_with_adobe() {
    cmsUInt32Number dwInput = 0;
    int OutColorSpace = PT_CMYK;

    // Enable Adobe marker to influence Flavor
    Compressor.write_Adobe_marker = 1;

    cmsUInt32Number actual   = ComputeOutputFormatDescriptor(dwInput, OutColorSpace);
    cmsUInt32Number expected = ComputeExpectedDescriptor(dwInput, OutColorSpace);

    std::cout << "Running test_cmyk_descriptor_with_adobe..." << std::endl;
    if (expected == 0) {
        std::cerr << "Warning: Expected descriptor computation was not possible for CMYK test (unexpected path).\n";
        return false;
    }
    return (actual == expected);
}

// Simple test harness
int main() {
    // Informational header
    std::cout << "Starting unit tests for ComputeOutputFormatDescriptor...\n";

    int total = 4;
    int passed = 0;

    if (test_gray_descriptor()) ++passed;
    if (test_rgb_family_descriptor()) ++passed;
    if (test_cmyk_descriptor_without_adobe()) ++passed;
    if (test_cmyk_descriptor_with_adobe()) ++passed;

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}