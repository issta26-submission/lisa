// test_checkdictionary24.cpp
// A lightweight C++11 test suite for the FOCAL_METHOD: CheckDictionary24.
// This test uses the LittleCMS library (lcms2) to create a profile,
// invokes CheckDictionary24 for different Pass values, and verifies
// the expected behavior without terminating on first failure.

#include <lcms2.h>
#include <iostream>
#include <string>
#include <testcms2.h>


// Forward-declare the focal function from the C test file.
// Ensure C linkage to prevent name mangling.
extern "C" cmsInt32Number CheckDictionary24(cmsInt32Number Pass, cmsHPROFILE hProfile);

// Simple non-terminating test assertion helpers.
// They log failures but do not abort subsequent tests.
static int gFailures = 0;
static void FailReport(const char* file, int line, const char* expr) {
    std::cerr << "FAILURE: " << file << ":" << line << " - " << expr << std::endl;
    ++gFailures;
}
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) FailReport(__FILE__, __LINE__, #cond); \
} while(0)
#define EXPECT_EQ(a,b) do { \
    if(static_cast<int>(a) != static_cast<int>(b)) FailReport(__FILE__, __LINE__, (std::string("Expected ") + std::to_string((int)(a)) + " == " + std::to_string((int)(b))).c_str()); \
} while(0)

int main() {
    // Initialize a standard test fixture: an sRGB profile.
    // This profile will be used by the focal function to attach metadata.
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "ERROR: Failed to create sRGB profile fixture.\n";
        return 1;
    }

    // Test Suite:
    // 1) Default path (Pass = 0) should return 0 (no action performed).
    //    This exercises the default branch (no case matched in the switch).
    // 2) Pass = 1 should create and write the dictionary into the profile.
    //    The function should return 1 on success.
    // 3) Pass = 2 should read the dictionary back and verify its contents.
    //    The function should return 1 on success.
    // All assertions are non-terminating to maximize coverage.

    // Case 1: Default path (no operation)
    // Purpose: Ensure that hitting an unsupported Pass value does not crash
    //          and returns 0 as per the function's default behavior.
    EXPECT_EQ(CheckDictionary24(0, hProfile), 0);

    // Case 2: Write dictionary into the profile
    // Purpose: Verify that the function can allocate, populate,
    //          and store a multi-language MLU, plus a couple of entries.
    // Expect a successful write path (return 1).
    // Note: This path also exercises the early return on failure (which we can't force
    // easily here without a faulty library), hence we only verify the success case.
    EXPECT_EQ(CheckDictionary24(1, hProfile), 1);

    // Case 3: Read and validate dictionary contents
    // Purpose: Validate that the metadata written in Pass 1 is retrievable
    //          and that the string data matches exactly (English, Spanish, French, Catalan).
    // Expect a successful read path (return 1) and internal correctness checks.
    EXPECT_EQ(CheckDictionary24(2, hProfile), 1);

    // Cleanup
    cmsCloseProfile(hProfile);

    // Final result reporting
    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED: CheckDictionary24" << std::endl;
        return 0;
    } else {
        std::cerr << "TOTAL FAILURES: " << gFailures << std::endl;
        return 2;
    }
}