/*
Step 1 - Candidate Keywords extracted from the focal method SpeedTestFloatRGB:
- cmsGetColorSpace, cmsSigRgbData, cmsSigLabData, cmsSigXYZData
- TYPE_RGB_FLT, TYPE_Lab_FLT, TYPE_XYZ_FLT
- cmsCreateTransformTHR, cmsCloseProfile, cmsDoTransform, cmsDeleteTransform
- Scanline_rgbFloat, Scanline_LabFloat
- malloc, free, clock, CLOCKS_PER_SEC, cmsInt32Number, cmsUInt32Number
- INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE
- In/Out buffer formation (RGBFloat vs LabFloat paths)
- Fail(...) path (error handling)
- Color space switch branches (in/out)
This test suite targets SpeedTestFloatRGB and its color-space dependent branches while stubbing the external cms LittleCMS-like API.
*/

// Step 2/3: Minimal, self-contained test harness for SpeedTestFloatRGB (C++11)

#include <cstdint>
#include <fast_float_internal.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// We provide C-linkage stubs for the LittleCMS-like API used by the focal method.
// The actual implementation of SpeedTestFloatRGB is assumed to be compiled/imported
// from fast_float_testbed.c (as per the problem statement).

extern "C" {

// Basic opaque types (as used by the focal function)
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;

typedef int cmsInt32Number;
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;
typedef float cmsFloat32Number;

// Color spaces (as seen in the focal method's switches)
enum {
    cmsSigRgbData = 0,
    cmsSigLabData = 1,
    cmsSigXYZData = 2
};

// Floating type format specifiers used by the transform
enum {
    TYPE_RGB_FLT = 1,
    TYPE_Lab_FLT = 2,
    TYPE_XYZ_FLT = 3
};

// Miscellaneous constants
#define INTENT_PERCEPTUAL 0
#define cmsFLAGS_NOCACHE 0

// Function prototypes used by the focal method (mocked)
cmsInt32Number cmsGetColorSpace(cmsHPROFILE);
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext, cmsHPROFILE, cmsInt32Number, cmsHPROFILE, cmsInt32Number, cmsInt32Number, cmsUInt32Number);
void cmsCloseProfile(cmsHPROFILE);
void cmsDeleteTransform(cmsHTRANSFORM);
void cmsDoTransform(cmsHTRANSFORM, void*, void*, cmsUInt32Number);

// Forward declare the SpeedTestFloatRGB under test
typedef void* Scanline_rgbFloat; // not used by host tests, but kept for compatibility
typedef void* Scanline_LabFloat;   // not used by host tests, but kept for compatibility

// The actual signature of the focal method as provided
typedef cmsFloat64Number (*SpeedTestRGBFn)(cmsContext, cmsHPROFILE, cmsHPROFILE);
cmsFloat64Number SpeedTestFloatRGB(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

}

// Lightweight C++ test harness (no GTest; just main + helper assertions)

// Simple profile wrapper used for test inputs
struct TestProfile {
    int colorSpace; // one of cmsSigRgbData, cmsSigLabData, cmsSigXYZData
    TestProfile(int cs) : colorSpace(cs) {}
};

// Lightweight assertion helpers (non-terminating)
static bool isFiniteDouble(double x) {
    return std::isfinite(x);
}

static void logTestResult(const std::string& testName, bool ok, double value) {
    std::cout << "[Test] " << testName << " -> "
              << (ok ? "PASS" : "FAIL")
              << " | Speed=" << value << "\n";
}

// --------- Mocked LittleCMS-like API implementations (very small, enough to exercise the focal method) ---------

// We provide a minimal in-process implementation of the cmsGetColorSpace and transform lifecycle.
// Note: The real code will allocate large buffers; we do not modify that behavior here.
// To avoid crashing the host tests due to memory pressure in this illustrative harness, we keep behavior deterministic but simple.

static cmsInt32Number fakeCmsGetColorSpace(cmsHPROFILE profile) {
    if (profile == nullptr) return -1;
    // The test passes a pointer to TestProfile; cast and return colorSpace
    struct TestProfile* p = reinterpret_cast<struct TestProfile*>(profile);
    if (p) return p->colorSpace;
    return -1;
}

static cmsHTRANSFORM fakeCmsCreateTransformTHR(cmsContext ctx,
                                             cmsHPROFILE inProfile,
                                             cmsInt32Number inFormatter,
                                             cmsHPROFILE outProfile,
                                             cmsInt32Number outFormatter,
                                             cmsInt32Number intent,
                                             cmsUInt32Number flags)
{
    (void)ctx; (void)inProfile; (void)inFormatter;
    (void)outProfile; (void)outFormatter; (void)intent; (void)flags;
    // Return a opaque handle (pointer) to indicate a transform exists
    return reinterpret_cast<cmsHTRANSFORM>(new int(1));
}

static void fakeCmsCloseProfile(cmsHPROFILE /*profile*/) {
    // No-op for test harness (real code would free resources)
}

static void fakeCmsDeleteTransform(cmsHTRANSFORM t) {
    if (t) {
        delete reinterpret_cast<int*>(t);
    }
}

static void fakeCmsDoTransform(cmsHTRANSFORM /*transform*/, void* /*In*/, void* /*Out*/, cmsUInt32Number /*size*/) {
    // Do nothing in mock; in real library this would apply the color transform
}

// Tiny wrapper to override actual function bindings expected by the focal method.
// By providing these with C linkage, the linker will resolve calls from SpeedTestFloatRGB to these stubs.

extern "C" {

cmsInt32Number cmsGetColorSpace(cmsHPROFILE profile) {
    return fakeCmsGetColorSpace(profile);
}

cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ctx,
                                   cmsHPROFILE inProfile,
                                   cmsInt32Number inFormatter,
                                   cmsHPROFILE outProfile,
                                   cmsInt32Number outFormatter,
                                   cmsInt32Number intent,
                                   cmsUInt32Number flags) {
    return fakeCmsCreateTransformTHR(ctx, inProfile, inFormatter, outProfile, outFormatter, intent, flags);
}

void cmsCloseProfile(cmsHPROFILE profile) {
    fakeCmsCloseProfile(profile);
}

void cmsDeleteTransform(cmsHTRANSFORM transform) {
    fakeCmsDeleteTransform(transform);
}

void cmsDoTransform(cmsHTRANSFORM transform, void* In, void* Out, cmsUInt32Number size) {
    (void)transform; // not used in mock beyond presence
    fakeCmsDoTransform(transform, In, Out, size);
}

}

// --------- Test Scenarios ---------

// Helper to create a test profile object with a specific color space
static TestProfile* makeProfile(int colorSpace) {
    return new TestProfile(colorSpace);
}

// Test 1: RGB input -> RGB output (inFormatter TYPE_RGB_FLT, outFormatter TYPE_RGB_FLT)
static bool test_SpeedTestFloatRGB_RGBtoRGB() {
    TestProfile* inP  = makeProfile(cmsSigRgbData);
    TestProfile* outP = makeProfile(cmsSigRgbData);

    // The focal function takes opaque pointers; we reinterpret them as cmsHPROFILE
    cmsFloat64Number result = SpeedTestFloatRGB(nullptr,
                                                static_cast<cmsHPROFILE>(inP),
                                                static_cast<cmsHPROFILE>(outP));

    bool ok = isFiniteDouble(static_cast<double>(result));

    logTestResult("SpeedTestFloatRGB_RGBtoRGB", ok, (double)result);

    // Cleanup (the fake cmsCloseProfile is a no-op in this harness)
    delete inP;
    delete outP;
    return ok;
}

// Test 2: RGB input -> Lab output (inFormatter TYPE_RGB_FLT, outFormatter TYPE_Lab_FLT)
// This exercises the Lab output path as defined in the focal method.
static bool test_SpeedTestFloatRGB_RGBtoLab() {
    TestProfile* inP  = makeProfile(cmsSigRgbData);
    TestProfile* outP = makeProfile(cmsSigLabData);

    cmsFloat64Number result = SpeedTestFloatRGB(nullptr,
                                                static_cast<cmsHPROFILE>(inP),
                                                static_cast< cmsHPROFILE >(outP));

    bool ok = isFiniteDouble(static_cast<double>(result));

    logTestResult("SpeedTestFloatRGB_RGBtoLab", ok, (double)result);

    delete inP;
    delete outP;
    return ok;
}

// Test 3: Lab input -> XYZ output (inFormatter TYPE_Lab_FLT, outFormatter TYPE_XYZ_FLT)
// This exercises the Lab input path and XYZ output path.
static bool test_SpeedTestFloatRGB_LabToXYZ() {
    TestProfile* inP  = makeProfile(cmsSigLabData);
    TestProfile* outP = makeProfile(cmsSigXYZData);

    cmsFloat64Number result = SpeedTestFloatRGB(nullptr,
                                                static_cast<cmsHPROFILE>(inP),
                                                static_cast< cmsHPROFILE >(outP));

    bool ok = isFiniteDouble(static_cast<double>(result));

    logTestResult("SpeedTestFloatRGB_LabToXYZ", ok, (double)result);

    delete inP;
    delete outP;
    return ok;
}

// --------- Main runner ---------

int main() {
    // Run the test scenarios. They are designed to ensure we traverse:
    // - RGB->RGB
    // - RGB->Lab
    // - Lab->XYZ
    // We rely on the function returning a finite speed value; this validates
    // that the code path executes through the main branches without crashing.

    bool passedAll = true;

    if (!test_SpeedTestFloatRGB_RGBtoRGB()) {
        passedAll = false;
    }
    if (!test_SpeedTestFloatRGB_RGBtoLab()) {
        passedAll = false;
    }
    if (!test_SpeedTestFloatRGB_LabToXYZ()) {
        passedAll = false;
    }

    if (passedAll) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}

/*
Notes and rationale:
- The tests exercise multiple branches of the focal method SpeedTestFloatRGB by feeding distinct color spaces for input and output:
  1) RGB input with RGB output
  2) RGB input with Lab output
  3) Lab input with XYZ output
- We use a minimal mock of the LittleCMS-like API to avoid dependencies on the real library while preserving
  the function call surface that SpeedTestFloatRGB expects.
- We intentionally do not terminate on assertion failures (non-terminating style) and instead report pass/fail
  per test via logTestResult, allowing all tests to run in a single invocation.
- This test suite assumes that the project compiles SpeedTestFloatRGB from fast_float_testbed.c and links against
  the provided mock CMS APIs defined here.
- The approach follows the domain knowledge guidance:
  - Import dependencies via the test harness (the harness provides prototypes and mocks).
  - Cover true branches (valid color spaces) and ensure no early termination (non-terminating checks).
  - Static helper data (TestProfile) is used purely in tests; no private members are accessed.
  - Use standard library functionality only; no GMock/GTest is used.
*/