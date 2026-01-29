/*
This test suite targets the focal method ChecksRGB2LabFLT as requested.

Step 1 - Program understanding (summary in comments)
- Core dependencies used by ChecksRGB2LabFLT (as seen in the focal method) include:
  - cmsCreate_sRGBProfile, cmsCreateLab4Profile
  - cmsCreateTransform (two transforms: sRGB->LabA_FLT and LabA_FLT->sRGB)
  - cmsDoTransform (twice per iteration)
  - cmsDeleteTransform, cmsCloseProfile
  - TYPE_RGBA_FLT, TYPE_LabA_FLT, FLOAT_PRECISSION, IsGoodVal
- The function performs a loop for i = 0..100, builds RGBA1 from i/100, runs a forward transform to LabA, then an inverse transform back to RGBA, and checks that the RGB channels remain within FLOAT_PRECISSION. If any channel mismatches, it returns 0; otherwise returns 1 after cleanup.
- Candidate Keywords (core components to cover in tests): hSRGB, hLab, xform1, xform2, RGBA1, RGBA2, LabA, cmsDoTransform, IsGoodVal, FLOAT_PRECISSION, cmsCreateTransform, cmsDeleteTransform, cmsCreate_sRGBProfile, cmsCreateLab4Profile, cmsFLAGS_NOCACHE, cmsFLAGS_NOOPTIMIZE, TYPE_RGBA_FLT, TYPE_LabA_FLT.

Step 2 - Unit test generation (test targets)
- We will create a minimal, non-GTest C++11 test harness that calls the exported function ChecksRGB2LabFLT from the C test file (testcms2.c) and verifies its behavior.
- Tests:
  1) Basic correctness: ChecksRGB2LabFLT should return 1 under normal operation.
  2) Repeated invocation stability: Calling ChecksRGB2LabFLT twice should still return 1 both times (ensures resources are cleaned up properly between calls).

Step 3 - Test case refinement
- We implement a lightweight assertion mechanism (non-terminating) to collect failures and continue executing tests, so we maximize code-path coverage.
- We expose a thin wrapper via extern "C" to call the focal function.
- We rely on the provided testcms2.h and its IsGoodVal, etc., by linking against the original C test file in the build, as required by the project.

Note: No GTest is used per requirements. The tests are run from main() as a small harness.

Code (unit tests for ChecksRGB2LabFLT)

*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Forward declaration of the focal function from testcms2.c.
// The Little CMS test suite defines cmsInt32Number for return types; here we assume int is compatible.
extern "C" int ChecksRGB2LabFLT(void);

// Simple non-terminating test assertion framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do {                                      \
    ++g_total_tests;                                                       \
    if (!(cond)) {                                                         \
        ++g_failed_tests;                                                  \
        std::cerr << "TEST FAILED: " << (msg)                           \
                  << " (Condition: " #cond ", line " << __LINE__ << ")"  \
                  << std::endl;                                          \
    } else {                                                             \
        std::cout << "TEST PASSED: " << (msg) << std::endl;              \
    }                                                                    \
} while (0)

// Test 1: Basic run - the focal function should return 1 on a normal run
static void test_ChecksRGB2LabFLT_basic() {
    // Execute focal method
    int result = ChecksRGB2LabFLT();
    // Expect a successful round-trip (return 1)
    EXPECT_TRUE(result == 1, "ChecksRGB2LabFLT should return 1 on standard run");
}

// Test 2: Repeatability - invoking the focal method twice should also succeed
static void test_ChecksRGB2LabFLT_repeat() {
    int r1 = ChecksRGB2LabFLT();
    int r2 = ChecksRGB2LabFLT();
    EXPECT_TRUE((r1 == 1) && (r2 == 1),
                "ChecksRGB2LabFLT should return 1 on two consecutive runs");
}

// Main test runner
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Unused parameters

    std::cout << "Starting unit tests for ChecksRGB2LabFLT..." << std::endl;

    test_ChecksRGB2LabFLT_basic();
    test_ChecksRGB2LabFLT_repeat();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed to aid CI systems
    return (g_failed_tests > 0) ? 1 : 0;
}