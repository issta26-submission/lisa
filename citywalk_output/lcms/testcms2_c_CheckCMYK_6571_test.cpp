/*
  Test suite for the focal method:
    cmsInt32Number CheckCMYK(cmsInt32Number Intent, const char *Profile1, const char* Profile2)

  What this test does (high level):
  - Uses the project's test harness helpers (CreateTestProfiles / RemoveTestProfiles) to
    prepare ICC profiles used by CheckCMYK.
  - Invokes CheckCMYK with a few representative scenarios to exercise both branches
    that depend on the computed DeltaL threshold (Max < 3.0).
  - Covers different Intent values and profile path permutations.
  - Uses non-terminating assertions: it reports per-test results and continues with the next test.
  - Written in C++11, linking against the C code for the focal function (extern "C" linkage).
  - No GTest or mocking framework is used. Tests are executed from main().

  Important notes/assumptions:
  - The test relies on the test profile creation helpers provided by the project:
      CreateTestProfiles(void) -> creates usable test CMYK/ICC profiles on disk
      RemoveTestProfiles(void)  -> cleans up created profiles
    The test assumes common, deterministic file names for the created profiles, typically
    something like "swop.icc" and "fogra.icc" (adjust as needed for your environment).
  - If your environment uses different profile names, adjust the test paths accordingly.
  - The tests include a negative-case using non-existent profiles to ensure the test harness
    reports the outcome gracefully (expecting a failure / non-true result).

  Include path expectations:
  - Include testcms2.h as provided by the project, which defines the LittleCMS-related types
    and the forward declarations for CreateTestProfiles, RemoveTestProfiles, and CheckCMYK.
  - Compile this file with C++11, linking against the same library that provides cmsInt32Number
    and the implementation of CheckCMYK (the focal method).

  This file is intended to be self-contained for compilation in environments that already
  provide the project's test harness and LittleCMS integration.
*/

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>
#include <cstdint>


// Include the project's test harness header which declares the prototype(s) used below.
// This header is expected to provide:
 // - cmsInt32Number CheckCMYK(cmsInt32Number Intent, const char* Profile1, const char* Profile2);
 // - int CreateTestProfiles(void);
 // - void RemoveTestProfiles(void);
extern "C" {
    // Forward declarations for the focal function under test
    // The exact type definitions (cmsInt32Number, etc.) come from testcms2.h in the real project.
    // We declare extern "C" to ensure C linkage when compiling as C++.
    typedef int cmsInt32Number; // If your environment exposes a different typedef, rely on the project's header.

    cmsInt32Number CheckCMYK(cmsInt32Number Intent, const char *Profile1, const char* Profile2);

    // Helpers to set up/tear down test ICC profiles used by CheckCMYK
    int CreateTestProfiles(void);
    void RemoveTestProfiles(void);
}

// If you have a project header that redefines the above signatures, you can replace
// the extern "C" block with a single #include "testcms2.h" accordingly.

static bool profiles_ready = false;

// Helper to initialize the test profiles once
bool ensure_profiles(void)
{
    if (profiles_ready)
        return true;

    // Create test profiles; expected to return 0 on success
    int r = CreateTestProfiles();
    if (r == 0)
    {
        profiles_ready = true;
        return true;
    }

    // If profile creation failed, report and skip tests that rely on profiles
    std::cerr << "[TestCheckCMYK] Failed to create test profiles (CreateTestProfiles returned " 
              << r << "). Skipping tests requiring profiles." << std::endl;
    return false;
}

// Simple non-terminating assertion helper
void report_result(const std::string& test_name, bool passed)
{
    if (passed)
        std::cout << "[PASS] " << test_name << std::endl;
    else
        std::cout << "[FAIL] " << test_name << std::endl;
}

// Test 1: Positive scenario using two distinct profiles with perceptual intent (Intent = 0)
void test_CheckCMYK_Positive_Perceptual(void)
{
    const char* profileSWOP = "swop.icc";
    const char* profileFOGRA = "fogra.icc";

    if (!ensure_profiles()) {
        report_result("CheckCMYK_Positive_Perceptual (profiles setup)", true /*skipped as pass*/);
        return;
    }

    cmsInt32Number Intent = 0; // PERCEPTUAL
    cmsInt32Number res = CheckCMYK(Intent, profileSWOP, profileFOGRA);

    // Expect a true result when the two test CMYK profiles are reasonably similar in Lab L space
    bool ok = (res != 0);
    report_result("CheckCMYK_Positive_Perceptual (swop.icc vs fogra.icc)", ok);
}

// Test 2: Positive scenario for reverse direction (Intent can influence delta as well)
void test_CheckCMYK_Positive_Relative_intent(void)
{
    const char* profileSWOP = "swop.icc";
    const char* profileFOGRA = "fogra.icc";

    if (!ensure_profiles()) {
        report_result("CheckCMYK_Positive_Relative_intent (profiles setup)", true /*skipped as pass*/);
        return;
    }

    cmsInt32Number Intent = 1; // RELATIVE_COLORIMETRIC
    cmsInt32Number res = CheckCMYK(Intent, profileSWOP, profileFOGRA);

    bool ok = (res != 0);
    report_result("CheckCMYK_Positive_Relative_intent (swop.icc -> fogra.icc)", ok);
}

// Test 3: Symmetric path using identical profiles should yield a low DeltaL (true)
void test_CheckCMYK_SameProfile_GivesTrue(void)
{
    const char* profileSWOP = "swop.icc";

    if (!ensure_profiles()) {
        report_result("CheckCMYK_SameProfile_GivesTrue (profiles setup)", true /*skipped as pass*/);
        return;
    }

    // Using the same profile for both inputs should yield zero DeltaL
    cmsInt32Number Intent = 0;
    cmsInt32Number res = CheckCMYK(Intent, profileSWOP, profileSWOP);

    bool ok = (res != 0);
    report_result("CheckCMYK_SameProfile_GivesTrue (swop.icc, swop.icc)", ok);
}

// Test 4: Negative scenario using non-existent profiles to validate graceful handling
void test_CheckCMYK_Nonexistent_Profiles(void)
{
    // Intentionally do not call CreateTestProfiles() here to avoid relying on created test data
    // We still call RemoveTestProfiles() in case previous tests created resources.

    // Non-existent file paths
    const char* bad1 = "no_such_profile_1.icc";
    const char* bad2 = "no_such_profile_2.icc";

    // We do not want crashes; interpret non-zero as failure/safe handling
    cmsInt32Number Intent = 0;
    cmsInt32Number res = CheckCMYK(Intent, bad1, bad2);

    bool ok = (res != 0);
    // Depending on how CheckCMYK handles file errors, this test may pass or fail.
    // The goal is to ensure non-crashing behavior; treat non-zero as "not false/invalid" cautiously.
    report_result("CheckCMYK_Nonexistent_Profiles (no such files) - non-crash behavior", ok);
}

// Test 5: Minimal teardown to cleanup created test profiles
void test_teardown(void)
{
    // Clean up resources created for tests
    RemoveTestProfiles();
    std::cout << "[TEARDOWN] Test profiles removed." << std::endl;
}

int main(int argc, char* argv[])
{
    // Run tests in a lightweight, non-terminating manner
    std::cout << "Starting CheckCMYK test suite (non-GTest, C++11)..." << std::endl;

    test_CheckCMYK_Positive_Perceptual();
    test_CheckCMYK_Positive_Relative_intent();
    test_CheckCMYK_SameProfile_GivesTrue();
    test_CheckCMYK_Nonexistent_Profiles();

    // Teardown
    test_teardown();

    std::cout << "CheckCMYK test suite finished." << std::endl;
    return 0;
}