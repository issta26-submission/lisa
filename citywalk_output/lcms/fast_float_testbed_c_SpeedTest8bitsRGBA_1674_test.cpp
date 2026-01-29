// Unit test suite for the focal method SpeedTest8bitsRGBA (fast_float_testbed.c)
// This test harness uses a lightweight, non-GTest approach suitable for C++11.
// It relies on stubbed CMS utility functions to isolate SpeedTest8bitsRGBA from
// external dependencies and focus on basic behavior verification.
//
// Key ideas (Initialized from the focal method):
// - Validates that given non-null input/output profiles, a transform is created and used.
// - Allocates a large in-memory buffer of 256^3 RGBA8 pixels (approx 64 MB).
// - Invokes a transform over all 256^3 pixel combinations, measures time, and computes MPix/sec.
// - Returns a double value representing the performance metric.
// - We cannot easily assert on the exact content of the transformed buffer, but we can
//   assert that the function returns a finite, non-negative value when profiles are valid.
//
// This test provides minimal, non-intrusive checks:
// 1) Non-negativity of the result with valid profiles.
// 2) Finite result (not NaN or Inf) with valid profiles.
// It avoids testing the internal branches that would require manipulating static/private internals.

/* Build notes:
   - This test file is written to be compiled alongside the C file that implements
     SpeedTest8bitsRGBA (fast_float_testbed.c). 
   - It provides C-style stubs for the cms* API used by the focal method so that the
     test can run in isolation without the real LittleCMS library.
   - The test does not rely on GTest; it uses a tiny in-process test harness.
*/

#include <cstdint>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdarg>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Forward declaration of the focal function (C linkage expected)
extern "C" {
    // Typedefs mirroring the common cms* types used by the focal code.
    typedef void* cmsContext;
    typedef void* cmsHPROFILE;
    typedef void* cmsHTRANSFORM;
    typedef unsigned char cmsUInt8Number;
    typedef unsigned int cmsUInt32Number;
    typedef int cmsInt32Number;
    typedef double cmsFloat64Number;
    typedef unsigned short cmsUInt16Number;

    // Constants used by the focal code
    #define TYPE_RGBA_8 0
    #define INTENT_PERCEPTUAL 0
    #define cmsFLAGS_NOCACHE 0

    // Forward declaration of the symbol under test
    cmsFloat64Number SpeedTest8bitsRGBA(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

    // Stubbed declarations for the LittleCMS-like API used by the focal method
    typedef cmsHTRANSFORM (*_cmsCreateTransformTHR_fptr)(cmsContext, cmsHPROFILE, int, cmsHPROFILE, int, int, unsigned int);
    extern "C" cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE InProfile, int TypeIn, cmsHPROFILE OutProfile, int TypeOut, int Intent, unsigned int Flags);
    extern "C" void cmsCloseProfile(cmsHPROFILE hProfile);
    extern "C" void cmsDoTransform(cmsHTRANSFORM hTransform, const void* In, void* Out, cmsUInt32Number InPix);
    extern "C" void cmsDeleteTransform(cmsHTRANSFORM hTransform);
    extern "C" cmsFloat64Number MPixSec(cmsFloat64Number diff);

    // A fallback Fail() used by the focal code; in tests we implement a no-op to avoid termination.
    extern "C" void Fail(const char* frm, ...);
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define CHECK(cond, msg) do {                                         \
    ++g_tests_run;                                                       \
    if (!(cond)) {                                                       \
        ++g_tests_failed;                                                \
        std::cerr << "[FAIL] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } else {                                                             \
        std::cout << "[PASS] " << msg << std::endl;                    \
    }                                                                    \
} while (0)

// Stubbed implementations to isolate the focal method from the real CMS library.
// Use C linkage to match the focal code's expectations.
extern "C" {

// Simple no-op implementation of Fail to avoid terminating tests.
void Fail(const char* frm, ...)
{
    // Consume variadic arguments but do nothing to keep tests running.
    (void)frm;
}

// Return a non-null dummy transform handle
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE InProfile, int TypeIn, cmsHPROFILE OutProfile, int TypeOut, int Intent, unsigned int Flags)
{
    (void)ContextID; (void)InProfile; (void)TypeIn; (void)OutProfile; (void)TypeOut; (void)Intent; (void)Flags;
    return (cmsHTRANSFORM)0x1;
}

// No-op profile close
void cmsCloseProfile(cmsHPROFILE hProfile)
{
    (void)hProfile;
}

// No-op transform operation
void cmsDoTransform(cmsHTRANSFORM hTransform, const void* In, void* Out, cmsUInt32Number InPix)
{
    (void)hTransform; (void)In; (void)Out; (void)InPix;
    // Simulate minimal work to resemble a transform runtime cost without real processing.
}

// No-op transform delete
void cmsDeleteTransform(cmsHTRANSFORM hTransform)
{
    (void)hTransform;
}

// MPixSec simply forwards the diff; the real computation isn't necessary for these tests.
cmsFloat64Number MPixSec(cmsFloat64Number diff)
{
    return diff;
}
} // extern "C"

// Test 1: Ensure non-null profiles produce a non-negative performance result.
void test_SpeedTest8bitsRGBA_nonNullProfiles()
{
    void* ct = nullptr;
    cmsHPROFILE inProfile = reinterpret_cast<cmsHPROFILE>(0x10);   // non-null dummy handle
    cmsHPROFILE outProfile = reinterpret_cast<cmsHPROFILE>(0x20);  // non-null dummy handle

    cmsFloat64Number res = SpeedTest8bitsRGBA(reinterpret_cast<cmsContext>(ct), inProfile, outProfile);
    CHECK(res >= 0.0, "SpeedTest8bitsRGBA returns value >= 0 for non-null profiles");
}

// Test 2: Ensure result is finite (not NaN or Inf) for valid input.
void test_SpeedTest8bitsRGBA_resultIsFinite()
{
    void* ct = nullptr;
    cmsHPROFILE inProfile = reinterpret_cast<cmsHPROFILE>(0x30);   // non-null dummy handle
    cmsHPROFILE outProfile = reinterpret_cast<cmsHPROFILE>(0x40);  // non-null dummy handle

    cmsFloat64Number res = SpeedTest8bitsRGBA(reinterpret_cast<cmsContext>(ct), inProfile, outProfile);
    bool isFinite = std::isfinite(res);
    CHECK(isFinite, "SpeedTest8bitsRGBA result should be finite (not NaN/Inf)");
}

// Entry point: run tests from main, as per domain guidance (no GTest usage)
int main()
{
    std::cout << "Starting SpeedTest8bitsRGBA unit tests (non-GTest harness)..." << std::endl;

    test_SpeedTest8bitsRGBA_nonNullProfiles();
    test_SpeedTest8bitsRGBA_resultIsFinite();

    std::cout << "Tests run: " << g_tests_run << " | Failures: " << g_tests_failed << std::endl;

    // If any test failed, return non-zero to indicate failure to the caller.
    return (g_tests_failed == 0) ? 0 : 1;
}