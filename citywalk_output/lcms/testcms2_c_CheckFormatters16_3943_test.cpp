/*
  C++11 test suite for the focal method:
  CheckFormatters16 in testcms2.c

  This test harness uses a lightweight, custom test runner (no GTest) and
  relies on the existing C APIs exposed by testcms2.h, including:
  - CheckFormatters16(void)
  - CheckSingleFormatter16(cmsContext, cmsUInt32Number, const char*)
  - DbgThread(void)
  - TYPE_* formatter type macros

  The tests are designed to be non-terminating: they log failures but always
  continue running to maximize code coverage.
*/

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdarg>


static std::vector<std::string> g_failures;

// Forward declarations for C API (imported from the CMS test header)
extern "C" {
  #include "testcms2.h"            // Provides CheckFormatters16, CheckSingleFormatter16, TYPE_* constants, etc.
  // The header is expected to declare cmsContext, cmsUInt32Number, etc.
  // If the header does not declare, you may add minimal externs as needed below.
  // Example (uncomment if needed and adjust types accordingly):
  // typedef void* cmsContext;
  // typedef unsigned int cmsUInt32Number;
  // int CheckFormatters16(void);
  // void CheckSingleFormatter16(cmsContext id, cmsUInt32Number Type, const char* Text);
  // cmsContext DbgThread(void);
}

// If the header isn't providing an extern for these, you may declare them here.
// This ensures tests can attempt to use a real test context and type constants.
// Remove/adjust if testcms2.h already provides them with C linkage.
// extern int FormatterFailed; // If you need to inspect internal state (not required for basic checks).

static void logFailure(const std::string& testName, const std::string& message) {
    std::cerr << "[FAIL] " << testName << ": " << message << std::endl;
    g_failures.push_back(testName + " - " + message);
}

// Test 1: Basic sanity for CheckFormatters16
// Purpose: Ensure the focal method completes and returns 1 when formatters are assumed to pass.
static void test_CheckFormatters16_AllPass() {
    // Call the focal method. Expect 1 on success.
    int result = CheckFormatters16();
    if (result != 1) {
        logFailure("CheckFormatters16_AllPass", "Expected return value 1 (success), got " + std::to_string(result));
    } else {
        std::cout << "[OK] CheckFormatters16_AllPass" << std::endl;
    }
}

// Test 2: Exercise the single-formatter tester for robustness without crashing.
// Purpose: Validate that the helper can be invoked with a known type and a valid context
//          without causing a crash or undefined behavior.
static void test_CheckSingleFormatter16_RGB8_NoCrash() {
    cmsContext ctx = DbgThread();
    if (ctx == nullptr) {
        logFailure("CheckSingleFormatter16_RGB8_NoCrash", "DbgThread() returned null context");
        return;
    }
    // Use a representative type; TYPE_RGB_8 is expected to be defined in testcms2.h
    // The Text is a descriptive label for the sub-test.
    CheckSingleFormatter16(ctx, TYPE_RGB_8, "Test_RGB_8");
    // We cannot assert internal state from this call directly here, but lack of crash is expected.
    std::cout << "[OK] CheckSingleFormatter16_RGB8_NoCrash" << std::endl;
}

// Test 3: Consistency check by invoking the focal method twice.
// Purpose: Ensure repeated calls do not corrupt internal static state and yield stable results.
static void test_CheckFormatters16_RepeatConsistency() {
    int res1 = CheckFormatters16();
    int res2 = CheckFormatters16();
    if (res1 != res2) {
        logFailure("CheckFormatters16_RepeatConsistency",
                   "Inconsistent results between consecutive calls: " +
                   std::to_string(res1) + " vs " + std::to_string(res2));
    } else {
        std::cout << "[OK] CheckFormatters16_RepeatConsistency" << std::endl;
    }
}

// Optional: Additional test to exercise another representative formatter type
// This helps broaden coverage by calling the single-formatter tester with a different type.
// If TYPE_YUV_8_PLANAR or TYPE_GRAY_16_SE is not defined in your environment, skip this test.
static void test_CheckSingleFormatter16_MultipleTypes() {
    cmsContext ctx = DbgThread();
    if (ctx == nullptr) {
        logFailure("CheckSingleFormatter16_MultipleTypes", "DbgThread() returned null context");
        return;
    }
    // Try a couple of different formatters to exercise more code paths in the tester.
    CheckSingleFormatter16(ctx, TYPE_GRAY_8, "Test_GRAY_8");
    CheckSingleFormatter16(ctx, TYPE_RGBA_8, "Test_RGBA_8");
    std::cout << "[OK] CheckSingleFormatter16_MultipleTypes (GRAY_8, RGBA_8)" << std::endl;
}

// Simple test runner
static void runAllTests() {
    std::cout << "Starting unit tests for CheckFormatters16 and related helpers..." << std::endl;

    test_CheckFormatters16_AllPass();
    test_CheckSingleFormatter16_RGB8_NoCrash();
    test_CheckFormatters16_RepeatConsistency();

    // Optional: run extra type tests if available in the environment
    test_CheckSingleFormatter16_MultipleTypes();

    std::cout << "Unit tests completed. Failures: " << g_failures.size() << std::endl;

    // Print summary of failures if any
    if (!g_failures.empty()) {
        std::cout << "\nFailure details:\n";
        for (const auto& f : g_failures) {
            std::cout << " - " << f << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    // Run tests in a non-terminating fashion; report results and exit non-zero if failures occurred.
    runAllTests();
    return g_failures.empty() ? 0 : 1;
}