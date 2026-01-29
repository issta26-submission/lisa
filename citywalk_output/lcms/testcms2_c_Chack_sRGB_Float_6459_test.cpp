// unit_tests.cpp
// C++11 test harness for the focal method Chack_sRGB_Float in testcms2.c
// This file provides a small, non-GTest test suite with non-terminating assertions.
// It imports the existing C test infrastructure and exercises both the focal
// function and related helper predicates to improve coverage.

#include <iostream>
#include <testcms2.h>
#include <iomanip>


// Import C dependencies
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Global test failure counter (non-terminating assertions)
static int g_failures = 0;

// Simple macro for non-terminating assertions with logging
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Helper structure to hold a pair of CMS transforms
struct TransformPair {
    cmsHTRANSFORM t1;
    cmsHTRANSFORM t2;
};

// Helper function to build the same transform pair used by Chack_sRGB_Float
// This mirrors the setup in the focal method: sRGB -> XYZ (float) and sRGB -> Lab (Lab-DBL)
static TransformPair Build_sRGB_XYZ_Lab_Transforms() {
    TransformPair p = { nullptr, nullptr };

    // Create profiles
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfileTHR(DbgThread());
    cmsHPROFILE hXYZ  = cmsCreateXYZProfileTHR(DbgThread());
    cmsHPROFILE hLab  = cmsCreateLab4ProfileTHR(DbgThread(), NULL);

    // Create transforms
    p.t1 = cmsCreateTransformTHR(DbgThread(), hsRGB, TYPE_RGB_FLT,
                                 hXYZ, TYPE_XYZ_DBL,
                                 INTENT_RELATIVE_COLORIMETRIC, 0);
    p.t2 = cmsCreateTransformTHR(DbgThread(), hsRGB, TYPE_RGB_FLT,
                                 hLab, TYPE_Lab_DBL,
                                 INTENT_RELATIVE_COLORIMETRIC, 0);

    // Close profiles (as in the focal method)
    cmsCloseProfile(hsRGB);
    cmsCloseProfile(hXYZ);
    cmsCloseProfile(hLab);

    return p;
}

// Test 1: Ensure Chack_sRGB_Float executes and returns non-zero (success path)
static void test_Chack_sRGB_Float_passes() {
    // This test ensures the focal method runs through and yields a successful result.
    int rc = Chack_sRGB_Float();
    EXPECT_TRUE(rc != 0, "Chack_sRGB_Float() should return non-zero (success path)");
}

// Test 2: Directly verify CheckOneRGB_f success path using a valid transform
static void test_CheckOneRGB_f_success_path() {
    TransformPair p = Build_sRGB_XYZ_Lab_Transforms();

    // Use the same inputs and expected XYZ values as in the focal method for a valid case
    cmsInt32Number rc = CheckOneRGB_f(p.t1, 1, 1, 1,
                                      0.0002927, 0.0003035, 0.000250, 0.0001);

    EXPECT_TRUE(rc != 0, "CheckOneRGB_f should return non-zero for a valid (1,1,1) input against Xform1");
    // Cleanup
    cmsDeleteTransform(p.t1);
    cmsDeleteTransform(p.t2);
}

// Test 3: Directly verify CheckOneRGB_f failure path by injecting an incorrect expected value
static void test_CheckOneRGB_f_failure_path() {
    TransformPair p = Build_sRGB_XYZ_Lab_Transforms();

    // Intentionally corrupt the expected Y value to force a mismatch (within the same tolerance)
    cmsInt32Number rc = CheckOneRGB_f(p.t1, 1, 1, 1,
                                      0.0002927, 0.0, 0.0, 0.0001);

    EXPECT_TRUE(rc == 0, "CheckOneRGB_f should return zero for a deliberately incorrect expected value");
    // Cleanup
    cmsDeleteTransform(p.t1);
    cmsDeleteTransform(p.t2);
}

// Test 4: Memory-leak sanity check after running the focal test and a helper
static void test_MemoryLeaks_after_Chack_sRGB_Float() {
    // Run the focal method to ensure normal operation
    int rc = Chack_sRGB_Float();
    EXPECT_TRUE(rc != 0, "Chack_sRGB_Float() should return non-zero (memory-leak sensitive path)");

    // Run memory leakage reporting if available
    // This mirrors the project's test harness usage
    TestMemoryLeaks(true);
}

// Runner: execute all tests and report summary
static void run_all_tests() {
    std::cout << "Running unit tests for Chack_sRGB_Float and related helpers..." << std::endl;

    test_Chack_sRGB_Float_passes();
    test_CheckOneRGB_f_success_path();
    test_CheckOneRGB_f_failure_path();
    test_MemoryLeaks_after_Chack_sRGB_Float();

    if(g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << "Tests completed with failures: " << g_failures << std::endl;
    }
}

// Main entry point for the test suite
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    run_all_tests();
    return g_failures ? 1 : 0;
}