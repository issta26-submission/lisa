/*
 * Lightweight unit test suite for the PackLabFloatFromFloat function.
 * This test suite is self-contained (C++11) and does not rely on GTest.
 * It re-implements the focal function (as provided) with minimal
 * environment to exercise both branches: planar and non-planar.
 * 
 * Notes:
 * - The tests cover true/false branches of the predicate T_PLANAR(Info->OutputFormat).
 * - We vary PixelSize via planar-flag to exercise Stride division.
 * - We also exercise the return pointer calculations with and without T_EXTRA.
 * - Floating-point comparisons use a tolerance to account for FP arithmetic.
 */

#include <cstring>
#include <cstdio>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific/types (simplified for unit testing)
typedef unsigned char cmsUInt8Number;
typedef unsigned int  cmsUInt32Number;
typedef float         cmsFloat32Number;

// Minimal TRANSFORM-like struct used by the focal function
typedef struct _cmsTRANSFORM {
    cmsUInt32Number OutputFormat;
} _cmsTRANSFORM;

// Helper float comparison with tolerance
static bool almostEqual(float a, float b, float tol = 1e-5f) {
    return std::fabs(a - b) <= tol;
}

// Lightweight fixtures/macros to mimic the focal environment
// Planar if bit 0x1 is set; PixelSize doubles when planar (to exercise Stride division)
#define T_PLANAR(fmt) ((fmt) & 0x1)
#define PixelSize(fmt)   ( ((fmt) & 0x1) ? 2 : 1 )
#define T_EXTRA(fmt)     ( ((fmt) & 0x2) ? 1 : 0 )

// Re-implementation of the focal method PackLabFloatFromFloat
cmsUInt8Number* PackLabFloatFromFloat(_cmsTRANSFORM* Info,
                                      cmsFloat32Number wOut[],
                                      cmsUInt8Number* output,
                                      cmsUInt32Number Stride)
{
    cmsFloat32Number* Out = (cmsFloat32Number*) output;
    if (T_PLANAR(Info -> OutputFormat)) {
        Stride /= PixelSize(Info->OutputFormat);
        Out[0]        = (cmsFloat32Number) (wOut[0] * 100.0);
        Out[Stride]   = (cmsFloat32Number) (wOut[1] * 255.0 - 128.0);
        Out[Stride*2] = (cmsFloat32Number) (wOut[2] * 255.0 - 128.0);
        return output + sizeof(cmsFloat32Number);
    }
    else {
        Out[0] = (cmsFloat32Number) (wOut[0] * 100.0);
        Out[1] = (cmsFloat32Number) (wOut[1] * 255.0 - 128.0);
        Out[2] = (cmsFloat32Number) (wOut[2] * 255.0 - 128.0);
        return output + (sizeof(cmsFloat32Number)*3 + T_EXTRA(Info ->OutputFormat) * sizeof(cmsFloat32Number));
    }
}

// Test 1: Planar path with PixelSize doubling (Stride division) - validates Out[0], Out[Stride], Out[Stride*2]
bool test_Planar_StrideDivision() {
    _cmsTRANSFORM Info;
    Info.OutputFormat = 0x1; // planar bit set

    cmsFloat32Number wOut[3] = {0.5f, -0.25f, 0.75f};

    // Buffer large enough to hold several floats; using 6 float slots
    cmsFloat32Number buffer[6];
    std::memset(buffer, 0, sizeof(buffer));
    cmsUInt8Number* output = reinterpret_cast<cmsUInt8Number*>(buffer);

    // Stride chosen to exercise division: 4 will become 2 due to PixelSize=2
    cmsUInt32Number Stride = 4;

    cmsUInt8Number* ret = PackLabFloatFromFloat(&Info, wOut, output, Stride);

    // Expected: return pointer equals original buffer + sizeof(float)
    if (ret != reinterpret_cast<cmsUInt8Number*>(buffer) + sizeof(cmsFloat32Number)) {
        printf("Test Planar_StrideDivision: Return pointer mismatch\n");
        return false;
    }

    // After operation, buffer[0] should be 50.0f
    // Because wOut[0] * 100.0 = 0.5 * 100 = 50
    // Stride = 2, so Out[Stride] = Out[2] should be wOut[1]*255 - 128
    // Out[Stride*2] = Out[4] should be wOut[2]*255 - 128
    if (!almostEqual(buffer[0], 50.0f)) {
        printf("Test Planar_StrideDivision: Out[0] expected 50, got %f\n", buffer[0]);
        return false;
    }
    if (!almostEqual(buffer[2], -191.75f)) {
        printf("Test Planar_StrideDivision: Out[Stride] expected -191.75, got %f\n", buffer[2]);
        return false;
    }
    if (!almostEqual(buffer[4], 63.25f)) {
        printf("Test Planar_StrideDivision: Out[Stride*2] expected 63.25, got %f\n", buffer[4]);
        return false;
    }

    return true;
}

// Test 2: Non-planar path without extra bytes - validates Out[0..2] and return offset
bool test_NonPlanar_NoExtra() {
    _cmsTRANSFORM Info;
    Info.OutputFormat = 0x0; // non-planar

    cmsFloat32Number wOut[3] = {0.2f, 0.4f, 0.6f};

    cmsFloat32Number buffer[4]; // extra space to ensure no overflow
    std::memset(buffer, 0, sizeof(buffer));
    cmsUInt8Number* output = reinterpret_cast<cmsUInt8Number*>(buffer);

    cmsUInt32Number Stride = 3; // unused in non-planar branch

    cmsUInt8Number* ret = PackLabFloatFromFloat(&Info, wOut, output, Stride);

    // Expect return: output + 3*sizeof(float) since T_EXTRA(0) = 0
    if (ret != output + 3 * sizeof(cmsFloat32Number)) {
        printf("Test NonPlanar_NoExtra: Return pointer mismatch\n");
        return false;
    }

    // Validate Out[0..2]:
    // 0.2 * 100 = 20
    // 0.4 * 255 - 128 = 102 - 128 = -26
    // 0.6 * 255 - 128 = 153 - 128 = 25
    if (!almostEqual(buffer[0], 20.0f)) {
        printf("Test NonPlanar_NoExtra: Out[0] expected 20, got %f\n", buffer[0]);
        return false;
    }
    if (!almostEqual(buffer[1], -26.0f)) {
        printf("Test NonPlanar_NoExtra: Out[1] expected -26, got %f\n", buffer[1]);
        return false;
    }
    if (!almostEqual(buffer[2], 25.0f)) {
        printf("Test NonPlanar_NoExtra: Out[2] expected 25, got %f\n", buffer[2]);
        return false;
    }

    return true;
}

// Test 3: Planar path with extra bytes flag set (T_EXTRA) and Stride division
bool test_Planar_WithExtra() {
    _cmsTRANSFORM Info;
    // planar and extra bits both set: 0x1 (planar) | 0x2 (extra)
    Info.OutputFormat = 0x3;

    cmsFloat32Number wOut[3] = {0.25f, 0.5f, 0.75f};

    cmsFloat32Number buffer[6];
    std::memset(buffer, 0, sizeof(buffer));
    cmsUInt8Number* output = reinterpret_cast<cmsUInt8Number*>(buffer);

    cmsUInt32Number Stride = 4; // planar with PixelSize=2 will use 2
    cmsUInt8Number* ret = PackLabFloatFromFloat(&Info, wOut, output, Stride);

    if (ret != output + sizeof(cmsFloat32Number)) {
        printf("Test Planar_WithExtra: Return pointer mismatch\n");
        return false;
    }

    // With planar and PixelSize=2, Out[0] = 0.25*100 = 25
    // Out[Stride] (Out[2]) = 0.5*255 - 128 = - -? 0.5*255=127.5; 127.5-128 = -0.5
    // Out[Stride*2] (Out[4]) = 0.75*255 - 128 = 191.25 - 128 = 63.25
    if (!almostEqual(buffer[0], 25.0f)) {
        printf("Test Planar_WithExtra: Out[0] expected 25, got %f\n", buffer[0]);
        return false;
    }
    if (!almostEqual(buffer[2], -0.5f)) {
        printf("Test Planar_WithExtra: Out[2] expected -0.5, got %f\n", buffer[2]);
        return false;
    }
    if (!almostEqual(buffer[4], 63.25f)) {
        printf("Test Planar_WithExtra: Out[4] expected 63.25, got %f\n", buffer[4]);
        return false;
    }

    return true;
}

// Test 4: Non-planar path with extra bytes flag set (T_EXTRA) - verifies return offset accounting
bool test_NonPlanar_WithExtra() {
    _cmsTRANSFORM Info;
    // non-planar with extra bytes flag: 0x2
    Info.OutputFormat = 0x2;

    cmsFloat32Number wOut[3] = {0.1f, 0.2f, 0.3f};

    cmsFloat32Number buffer[6];
    std::memset(buffer, 0, sizeof(buffer));
    cmsUInt8Number* output = reinterpret_cast<cmsUInt8Number*>(buffer);

    cmsUInt32Number Stride = 3;
    cmsUInt8Number* ret = PackLabFloatFromFloat(&Info, wOut, output, Stride);

    // Non-planar: return should be output + (3*sizeof(float) + T_EXTRA* sizeof(float)) = +16
    if (ret != output + 4 * sizeof(cmsFloat32Number)) {
        printf("Test NonPlanar_WithExtra: Return pointer mismatch\n");
        return false;
    }

    // Validate values
    if (!almostEqual(buffer[0], 10.0f)) {
        printf("Test NonPlanar_WithExtra: Out[0] expected 10, got %f\n", buffer[0]);
        return false;
    }
    if (!almostEqual(buffer[1], -77.0f)) {
        printf("Test NonPlanar_WithExtra: Out[1] expected -77, got %f\n", buffer[1]);
        return false;
    }
    if (!almostEqual(buffer[2], -51.5f)) {
        printf("Test NonPlanar_WithExtra: Out[2] expected -51.5, got %f\n", buffer[2]);
        return false;
    }

    return true;
}

int main() {
    int totalTests = 4;
    int passed = 0;

    if (test_Planar_StrideDivision()) {
        printf("PASS: Planar_StrideDivision\n");
        ++passed;
    } else {
        printf("FAIL: Planar_StrideDivision\n");
    }

    if (test_NonPlanar_NoExtra()) {
        printf("PASS: NonPlanar_NoExtra\n");
        ++passed;
    } else {
        printf("FAIL: NonPlanar_NoExtra\n");
    }

    if (test_Planar_WithExtra()) {
        printf("PASS: Planar_WithExtra\n");
        ++passed;
    } else {
        printf("FAIL: Planar_WithExtra\n");
    }

    if (test_NonPlanar_WithExtra()) {
        printf("PASS: NonPlanar_WithExtra\n");
        ++passed;
    } else {
        printf("FAIL: NonPlanar_WithExtra\n");
    }

    printf("Summary: %d/%d tests passed\n", passed, totalTests);
    return (passed == totalTests) ? 0 : 1;
}