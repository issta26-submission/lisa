#include <cstring>
#include <mex.h>
#include <fstream>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <lcms2.h>
#include <string.h>


// Provide C linkage for the focal function if compiled with a C compiler in some setups
extern "C" cmsHPROFILE OpenProfile(const char* File);

// Simple lightweight assertion helpers (no GTest/GMock)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "TEST FAILED: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

#define ASSERT_NOT_NULL(ptr, msg) ASSERT_TRUE((ptr) != nullptr, msg)
#define ASSERT_NULL(ptr, msg) ASSERT_TRUE((ptr) == nullptr, msg)
#define ASSERT_EQ(a, b, msg) { \
    ++g_tests_run; \
    if (!((a) == (b))) { \
        std::cerr << "TEST FAILED: " << (msg) << " | Expected: " << (b) << " Got: " << (a) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
}
#define SAFE_CLOSE_PROFILE(p) do { if ((p) != nullptr) { cmsCloseProfile((p)); (p) = nullptr; } } while(0)

// Test helpers for OpenProfile
// Each test exercises a specific branch (true path) and ensures a non-null profile is returned.
// For the default branch, we attempt to open a non-existing file to trigger cmsOpenProfileFromFileTHR.

static void test_OpenProfile_Null_Returns_sRGB()
{
    // Branch: if (!File) return cmsCreate_sRGBProfileTHR(ContextID);
    cmsHPROFILE prof = OpenProfile(nullptr);
    ASSERT_NOT_NULL(prof, "OpenProfile(nullptr) should return a non-null sRGB profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_Lab2()
{
    // Branch: "*Lab2"
    cmsHPROFILE prof = OpenProfile("*Lab2");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*Lab2\") should return a non-null Lab2 profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_Lab4()
{
    // Branch: "*Lab4"
    cmsHPROFILE prof = OpenProfile("*Lab4");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*Lab4\") should return a non-null Lab4 profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_LabAlias()
{
    // Branch: "*Lab" (alias uses Lab4 path in implementation)
    cmsHPROFILE prof = OpenProfile("*Lab");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*Lab\") should return a non-null Lab profile (alias to Lab4)");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_LabD65()
{
    // Branch: "*LabD65"
    cmsHPROFILE prof = OpenProfile("*LabD65");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*LabD65\") should return a non-null Lab with D65 white point profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_XYZ()
{
    // Branch: "*XYZ"
    cmsHPROFILE prof = OpenProfile("*XYZ");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*XYZ\") should return a non-null XYZ profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_Gray22()
{
    // Branch: "*Gray22"
    cmsHPROFILE prof = OpenProfile("*Gray22");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*Gray22\") should return a non-null Gray profile with 2.2 gamma");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_Gray30()
{
    // Branch: "*Gray30"
    cmsHPROFILE prof = OpenProfile("*Gray30");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*Gray30\") should return a non-null Gray profile with 3.0 gamma");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_srgbAlias()
{
    // Branch: "*srgb"
    cmsHPROFILE prof = OpenProfile("*srgb");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*srgb\") should return a non-null sRGB profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_NullProfileAlias()
{
    // Branch: "*null"
    cmsHPROFILE prof = OpenProfile("*null");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*null\") should return a non-null NULL-profile (implementation specific)");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_Lin2222()
{
    // Branch: "*Lin2222"
    cmsHPROFILE prof = OpenProfile("*Lin2222");
    ASSERT_NOT_NULL(prof, "OpenProfile(\"*Lin2222\") should return a non-null Linearization Device Link profile");
    if (prof) cmsCloseProfile(prof);
}

static void test_OpenProfile_DefaultNonExistingFile()
{
    // Branch: default path - OpenProfileFromFileTHR
    const char* fname = "OpenProfile_Default_Test_File_DoNotExist.icc";

    // Ensure the file does not exist to avoid flakiness in environments with a pre-existing file
    std::ifstream infile(fname);
    bool exists = infile.good();
    infile.close();

    if (exists) {
        // If such a file happens to exist in this environment (uncommon), skip this test to avoid false negatives.
        std::cout << "SKIP: OpenProfile_DefaultNonExistingFile test skipped because file unexpectedly exists: " << fname << std::endl;
        return;
    }

    cmsHPROFILE prof = OpenProfile(fname);
    // Expect NULL since file does not exist (typical behavior of cmsOpenProfileFromFileTHR for missing files)
    ASSERT_NULL(prof, "OpenProfile with non-existing file should return NULL");
    // No need to cmsCloseProfile since prof is NULL
}

// Entry point for running all tests
int main()
{
    std::cout << "Starting OpenProfile test suite (no GTest, plain C++11 tests)" << std::endl;

    test_OpenProfile_Null_Returns_sRGB();
    test_OpenProfile_Lab2();
    test_OpenProfile_Lab4();
    test_OpenProfile_LabAlias();
    test_OpenProfile_LabD65();
    test_OpenProfile_XYZ();
    test_OpenProfile_Gray22();
    test_OpenProfile_Gray30();
    test_OpenProfile_srgbAlias();
    test_OpenProfile_NullProfileAlias();
    test_OpenProfile_Lin2222();
    test_OpenProfile_DefaultNonExistingFile();

    // Summary
    std::cout << "OpenProfile tests completed. Ran: " << g_tests_run << ", Passed: "
              << (g_tests_run - g_tests_failed) << ", Failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}