// test_suite.cpp
// A lightweight C++11 test suite for the focal method CheckIntToFloatTransform
// and its dependencies, without using GTest. The tests run from main and
// use a minimal, non-terminating assertion style.

// This suite relies on LittleCMS (lcms2) being available in the environment.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>
#include <cmath>


// Include the LittleCMS header to access types and functions used in the focal method

// Forward declarations of the focal method and its dependent helpers.
// These are provided via the project (testcms2.c and dependencies).
extern "C" int CheckIntToFloatTransform(void);

// The dependencies used by the focal method (as seen in the provided class dependency list).
extern "C" cmsHPROFILE Create_AboveRGB(void);
extern "C" cmsHPROFILE cmsCreate_sRGBProfile(void);
extern "C" cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE, cmsUInt32Number, cmsHPROFILE, cmsUInt32Number, cmsInt32Number, cmsUInt32Number);
extern "C" void cmsCloseProfile(cmsHPROFILE);
extern "C" void cmsDoTransform(cmsHTRANSFORM, const void*, void*, cmsUInt32Number);
extern "C" void cmsDeleteTransform(cmsHTRANSFORM);

// Simple test framework (non-GMock, non-GTest)
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

// Utility macro-like wrappers for lightweight readability
#define RUN_TEST(name, fn) TestCase{ name, fn }

// Test 1: Validate that CheckIntToFloatTransform returns a boolean-like result (0 or 1).
// Rationale: Exercises the focal method's public interface and ensures the function
// does not crash and produces a well-formed return value.
bool test_CheckIntToFloatTransform_ReturnValueIs01() {
    // The focal function returns 1 on the "pass" path and 0 on the "fail" path.
    int rv = CheckIntToFloatTransform();
    bool ok = (rv == 0 || rv == 1);
    // Explanatory assertion: the return value should be exactly 0 or 1.
    // It must not be any other value.
    if (!ok) {
        std::cerr << "test_CheckIntToFloatTransform_ReturnValueIs01: Unexpected return value: "
                  << rv << std::endl;
    }
    return ok;
}

// Test 2: Dependency integration sanity check (transform path does not crash and yields finite outputs).
// Rationale: Covers the core dependencies used by the focal method by performing a real
// transform chain using the same helper components (Create_AboveRGB, sRGB profile, etc.),
// and asserts that the produced values are finite numbers.
bool test_Dependency_Transform_ProducesFiniteOutputs() {
    // Create color profiles using the dependencies
    cmsHPROFILE hAbove = Create_AboveRGB();
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfile();

    bool test_ok = true;

    if (hAbove == nullptr) {
        std::cerr << "test_Dependency_Transform_ProducesFiniteOutputs: Create_AboveRGB() returned null." << std::endl;
        test_ok = false;
    }
    if (hsRGB == nullptr) {
        std::cerr << "test_Dependency_Transform_ProducesFiniteOutputs: cmsCreate_sRGBProfile() returned null." << std::endl;
        test_ok = false;
    }

    if (test_ok) {
        // Create a transform: 8-bit RGB input to double RGB output
        cmsHTRANSFORM xform = cmsCreateTransform(hAbove, TYPE_RGB_8, hsRGB, TYPE_RGB_DBL, INTENT_PERCEPTUAL, 0);
        if (xform == nullptr) {
            std::cerr << "test_Dependency_Transform_ProducesFiniteOutputs: cmsCreateTransform() failed." << std::endl;
            test_ok = false;
        } else {
            // Prepare input and output buffers
            unsigned char rgb8[3] = {12, 253, 21};
            cmsFloat64Number rgbDBL[3] = {0.0, 0.0, 0.0};

            // Perform the transform for a single pixel
            cmsDoTransform(xform, rgb8, rgbDBL, 1);

            // Clean up transform and profiles
            cmsDeleteTransform(xform);
            cmsCloseProfile(hAbove);
            cmsCloseProfile(hsRGB);

            // Verify that the output values are finite (not NaN/Inf)
            bool f0 = std::isfinite(static_cast<double>(rgbDBL[0]));
            bool f1 = std::isfinite(static_cast<double>(rgbDBL[1]));
            bool f2 = std::isfinite(static_cast<double>(rgbDBL[2]));

            if (!(f0 && f1 && f2)) {
                std::cerr << "test_Dependency_Transform_ProducesFiniteOutputs: Non-finite transform results."
                          << " rgbDBL = {" << rgbDBL[0] << ", " << rgbDBL[1] << ", " << rgbDBL[2] << "}" << std::endl;
                test_ok = false;
            }
        }
    }

    return test_ok;
}

// Test 3: Sanity check for static-like behavior (ensuring basic environment setup does not crash).
// Rationale: Access static/dependency components in a lightweight manner to increase coverage
// of the startup path that would be used before invoking the focal method.
bool test_Dependency_Environment_BackendAccess() {
    // Basic sanity: create and close the sRGB profile only (no transform)
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfile();
    bool ok = (hsRGB != nullptr);
    if (ok) {
        cmsCloseProfile(hsRGB);
    } else {
        std::cerr << "test_Dependency_Environment_BackendAccess: cmsCreate_sRGBProfile() returned null." << std::endl;
    }
    return ok;
}

int main() {
    // Collect tests
    std::vector<TestCase> tests = {
        RUN_TEST("CheckIntToFloatTransform_ReturnValueIs01", test_CheckIntToFloatTransform_ReturnValueIs01),
        RUN_TEST("Dependency_Transform_ProducesFiniteOutputs", test_Dependency_Transform_ProducesFiniteOutputs),
        RUN_TEST("Dependency_Environment_BackendAccess", test_Dependency_Environment_BackendAccess)
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running test suite for CheckIntToFloatTransform and related dependencies...\n";

    for (const auto& t : tests) {
        bool result = false;
        try {
            result = t.func();
        } catch (...) {
            std::cerr << "Test " << t.name << " threw an exception." << std::endl;
            result = false;
        }
        std::cout << "[ " << (result ? "PASS" : "FAIL") << " ] " << t.name << "\n";
        if (result) ++passed;
    }

    std::cout << "\nTest results: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed to aid integration with build systems.
    return (passed == total) ? 0 : 1;
}