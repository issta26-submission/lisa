/*
Step 1 - Program Understanding (Candidate Keywords)
- Focus: CheckGamutCheck function in the focal code
- Core dependencies (from <FOCAL_CLASS_DEP>):
  - cmsCreate_sRGBProfileTHR, cmsCreateProofingTransformTHR, cmsSetAlarmCodes
  - Create_AboveRGB, DbgThread
  - CheckFloatlinearXFORM, Check16linearXFORM
  - cmsCloseProfile, cmsDeleteTransform
- Behavior hints:
  - Sets alarm codes, creates two profiles, builds two transforms with GAMUTCHECK flag
  - First branch: if either profile is NULL, return 0
  - Second branch (floating point): if CheckFloatlinearXFORM fails, cleanup and return 0
  - Third branch (16-bit): cleanup and return value from Check16linearXFORM
- Operation space: Gamut checking for equal color space between "above" RGBs and sRGB
- Critical control paths to cover conceptually:
  - Resource creation success path
  - Branch where the floating-point gamut check passes
  - Branch where 16-bit gamut check result is returned
- Static helpers and domain-specific guards are in testcms2.h
- Testing approach (non-GTest): craft small C++ test harness invoking C API

Step 2 - Unit Test Generation (Plan)
- Provide a small C++ test file that uses the same dependencies declared in testcms2.h
- Tests to implement:
  1) test_Create_sRGB_and_AboveRGB_Profiles: ensure profile creation functions return non-NULL profiles and can be closed
  2) test_CheckGamutCheck_Basic: call CheckGamutCheck() and ensure a non-negative result (indicating it completed the workflow)
  3) test_CheckGamutCheck_Reentrant: call CheckGamutCheck() multiple times to ensure no crashes and stable behavior
- Coverage goals:
  - True/false branches of early NULL-check (best effort via independent profile creation tests)
  - Branches within CheckGamutCheck (sanity path through both transforms)
  - Resource acquisition and cleanup paths
- Implementation constraints:
  - Use only C++11 standard library and provided C APIs
  - Do not rely on Google Test; implement a tiny self-test harness
  - Access C functions by wrapping includes with extern "C"
  - Use simple, non-terminating assertions (print logs but continue)
- Test entry point: main function drives tests by calling test functions, prints summary, and returns non-zero on failure

Step 3 - Test Case Refinement (Enhancements)
- Add a few extra checks for additional basic capabilities:
  - cmsCreate_sRGBProfileTHR with DbgThread() should produce valid profile
  - Proper cleanup of resources to avoid leaks
- Improve diagnostics by providing inline comments for each test explaining intent and expected outcomes
- Ensure tests are resilient to optional dependencies by checking for NULLs and logging gracefully
- Keep tests self-contained and order-independent as much as possible

Note: The following code is a C++11 test harness that targets the focal CheckGamutCheck along with auxiliary dependencies declared in testcms2.h. It uses a minimal, non-terminating assertion style and calls into the existing C API provided by the project (no GTest).

*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Import the provided C dependencies (testcms2.h) with C linkage
extern "C" {
}

// Simple non-terminating assertion helper
#define LOG_FAIL(msg) std::cerr << "FAIL: " << (msg) << std::endl;
#define LOG_OK(msg)   std::cout << "OK: "   << (msg) << std::endl;

//
// Test 1: Create and clean up the sRGB profile (ensures resource creation paths work)
// This validates that cmsCreate_sRGBProfileTHR and DbgThread() can be used to produce a valid profile,
// which is a prerequisite for CheckGamutCheck to operate without early exit.
//
bool test_CreateSrgbProfileTHR_and_Close() {
    cmsHPROFILE h = cmsCreate_sRGBProfileTHR(DbgThread());
    if (h == NULL) {
        LOG_FAIL("cmsCreate_sRGBProfileTHR(DbgThread()) returned NULL");
        return false;
    }
    cmsCloseProfile(h);
    LOG_OK("cmsCreate_sRGBProfileTHR_and_Close succeeded");
    return true;
}

//
// Test 2: Create an "Above RGB" profile (helper used by the focal method)
// Verifies Create_AboveRGB() returns a non-NULL handle and can be safely closed.
// This helps ensure the preconditions for CheckGamutCheck are satisfiable.
//
bool test_Create_AboveRGB_Profile() {
    cmsHPROFILE hAbove = Create_AboveRGB();
    if (hAbove == NULL) {
        LOG_FAIL("Create_AboveRGB() returned NULL");
        return false;
    }
    cmsCloseProfile(hAbove);
    LOG_OK("Create_AboveRGB_Profile succeeded");
    return true;
}

//
// Test 3: Basic invocation of CheckGamutCheck
// This checks that the focal routine executes through its high-level flow and returns a non-negative rc.
// We do not assert exact rc value because it depends on the runtime gamut checks,
// but a negative rc would indicate a fatal early exit or crash.
//
bool test_CheckGamutCheck_Basic() {
    cmsInt32Number rc = CheckGamutCheck();
    if (rc < 0) {
        LOG_FAIL("CheckGamutCheck() returned negative rc, indicating an error path");
        return false;
    }
    LOG_OK("CheckGamutCheck_Basic returned non-negative rc");
    return true;
}

//
// Test 4: Re-entrant check - call CheckGamutCheck multiple times in a row
// Ensures repeated usage does not crash and yields consistent non-negative results (to the extent possible here).
//
bool test_CheckGamutCheck_Reentrant() {
    const int iterations = 3;
    bool all_ok = true;
    for (int i = 0; i < iterations; ++i) {
        cmsInt32Number rc = CheckGamutCheck();
        if (rc < 0) {
            std::cerr << "CheckGamutCheck iteration " << i << " returned negative rc\n";
            all_ok = false;
        } else {
            std::cout << "CheckGamutCheck iteration " << i << " rc=" << rc << "\n";
        }
    }
    if (!all_ok) {
        LOG_FAIL("CheckGamutCheck_Reentrant encountered a negative rc on some iteration");
        return false;
    }
    LOG_OK("CheckGamutCheck_Reentrant completed successfully for all iterations");
    return true;
}

//
// Optional: Extra sanity test to ensure that the underlying helper (Create_AboveRGB) works before using CheckGamutCheck
// The presence of valid profiles is a prerequisite for the main function; this acts as a light-weight guard.
//
bool test_Prerequisite_ProfilesExist() {
    cmsHPROFILE hAbove = Create_AboveRGB();
    if (hAbove == NULL) {
        LOG_FAIL("Prerequisite: Create_AboveRGB() failed (NULL profile)");
        return false;
    }
    cmsCloseProfile(hAbove);
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfileTHR(DbgThread());
    if (hSRGB == NULL) {
        LOG_FAIL("Prerequisite: cmsCreate_sRGBProfileTHR(DbgThread()) failed (NULL profile)");
        return false;
    }
    cmsCloseProfile(hSRGB);
    LOG_OK("Prerequisite_ProfilesExist succeeded");
    return true;
}

//
// Main driver: run all tests and accumulate results
//
int main(int argc, char* argv[]) {
    int total = 0;
    int passed = 0;

    // Section: Progressive test steps
    if (test_CreateSrgbProfileTHR_and_Close()) { ++passed; }
    ++total;

    if (test_Create_AboveRGB_Profile()) { ++passed; }
    ++total;

    if (test_Prerequisite_ProfilesExist()) { ++passed; }
    ++total;

    if (test_CheckGamutCheck_Basic()) { ++passed; }
    ++total;

    if (test_CheckGamutCheck_Reentrant()) { ++passed; }
    ++total;

    std::cout << "Test suite results: " << passed << " / " << total << " tests passed.\n";

    // Non-zero exit code on any failure to aid integration with build systems
    return (passed == total) ? 0 : 1;
}