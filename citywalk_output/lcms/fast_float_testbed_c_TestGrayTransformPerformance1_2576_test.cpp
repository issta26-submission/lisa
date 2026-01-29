// Lightweight C++11 unit test suite for the focal method TestGrayTransformPerformance1
// and its dependent components (lcms2 based). No GTest used; a small in-house runner is implemented.
// The tests exercise creation of gamma curves, device-link profiles, and a transform path,
// plus a direct invocation of the focal performance test function.
//
// Note: This test suite assumes the Little CMS API (lcms2.h) is available in the include path.
// The focal method is defined in a C file and linked with the test binary.

#include <exception>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>


extern "C" {
}

// If the MPixSec function is defined in the focal C file, declare it here for testing.
extern "C" cmsFloat64Number MPixSec(cmsFloat64Number diff);

// Forward declaration of the focal method to be invoked by tests.
extern "C" void TestGrayTransformPerformance1();

// Simple assertion utilities for test clarity
#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { throw std::runtime_error(std::string("Assertion failed: ") + #cond); } \
} while(0)

#define ASSERT_NOT_NULL(p) do { \
    if((p) == nullptr) { throw std::runtime_error(std::string("Null pointer encountered: ") + #p); } \
} while(0)

// Lightweight test harness
struct TestCase {
    const char* name;
    void (*func)();
};

// Test 1: Basic invocation of the focal method
// Purpose: Ensure that the focal method TestGrayTransformPerformance1 runs without crashing.
// Rationale: Validates the integration of the overall pipeline (gamma curves, profiles, transform, timing).
void TestGrayTransformPerformance1_BasicRun() {
    // Call the focal performance test as-is. If it crashes, the test will fail (uncaught crash).
    TestGrayTransformPerformance1();
}

// Test 2: Gamma curves and linearization device-link profiles creation/destruction
// Purpose: Verify that gamma curves can be built and both input/output profiles can be created
//          and destroyed without leaking resources or crashing.
void TestGammaProfilesCreation() {
    // Create two gamma curves (1.8 and 1/2.2)
    cmsToneCurve* gamma18 = cmsBuildGamma(0, 1.8);
    cmsToneCurve* gamma22 = cmsBuildGamma(0, 1.0/2.2);

    ASSERT_NOT_NULL(gamma18);
    ASSERT_NOT_NULL(gamma22);

    // Create linearization profiles (gray data) using the gamma curves
    cmsHPROFILE hlcmsProfileIn = cmsCreateLinearizationDeviceLink(cmsSigGrayData, &gamma18);
    cmsHPROFILE hlcmsProfileOut = cmsCreateLinearizationDeviceLink(cmsSigGrayData, &gamma22);

    ASSERT_NOT_NULL(hlcmsProfileIn);
    ASSERT_NOT_NULL(hlcmsProfileOut);

    // Free the gamma curves (profiles should retain their own data)
    cmsFreeToneCurve(gamma18);
    cmsFreeToneCurve(gamma22);

    // Close the profiles
    cmsCloseProfile(hlcmsProfileIn);
    cmsCloseProfile(hlcmsProfileOut);
}

// Test 3: Transform creation, minimal execution, and cleanup
// Purpose: Ensure the transform path can be created and a single-pixel transform can be executed
//          without crashing. This exercises the API surface used by TestGrayTransformPerformance1
//          at a reduced workload.
void TestTransformCreationAndExecute() {
    // Create gamma curves
    cmsToneCurve* gamma18 = cmsBuildGamma(0, 1.8);
    cmsToneCurve* gamma22 = cmsBuildGamma(0, 1.0/2.2);

    ASSERT_NOT_NULL(gamma18);
    ASSERT_NOT_NULL(gamma22);

    // Create input/output profiles
    cmsHPROFILE hlcmsProfileIn  = cmsCreateLinearizationDeviceLink(cmsSigGrayData, &gamma18);
    cmsHPROFILE hlcmsProfileOut = cmsCreateLinearizationDeviceLink(cmsSigGrayData, &gamma22);

    ASSERT_NOT_NULL(hlcmsProfileIn);
    ASSERT_NOT_NULL(hlcmsProfileOut);

    // Free the gamma curves as in the focal method
    cmsFreeToneCurve(gamma18);
    cmsFreeToneCurve(gamma22);

    // Create a transform for gray float data
    cmsHTRANSFORM hlcmsxform = cmsCreateTransform(hlcmsProfileIn, TYPE_GRAY_FLT,
                                                  hlcmsProfileOut, TYPE_GRAY_FLT,
                                                  INTENT_PERCEPTUAL, 0);
    ASSERT_NOT_NULL(hlcmsxform);

    // Prepare a single input sample (1 pixel)
    float input = 0.5f;
    float output = 0.0f;

    // Perform the transform on a tiny buffer
    cmsDoTransform(hlcmsxform, &input, &output, 1);

    // Basic sanity: output should be a float value in [0,1]
    ASSERT_TRUE(output >= 0.0f && output <= 1.0f);

    // Cleanup
    cmsDeleteTransform(hlcmsxform);
    cmsCloseProfile(hlcmsProfileIn);
    cmsCloseProfile(hlcmsProfileOut);
}

// Test 4: MPixSec function basic sanity
// Purpose: Ensure MPixSec returns a non-negative value for a representative diff value.
// This exercises a tiny portion of the performance-related utilities used by the focal method.
void Test_MPixSec_Function() {
    cmsFloat64Number diff = 123.456;
    cmsFloat64Number result = MPixSec(diff);
    ASSERT_TRUE(result >= 0.0);
}

// Test 5: Re-run focal method as a separate verification path
// Purpose: Double-check the focal method's stability across multiple invocations.
void TestGrayTransformPerformance1_ReRun() {
    TestGrayTransformPerformance1();
}

// Main runner
int main() {
    // Aggregate tests
    std::vector<TestCase> tests = {
        { "TestGrayTransformPerformance1_BasicRun", TestGrayTransformPerformance1_BasicRun },
        { "TestGammaProfilesCreation",        TestGammaProfilesCreation },
        { "TestTransformCreationAndExecute",  TestTransformCreationAndExecute },
        { "Test_MPixSec_Function",           Test_MPixSec_Function },
        { "TestGrayTransformPerformance1_ReRun", TestGrayTransformPerformance1_ReRun }
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running " << total << " tests for TestGrayTransformPerformance1 focal path..." << std::endl;

    for (const auto& tc : tests) {
        try {
            std::cout << " [ RUN      ] " << tc.name << std::endl;
            tc.func();
            std::cout << " [       OK ] " << tc.name << std::endl;
            ++passed;
        } catch (const std::exception& e) {
            std::cerr << " [  FAILED  ] " << tc.name << " - Exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << " [  FAILED  ] " << tc.name << " - Unknown exception" << std::endl;
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Non-zero exit code on failure to enable integration with CI systems
    return (passed == total) ? 0 : 1;
}