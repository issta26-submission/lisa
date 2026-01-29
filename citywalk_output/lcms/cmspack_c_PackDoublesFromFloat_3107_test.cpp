/*
Unit test suite for the focal method PackDoublesFromFloat in cmspack.c

Step 1 (Program Understanding) and Step 2 (Unit Test Generation) summary:
- Focused method: PackDoublesFromFloat(_cmsTRANSFORM* info, cmsFloat32Number wOut[], cmsUInt8Number* output, cmsUInt32Number Stride)
- Core dependent components (Candidate Keywords):
  - nChan (T_CHANNELS(info->OutputFormat))
  - DoSwap (T_DOSWAP(info->OutputFormat))
  - Reverse (T_FLAVOR(info->OutputFormat))
  - Extra (T_EXTRA(info->OutputFormat))
  - SwapFirst (T_SWAPFIRST(info->OutputFormat))
  - Planar (T_PLANAR(info->OutputFormat))
  - ExtraFirst = DoSwap ^ SwapFirst
  - maximum = IsInkSpace(info->OutputFormat) ? 100.0 : 1.0
  - v (temporary value)
  - swap1 (pointer alias to output as cmsFloat64Number*)
  - Stride adjusted by PixelSize(info->OutputFormat)
  - Copy/assignment behavior into either planar or chunky storage
  - Optional special-case: if (Extra == 0 && SwapFirst) { memmove(...) ; *swap1 = v; }
  - Return pointer path: if planar -> output + sizeof(cmsFloat64Number); else -> output + (nChan + Extra) * sizeof(cmsFloat64Number)

- Step 3 (Test Case Refinement) plan adheres to domain knowledge:
  - Use actual constants provided by the LCMS headers (TYPE_GRAY_8 and TYPE_RGB_8) to drive nChan = 1 and nChan = 3 respectively.
  - Validate both 1-channel and multi-channel paths.
  - Validate memory layout by reading back doubles written into the output buffer.
  - Use non-terminating assertions by reporting failures but continuing when possible.
  - Use only standard library facilities and the provided LCMS types/macros. Tests are written for C++, but avoid GTest usage per instructions.

Notes:
- The test relies on the LCMS internal header availability (lcms2_internal.h) and the public/provided constants TYPE_GRAY_8 and TYPE_RGB_8. If those symbols are not defined in the environment, the corresponding tests will be skipped with a message.
- The tests assume the library body cmspack.c is compiled and linked into the test binary, providing the PackDoublesFromFloat symbol.

Test expectations:
- Test 1 (Basic Gray8, 1-channel): With OutputFormat = TYPE_GRAY_8 and wOut[0] = 3.25, the first 8 bytes of output should contain 3.25 as a double, and the function should return output + 8.
- Test 2 (RGB8, 3-channel interleaved): With OutputFormat = TYPE_RGB_8 and wOut = {1.1, 2.2, 3.3}, the first three 8-byte doubles in the output should be 1.1, 2.2, and 3.3, respectively, and the function should return output + (3) * 8 = output + 24.

Limitations:
- If TYPE_GRAY_8 or TYPE_RGB_8 are not present in the included headers, the tests will be skipped gracefully with a note.

Code (test suite):

*/

#include <lcms2_internal.h>
#include <cstring>
#include <cmath>
#include <iostream>


// Ensure linkage with C-style function from cmspack.c
extern "C" {
    // Prototype for the focal method under test
    cmsUInt8Number* PackDoublesFromFloat(_cmsTRANSFORM* info,
                                         cmsFloat32Number wOut[],
                                         cmsUInt8Number* output,
                                         cmsUInt32Number Stride);
}

// Include internal LCMS definitions to obtain _cmsTRANSFORM and format constants

// Helper: numerical comparison with tolerance for doubles
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Basic 1-channel gray-scale path (TYPE_GRAY_8)
bool test_basic_gray8() {
#ifdef TYPE_GRAY_8
    _cmsTRANSFORM info = {};
    info.OutputFormat = TYPE_GRAY_8; // 1-channel, non-planar typical path
    cmsFloat32Number wOut[1] = { 3.25f };

    // Allocate output buffer large enough to hold 8 bytes for the single channel double
    cmsUInt8Number buffer[16] = {0};

    cmsUInt8Number* ret = PackDoublesFromFloat(&info, wOut, buffer, 8);

    // Read back the first double written
    double value = 0.0;
    std::memcpy(&value, buffer, sizeof(double));

    bool ok = almost_equal(value, 3.25);
    if (!ok) {
        std::cerr << "[TestBasicGray8] Expected 3.25, got " << value << std::endl;
    }
    // Verify the returned pointer offset
    cmsUInt8Number* expected = buffer + sizeof(double);
    if (ret != expected) {
        std::cerr << "[TestBasicGray8] Return pointer mismatch: expected " 
                  << reinterpret_cast<void*>(expected) << ", got " 
                  << reinterpret_cast<void*>(ret) << std::endl;
        ok = false;
    }

    return ok;
#else
    std::cout << "[TestBasicGray8] Skipped: TYPE_GRAY_8 not defined in this environment." << std::endl;
    return true;
#endif
}

// Test 2: 3-channel RGB path (TYPE_RGB_8)
bool test_rgb8_three_channels() {
#ifdef TYPE_RGB_8
    _cmsTRANSFORM info = {};
    info.OutputFormat = TYPE_RGB_8; // 3-channel, interleaved path
    cmsFloat32Number wOut[3] = { 1.1f, 2.2f, 3.3f };

    // Allocate output buffer large enough to hold 3 doubles
    cmsUInt8Number buffer[48] = {0};

    cmsUInt8Number* ret = PackDoublesFromFloat(&info, wOut, buffer, 8);

    // Read back the first three doubles written
    double a = 0.0, b = 0.0, c = 0.0;
    std::memcpy(&a, buffer, sizeof(double));
    std::memcpy(&b, buffer + sizeof(double), sizeof(double));
    std::memcpy(&c, buffer + 2 * sizeof(double), sizeof(double));

    bool ok = almost_equal(a, 1.1) && almost_equal(b, 2.2) && almost_equal(c, 3.3);
    if (!ok) {
        std::cerr << "[TestRGB8] Expected (1.1, 2.2, 3.3), got (" 
                  << a << ", " << b << ", " << c << ")" << std::endl;
    }

    // Verify the returned pointer offset: should be buffer + (nChan + Extra) * sizeof(double)
    // Here nChan=3, Extra=0, so offset = 3*sizeof(double) = 24 bytes
    cmsUInt8Number* expected = buffer + 3 * sizeof(double);
    if (ret != expected) {
        std::cerr << "[TestRGB8] Return pointer mismatch: expected " 
                  << reinterpret_cast<void*>(expected) << ", got " 
                  << reinterpret_cast<void*>(ret) << std::endl;
        ok = false;
    }

    return ok;
#else
    std::cout << "[TestRGB8] Skipped: TYPE_RGB_8 not defined in this environment." << std::endl;
    return true;
#endif
}

int main() {
    int failures = 0;

    std::cout << "Running PackDoublesFromFloat unit tests (no GTest) ..." << std::endl;

    if (!test_basic_gray8()) {
        ++failures;
    } else {
        std::cout << "[TestBasicGray8] Passed." << std::endl;
    }

    if (!test_rgb8_three_channels()) {
        ++failures;
    } else {
        std::cout << "[TestRGB8] Passed." << std::endl;
    }

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
    }

    return failures;
}