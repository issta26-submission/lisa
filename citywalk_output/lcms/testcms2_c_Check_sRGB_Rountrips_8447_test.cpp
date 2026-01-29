/*
Unit Test Suite for Check_sRGB_Rountrips (C function in testcms2.c)

Step 1 (Context and Candidate Keywords)
- Core dependencies and concepts inferred from the focal method:
  Candidate Keywords: cmsUInt16Number, rgb, seed, Lab, cmsCIELab, cmsHPROFILE, hsRGB, hLab, cmsHTRANSFORM, hBack, hForth, cmsCloseProfile, cmsDoTransform, cmsDeleteTransform, cmsCreateTransform, cmsCreate_sRGBProfile, cmsCreateLab4Profile, distance, maxErr, err, printf, INTENT_RELATIVE_COLORIMETRIC, TYPE_Lab_DBL, TYPE_RGB_16
- Function under test: Check_sRGB_Rountrips
- Behavior: Performs forward and backward sRGB-Lab color transformations, tracks maximum roundtrip error, returns 1 if max error <= 20.0, else prints a message and returns 0.

Step 2 (Test Focus)
- We test the focal function Check_sRGB_Rountrips(void) by invoking it from C++ test code (linking against the C file containing the function).
- Since the function deterministically computes a roundtrip error over a fixed grid, we validate that the function returns 1 (success) under typical environment assumptions.
- We cannot reliably force the failure path (maxErr > 20.0) without altering the production code; thus we provide tests that exercise the successful path and confirm consistent results across multiple invocations.

Step 3 (Test Case Refinement)
- Use a lightweight, non-terminating assertion approach (no aborts) to maximize code coverage.
- Implement a minimal C++ test harness (no GTest/GMock) with:
  - extern "C" declaration to call the C function Check_sRGB_Rountrips
  - EXPECT-like macros (non-terminating) to record failures while continuing execution
  - A small suite that runs:
    - Test_Check_sRGB_Rountrips_ReturnsOne
    - Test_Check_sRGB_Rountrips_Idempotence (two successive calls)
  - A simple summary printed to stdout
- Ensure tests are invoked from main, per domain guidance.

Note: The test targets the public C API provided by the focal code; static/internal helpers in testcms2.c are not directly accessible.

Code (C++11) - test_srgb_roundtrip.cpp
- Exposes two tests for the Check_sRGB_Rountrips function and reports results without terminating on first failure.
- Requires linking with the object containing testcms2.c (and CMS library as needed).

Compile and run (example, not included in output):
- g++ -std=c++11 -O2 -o test_srgb_roundtrip test_srgb_roundtrip.cpp testcms2.o -lcms
- ./test_srgb_roundtrip

Code begins here:

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Step 2/3: Declare the C function under test
extern "C" int Check_sRGB_Rountrips(void);

// Lightweight non-terminating test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Non-terminating expectation macros
#define EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ " - Expected true: " #cond << "\n"; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_tests_run; \
    if( (a) != (b) ) { \
        ++g_tests_failed; \
        std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ " - Expected " #a " == " #b \
                  << " (actual: " << (a) << ", " << #b << ": " << (b) << ")\n"; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

// Test 1: Check that the function reports success (returns 1) on typical environment
// This exercises the true-branch of the function's conditional.
int Test_Check_sRGB_Rountrips_ReturnsOne() {
    int result = Check_sRGB_Rountrips();
    // The function is expected to return 1 on a healthy environment.
    EXPECT_EQ(result, 1);
    return (g_tests_failed == 0);
}

// Test 2: Call the function twice to ensure consistency (idempotence in this context)
int Test_Check_sRGB_Rountrips_Idempotence() {
    int r1 = Check_sRGB_Rountrips();
    int r2 = Check_sRGB_Rountrips();
    // Both invocations should yield the same expected result (1) in a stable environment.
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(r2, 1);
    // If both are 1, test passes
    return (g_tests_failed == 0);
}

int main(int argc, char* argv[]) {
    // Run tests with explanatory comments
    // Test 1: Basic validity of the foccused transformation round-trip
    bool ok1 = Test_Check_sRGB_Rountrips_ReturnsOne();
    // Test 2: Idempotence and stability across multiple invocations
    bool ok2 = Test_Check_sRGB_Rountrips_Idempotence();

    // Summary
    std::cout << "Test Summary: "
              << g_tests_run << " tests run, "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}