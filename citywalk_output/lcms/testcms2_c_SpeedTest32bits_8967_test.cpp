// Test suite for SpeedTest32bits in testcms2.c
// Approach:
// - Use the automated harness provided by the project (testcms2.h) to create and
//   manage test profiles (Create_AboveRGB, Create_Gray22, etc.).
// - Exercise SpeedTest32bits with multiple valid profile pairs to ensure normal
//   operation and memory handling without crashes.
// - Provide lightweight test harness (no GTest) with simple boolean-based tests
//   and explanatory comments for each unit test.
// - Follow the domain guidance: only standard library and provided methods, no
//   private access required, and run tests from main().

#include <iostream>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Import C interfaces from the project (testcms2.h is part of the focal class dependencies)
// The header exposes helper functions like Create_AboveRGB, Create_Gray22, Create_GrayLab,
// RemoveTestProfiles, and SpeedTest32bits (the function under test).
extern "C" {
} // extern "C"

// Lightweight test harness (non-terminating assertions)
static int gTestsRun = 0;
static int gTestsFailed = 0;

#define TEST_PASSED()  do { gTestsRun++; } while(0)
#define TEST_FAILED()  do { gTestsRun++; gTestsFailed++; } while(0)

// Helper to print a clear test result
static void print_result(const char* testName, bool passed)
{
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Domain note:
// Step 3 (Test Case Refinement) emphasizes high coverage and executable tests.
// SpeedTest32bits contains a guard for NULL profiles. However, to avoid unsafe
// behavior in this test harness (the Die() function in the dependency is a
// no-op in the provided stubs), we only exercise the normal, well-defined path
// by supplying valid profiles created through the project's helpers.
// This set of tests focuses on validating correct integration with the LittleCMS
// transform pipeline (cmsCreateTransformTHR, cmsDoTransform, cmsDeleteTransform)
// and ensures proper resource management (profiles closed, memory released).

// Test 1: Basic path - AboveRGB -> Gray22 with default Intent (0)
static bool test_SpeedTest32bits_Basic_AboveRGB_to_Gray22() {
    // Create input and output color profiles using project-provided helpers
    cmsHPROFILE hlcmsProfileIn  = Create_AboveRGB();
    cmsHPROFILE hlcmsProfileOut = Create_Gray22();

    if (hlcmsProfileIn == NULL || hlcmsProfileOut == NULL) {
        // If profile creation failed, mark test as failed
        print_result("SpeedTest32bits_Basic_AboveRGB_to_Gray22", false);
        TEST_FAILED();
        return false;
    }

    // Run the focal method with a reasonable, valid intent value (0 as a typical default)
    SpeedTest32bits("SpeedTest32bits_Basic_AboveRGB_to_Gray22",
                    hlcmsProfileIn, hlcmsProfileOut, 0);

    // Cleanup: The focal method itself closes the provided profiles, but the
    // test harness may have created additional resources. Call RemoveTestProfiles
    // to ensure a clean slate for subsequent tests.
    RemoveTestProfiles();

    // Mark test as passed (the test is considered successful if no crash occurs)
    print_result("SpeedTest32bits_Basic_AboveRGB_to_Gray22", true);
    TEST_PASSED();
    return true;
}

// Test 2: Basic path - Gray22 -> GrayLab with default Intent (0)
static bool test_SpeedTest32bits_Basic_Gray22_to_GrayLab() {
    cmsHPROFILE hlcmsProfileIn  = Create_Gray22();
    cmsHPROFILE hlcmsProfileOut = Create_GrayLab();

    if (hlcmsProfileIn == NULL || hlcmsProfileOut == NULL) {
        print_result("SpeedTest32bits_Basic_Gray22_to_GrayLab", false);
        TEST_FAILED();
        return false;
    }

    SpeedTest32bits("SpeedTest32bits_Basic_Gray22_to_GrayLab",
                    hlcmsProfileIn, hlcmsProfileOut, 0);

    RemoveTestProfiles();

    print_result("SpeedTest32bits_Basic_Gray22_to_GrayLab", true);
    TEST_PASSED();
    return true;
}

// Test 3: Variant - Different Intent value to exercise alternative code path
static bool test_SpeedTest32bits_Intents_Variants() {
    cmsHPROFILE hlcmsProfileIn  = Create_AboveRGB();
    cmsHPROFILE hlcmsProfileOut = Create_Gray22();

    if (hlcmsProfileIn == NULL || hlcmsProfileOut == NULL) {
        print_result("SpeedTest32bits_Intents_Variants", false);
        TEST_FAILED();
        return false;
    }

    // Use a non-zero Intent value to exercise a potential code path
    SpeedTest32bits("SpeedTest32bits_Intents_Variants",
                    hlcmsProfileIn, hlcmsProfileOut, 1);

    RemoveTestProfiles();

    print_result("SpeedTest32bits_Intents_Variants", true);
    TEST_PASSED();
    return true;
}

// Optional: a fourth test that uses another valid profile pair to increase coverage
static bool test_SpeedTest32bits_GrayLab_to_GrayLabSameInput() {
    cmsHPROFILE hlcmsProfileIn  = Create_GrayLab();
    cmsHPROFILE hlcmsProfileOut = Create_GrayLab();

    if (hlcmsProfileIn == NULL || hlcmsProfileOut == NULL) {
        print_result("SpeedTest32bits_GrayLab_to_GrayLabSameInput", false);
        TEST_FAILED();
        return false;
    }

    SpeedTest32bits("SpeedTest32bits_GrayLab_to_GrayLabSameInput",
                    hlcmsProfileIn, hlcmsProfileOut, 0);

    RemoveTestProfiles();

    print_result("SpeedTest32bits_GrayLab_to_GrayLabSameInput", true);
    TEST_PASSED();
    return true;
}

// Main: Run all tests
int main(int argc, char* argv[]) {
    // Greet and explain
    std::cout << "Running SpeedTest32bits test suite (no GTest; plain C++ harness)\n";

    bool t1 = test_SpeedTest32bits_Basic_AboveRGB_to_Gray22();
    bool t2 = test_SpeedTest32bits_Basic_Gray22_to_GrayLab();
    bool t3 = test_SpeedTest32bits_Intents_Variants();
    bool t4 = test_SpeedTest32bits_GrayLab_to_GrayLabSameInput();

    // Summary
    int total = 4;
    int failures = 0;
    if (!t1) ++failures;
    if (!t2) ++failures;
    if (!t3) ++failures;
    if (!t4) ++failures;

    std::cout << "\nTest summary: " << (total - failures) << " passed, "
              << failures << " failed, out of " << total << " tests.\n";

    // Return non-zero if any test failed to aid integration with CI
    return failures ? 1 : 0;
}