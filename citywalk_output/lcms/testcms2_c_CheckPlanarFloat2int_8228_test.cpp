// Lightweight C++11 test harness for the focal method CheckPlanarFloat2int
// This test suite avoids external testing frameworks (GTest) as requested.
// It imports the focal method from the C source via C linkage and verifies
// its basic behavior and stability.

#include <iostream>
#include <testcms2.h>


// Import the test harness header that contains necessary declarations.
// This mirrors the project's <FOCAL_CLASS_DEP> dependencies.

// Ensure linkage with the focal function from the C source.
extern "C" int CheckPlanarFloat2int(void);

// Global counters for test reporting
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper to log a passed test
static void log_pass(const char* name, const char* detail = nullptr) {
    std::cout << "[PASS] " << name;
    if (detail && detail[0] != '\0') {
        std::cout << " - " << detail;
    }
    std::cout << "\n";
}

// Helper to log a failed test
static void log_fail(const char* name, const char* reason) {
    std::cerr << "[FAIL] " << name << ": " << reason << "\n";
    ++g_tests_failed;
}

// Test 1: Basic return value check
// This validates that the focal function executes and returns the expected value.
// Rationale: CheckPlanarFloat2int is expected to return 1 on success.
// This test also exercises the function to ensure it doesn't crash under normal conditions.
static void test_CheckPlanarFloat2int_returns_one() {
    const char* testName = "test_CheckPlanarFloat2int_returns_one";
    ++g_tests_run;

    int result = CheckPlanarFloat2int();
    if (result == 1) {
        log_pass(testName, "Function returned 1 as expected.");
    } else {
        // Provide detail on the actual result for debugging
        char detail[64];
        std::snprintf(detail, sizeof(detail), "Unexpected return value: %d", result);
        log_fail(testName, detail);
    }
}

// Test 2: Consistency check by invoking the function multiple times
// Rationale: Some transforms may allocate internal resources on first call; verify
// subsequent calls are also successful and return 1.
// This increases coverage by exercising the function more than once.
static void test_CheckPlanarFloat2int_consistency() {
    const char* testName = "test_CheckPlanarFloat2int_consistency";
    ++g_tests_run;

    int r1 = CheckPlanarFloat2int();
    int r2 = CheckPlanarFloat2int();

    if (r1 == 1 && r2 == 1) {
        log_pass(testName, "Both consecutive calls returned 1.");
    } else {
        char detail[128];
        std::snprintf(detail, sizeof(detail),
                      "Inconsistent results: r1=%d, r2=%d", r1, r2);
        log_fail(testName, detail);
    }
}

// Entry point for running tests
int main() {
    // Run focused tests for the focal method
    test_CheckPlanarFloat2int_returns_one();
    test_CheckPlanarFloat2int_consistency();

    // Summary
    std::cout << "\nTest Summary: " << g_tests_run << " run, "
              << g_tests_failed << " failed.\n";

    // Non-zero exit code on failure to signal issues to build systems
    return g_tests_failed ? 1 : 0;
}