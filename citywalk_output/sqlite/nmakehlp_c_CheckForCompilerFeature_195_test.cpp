// Note: This test suite is designed for the focal method:
//       int CheckForCompilerFeature(const char *option)
//       located in nmakehlp.c
//
// Assumptions and environment:
// - The code under test uses Windows API (CreateProcess, pipes, etc.).
// - This test is intended to be compiled and run on Windows with a C++11 capable toolchain.
// - The tests avoid GTest and instead implement a simple, self-contained test harness.
// - The tests deliberately manipulate the PATH environment variable to exercise failure
//   and success scenarios in a deterministic manner where possible.
//
// What this test suite covers (Step 2 / Step 3 guidance applied):
// - Validates that the function gracefully handles the case where cl.exe cannot be found
//   by temporarily clearing PATH (simulating "cl.exe missing") and expecting a return of 2.
// - Provides a secondary check to ensure that the function executes and returns a value
//   within the expected domain when PATH is normal (non-deterministic in CI environments).
// - Uses a minimal, non-terminating assertion style (no GTest); test results are printed to stdout.
// - Keeps tests isolated and free from private data access; relies on the public API only.
//
// Important: This test file should be compiled together with the nmakehlp.c unit under test.
// If the project uses a separate build system, ensure both the C file and this test file link
// into the same executable.
//
// The tests include explanatory comments for each test case.

#ifdef _WIN32
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>


// Forward declaration of the focal function from nmakehlp.c.
// Since the file is compiled together in the target build, this extern "C" avoids
// C++ name mangling when the test is compiled as C++.
extern "C" int CheckForCompilerFeature(const char *option);

// Simple test harness utilities
static int g_total = 0;
static int g_failed = 0;

#define TEST_PASSED() (printf("[PASS] %s\n", __func__), (g_total++))
#define TEST_FAILED(msg) (printf("[FAIL] %s: %s\n", __func__, (msg)), (g_total++), (g_failed++))

// Helper: print summary at end
static void print_summary() {
    printf("Test Summary: Total=%d, Passed=%d, Failed=%d\n",
           g_total, g_total - g_failed, g_failed);
}

// Utility to safely restore PATH after a test
static void restore_path(const char* oldPath) {
    if (oldPath != NULL) {
        // Restore the previous PATH content
        // _putenv_s requires a modifiable string; we pass oldPath directly
        // The implementation stores the string as is for environment variable back
        _putenv_s("PATH", oldPath);
    }
}

// Test 1: Validate behavior when cl.exe is not found (PATH cleared)
static void Test_CheckForCompilerFeature_PathCleared_Returns2() {
    // Objective:
    // - Simulate "cl.exe" not found by clearing PATH.
    // - Expect CheckForCompilerFeature to return 2 (as per the error path when CreateProcess fails).
    printf("Running Test_CheckForCompilerFeature_PathCleared_Returns2...\n");

    // Save current PATH
    char* oldPath = NULL;
    size_t oldLen = 0;
    errno_t er = _dupenv_s(&oldPath, &oldLen, "PATH");
    if (er != 0) {
        TEST_FAILED("Unable to duplicate PATH environment for restoration.");
        return;
    }

    // Clear PATH to force CreateProcess to fail locating cl.exe
    er = _putenv_s("PATH", "");
    if (er != 0) {
        free(oldPath);
        TEST_FAILED("Unable to clear PATH for test.");
        return;
    }

    // Call the function under test with an empty option (typical value used in production)
    int result = CheckForCompilerFeature("");

    // Restore PATH immediately
    restore_path(oldPath);
    free(oldPath);

    // Expected outcome: 2 (per failure to launch cl.exe)
    if (result == 2) {
        TEST_PASSED();
    } else {
        char buf[128];
        snprintf(buf, sizeof(buf), "Expected 2, got %d", result);
        TEST_FAILED(buf);
    }
}

// Test 2: Sanity check path (normal PATH). Expect the function to return a value in [0,2].
// This test does not rely on the internal exact MSVC behavior and ensures the function
// executes without crashing in a typical environment.
static void Test_CheckForCompilerFeature_NormalPath_ReturnsInRange() {
    // Objective:
    // - Do not tamper with PATH (assuming a normal development environment where cl.exe might be found).
    // - Ensure the function returns a value within the expected range [0, 2].
    printf("Running Test_CheckForCompilerFeature_NormalPath_ReturnsInRange...\n");

    // Use a benign option
    const char* option = "";

    int result = CheckForCompilerFeature(option);

    if (result >= 0 && result <= 2) {
        TEST_PASSED();
    } else {
        char buf[128];
        snprintf(buf, sizeof(buf), "Result %d out of expected range [0,2]", result);
        TEST_FAILED(buf);
    }
}

int main() {
    // Run tests
    printf("Starting unit tests for CheckForCompilerFeature (nmakehlp.c) under Windows (C++11).\n");

    Test_CheckForCompilerFeature_PathCleared_Returns2();
    Test_CheckForCompilerFeature_NormalPath_ReturnsInRange();

    print_summary();
    // Return 0 if all tests pass, 1 otherwise for convenience
    return (g_failed > 0) ? 1 : 0;
}

#else
// Non-Windows environments: provide a brief explanation and skip tests.
int main() {
    std::cout << "This test suite is designed for Windows environments using the Windows API.\n";
    return 0;
}
#endif