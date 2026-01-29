// cmspack_unit_test.cpp
// Lightweight C++11 unit tests for UnrollLabFloatToFloat (focal method).
// This test provides minimal stubs for dependencies and executes two targeted tests:
// 1) Planar input path (true branch)
// 2) Chunky (non-planar) input path (false branch)
// The tests are designed to be self-contained (no GTest) and use only the C++ standard library.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// --------------------
// Minimal type definitions (compat with focal method)
// --------------------
typedef unsigned char      cmsUInt8Number;
typedef unsigned short     cmsUInt16Number;
typedef unsigned int       cmsUInt32Number;
typedef float              cmsFloat32Number;
typedef int                cmsBool;

// Forward declaration to mimic cmsLCMS structures used by focal method
struct _cmsTRANSFORM {
    cmsUInt32Number InputFormat;
};
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// --------------------
// Helpers and macros to drive test logic (mimicking cms library behavior)
// --------------------

// Planar vs chunky decision
// In the focal code: if (T_PLANAR(info -> InputFormat)) { ... } else { ... }
static inline int T_PLANAR(cmsUInt32Number fmt) { return (fmt == 1); }

// PixelSize for a given input format (simplified for tests)
static inline cmsUInt32Number PixelSize(cmsUInt32Number /*fmt*/) { return 3; }

// Extra components in planar format (simplified for tests)
static inline cmsUInt32Number T_EXTRA(cmsUInt32Number /*fmt*/) { return 0; }

// --------------------
// Focused function under test (copied/adapted from focal method)
// --------------------
// UnrollLabFloatToFloat
cmsUInt8Number* UnrollLabFloatToFloat(_cmsTRANSFORM* info,
                                      cmsFloat32Number wIn[],
                                      cmsUInt8Number* accum,
                                      cmsUInt32Number Stride)
{
    cmsFloat32Number* Pt = (cmsFloat32Number*) accum;
    if (T_PLANAR(info -> InputFormat)) {
        Stride /= PixelSize(info->InputFormat);
        wIn[0] = (cmsFloat32Number) (Pt[0] / 100.0);                 // from 0..100 to 0..1
        wIn[1] = (cmsFloat32Number) ((Pt[Stride] + 128) / 255.0);    // form -128..+127 to 0..1
        wIn[2] = (cmsFloat32Number) ((Pt[Stride*2] + 128) / 255.0);
        return accum + sizeof(cmsFloat32Number);
    }
    else {
        wIn[0] = (cmsFloat32Number) (Pt[0] / 100.0);            // from 0..100 to 0..1
        wIn[1] = (cmsFloat32Number) ((Pt[1] + 128) / 255.0);    // form -128..+127 to 0..1
        wIn[2] = (cmsFloat32Number) ((Pt[2] + 128) / 255.0);
        accum += sizeof(cmsFloat32Number)*(3 + T_EXTRA(info ->InputFormat));
        return accum;
    }
}

// --------------------
// Lightweight test harness
// --------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper: compare floating values with tolerance
static bool almost_equal_float(cmsFloat32Number a, cmsFloat32Number b, cmsFloat32Number tol = 1e-6f) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Planar path
static void test_planar_float_to_float() {
    g_total_tests++;
    // Prepare info: planar format (flag = 1)
    _cmsTRANSFORM info;
    info.InputFormat = 1; // planar

    // Stride corresponds to number of elements per channel group
    // For planar path we expect Stride to be scaled down by PixelSize (=3)
    cmsUInt32Number Stride = 3; // 3 elements per channel

    // Pt (accumulated input values) needs length at least Stride*2 + 1
    // We allocate 7 floats to safely index Pt[0], Pt[3], Pt[6]
    cmsFloat32Number Pt[7] = {
        50.0f, // Pt[0]  -> wIn[0] = 50/100 = 0.5
        0.0f,   // placeholder, not used in planar path for wIn[1] directly
        0.0f,   // placeholder
        -50.0f, // Pt[3]  -> used for wIn[1]: (-50 + 128)/255
        0.0f,   // placeholder
        0.0f,   // placeholder
        -128.0f  // Pt[6]  -> used for wIn[2]: (-128 + 128)/255 = 0
    };

    // Copy Pt into accum buffer as raw bytes
    cmsUInt8Number accum[sizeof(Pt)];
    std::memcpy(accum, Pt, sizeof(Pt));

    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};

    cmsUInt8Number* ret = UnrollLabFloatToFloat(&info, wIn, accum, Stride);

    // Expected values
    cmsFloat32Number exp0 = Pt[0] / 100.0f;            // 0.5
    cmsFloat32Number exp1 = (Pt[Stride] + 128.0f) / 255.0f; // (-50 + 128)/255
    cmsFloat32Number exp2 = (Pt[Stride*2] + 128.0f) / 255.0f; // (-128 + 128)/255 = 0

    bool ok = almost_equal_float(wIn[0], exp0) &&
              almost_equal_float(wIn[1], exp1) &&
              almost_equal_float(wIn[2], exp2) &&
              ret == accum + sizeof(cmsFloat32Number);

    if (!ok) {
        std::cerr << "test_planar_float_to_float FAILED:\n"
                  << "  wIn[0]=" << wIn[0] << " expected=" << exp0 << "\n"
                  << "  wIn[1]=" << wIn[1] << " expected=" << exp1 << "\n"
                  << "  wIn[2]=" << wIn[2] << " expected=" << exp2 << "\n"
                  << "  return ptr mismatch." << std::endl;
        g_failed_tests++;
    } else {
        std::cout << "test_planar_float_to_float PASSED." << std::endl;
    }

}

// Test 2: Non-planar (chunky) path
static void test_chunky_float_to_float() {
    g_total_tests++;
    // Prepare info: chunky format (flag != 1)
    _cmsTRANSFORM info;
    info.InputFormat = 0; // non-planar

    cmsUInt32Number Stride = 3; // not used meaningfully in chunky path except for interface

    // Pt[0], Pt[1], Pt[2] are used
    cmsFloat32Number Pt[3] = {
        80.0f,   // Pt[0] -> wIn[0] = 0.8
        -128.0f, // Pt[1] -> wIn[1] = 0
        127.0f    // Pt[2] -> wIn[2] = (255)/255 = 1.0
    };

    cmsUInt8Number accum[sizeof(Pt)];
    std::memcpy(accum, Pt, sizeof(Pt));

    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};

    cmsUInt8Number* ret = UnrollLabFloatToFloat(&info, wIn, accum, Stride);

    cmsFloat32Number exp0 = Pt[0] / 100.0f;          // 0.8
    cmsFloat32Number exp1 = (Pt[1] + 128.0f) / 255.0f; // ( -128 + 128)/255 = 0
    cmsFloat32Number exp2 = (Pt[2] + 128.0f) / 255.0f; // (127 + 128)/255 = 255/255 = 1

    // In non-planar path, return should advance by sizeof(float) * (3 + T_EXTRA(...))
    // We set T_EXTRA(...) to 0, so 12 bytes
    bool ok = almost_equal_float(wIn[0], exp0) &&
              almost_equal_float(wIn[1], exp1) &&
              almost_equal_float(wIn[2], exp2) &&
              ret == accum + sizeof(cmsFloat32Number) * (3 + 0);

    if (!ok) {
        std::cerr << "test_chunky_float_to_float FAILED:\n"
                  << "  wIn[0]=" << wIn[0] << " expected=" << exp0 << "\n"
                  << "  wIn[1]=" << wIn[1] << " expected=" << exp1 << "\n"
                  << "  wIn[2]=" << wIn[2] << " expected=" << exp2 << "\n"
                  << "  return ptr mismatch." << std::endl;
        g_failed_tests++;
    } else {
        std::cout << "test_chunky_float_to_float PASSED." << std::endl;
    }
}

// --------------------
// Main entry point: run tests
// --------------------
int main() {
    std::cout << "Starting cmsUnrollLabFloatToFloat unit tests (no GTest)..." << std::endl;

    test_planar_float_to_float();
    test_chunky_float_to_float();

    std::cout << "Tests completed: " << g_total_tests << " executed, "
              << g_failed_tests << " failed." << std::endl;

    return g_failed_tests;
}