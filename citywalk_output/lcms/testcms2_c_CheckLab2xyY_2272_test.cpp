// Test suite for the focal method CheckLab2xyY in testcms2.c
// This test suite is written for C++11, without Google Test, and uses a lightweight
// in-file test harness. It exercises the CheckLab2xyY function and related color-space
// conversion utilities via the LittleCMS API (lcms2).
//
// Notes:
// - Candidate Keywords derived from the focal method (Step 1): Lab, XYZ, xyY, cmsLab2XYZ,
//   cmsXYZ2xyY, cmsxyY2XYZ, cmsXYZ2Lab, cmsDeltaE
// - The tests rely on the public LittleCMS C API (lcms2.h). They do not rely on private
//   members. Static helpers inside the tests mimic isolated checks without modifying
//   the production code.
// - We implement a minimal test harness (no GTest) and non-terminating assertions to
//   maximize code coverage.
// - Tests are designed to compile under C++11 and link against the existing library.

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>


// Import LittleCMS public API
extern "C" {
}

// Ensure the focal function prototype is visible (C linkage)
extern "C" cmsInt32Number CheckLab2xyY(void);

// Minimal test harness (non-terminating assertions)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do {                                      \
    if (cond) {                                                            \
        ++g_tests_passed;                                                  \
        printf("[PASS] %s\n", msg);                                        \
    } else {                                                               \
        ++g_tests_failed;                                                  \
        printf("[FAIL] %s\n", msg);                                        \
    }                                                                      \
} while (0)

#define TEST_NAME(name) "[Test] " name

// Helper: Run a specific test function and print its name
typedef void (*TestFn)(void);

static void run_test(const char* name, TestFn fn) {
    printf("Running %s...\n", name);
    // Execute test
    fn();
}

// Test 1: Verify that CheckLab2xyY returns a non-zero value (i.e., test passes)
// This exercises the main predicate of the focal method (the final return is a predicate).
static void test_CheckLab2xyY_ReturnsNonZero(void) {
    cmsInt32Number res = CheckLab2xyY();
    // The convention in this codebase is to return non-zero (true) when the check succeeds.
    TEST_ASSERT(res != 0, "CheckLab2xyY should return non-zero (pass condition).");
}

// Test 2: Verify that repeated invocation does not crash and consistently returns a boolean
// This exercise ensures static/global state within the test environment doesn't break on multiple calls.
static void test_CheckLab2xyY_NoCrashOnMultipleCalls(void) {
    bool all_ok = true;
    for (int i = 0; i < 5; ++i) {
        cmsInt32Number res = CheckLab2xyY();
        if (res == 0) {
            // If any call returns zero, the predicate is not satisfied for this run.
            // Still count as a potential failure for continued correctness checking.
            all_ok = false;
        }
    }
    TEST_ASSERT(all_ok, "CheckLab2xyY repeated calls should not crash and return non-zero consistently.");
}

// Test 3: Independent sanity check of the underlying conversion chain for a known Lab value
// This test mirrors a single pass of the conversion chain used inside CheckLab2xyY
// and asserts that the deltaE distance is within the extremely small tolerance (1e-12).
static void test_CheckLab2xyY_SingleSampleTolerance(void) {
    cmsCIELab Lab, Lab2;
    cmsCIEXYZ XYZ;
    cmsCIExyY xyY;

    // Pick a representative LAB triplet
    Lab.L = 50;
    Lab.a = 0;
    Lab.b = 0;

    // Forward and back conversions
    cmsLab2XYZ(NULL, &XYZ, &Lab);
    cmsXYZ2xyY(&xyY, &XYZ);
    cmsxyY2XYZ(&XYZ, &xyY);
    cmsXYZ2Lab(NULL, &Lab2, &XYZ);

    // DeltaE distance between Lab and recovered Lab
    cmsFloat64Number dist = cmsDeltaE(&Lab, &Lab2);

    // Extremely small tolerance to reflect near-identity roundtrips
    TEST_ASSERT(dist < 1e-12, "DeltaE after Lab->XYZ->xyY->XYZ->Lab should be < 1e-12 for a representative sample.");
}

// Main function: run all tests and print summary
int main(int argc, char* argv[]) {
    // Use a small set of tests focusing on the focal method CheckLab2xyY
    run_test("Test 1: CheckLab2xyY_ReturnsNonZero", test_CheckLab2xyY_ReturnsNonZero);
    run_test("Test 2: CheckLab2xyY_NoCrashOnMultipleCalls", test_CheckLab2xyY_NoCrashOnMultipleCalls);
    run_test("Test 3: CheckLab2xyY_SingleSampleTolerance", test_CheckLab2xyY_SingleSampleTolerance);

    // Summary
    printf("\nTest Summary: %d passed, %d failed.\n", g_tests_passed, g_tests_failed);
    // Exit status: 0 if all tests passed, 1 otherwise
    return (g_tests_failed == 0) ? 0 : 1;
}