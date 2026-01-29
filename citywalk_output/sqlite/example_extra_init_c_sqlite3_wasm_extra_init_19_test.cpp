/*
Unit test suite for the focal function:
  int sqlite3_wasm_extra_init(const char *z)

Context and approach notes:
- The focal function is a simple C function defined in example_extra_init.c that writes to stderr and returns 0.
- There are no class dependencies or static members inside the provided focal snippet itself. To align with the requested structure, tests focus on the function's observable behavior: return value and emitted stderr output.
- Tests are written in C++11 without using Google Test. A lightweight, non-terminating assertion framework is implemented via helper functions.
- To validate stderr output, a small POSIX-based captureStderr helper is used (redirects stderr to a pipe, executes the test code, then restores and captures the output).
- The tests are designed to be executable from main() and do not rely on private/internal implementation details.
*/

// Note: We assume the test binary is linked with the object file containing
// sqlite3_wasm_extra_init (e.g., example_extra_init.c) so that the symbol is available.
// The function signature is declared with C linkage to match the focal function.

#include <functional>
#include <errno.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <iostream>
#include <algorithm>


extern "C" int sqlite3_wasm_extra_init(const char *z);

// Candidate Keywords (Step 1 concept): sqlite3_wasm_extra_init, fprintf, stderr, __FILE__, __func__, return 0

// Simple non-terminating test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

// Log test result without aborting on failure
static void logTestResult(const char* testName, bool passed, const std::string& detail = "") {
    ++g_totalTests;
    if (!passed) {
        ++g_failedTests;
        std::cerr << "[TEST FAILED] " << testName;
        if (!detail.empty()) {
            std::cerr << " - " << detail;
        }
        std::cerr << std::endl;
    } else {
        std::cout << "[TEST PASSED] " << testName << std::endl;
    }
}

// Capture stderr output produced by a function call.
// Uses a POSIX pipe to redirect stderr temporarily.
static std::string captureStderr(const std::function<void()> &func) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty output
        return "";
    }

    // Save current stderr
    int savedStderr = dup(fileno(stderr));
    if (savedStderr == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stderr to the pipe's write end
    fflush(stderr);
    if (dup2(pipefd[1], fileno(stderr)) == -1) {
        // Restore and cleanup on failure
        dup2(savedStderr, fileno(stderr));
        close(savedStderr);
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }
    // We no longer need this copy of the write end after dup2
    close(pipefd[1]);

    // Execute the function which writes to stderr
    func();

    // Important: flush potential buffered output
    fflush(stderr);

    // Restore original stderr
    dup2(savedStderr, fileno(stderr));
    close(savedStderr);

    // Read captured data from the read end
    std::string output;
    char buffer[4096];
    ssize_t n;
    // Read until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Test 1: Verify that sqlite3_wasm_extra_init returns 0 for NULL input and for non-NULL input
// Rationale: The function body always returns 0; this test ensures no accidental modification of behavior.
static void test_ReturnValueIsZero() {
    // NULL input
    int rvNull = sqlite3_wasm_extra_init(nullptr);
    bool okNull = (rvNull == 0);
    logTestResult("sqlite3_wasm_extra_init returns 0 for NULL input", okNull);

    // Non-NULL input
    int rvNonNull = sqlite3_wasm_extra_init("test");
    bool okNonNull = (rvNonNull == 0);
    logTestResult("sqlite3_wasm_extra_init returns 0 for non-NULL input", okNonNull);
}

// Test 2: Verify that stderr output contains the function name and the expected format
// Rationale: The function logs its invocation via fprintf(stderr, "%s: %s()\n", __FILE__, __func__);
// We capture stderr and check for "sqlite3_wasm_extra_init" within the output.
static void test_StderrContainsFunctionName() {
    auto output = captureStderr([]() {
        sqlite3_wasm_extra_init("sample");
    });
    bool containsFuncName = output.find("sqlite3_wasm_extra_init") != std::string::npos;
    // It's also desirable to ensure a newline is present; the function prints with '\n'
    bool endsWithNewline = !output.empty() && output.back() == '\n';
    logTestResult("stderr contains function name", containsFuncName, output);
    // Non-terminal assertion: we also record whether a newline appears to improve coverage.
    logTestResult("stderr ends with newline", endsWithNewline, output);
}

// Test 3: Ensure multiple consecutive calls to the function emit output on each call (non-trivial coverage)
static void test_MultipleCallsEmitOutputPerCall() {
    auto firstOutput = captureStderr([]() {
        sqlite3_wasm_extra_init("A");
    });
    auto secondOutput = captureStderr([]() {
        sqlite3_wasm_extra_init("B");
    });

    int linesFirst = static_cast<int>(std::count(firstOutput.begin(), firstOutput.end(), '\n'));
    int linesSecond = static_cast<int>(std::count(secondOutput.begin(), secondOutput.end(), '\n'));

    bool okFirst = linesFirst >= 1;
    bool okSecond = linesSecond >= 1;

    logTestResult("First call emits stderr line", okFirst, firstOutput);
    logTestResult("Second call emits stderr line", okSecond, secondOutput);
}

// Main function to run all tests
int main() {
    std::cout << "Starting unit tests for sqlite3_wasm_extra_init..." << std::endl;

    test_ReturnValueIsZero();
    test_StderrContainsFunctionName();
    test_MultipleCallsEmitOutputPerCall();

    // Summary
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests run: " << g_totalTests << std::endl;
    std::cout << "Tests failed   : " << g_failedTests << std::endl;
    if (g_failedTests == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failedTests << " test(s) failed." << std::endl;
        return 1;
    }
}