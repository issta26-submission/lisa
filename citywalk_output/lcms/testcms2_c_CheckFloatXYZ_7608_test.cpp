/*
Step 1 - Program Understanding (Candidate Keywords)
- Core function under test: cmsInt32Number CheckFloatXYZ(void)
- Key dependencies/operations used within the focal method:
  - cmsHPROFILE, cmsProfile handling: cmsCreateXYZProfile(), IdentityMatrixProfile(...)
  - Transform creation/usage: cmsCreateTransform(), cmsDoTransform(), cmsDeleteTransform(), cmsCloseProfile()
  - Data buffers: cmsFloat32Number in[4], out[4]
  - RGB/XYZ data signatures and types: cmsSigRgbData, cmsSigXYZData, TYPE_RGB_FLT, TYPE_XYZ_FLT, TYPE_XYZA_FLT
  - Color management intents/flags: INTENT_RELATIVE_COLORIMETRIC, cmsFLAGS_COPY_ALPHA
  - Validation: IsGoodVal(...)
  - Tolerance: FLOAT_PRECISSION
  - Operation sequence: RGB -> XYZ, XYZ -> XYZ, XYZA -> XYZA, XYZ -> RGB, RGB -> RGB
- Representative component keywords (for test planning): CheckFloatXYZ, IdentityMatrixProfile, IsGoodVal, FLOAT_PRECISSION, cmsCreateTransform, cmsDoTransform, cmsDeleteTransform, cmsCloseProfile, cmsCreateXYZProfile, cmsSigRgbData, cmsSigXYZData, TYPE_RGB_FLT, TYPE_XYZ_FLT, TYPE_XYZA_FLT, INTENT_RELATIVE_COLORIMETRIC, cmsFLAGS_COPY_ALPHA

Notes:
- The focal function is self-contained and deterministic for a given environment. It performs a sequence of color-space transforms and validates each step. If any step’s IsGoodVal test fails, it returns 0; otherwise it completes and returns 1.
- The test suite below will exercise CheckFloatXYZ and verify basic behavior (correct return value and determinism). Because the internal IsGoodVal checks are deterministic with the provided data, achieving true/false branch coverage for each internal IsGoodVal may require altering internal static behavior or macro values, which is outside the scope of a black-box test. The tests focus on observable, stable behavior and ensure the function can be invoked end-to-end.

Step 2 & 3 - Unit Test Suite (C++11, no GoogleTest; non-terminating assertions)
This test suite calls the focal function CheckFloatXYZ from the existing C code (testcms2.c) and validates its behavior using a lightweight, non-terminating assertion mechanism.

Code (test_checkfloatxyz.cpp):
*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Include the project's test harness header to ensure correct type names and prototypes.
// The header should declare cmsInt32Number and related types used by the focal function.

extern "C" cmsInt32Number CheckFloatXYZ(void); // Forward declaration for C linkage

// Lightweight non-terminating assertion framework
static int g_tests_total = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_total; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[TEST FAILED] " << (msg) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_total; \
    if(!((a) == (b))) { \
        ++g_tests_failed; \
        std::cerr << "[TEST FAILED] " << (msg) \
                  << " - Expected: " << (a) << "  Got: " << (b) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Test 1: Basic sanity - CheckFloatXYZ should return a non-zero value on a correct environment.
static void Test_CheckFloatXYZ_ReturnsNonZero(void) {
    cmsInt32Number res = CheckFloatXYZ();
    EXPECT_TRUE(res != 0, "CheckFloatXYZ() should return non-zero (true) on valid environment");
}

// Test 2: Determinism - Repeated invocations should yield the same result.
static void Test_CheckFloatXYZ_IsDeterministic(void) {
    cmsInt32Number r1 = CheckFloatXYZ();
    cmsInt32Number r2 = CheckFloatXYZ();
    EXPECT_EQ(r1, r2, "CheckFloatXYZ() should be deterministic across successive invocations");
}

// Optional: Test 3 (exploratory) - Verify that the function can be invoked after a clean start
// This helps ensure no residual static state from prior tests in a single process.
// Note: The internal static state in the focal file is not accessible; this test simply ensures
// that two consecutive runs do not crash and return a valid value.
static void Test_CheckFloatXYZ_ConsecutiveRuns(void) {
    cmsInt32Number a = CheckFloatXYZ();
    cmsInt32Number b = CheckFloatXYZ();
    EXPECT_TRUE((a != 0) && (b != 0), "Consecutive CheckFloatXYZ() calls should both return non-zero values");
}

int main(int argc, char* argv[]) {
    // Run tests
    std::cout << "Starting unit tests for CheckFloatXYZ (from testcms2.c) in C++11 test harness." << std::endl;

    Test_CheckFloatXYZ_ReturnsNonZero();
    Test_CheckFloatXYZ_IsDeterministic();
    Test_CheckFloatXYZ_ConsecutiveRuns();

    // Summary
    if (g_tests_failed == 0) {
        std::cout << "[TEST SUITE] All tests passed for CheckFloatXYZ." << std::endl;
    } else {
        std::cerr << "[TEST SUITE] " << g_tests_failed << " test(s) failed out of "
                  << g_tests_total << "." << std::endl;
    }

    // Return non-zero if any test failed
    return g_tests_failed;
}