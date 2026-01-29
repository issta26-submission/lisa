// Test suite for the focal method: BuildRGBInputMatrixShaper
// This test suite is designed to be compiled with C++11 and linked against the LittleCMS (lcms2) library.
// It uses a lightweight, self-contained assertion mechanism (no GoogleTest) and provides
// explanatory comments for each unit test.

// Important: The environment must have the LittleCMS library available (lcms2).link accordingly.
// The test focuses on the positive-path where a valid profile yields a non-NULL pipeline.
// Negative-path coverage for missing TRC tags or ReadICCMatrixRGB2XYZ failure requires crafted profiles or mocks
// of library internals, which is non-trivial without altering the build unit. The test below demonstrates
// the standard successful scenario using a known-good profile.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <lcms2.h>


// Simple non-terminating assertion macros to satisfy the "non-terminating assertions" guideline.
static int g_testFailures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
            ++g_testFailures; \
        } \
    } while(0)

#define EXPECT_NOT_NULL(ptr) \
    do { \
        if((ptr) == nullptr) { \
            std::cerr << "EXPECT_NOT_NULL failed: " #ptr " is NULL at " __FILE__ ":" << __LINE__ << std::endl; \
            ++g_testFailures; \
        } \
    } while(0)

#define TEST_PASSED_MSG(name) \
    do { \
        std::cout << "[TEST PASSED] " #name << std::endl; \
    } while(0)

#define TEST_FAILED_MSG(name) \
    do { \
        std::cerr << "[TEST FAILED] " #name << std::endl; \
    } while(0)

// Test 1: BuildRGBInputMatrixShaper with a standard sRGB profile should succeed and return a non-NULL pipeline.
// Rationale: The sRGB ICC profile is a canonical, valid profile with RGB-to-XYZ matrix and TRC curves for R, G, B.
// The method should read the matrix, adjust it, read the TRCs, allocate a 3x3 pipeline, and insert stages.
void test_BuildRGBInputMatrixShaper_WithSRGBProfile_ReturnsNonNull() {
    // Create a standard sRGB profile using LittleCMS public API
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    EXPECT_NOT_NULL(hProfile);

    // Call the focal method under test
    cmsPipeline* Lut = BuildRGBInputMatrixShaper(hProfile);

    // Expect a non-NULL pipeline
    EXPECT_NOT_NULL(Lut);

    // Cleanup resources
    if (Lut) cmsPipelineFree(Lut);
    if (hProfile) cmsCloseProfile(hProfile);

    if (g_testFailures == 0) TEST_PASSED_MSG(test_BuildRGBInputMatrixShaper_WithSRGBProfile_ReturnsNonNull);
    else TEST_FAILED_MSG(test_BuildRGBInputMatrixShaper_WithSRGBProfile_ReturnsNonNull);
}

// Note on test coverage for false branches:
// - The code path contains several conditional branches:
//   - If ReadICCMatrixRGB2XYZ fails, return NULL.
//   - If any of Shapes[0], Shapes[1], Shapes[2] is NULL, return NULL.
//   - After building the Lut, if the PCS is LabData, insert XYZ2Lab stage.
// - Crafting a profile to deterministically trigger the "Shapes missing" NULL branch or
//   the "ReadICCMatrixRGB2XYZ fails" branch requires either a tailored fake ICC profile
//   or internal hooks/mocks to override the dependency behavior (e.g., cmsReadTag, ReadICCMatrixRGB2XYZ).
// - In a typical test environment, this requires more extensive mocking of the LittleCMS internals
//   or the creation of an intentionally malformed ICC profile. Such setup is beyond the scope of
//   a lightweight unit-test harness and may require additional infrastructure.
//
// If a test environment provides a way to synthesize a profile lacking TRC tags (or to fail
// ReadICCMatrixRGB2XYZ), it would look conceptually similar to the following pseudocode:
//   cmsHPROFILE badProfile = CreateMalformedProfile(); // profile without TRC/TRCTag data or invalid matrix
//   cmsPipeline* LutBad = BuildRGBInputMatrixShaper(badProfile);
//   EXPECT_NULL(LutBad);
//   Cleanup...

int main() {
    // Run test 1
    test_BuildRGBInputMatrixShaper_WithSRGBProfile_ReturnsNonNull();

    // Summary
    if (g_testFailures == 0) {
        std::cout << "All tests completed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) failed." << std::endl;
        return 1;
    }
}