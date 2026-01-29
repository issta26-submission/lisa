/*
 * Unit test suite for OpenStockProfile (from vprf.c)
 * - C++11, no GTest, lightweight test harness with non-terminating EXPECT_ assertions
 * - Focuses on coverage of OpenStockProfile branches
 * - Uses LCMS2 types and functions; assumes lcms2 library is available in the build environment
 *
 * Important notes:
 * - We purposely cover all explicit string-driven branches (e.g., "*Lab2", "*Lab4", "*Lab", "*LabD65",
 *   "*XYZ", "*Gray22", "*Gray30", "*srgb", "*null", "*oklab", "*Lin2222") and the null File path (first if).
 * - The final default branch (cmsOpenProfileFromFileTHR) is not tested with a real file to avoid IO dependency
 *   and platform-specific ICC file requirements. If needed, a separate mock can be added.
 * - The test harness implements its own minimal assertion facility that does not terminate on failure
 *   to maximize code coverage.
 *
 * Build hints (example, adapt to your environment):
 *   g++ -std=c++11 -I<path-to-lcms2/include> test_openstockprofile.cpp -lcms2 -o test_openstockprofile
 *
 * Dependency notes:
 *   - Include path for lcms2 headers should be provided via -I flag
 *   - Link against liblcms2 (e.g., -llcms2)
 */

// Include standard library
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <lcms2.h>
#include <utils.h>


// Forward declaration of the focal function from vprf.c (C linkage)
extern "C" cmsHPROFILE OpenStockProfile(cmsContext ContextID, const char* File);

// Local test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Simple non-terminating EXPECT macros
#define EXPECT_TRUE(cond) do {                                                  \
    ++g_total;                                                                   \
    if(!(cond)) {                                                                \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__     \
                  << " - condition: " #cond << std::endl;                       \
        ++g_failed;                                                              \
    }                                                                            \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { EXPECT_TRUE((ptr) != nullptr); } while(0)
#define EXPECT_NULL(ptr)     do { EXPECT_TRUE((ptr) == nullptr); } while(0)

// Convenience wrapper to release an opened profile if non-null
static void SafeCloseProfile(cmsHPROFILE h)
{
    if (h) cmsCloseProfile(h);
}

// Candidate Keywords (step 1: key branches in the focal method)
// - File == nullptr branch (default sRGB)
// - "*Lab2", "*Lab4", "*Lab", "*LabD65" (Lab family with D65 white point)
// - "*XYZ" (XYZ profile)
// - "*Gray22", "*Gray30" (gray gamma profiles with 2.2 and 3.0 gamma)
// - "*srgb" (sRGB profile)
// - "*null" (NULL profile)
// - "*oklab" (OkLab profile)
// - "*Lin2222" (4-channel linearization device link)
// - default path: cmsOpenProfileFromFileTHR (not exercised with a real file here)

// Test: null File should return a non-null sRGB profile
static void test_null_file() {
    cmsHPROFILE h = OpenStockProfile(0, nullptr);
    EXPECT_NOT_NULL(h);       // True branch: !File
    if (h) SafeCloseProfile(h);
}

// Test: Lab2 profile path
static void test_lab2_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*Lab2");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: Lab4 profile path
static void test_lab4_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*Lab4");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: Lab profile path (alias for Lab4 in implementation)
static void test_lab_alias_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*Lab");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: Lab D65 path
static void test_lab_d65_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*LabD65");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: XYZ profile path
static void test_xyz_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*XYZ");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: Gray22 gamma path
static void test_gray22_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*Gray22");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: Gray30 gamma path
static void test_gray30_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*Gray30");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: srgb path
static void test_srgb_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*srgb");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: NULL profile path
static void test_null_profile_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*null");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: OkLab path
static void test_oklab_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*oklab");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Test: Lin2222 path (4-channel linearization)
static void test_lin2222_path() {
    cmsHPROFILE h = OpenStockProfile(0, "*Lin2222");
    EXPECT_NOT_NULL(h);
    if (h) SafeCloseProfile(h);
}

// Run all tests and report summary
int main() {
    std::cout << "Running OpenStockProfile unit tests (C++11, no GTest)" << std::endl;

    test_null_file();
    test_lab2_path();
    test_lab4_path();
    test_lab_alias_path();
    test_lab_d65_path();
    test_xyz_path();
    test_gray22_path();
    test_gray30_path();
    test_srgb_path();
    test_null_profile_path();
    test_oklab_path();
    test_lin2222_path();

    std::cout << "Tests completed. Ran: " << g_total << ", Failed: " << g_failed << std::endl;

    return (g_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}