// Test suite for ConsoleErrorHandler in tificc.c
// - No GTest/GMock used
// - C++11 compatible
// - Tests redirect and capture stderr, manipulate Verbose, and verify output
// - Uses a small, self-contained test harness with simple assertions
//
// Key dependencies and concepts (Candidate Keywords):
// - Verbose global flag controls output\n- Module parameter affects "[<module>] " prefix\n- fmt and va_list usage to format message\n- vfprintf/fprintf/fprintf(stderr) and fflush(stderr) behavior\n- NULL vs non-NULL module handling
//
// Notes:
// - This test assumes the Verbose symbol in the C code is publicly accessible as a small integral type (commonly int).
// - A lightweight wrapper is provided to call ConsoleErrorHandler with variadic arguments, which forwards to the C function using va_list.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <lcms2_plugin.h>
#include <functional>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <cstdarg>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>


// Declarations for the focal C function and its global dependent variable.
// The code in tificc.c defines ConsoleErrorHandler and a Verbose flag.
// We declare them for C++ linkage with C
extern "C" {
    // C function under test
    void ConsoleErrorHandler(const char* module, const char* fmt, va_list ap);

    // Verbose global flag (type is unknown from test; using int for compatibility)
    extern int Verbose;
}

// Helper: Variadic wrapper to call ConsoleErrorHandler with a variadic interface.
// This makes it easy to invoke the C function from C++ tests.
static void CallConsoleErrorHandler_va(const char* module, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    ConsoleErrorHandler(module, fmt, ap);
    va_end(ap);
}

// Helper: Capture stderr output produced by invoking a function.
// It redirects stderr to a pipe, runs the function, flushes, then restores and reads captured data.
static std::string captureStderrOutput(const std::function<void()>& func)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, fallback to empty output
        return "";
    }

    // Save current stderr
    int savedStderr = dup(STDERR_FILENO);
    // Redirect stderr to pipe write end
    dup2(pipefd[1], STDERR_FILENO);
    // Close unused write end in this process
    close(pipefd[1]);

    // Run the test function which will write to stderr
    func();

    // Flush and restore
    fflush(stderr);
    dup2(savedStderr, STDERR_FILENO);
    close(savedStderr);

    // Read all data from the pipe
    std::string output;
    char buffer[1024];
    ssize_t n;
    // Seek to read from pipe's read end
    // pipefd[0] remains open to read; capture until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Simple test harness state
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

static void report_result(const std::string& testName, bool passed)
{
    ++g_total;
    if (passed) {
        ++g_passed;
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        ++g_failed;
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Macro-like helpers for assertions on strings
#define ASSERT_EQ_STR(expected, actual, testName)                           \
    do {                                                                    \
        bool _ok = (std::string(expected) == std::string(actual));        \
        report_result(testName, _ok);                                     \
        if (!_ok) {                                                        \
            std::cerr << "  Expected: \"" << (expected) << "\"\n";         \
            std::cerr << "  Actual:   \"" << (actual) << "\"\n";         \
        }                                                                 \
    } while (false)

#define ASSERT_TRUE(cond, testName)                                          \
    do {                                                                       \
        bool _ok = !!(cond);                                                 \
        report_result(testName, _ok);                                        \
        if (!_ok) {                                                          \
            std::cerr << "  Condition failed: " << #cond << "\n";            \
        }                                                                    \
    } while (false)


// Test 1: Verbose is 0 -> no output regardless of module/format
static void test_verbose_off_no_output()
{
    // Ensure Verbose is treated as false
    Verbose = 0;

    auto out = captureStderrOutput([]() {
        CallConsoleErrorHandler_va("ModuleX", "Value=%d", 123);
    });

    ASSERT_EQ_STR("", out, "test_verbose_off_no_output");
}

// Test 2: Verbose on, module provided -> prints "[Module] " prefix
static void test_verbose_on_with_module()
{
    Verbose = 1;

    auto out = captureStderrOutput([]() {
        CallConsoleErrorHandler_va("ModuleA", "Value=%d", 42);
    });

    ASSERT_EQ_STR("Error: [ModuleA] Value=42\n", out, "test_verbose_on_with_module");
}

// Test 3: Verbose on, module is NULL -> no module prefix
static void test_verbose_on_with_null_module()
{
    Verbose = 1;

    auto out = captureStderrOutput([]() {
        CallConsoleErrorHandler_va(nullptr, "Value=%d", 7);
    });

    ASSERT_EQ_STR("Error: Value=7\n", out, "test_verbose_on_with_null_module");
}

// Test 4: Verbose on, multiple args including strings -> verify formatting
static void test_verbose_on_multiple_args()
{
    Verbose = 1;

    auto out = captureStderrOutput([]() {
        CallConsoleErrorHandler_va("ModTwo", "A=%d, B=%s", 1, "two");
    });

    ASSERT_EQ_STR("Error: [ModTwo] A=1, B=two\n", out, "test_verbose_on_multiple_args");
}

// Test 5: Verifies escaped percent in format string is handled correctly
static void test_verbose_on_escaped_percent()
{
    Verbose = 1;

    auto out = captureStderrOutput([]() {
        CallConsoleErrorHandler_va("FmtMod", "Progress: %d%% complete", 50);
    });

    ASSERT_EQ_STR("Error: [FmtMod] Progress: 50% complete\n", out, "test_verbose_on_escaped_percent");
}

// Entry point for tests
int main()
{
    // Run all tests
    test_verbose_off_no_output();
    test_verbose_on_with_module();
    test_verbose_on_with_null_module();
    test_verbose_on_multiple_args();
    test_verbose_on_escaped_percent();

    // Summary
    std::cout << "\nTest Summary: " << g_passed << " / " << g_total << " tests passed." << std::endl;
    if (g_failed > 0) {
        std::cout << g_failed << " tests failed." << std::endl;
    }

    return (g_failed == 0) ? 0 : 1;
}

/*
Notes on test coverage and design decisions:
- Coverage of both branches of Verbose (0 and 1) ensures the early return and the printing path are exercised.
- Coverage of module != NULL vs module == NULL tests verifies presence/absence of the "[<module>] " prefix.
- Coverage of formatting with integers, strings, and escaped percent signs ensures vfprintf formatting and newline flushing are exercised.
- The tests use a variadic wrapper (CallConsoleErrorHandler_va) to provide test inputs without requiring direct va_list construction in each test.
- Static dependencies: We rely on Verbose being a publicly accessible global (likely defined as an int in the C code). If the real type differs, the linker will still resolve the symbol, but care should be taken to ensure the value is interpreted truthily for true/false branches.
- Non-terminating assertions: Tests report pass/fail but do not terminate on first failure; a final summary indicates overall status.
- Namespace and standard library usage are kept minimal and self-contained to fit C++11 constraints and avoid external test frameworks.
*/