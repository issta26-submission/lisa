// Test suite for UnityPrintNumber in unity.c using a POSIX stdout capture strategy.
// This test harness is written in C++11 and avoids GTest; it uses lightweight, non-terminating checks
// (print-and-continue style) to maximize code coverage across scenarios.

#include <stddef.h>
#include <unistd.h>
#include <unity.h>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <string>



// The Unity build in this project defines UnityPrintNumber with the signature
// void UnityPrintNumber(const UNITY_INT number_to_print);
// We mirror the type UNITY_INT to int here for compatibility during linking.
typedef int UNITY_INT;
typedef unsigned int UNITY_UINT;
typedef unsigned int UNITY_UINT32;

// Forward declaration of the focal function under test (defined in unity.c)
extern "C" void UnityPrintNumber(const UNITY_INT number_to_print);

// Global output capture buffer (not strictly needed with per-test captures, but left for debugging)
std::string g_lastCapturedOutput;

// Helper: capture the stdout output produced by a call to UnityPrintNumber
// This redirects stdout to a pipe, invokes UnityPrintNumber, then reads the produced characters.
static std::string CaptureOutputOf_UnityPrintNumber(const UNITY_INT value)
{
    int pipefd[2] = {-1, -1};
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe creation failed: " << std::strerror(errno) << "\n";
        return "";
    }

    // Save original stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        std::cerr << "dup failed: " << std::strerror(errno) << "\n";
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        std::cerr << "dup2 failed: " << std::strerror(errno) << "\n";
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }

    // We're done with the now-redirected write end
    close(pipefd[1]);

    // Clear any prior content
    fflush(stdout);

    // Call the function under test
    UnityPrintNumber((UNITY_INT)value);

    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        std::cerr << "Restoring stdout failed: " << std::strerror(errno) << "\n";
        // Even if restore fails, attempt to read what was written
    }
    close(saved_stdout);

    // Read data from the pipe (the reader end is pipefd[0])
    std::string captured;
    const size_t BUF_SIZE = 1024;
    char buffer[BUF_SIZE];
    ssize_t r;
    // Important: the writer end was closed, so we should read until EOF
    while ((r = read(pipefd[0], buffer, BUF_SIZE)) > 0) {
        captured.append(buffer, static_cast<size_t>(r));
    }

    // Clean up reader end
    close(pipefd[0]);

    // Return captured output
    g_lastCapturedOutput = captured;
    return captured;
}

// Lightweight assertion helper that reports non-terminating failures
static void ExpectEquals(const std::string& actual, const std::string& expected, const std::string& test_name,
                         int& fail_count, int& pass_count)
{
    if (actual == expected) {
        ++pass_count;
        std::cout << "[PASS] " << test_name << " -> got expected: \"" << actual << "\"\n";
    } else {
        ++fail_count;
        std::cout << "[FAIL] " << test_name << " -> expected: \"" << expected
                  << "\", actual: \"" << actual << "\"\n";
    }
}

// Test 1: Positive number prints without leading minus
static void Test_UnityPrintNumber_Positive()
{
    const std::string test_name = "UnityPrintNumber_Positive(12345)";
    UNITY_INT input = 12345;
    std::string expected = "12345";

    std::string actual = CaptureOutputOf_UnityPrintNumber(input);
    ExpectEquals(actual, expected, test_name, /*fail_count*/ *(new int(0)), /*pass_count*/ *(new int(0)));
}

// Test 2: Negative number prints '-' followed by magnitude
static void Test_UnityPrintNumber_Negative()
{
    const std::string test_name = "UnityPrintNumber_Negative(-6789)";
    UNITY_INT input = -6789;
    std::string expected = "-6789";

    std::string actual = CaptureOutputOf_UnityPrintNumber(input);
    // Using a local counters for demonstration; we will re-wire using static counters for robustness
    static int fail_count = 0;
    static int pass_count = 0;
    ExpectEquals(actual, expected, test_name, fail_count, pass_count);
}

// Test 3: Zero prints "0"
static void Test_UnityPrintNumber_Zero()
{
    const std::string test_name = "UnityPrintNumber_Zero(0)";
    UNITY_INT input = 0;
    std::string expected = "0";

    std::string actual = CaptureOutputOf_UnityPrintNumber(input);
    static int fail_count = 0;
    static int pass_count = 0;
    ExpectEquals(actual, expected, test_name, fail_count, pass_count);
}

// Test 4: INT_MIN negative handling (MIN negative case)
static void Test_UnityPrintNumber_IntMin()
{
    const std::string test_name = "UnityPrintNumber_IntMin(-2147483648)";
    UNITY_INT input = -2147483648; // INT_MIN on 32-bit platforms
    std::string expected = "-2147483648";

    std::string actual = CaptureOutputOf_UnityPrintNumber(input);
    static int fail_count = 0;
    static int pass_count = 0;
    ExpectEquals(actual, expected, test_name, fail_count, pass_count);
}

// Test 5: Large positive value to ensure whole number printing (e.g., 987654321)
static void Test_UnityPrintNumber_LargePositive()
{
    const std::string test_name = "UnityPrintNumber_LargePositive(987654321)";
    UNITY_INT input = 987654321;
    std::string expected = "987654321";

    std::string actual = CaptureOutputOf_UnityPrintNumber(input);
    static int fail_count = 0;
    static int pass_count = 0;
    ExpectEquals(actual, expected, test_name, fail_count, pass_count);
}

// Entry point: run all tests and summarize results
int main()
{
    // Note: We run each test sequentially and accumulate pass/fail counts.
    // We intentionally avoid terminating on first failure to maximize code coverage.

    // Some environments may call main multiple times; protect via fresh captures in each test.

    // We'll implement a simple local counting mechanism
    int total = 0;
    int passed = 0;
    int failed = 0;

    // To keep tests independent, we re-use the same helper but update counts explicitly.
    // Test 1
    {
        UNITY_INT input = 12345;
        std::string actual = CaptureOutputOf_UnityPrintNumber(input);
        std::string expected = "12345";
        if (actual == expected) ++passed; else ++failed;
        ++total;
        std::cout << (actual == expected ? "[OK] " : "[ERR] ") << "Test UnityPrintNumber_Positive ------------------------------------------------\n";
    }
    // Test 2
    {
        UNITY_INT input = -6789;
        std::string actual = CaptureOutputOf_UnityPrintNumber(input);
        std::string expected = "-6789";
        if (actual == expected) ++passed; else ++failed;
        ++total;
        std::cout << (actual == expected ? "[OK] " : "[ERR] ") << "Test UnityPrintNumber_Negative -------------------------------------------------\n";
    }
    // Test 3
    {
        UNITY_INT input = 0;
        std::string actual = CaptureOutputOf_UnityPrintNumber(input);
        std::string expected = "0";
        if (actual == expected) ++passed; else ++failed;
        ++total;
        std::cout << (actual == expected ? "[OK] " : "[ERR] ") << "Test UnityPrintNumber_Zero -----------------------------------------------------\n";
    }
    // Test 4
    {
        UNITY_INT input = -2147483648; // INT_MIN
        std::string actual = CaptureOutputOf_UnityPrintNumber(input);
        std::string expected = "-2147483648";
        if (actual == expected) ++passed; else ++failed;
        ++total;
        std::cout << (actual == expected ? "[OK] " : "[ERR] ") << "Test UnityPrintNumber_IntMin ---------------------------------------------------\n";
    }
    // Test 5
    {
        UNITY_INT input = 987654321;
        std::string actual = CaptureOutputOf_UnityPrintNumber(input);
        std::string expected = "987654321";
        if (actual == expected) ++passed; else ++failed;
        ++total;
        std::cout << (actual == expected ? "[OK] " : "[ERR] ") << "Test UnityPrintNumber_LargePositive -----------------------------------------------\n";
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " passed, " << failed << " failed, out of " << total << " tests.\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}

// Notes and caveats:
// - This test harness relies on the UnityPrintNumber function being defined in unity.c
//   and printing to stdout via the UNITY_OUTPUT_CHAR macro and the internal
//   UnityPrintNumberUnsigned function. We capture stdout to validate the exact
//   sequence of characters produced.
// - The approach is POSIX-specific due to the use of pipe and dup2 for stdout capture.
//   It should work on Linux/macOS environments typically used for C/C++ unit tests.
// - Static values (INT_MIN, INT_MAX considerations) rely on typical 2's complement
//   representations. If the target environment uses a different representation, adjust accordingly.
// - This test avoids GTest and uses simple print-based pass/fail reporting for clarity.