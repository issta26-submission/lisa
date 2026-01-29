// Test suite for CheckPlanar8opt in testcms2.c
// This test file is designed for a C++11 environment WITHOUT Google Test.
// It uses a lightweight in-file test harness with non-terminating assertions.
// The tests rely on the existing C API declared in testcms2.h.

#include <cstdarg>
#include <iostream>
#include <cstdio>
#include <testcms2.h>


extern "C" {
}

// Lightweight, non-terminating assertion macro.
// It reports the failure but does not abort the test run, enabling higher coverage.
#define ASSERT_TRUE(cond, msg)                                  \
    do {                                                        \
        if(!(cond)) {                                           \
            std::cerr << "ASSERT FAILED: " << (msg)             \
                      << " (File: " << __FILE__                 \
                      << ", Line: " << __LINE__ << ")" << '\n';\
            ++g_TestFailures;                                   \
        }                                                       \
    } while(0)

// Global counter to accumulate test failures without exiting prematurely.
static int g_TestFailures = 0;

// Helper: Run a single test and print its result.
static void RunTest(const char* testName, void (*testFunc)()) {
    std::cout << "[ RUN    ] " << testName << std::endl;
    testFunc();
    if (g_TestFailures == 0) {
        std::cout << "[       OK ] " << testName << std::endl;
    } else {
        std::cout << "[  FAILED ] " << testName
                  << " (failures so far: " << g_TestFailures << ")" << std::endl;
    }
}

// Test 1: Verify that CheckPlanar8opt returns 1 under normal conditions.
// Rationale: The focal method is designed to perform a sequence of CMS LUT/profiles
// operations and then return 1. This test validates the basic contract.
static void Test_CheckPlanar8opt_Returns1_Version1() {
    // Act
    int result = CheckPlanar8opt();

    // Assert: Expect a non-error result of 1 as per the implementation.
    ASSERT_TRUE(result == 1, "CheckPlanar8opt should return 1 under normal conditions");
}

// Test 2: Verify that multiple consecutive invocations of CheckPlanar8opt are stable.
// Rationale: Ensures that resources are properly allocated and released across calls
// and that no internal state left behind causes failures on subsequent runs.
static void Test_CheckPlanar8opt_Twice_Returns1() {
    // Act
    int r1 = CheckPlanar8opt();
    int r2 = CheckPlanar8opt();

    // Assert: Both invocations should yield 1.
    ASSERT_TRUE((r1 == 1) && (r2 == 1),
                "Consecutive CheckPlanar8opt calls should both return 1");
}

// Test runner: executes all tests defined above.
static void Run_All_Tests() {
    std::cout << "Starting test suite for CheckPlanar8opt...\n" << std::endl;

    // Execute tests; each test uses non-terminating assertions.
    RunTest("Test_CheckPlanar8opt_Returns1_Version1", Test_CheckPlanar8opt_Returns1_Version1);
    RunTest("Test_CheckPlanar8opt_Twice_Returns1", Test_CheckPlanar8opt_Twice_Returns1);

    // Summary
    if (g_TestFailures == 0) {
        std::cout << "\nAll tests passed." << std::endl;
    } else {
        std::cout << "\nTest suite finished with " << g_TestFailures
                  << " failure(s)." << std::endl;
    }
}

// Entry point for the test executable.
// Note: If the project already provides a main via testcms2.h, this should be adjusted
// to avoid multiple definitions. This standalone main is intended to be used in
// environments where a separate test runner is appropriate.
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Run all tests and return non-zero if any test failed.
    Run_All_Tests();
    return (g_TestFailures != 0) ? 1 : 0;
}