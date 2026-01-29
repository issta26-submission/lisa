// C++11 test suite for the focal method CheckCRDinfo in testcms2.c
// This test suite uses the Little CMS (lcms2) API to exercise
// writing and reading an MLU tag via CheckCRDinfo.
// No GTest is used; a lightweight, non-terminating assertion approach is implemented.

#include <lcms2.h>
#include <iostream>
#include <string>
#include <testcms2.h>


// Simple non-terminating assertion helper
static int g_failures = 0;
#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST_FAIL] " << (msg) << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[TEST_PASS] " << (msg) << std::endl; \
    } \
} while (0)

// Helper to run a single test and print its name
#define RUN_TEST(name) void name(); int main_runner_##name = 0; /* ensure linkage */ void name()

// Externals: the focal function under test is defined in the production code.
// We assume its signature is visible via the include path to lcms2.h
// Using the signature exactly as provided in the problem statement.
extern "C" {
    cmsInt32Number CheckCRDinfo(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);
}

// Test 1: Ensure Pass = 1 writes the MLU tag and returns a non-zero code.
// This validates the "case 1" branch of CheckCRDinfo.
RUN_TEST(Test_CheckCRDinfo_Pass1_WritesMLUReturnsNonZero)
{
    // Arrange
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsTagSignature tag = cmsSigMLU; // Use the standard MLU tag type ("mluc")
    
    // Act
    cmsInt32Number rc = CheckCRDinfo(1, hProfile, tag);
    
    // Assert: rc should be non-zero (success)
    ASSERT(rc != 0, "CheckCRDinfo Pass=1 should return non-zero on successful write");
    
    // Cleanup
    cmsCloseProfile(hProfile);
}

// Test 2: Ensure Pass = 2 reads back the previously written MLU tag and returns 1 (success).
// This validates the "case 2" branch when data matches exactly.
RUN_TEST(Test_CheckCRDinfo_Pass2_ReadBackReturnsOne)
{
    // Arrange: First write the MLU data
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsTagSignature tag = cmsSigMLU;
    cmsInt32Number rc1 = CheckCRDinfo(1, hProfile, tag);
    // Act: Read back the data
    cmsInt32Number rc2 = CheckCRDinfo(2, hProfile, tag);
    
    // Assert: Pass=1 should succeed, and Pass=2 should read back and validate data
    ASSERT(rc1 != 0, "CheckCRDinfo Pass=1 should succeed before ReadBack");
    ASSERT(rc2 == 1, "CheckCRDinfo Pass=2 should return 1 after successful write/read");
    
    // Cleanup
    cmsCloseProfile(hProfile);
}

// Test 3: Ensure Pass = 2 on a profile without the MLU tag returns 0 (negative path).
RUN_TEST(Test_CheckCRDinfo_Pass2_NoTagReturnsZero)
{
    // Arrange: Create a profile without writing the MLU tag
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsTagSignature tag = cmsSigMLU;
    
    // Act
    cmsInt32Number rc = CheckCRDinfo(2, hProfile, tag);
    
    // Assert: Should return 0 due to missing tag
    ASSERT(rc == 0, "CheckCRDinfo Pass=2 on missing MLU should return 0");
    
    // Cleanup
    cmsCloseProfile(hProfile);
}

// Test 4: Default/unexpected Pass value should return 0 (ensures default branch).
RUN_TEST(Test_CheckCRDinfo_DefaultCaseReturnsZero)
{
    // Arrange
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsTagSignature tag = cmsSigMLU;
    
    // Act
    cmsInt32Number rc = CheckCRDinfo(0, hProfile, tag);
    
    // Assert: Default case fallback should return 0
    ASSERT(rc == 0, "CheckCRDinfo default case (Pass=0) should return 0");
    
    // Cleanup
    cmsCloseProfile(hProfile);
}

// A small helper to run all tests in sequence and report a final summary
int main(void)
{
    // Run each test in sequence
    Test_CheckCRDinfo_Pass1_WritesMLUReturnsNonZero();
    Test_CheckCRDinfo_Pass2_ReadBackReturnsOne();
    Test_CheckCRDinfo_Pass2_NoTagReturnsZero();
    Test_CheckCRDinfo_DefaultCaseReturnsZero();

    if (g_failures == 0) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] " << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}

// Notes:
// - The tests rely on the Little CMS API (lcms2). Ensure the project links against liblcms2.
// - We use the standard MLU tag type (cmsSigMLU / "mluc") for writing and reading.
// - The test suite follows a minimal "unit test" style without GTest, using non-terminating assertions
//   to maximize code coverage through all test paths.
// - Static or global state from the focal method is not rely-oned in these tests; tests interact with
//   the API via the public interface (cmsCreate_sRGBProfile, cmsWriteTag, cmsReadTag, etc.).