/*
  Lightweight C++11 test suite for the focal method:
  cmsBool SetTextTags(cmsHPROFILE hProfile)

  - This test targets the true-path behavior by creating a placeholder profile,
    providing Description and Copyright strings, invoking SetTextTags, and
    verifying that the tags were written correctly by reading them back.

  - A secondary test exercises the false-path branch by temporarily setting
    Description to NULL to observe that the function returns FALSE (when the
    CMS API behaves accordingly for NULL input). This covers one of the
    conditional branches in the method, as far as the test environment allows.

  Notes:
  - This suite relies on Little CMS (lcms2) API. It uses only the C API exposed
    by lcms and the C++ standard library for test orchestration.
  - No GTest/GMock is used per the instructions. A minimal in-house test harness is
    provided with non-terminating assertions (tests continue after failures).
  - The test suite is designed to compile with C++11.
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <utils.h>


// Ensure linkage with the focal C function SetTextTags
extern "C" {
    cmsBool SetTextTags(cmsHPROFILE hProfile);
}

// The focal code presumably relies on these global variables.
// We declare them so we can modify them from tests to trigger different branches.
extern const char* Description;
extern const char* Copyright;

// Provide concrete definitions for the globals used by SetTextTags.
const char* Description = "This is a test description.";
const char* Copyright = "© 2024 Test";


// Simple in-house test harness
namespace TestFramework {

static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAIL] " << msg << " (Condition: " #cond ")\n"; \
    } else { \
        std::cout << "[PASS] " << msg << "\n"; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_total; \
    if (cond) { \
        ++g_failed; \
        std::cerr << "[FAIL] " << msg << " (Condition: " #cond ")\n"; \
    } else { \
        std::cout << "[PASS] " << msg << "\n"; \
    } \
} while (0)

static void summary() {
    std::cout << "\nTest Summary: " << g_total << " tests, "
              << g_failed << " failures.\n";
}

} // namespace TestFramework


// Helper to create a fresh in-memory profile to test with
static cmsHPROFILE createTestProfile() {
    // Create a placeholder/profile to attach tags to.
    // In Little CMS, cmsCreateProfilePlaceholder can be used to obtain a blank profile.
    // Depending on the version, you may need to provide a color space. We'll use NULL to
    // obtain a general placeholder if supported; otherwise fall back to a simple sRGB profile.
    cmsHPROFILE h = cmsCreateProfilePlaceholder(NULL);
    if (!h) {
        // Fallback to a built-in profile if placeholder creation fails in this environment
        // (This path is optional and only used if placeholders aren't supported).
        h = cmsCreate_sRGBProfile(NULL);
    }
    return h;
}


// Test 1: Normal path where Description and Copyright are valid and written correctly.
// Verifies that SetTextTags returns TRUE and that the profile contains the expected texts.
static void test_SetTextTags_NormalPath() {
    std::cout << "Running test_SetTextTags_NormalPath...\n";

    cmsHPROFILE hProfile = createTestProfile();
    if (!hProfile) {
        std::cerr << "[ERROR] Failed to create test profile.\n";
        TestFramework::g_failed++;
        return;
    }

    // Ensure the global strings are set to known values
    Description = "This is a test description.";
    Copyright = "© 2024 Test";

    cmsBool rc = SetTextTags(hProfile);

    TestFramework::EXPECT_TRUE(rc == TRUE, "SetTextTags should return TRUE on valid inputs");

    // Read back the Description tag
    cmsMLU* readDesc = (cmsMLU*) cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
    const char* descValue = NULL;
    if (readDesc) {
        descValue = cmsMLUgetASCII(readDesc, "en", "US");
    }

    bool okDesc = (descValue != NULL) && (Description != NULL) && (std::string(descValue) == std::string(Description));
    TestFramework::EXPECT_TRUE(okDesc, "Profile description tag should match the Description string");

    // Read back the Copyright tag
    cmsMLU* readCopy = (cmsMLU*) cmsReadTag(hProfile, cmsSigCopyrightTag);
    const char* copyValue = NULL;
    if (readCopy) {
        copyValue = cmsMLUgetASCII(readCopy, "en", "US");
    }

    bool okCopy = (copyValue != NULL) && (Copyright != NULL) && (std::string(copyValue) == std::string(Copyright));
    TestFramework::EXPECT_TRUE(okCopy, "Profile copyright tag should match the Copyright string");

    // Cleanup
    cmsCloseProfile(hProfile);

    std::cout << "Finished test_SetTextTags_NormalPath.\n\n";
}

// Test 2: False path where Description becomes NULL to exercise error handling path.
// Depending on the CMS API behavior with NULL input, SetTextTags should ideally return FALSE.
// This test verifies that the code path where a NULL Description triggers an error is exercised.
static void test_SetTextTags_NullDescription_FalsePath() {
    std::cout << "Running test_SetTextTags_NullDescription_FalsePath...\n";

    cmsHPROFILE hProfile = createTestProfile();
    if (!hProfile) {
        std::cerr << "[ERROR] Failed to create test profile.\n";
        TestFramework::g_failed++;
        return;
    }

    // Temporarily set Description to NULL to force failure in cmsMLUsetASCII
    const char* oldDescription = Description;
    Description = NULL;

    cmsBool rc = SetTextTags(hProfile);

    // We expect a FALSE result when Description is NULL
    TestFramework::EXPECT_TRUE(rc == FALSE, "SetTextTags should return FALSE when Description is NULL");

    // Restore original value to avoid affecting other tests
    Description = oldDescription;

    cmsCloseProfile(hProfile);
    std::cout << "Finished test_SetTextTags_NullDescription_FalsePath.\n\n";
}

int main() {
    // Run tests
    test_SetTextTags_NormalPath();
    test_SetTextTags_NullDescription_FalsePath();

    // Print a brief summary
    TestFramework::summary();
    // Return non-zero if any tests failed
    return (TestFramework::g_failed > 0) ? 1 : 0;
}