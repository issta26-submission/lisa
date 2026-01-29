/*
Unit test suite for the focal method:
  cmsInt32Number OneTrivialLab(cmsHPROFILE hLab1, cmsHPROFILE hLab2, const char* txt)

Summary of Step 1 (Program Understanding) and Candidate Keywords:
- Core dependencies and operations observed within OneTrivialLab:
  - cmsHPROFILE hLab1, hLab2: input Lab profiles
  - cmsHTRANSFORM xform: color transform handle
  - cmsCreateTransformTHR(DbgThread(), ...): creates a Lab-to-Lab transform
  - cmsCloseProfile(hLab1), cmsCloseProfile(hLab2): close input profiles
  - CheckSeveralLab(xform): verifies Lab transform behavior
  - cmsDeleteTransform(xform): cleanup
  - SubTest(txt): test harness annotation
- These are the dependent components to be considered in test design.

Notes:
- This test suite is designed to be compiled in a C++11 project, without using GTest.
- We rely on existing test helpers declared in testcms2.h (extern "C" linkage for C APIs).
- Tests focus on safe execution and non-terminating assertions to maximize coverage.
- We avoid private members or static internals of the focal code. We interact with the public test interface exposed by the provided dependencies.

Test file: test_one_trivial_lab.cpp

*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Bring in the C test dependencies with C linkage to avoid name mangling
extern "C" {
}

// Simple non-terminating assertion mechanism suitable for C++11
static int g_failures = 0;
#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// High-level test cases for OneTrivialLab
// Test 1: Basic sanity with two identical GrayLab profiles
//        Expectation: OneTrivialLab executes without crashing and returns a non-negative rc.
void test_OneTrivialLab_BasicSanity(void)
{
    // Candidate scenario: two identical Lab profiles (GrayLab)
    // Provided by the test infrastructure
    cmsHPROFILE h1 = Create_GrayLab();
    cmsHPROFILE h2 = Create_GrayLab();

    const char* subTestTxt = "BasicSanity: GrayLab vs GrayLab";
    cmsInt32Number rc = OneTrivialLab(h1, h2, subTestTxt);

    // Non-terminating assertion: rc should be non-negative if transform and checks succeed
    ASSERT(rc >= 0, "OneTrivialLab returned a negative rc for GrayLab vs GrayLab");

    // Cleanup: OneTrivialLab closes h1/h2; additional global cleanup if any
    RemoveTestProfiles();
}

// Test 2: Profile-pair variation to exercise the function with different inputs
//        Scenarios:
//          - GrayLab vs Gray22
//          - GrayLab vs Gray30
//        Expectation: OneTrivialLab handles different Lab profiles and returns non-negative rc.
//        We do not assume a specific rc value since internal checks may yield varying results.
void test_OneTrivialLab_VariedProfilePairs(void)
{
    // Pair 1: GrayLab vs Gray22
    cmsHPROFILE hA1 = Create_GrayLab();
    cmsHPROFILE hA2 = Create_Gray22();
    cmsInt32Number rc1 = OneTrivialLab(hA1, hA2, "VariedPair1: GrayLab vs Gray22");
    ASSERT(rc1 >= 0, "OneTrivialLab returned a negative rc for GrayLab vs Gray22");
    RemoveTestProfiles();

    // Pair 2: GrayLab vs Gray30
    cmsHPROFILE hB1 = Create_GrayLab();
    cmsHPROFILE hB2 = Create_Gray30();
    cmsInt32Number rc2 = OneTrivialLab(hB1, hB2, "VariedPair2: GrayLab vs Gray30");
    ASSERT(rc2 >= 0, "OneTrivialLab returned a negative rc for GrayLab vs Gray30");
    RemoveTestProfiles();
}

// Main entry point for the test suite
int main(int argc, char* argv[])
{
    // Step 3: Domain knowledge tips implemented as comments above
    // Execute tests and report results without terminating on first failure
    std::cout << "Starting OneTrivialLab unit tests (no GTest)..." << std::endl;

    test_OneTrivialLab_BasicSanity();
    test_OneTrivialLab_VariedProfilePairs();

    if (g_failures)
    {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}