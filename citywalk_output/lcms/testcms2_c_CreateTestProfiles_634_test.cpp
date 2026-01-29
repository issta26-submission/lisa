/*
Unit Test Suite for CreateTestProfiles (testcms2.c)
Target: Validate that cmsInt32Number CreateTestProfiles(void) executes the expected true-path
and returns success in the typical scenario. This test is written in C++11 (no GTest) and
assumes the test CMS environment (testcms2.c and LittleCMS helpers) is available to link.

Notes on what is being tested:
- The focal method CreateTestProfiles() sequentially creates a series of ICC profiles using the
  cmsCreate_*ProfileTHR and related helpers, and for each created profile calls OneVirtual to
  validate them against expected reference files. The function returns 1 on success (all
  profiles validated) and 0 if any step fails (e.g., OneVirtual returns false, or a profile
  creation returns NULL).
- This test suite focuses on:
  - Verifying the successful path (return value 1) when everything works as in normal operation.
  - Verifying stability across multiple consecutive invocations (no crash or unexpected behavior).
- Due to the integration with the test helper OneVirtual and various ICC profile resources, this
  test relies on the actual testCMS environment and resource files present in the project.

Candidate Keywords (Step 1 extraction) illustrating dependencies in the focal method:
- cmsCreate_sRGBProfileTHR, DbgThread
- OneVirtual
- Create_AboveRGB, Create_Gray22, Create_Gray30, Create_GrayLab
- Create_CMYK_DeviceLink
- cmsCreateInkLimitingDeviceLinkTHR, cmsSigCmykData
- cmsCreateLab2ProfileTHR, cmsCreateLab4ProfileTHR
- cmsCreateXYZProfileTHR, cmsCreateNULLProfileTHR
- cmsCreateBCHSWabstractProfileTHR
- CreateFakeCMYK
- 150 (ink limit), 300 (brightness/test parameter)
- Associated file names and references: sRGBlcms2.icc, aRGBlcms2.icc, graylcms2.icc, gray3lcms2.icc, glablcms2.icc, linlcms2.icc, limitlcms2.icc, labv2lcms2.icc, labv4lcms2.icc, xyzlcms2.icc, nullcms2.icc, bchslcms2.icc, lcms2cmyk.icc, brightness.icc
- The function’s early returns on failure of OneVirtual or NULL profile creation.

Implementation details:
- The test suite is implemented in C++11, using a lightweight custom test harness (no GTest).
- We declare the focal function with extern "C" to ensure correct linkage.
- We provide two tests:
  1) CreateTestProfiles_Returns1: ensures the normal path returns 1.
  2) CreateTestProfiles_MultipleCalls_DoNotCrash: ensures repeated invocations do not crash and each returns 1.
- Each test includes explanatory comments as requested.
- The tests are designed to be compiled together with testcms2.c (and any required LittleCMS runtime) in a single executable.

Code:
*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Linker will supply the C function; declare with C linkage
extern "C" int CreateTestProfiles(void);

// Helper macro for simple test result reporting
#define TEST_CASE(name) void name()

static void report_pass(const std::string& test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}
static void report_fail(const std::string& test_name, const std::string& reason) {
    std::cerr << "[FAIL] " << test_name << " - " << reason << std::endl;
}

// Test 1: Ensure normal path returns 1
// This validates the true-path behavior when all profile creations and validations succeed.
TEST_CASE(test_CreateTestProfiles_Returns1)
{
    const char* test_name = "test_CreateTestProfiles_Returns1";

    int result = CreateTestProfiles();
    if (result == 1) {
        report_pass(test_name);
    } else {
        report_fail(test_name, "CreateTestProfiles() did not return 1 on expected success path.");
    }
}

// Test 2: Ensure multiple consecutive invocations do not crash and return 1 each time
// This helps verify stability and lack of stateful side-effects between calls.
TEST_CASE(test_CreateTestProfiles_MultipleCalls_DoNotCrash)
{
    const char* test_name = "test_CreateTestProfiles_MultipleCalls_DoNotCrash";

    const int iterations = 3;
    bool all_pass = true;
    for (int i = 0; i < iterations; ++i) {
        int result = CreateTestProfiles();
        if (result != 1) {
            all_pass = false;
            report_fail(test_name, "Failure on iteration " + std::to_string(i) +
                                      ": expected 1, got " + std::to_string(result));
            break;
        }
    }
    if (all_pass) {
        report_pass(test_name);
    }
}

// Simple test runner (no external test framework)
int main(void)
{
    // Run tests
    test_CreateTestProfiles_Returns1();
    test_CreateTestProfiles_MultipleCalls_DoNotCrash();

    // Exit code: 0 indicates test runner completed.
    // We rely on per-test reporting for pass/fail feedback.
    return 0;
}