/*
Step 1 - Candidate Keywords extracted from the focal method (CheckVCGT):
- Pass (cmsInt32Number)
- hProfile (cmsHPROFILE)
- cmsToneCurve*, Curves[3]
- cmsToneCurve** PtrCurve
- cmsBuildGamma, DbgThread
- cmsWriteTag, cmsSigVcgtTag
- cmsFreeToneCurveTriple
- cmsReadTag
- cmsEstimateGamma
- IsGoodVal
- PtrCurve[0..2] (R, G, B)
- Return values 0/1 indicating success/failure
- Memory management and tag round-trip behavior

The tests below are designed to exercise both branches of the CheckVCGT function:
- Case 1: Writing the VCGT tag with three gamma curves (R=1.1, G=2.2, B=3.4)
- Case 2: Reading and validating the stored gamma curves
- Case 3: Default path (Fall-through) returns 0
- Case 4: Reading without a VCGT tag should return 0

Domain notes implemented:
- Tests use provided helper functions (Create_AboveRGB, DbgThread, etc.) to obtain a valid cmsHPROFILE
- Tests are non-terminating (they do not exit on first failure; they accumulate failures)
- Only public APIs are used; static/internal helpers are not accessed directly
- The tests are orchestrated from main() since gtest is not allowed
- Minimal use of standard library; C linkage for C APIs

Compile-time assumption:
- The Little CMS headers and the test helper header testcms2.h (as included by the project) are available.
- The following C functions are available with C linkage: CheckVCGT, Create_AboveRGB, cmsCloseProfile
- The code is compiled as C++11, linking against the C library implementations.

Note:
- The tests use simple non-terminating assertions (EXPECT_TRUE, EXPECT_EQ) to maximize code coverage without aborting on first failure.
- Extern "C" wrappers are provided for C declarations to ensure correct linkage when compiling as C++.
*/

// Include standard I/O for test reporting
#include <lcms2.h>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Include Little CMS headers for type definitions

// Include the test helper header (as in the original project)
extern "C" {
}

// Extern declarations for the focal function and test helpers used in tests.
// We assume these are provided by the project environment.
extern "C" cmsInt32Number CheckVCGT(cmsInt32Number Pass, cmsHPROFILE hProfile);
extern "C" cmsHPROFILE Create_AboveRGB(void);
extern "C" void cmsCloseProfile(cmsHPROFILE h);

// Simple non-terminating test harness (no GTest). Counts failures but continues.
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s\n", (msg)); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(((a) != (b))) { \
        fprintf(stderr, "EXPECT_EQ failed: %s. Got %ld, Expected %ld\n", (msg), (long)(a), (long)(b)); \
        ++g_failures; \
    } \
} while(0)

// Domain knowledge and coverage-focused tests for CheckVCGT

// Test 1: Case Pass = 1 should write VCGT tag with three gamma curves and return 1
void test_CheckVCGT_case1(void)
{
    // Create a valid RGB profile to attach the VCGT tag
    cmsHPROFILE hProfile = Create_AboveRGB();
    EXPECT_TRUE(hProfile != nullptr, "Create_AboveRGB() returned NULL; cannot proceed with Case 1");

    if (hProfile == nullptr) {
        return; // skip further checks if profile creation failed
    }

    int res = (int)CheckVCGT(1, hProfile);
    EXPECT_EQ(res, 1, "CheckVCGT with Pass=1 should return 1 after writing VCGT tag");

    // Clean up
    cmsCloseProfile(hProfile);
}

// Test 2: Case Pass = 2 should read back the tag written in Case 1 and validate gamma values
void test_CheckVCGT_case2(void)
{
    // Create a profile and write the VCGT tag first
    cmsHPROFILE hProfile = Create_AboveRGB();
    EXPECT_TRUE(hProfile != nullptr, "Create_AboveRGB() returned NULL; cannot proceed with Case 2");
    if (hProfile == nullptr) return;

    int writeRes = (int)CheckVCGT(1, hProfile);
    EXPECT_EQ(writeRes, 1, "Precondition: CheckVCGT(1) should succeed to set up VCGT tag for Case 2");

    // Now read back and validate values
    int readRes = (int)CheckVCGT(2, hProfile);
    EXPECT_EQ(readRes, 1, "CheckVCGT with Pass=2 should read and validate VCGT gamma values");

    // Clean up
    cmsCloseProfile(hProfile);
}

// Test 3: Default path (Pass value not matched by switch) should return 0
void test_CheckVCGT_case3(void)
{
    cmsHPROFILE hProfile = Create_AboveRGB();
    EXPECT_TRUE(hProfile != nullptr, "Create_AboveRGB() failed for Case 3");
    if (hProfile == nullptr) return;

    int res = (int)CheckVCGT(0, hProfile); // Pass values other than 1 or 2
    EXPECT_EQ(res, 0, "CheckVCGT with Pass=0 should return 0 (default case)");

    cmsCloseProfile(hProfile);
}

// Test 4: Pass = 2 on a profile without a VCGT tag should fail (return 0)
void test_CheckVCGT_case4(void)
{
    cmsHPROFILE hProfile = Create_AboveRGB();
    EXPECT_TRUE(hProfile != nullptr, "Create_AboveRGB() failed for Case 4");
    if (hProfile == nullptr) return;

    // Ensure there is no VCGT tag initially
    int res = (int)CheckVCGT(2, hProfile);
    // Depending on implementation, this could be 0 if no tag is present
    EXPECT_EQ(res, 0, "CheckVCGT(2) on profile without VCGT tag should return 0");

    cmsCloseProfile(hProfile);
}

// Entrypoint for running tests
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    fprintf(stdout, "Starting CheckVCGT unit tests (C++11 harness, no GTest)\n");

    test_CheckVCGT_case1();
    test_CheckVCGT_case2();
    test_CheckVCGT_case3();
    test_CheckVCGT_case4();

    if (g_failures) {
        fprintf(stderr, "CheckVCGT tests completed with %d failure(s).\n", g_failures);
        return 1;
    } else {
        printf("CheckVCGT tests completed successfully with all expectations met.\n");
        return 0;
    }
}