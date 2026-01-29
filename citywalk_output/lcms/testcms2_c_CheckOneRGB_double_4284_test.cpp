/*
  Unit Test Suite for cmsInt32Number CheckOneRGB_double(cmsHTRANSFORM, cmsFloat64Number, cmsFloat64Number, cmsFloat64Number, cmsFloat64Number, cmsFloat64Number, cmsFloat64Number)

  Purpose:
  - Validate behavior of the focal method CheckOneRGB_double located in testcms2.c.
  - Ensure proper operation for identity transforms and verify true/false branches of the predicate.
  - Use a real Little CMS transform (TYPE_RGB_DBL) to exercise the path, covering:
      - All channels match within tolerance -> true
      - R channel mismatch -> false (short-circuits, G/B not evaluated)
      - G channel mismatch with R matching -> false
      - B channel mismatch with R/G matching -> false

  Approach:
  - Build a simple identity transform (RGB to RGB) using the sRGB profile as both input and output.
  - Feed various inputs and expected outputs to CheckOneRGB_double.
  - Implement a small test harness in C++11 (no GTest) and invoke the focal method from main.
  - Use informative printouts to reflect PASS/FAIL per test.

  Notes:
  - This test relies on Little CMS (lcms2) being available in the build environment.
  - We rely on the TYPE_RGB_DBL transform type to exercise double-precision color values (cmsFloat64Number).
  - The tests are designed to maximize coverage through independent scenarios, not to terminate on first failure.
  - All tests are self-contained within this translation unit.
*/

#include <iostream>
#include <lcms2.h>
#include <testcms2.h>
#include <iomanip>


// Import C API from Little CMS
extern "C" {
}

// Forward declaration of the focal function under test.
// The function is defined in testcms2.c as:
// cmsInt32Number CheckOneRGB_double(cmsHTRANSFORM xform, cmsFloat64Number R, cmsFloat64Number G, cmsFloat64Number B, cmsFloat64Number Ro, cmsFloat64Number Go, cmsFloat64Number Bo);
extern "C" cmsInt32Number CheckOneRGB_double(cmsHTRANSFORM xform,
                                          cmsFloat64Number R, cmsFloat64Number G, cmsFloat64Number B,
                                          cmsFloat64Number Ro, cmsFloat64Number Go, cmsFloat64Number Bo);

// Helper: create a simple identity transform for RGB to RGB using sRGB profile
static cmsHTRANSFORM CreateIdentityXForm() {
    // Create input and output profiles (sRGB)
    cmsHPROFILE hIn = cmsCreate_sRGBProfile();
    cmsHPROFILE hOut = cmsCreate_sRGBProfile();

    // Create a double-precision RGB to RGB transform
    cmsHTRANSFORM xform = cmsCreateTransform(hIn, TYPE_RGB_DBL,
                                             hOut, TYPE_RGB_DBL,
                                             INTENT_PERCEPTUAL, 0);

    // Release profile handles; transform retains its own copy/reference as needed
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);
    return xform;
}

// Utility: wrap a single test result for printing
static void PrintTestResult(const std::string& testName, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Test 1: Identity transform; all channels match within tolerance
static bool test_CheckOneRGB_double_identity_pass() {
    cmsHTRANSFORM xform = CreateIdentityXForm();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform." << std::endl;
        return false;
    }

    // Input RGB and expected output (identity)
    cmsFloat64Number R = 0.10;
    cmsFloat64Number G = 0.25;
    cmsFloat64Number B = 0.75;
    cmsFloat64Number Ro = 0.10;
    cmsFloat64Number Go = 0.25;
    cmsFloat64Number Bo = 0.75;

    cmsInt32Number result = CheckOneRGB_double(xform, R, G, B, Ro, Go, Bo);

    cmsDeleteTransform(xform);

    return result != 0;
}

// Test 2: R channel mismatch beyond tolerance; should short-circuit to false
static bool test_CheckOneRGB_double_R_mismatch_fails() {
    cmsHTRANSFORM xform = CreateIdentityXForm();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform." << std::endl;
        return false;
    }

    cmsFloat64Number R = 0.11;  // Out[0] will be 0.10; mismatch > 0.01
    cmsFloat64Number G = 0.25;
    cmsFloat64Number B = 0.75;

    cmsFloat64Number Ro = 0.10;
    cmsFloat64Number Go = 0.25;
    cmsFloat64Number Bo = 0.75;

    cmsInt32Number result = CheckOneRGB_double(xform, R, G, B, Ro, Go, Bo);

    cmsDeleteTransform(xform);

    return result == 0;
}

// Test 3: G channel mismatch within tolerance? No, to exercise non-short-circuit path we use mismatch just beyond tolerance on G
// R matches; G mismatch (beyond 0.01); B matches
static bool test_CheckOneRGB_double_G_mismatch_fails() {
    cmsHTRANSFORM xform = CreateIdentityXForm();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform." << std::endl;
        return false;
    }

    cmsFloat64Number R = 0.12;
    cmsFloat64Number G = 0.25;
    cmsFloat64Number B = 0.68;

    cmsFloat64Number Ro = 0.12;
    cmsFloat64Number Go = 0.27;  // mismatch by 0.02 > 0.01
    cmsFloat64Number Bo = 0.68;

    cmsInt32Number result = CheckOneRGB_double(xform, R, G, B, Ro, Go, Bo);

    cmsDeleteTransform(xform);

    return result == 0;
}

// Test 4: B channel mismatch beyond tolerance; R and G pass
static bool test_CheckOneRGB_double_B_mismatch_fails() {
    cmsHTRANSFORM xform = CreateIdentityXForm();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform." << std::endl;
        return false;
    }

    cmsFloat64Number R = 0.15;
    cmsFloat64Number G = 0.30;
    cmsFloat64Number B = 0.60;

    cmsFloat64Number Ro = 0.15;
    cmsFloat64Number Go = 0.30;
    cmsFloat64Number Bo = 0.65; // mismatch by 0.05 > 0.01

    cmsInt32Number result = CheckOneRGB_double(xform, R, G, B, Ro, Go, Bo);

    cmsDeleteTransform(xform);

    return result == 0;
}

int main() {
    // Step 3/4: Run test suite for CheckOneRGB_double
    bool all_pass = true;

    // Test 1: Identity passes
    bool t1 = test_CheckOneRGB_double_identity_pass();
    PrintTestResult("CheckOneRGB_double_identity_pass", t1);
    all_pass = all_pass && t1;

    // Test 2: R mismatch fails (short-circuit should prevent G/B checks)
    bool t2 = test_CheckOneRGB_double_R_mismatch_fails();
    PrintTestResult("CheckOneRGB_double_R_mismatch_fails", t2);
    all_pass = all_pass && t2;

    // Test 3: G mismatch fails (R must pass first)
    bool t3 = test_CheckOneRGB_double_G_mismatch_fails();
    PrintTestResult("CheckOneRGB_double_G_mismatch_fails", t3);
    all_pass = all_pass && t3;

    // Test 4: B mismatch fails (R and G pass)
    bool t4 = test_CheckOneRGB_double_B_mismatch_fails();
    PrintTestResult("CheckOneRGB_double_B_mismatch_fails", t4);
    all_pass = all_pass && t4;

    // Final summary
    if (all_pass) {
        std::cout << "\nAll tests PASSED for CheckOneRGB_double." << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests FAILED for CheckOneRGB_double." << std::endl;
        return 1;
    }
}