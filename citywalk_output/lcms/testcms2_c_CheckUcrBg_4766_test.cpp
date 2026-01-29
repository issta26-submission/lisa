// Minimal C++11 test suite for the focal method CheckUcrBg in testcms2.c
// This test suite exercises Pass = 1 (write UCR/BG tag) and Pass = 2 (read and verify),
// along with edge cases (default case and read without prior write).
// Notes:
// - Uses LittleCMS API (lcms2.h) and test harness helpers (testcms2.h) provided by the project.
// - No Google Test; a lightweight in-file test harness with non-terminating assertions is used.
// - The tests print diagnostics and return a non-zero exit code if any test failed.

#include <cstring>
#include <iostream>
#include <lcms2.h>
#include <testcms2.h>


// Ensure C linkage for C headers when included in a C++ translation unit
extern "C" {
}

// Lightweight assertion utilities (non-terminating)
static int g_test_failures = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT_TRUE FAILED: " << (msg) << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

#define ASSERT_EQ(a, b, msg) do { \
    if (static_cast<decltype(a)>(a) != static_cast<decltype(b)>(b)) { \
        std::cerr << "ASSERT_EQ FAILED: " << (msg) \
                  << " (Expected: " << (b) << ", Got: " << (a) << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

static void RunTest_WriteReadRoundTrip_Test()
{
    // Test 1: Write a UCR/BG tag (Pass = 1) and then read/verify (Pass = 2) using a known profile.
    // Create a simple sRGB profile as the test target.
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "TEST FAILED: Unable to create sRGB profile for Test 1." << std::endl;
        ++g_test_failures;
        return;
    }

    // Use the UCR/BG tag signature expected by the focal method.
    cmsTagSignature tag = cmsSigUcrBgTag;

    // Pass 1: Write the UCR/BG tag
    cmsInt32Number rc_write = CheckUcrBg(1, hProfile, tag);
    ASSERT_TRUE(rc_write != 0, "CheckUcrBg Pass 1 should return non-zero (successful write).");

    // Pass 2: Read back and verify the Desc string
    cmsInt32Number rc_read = CheckUcrBg(2, hProfile, tag);
    ASSERT_EQ(rc_read, 1, "CheckUcrBg Pass 2 should return 1 after successful read and verification.");

    // Cleanup
    cmsCloseProfile(hProfile);
}

static void RunTest_ReadWithoutWrite_Test()
{
    // Test 2: Ensure that attempting to read a non-existent tag returns 0.
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "TEST FAILED: Unable to create sRGB profile for Test 2." << std::endl;
        ++g_test_failures;
        return;
    }

    cmsTagSignature tag = cmsSigUcrBgTag;

    // Do not write any data to this profile. Directly call Pass = 2.
    cmsInt32Number rc = CheckUcrBg(2, hProfile, tag);
    ASSERT_EQ(rc, 0, "CheckUcrBg Pass 2 on a fresh profile should return 0 (no tag present).");

    cmsCloseProfile(hProfile);
}

static void RunTest_DefaultCase_Test()
{
    // Test 3: Verify that an unsupported Pass value (default case) returns 0.
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "TEST FAILED: Unable to create sRGB profile for Test 3." << std::endl;
        ++g_test_failures;
        return;
    }

    cmsTagSignature tag = cmsSigUcrBgTag;

    // Pass 3 (default case)
    cmsInt32Number rc = CheckUcrBg(3, hProfile, tag);
    ASSERT_EQ(rc, 0, "CheckUcrBg Pass 3 (default) should return 0.");

    cmsCloseProfile(hProfile);
}

int main(void)
{
    // Run tests
    RunTest_WriteReadRoundTrip_Test();
    RunTest_ReadWithoutWrite_Test();
    RunTest_DefaultCase_Test();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}