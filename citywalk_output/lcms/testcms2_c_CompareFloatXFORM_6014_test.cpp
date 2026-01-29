// High-quality C++11 unit tests for the focal method:
// CompareFloatXFORM in the file testcms2.c
// This test suite does not use GTest. It uses a small, handcrafted test harness
// and relies on the LittleCMS API exposed via headers present in the project.
// The tests are designed to exercise true/false branches of CompareFloatXFORM
// by constructing real cmsHTRANSFORM objects using the project-provided profile
// builders (Create_AboveRGB, Create_Gray22, Create_Gray30) and the floating-point
// pixel formats. Each test includes explanatory comments describing intent and
// expectations.

#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>


// Include LittleCMS header to obtain types and API declarations.
// If the environment uses a different include path, adjust accordingly.

// Declarations for the focal method and profile builders provided by the project.
// The focal method is defined in testcms2.c and uses C linkage.
extern "C" cmsInt32Number CompareFloatXFORM(cmsHTRANSFORM xform1, cmsHTRANSFORM xform2, cmsInt32Number nChan);

// Profile builder helpers provided in the project (presumably in testcms2.*).
extern "C" cmsHPROFILE Create_AboveRGB(void);
extern "C" cmsHPROFILE Create_Gray22(void);
extern "C" cmsHPROFILE Create_Gray30(void);

// Helper: simple test harness utilities (log and result aggregation)
static int g_total = 0;
static int g_failed = 0;

static void log_pass(const char* testName) {
    std::cout << "[PASS] " << testName << std::endl;
}
static void log_fail(const char* testName, const char* reason) {
    std::cerr << "[FAIL] " << testName << " - " << reason << std::endl;
    ++g_failed;
}
#define TEST_START(name) const char* testName = name; ++g_total;
#define ASSERT_TRUE(cond, reason) \
    do { if (cond) { log_pass(testName); } else { log_fail(testName, reason); } } while(0)


// Test 1: Identity-like floating-point transforms should produce identical outputs
// for every input sample. We construct two identical transforms (same input/output
// profiles and same pixel format) and verify that CompareFloatXFORM returns 1.
// Rationale: If the two xforms are effectively the same, their per-channel
// transformed outputs for all channels across the full input range must match
// within the fixed-point tolerance used by IsGoodFixed15_16.
static void test_compare_float_xform_identity_transforms(void)
{
    TEST_START("test_compare_float_xform_identity_transforms");

    // Create two transforms that should be functionally identical.
    cmsHPROFILE hIn  = Create_AboveRGB(); // Input profile (RGB)
    cmsHPROFILE hOut = Create_Gray22();    // Output profile (Gray22)

    if (hIn == NULL || hOut == NULL) {
        log_fail(testName, "Failed to create input/output profiles");
        // Do not proceed; record as failure and return
        if (hIn) cmsCloseProfile(hIn);
        if (hOut) cmsCloseProfile(hOut);
        return;
    }

    // Pixel formats assumed to be floating-point per channel for 3-channel input.
    // The exact macro names depend on the library; these are the common ones used
    // to request 32-bit floating-point per channel transforms.
    cmsUInt32Number fmtRGB_FLT  = TYPE_RGB_FLT;
    cmsUInt32Number fmtGRAY_FLT = TYPE_GRAY_FLT;
    cmsInt32Number intent = INTENT_PERCEPTUAL;

    cmsHTRANSFORM xform1 = cmsCreateTransform(hIn,  fmtRGB_FLT,
                                              hOut, fmtGRAY_FLT,
                                              intent, 0);

    cmsHTRANSFORM xform2 = cmsCreateTransform(hIn,  fmtRGB_FLT,
                                              hOut, fmtGRAY_FLT,
                                              intent, 0);

    if (xform1 == NULL || xform2 == NULL) {
        log_fail(testName, "Failed to create transforms for identity test");
        if (xform1) cmsDeleteTransform(xform1);
        if (xform2) cmsDeleteTransform(xform2);
        cmsCloseProfile(hIn);
        cmsCloseProfile(hOut);
        return;
    }

    // We test with 3 channels as a representative case (RGB-like).
    cmsInt32Number nChan = 3;
    cmsInt32Number result = CompareFloatXFORM(xform1, xform2, nChan);

    // Cleanup
    cmsDeleteTransform(xform1);
    cmsDeleteTransform(xform2);
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);

    // Expect success (non-zero -> 1)
    if (result == 1) {
        log_pass(testName);
    } else {
        log_fail(testName, "CompareFloatXFORM did not return 1 for identical transforms");
        ++g_failed;
    }
}


// Test 2: Different destination (Gray22 vs Gray30) should yield a non-matching pair.
// We expect CompareFloatXFORM to return 0, since the two transforms produce
// different outputs for at least some channel/input combination within the
// tested range.
static void test_compare_float_xform_different_destinations(void)
{
    TEST_START("test_compare_float_xform_different_destinations");

    cmsHPROFILE hIn  = Create_AboveRGB(); // Input RGB profile
    cmsHPROFILE hOut1 = Create_Gray22();  // Destination Gray22
    cmsHPROFILE hOut2 = Create_Gray30();  // Destination Gray30 (different)

    if (hIn == NULL || hOut1 == NULL || hOut2 == NULL) {
        log_fail(testName, "Failed to create input/output profiles");
        if (hIn) cmsCloseProfile(hIn);
        if (hOut1) cmsCloseProfile(hOut1);
        if (hOut2) cmsCloseProfile(hOut2);
        return;
    }

    // Use the same floating-point RGB input format and floating-point Gray output format.
    cmsUInt32Number fmtRGB_FLT  = TYPE_RGB_FLT;
    cmsUInt32Number fmtGRAY_FLT = TYPE_GRAY_FLT;
    cmsInt32Number intent = INTENT_PERCEPTUAL;

    cmsHTRANSFORM xform1 = cmsCreateTransform(hIn,  fmtRGB_FLT,
                                              hOut1, fmtGRAY_FLT,
                                              intent, 0);

    cmsHTRANSFORM xform2 = cmsCreateTransform(hIn,  fmtRGB_FLT,
                                              hOut2, fmtGRAY_FLT,
                                              intent, 0);

    if (xform1 == NULL || xform2 == NULL) {
        log_fail(testName, "Failed to create transforms for different-dest test");
        if (xform1) cmsDeleteTransform(xform1);
        if (xform2) cmsDeleteTransform(xform2);
        cmsCloseProfile(hIn);
        cmsCloseProfile(hOut1);
        cmsCloseProfile(hOut2);
        return;
    }

    cmsInt32Number nChan = 3;
    cmsInt32Number result = CompareFloatXFORM(xform1, xform2, nChan);

    // Cleanup
    cmsDeleteTransform(xform1);
    cmsDeleteTransform(xform2);
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut1);
    cmsCloseProfile(hOut2);

    // Expect failure (0)
    if (result == 0) {
        log_pass(testName);
    } else {
        log_fail(testName, "CompareFloatXFORM unexpectedly returned 1 for different-dest test");
        ++g_failed;
    }
}


// Entry point: run all tests and report a summary.
// Step 3 (Test Case Refinement) notes:
// - This harness uses non-terminating checks (it logs PASS/FAIL but continues).
// - The tests rely on the provided profile builders (Create_AboveRGB, Create_Gray22, Create_Gray30).
// - If the environment uses different pixel-type macros, adjust TYPE_RGB_FLT / TYPE_GRAY_FLT accordingly.
int main(void)
{
    // Initialize (optional, depending on environment)
    g_total = 0;
    g_failed = 0;

    // Run tests
    test_compare_float_xform_identity_transforms();
    test_compare_float_xform_different_destinations();

    // Summary
    std::cout << "\nTest summary: " << g_total << " tests, "
              << g_failed << " failures." << std::endl;

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}