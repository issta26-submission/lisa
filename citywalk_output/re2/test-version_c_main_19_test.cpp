// Test suite for the focal method main (as provided in test-version.c) using C++11 without gtest.
// The test targets the CRE2 library version reporting functions invoked by main.
// It captures stdout output to validate printed version information and formats.

#include <stdlib.h>
#include <cre2.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstdio>
#include <fcntl.h>


// Bind the C library header
extern "C" {
}

// Helper: capture stdout produced by a function (which prints to stdout) into a string.
// Uses POSIX pipes to redirect stdout during the function execution.

static std::string capture_output(void (*func)()) {
    int fds[2];
    if (pipe(fds) != 0) {
        throw std::runtime_error("pipe creation failed");
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        throw std::runtime_error("failed to duplicate stdout");
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(fds[1], fileno(stdout)) == -1) {
        throw std::runtime_error("failed to redirect stdout");
    }
    close(fds[1]); // The write end is now stdout

    // Ensure buffer is flushed before calling the function
    fflush(stdout);
    func();
    fflush(stdout);

    // Restore original stdout
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read the captured output from the read end
    FILE* reader = fdopen(fds[0], "r");
    if (!reader) {
        close(fds[0]);
        throw std::runtime_error("failed to open read end of pipe");
    }

    char buf[4096];
    std::string output;
    while (fgets(buf, sizeof(buf), reader) != nullptr) {
        output.append(buf);
    }

    fclose(reader);
    close(fds[0]);
    return output;
}

// Wrapper function that reproduces the two printf lines the focal main would produce.
// This avoids terminating the test process by calling exit(EXIT_SUCCESS).
static void test_main_prints_version_info() {
    printf("version number string: %s\n", cre2_version_string());
    printf("libtool version number: %d:%d:%d\n",
           cre2_version_interface_current(),
           cre2_version_interface_revision(),
           cre2_version_interface_age());
}

// Test 1: Verify cre2_version_string() returns a non-empty string.
// This tests the core dependency without requiring the actual main to run.
static void test_version_string_non_empty() {
    const char* s = cre2_version_string();
    if (s == nullptr) {
        throw std::runtime_error("cre2_version_string() returned null");
    }
    if (std::strlen(s) == 0) {
        throw std::runtime_error("cre2_version_string() returned empty string");
    }
    // If needed, we could also check that the string contains printable characters.
}

// Test 2: Verify version interface components are non-negative.
// This checks that the static versioning information is present and non-negative.
static void test_version_numbers_non_negative() {
    int cur = cre2_version_interface_current();
    int rev = cre2_version_interface_revision();
    int age = cre2_version_interface_age();

    if (cur < 0 || rev < 0 || age < 0) {
        throw std::runtime_error("One of the version components is negative");
    }
}

// Test 3: Capture and validate the actual main-like output produced by the wrapper.
// This ensures the formatting matches the expected prefixes and content is non-empty.
static void test_main_output_format_capture() {
    auto output = capture_output(test_main_prints_version_info);

    // Expect two lines, each ending with a newline
    // 1) "version number string: <value>"
    // 2) "libtool version number: <current:revision:age>"
    const std::string line1_prefix = "version number string: ";
    const std::string line2_prefix = "libtool version number: ";

    size_t pos1 = output.find(line1_prefix);
    if (pos1 == std::string::npos) {
        throw std::runtime_error("Output missing expected 'version number string' prefix");
    }
    size_t end1 = output.find('\n', pos1);
    if (end1 == std::string::npos) {
        throw std::runtime_error("Output missing newline after first line");
    }
    std::string version_str = output.substr(pos1 + line1_prefix.size(), end1 - (pos1 + line1_prefix.size()));
    if (version_str.empty()) {
        throw std::runtime_error("Version string printed by main wrapper is empty");
    }

    size_t pos2 = output.find(line2_prefix, end1);
    if (pos2 == std::string::npos) {
        throw std::runtime_error("Output missing expected 'libtool version number' prefix");
    }
    size_t end2 = output.find('\n', pos2);
    if (end2 == std::string::npos) {
        throw std::runtime_error("Output missing newline after second line");
    }
    std::string libtool_numbers = output.substr(pos2 + line2_prefix.size(), end2 - (pos2 + line2_prefix.size()));
    if (libtool_numbers.empty()) {
        throw std::runtime_error("Libtool version number printed is empty");
    }

    // Validate libtool_numbers structure: should be digits:digits:digits
    size_t c1 = libtool_numbers.find(':');
    if (c1 == std::string::npos) {
        throw std::runtime_error("Libtool version number missing first colon");
    }
    size_t c2 = libtool_numbers.find(':', c1 + 1);
    if (c2 == std::string::npos) {
        throw std::runtime_error("Libtool version number missing second colon");
    }

    auto is_digits = [](const std::string& s) {
        if (s.empty()) return false;
        for (char ch : s) {
            if (!std::isdigit(static_cast<unsigned char>(ch))) return false;
        }
        return true;
    };

    std::string part1 = libtool_numbers.substr(0, c1);
    std::string part2 = libtool_numbers.substr(c1 + 1, c2 - (c1 + 1));
    std::string part3 = libtool_numbers.substr(c2 + 1);

    if (!is_digits(part1) || !is_digits(part2) || !is_digits(part3)) {
        throw std::runtime_error("Libtool version components are not all digits");
    }
}

// Simple test harness
static void run_test(void (*test_func)(), const char* name) {
    try {
        test_func();
        std::cout << "[PASSED] " << name << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "[FAILED] " << name << " - " << ex.what() << std::endl;
        throw; // propagate to accumulate failures if desired
    } catch (...) {
        std::cerr << "[FAILED] " << name << " - unknown exception" << std::endl;
        throw;
    }
}

// Entry point: run all tests in sequence
int main() {
    std::vector<void (*)()> tests;
    // Register tests
    tests.push_back(test_version_string_non_empty);
    tests.push_back(test_version_numbers_non_negative);
    tests.push_back(test_main_output_format_capture);

    int passed = 0;
    int failed = 0;

    for (size_t i = 0; i < tests.size(); ++i) {
        const char* test_name = nullptr;
        // Assign a human-friendly name for each test
        switch (i) {
            case 0: test_name = "test_version_string_non_empty"; break;
            case 1: test_name = "test_version_numbers_non_negative"; break;
            case 2: test_name = "test_main_output_format_capture"; break;
            default: test_name = "unknown_test"; break;
        }
        try {
            run_test(tests[i], test_name);
            ++passed;
        } catch (...) {
            ++failed;
        }
    }

    // Summary
    std::cout << "Test summary: " << passed << " passed, " << failed << " failed." << std::endl;

    // Exit with non-zero status if any test failed
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}