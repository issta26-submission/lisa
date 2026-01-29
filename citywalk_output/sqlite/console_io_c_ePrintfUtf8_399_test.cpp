/*
Unit test suite for the focal method ePrintfUtf8 (C function) using a C++11 test harness.
Notes and assumptions:
- The test targets the provided ePrintfUtf8 function located in console_io.c.
- We assume the project builds ePrintfUtf8 with the standard environment (PST_INITIALIZER, getEmitStreamInfo, etc.)
- The tests focus on exercising the function's basic output behavior via its error stream (pfErr).
- We capture writes to stderr (typical for error/output streams in such code) by temporarily redirecting stderr to a temp file.
- This test suite uses only the C++ standard library (no Google Test or third-party mocking frameworks).
- The test runner is intended to be compiled and linked against the project (console_io.c) so that ePrintfUtf8 is available.
- Each test case includes explanatory comments describing intent and expected behavior.
*/

#include <functional>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>


// Declaration of the focal C function under test.
extern "C" int ePrintfUtf8(const char *zFormat, ...);

// Helper to capture output written to stderr during an action.
// It redirects stderr to a temporary file, runs the action, then reads the file content.
static std::string captureStderrOutput(const std::function<void()> &action) {
    // Save current stderr descriptor
    int stderrFd = fileno(stderr);
    int saved = dup(stderrFd);
    if (saved == -1) {
        std::cerr << "[TestHarness] Failed to duplicate stderr descriptor." << std::endl;
        return "";
    }

    // Create a temporary file to capture stderr output
    char tmpName[] = "/tmp/etestXXXXXX";
    int tmpFd = mkstemp(tmpName);
    if (tmpFd == -1) {
        std::cerr << "[TestHarness] Failed to create temporary file for stderr capture." << std::endl;
        close(saved);
        return "";
    }

    // Redirect stderr to the temporary file
    if (dup2(tmpFd, stderrFd) == -1) {
        std::cerr << "[TestHarness] Failed to redirect stderr." << std::endl;
        close(tmpFd);
        close(saved);
        unlink(tmpName);
        return "";
    }
    // We can close tmpFd now; it's duplicated to stderr
    close(tmpFd);

    // Run the test action that uses ePrintfUtf8
    action();

    // Ensure all output is flushed
    fflush(stderr);

    // Restore original stderr
    if (dup2(saved, stderrFd) == -1) {
        std::cerr << "[TestHarness] Failed to restore original stderr." << std::endl;
        close(saved);
        unlink(tmpName);
        return "";
    }
    close(saved);

    // Read captured output from the temporary file
    // Reopen the same file path to read from the beginning
    FILE *f = fopen(tmpName, "r");
    std::string content;
    if (f) {
        char buf[4096];
        // Move to start just in case
        fseek(f, 0, SEEK_SET);
        while (fgets(buf, sizeof(buf), f) != nullptr) {
            content += buf;
        }
        fclose(f);
    } else {
        std::cerr << "[TestHarness] Failed to open temporary file for reading." << std::endl;
    }

    // Cleanup temporary file
    unlink(tmpName);
    return content;
}

// Minimal assertion helpers (non-terminating)
static void assertEq(const std::string &got, const std::string &expected, const char *testName) {
    if (got == expected) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cerr << "[FAIL] " << testName << " - Expected: \"" << expected
                  << "\" | Got: \"" << got << "\"" << std::endl;
    }
}

// Step 1: Candidates (core dependencies) identified from focal method:
// - zFormat, va_list, PerStreamTags, PST_INITIALIZER
// - getEmitStreamInfo, CIO_WIN_WC_XLATE, pstReachesConsole, conioVmPrintf, vfprintf
// - pfErr captured via getEmitStreamInfo
// The tests exercise only the observable I/O behavior (output captured via stderr) and the return value.
// These keywords guide test intent and coverage expectations.

int main() {
    int totalTests = 3;
    int testsPassed = 0;

    // Test 1: Basic usage - simple string without format specifiers
    {
        std::string testName = "ePrintfUtf8_basic_string_no_args";
        std::string expected = "Hello World\n";

        std::string output = captureStderrOutput([]() {
            // Call the focal function with a simple string
            ePrintfUtf8("Hello World\n");
        });

        // Compare captured output with expected
        if (output == expected) {
            std::cout << "[PASS] " << testName << std::endl;
            testsPassed++;
        } else {
            std::cerr << "[FAIL] " << testName
                      << " - Expected: \"" << expected << "\""
                      << " | Got: \"" << output << "\""
                      << std::endl;
        }
    }

    // Test 2: Formatted integer argument - verify varargs path is exercised
    {
        std::string testName = "ePrintfUtf8_formatted_int";
        std::string expected = "Number=42\n";

        std::string output = captureStderrOutput([]() {
            ePrintfUtf8("Number=%d\n", 42);
        });

        if (output == expected) {
            std::cout << "[PASS] " << testName << std::endl;
            testsPassed++;
        } else {
            std::cerr << "[FAIL] " << testName
                      << " - Expected: \"" << expected << "\""
                      << " | Got: \"" << output << "\""
                      << std::endl;
        }
    }

    // Test 3: Formatted string argument - verify string formatting with %s
    {
        std::string testName = "ePrintfUtf8_formatted_string";
        std::string expected = "Greeting: Alice\n";

        std::string output = captureStderrOutput([]() {
            ePrintfUtf8("Greeting: %s\n", "Alice");
        });

        if (output == expected) {
            std::cout << "[PASS] " << testName << std::endl;
            testsPassed++;
        } else {
            std::cerr << "[FAIL] " << testName
                      << " - Expected: \"" << expected << "\""
                      << " | Got: \"" << output << "\""
                      << std::endl;
        }
    }

    // Summary
    if (testsPassed == totalTests) {
        std::cout << "All tests passed: " << testsPassed << "/" << totalTests << std::endl;
        return 0;
    } else {
        std::cerr << "Tests passed: " << testsPassed << "/" << totalTests << std::endl;
        return 1;
    }
}