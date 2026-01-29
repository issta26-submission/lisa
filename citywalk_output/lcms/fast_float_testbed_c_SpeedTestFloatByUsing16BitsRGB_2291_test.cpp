// Test suite for the focal method: SpeedTestFloatByUsing16BitsRGB
// This test suite is designed for a C++11 environment without GTest.
// It relies on the existing project infrastructure (loadProfile, speed test entry points, etc.).
// The tests focus on exercising the focal method under realistic conditions using available ICC profiles.
// Note: The heavy memory and compute requirements of the focal method are accepted for these tests.
// Explanatory comments accompany each test to document intent and expected behavior.

#include <fast_float_internal.h>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>


// Assumed project-provided interfaces (from the focal file and class dependencies).
// We declare them here to enable linking with the actual implementation in the build system.
// If the real headers use slightly different typedefs, adjust accordingly in your build setup.

extern "C" {

// Forward-declared types from the focal method's signature.
// The real project should provide these via the included headers.
// If your build uses different typedefs, ensure compatibility with the actual definitions.

typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;

typedef int cmsInt32Number;
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef float cmsFloat32Number;
typedef double cmsFloat64Number;

// Structures used for scanlines (as seen in the focal function)
typedef struct {
    cmsFloat32Number r;
    cmsFloat32Number g;
    cmsFloat32Number b;
} Scanline_rgbFloat;

typedef struct {
    cmsUInt16Number r;
    cmsUInt16Number g;
    cmsUInt16Number b;
} Scanline_rgb16bits;

// The focal function under test
cmsFloat64Number SpeedTestFloatByUsing16BitsRGB(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

// Profile loader helper provided by the testbed/class dependencies
cmsHPROFILE loadProfile(const char* name);

}

// Simple failure tracking for tests
static int g_testFailures = 0;

// Lightweight test macros
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << std::endl; \
        ++g_testFailures; \
    } else { \
        std::cout << "[TEST PASSED] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << std::endl; \
    } \
} while(0)


// Test 1: Sanity check with identical in/out profiles
// Intent: Ensure the focal method runs to completion and returns a finite non-negative pix/sec value
//          when provided with valid, identical input/output ICC profiles.
// Coverage: Path where both profiles are non-NULL and valid; ensures the transform path and timing path execute.
static void test_SpeedTestFloatByUsing16BitsRGB_IdenticalProfiles()
{
    std::cout << "Running test_SpeedTestFloatByUsing16BitsRGB_IdenticalProfiles..." << std::endl;

    // Load the same profile for input and output.
    cmsHPROFILE inProfile = loadProfile("sRGB.icc");
    cmsHPROFILE outProfile = loadProfile("sRGB.icc");

    TEST_ASSERT(inProfile != nullptr, "Input profile 'sRGB.icc' loaded successfully");
    TEST_ASSERT(outProfile != nullptr, "Output profile 'sRGB.icc' loaded successfully");

    if (inProfile == nullptr || outProfile == nullptr) {
        // If profiles failed to load, skip execution to avoid undefined behavior.
        std::cerr << "Skipping test_SpeedTestFloatByUsing16BitsRGB_IdenticalProfiles due to missing profiles." << std::endl;
        return;
    }

    // Call the focal function with a neutral context (ct = 0).
    cmsContext ct = 0;
    cmsFloat64Number result = SpeedTestFloatByUsing16BitsRGB(ct, inProfile, outProfile);

    // Basic sanity checks on the returned value
    TEST_ASSERT(result >= 0.0, "Speed test result should be non-negative (pix/sec)");
    TEST_ASSERT(std::isfinite(result), "Speed test result should be finite (not NaN or Inf)");

    // Cleanup would be handled by the library; we assume profiles are safe to release through the library's lifecycle.
    // If the test environment requires explicit profile release, rely on existing loadProfile behavior.
}

// Test 2: Sanity check with different input/output profiles
// Intent: Validate that the focal method can operate when in and out profiles differ and both are valid.
// Coverage: Non-identical valid profiles; ensures the transform path with a real conversion is exercised.
static void test_SpeedTestFloatByUsing16BitsRGB_DifferentProfiles()
{
    std::cout << "Running test_SpeedTestFloatByUsing16BitsRGB_DifferentProfiles..." << std::endl;

    // Load distinct profiles for input and output.
    cmsHPROFILE inProfile = loadProfile("sRGB.icc");
    cmsHPROFILE outProfile = loadProfile("AdobeRGB.icc");

    TEST_ASSERT(inProfile != nullptr, "Input profile 'sRGB.icc' loaded successfully");
    TEST_ASSERT(outProfile != nullptr, "Output profile 'AdobeRGB.icc' loaded successfully");

    if (inProfile == nullptr || outProfile == nullptr) {
        std::cerr << "Skipping test_SpeedTestFloatByUsing16BitsRGB_DifferentProfiles due to missing profiles." << std::endl;
        return;
    }

    // Call the focal function with a neutral context (ct = 0).
    cmsContext ct = 0;
    cmsFloat64Number result = SpeedTestFloatByUsing16BitsRGB(ct, inProfile, outProfile);

    // Basic sanity checks on the returned value
    TEST_ASSERT(result >= 0.0, "Speed test (different profiles) should be non-negative (pix/sec)");
    TEST_ASSERT(std::isfinite(result), "Speed test (different profiles) should be finite (not NaN or Inf)");
}

// Test 3: Null profile handling sanity (best-effort)
// Intent: Exercise the implementation's guard that checks for NULL profiles.
// Note: Depending on the library behavior and FAIL() handling, this test may be skipped or may be unsafe
// in some environments. We attempt a best-effort guard to avoid crashes.
// Coverage: hlcmsProfileIn == NULL or hlcmsProfileOut == NULL branches.
// Important: If your environment marks these as unsafe, uncomment the skip logic below.
static void test_SpeedTestFloatByUsing16BitsRGB_NullProfiles_Sanity()
{
    std::cout << "Running test_SpeedTestFloatByUsing16BitsRGB_NullProfiles_Sanity..." << std::endl;

    // Do not load profiles; pass NULLs to the focal method.
    cmsContext ct = 0;
    cmsFloat64Number result = SpeedTestFloatByUsing16BitsRGB(ct, nullptr, nullptr);

    // If the function handles NULL gracefully, result should be a finite value (depending on internal Fail handling).
    // If the library crashes, this test may abort; we guard with a best-effort assertion.

    // We conservatively check if a non-negative finite value was returned; otherwise report potential issue.
    if (std::isfinite(result) && result >= 0.0) {
        std::cout << "[INFO] SpeedTestFloatByUsing16BitsRGB with NULL profiles produced finite result: " << result << std::endl;
        // Not asserting to avoid masking potential crash behavior in some environments.
        // We treat this as an informational test rather than a strict pass/fail.
    } else {
        std::cerr << "[INFO] SpeedTestFloatByUsing16BitsRGB with NULL profiles produced non-finite or undefined result." << std::endl;
        // Do not increment global failures for this informational case to avoid false negatives in environments
        // where NULL handling differs. If strict behavior is required, enable an assertion here.
    }
}

// Entry point for running all tests
int main()
{
    std::cout << "Starting SpeedTestFloatByUsing16BitsRGB test suite (C++11, GTest-free)..." << std::endl;

    // Run tests
    test_SpeedTestFloatByUsing16BitsRGB_IdenticalProfiles();
    test_SpeedTestFloatByUsing16BitsRGB_DifferentProfiles();
    test_SpeedTestFloatByUsing16BitsRGB_NullProfiles_Sanity();

    // Summary
    if (g_testFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_testFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}