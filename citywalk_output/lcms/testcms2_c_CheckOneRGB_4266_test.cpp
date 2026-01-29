/*
   Test Suite: CheckOneRGB (testcms2.c)
   Purpose: Validate the focal method cmsInt32Number CheckOneRGB(...)
            from the provided testcms2.c (focal method under test).
   Approach:
   - Create a simple identity 16-bit RGB transform (R,G,B unchanged) using
     the harness-provided Create_AboveRGB() as input/output profiles.
   - Exercise true and false branches of the predicate using different Ro/Go/Bo
     values to ensure complete coverage of the IsGoodWord comparisons.
   - Use a lightweight, non-terminating test harness (no GTest) with simple
     pass/fail reporting and per-test comments.
   - Adhere to the constraints: only standard library + provided test utilities,
     no private member access, and tests can be invoked from main().
*/

/* 
   Note on Candidate Keywords (Step 1):
   - xform: cmsHTRANSFORM handle passed into CheckOneRGB
   - cmsDoTransform: internal operation invoked inside CheckOneRGB
   - RGB / Out: input and output 3-element buffers
   - R, G, B: input channel values
   - Ro, Go, Bo: expected output values used for comparison
   - IsGoodWord: predicate used by the focal method to compare values
   These keywords guide test coverage (identity transform, matching vs. mismatching values).
*/

#include <stdio.h>
#include <stdlib.h>
#include <lcms2.h>
#include <testcms2.h>


/* 
   Expose the focal function under test. In the real project this is defined
   in testcms2.c, but to compile this test as a separate translation unit we
   declare it here as extern to link against the actual implementation.
*/
extern cmsInt32Number CheckOneRGB(cmsHTRANSFORM xform,
                                 cmsUInt16Number R, cmsUInt16Number G, cmsUInt16Number B,
                                 cmsUInt16Number Ro, cmsUInt16Number Go, cmsUInt16Number Bo);

/* 
   Helper: Run a single test scenario using an identity 16-bit RGB transform.
   Returns 1 on success (CheckOneRGB returned non-zero) and 0 on failure.
*/
static int run_CheckOneRGB_case(cmsUInt16Number R, cmsUInt16Number G, cmsUInt16Number B,
                              cmsUInt16Number Ro, cmsUInt16Number Go, cmsUInt16Number Bo)
{
    // Create an identity-like transform: input RGB_16 -> output RGB_16 using same profile
    cmsHPROFILE hProfile = Create_AboveRGB(); // Provided by harness
    if (hProfile == NULL) {
        printf("ERROR: Failed to create test RGB profile (Create_AboveRGB).\n");
        return 0;
    }

    // Create a simple 16-bit RGB transform from RGB to RGB (identity-like)
    cmsHTRANSFORM xform = cmsCreateTransform(hProfile, TYPE_RGB_16, hProfile, TYPE_RGB_16, INTENT_PERCEPTUAL);
    if (xform == NULL) {
        printf("ERROR: Failed to create identity RGB transform (cmsCreateTransform).\n");
        cmsCloseProfile(hProfile);
        return 0;
    }

    // Invoke the focal method under test
    cmsInt32Number res = CheckOneRGB(xform, R, G, B, Ro, Go, Bo);

    // Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(hProfile);

    // Return boolean-like result
    return (res != 0) ? 1 : 0;
}

/* Individual test cases with explanatory comments for coverage refinement */

static int test_CheckOneRGB_identity_zero_zero_zero(void)
{
    // Identity: 0,0,0 should map to 0,0,0
    int ok = run_CheckOneRGB_case(0, 0, 0, 0, 0, 0);
    printf("Test: Identity RGB(0,0,0) -> (0,0,0) => %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

static int test_CheckOneRGB_identity_1000_2000_3000(void)
{
    // Identity: 1000,2000,3000 should map to 1000,2000,3000
    int ok = run_CheckOneRGB_case(1000, 2000, 3000, 1000, 2000, 3000);
    printf("Test: Identity RGB(1000,2000,3000) -> (1000,2000,3000) => %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

static int test_CheckOneRGB_mismatch_R(void)
{
    // Mismatch on Red channel should fail the predicate
    int ok = run_CheckOneRGB_case(1000, 2000, 3000, 1001, 2000, 3000);
    printf("Test: Mismatch on Red channel (Ro=1001) => %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

static int test_CheckOneRGB_mismatch_G(void)
{
    // Mismatch on Green channel should fail the predicate
    int ok = run_CheckOneRGB_case(1000, 2000, 3000, 1000, 2001, 3000);
    printf("Test: Mismatch on Green channel (Go=2001) => %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

static int test_CheckOneRGB_mismatch_B(void)
{
    // Mismatch on Blue channel should fail the predicate
    int ok = run_CheckOneRGB_case(1000, 2000, 3000, 1000, 2000, 3001);
    printf("Test: Mismatch on Blue channel (Bo=3001) => %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

static int test_CheckOneRGB_edge_65535_and_zero(void)
{
    // Edge case: extreme values to ensure handling across full 16-bit range
    int ok = run_CheckOneRGB_case(65535, 0, 0, 65535, 0, 0);
    printf("Test: Edge RGB(65535,0,0) -> (65535,0,0) => %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

/* Main entry: run all tests and report overall result */
int main(void)
{
    printf("Starting CheckOneRGB test suite (C, non-GTest, harness-provided):\n\n");

    int total = 0;
    int passed = 0;

    total += 1; if (test_CheckOneRGB_identity_zero_zero_zero()) passed++;

    total += 1; if (test_CheckOneRGB_identity_1000_2000_3000()) passed++;

    total += 1; if (test_CheckOneRGB_mismatch_R()) passed++;

    total += 1; if (test_CheckOneRGB_mismatch_G()) passed++;

    total += 1; if (test_CheckOneRGB_mismatch_B()) passed++;

    total += 1; if (test_CheckOneRGB_edge_65535_and_zero()) passed++;

    printf("\nSummary: %d/%d tests PASSED\n", passed, total);
    return (passed == total) ? 0 : 1;
}