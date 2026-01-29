// Test suite for the focal function CreateCurves in threaded_testbed.c
// This test is designed to be compiled with C++11, without using GTest.
// It relies on the Little CMS (lcms2) library to validate the produced profile.
// The tests are written as a small, non-terminating harness: they report PASS/FAIL
// but do not abort on first failure, allowing multiple tests to execute in a single run.
//
// Assumptions:
// - Little CMS (lcms2) headers and library are available on the system.
// - The focal function CreateCurves has C linkage and is accessible from this test.
// - The project uses the signature: cmsHPROFILE CreateCurves(void);
//
// Notes from Step 1/2/3 (mapped to tests):
// - Core dependent components (Candidate Keywords): Gamma, Transfer array, cmsCreateLinearizationDeviceLink,
//   cmsBuildGamma, cmsFreeToneCurve, cmsHPROFILE, cmsSigRgbData, device-link profile.
// - Tests cover: non-null return, color space characteristics (rgb data space), and behavior across multiple calls.
// - Static/internal helpers are not directly invoked; we access only the public interface of CreateCurves.
// - No GTest; use simple C++ I/O with clear per-test comments and pass/fail reporting.

#include <cstdio>
#include <threaded_internal.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>


// Little CMS header (assumed to be available in the environment)

// Ensure C linkage for the focal function when declaring it for the test.
// The focal function is defined in threaded_testbed.c (C source).
extern "C" cmsHPROFILE CreateCurves(void);

// Test 1: Validate that CreateCurves returns a non-null profile and that its color space
// is the expected RGB data space. This checks the primary contract of the function
// (creation of a valid device-link profile for RGB data).
bool test_CreateCurves_ReturnsNonNullAndRGBSpace()
{
    // Call the focal function
    cmsHPROFILE h = CreateCurves();

    // Result record
    bool passed = true;

    if (h == nullptr) {
        std::cerr << "[Test 1] FAIL: CreateCurves returned null profile." << std::endl;
        passed = false;
    } else {
        // Inspect the color space signature of the resulting profile.
        // For a transfer curve built from Gamma for RGB data, the color space is expected to be RGB data.
        cmsColorSpaceSignature space = cmsGetColorSpace(h);

        // The expected color space for the provided Gamma/Transfer is RGB data.
        // If this assumption is wrong on a particular Little CMS version, this check may fail.
        if (space != cmsSigRgbData) {
            std::cerr << "[Test 1] FAIL: Expected color space cmsSigRgbData, got "
                      << std::hex << static_cast<unsigned>(space) << std::dec << std::endl;
            passed = false;
        }
    }

    // Cleanup
    if (h != nullptr) {
        cmsCloseProfile(h);
    }

    return passed;
}

// Test 2: Ensure that multiple invocations of CreateCurves produce distinct profile handles.
// This helps exercise the allocation path and verifies that successive calls do not
// inadvertently return the same handle (which would indicate caching or incorrect reuse).
bool test_CreateCurves_MultipleCallsProduceDistinctHandles()
{
    cmsHPROFILE h1 = CreateCurves();
    cmsHPROFILE h2 = CreateCurves();

    bool passed = true;

    if (h1 == nullptr) {
        std::cerr << "[Test 2] FAIL: First call to CreateCurves returned null." << std::endl;
        passed = false;
    }

    if (h2 == nullptr) {
        std::cerr << "[Test 2] FAIL: Second call to CreateCurves returned null." << std::endl;
        if (h1 != nullptr) cmsCloseProfile(h1);
        passed = false;
        return passed;
    }

    if (h1 == h2) {
        std::cerr << "[Test 2] FAIL: Consecutive calls returned the same profile handle." << std::endl;
        passed = false;
    }

    // Cleanup
    if (h1 != nullptr) cmsCloseProfile(h1);
    if (h2 != nullptr) cmsCloseProfile(h2);

    return passed;
}

int main()
{
    // Simple, non-terminating test runner.
    // Step 2/Step 3 coverage: two core scenarios for the focal function CreateCurves.

    int total = 2;
    int passed = 0;

    std::cout << "Running tests for CreateCurves (threaded_testbed.c) using Little CMS ..." << std::endl;

    if (test_CreateCurves_ReturnsNonNullAndRGBSpace()) {
        std::cout << "[PASS] Test_CreateCurves_ReturnsNonNullAndRGBSpace" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test_CreateCurves_ReturnsNonNullAndRGBSpace" << std::endl;
    }

    if (test_CreateCurves_MultipleCallsProduceDistinctHandles()) {
        std::cout << "[PASS] Test_CreateCurves_MultipleCallsProduceDistinctHandles" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test_CreateCurves_MultipleCallsProduceDistinctHandles" << std::endl;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed to aid CI systems.
    return (passed == total) ? 0 : 1;
}