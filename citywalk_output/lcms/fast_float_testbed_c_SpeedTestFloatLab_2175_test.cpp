// fast_float_test.cpp
// Minimal C++11 test harness for SpeedTestFloatLab, using project-provided
// dependencies and a lightweight, non-terminating assertion approach.
// This test does not rely on GTest; it uses a simple main() and prints PASS/FAIL.
//
// The test uses a lightweight CMS API mock to drive SpeedTestFloatLab through
// both RGB and XYZ output formatter paths by simulating the cms* API functions
// that SpeedTestFloatLab calls ( cmsGetColorSpace, cmsCreateTransformTHR, cmsDoTransform, etc ).
// The mocks are implemented with C linkage to match the focal method's expectations.

#include <thread>
#include <fast_float_internal.h>
#include <chrono>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Forward declaration of the focal function under test (assumed to be linked in the build).
extern "C" double SpeedTestFloatLab(void* ct, void* hlcmsProfileIn, void* hlcmsProfileOut);

// Lightweight, non-terminating test assertion helper
static void assert_test(bool condition, const char* test_desc, int& fail_count) {
    if (condition) {
        std::cout << "[PASS] " << test_desc << "\n";
    } else {
        std::cout << "[FAIL] " << test_desc << "\n";
        ++fail_count;
    }
}

// Mocked CMS API surface to drive SpeedTestFloatLab without requiring the real library.
// We provide C linkage so that the focal method calls resolve to these implementations.
extern "C" {

// Typedefs mirroring the minimal subset used by SpeedTestFloatLab
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef int   cmsInt32Number;
typedef unsigned int cmsUInt32Number;
typedef float cmsFloat32Number;
typedef double cmsFloat64Number;

// Color space signatures (simplified values for testing)
static const unsigned int cmsSigLabData = 0x6C616220; // "lab "
static const unsigned int cmsSigRgbData = 0x52474200; // "RGB\0"
static const unsigned int cmsSigXYZData = 0x58595A00; // "XYZ\0"

// Type and flag constants used by the focal method
static const unsigned int TYPE_Lab_FLT = 1;
static const unsigned int TYPE_RGB_FLT = 2;
static const unsigned int TYPE_XYZ_FLT = 3;
static const unsigned int INTENT_PERCEPTUAL = 0;
static const unsigned int cmsFLAGS_NOCACHE = 0;

// Forward declarations of the minimal prototypes that SpeedTestFloatLab relies on
cmsInt32Number cmsGetColorSpace(cmsHPROFILE);
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext, cmsHPROFILE, cmsUInt32Number, cmsHPROFILE, cmsUInt32Number, cmsInt32Number, cmsUInt32Number);
void cmsCloseProfile(cmsHPROFILE);
void cmsDoTransform(cmsHTRANSFORM, const void* In, void* Out, cmsUInt32Number Size);
void cmsDeleteTransform(cmsHTRANSFORM);

}

// Provide a couple of test profiles as unique, identifiable addresses
static char InProfileLabObj;
static char OutProfileRgbObj;
static char OutProfileXYZObj;

// Expose the declarations to the linker (as if they were provided by the actual CMS library)
extern "C" {

cmsInt32Number cmsGetColorSpace(cmsHPROFILE h) {
    // Return color spaces based on the address identity of the profile pointers
    if (h == (cmsHPROFILE)&InProfileLabObj)  return cmsSigLabData;
    if (h == (cmsHPROFILE)&OutProfileRgbObj) return cmsSigRgbData;
    if (h == (cmsHPROFILE)&OutProfileXYZObj) return cmsSigXYZData;
    // Default/unrecognized
    return 0;
}

cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ctx,
                                   cmsHPROFILE InProfile, cmsUInt32Number TypeIn,
                                   cmsHPROFILE OutProfile, cmsUInt32Number TypeOut,
                                   cmsInt32Number Intent, cmsUInt32Number Flags) {
    // Return a fake non-null handle to indicate a transform was created
    (void)ctx; (void)InProfile; (void)TypeIn;
    (void)OutProfile; (void)TypeOut; (void)Intent; (void)Flags;
    return (cmsHTRANSFORM)0x1;
}

void cmsCloseProfile(cmsHPROFILE h) {
    (void)h;
    // No-op in mock
}

void cmsDoTransform(cmsHTRANSFORM h, const void* In, void* Out, cmsUInt32Number Size) {
    (void)h;
    (void)In;
    (void)Out;
    (void)Size;
    // Simulate some processing time to avoid zero seconds; adjust to create a visible speed
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
}

void cmsDeleteTransform(cmsHTRANSFORM h) {
    (void)h;
    // No-op in mock
}
} // extern "C"

int main() {
    // Simple harness that exercises SpeedTestFloatLab through two distinct output color spaces:
    // - RGB (to trigger TYPE_RGB_FLT path)
    // - XYZ (to trigger TYPE_XYZ_FLT path)
    //
    // We rely on the project-provided implementation of SpeedTestFloatLab to be linked in.
    // We use mock CMS API functions above to control behavior deterministically.

    // Create fake profile handles
    cmsHPROFILE InLab   = (cmsHPROFILE)&InProfileLabObj;
    cmsHPROFILE OutRgb  = (cmsHPROFILE)&OutProfileRgbObj;
    cmsHPROFILE OutXYZ  = (cmsHPROFILE)&OutProfileXYZObj;

    int failures = 0;

    std::cout << "Starting SpeedTestFloatLab unit tests (mock CMS API)..." << std::endl;

    // Test 1: Lab input -> RGB output path
    // This should exercise the "case cmsSigRgbData" branch for the output formatter.
    double sp_rgb = SpeedTestFloatLab(nullptr, InLab, OutRgb);
    assert_test(std::isfinite(sp_rgb) && sp_rgb > 0.0, "SpeedTestFloatLab: Lab->RGB returns finite positive speed", failures);

    // Test 2: Lab input -> XYZ output path
    // This should exercise the "case cmsSigXYZData" branch for the output formatter.
    double sp_xyz = SpeedTestFloatLab(nullptr, InLab, OutXYZ);
    assert_test(std::isfinite(sp_xyz) && sp_xyz > 0.0, "SpeedTestFloatLab: Lab->XYZ returns finite positive speed", failures);

    // Summary
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}