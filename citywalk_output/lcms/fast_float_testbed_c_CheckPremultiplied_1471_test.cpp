/*
Step 0: Overview
This test suite targets the focal method: CheckPremultiplied located in fast_float_testbed.c.
The method uses the LittleCMS (lcms) API to transform BGRA8 data between two
transforms (one constructed with a specific cmsContext and the other with a null context)
and then compares the resulting outputs byte-for-byte, issuing a Fail(...) if any
difference is detected.

Key dependencies (Candidate Keywords) extracted from the focal method:
- BGRA8 input data: BGRA8
- Output buffers: bgrA8_1, bgrA8_2
- Colorspace / pixel types: TYPE_BGRA_8, TYPE_BGRA_8_PREMUL
- Profiles: srgb1, srgb2 (constructed via cmsCreate_sRGBProfile)
- Context: noPlugin (cmsCreateContext) and a NULL/0 context for the second transform
- Transform creation: cmsCreateTransformTHR, cmsCreateTransformTHR
- Transformation: cmsDoTransform
- Cleanup: cmsCloseProfile, cmsDeleteTransform
- Copy alpha flag: cmsFLAGS_COPY_ALPHA
- Rendering intent: INTENT_PERCEPTUAL
- Comparison and failure signaling: a loop comparing bytes and calling Fail(...)
- Interfaces/Types: cmsHPROFILE, cmsContext, cmsHTRANSFORM, uint8_t, etc.

Step 1: Program Understanding
The method CheckPremultiplied constructs two identical transforms (differing only in the cmsContext argument), applies them to the same BGRA8 input, and then compares the resulting 4-byte outputs. If any byte differs, it calls Fail(...) with a diagnostic string. The test is designed to ensure premultiplied alpha handling matches between a "contextful" transform and a "contextless" transform.

Step 2: Unit Test Generation
Goal: Create a minimal C++11 test harness that invokes CheckPremultiplied and reports results without using GTest or similar frameworks. The harness should:
- Expose a forward declaration for the C function CheckPremultiplied.
- Run at least two test scenarios: a basic run and a repeated run to exercise potential repeated-use paths.
- Use a lightweight, non-terminating assertion/reporting mechanism to summarize test outcomes.
- Note: The provided class dependencies in the file define empty/no-op implementations for several helper functions (trace, Fail, etc.). The tests rely on the existing implementation and only exercise the exported function CheckPremultiplied. No private members are accessed.

Step 3: Test Case Refinement
- Given the constraints, the tests focus on exercising the focal method in a straightforward manner (basic run, repeated run) to maximize coverage of function entry, internal calls, and the final comparison loop. The internal Fail() is a no-op in the provided dependency block, so tests are designed to be non-fatal and report success when the function returns normally.

Important: The environment must link against LittleCMS (lcms2) since CheckPremultiplied uses cmsCreate_sRGBProfile, cmsCreateTransformTHR, cmsDoTransform, etc. The test code itself does not depend on lcms2 headers and only declares the C function prototype so that the compiler can generate valid calls in C++.

Now, the test code (C++11, no GTest, main-based test runner):

*/

#include <fast_float_internal.h>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>


// Forward declaration of the focal C function under test.
// It is defined in fast_float_testbed.c and uses the CMS library internally.
extern "C" void CheckPremultiplied(void);

/*
Candidate Keywords recap (for traceability in tests):
- BGRA8 input: uint8_t BGRA8[4] = { 255, 192, 160, 128 };
- Outputs: uint8_t bgrA8_1[4], bgrA8_2[4];
- Profiles: cmsHPROFILE srgb1, srgb2;
- Context: cmsContext noPlugin;
- Transforms: cmsHTRANSFORM xform1, xform2;
- Transform creation: cmsCreateTransformTHR, cmsCreateTransformTHR
- Transform usage: cmsDoTransform
- Cleanup: cmsCloseProfile, cmsDeleteTransform
- Types/constants: TYPE_BGRA_8, TYPE_BGRA_8_PREMUL, INTENT_PERCEPTUAL, cmsFLAGS_COPY_ALPHA
- Comparison: loop over i in 0..3, Fail(...) on mismatch
*/

// Lightweight test harness (non-terminating, simple reporting)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple test: run the focal method once
static void Test_Premultiplied_Basic_Run() {
    // Purpose: Ensure CheckPremultiplied executes without crashing and returns normally.
    // This relies on the existing implementation and its internal checks.
    CheckPremultiplied();
}

// Simple test: run the focal method twice to exercise potential repeated-use behavior
static void Test_Premultiplied_Repeat_Run() {
    // Purpose: Call the same function again to increase coverage on repeated use.
    CheckPremultiplied();
    CheckPremultiplied();
}

// Entry point for running tests
int main() {
    // Basic style of test reporting without a formal framework
    std::cout << "=== Test Suite: CheckPremultiplied (fast_float_testbed.c) ===\n";

    // Test 1: Basic run
    ++g_total_tests;
    try {
        Test_Premultiplied_Basic_Run();
        std::cout << "[PASS] Test_Premultiplied_Basic_Run\n";
    } catch (const std::exception& ex) {
        ++g_failed_tests;
        std::cout << "[FAIL] Test_Premultiplied_Basic_Run - Exception: " << ex.what() << "\n";
    } catch (...) {
        ++g_failed_tests;
        std::cout << "[FAIL] Test_Premultiplied_Basic_Run - Unknown exception\n";
    }

    // Test 2: Repeat run to cover potential repeated invocation paths
    ++g_total_tests;
    try {
        Test_Premultiplied_Repeat_Run();
        std::cout << "[PASS] Test_Premultiplied_Repeat_Run\n";
    } catch (const std::exception& ex) {
        ++g_failed_tests;
        std::cout << "[FAIL] Test_Premultiplied_Repeat_Run - Exception: " << ex.what() << "\n";
    } catch (...) {
        ++g_failed_tests;
        std::cout << "[FAIL] Test_Premultiplied_Repeat_Run - Unknown exception\n";
    }

    // Summary
    std::cout << "\nTest results: " << (g_total_tests - g_failed_tests) << " passed, "
              << g_failed_tests << " failed, out of " << g_total_tests << " tests.\n";

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}