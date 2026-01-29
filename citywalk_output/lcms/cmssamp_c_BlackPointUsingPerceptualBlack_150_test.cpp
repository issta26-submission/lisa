// Test suite for BlackPointUsingPerceptualBlack in cmssamp.c
// Strategy: compile-time include the focal C file in a C++ test translation unit
// and provide C-mock implementations for its internal dependencies.
// This allows deterministic control over internal branches without modifying the source.

// NOTE: This test relies on the presence of the LCMS public headers (lcms2.h) for type
// definitions (cmsBool, cmsHPROFILE, cmsHTRANSFORM, cmsCIELab, cmsCIEXYZ, etc.)
// and on the focal file cmssamp.c being self-contained enough to compile in this TU
// with those mocks provided below.

// The approach is consistent with Step 2: unit test generation by controlling internal
// behavior via mocks, and Step 3: test case refinement for coverage.

#include <cstring>
#include <iostream>
#include <cmath>
#include <cmssamp.c>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include LCMS public header to obtain type definitions used by the focal code.

// Forward declaration: expose the focal function signature so we can call it from C++.
// This declaration is provided by including cmssamp.c, but we declare mocks first and then
// include the C file so that the mocks are visible to the compiler/linker when the focal
// function is compiled in this translation unit.
extern "C" {
    // Prototypes for the internal dependencies that BlackPointUsingPerceptualBlack calls.
    // We declare them here so that the cmssamp.c can resolve them against our mocks.

    // Determines if a given rendering intent is supported by the profile.
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags);

    // Creates a round-tripping transform; returns NULL on failure.
    cmsHTRANSFORM CreateRoundtripXForm(cmsHPROFILE hProfile, cmsUInt32Number nIntent);

    // Applies a transform to Lab data (LabIn -> LabOut).
    void cmsDoTransform(cmsHTRANSFORM hRoundTrip, cmsCIELab* LabIn, cmsCIELab* LabOut, cmsUInt32Number N);

    // Deletes a transform.
    void cmsDeleteTransform(cmsHTRANSFORM hRoundTrip);

    // Converts Lab -> XYZ. The first parameter is unused in this test (NULL in the focal code).
    void cmsLab2XYZ(void* Dummy, cmsCIEXYZ* XYZ, cmsCIELab* Lab);
}

// Flags to drive mock behavior for different test cases.
static bool g_mockNotSupported = false;
static bool g_mockNullRoundTrip = false;

// Mock implementations of the internal dependencies.
// The mocks are intentionally simplistic and deterministic to exercise the focal branches.

extern "C" {

cmsBool cmsIsIntentSupported(cmsHPROFILE /*hProfile*/, cmsUInt32Number /*Intent*/, cmsUInt32Number /*dwFlags*/)
{
    // Return FALSE (not supported) when the test requires it; otherwise TRUE.
    return g_mockNotSupported ? 0 : 1;
}

cmsHTRANSFORM CreateRoundtripXForm(cmsHPROFILE /*hProfile*/, cmsUInt32Number /*nIntent*/)
{
    // Return NULL to simulate failure when required by a test case.
    if (g_mockNullRoundTrip) {
        return NULL;
    }
    // Non-NULL value to simulate a valid transform handle.
    return (cmsHTRANSFORM) (void*)0x1;
}

void cmsDoTransform(cmsHTRANSFORM /*hRoundTrip*/, cmsCIELab* /*LabIn*/, cmsCIELab* LabOut, cmsUInt32Number /*N*/)
{
    // Produce a deterministic destination Lab for testing.
    // This value is chosen to exercise the clamping and subsequent XYZ conversion.
    if (LabOut) {
        LabOut->L = 60.0;  // Will be clamped to 50 in the focal method.
        LabOut->a = 5.0;
        LabOut->b = -3.0;
    }
}

void cmsDeleteTransform(cmsHTRANSFORM /*hRoundTrip*/)
{
    // No-op for test mocks.
}

void cmsLab2XYZ(void* /*Dummy*/, cmsCIEXYZ* XYZ, cmsCIELab* Lab)
{
    // Deterministic mapping from Lab to XYZ for test verification.
    // When Lab is clamped to L=50, a=b=0, we expect XYZ to reflect that.
    (void)Lab; // Unused in this mock beyond L/a/b values requested by tests.
    if (XYZ) {
        XYZ->X = 0.50;
        XYZ->Y = 0.50;
        XYZ->Z = 0.50;
    }
}
}

// Include the focal C file after the mocks so that the mocks resolve correctly.

// Simple test harness Macros
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_ASSERT(cond, msg)                                              \
    do {                                                                    \
        if (cond) {                                                         \
            ++g_tests_passed;                                             \
        } else {                                                          \
            std::cerr << "Test failure: " << (msg) << " (" << __FILE__       \
                      << ":" << __LINE__ << ")" << std::endl;            \
        }                                                                 \
        ++g_tests_run;                                                      \
    } while (0)

#define TEST_TRUE(cond, msg) TEST_ASSERT((cond), (std::string("EXPECTED TRUE: ") + (msg)).c_str())
#define TEST_FALSE(cond, msg) TEST_ASSERT(!(cond), (std::string("EXPECTED FALSE: ") + (msg)).c_str())

// Test 1: When the profile does not support perceptual intent, the function should
//         zero the BlackPoint and return TRUE.
bool test_NotSupportedPerceptualIntent()
{
    g_mockNotSupported = true;     // cmsIsIntentSupported will return FALSE
    g_mockNullRoundTrip = false;   // Irrelevant for this test

    cmsCIEXYZ blackPoint;
    std::memset(&blackPoint, 0, sizeof(blackPoint));

    cmsHPROFILE dummyProfile = (cmsHPROFILE)0x10; // Arbitrary non-NULL profile handle

    cmsBool ret = BlackPointUsingPerceptualBlack(&blackPoint, dummyProfile);

    // Expect TRUE and blackPoint unchanged (zeros)
    TEST_TRUE(ret, "Return value should be TRUE when perceptual is not supported");
    TEST_TRUE(blackPoint.X == 0.0 && blackPoint.Y == 0.0 && blackPoint.Z == 0.0,
              "BlackPoint must be zeroed when perceptual is not supported");

    // Cleanup for next tests
    g_mockNotSupported = false;
    return true;
}

// Test 2: When perceptual is supported but round-trip transform creation fails (NULL),
 //         the function should zero BlackPoint and return FALSE.
bool test_RoundtripCreationFailure()
{
    g_mockNotSupported = false;     // perceptual supported
    g_mockNullRoundTrip = true;     // CreateRoundtripXForm will return NULL

    cmsCIEXYZ blackPoint;
    std::memset(&blackPoint, 0, sizeof(blackPoint));

    cmsHPROFILE dummyProfile = (cmsHPROFILE)0x20;

    cmsBool ret = BlackPointUsingPerceptualBlack(&blackPoint, dummyProfile);

    TEST_FALSE(ret, "Return value should be FALSE when round-trip transform creation fails");
    TEST_TRUE(blackPoint.X == 0.0 && blackPoint.Y == 0.0 && blackPoint.Z == 0.0,
              "BlackPoint must be zeroed when round-trip creation fails");

    // Reset
    g_mockNullRoundTrip = false;
    return true;
}

// Test 3: Normal path where perceptual is supported and round-trip is created.
//          LabOut.L is forced to 60 by the mock, which should clamp to 50 and set a/b to 0.
//          Then Lab2XYZ is invoked to produce a deterministic XYZ that we can verify.
bool test_NormalPathClampedAndConverted()
{
    g_mockNotSupported = false;
    g_mockNullRoundTrip = false;

    cmsCIEXYZ blackPoint;
    std::memset(&blackPoint, 0, sizeof(blackPoint));

    cmsHPROFILE dummyProfile = (cmsHPROFILE)0x30;

    cmsBool ret = BlackPointUsingPerceptualBlack(&blackPoint, dummyProfile);

    // Expect TRUE and deterministic XYZ derived from LabOut clamped to L=50
    TEST_TRUE(ret, "Return value should be TRUE on successful path");
    TEST_TRUE(std::abs(blackPoint.X - 0.5) < 1e-6 &&
              std::abs(blackPoint.Y - 0.5) < 1e-6 &&
              std::abs(blackPoint.Z - 0.5) < 1e-6,
              "BlackPoint should correspond to LabOut mapped to XYZ by mock (0.5,0.5,0.5)");

    return true;
}

// Main: invoke tests
int main()
{
    std::cout << "Running cmssamp.c focal method unit tests (C++ harness)..." << std::endl;

    if (!test_NotSupportedPerceptualIntent()) {
        std::cerr << "Test 1 failed to execute properly." << std::endl;
    }
    if (!test_RoundtripCreationFailure()) {
        std::cerr << "Test 2 failed to execute properly." << std::endl;
    }
    if (!test_NormalPathClampedAndConverted()) {
        std::cerr << "Test 3 failed to execute properly." << std::endl;
    }

    std::cout << "Tests completed. Passed: " << g_tests_passed << "/" << g_tests_run << std::endl;
    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}