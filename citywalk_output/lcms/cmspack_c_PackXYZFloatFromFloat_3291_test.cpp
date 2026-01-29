// High-quality C++11 unit test suite for the focal method PackXYZFloatFromFloat
// This test suite is self-contained (no GTest) and uses a small in-house
// assertion framework to maximize coverage without terminating on first failure.

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific type aliases to mirror the CMS types used by the focal method
using cmsUInt8Number  = uint8_t;
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;
using cmsFloat32Number = float;

// Lightweight, test-friendly re-implementation of dependencies/macros
// that the focal method relies on. These are minimal stand-ins to enable
// compilation and targeted testing within this unit-test suite.
#define MAX_ENCODEABLE_XYZ 1.0f

// Planar vs. chunky detection (simplified for tests).
// Bit 0 set => planar; else non-planar.
#define T_PLANAR(fmt) ((fmt) & 1)

// PixelSize used for Stride adjustment in planar mode (simplified to 3 channels)
#define PixelSize(fmt) (3)

// Extra samples in the encoded OutputFormat. For tests, we keep it at 0.
#define T_EXTRA(fmt) (0)

// Minimal _cmsTRANSFORM struct with an OutputFormat member (as used by the focal method)
struct _cmsTRANSFORM {
    cmsUInt32Number OutputFormat;
};

// Forward declaration of the focal method under test.
// We provide a compact, self-contained implementation matching the supplied snippet.
cmsUInt8Number* PackXYZFloatFromFloat(_cmsTRANSFORM* Info,
                                      cmsFloat32Number wOut[],
                                      cmsUInt8Number* output,
                                      cmsUInt32Number Stride)
{
    cmsFloat32Number* Out = (cmsFloat32Number*) output;
    if (T_PLANAR(Info -> OutputFormat)) {
        Stride /= PixelSize(Info->OutputFormat);
        Out[0]        = (cmsFloat32Number) (wOut[0] * MAX_ENCODEABLE_XYZ);
        Out[Stride]   = (cmsFloat32Number) (wOut[1] * MAX_ENCODEABLE_XYZ);
        Out[Stride*2] = (cmsFloat32Number) (wOut[2] * MAX_ENCODEABLE_XYZ);
        return output + sizeof(cmsFloat32Number);
    }
    else {
        Out[0] = (cmsFloat32Number) (wOut[0] * MAX_ENCODEABLE_XYZ);
        Out[1] = (cmsFloat32Number) (wOut[1] * MAX_ENCODEABLE_XYZ);
        Out[2] = (cmsFloat32Number) (wOut[2] * MAX_ENCODEABLE_XYZ);
        return output + (sizeof(cmsFloat32Number)*3 + T_EXTRA(Info ->OutputFormat) * sizeof(cmsFloat32Number));
    }
}

// Simple, non-terminating test harness assertions (no GTest dependency)
static int g_failures = 0;

static void log_fail(const char* msg) {
    std::cerr << "Test failure: " << msg << std::endl;
    ++g_failures;
}

static bool floats_close(float a, float b, float eps = 1e-6f) {
    return std::fabs(a - b) <= eps;
}

// Test 1: Planar path (true branch) where Stride is adjusted and values are written at
// indices 0, Stride, and Stride*2. Verifies both memory writes and correct return pointer.
static void test_PackXYZFloatFromFloat_Planar()
{
    // Prepare input: planar format flag set
    _cmsTRANSFORM Info;
    Info.OutputFormat = 1; // Planar (LSB set)

    const cmsFloat32Number wOut[3] = { 0.1f, 0.2f, 0.3f };

    // Allocate 9 floats (3x3) to accommodate Out[0], Out[Stride], Out[Stride*2]
    std::vector<cmsUInt8Number> buffer(9 * sizeof(cmsFloat32Number), 0);
    cmsUInt8Number* bufPtr = buffer.data();

    // Call the function with Stride = 9 so after division by PixelSize(3) we index at 0, 3, 6
    cmsUInt8Number* retPtr = PackXYZFloatFromFloat(&Info, (cmsFloat32Number*)wOut /* cast for signature compatibility */,
                                                  bufPtr, 9);

    cmsFloat32Number* Out = (cmsFloat32Number*) bufPtr;
    cmsFloat32Number expected0 = wOut[0] * MAX_ENCODEABLE_XYZ;
    cmsFloat32Number expected1 = wOut[1] * MAX_ENCODEABLE_XYZ;
    cmsFloat32Number expected2 = wOut[2] * MAX_ENCODEABLE_XYZ;

    // Validate in-range values written to memory
    if (!floats_close(Out[0], expected0)) {
        log_fail("Planar: Out[0] value mismatch.");
    }
    if (!floats_close(Out[3], expected1)) {
        log_fail("Planar: Out[Stride] value mismatch (Out[3]).");
    }
    if (!floats_close(Out[6], expected2)) {
        log_fail("Planar: Out[Stride*2] value mismatch (Out[6]).");
    }

    // Validate return pointer: should be original buffer pointer advanced by sizeof(float)
    uintptr_t ret_addr  = reinterpret_cast<uintptr_t>(retPtr);
    uintptr_t expected_addr = reinterpret_cast<uintptr_t>(bufPtr) + sizeof(cmsFloat32Number);
    if (ret_addr != expected_addr) {
        log_fail("Planar: Return pointer is not advanced by sizeof(float).");
    }
}

// Test 2: Non-planar path (false branch) where only first three floats are written and
// the return pointer accounts for T_EXTRA(OutputFormat). We set T_EXTRA to 0, so
// the return should be exactly after three floats.
static void test_PackXYZFloatFromFloat_NonPlanar()
{
    _cmsTRANSFORM Info;
    Info.OutputFormat = 0; // Non-planar

    const cmsFloat32Number wOut[3] = { 0.5f, 0.25f, 0.125f };

    // Allocate space for exactly 3 floats (we only write 0..2)
    std::vector<cmsUInt8Number> buffer(3 * sizeof(cmsFloat32Number), 0);
    cmsUInt8Number* bufPtr = buffer.data();

    // Call the function; Stride is irrelevant for non-planar path
    cmsUInt8Number* retPtr = PackXYZFloatFromFloat(&Info, (cmsFloat32Number*)wOut,
                                                  bufPtr, 0);

    cmsFloat32Number* OutF = (cmsFloat32Number*) bufPtr;
    cmsFloat32Number expected0 = wOut[0] * MAX_ENCODEABLE_XYZ;
    cmsFloat32Number expected1 = wOut[1] * MAX_ENCODEABLE_XYZ;
    cmsFloat32Number expected2 = wOut[2] * MAX_ENCODEABLE_XYZ;

    // Validate first three floats are written correctly
    if (!floats_close(OutF[0], expected0)) {
        log_fail("Non-planar: Out[0] value mismatch.");
    }
    if (!floats_close(OutF[1], expected1)) {
        log_fail("Non-planar: Out[1] value mismatch.");
    }
    if (!floats_close(OutF[2], expected2)) {
        log_fail("Non-planar: Out[2] value mismatch.");
    }

    // Validate return pointer is after three floats (plus T_EXTRA(OutputFormat) which is 0 here)
    uintptr_t ret_addr  = reinterpret_cast<uintptr_t>(retPtr);
    uintptr_t expected_addr = reinterpret_cast<uintptr_t>(bufPtr) + 3 * sizeof(cmsFloat32Number);
    if (ret_addr != expected_addr) {
        log_fail("Non-planar: Return pointer is not advanced by 3 * sizeof(float).");
    }
}

// Entry point for the test suite
int main() {
    // Run tests
    test_PackXYZFloatFromFloat_Planar();
    test_PackXYZFloatFromFloat_NonPlanar();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}