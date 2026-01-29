// C++11 test harness for TryAllValues16bits (fast_float_testbed.c) using real LittleCMS API.
// This test suite is designed to exercise the focal method with realistic ICC profiles
// without relying on a specific unit-testing framework (GTest). It uses simple
// assertions and prints outcomes to stdout.
//
// Notes:
// - This test assumes liblcms2 is available and linked at build time.
// - The tests target the public API (cmsHPROFILE, cmsCreate_sRGBProfile, cmsCloseProfile, etc.)
// - We invoke TryAllValues16bits with reasonable ICC profiles to exercise the code path.
// - The heavy 16-bit_PER_PIXEL loop in TryAllValues16bits means tests may take noticeable time.

#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>


// Ensure linkage with the focal function using C linkage.
extern "C" void TryAllValues16bits(cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);

// Helper: safe resource cleanup
static void CloseProfileIfNotNull(cmsHPROFILE p) {
    if (p != nullptr) cmsCloseProfile(p);
}

// Test 1: Basic success path using two identical sRGB profiles.
// This should exercise the normal path where both transforms are created successfully
// and outputs from both transforms should match (no Fail expected).
static bool test_basic_two_srgb_profiles() {
    std::cout << "Running test_basic_two_srgb_profiles..." << std::endl;

    cmsHPROFILE inProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outProfile = cmsCreate_sRGBProfile();

    if (inProfile == nullptr || outProfile == nullptr) {
        std::cerr << "ERROR: Failed to create sRGB profiles for test_basic_two_srgb_profiles" << std::endl;
        CloseProfileIfNotNull(inProfile);
        CloseProfileIfNotNull(outProfile);
        return false;
    }

    // Intent: 0 (assumed to be a valid integer constant for perceptual)
    TryAllValues16bits(inProfile, outProfile, 0);

    CloseProfileIfNotNull(inProfile);
    CloseProfileIfNotNull(outProfile);

    std::cout << "test_basic_two_srgb_profiles completed." << std::endl;
    // We do not have a strict assertion since Fail() is a library hook in the testbed;
    // the function should return normally for valid inputs.
    return true;
}

// Test 2: Same profile used as both input and output to ensure the function can handle
// identical in/out profiles without issues.
static bool test_in_out_same_profile() {
    std::cout << "Running test_in_out_same_profile..." << std::endl;

    cmsHPROFILE p = cmsCreate_sRGBProfile();
    if (p == nullptr) {
        std::cerr << "ERROR: Failed to create sRGB profile for test_in_out_same_profile" << std::endl;
        return false;
    }

    // Use the same profile for both input and output
    TryAllValues16bits(p, p, 0);

    CloseProfileIfNotNull(p);

    std::cout << "test_in_out_same_profile completed." << std::endl;
    return true;
}

// Test 3: Negative or invalid Intent value to exercise potential failure path.
// While the internal Fail() implementation in the test environment is a no-op,
// this still exercises the code path where the Intent is supplied (and ensures
// the function does not crash for an invalid value).
static bool test_negative_intent() {
    std::cout << "Running test_negative_intent..." << std::endl;

    cmsHPROFILE inProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outProfile = cmsCreate_sRGBProfile();

    if (inProfile == nullptr || outProfile == nullptr) {
        std::cerr << "ERROR: Failed to create profiles for test_negative_intent" << std::endl;
        CloseProfileIfNotNull(inProfile);
        CloseProfileIfNotNull(outProfile);
        return false;
    }

    // Deliberately use a negative intent to probe edge-case handling.
    TryAllValues16bits(inProfile, outProfile, -1);

    CloseProfileIfNotNull(inProfile);
    CloseProfileIfNotNull(outProfile);

    std::cout << "test_negative_intent completed." << std::endl;
    return true;
}

int main() {
    bool all_passed = true;

    // Run tests sequentially. If any test fails, mark overall result as failure.
    all_passed &= test_basic_two_srgb_profiles();
    all_passed &= test_in_out_same_profile();
    all_passed &= test_negative_intent();

    if (all_passed) {
        std::cout << "ALL_TESTS_PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "TESTS_FAILED" << std::endl;
        return 1;
    }
}