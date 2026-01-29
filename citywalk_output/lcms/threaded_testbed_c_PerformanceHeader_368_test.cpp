// Unit test suite for the focal method PerformanceHeader located in threaded_testbed.c
// This test suite is designed for C++11 without GTest. It captures stdout produced by
// PerformanceHeader (which calls trace with a formatted string) and verifies the
// output matches the expected header line.
// Note: This test relies on the trace() implementation writing to stdout.

#include <utility>
#include <cstdio>
#include <functional>
#include <threaded_internal.h>
#include <string>
#include <iostream>
#include <time.h>
#include <type_traits>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <cerrno>


// Forward declaration of the focal function implemented in the C source file.
// The function uses C linkage.
extern "C" void PerformanceHeader(void);

// Simple non-terminating assertion helpers (to maximize test execution).
static int test_failures = 0;
#define ASSERT_EQUAL(a, b) { auto _a = (a); auto _b = (b); if (!(_a == _b)) { std::cerr << "ASSERT FAILED: " #a " != " #b " ("
; std::cerr << _a << " vs " << _b << ")\n"; ++test_failures; } }
#define ASSERT_TRUE(x) { bool _x = (x); if (!_x) { std::cerr << "ASSERT FAILED: condition " #x " is false\n"; ++test_failures; } }

// Capture stdout output produced by a function call using a POSIX pipe.
// The library function PerformanceHeader writes to stdout via trace(...).
// We redirect stdout to a pipe, execute the call, then read the captured output.
static std::string capture_stdout(const std::function<void()>& fn)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) return "";

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }
    close(pipefd[1]); // not needed after dup2

    // Execute the function that prints to stdout
    fn();

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        close(saved_stdout);
        close(pipefd[0]);
        return "";
    }
    close(saved_stdout);

    // Read captured output from the pipe
    std::string result;
    char buffer[1024];
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        result.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return result;
}

// Expected header line printed by PerformanceHeader
static const char* expectedHeader = "                                  MPixel/sec.   MByte/sec.\n";

// Test 1: Validate that PerformanceHeader prints the exact header line
// Comment: This ensures the function produces the intended header string via trace.
static void test_PerformanceHeader_PrintsHeader()
{
    std::string out = capture_stdout([]{ PerformanceHeader(); });
    ASSERT_EQUAL(out, std::string(expectedHeader));
}

// Test 2: Ensure repeated invocations produce identical output (consistency check)
// Comment: Verifies deterministic behavior across multiple calls.
static void test_PerformanceHeader_MultipleCallsConsistency()
{
    std::string o1 = capture_stdout([]{ PerformanceHeader(); });
    std::string o2 = capture_stdout([]{ PerformanceHeader(); });
    ASSERT_EQUAL(o1, std::string(expectedHeader));
    ASSERT_EQUAL(o2, std::string(expectedHeader));
    ASSERT_EQUAL(o1, o2);
}

// Test 3: Basic robustness check (no crash on invocation)
// Comment: Confirms the function can be called without throwing exceptions.
static void test_PerformanceHeader_DoesNotCrash()
{
    try {
        PerformanceHeader();
        ASSERT_TRUE(true);
    } catch (...) {
        ASSERT_TRUE(false);
    }
}

int main()
{
    // Run tests
    test_PerformanceHeader_PrintsHeader();
    test_PerformanceHeader_MultipleCallsConsistency();
    test_PerformanceHeader_DoesNotCrash();

    // Summarize results
    if (test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Total failures: " << test_failures << "\n";
        return 1;
    }
}