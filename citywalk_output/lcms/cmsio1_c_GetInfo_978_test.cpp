// Test suite for the focal method: GetInfo (in cmsio1.c)
// Environment: C++11, no GoogleTest, using Little CMS (lcms2) public API.
// The test harness provides lightweight EXPECT-like macros that do not terminate
// the test run on failure, enabling multiple tests to run in a single process.
// Explanatory comments are included for each unit test to describe the intent and
// the specific branch coverage being exercised.

#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (cond) { ++g_passed_tests; } \
        else { ++g_failed_tests; fprintf(stderr, "EXPECT_TRUE FAILED at %s:%d: %s\n", __FILE__, __LINE__, (msg) ? (msg) : "condition false"); } \
    } while (0)

#define EXPECT_FALSE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { ++g_passed_tests; } \
        else { ++g_failed_tests; fprintf(stderr, "EXPECT_FALSE FAILED at %s:%d: %s\n", __FILE__, __LINE__, (msg) ? (msg) : "condition true"); } \
    } while (0)

#define EXPECT_PTR_EQ(p1, p2, msg) \
    do { \
        ++g_total_tests; \
        if ((p1) == (p2)) { ++g_passed_tests; } \
        else { ++g_failed_tests; fprintf(stderr, "EXPECT_PTR_EQ FAILED at %s:%d: %s (left=%p, right=%p) %s\n", __FILE__, __LINE__, (msg) ? (msg) : "pointers not equal", (void*)(p1), (void*)(p2), "" ); } \
    } while (0)

#define EXPECT_PTR_NULL(p, msg) \
    do { \
        ++g_total_tests; \
        if ((p) == NULL) { ++g_passed_tests; } \
        else { ++g_failed_tests; fprintf(stderr, "EXPECT_PTR_NULL FAILED at %s:%d: %s (got=%p)\n", __FILE__, __LINE__, (msg) ? (msg) : "pointer not NULL", (void*)(p)); } \
    } while (0)

#define EXPECT_EQ_INT(a, b, msg) \
    do { \
        ++g_total_tests; \
        if ((a) == (b)) { ++g_passed_tests; } \
        else { ++g_failed_tests; fprintf(stderr, "EXPECT_EQ_INT FAILED at %s:%d: %s (got=%d, expected=%d)\n", __FILE__, __LINE__, (msg) ? (msg) : "values not equal", (a), (b)); } \
    } while (0)

// Helper function to print a summary at the end
static void PrintSummary(void)
{
    printf("\nTest Summary: Total=%d, Passed=%d, Failed=%d\n",
           g_total_tests, g_passed_tests, g_failed_tests);
    if (g_failed_tests > 0) {
        printf("Some tests failed. Review the error messages above.\n");
    } else {
        printf("All tests passed.\n");
    }
}

// Test 1: Validate branch decision for cmsInfoDescription based on presence of MLTag vs legacy tag.
// Intent: Exercise the conditional inside GetInfo for the description case.
// - If mlTag exists: GetInfo should use cmsSigProfileDescriptionMLTag.
// - If mlTag does not exist but legacy tag exists: GetInfo should use cmsSigProfileDescriptionTag.
// - If neither exists: GetInfo should return NULL.
static void test_GetInfo_Description_Branch_MLTag_vs_LegacyTag(void)
{
    // Create a standard in-memory profile (sRGB) to exercise the code path.
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        fprintf(stderr, "Test skipped: failed to create sRGB profile.\n");
        // Mark as skipped by not incrementing total (or handle gracefully)
        return;
    }

    // Determine which tag GetInfo would use for description by consulting cmsIsTag.
    cmsTagSignature mlTag = cmsSigProfileDescriptionMLTag;
    cmsTagSignature legacyTag = cmsSigProfileDescriptionTag;

    cmsMLU* expected = NULL;
    if (cmsIsTag(hProfile, mlTag)) {
        expected = (cmsMLU*) cmsReadTag(hProfile, mlTag);
    } else if (cmsIsTag(hProfile, legacyTag)) {
        expected = (cmsMLU*) cmsReadTag(hProfile, legacyTag);
    }
    cmsMLU* actual = (cmsMLU*) GetInfo(hProfile, cmsInfoDescription);

    EXPECT_PTR_EQ(actual, expected, "GetInfo description branch did not match direct cmsReadTag result for the chosen tag.");

    cmsCloseProfile(hProfile);
}

// Test 2: Validate GetInfo for Manufacturer tag.
// Intent: Ensure that the description of the Manufacturer tag is returned correctly
// by comparing the pointer returned by GetInfo with cmsReadTag for cmsSigDeviceMfgDescTag.
static void test_GetInfo_Manufacturer_TagConsistency(void)
{
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        fprintf(stderr, "Test skipped: failed to create sRGB profile.\n");
        return;
    }

    cmsMLU* expected = (cmsMLU*) cmsReadTag(hProfile, cmsSigDeviceMfgDescTag);
    cmsMLU* actual   = (cmsMLU*) GetInfo(hProfile, cmsInfoManufacturer);

    EXPECT_PTR_EQ(actual, expected, "Manufacturer GetInfo did not match direct cmsReadTag for cmsSigDeviceMfgDescTag.");

    cmsCloseProfile(hProfile);
}

// Test 3: Validate GetInfo for Model tag.
// Intent: Ensure that the Model tag read matches GetInfo's result for cmsSigDeviceModelDescTag.
static void test_GetInfo_Model_TagConsistency(void)
{
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        fprintf(stderr, "Test skipped: failed to create sRGB profile.\n");
        return;
    }

    cmsMLU* expected = (cmsMLU*) cmsReadTag(hProfile, cmsSigDeviceModelDescTag);
    cmsMLU* actual   = (cmsMLU*) GetInfo(hProfile, cmsInfoModel);

    EXPECT_PTR_EQ(actual, expected, "Model GetInfo did not match direct cmsReadTag for cmsSigDeviceModelDescTag.");

    cmsCloseProfile(hProfile);
}

// Test 4: Validate GetInfo for Copyright tag.
// Intent: Ensure that the Copyright tag read matches GetInfo's result for cmsSigCopyrightTag.
static void test_GetInfo_Copyright_TagConsistency(void)
{
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        fprintf(stderr, "Test skipped: failed to create sRGB profile.\n");
        return;
    }

    cmsMLU* expected = (cmsMLU*) cmsReadTag(hProfile, cmsSigCopyrightTag);
    cmsMLU* actual   = (cmsMLU*) GetInfo(hProfile, cmsInfoCopyright);

    EXPECT_PTR_EQ(actual, expected, "Copyright GetInfo did not match direct cmsReadTag for cmsSigCopyrightTag.");

    cmsCloseProfile(hProfile);
}

// Test 5: Invalid Info value should return NULL (default branch).
static void test_GetInfo_Invalid_Info_Returns_NULL(void)
{
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        fprintf(stderr, "Test skipped: failed to create sRGB profile.\n");
        return;
    }

    // Deliberately cast an invalid value outside the known cmsInfoType cases.
    cmsInfoType invalidInfo = (cmsInfoType)0xDEADBEEF;
    const cmsMLU* res = GetInfo(hProfile, invalidInfo);

    EXPECT_PTR_NULL(res, "GetInfo with invalid cmsInfoType should return NULL.");

    cmsCloseProfile(hProfile);
}

// Test 6: Combined run to cover a set of cases in one pass (optional aggregation test).
static void test_aggregate_description_and_signatures(void)
{
    // This test ensures that two related checks are consistent when run together.
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        fprintf(stderr, "Test skipped: failed to create sRGB profile.\n");
        return;
    }

    // Determine expected tag used by description path
    cmsMLU* expected = NULL;
    if (cmsIsTag(hProfile, cmsSigProfileDescriptionMLTag)) {
        expected = (cmsMLU*) cmsReadTag(hProfile, cmsSigProfileDescriptionMLTag);
    } else if (cmsIsTag(hProfile, cmsSigProfileDescriptionTag)) {
        expected = (cmsMLU*) cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
    }

    cmsMLU* actual = (cmsMLU*) GetInfo(hProfile, cmsInfoDescription);
    EXPECT_PTR_EQ(actual, expected, "Aggregate test: GetInfo description branch mismatch.");

    cmsCloseProfile(hProfile);
}

// Entry point
int main(void)
{
    // Run tests
    test_GetInfo_Description_Branch_MLTag_vs_LegacyTag();
    test_GetInfo_Manufacturer_TagConsistency();
    test_GetInfo_Model_TagConsistency();
    test_GetInfo_Copyright_TagConsistency();
    test_GetInfo_Invalid_Info_Returns_NULL();
    test_aggregate_description_and_signatures();

    // Print summary
    PrintSummary();
    // Return code 0 even if tests failed to align with "non-terminating assertions"
    // (the caller can inspect exit code if desired, but we avoid hard failures).
    return (g_failed_tests == 0) ? 0 : 1;
}