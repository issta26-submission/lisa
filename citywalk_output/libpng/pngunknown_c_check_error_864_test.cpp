// Test suite for the focal method: check_error(display *d, png_uint_32 flags, const char *message)
// This test harness is written for C++11, without GoogleTest, and relies on the project
// providing the C function `check_error` along with the necessary C dependencies.
// The tests focus on exercising the behavior of the bit-flag loop inside check_error,
// ensuring that each set flag is processed (invoking find_by_flag and chunk_info) and
// that the error_count is incremented accordingly. We also verify that an error message is
// printed to stderr in the expected format.
// Notes:
// - We provide a minimal C-compatible display structure to match the expectations of check_error.
// - We use a lightweight, custom assertion framework (no GTest) and a small test runner.
// - We attempt to capture stderr output to validate that the message contains the provided text.
// - This harness assumes POSIX compatibility for stderr capture (pipe/dup2). If run on non-POSIX
//   platforms, the stderr capture may be skipped with a warning.

#include <cstdint>
#include <unistd.h>
#include <setjmp.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <string>
#include <memory>
#include <functional>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>


// Domain-specific typedefs and declarations that mirror the C code's expectations.
// These definitions should be compatible with the actual project headers.
// If your build environment provides a different layout, adjust accordingly.

extern "C" {

// Forward-declare the focal function under test.
// The actual project should provide a proper prototype in a header; this mirrors typical usage.
typedef struct display display;

typedef uint32_t png_uint_32;
typedef int32_t png_int_32;

// The real project provides a struct named 'display' with at least these fields.
// We declare a matching layout to allow the test to construct a valid object.
struct display {
    const char *file;
    const char *test;
    int error_count;
};

// The focal function under test
void check_error(display *d, png_uint_32 flags, const char *message);

// The following symbols are typically provided by the project.
// They must exist in the linked binary; tests rely on their behavior.
// We declare them here as extern for linking with the library that implements them.
struct chunk_info_entry {
    const char *name;
};

// The real project defines an array of chunk_info with entries that have a `name` field.
// We declare the symbol so tests can rely on the linkage to the library.
extern "C" chunk_info_entry chunk_info[];

extern "C" png_uint_32 find_by_flag(png_uint_32 flag);

} // extern "C"


// Lightweight test framework (no GTest)
struct TestCase {
    std::string name;
    std::function<void()> func;
    bool passed;
    std::string failure_reason;
};

// Capture stderr output produced by a function call.
// Returns captured output as a string. Works on POSIX systems (pipe/dup2).
static std::string capture_stderr(const std::function<void()> &func) {
    // Create a pipe to capture stderr
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Fallback: cannot capture, run function and return empty string
        func();
        return "";
    }

    // Save old stderr and redirect to the pipe's write end
    int old_stderr_fd = dup(fileno(stderr));
    if (old_stderr_fd == -1) {
        // Fallback
        close(pipefd[0]);
        close(pipefd[1]);
        func();
        return "";
    }

    // Redirect
    if (dup2(pipefd[1], fileno(stderr)) == -1) {
        // Fallback
        close(pipefd[0]);
        close(pipefd[1]);
        close(old_stderr_fd);
        func();
        return "";
    }
    // Close the write end now that it's duplicated
    close(pipefd[1]);

    // Execute the function that will write to stderr
    func();

    // Flush and restore stderr
    fflush(stderr);
    dup2(old_stderr_fd, fileno(stderr));
    close(old_stderr_fd);

    // Read the captured output
    const size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    std::string output;
    ssize_t n;
    // Move to a safe blocking read from the read end
    while ((n = read(pipefd[0], buffer, BUF_SIZE)) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);

    return output;
}

// Simple assertion helpers
static void assert_true(bool condition, const std::string &msg) {
    if (!condition) {
        throw std::runtime_error("Assertion failed: " + msg);
    }
}

static void assert_eq_uint32(uint32_t expected, uint32_t actual, const std::string &msg) {
    if (expected != actual) {
        std::ostringstream oss;
        oss << msg << " | expected: " << expected << ", actual: " << actual;
        throw std::runtime_error(oss.str());
    }
}

// Test 1: No flags should produce no error_count increment and produce no extraneous output.
void test_check_error_no_flags() {
    display d;
    d.file = "unit_test.png";
    d.test = "no_flags";
    d.error_count = 0;

    // No flags: should not enter the loop
    check_error(&d, 0, "NO_FLAGS_ERROR");

    // Expect no changes
    assert_eq_uint32(0u, static_cast<uint32_t>(d.error_count),
                     "check_error should not modify error_count when flags == 0");
}

// Test 2: Single flag should increment error_count by 1 and print a line
void test_check_error_single_flag() {
    display d;
    d.file = "unit_test.png";
    d.test = "single_flag";
    d.error_count = 0;

    // Use a single bit flag (1). The library's find_by_flag should map it to some chunk index.
    // We only verify side-effect: error_count increased by 1.
    check_error(&d, 1u, "SINGLE_FLAG_ERROR");
    assert_eq_uint32(1u, static_cast<uint32_t>(d.error_count),
                     "check_error should increment error_count by 1 for a single flag");
}

// Test 3: Multiple flags should increment error_count correctly for each flag in order.
// The function isolates the least significant bit each iteration.
void test_check_error_multiple_flags() {
    display d;
    d.file = "unit_test.png";
    d.test = "multiple_flags";
    d.error_count = 0;

    // Use two flags: 1 and 2. Expect two increments.
    check_error(&d, 0x3u, "MULTI_FLAG_ERROR");
    assert_eq_uint32(2u, static_cast<uint32_t>(d.error_count),
                     "check_error should increment error_count for each set bit in flags");
}

// Test 4: Verify that the error message contains the provided text and chunk name appears in the output.
// We capture stderr to verify that the formatted line is emitted as per the implementation.
void test_check_error_output_format() {
    display d;
    d.file = "unit_test.png";
    d.test = "output_format";
    d.error_count = 0;

    // We use a small combination to trigger at least one print.
    auto captured = capture_stderr([&]() {
        check_error(&d, 1u, "OUTPUT_FORMAT_TEST");
    });

    // Validate that the output contains the provided message and chunk name placeholder.
    // Since we don't know the exact chunk name (depends on find_by_flag), we at least ensure
    // that the message string is present and that the file/test identifiers are echoed.
    if (captured.empty()) {
        throw std::runtime_error("check_error did not emit any output to stderr");
    }

    // Check for the known message substring
    if (captured.find("OUTPUT_FORMAT_TEST") == std::string::npos) {
        std::ostringstream error;
        error << "check_error output should contain the provided message. Output: " << captured;
        throw std::runtime_error(error.str());
    }

    // Check that the filename and test name appear in the output as part of the formatted line
    if (captured.find("unit_test.png") == std::string::npos ||
        captured.find("output_format") == std::string::npos) {
        std::ostringstream error;
        error << "check_error output should contain d->file and d->test identifiers. Output: " << captured;
        throw std::runtime_error(error.str());
    }
}

// Helper to run all tests and report
int main() {
    std::vector<TestCase> suite = {
        {"test_check_error_no_flags", test_check_error_no_flags, false, ""},
        {"test_check_error_single_flag", test_check_error_single_flag, false, ""},
        {"test_check_error_multiple_flags", test_check_error_multiple_flags, false, ""},
        {"test_check_error_output_format", test_check_error_output_format, false, ""}
    };

    int passed = 0;
    int total = static_cast<int>(suite.size());

    for (auto &tc : suite) {
        try {
            tc.func();
            tc.passed = true;
            ++passed;
            // Print a concise success message per test
            std::cout << "[PASS] " << tc.name << "\n";
        } catch (const std::exception &ex) {
            tc.passed = false;
            tc.failure_reason = ex.what();
            std::cerr << "[FAIL] " << tc.name << " - " << tc.failure_reason << "\n";
        } catch (...) {
            tc.passed = false;
            tc.failure_reason = "Unknown exception";
            std::cerr << "[FAIL] " << tc.name << " - Unknown exception\n";
        }
    }

    // Summary
    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}