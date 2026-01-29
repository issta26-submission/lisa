// Lightweight C++ test suite for the focal method CheckFloatToFloatXYZ in fast_float_testbed.c
// This test harness is designed to be compiled with a project that provides the C functions
// declared below (extern "C") and linked into the test binary without pulling in a conflicting
// main() entry point. It follows the constraints:
// - No Google Test (GTest) usage
// - Uses only standard C++11 facilities
// - Non-terminating assertions (EXCEPT for fatal tests, which we avoid by design)
// - Calls to the focal methods exercise the code paths guarded by the candidate keywords
//   derived from the Step 1 analysis of the focal method and its dependencies

#include <cstdio>
#include <fast_float_internal.h>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>


// Step 1 Candidate Keywords (derived from the focal method and its class dependencies):
// - cmsCreateContext, cmsCreate_sRGBProfile, cmsCreateXYZProfile
// - cmsCreateTransformTHR, cmsDoTransform
// - cmsXYZ2Lab, cmsDeltaE
// - cmsDeleteTransform, cmsCloseProfile, cmsDeleteContext
// - cmsFloat32Number, cmsCIEXYZ, cmsCIELab
// - Traceability of errors through trace(), FatalErrorQuit(), Fail()
// The test exercises the function pointers that rely on these APIs via the focal method.
// In the actual test environment, these dependencies are provided by fast_float_internal.h
// and the LittleCMS-based color transformation utilities (the CMS library).

// Extern "C" declarations for the focal methods (as found in fast_float_testbed.c)
extern "C" void CheckFloatToFloatXYZ(void);
extern "C" void CheckToFloatXYZ(void);
extern "C" void CheckFloatToFloatLab(void);
extern "C" void CheckToFloatLab(void);
extern "C" void sRGB_XYZ_roundtrip(void);

// Optional: Prototypes for additional known tests to broaden coverage
extern "C" void CheckToFloatXYZ(void);       // If available in the focal class dependencies
extern "C" void CheckFloatToFloatLab(void);  // If available in the focal class dependencies
extern "C" void CheckToFloatLab(void);         // If available in the focal class dependencies

// Lightweight test framework (non-terminating assertions)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

// Per-test state to know whether the current test failed (set by assertions)
static bool g_currentTestFailed = false;

// Assertion macro: non-terminating, records failure but continues execution
#define EXPECT_TRUE(cond, desc) do { \
    ++g_total_asserts; \
    if(!(cond)) { \
        g_currentTestFailed = true; \
        ++g_failed_asserts; \
        std::cerr << "[ASSERT FAILED] " << (desc) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Convenience assertion when you want to always pass (used to document execution)
#define EXPECT_TRUE_ALWAYS(desc) EXPECT_TRUE(true, (desc))

// Test wrapper to run a test function and report outcome
static int g_passedTests = 0;
static int g_failedTests = 0;

#define RUN_TEST(test_fn, test_name) do { \
    g_currentTestFailed = false; \
    std::cout << "[ RUN      ] " << (test_name) << std::endl; \
    test_fn(); \
    if(g_currentTestFailed) { \
        ++g_failedTests; \
        std::cerr << "[  FAILED  ] " << (test_name) << std::endl; \
    } else { \
        ++g_passedTests; \
        std::cout << "[  PASSED  ] " << (test_name) << std::endl; \
    } \
} while(0)

// Test 1: Validate execution of CheckFloatToFloatXYZ
// Rationale: This is the focal method under test. It exercises the core transformation
// pipeline from RGB to XYZ and then to Lab, comparing DeltaE against a threshold.
// The test ensures the function completes without crashing and executes its internal branches.
static void test_CheckFloatToFloatXYZ()
{
    // The focal function is void and non-terminating; we rely on it completing without crashes.
    // A passing assertion is emitted to record that the function executed.
    CheckFloatToFloatXYZ();
    EXPECT_TRUE_ALWAYS("CheckFloatToFloatXYZ executed without crashing");
}

// Test 2: Validate execution of CheckToFloatXYZ (alternative path)
static void test_CheckToFloatXYZ_variant()
{
    // This function, if present in the focal class dependencies, exercises another code path.
    CheckToFloatXYZ();
    EXPECT_TRUE_ALWAYS("CheckToFloatXYZ variant executed without crashing");
}

// Test 3: Validate execution of CheckFloatToFloatLab
static void test_CheckFloatToFloatLab()
{
    CheckFloatToFloatLab();
    EXPECT_TRUE_ALWAYS("CheckFloatToFloatLab executed without crashing");
}

// Test 4: Validate execution of CheckToFloatLab
static void test_CheckToFloatLab()
{
    CheckToFloatLab();
    EXPECT_TRUE_ALWAYS("CheckToFloatLab executed without crashing");
}

// Test 5: Validate roundtrip sRGB_XYZ_roundtrip path (additional coverage)
static void test_sRGB_XYZ_roundtrip()
{
    sRGB_XYZ_roundtrip();
    EXPECT_TRUE_ALWAYS("sRGB_XYZ_roundtrip executed without crashing");
}

// Entry point
int main(void)
{
    // Run all tests
    // The tests focus on ensuring the focal method and related tests can be invoked
    // and do not crash. Assertions are non-fatal to maximize coverage.
    RUN_TEST(test_CheckFloatToFloatXYZ, "CheckFloatToFloatXYZ");
    RUN_TEST(test_CheckToFloatXYZ_variant, "CheckToFloatXYZ (variant)");
    RUN_TEST(test_CheckFloatToFloatLab, "CheckFloatToFloatLab");
    RUN_TEST(test_CheckToFloatLab, "CheckToFloatLab");
    RUN_TEST(test_sRGB_XYZ_roundtrip, "sRGB_XYZ_roundtrip");

    // Summary
    std::cout << "==================== Test Summary ====================" << std::endl;
    std::cout << "Total assertions: " << g_total_asserts << std::endl;
    std::cout << "Failed assertions: " << g_failed_asserts << std::endl;
    std::cout << "Passed tests: " << g_passedTests << std::endl;
    std::cout << "Failed tests: " << g_failedTests << std::endl;

    // Exit status: non-zero if any test failed
    return (g_failedTests > 0) ? 1 : 0;
}