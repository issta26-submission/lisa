/*
Unit test suite for ConsoleWarningHandler in tificc.c
- Uses a custom lightweight test harness (no GTest).
- Captures stderr output by redirecting the process' STDERR to a pipe.
- Tests the behavior under different Verbose and module arguments.
- Provides explanatory comments for each test case.
- Requires a POSIX environment (pipe/dup2/read/write) for stdout/stderr capture.
*/

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cstdarg>
#include <unistd.h>
#include <errno.h>
#include <utils.h>
#include <tiffio.h>


// Forward declarations of the focal function and its dependencies.
// The actual implementation resides in tificc.c, included during linking.
// We declare with C linkage to ensure correct symbol resolution.
extern "C" {
    void ConsoleWarningHandler(const char* module, const char* fmt, va_list ap);
    // The Verbose flag is assumed to be a global (likely int/bool) defined in the C source.
    extern int Verbose;
}

// Wrapper to invoke ConsoleWarningHandler with a va_list via an ellipsis function.
// This allows us to re-create a va_list in C++ test code without directly constructing va_list here.
extern "C" void TestCallConsoleWarningHandler(const char* module, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    ConsoleWarningHandler(module, fmt, ap);
    va_end(ap);
}

// A small, POSIX-based stderr capture helper.
// Redirects stderr to a pipe during its lifetime; provides a method to read accumulated output.
class StderrCapture {
public:
    StderrCapture() : fdRead(-1), fdWrite(-1), oldStderr(-1) {
        int fds[2];
        if (pipe(fds) != 0) {
            perror("pipe");
            // Leave fds as -1 to indicate error; tests will fail accordingly.
            fdRead = fdWrite = -1;
            return;
        }
        fdRead = fds[0];
        fdWrite = fds[1];
        // Flush and replace stderr with the pipe's write end
        fflush(stderr);
        oldStderr = dup(fileno(stderr));
        if (oldStderr == -1) {
            perror("dup");
        } else {
            if (dup2(fdWrite, fileno(stderr)) != fileno(stderr) {
                perror("dup2");
            }
        }
        // Close the duplicate write end in this process; the redirected stdout/stderr now points to the pipe's write end.
        close(fdWrite);
    }

    ~StderrCapture() {
        // Restore original stderr
        if (oldStderr != -1) {
            fflush(stderr);
            dup2(oldStderr, fileno(stderr));
            close(oldStderr);
        }
        // Close the read end
        if (fdRead != -1) {
            close(fdRead);
        }
    }

    // Reads all data currently available in the pipe (flushing the buffer first).
    std::string get() {
        if (fdRead == -1) {
            return "";
        }
        // Ensure writers flush their data
        fflush(stderr);
        char buf[4096];
        std::string out;
        ssize_t n;
        // Read until EOF
        while ((n = read(fdRead, buf, sizeof(buf))) > 0) {
            out.append(buf, static_cast<size_t>(n));
            // If more data is written afterwards, caller should capture again; but tests call once per action.
        }
        return out;
    }

private:
    int fdRead;
    int fdWrite;
    int oldStderr;
};

// Convenience helpers for test success/failure reporting
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define RUN_TEST(test_func) do {                                      \
    ++g_total_tests;                                                    \
    bool passed = (test_func)();                                         \
    if (passed) { std::cout << "[PASS] " #test_func "\n"; }            \
    else {       ++g_failed_tests; std::cout << "[FAIL] " #test_func "\n"; }  \
} while (0)

#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { \
    ++g_failed_tests; \
    std::cerr << "  EXPECT_TRUE failed: " << (msg) << "\n"; \
} } while(0)

#define EXPECT_EQ_STR(a, b, msg) do { \
    if ((a) != nullptr && (b) != nullptr) { \
        if (std::strcmp((a), (b)) != 0) { \
            ++g_failed_tests; \
            std::cerr << "  EXPECT_EQ_STR failed: " << (msg) \
                      << " | expected \"" << (b) << "\", got \"" << (a) << "\"\n"; \
        } \
    } else if ((a) != (b)) { \
        ++g_failed_tests; \
        std::cerr << "  EXPECT_EQ_STR failed: " << (msg) \
                  << " | one side is null while the other is not\n"; \
    } \
} while(0)

#define EXPECT_EQ_STR_CONTENT(a, b, msg) do { \
    if (std::string(a) != std::string(b)) { \
        ++g_failed_tests; \
        std::cerr << "  EXPECT_EQ_STR_CONTENT failed: " << (msg) \
                  << " | expected \"" << (b) << "\", got \"" << (a) << "\"\n"; \
    } \
} while(0)

// Test 1: Verbose is off; ConsoleWarningHandler should produce no output.
bool test_verbose_off_no_output() {
    // Ensure global Verbose is off
    Verbose = 0;

    StderrCapture sc;
    // No arguments; simple fmt
    TestCallConsoleWarningHandler("ModuleA", "Test message with no args");
    std::string out = sc.get();

    // Expect no output
    bool pass = out.empty();
    if (!pass) {
        std::cerr << "  Unexpected output when Verbose=0: \"" << out << "\"\n";
    }
    return pass;
}

// Test 2: Verbose is on and module is provided; expect proper formatting with module tag.
bool test_verbose_on_with_module() {
    Verbose = 1;

    StderrCapture sc;
    TestCallConsoleWarningHandler("ModuleB", "Value=%d", 42);
    std::string out = sc.get();

    // Expected: "Warning: [ModuleB] Value=42\n"
    const std::string expected = "Warning: [ModuleB] Value=42\n";
    bool pass = (out == expected);
    if (!pass) {
        std::cerr << "  Expected: \"" << expected << "\" Got: \"" << out << "\"\n";
    }
    return pass;
}

// Test 3: Verbose is on and module is NULL; expect no module bracket, just the message.
bool test_verbose_on_null_module() {
    Verbose = 1;

    StderrCapture sc;
    TestCallConsoleWarningHandler(nullptr, "NoModule%d", 7);
    std::string out = sc.get();

    // Expected: "Warning: NoModule7\n"
    const std::string expected = "Warning: NoModule7\n";
    bool pass = (out == expected);
    if (!pass) {
        std::cerr << "  Expected: \"" << expected << "\" Got: \"" << out << "\"\n";
    }
    return pass;
}

// Main test runner
int main() {
    std::cout << "Starting ConsoleWarningHandler test suite...\n";

    RUN_TEST(test_verbose_off_no_output);
    RUN_TEST(test_verbose_on_with_module);
    RUN_TEST(test_verbose_on_null_module);

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << "\n";

    if (g_failed_tests > 0) {
        std::cerr << "Some tests failed. Please review the failure messages above.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}

/*
Notes and reasoning:
- The tests assume a POSIX environment to manipulate file descriptors for capturing stderr.
- TestCallConsoleWarningHandler provides a safe way to build a va_list in C++ by using a C-style ellipsis wrapper.
- The tests exercise:
  1) The false branch (Verbose == 0) -> no output.
  2) True branch with a non-null module: verifies proper formatting "[Module]".
  3) True branch with a null module: verifies no module bracket is printed.
- We rely on the exact textual output produced by ConsoleWarningHandler, including newline and spacing:
  - "Warning: " prefix
  - Optional " [%s] " module block when module is non-NULL
  - The formatted message
  - A trailing newline and flush
- If the actual Verbose type differs (e.g., bool vs int) in the focal code, the extern int Verbose declaration may still work in typical environments since we only assign and test; however, if this causes linkage or memory-size issues, adjust the type accordingly in the test harness.
*/