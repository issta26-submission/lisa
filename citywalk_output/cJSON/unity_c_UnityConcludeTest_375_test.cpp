// UnityConcludeTest Unit Test Suite (C++11) for Unity C framework
// This test harness targets the focal method UnityConcludeTest() declared in unity.c
// It uses the provided Unity framework (unity.h) and the global Unity state.
// No GoogleTest is used; a lightweight, non-terminating assertion style is employed.

#include <stddef.h>
#include <cstdio>
#include <unity.h>


// Step 1/2: Include Unity headers to access the Unity global state and API

#ifdef __cplusplus
extern "C" {
#endif
// The focal method under test located in unity.c
void UnityConcludeTest(void);
#ifdef __cplusplus
}
#endif

// Domain-specific: Minimal helper to report test outcomes without terminating tests
static int g_total_failures = 0;
static int g_total_tests = 0;

static void report_result(const char* test_name, bool passed) {
    if (passed) {
        printf("PASS: %s\n", test_name);
    } else {
        printf("FAIL: %s\n", test_name);
        g_total_failures++;
    }
    g_total_tests++;
}

// Helper: Reset the Unity framework's state to a clean slate for each test.
// We rely on the Unity header declaring the global Unity instance and its fields.
static void reset_unity_state() {
    // Reset control flags
    Unity.CurrentTestIgnored = 0;
    Unity.CurrentTestFailed  = 0;

    // Reset counters
    Unity.TestIgnores  = 0;
    Unity.TestFailures = 0;

    // Optional/auxiliary fields (not strictly required for these tests)
    Unity.TestFile = nullptr;
    Unity.CurrentTestLineNumber = 0;
    Unity.CurrentTestName = nullptr;
}

// Candidate Keywords (from Step 1): Unity, UnityConcludeTest, CurrentTestIgnored, CurrentTestFailed,
// TestIgnores, TestFailures, TestFile, CurrentTestLineNumber, CurrentTestName, UnityPrint,
// UnityTestResultsBegin, UnityStrPass, UNITY_PRINT_EOL, UNITY_FLUSH_CALL

// Test 1: When the current test is ignored, UnityConcludeTest should increment TestIgnores and reset flags.
static bool test_UnityConcludeTest_IgnoredIncrementsIgnores() {
    reset_unity_state();

    // Arrange: simulate an ignored test
    Unity.CurrentTestIgnored = 1;
    Unity.CurrentTestFailed  = 0;
    Unity.TestIgnores  = 0;
    Unity.TestFailures = 0;

    // Act
    UnityConcludeTest();

    // Assert (non-terminating): verify the ignore path was taken and state reset
    bool pass = (Unity.TestIgnores == 1) &&
                (Unity.CurrentTestIgnored == 0) &&
                (Unity.CurrentTestFailed == 0) &&
                (Unity.TestFailures == 0);

    report_result("UnityConcludeTest_IgnoredIncrementsIgnores", pass);
    return pass;
}

// Test 2: When the test is not ignored and not failed, counters should remain (or reset to 0) and flags cleared.
static bool test_UnityConcludeTest_NoFailResetsFlags() {
    reset_unity_state();

    // Arrange: some non-zero counters that should be preserved (or reset as per function behavior)
    Unity.CurrentTestIgnored = 0;
    Unity.CurrentTestFailed  = 0;
    Unity.TestIgnores  = 5;
    Unity.TestFailures = 3;

    // Act
    UnityConcludeTest();

    // Assert
    bool pass = (Unity.TestIgnores == 5) &&
                (Unity.TestFailures == 3) &&
                (Unity.CurrentTestIgnored == 0) &&
                (Unity.CurrentTestFailed == 0);

    report_result("UnityConcludeTest_NoFailResetsFlags", pass);
    return pass;
}

// Test 3: When the test has failed, UnityConcludeTest should increment TestFailures and reset flags.
static bool test_UnityConcludeTest_FailedIncrementsFailures() {
    reset_unity_state();

    // Arrange: simulate a failed test
    Unity.CurrentTestIgnored = 0;
    Unity.CurrentTestFailed  = 1;
    Unity.TestIgnores  = 2;
    Unity.TestFailures = 7;

    // Act
    UnityConcludeTest();

    // Assert
    bool pass = (Unity.TestFailures == 8) &&
                (Unity.CurrentTestIgnored == 0) &&
                (Unity.CurrentTestFailed == 0) &&
                (Unity.TestIgnores == 2); // unchanged in this path

    report_result("UnityConcludeTest_FailedIncrementsFailures", pass);
    return pass;
}

// Test 4: If a test is both ignored and failed (ignores take precedence), ensure ignore path executes.
static bool test_UnityConcludeTest_IgnoredWinsOverFailed() {
    reset_unity_state();

    // Arrange: both flags set
    Unity.CurrentTestIgnored = 1;
    Unity.CurrentTestFailed  = 1;
    Unity.TestIgnores  = 2;
    Unity.TestFailures = 5;

    // Act
    UnityConcludeTest();

    // Assert: ignore path should win
    bool pass = (Unity.TestIgnores == 3) && // increments by 1
                (Unity.TestFailures == 5) &&
                (Unity.CurrentTestIgnored == 0) &&
                (Unity.CurrentTestFailed == 0);

    report_result("UnityConcludeTest_IgnoredWinsOverFailed", pass);
    return pass;
}

// Main test runner
int main() {
    // Run tests
    test_UnityConcludeTest_IgnoredIncrementsIgnores();
    test_UnityConcludeTest_NoFailResetsFlags();
    test_UnityConcludeTest_FailedIncrementsFailures();
    test_UnityConcludeTest_IgnoredWinsOverFailed();

    // Summary
    printf("Summary: %d tests, %d failures\n", g_total_tests, g_total_failures);
    // Return non-zero if any test failed
    return (g_total_failures == 0) ? 0 : 1;
}