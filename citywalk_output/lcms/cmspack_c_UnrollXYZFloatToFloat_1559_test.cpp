// Unit tests for the focal method: UnrollXYZFloatToFloat
// Target: C++11, no Google Test. Uses a lightweight, non-terminating test harness.
// Notes:
// - This test suite is written to be compiled and linked with the project that provides
//   the cmspack.c implementation and its proper dependencies (lcms headers, etc.).
// - The tests assume the public prototype of UnrollXYZFloatToFloat is available to the test
//   code via an extern "C" declaration.
// - The internal _cmsTRANSFORM type is modeled with the first field being InputFormat to
//   match how the focal method accesses info->InputFormat. This keeps the test decoupled
//   from full LCMS internal layout while preserving correct behavior for InputFormat access.

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Forward-declare the types expected by the focal method.
// We declare only what is needed by the test to compile and link against cmspack.c.
typedef unsigned char cmsUInt8Number;
typedef float cmsFloat32Number;
typedef unsigned int cmsUInt32Number;

// The focal function is declared with C linkage.
extern "C" {
    // Minimal external declaration of the focal method under test.
    // The real signature in cmspack.c is:
    // cmsUInt8Number* UnrollXYZFloatToFloat(_cmsTRANSFORM* info,
    //                                       cmsFloat32Number wIn[],
    //                                       cmsUInt8Number* accum,
    //                                       cmsUInt32Number Stride)
    typedef struct _cmsTRANSFORM {
        cmsUInt32Number InputFormat;
        // real library may have more fields, but for our test we only rely on InputFormat
    } cmsTRANSFORM;

    cmsUInt8Number* UnrollXYZFloatToFloat(_cmsTRANSFORM* info,
                                         cmsFloat32Number wIn[],
                                         cmsUInt8Number* accum,
                                         cmsUInt32Number Stride);
}

// Lightweight non-terminating test harness
struct TestContext {
    int failures = 0;
    std::vector<std::string> messages;

    void fail(const std::string& msg) {
        ++failures;
        messages.push_back("FAIL: " + msg);
    }

    void pass(const std::string& msg) {
        messages.push_back("PASS: " + msg);
    }
};

static bool almostEqual(float a, float b, float eps = 1e-6f) {
    return std::fabs(a - b) <= eps;
}

// Test 1: Planar path (true branch). Assumes that the symbolic planar flag is triggered
// by info.InputFormat == PLANAR_INPUT (assumed 0 in this test scenario).
// We rely on MAX_ENCODEABLE_XYZ from the library to construct expected Pt values.
void test_PlanarBranch(TestContext& ctx) {
    // Arrange
    cmsTRANSFORM info;
    // Best-effort planar flag used by the library. We hard-code an assumption:
    // PLANAR path when InputFormat == 0 (adjust if your build uses a different mapping).
    const cmsUInt32Number PLANAR_INPUT = 0;
    info.InputFormat = PLANAR_INPUT;

    // We prepare an accum buffer large enough to hold Pt[0], Pt[Stride], Pt[Stride*2].
    // We'll use Stride = 4 to keep indices simple (0, 4, 8).
    const cmsUInt32Number Stride = 4;
    const size_t FLOATS_NEEDED_PLANAR = 9; // indices 0..8 inclusive
    const size_t BYTES_NEEDED_PLANAR = FLOATS_NEEDED_PLANAR * sizeof(float);
    unsigned char accumBuffer[256];
    std::memset(accumBuffer, 0, sizeof(accumBuffer));

    // Pt points to the same memory; populate the required indices.
    float* Pt = reinterpret_cast<float*>(accumBuffer);
    // Pt[0], Pt[Stride], Pt[Stride*2] values
    // Use MAX_ENCODEABLE_XYZ (from LCMS) to derive predictable normalized results.
    // If MAX_ENCODEABLE_XYZ is a macro, the value will be substituted at compile-time.
    // We purposely compute values as fractions of MAX_ENCODEABLE_XYZ.
    const float scale = 1.0f; // 1.0 * MAX_ENCODEABLE_XYZ
    Pt[0]        = std::numeric_limits<float>::quiet_NaN(); // placeholder will be overwritten below
    Pt[Stride]     = std::numeric_limits<float>::quiet_NaN();
    Pt[Stride*2]   = std::numeric_limits<float>::quiet_NaN();

    // Compute expected ratio values using MAX_ENCODEABLE_XYZ from library
    // to avoid hard-coding numeric constants.
    // Guard against missing macro by falling back to 1.0f if not defined (best-effort).
    // Note: We rely on the presence of MAX_ENCODEABLE_XYZ in the linked LCMS headers.
#ifndef MAX_ENCODEABLE_XYZ
#define MAX_ENCODEABLE_XYZ 1.0f
#endif

    // The test should place Pt values as MAX_ENCODEABLE_XYZ * fraction.
    Pt[0]       = MAX_ENCODEABLE_XYZ * 0.25f;
    Pt[Stride]    = MAX_ENCODEABLE_XYZ * 0.50f;
    Pt[Stride*2]  = MAX_ENCODEABLE_XYZ * 0.75f;

    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};

    // Act
    cmsUInt8Number* ret = UnrollXYZFloatToFloat(&info, wIn, accumBuffer, Stride);

    // Assert
    bool ok = true;

    // Check wIn values: wIn[i] should be Pt[i] / MAX_ENCODEABLE_XYZ according to the planar path.
    if (!almostEqual(wIn[0], 0.25f)) {
        ctx.fail("PlanarBranch: wIn[0] != Pt[0] / MAX_ENCODEABLE_XYZ (expected 0.25)");
        ok = false;
    }
    if (!almostEqual(wIn[1], 0.50f)) {
        ctx.fail("PlanarBranch: wIn[1] != Pt[Stride] / MAX_ENCODEABLE_XYZ (expected 0.50)");
        ok = false;
    }
    if (!almostEqual(wIn[2], 0.75f)) {
        ctx.fail("PlanarBranch: wIn[Stride*2] != Pt[Stride*2] / MAX_ENCODEABLE_XYZ (expected 0.75)");
        ok = false;
    }

    // Check return pointer advances by sizeof(cmsFloat32Number)
    size_t expectedAdvance = sizeof(float);
    size_t actualAdvance = reinterpret_cast<char*>(ret) - reinterpret_cast<char*>(accumBuffer);
    if (actualAdvance != expectedAdvance) {
        std::stringstream ss;
        ss << "PlanarBranch: return pointer advancement mismatch (expected " << expectedAdvance
           << ", actual " << actualAdvance << ")";
        ctx.fail(ss.str());
        ok = false;
    }

    if (ok) ctx.pass("PlanarBranch: All checks passed for planar path assuming InputFormat=PLANAR_INPUT.");
}

// Test 2: Non-planar path (false branch). We drive the else-branch where
// wIn[0..2] read from Pt[0], Pt[1], Pt[2] and accum is advanced by
// sizeof(float) * (3 + T_EXTRA(info->InputFormat)). We accept both possible
// extension sizes to remain robust across implementations.
void test_NonPlanarBranch(TestContext& ctx) {
    // Arrange
    cmsTRANSFORM info;
    // Assumed non-planar InputFormat value
    const cmsUInt32Number NONPLANAR_INPUT = 1;
    info.InputFormat = NONPLANAR_INPUT;

    // Stride value is irrelevant for non-planar path, but must be positive.
    const cmsUInt32Number Stride = 4;

    // Prepare 3 floats Pt[0], Pt[1], Pt[2]
    const size_t FLOATS_NEEDED_NONPLANAR = 3;
    unsigned char accumBuffer[128];
    std::memset(accumBuffer, 0, sizeof(accumBuffer));

    float* Pt = reinterpret_cast<float*>(accumBuffer);
    // Pt values to be normalized
#ifndef MAX_ENCODEABLE_XYZ
#define MAX_ENCODEABLE_XYZ 1.0f
#endif
    Pt[0] = MAX_ENCODEABLE_XYZ * 0.20f;
    Pt[1] = MAX_ENCODEABLE_XYZ * 0.40f;
    Pt[2] = MAX_ENCODEABLE_XYZ * 0.60f;

    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};

    // Act
    cmsUInt8Number* ret = UnrollXYZFloatToFloat(&info, wIn, accumBuffer, Stride);

    // Assert
    bool ok = true;

    // wIn should be Pt[i] / MAX_ENCODEABLE_XYZ for i = 0,1,2
    if (!almostEqual(wIn[0], 0.20f)) {
        ctx.fail("NonPlanarBranch: wIn[0] != Pt[0] / MAX_ENCODEABLE_XYZ (expected 0.20)");
        ok = false;
    }
    if (!almostEqual(wIn[1], 0.40f)) {
        ctx.fail("NonPlanarBranch: wIn[1] != Pt[1] / MAX_ENCODEABLE_XYZ (expected 0.40)");
        ok = false;
    }
    if (!almostEqual(wIn[2], 0.60f)) {
        ctx.fail("NonPlanarBranch: wIn[2] != Pt[2] / MAX_ENCODEABLE_XYZ (expected 0.60)");
        ok = false;
    }

    // The function returns accum advanced by 3 * sizeof(float) plus possible extra.
    // Accept either 12 or 16 as valid advancements depending on T_EXTRA value.
    size_t diff = reinterpret_cast<char*>(ret) - reinterpret_cast<char*>(accumBuffer);
    if (diff != 12 && diff != 16) {
        std::stringstream ss;
        ss << "NonPlanarBranch: return pointer advancement unexpected (diff=" << diff
           << ", expected 12 or 16)";
        ctx.fail(ss.str());
        ok = false;
    }

    if (ok) ctx.pass("NonPlanarBranch: All checks passed for non-planar path (InputFormat != PLANAR).");
}

int main() {
    TestContext ctx;

    // Run tests
    test_PlanarBranch(ctx);
    test_NonPlanarBranch(ctx);

    // Summary
    std::cout << "\nTest Summary:\n";
    for (const auto& m : ctx.messages) {
        std::cout << m << "\n";
    }
    if (ctx.failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << ctx.failures << " test(s) FAILED.\n";
        return 1;
    }
}