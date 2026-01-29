/* 
   Lightweight C++11 test suite for the focal function:
   cmsBool GetXFormColorSpaces(cmsUInt32Number nProfiles, cmsHPROFILE hProfiles[],
                               cmsColorSpaceSignature* Input, cmsColorSpaceSignature* Output)

   Approach:
   - Use public CMS library helpers to build test profiles (e.g., sRGB, Gray where available).
   - Avoid GoogleTest; implement a minimal test harness with non-terminating expectations (EXPECT_* macros).
   - Cover true/false branches where feasible:
     * nProfiles == 0
     * hProfiles[0] == NULL
     * Single SRGB profile (covering the main branch and setting Input/Output)
     * Two profiles (same SRGB for deterministic multi-profile path)
     * Second profile NULL in array triggers FALSE path
   - Use actual LittleCMS API functions to create and release profiles.
   - Provide explanatory comments for each test case.

   Notes:
   - We declare the function prototype for GetXFormColorSpaces manually with extern "C" to allow linking
     with cmsxform.c if the function isn't exposed via a public header.
   - The tests rely on cmsCreate_sRGBProfile() and basic color-space constants such as cmsSigRgbData and cmsSigXYZData.
   - The tests avoid private members and focus on public behavior of the function.

   Compile (example):
     g++ -std=c++11 -I<path-to-lcms-include> test_cms_xform_color_spaces.cpp -lcms2 -o test_xform_color_spaces
*/

#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdio>
#include <cstdint>


// Public include for CMS data types and memory helpers

// Declare the focal function (definition is in cmsxform.c within the library)
extern "C" cmsBool GetXFormColorSpaces(cmsUInt32Number nProfiles,
                                     cmsHPROFILE hProfiles[],
                                     cmsColorSpaceSignature* Input,
                                     cmsColorSpaceSignature* Output);

// Simple test harness: non-terminating assertions to maximize coverage
static int g_failures = 0;

#define EXPECT_TRUE(cond, desc)  do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", (desc)); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_FALSE(cond, desc) do { \
    if (cond) { \
        fprintf(stderr, "EXPECT_FALSE FAILED: %s\n", (desc)); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ_U(a, b, desc) do { \
    unsigned long long _va = (unsigned long long)(a); \
    unsigned long long _vb = (unsigned long long)(b); \
    if (_va != _vb) { \
        fprintf(stderr, "EXPECT_EQ FAILED: %s. Got %llu, expected %llu\n", (desc), _va, _vb); \
        ++g_failures; \
    } \
} while (0)

// Helper to safely close a CMS profile if non-null
static void SafeClose(cmsHPROFILE h)
{
    if (h != NULL) cmsCloseProfile(h);
}

// Test 1: nProfiles == 0 should be rejected by the function (FALSE)
static void Test_GetXFormColorSpaces_ZeroProfiles()
{
    cmsColorSpaceSignature inSig = 0;
    cmsColorSpaceSignature outSig = 0;
    cmsBool res = GetXFormColorSpaces(0, NULL, &inSig, &outSig);
    EXPECT_TRUE(res == 0, "GetXFormColorSpaces with nProfiles==0 must return FALSE");
}

// Test 2: hProfiles[0] == NULL should be rejected (FALSE)
static void Test_GetXFormColorSpaces_PrimaryNullProfile()
{
    cmsHPROFILE profiles[1] = { NULL };
    cmsColorSpaceSignature inSig = 0;
    cmsColorSpaceSignature outSig = 0;
    cmsBool res = GetXFormColorSpaces(1, profiles, &inSig, &outSig);
    EXPECT_TRUE(res == 0, "GetXFormColorSpaces with first profile NULL must return FALSE");
}

// Test 3: Single SRGB profile: expect Input to be rgb and Output to be XYZ
static void Test_GetXFormColorSpaces_SingleSRGB()
{
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB == NULL) {
        fprintf(stderr, "SKIP: Unable to create sRGB profile for Test_GetXFormColorSpaces_SingleSRGB\n");
        return;
    }

    cmsHPROFILE profiles[1] = { hSRGB };
    cmsColorSpaceSignature inSig = 0;
    cmsColorSpaceSignature outSig = 0;

    cmsBool res = GetXFormColorSpaces(1, profiles, &inSig, &outSig);
    EXPECT_TRUE(res != 0, "GetXFormColorSpaces should succeed for a single SRGB profile");

    // For a typical SRGB profile, the input space should be RGB and the output space should be XYZ
    EXPECT_EQ_U((unsigned long long)inSig, (unsigned long long)cmsSigRgbData, "Single SRGB: Input should be cmsSigRgbData");
    EXPECT_EQ_U((unsigned long long)outSig, (unsigned long long)cmsSigXYZData, "Single SRGB: Output should be cmsSigXYZData");

    SafeClose(hSRGB);
}

// Test 4: Two identical SRGB profiles to exercise multi-profile path; expect same as single SRGB
static void Test_GetXFormColorSpaces_DoubleSRGB()
{
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB == NULL) {
        fprintf(stderr, "SKIP: Unable to create sRGB profile for Test_GetXFormColorSpaces_DoubleSRGB\n");
        return;
    }

    // Use the same profile twice; array-like with duplicated profiles
    cmsHPROFILE profiles[2] = { hSRGB, hSRGB };
    cmsColorSpaceSignature inSig = 0;
    cmsColorSpaceSignature outSig = 0;

    cmsBool res = GetXFormColorSpaces(2, profiles, &inSig, &outSig);
    EXPECT_TRUE(res != 0, "GetXFormColorSpaces should succeed for two SRGB profiles");

    // As in the single-SRGB case, we expect Input = rgb and Output = XYZ
    EXPECT_EQ_U((unsigned long long)inSig, (unsigned long long)cmsSigRgbData, "Double SRGB: Input should be cmsSigRgbData");
    EXPECT_EQ_U((unsigned long long)outSig, (unsigned long long)cmsSigXYZData, "Double SRGB: Output should be cmsSigXYZData");

    SafeClose(hSRGB);
}

// Test 5: Second profile NULL in a multi-profile request should fail (FALSE)
static void Test_GetXFormColorSpaces_SecondNullProfile()
{
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB == NULL) {
        fprintf(stderr, "SKIP: Unable to create sRGB profile for Test_GetXFormColorSpaces_SecondNullProfile\n");
        return;
    }
    cmsHPROFILE profiles[2] = { hSRGB, NULL };
    cmsColorSpaceSignature inSig = 0;
    cmsColorSpaceSignature outSig = 0;

    cmsBool res = GetXFormColorSpaces(2, profiles, &inSig, &outSig);
    EXPECT_TRUE(res == 0, "Two-profile request with second NULL should return FALSE");

    SafeClose(hSRGB);
}

// Optional Test 6: Single Gray (if available) to exercise a different color space path
// Note: This test depends on cmsCreateGrayProfile() availability in the linked LittleCMS library.
// If unavailable, this test is skipped gracefully.
static void Test_GetXFormColorSpaces_SingleGray()
{
#ifdef __GNUC__
#pragma message("Note: Testing with Gray profile if available in the linked library.")
#endif
    cmsHPROFILE hGray = cmsCreateGrayProfile();
    if (hGray == NULL) {
        fprintf(stderr, "SKIP: Gray profile not available; skipping Test_GetXFormColorSpaces_SingleGray\n");
        return;
    }

    cmsHPROFILE profiles[1] = { hGray };
    cmsColorSpaceSignature inSig = 0;
    cmsColorSpaceSignature outSig = 0;

    cmsBool res = GetXFormColorSpaces(1, profiles, &inSig, &outSig);
    EXPECT_TRUE(res != 0, "Single Gray profile should produce a valid transform (if Gray is supported)");

    // For Gray, the input color space is typically cmsSigGreyData (Gray) and PCS is XYZ.
    EXPECT_EQ_U((unsigned long long)inSig, (unsigned long long)cmsSigGrayData, "Single Gray: Input should be cmsSigGrayData");
    EXPECT_EQ_U((unsigned long long)outSig, (unsigned long long)cmsSigXYZData, "Single Gray: Output should be cmsSigXYZData");

    SafeClose(hGray);
}

// Entry point: run all tests and report summary
int main()
{
    printf("Running GetXFormColorSpaces unit tests (no GTest; lightweight harness)\n");

    Test_GetXFormColorSpaces_ZeroProfiles();
    Test_GetXFormColorSpaces_PrimaryNullProfile();
    Test_GetXFormColorSpaces_SingleSRGB();
    Test_GetXFormColorSpaces_DoubleSRGB();
    Test_GetXFormColorSpaces_SecondNullProfile();
    Test_GetXFormColorSpaces_SingleGray();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED (%d tests).\n", /* approximate count */ 6);
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s).\n", g_failures);
        return 1;
    }
}