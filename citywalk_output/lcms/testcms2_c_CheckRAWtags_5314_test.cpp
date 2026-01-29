// Minimal C++11 test suite for the focal C function CheckRAWtags
// This test uses a lightweight, non-terminating assertion approach and a small main()-driven test runner
// It targets the function:
//   cmsInt32Number CheckRAWtags(cmsInt32Number Pass, cmsHPROFILE hProfile)
//
// Assumptions:
// - LittleCMS (lcms2) is available and linked (headers: lcms2.h)
// - The function CheckRAWtags is defined in testcms2.c (as per the provided FOCAL_METHOD)
// - The tests create a real profile (srgb) to exercise the tag write/read path

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


extern "C" {
    // Forward declare the focal function (C linkage)
    // The actual implementation is in testcms2.c
    typedef int cmsInt32Number;
    typedef void* cmsHPROFILE;
    cmsInt32Number CheckRAWtags(cmsInt32Number Pass, cmsHPROFILE hProfile);
    // LittleCMS profile creation/deletion (we rely on standard API)
    cmsHPROFILE cmsCreate_sRGBProfile(void);
    void cmsCloseProfile(cmsHPROFILE hProfile);
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        fprintf(stderr, "ASSERT FAILED: %s (in %s:%d)\\n", msg, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)


// Test 1: Verify that Pass = 1 writes a raw tag to a valid profile and returns success
// - Creates a real sRGB profile
// - Calls CheckRAWtags with Pass = 1 and expects non-zero (success)
// - Cleans up profile
void test_CheckRAWtags_WriteTag_Succeeds_FromValidProfile() {
    // Create a real profile that can receive a raw tag
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    EXPECT_TRUE(hProfile != nullptr, "Failed to create sRGB profile for Pass=1 test");

    if (hProfile) {
        cmsInt32Number res = CheckRAWtags(1, hProfile);
        EXPECT_TRUE(res != 0, "CheckRAWtags with Pass=1 should succeed writing raw tag into valid profile");
    }

    if (hProfile) cmsCloseProfile(hProfile);
}

// Test 2: Verify that Pass = 2 reads back the previously written raw tag correctly on a valid profile
// - Writes the tag first (via Pass = 1) to ensure a tag exists
// - Then reads it back (Pass = 2) and expects 1 (success path)
// - Cleans up profile
void test_CheckRAWtags_ReadTag_Succeeds_AfterWrite() {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    EXPECT_TRUE(hProfile != nullptr, "Failed to create sRGB profile for Pass=2 test");

    if (!hProfile) return;

    // First write the tag
    cmsInt32Number wres = CheckRAWtags(1, hProfile);
    EXPECT_TRUE(wres != 0, "Precondition: Pass=1 should succeed to write raw tag before Pass=2");

    // Now read and verify
    cmsInt32Number rres = CheckRAWtags(2, hProfile);
    EXPECT_TRUE(rres == 1, "CheckRAWtags with Pass=2 should read and verify the data written by Pass=1");

    if (hProfile) cmsCloseProfile(hProfile);
}

// Test 3: Verify that Pass = 2 on a fresh (untouched) profile returns 0 (read failure path)
// - Creates a fresh profile with no tag
// - Calls Pass = 2 and expects 0
void test_CheckRAWtags_ReadTag_OnFreshProfile_Returns0() {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    EXPECT_TRUE(hProfile != nullptr, "Failed to create sRGB profile for Pass=2-on-fresh profile test");

    if (!hProfile) return;

    cmsInt32Number rres = CheckRAWtags(2, hProfile);
    EXPECT_TRUE(rres == 0, "Pass=2 on a fresh profile should fail to read and return 0");

    if (hProfile) cmsCloseProfile(hProfile);
}

// Test 4: Verify that default branch is taken for arbitrary Pass values (not 1 or 2)
// - Uses a valid profile and a Pass value outside {1,2} (e.g., 0)
// - Expects 0
void test_CheckRAWtags_DefaultBranch_Returns0() {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    EXPECT_TRUE(hProfile != nullptr, "Failed to create sRGB profile for default-branch test");

    if (!hProfile) return;

    cmsInt32Number res = CheckRAWtags(0, hProfile);
    EXPECT_TRUE(res == 0, "Default branch should return 0 for Pass values not equal to 1 or 2");

    if (hProfile) cmsCloseProfile(hProfile);
}


// Simple test runner
void run_all_tests() {
    test_CheckRAWtags_WriteTag_Succeeds_FromValidProfile();
    test_CheckRAWtags_ReadTag_Succeeds_AfterWrite();
    test_CheckRAWtags_ReadTag_OnFreshProfile_Returns0();
    test_CheckRAWtags_DefaultBranch_Returns0();
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    printf("Running tests for CheckRAWtags...\n");
    run_all_tests();

    printf("Test results: total=%d, failed=%d\n", g_total_tests, g_failed_tests);
    if (g_failed_tests > 0) {
        printf("Some tests FAILED.\n");
        return 1;
    } else {
        printf("All tests PASSED.\n");
        return 0;
    }
}