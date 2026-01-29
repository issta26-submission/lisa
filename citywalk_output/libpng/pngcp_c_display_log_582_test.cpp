// pngcp_display_log_tests.cpp
// A lightweight C++11 unit test suite for the focal method display_log
// in pngcp.c. This test harness avoids GoogleTest and uses a small
// custom assertion framework. It relies on the project providing the
// declarations for struct display, error_level, and the function
// display_log with C linkage (extern "C") in a header (e.g., pngcp.h).
// The tests redirect STDERR to capture output produced by display_log
// and verify the printed messages match expected strings.
// Note: This test is intended to be compiled and linked with the actual
// pngcp.c and its headers in the target environment.

#include <stdarg.h>
#include <functional>
#include <sys/types.h>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <limits.h>
#include <assert.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pnglibconf.h>
#include <setjmp.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <iostream>
#include <cstring>


extern "C" {
    // Include the project's header that defines struct display, error_level,
    // and the display_log function. The actual project header name may vary;
    // adjust the include accordingly (e.g., #include "pngcp.h").
    #include "pngcp.h"  // Placeholder: adapt to the real header in the project
}

// Lightweight assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT TRUE failed: " << (msg) << "\n"; \
        return false; \
    } \
} while (0)

#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::cerr << "ASSERT EQUAL failed: " << (msg) \
                  << " expected [" << (expected) << "] got [" << (actual) << "]\n"; \
        return false; \
    } \
} while (0)


// Capture STDERR output produced during a function call.
class StderrCapture {
public:
    StderrCapture() : saved_stderr_fd(-1), pipe_in(-1), pipe_out(-1) {}

    // Start redirecting STDERR to a pipe
    void start() {
        int fds[2];
        if (pipe(fds) != 0) {
            perror("pipe");
            return;
        }
        pipe_in = fds[0];
        pipe_out = fds[1];
        saved_stderr_fd = dup(STDERR_FILENO);
        if (saved_stderr_fd == -1) {
            perror("dup");
            return;
        }
        // Redirect STDERR to the write end of the pipe
        if (dup2(pipe_out, STDERR_FILENO) == -1) {
            perror("dup2");
            return;
        }
        // Close unused write end descriptor in this thread
        close(pipe_out);
    }

    // Stop redirecting and return the captured string
    std::string stop() {
        // Flush and restore STDERR
        fflush(stderr);
        if (saved_stderr_fd != -1) {
            dup2(saved_stderr_fd, STDERR_FILENO);
            close(saved_stderr_fd);
            saved_stderr_fd = -1;
        }

        // Read from the read end of the pipe
        std::string output;
        char buffer[4096];
        ssize_t n;
        while ((n = read(pipe_in, buffer, sizeof(buffer))) > 0) {
            output.append(buffer, static_cast<size_t>(n));
        }
        // Cleanup
        if (pipe_in != -1) close(pipe_in);
        pipe_in = -1;
        return output;
    }

private:
    int saved_stderr_fd;
    int pipe_in;
    int pipe_out;
};


// Helper to run a test with stderr capture
template <typename Func>
std::string run_with_stderr(Func f) {
    StderrCapture cap;
    cap.start();
    f();
    return cap.stop();
}


// Test Case A: Basic information with a non-null filename and LIBPNG_WARNING level.
// Expect a properly formatted message including the filename, mapped label,
// operation, and the formatted message.
bool test_case_A_basic_print_with_filename() {
    // Prepare a display struct instance (zero-initialized to avoid uninitialized fields)
    struct display dp = {};
    // Populate fields expected by display_log
    // These field names/types come from the focal code snippet and typical PNGCP code.
    dp.results = 0;
    // Apply CONTINUE flag to avoid exiting or longjmp in test runs
    dp.options = CONTINUE;
    dp.filename = "example.png";      // non-null filename
    dp.operation = "read";              // example operation
    dp.errset = 0;
    // dp.error_return left uninitialized on purpose; not used in this path

    // Run the focal function while capturing stderr
    std::string output = run_with_stderr([&](){
        display_log(&dp, LIBPNG_WARNING, "test %d", 42);
    });

    // Expected output format:
    // "filename: label(label from level): operation: test 42\n"
    std::string expected = "example.png: warning(libpng): read: test 42\n";

    // Compare
    if (output != expected) {
        std::cerr << "Test A failed. Expected:\n" << expected
                  << "Actual:\n" << output << "\n";
        return false;
    }
    return true;
}


// Test Case B: Null filename should use "<stdin>" in the output.
bool test_case_B_null_filename_uses_stdin() {
    struct display dp = {};
    dp.results = 0;
    dp.options = CONTINUE;
    dp.filename = NULL;                 // NULL filename => "<stdin>"
    dp.operation = "process";
    dp.errset = 0;

    std::string output = run_with_stderr([&](){
        // Use a level that will trigger printing given typical LEVEL_MASK semantics.
        display_log(&dp, INFORMATION, "value=%d", 7);
    });

    // Expected output should start with "<stdin>:" and contain the operation and message
    // Since the exact level label for INFORMATION is "information"
    std::string expected_prefix = "<stdin>: information: process: ";
    if (output.find(expected_prefix) != 0) {
        std::cerr << "Test B failed. Output did not start with expected prefix. Output:\n"
                  << output << "\n";
        return false;
    }
    // Also ensure the numeric payload appears
    if (output.find("value=7") == std::string::npos) {
        std::cerr << "Test B failed. Expected payload 'value=7' not found in output:\n"
                  << output << "\n";
        return false;
    }
    // Ensure it ends with newline
    if (!output.empty() && output.back() != '\n') {
        std::cerr << "Test B failed. Output does not end with newline:\n"
                  << output << "\n";
        return false;
    }
    return true;
}


// Test Case C: APP_WARNING level maps to "warning(pngcp)" label.
bool test_case_C_app_warning_label_mapping() {
    struct display dp = {};
    dp.results = 0;
    dp.options = CONTINUE;
    dp.filename = "appwarn.png";
    dp.operation = "transform";

    std::string output = run_with_stderr([&](){
        display_log(&dp, APP_WARNING, "ok %s", "yes");
    });

    // Expect the label "warning(pngcp)" for APP_WARNING
    std::string expected = "appwarn.png: warning(pngcp): transform: ok yes\n";
    if (output != expected) {
        std::cerr << "Test C failed. Expected:\n" << expected
                  << "Actual:\n" << output << "\n";
        return false;
    }
    return true;
}


// Test Case D: INTERNAL_ERROR and similar non-specific levels should map to "bug(pngcp)".
bool test_case_D_internal_or_default_maps_to_bug_label() {
    struct display dp = {};
    dp.results = 0;
    dp.options = CONTINUE;
    dp.filename = "unknown.png";
    dp.operation = "op";

    std::string output = run_with_stderr([&](){
        display_log(&dp, INTERNAL_ERROR, "crash %d", 1);
    });

    // Expect the label "bug(pngcp)" for INTERNAL_ERROR (as per switch-default path)
    std::string expected = "unknown.png: bug(pngcp): op: crash 1\n";
    if (output != expected) {
        std::cerr << "Test D failed. Expected:\n" << expected
                  << "Actual:\n" << output << "\n";
        return false;
    }
    return true;
}


// Entry point for running all tests
int main() {
    std::vector<std::pair<std::string, bool>> results;

    // Run Test A
    bool okA = test_case_A_basic_print_with_filename();
    results.emplace_back("test_case_A_basic_print_with_filename", okA);

    // Run Test B
    bool okB = test_case_B_null_filename_uses_stdin();
    results.emplace_back("test_case_B_null_filename_uses_stdin", okB);

    // Run Test C
    bool okC = test_case_C_app_warning_label_mapping();
    results.emplace_back("test_case_C_app_warning_label_mapping", okC);

    // Run Test D
    bool okD = test_case_D_internal_or_default_maps_to_bug_label();
    results.emplace_back("test_case_D_internal_or_default_maps_to_bug_label", okD);

    // Summary
    int passed = 0;
    for (auto &r : results) if (r.second) ++passed;
    std::cout << "pngcp_display_log_tests: " << passed << " / " << results.size() << " tests passed.\n";
    for (auto &r : results) {
        std::cout << " - " << r.first << ": " << (r.second ? "PASS" : "FAIL") << "\n";
    }
    return (passed == (int)results.size()) ? 0 : 1;
}