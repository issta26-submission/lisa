/*
Step 1 - Program Understanding (Candidate Keywords)
- TYPE_LABA_F32
- cmsCreate_OkLabProfile
- cmsCreate_sRGBProfile
- cmsCreateTransform
- cmsDoTransform
- cmsCloseProfile
- cmsDeleteTransform
- cmsUInt16Number rgb[3]
- cmsFloat32Number lab[4]
- cmsHTRANSFORM hBack, hForth
- OkLab round-trip transformation
- Dependency: testcms2.h (test harness dependencies)
This test file provides a small suite to validate the focal function Check_OkLab2(void).
The function is part of a larger test suite (testcms2.c) and relies on LittleCMS-like APIs.
*/

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Import the test harness and dependencies (best effort for C/C++ interop)

#ifdef __cplusplus
extern "C" {
#endif
// Prototype for the focal function under test.
// If testcms2.h already declares Check_OkLab2, this is redundant but safe for C linkage.
int Check_OkLab2(void);
#ifdef __cplusplus
}
#endif

// Simple lightweight test harness (non-terminating assertions)
static int g_tests = 0;
static int g_failed = 0;

static void report_failure(const char* test_name, const char* reason) {
    std::cerr << "TEST FAILED: " << test_name << " - " << reason << std::endl;
    ++g_failed;
}

int main() {
    // Step 2: Unit Test Generation for Check_OkLab2
    // Test 1: Basic return type sanity for the focal function
    // This ensures the function returns either 0 or 1 (as designed by the implementation)
    // even if the internal round-trip behavior may vary slightly across environments.
    {
        const char* test_name = "Check_OkLab2_basic_return_type";
        ++g_tests;
        int ret = Check_OkLab2();
        if (!(ret == 0 || ret == 1)) {
            report_failure(test_name, "Expected return value to be 0 or 1.");
        } else {
            // Passed: the function returns a boolean-like value (0 or 1)
        }
    }

    // Test 2: Expectation of a successful OkLab round-trip (return value == 1)
    // This test asserts that under normal circumstances the round-trip through OkLab is preserved.
    // Note: Depending on the library configuration and environment, this may be 1 or 0.
    // We document the expectation and fail gracefully if not met.
    {
        const char* test_name = "Check_OkLab2_expected_1";
        ++g_tests;
        int ret = Check_OkLab2();
        if (ret != 1) {
            report_failure(test_name, "Expected OkLab round-trip to succeed (return 1).");
        }
    }

    // Test 3: Idempotence / Determinism of consecutive calls
    // Call the function twice and ensure the results are consistent between calls.
    {
        const char* test_name = "Check_OkLab2_idempotence";
        ++g_tests;
        int first = Check_OkLab2();
        int second = Check_OkLab2();
        if (first != second) {
            report_failure(test_name, "Consecutive calls produced different results (not idempotent).");
        }
    }

    // Summary
    std::cout << "Tests run: " << g_tests << ", Failures: " << g_failed << std::endl;
    return g_failed ? 1 : 0;
}