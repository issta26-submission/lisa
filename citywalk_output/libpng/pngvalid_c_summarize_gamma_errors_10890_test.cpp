// pngvalid_test.cpp
// A lightweight C++11 unit test harness (no GTest) for the focal method
// summarize_gamma_errors in the pngvalid.c file.
// The tests are designed to be self-contained and rely only on standard C/C++
// facilities plus the provided focal function. They capture stdout/stderr to
// validate the textual output without terminating the test run early on
// assertion failures.

#include <unistd.h>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain knowledge guidance reference:
// - Import dependencies and compile with correct paths in your build system.
// - Cover true/false branches of predicates (which we simulate via different inputs).
// - Static members are not part of this focal test; here we test only public-like API.
// - Use only standard C/C++ facilities; avoid GTest or GMock.
// - Use non-terminating assertions; accumulate failures and report at end.
// - Access to static/private internals is avoided; we rely on the provided interface.
// - Since gmock is not used, no mocking is performed.
// - Distinguish by checking textual outputs (strings) rather than internal state.
// - All tests are invoked from main as per the domain guidance.

typedef const char* png_const_charp;

// Minimal compatible definition of png_modifier to be used by tests.
// This mirrors the fields accessed by summarize_gamma_errors in the focal method.
// Real libpng code may have a larger structure; for unit testing the target method
// we only need these fields to influence the printed output.
typedef struct png_modifier {
    double error_gray_2;
    double error_gray_4;
    double error_gray_8;
    double error_color_8;
    double error_indexed;      // only used when 'indexed' == 1
    double error_gray_16;
    double error_color_16;
} png_modifier;

// Declaration of the focal function under test. We declare it with C linkage
// to ensure proper symbol naming when linking against the library/object
// containing the real implementation.
extern "C" void summarize_gamma_errors(png_modifier *pm, png_const_charp who,
                                       int low_bit_depth, int indexed);

// Helper function: capture stdout and stderr while invoking the focal function.
static std::string capture_gamma_output(png_modifier &pm, const char *who,
                                        int low_bit_depth, int indexed)
{
    // Pipes to capture stdout and stderr
    int stdout_pipe[2];
    int stderr_pipe[2];
    if (pipe(stdout_pipe) != 0 || pipe(stderr_pipe) != 0) {
        // If pipe creation fails, return empty output to indicate failure
        return "";
    }

    // Save original fds
    int saved_stdout = dup(fileno(stdout));
    int saved_stderr = dup(fileno(stderr));

    // Redirect stdout/stderr to pipes
    dup2(stdout_pipe[1], fileno(stdout));
    dup2(stderr_pipe[1], fileno(stderr));

    // Close ends that are now duplicated
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    // Invoke the focal function
    summarize_gamma_errors(&pm, who, low_bit_depth, indexed);

    // Flush to ensure all data is written
    fflush(stdout);
    fflush(stderr);

    // Read all data from the pipes
    // Helper lambda to read all data from a given file descriptor
    auto read_all = [](int fd) -> std::string {
        std::string result;
        char buffer[4096];
        ssize_t n;
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            result.append(buffer, static_cast<size_t>(n));
        }
        return result;
    };

    std::string out = read_all(stdout_pipe[0]);
    std::string err = read_all(stderr_pipe[0]);

    // Restore original stdout/stderr
    dup2(saved_stdout, fileno(stdout));
    dup2(saved_stderr, fileno(stderr));

    // Close duplicated fds
    close(saved_stdout);
    close(saved_stderr);
    // Close the read ends
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);

    // Return combined output for assertions
    return out + err;
}

// Simple test harness utilities

static bool g_test_failed = false;
static std::vector<std::string> g_failure_details;

#define CHECK(cond, detail) do { \
    if(!(cond)) { \
        g_test_failed = true; \
        g_failure_details.push_back(detail); \
    } \
} while(0)

static void report_test_result(const char* test_name, bool passed)
{
    if(passed) {
        std::cout << "[PASSED] " << test_name << std::endl;
    } else {
        std::cout << "[FAILED] " << test_name << std::endl;
        for(const auto &d : g_failure_details) {
            std::cout << "  - " << d << std::endl;
        }
        // Clear details for next test
        g_failure_details.clear();
    }
}

// Test 1: Basic behavior when who is provided and all low-bit-depth fields are present.
// Expect to see a "Gamma correction with <who>:" header and sections for 2/4/8 gray,
// 8-bit color, indexed, and 16-bit gray/color.
static bool test_summarize_gamma_errors_with_who_and_all_bits()
{
    png_modifier pm;
    pm.error_gray_2 = 0.11;
    pm.error_gray_4 = 0.22;
    pm.error_gray_8 = 0.33;
    pm.error_color_8 = 0.44;
    pm.error_indexed = 0.55;
    pm.error_gray_16 = 0.66;
    pm.error_color_16 = 0.77;

    std::string output = capture_gamma_output(pm, "test-case", 1, 1);

    bool contains_header = output.find("Gamma correction with test-case:") != std::string::npos;
    bool contains_2bit   = output.find("2 bit gray") != std::string::npos;
    bool contains_4bit   = output.find("4 bit gray") != std::string::npos;
    bool contains_8bit   = output.find("8 bit gray") != std::string::npos;
    bool contains_color8 = output.find("8 bit color") != std::string::npos;
    bool contains_index  = output.find("indexed:    ") != std::string::npos;
    bool contains_16gray = output.find("16 bit gray") != std::string::npos;
    bool contains_16color= output.find("16 bit color") != std::string::npos;

    bool ok = contains_header && contains_2bit && contains_4bit && contains_8bit &&
              contains_color8 && contains_index && contains_16gray && contains_16color;

    // For readability, also print the captured output for debugging (optional)
    // std::cout << "Captured Output:\n" << output << "\n";

    report_test_result("test_summarize_gamma_errors_with_who_and_all_bits", ok);
    return ok;
}

// Test 2: Behavior when who is NULL and low_bit_depth is 0 (no low-bit-depth sections).
// Expect to see the "Basic gamma correction:" header and 16-bit sections, but no "2 bit gray" lines.
static bool test_summarize_gamma_errors_without_who_or_low_bits()
{
    png_modifier pm;
    pm.error_gray_2 = 0.0;
    pm.error_gray_4 = 0.0;
    pm.error_gray_8 = 0.0;
    pm.error_color_8 = 0.0;
    pm.error_indexed = 0.0;
    pm.error_gray_16 = 1.23;
    pm.error_color_16 = 4.56;

    std::string output = capture_gamma_output(pm, nullptr, 0, 0);

    bool contains_header = output.find("Basic gamma correction:") != std::string::npos;
    bool contains_2bit   = output.find("2 bit gray") != std::string::npos;
    bool contains_16gray = output.find("16 bit gray") != std::string::npos;
    bool contains_16color= output.find("16 bit color") != std::string::npos;

    bool ok = contains_header && contains_16gray && contains_16color && !contains_2bit;

    report_test_result("test_summarize_gamma_errors_without_who_or_low_bits", ok);
    return ok;
}

// Test 3: When indexed is 0, the "indexed:" line should not be printed even if low-bit-depth
// is enabled. This validates that the function respects the indexed flag.
static bool test_summarize_gamma_errors_indexed_flag_respected()
{
    png_modifier pm;
    pm.error_gray_2 = 0.9;
    pm.error_gray_4 = 0.8;
    pm.error_gray_8 = 0.7;
    pm.error_color_8 = 0.6;
    pm.error_indexed = 0.5;
    pm.error_gray_16 = 0.4;
    pm.error_color_16 = 0.3;

    std::string output = capture_gamma_output(pm, "indexed_test", 1, 0); // indexed = 0

    bool has_indexed_line = output.find("indexed:    ") != std::string::npos;

    report_test_result("test_summarize_gamma_errors_indexed_flag_respected", !has_indexed_line);
    return !has_indexed_line;
}

// Entry point
int main(void)
{
    // Run tests
    bool t1 = test_summarize_gamma_errors_with_who_and_all_bits();
    bool t2 = test_summarize_gamma_errors_without_who_or_low_bits();
    bool t3 = test_summarize_gamma_errors_indexed_flag_respected();

    // Overall result
    if (t1 && t2 && t3) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
        return 0;
    } else {
        std::cout << "[SOME TESTS FAILED]" << std::endl;
        // Non-fatal exit; report individual test results above.
        return 1;
    }
}