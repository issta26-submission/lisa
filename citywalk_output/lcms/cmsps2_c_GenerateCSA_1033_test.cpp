// High-quality C++11 unit tests for the focal method: GenerateCSA
// This test suite targets the GenerateCSA function found in cmsps2.c.
// The tests use the public Little CMS API where possible and a minimal
// in-house test harness (no GoogleTest). The tests are designed to
// be executable in a standard C++11 environment and avoid private access.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the Little CMS public API.
// If the environment places cms headers in a non-standard location, adjust include path accordingly.

// Forward declaration of the focal function to enable direct testing.
// It is defined in cmsps2.c (as a non-static global function in the original code).
extern "C" cmsUInt32Number GenerateCSA(cmsContext ContextID,
                                      cmsHPROFILE hProfile,
                                      cmsUInt32Number Intent,
                                      cmsUInt32Number dwFlags,
                                      cmsIOHANDLER* mem);

// Simple test harness utilities
#define ASSERT_TRUE(cond, msg)                                  \
    do {                                                         \
        if (!(cond)) {                                           \
            std::cerr << "[FAIL] " << (msg) << std::endl;       \
            return false;                                        \
        }                                                        \
    } while (0)

#define ASSERT_EQ(a, b, msg)                                     \
    do {                                                           \
        if ((a) != (b)) {                                         \
            std::cerr << "[FAIL] " << (msg) << " (expected " << (b) \
                      << ", got " << (a) << ")" << std::endl;  \
            return false;                                         \
        }                                                         \
    } while (0)

// Helper to safely clean up resources
static void safeClose(cmsIOHANDLER* m, cmsHPROFILE profile) {
    if (m) cmsCloseIOhandler(m);
    if (profile) cmsCloseProfile(profile);
}

// Test 1: GenerateCSA with a standard sRGB profile
// - Purpose: exercise the non-named-color path for a common, valid PCS (XYZ).
// - Coverage: ensures the primary code path executes, and memory usage is reported.
bool test_GenerateCSA_With_sRGB_Profile() {
    std::cout << "Running test_GenerateCSA_With_sRGB_Profile...\n";

    // Create a standard sRGB profile (PCS is XYZ; device class is display)
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    ASSERT_TRUE(hProfile != nullptr, "Failed to create sRGB profile");

    // Create an IO handler that writes to an internal memory buffer (NULL-based writer)
    cmsIOHANDLER* mem = cmsOpenIOhandlerFromNULL(nullptr);
    ASSERT_TRUE(mem != nullptr, "Failed to create NULL IO handler");

    // Call the focal function
    cmsUInt32Number used = GenerateCSA(nullptr, hProfile, 0, 0, mem);

    // Sanity checks: the function should return a non-zero byte count
    ASSERT_TRUE(used > 0, "GenerateCSA should return non-zero bytes for sRGB");

    // The return value should match the internal memory usage reported by the IO handler
    // Note: Access to public member UsedSpace of cmsIOHANDLER is relied upon here.
    ASSERT_EQ(static_cast<uint64_t>(used), static_cast<uint64_t>(mem->UsedSpace),
              "Return value should equal mem->UsedSpace after GenerateCSA");

    // Clean up
    safeClose(mem, hProfile);

    std::cout << "test_GenerateCSA_With_sRGB_Profile passed.\n";
    return true;
}

// Test 2: GenerateCSA with a Lab4 profile (XYZ/LM-based PCS is Lab)
// - Purpose: exercise the Lab data path (ColorSpace == cmsSigLabData) as allowed by the code.
// - Coverage: ensures the Lab data branch is exercised and that memory usage is produced.
bool test_GenerateCSA_With_Lab4_Profile() {
    std::cout << "Running test_GenerateCSA_With_Lab4_Profile...\n";

    // Create a Lab4 profile with D50 white point
    cmsCIEXYZ WhitePt = { 0.9642, 1.0, 0.8249 }; // D50
    cmsHPROFILE hProfile = cmsCreateLab4Profile(&WhitePt);
    ASSERT_TRUE(hProfile != nullptr, "Failed to create Lab4 profile");

    // Use NULL IO handler to request in-memory handling
    cmsIOHANDLER* mem = cmsOpenIOhandlerFromNULL(nullptr);
    ASSERT_TRUE(mem != nullptr, "Failed to create NULL IO handler for Lab4 path");

    // Call the focal function
    cmsUInt32Number used = GenerateCSA(nullptr, hProfile, 0, 0, mem);

    // Expect a successful non-zero result
    ASSERT_TRUE(used > 0, "GenerateCSA should return non-zero bytes for Lab4 profile");

    // Basic internal consistency: mem->UsedSpace should reflect the return value
    ASSERT_EQ(static_cast<uint64_t>(used), static_cast<uint64_t>(mem->UsedSpace),
              "Return value should equal mem->UsedSpace after GenerateCSA (Lab4 path)");

    // Clean up
    safeClose(mem, hProfile);

    std::cout << "test_GenerateCSA_With_Lab4_Profile passed.\n";
    return true;
}

// Optional: Test infrastructure - run all tests
int main() {
    bool all_passed = true;

    if (!test_GenerateCSA_With_sRGB_Profile()) {
        std::cerr << "Test suite aborted: test_GenerateCSA_With_sRGB_Profile failed.\n";
        all_passed = false;
    }

    if (!test_GenerateCSA_With_Lab4_Profile()) {
        std::cerr << "Test suite aborted: test_GenerateCSA_With_Lab4_Profile failed.\n";
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "All tests passed successfully.\n";
        return 0;
    } else {
        std::cerr << "One or more tests failed.\n";
        return 1;
    }
}