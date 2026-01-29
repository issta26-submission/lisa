/*
  Test Suite for the focal method:
  CheckCurvesOnlyTransforms located in testcms2.c

  Step 1: Program Understanding (Candidate Keywords)
  - Gamma curves: cmsBuildGamma, c1/c2/c3 constructed with gamma values (2.2, 1/2.2, 4.84)
  - DeviceLink: cmsCreateLinearizationDeviceLinkTHR with cmsSigGrayData
  - Transform creation: cmsCreateTransform with various TYPE_GRAY_* formats and INTENT_PERCEPTUAL
  - Validation helpers: CheckFloatlinearXFORM, Check8linearXFORM, Check16linearXFORM, CompareFloatXFORM, Compare8bitXFORM, Compare16bitXFORM
  - Memory cleanup: cmsDeleteTransform, cmsCloseProfile, cmsFreeToneCurve
  - Branching: optimized vs non-optimized transforms (FLT/8/16 and non-optimizable paths)
  - Data types and signs: TYPE_GRAY_FLT, TYPE_GRAY_8, TYPE_GRAY_16; cmsSigGrayData
  - Overall flow: build curves, build profiles, create transforms, verify results, cleanup, return rc

  Step 2: Unit Test Generation (without GTest)
  - We expose the focal function via extern "C" and invoke it from C++ test harness.
  - Since testcms2.c exposes a fixed sequence, we craft tests that validate that the function can run in the normal environment and returns a non-zero rc (success signal used by the original code path).
  - We add multiple invocations to ensure repeated use does not crash and to exercise the code path multiple times if applicable.

  Step 3: Test Case Refinement
  - Use a lightweight, non-terminating assertion mechanism (EXPECT_*) to maximize execution of subsequent tests.
  - Include explanatory comments on each test case.
  - Ensure tests are portable to C++11 and rely only on standard library plus the provided LittleCMS dependencies.
  - Do not rely on private members or private visibility; call only the public function CheckCurvesOnlyTransforms().

  Note: This test suite assumes LittleCMS (lcms2) headers are available on the include path and that testcms2.c is linked into the test binary.
*/

#include <lcms2.h>
#include <iostream>
#include <string>
#include <testcms2.h>


// Domain knowledge: Use the actual LittleCMS type for correctness.

// Focal function from testcms2.c
extern "C" cmsInt32Number CheckCurvesOnlyTransforms(void);

// Lightweight test harness (non-terminating assertions)
static int gTestCount = 0;
static int gFailureCount = 0;

#define EXPECT_TRUE(cond, desc) do {                               \
    ++gTestCount;                                                    \
    if(!(cond)) {                                                    \
        ++gFailureCount;                                             \
        std::cerr << "[FAIL] " << (desc)                          \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } else {                                                         \
        std::cout << "[PASS] " << (desc) << "\n";                   \
    }                                                                \
} while(0)

#define EXPECT_NOTZERO(val, desc) do {                              \
    ++gTestCount;                                                   \
    if((val) == 0) {                                                \
        ++gFailureCount;                                            \
        std::cerr << "[FAIL] " << (desc)                              \
                  << " (expected non-zero, got 0) at "                 \
                  << __FILE__ << ":" << __LINE__ << "\n";            \
    } else {                                                        \
        std::cout << "[PASS] " << (desc) << "\n";                   \
    }                                                               \
} while(0)

/*
  Candidate Keywords (from Step 1) mapping to test design:
  - Gamma curves: gamma builds with 2.2, 1/2.2, 4.84
  - Gray device link: cmsCreateLinearizationDeviceLinkTHR(DbgThread(), cmsSigGrayData, &c)
  - Transform creation: cmsCreateTransform with TYPE_GRAY_FLT/TYPE_GRAY_8/TYPE_GRAY_16
  - Validation helpers: CheckFloatlinearXFORM, Check8linearXFORM, Check16linearXFORM
  - Non-optimized vs non-optimizable paths
  - Cleanup: cmsDeleteTransform, cmsCloseProfile, cmsFreeToneCurve
  - Return code handling: rc accumulation and final return
  - Data: gray data profiles
  - Intent: INTENT_PERCEPTUAL
  - Environment: DbgThread() and gamma tone curves
*/

static void Run_CheckCurvesOnlyTransforms_BasicRun()
{
    // Test Case 1: Basic run - ensure the function executes and returns a non-zero rc
    // This validates the normal execution path (gray data, gamma curves, transforms).
    cmsInt32Number rc = CheckCurvesOnlyTransforms();
    EXPECT_NOTZERO(rc, "CheckCurvesOnlyTransforms should return non-zero on valid environment");
}

static void Run_CheckCurvesOnlyTransforms_MultipleRuns()
{
    // Test Case 2: Multiple successive invocations to check stability and cleanup
    // Repeat a few times to exercise potential repeated allocations and deallocations.
    const int repeats = 3;
    for (int i = 0; i < repeats; ++i) {
        cmsInt32Number rc = CheckCurvesOnlyTransforms();
        EXPECT_NOTZERO(rc, "CheckCurvesOnlyTransforms (repeat) should return non-zero");
    }
}

static void Run_CheckCurvesOnlyTransforms_All()
{
    // Aggregate test runner to ensure all scenarios are executed
    std::cout << "Starting test suite for CheckCurvesOnlyTransforms...\n";
    Run_CheckCurvesOnlyTransforms_BasicRun();
    Run_CheckCurvesOnlyTransforms_MultipleRuns();
    std::cout << "Test suite finished. Total tests: " << gTestCount
              << ", Failures: " << gFailureCount << "\n";
}

int main(void)
{
    // Namespace: use the global test harness (no gtest/gmock)
    Run_CheckCurvesOnlyTransforms_All();

    // Return non-zero if any test failed
    if (gFailureCount != 0) {
        std::cerr << "Some tests failed. Failures: " << gFailureCount << "\n";
        return 1;
    }
    return 0;
}