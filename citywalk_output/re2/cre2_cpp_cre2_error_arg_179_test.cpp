// Unit tests for cre2_error_arg in cre2.cpp
// Note: This test suite avoids GTest and uses a lightweight, non-terminating assertion approach.
// It focuses on exercising cre2_error_arg by validating behavior for both a valid and an invalid pattern.

#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the focal library header to access the API under test.

// Lightweight non-terminating assertion helper.
// Reports failures but does not abort test execution, enabling broader coverage.
static void expect_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "ERROR: " << message << std::endl;
    }
}

// Test 1: Validate that for a valid regular expression pattern, cre2_error_arg
// returns an empty error argument (length zero). This exercises the "true" (no error)
// branch-like behavior when the pattern is valid.
static void test_cre2_error_arg_valid_pattern() {
    std::cout << "Test cre2_error_arg with a valid pattern: ";

    // Create a CRE2 regex with a valid pattern. Opt is optional; pass NULL for defaults.
    cre2_regexp_t *re = cre2_new("a", 1, NULL);
    expect_true(re != nullptr, "cre2_new should return non-null for valid pattern");

    if (re != nullptr) {
        cre2_string_t arg{};
        cre2_error_arg(re, &arg);

        // Build a std::string from the returned C-string view.
        // Note: arg is interpreted as a string with length arg.length.
        std::string got(arg.data, arg.length);

        // For a valid pattern, error_arg should be empty.
        expect_true(arg.length == 0, "error_arg length should be 0 for valid pattern");
        expect_true(got.empty(), "error_arg content should be empty for valid pattern");

        cre2_delete(re);
    }

    std::cout << std::endl;
}

// Test 2: Validate that for an invalid regular expression pattern, cre2_error_arg
// returns a non-empty error argument. This exercises the non-empty-path behavior
// when the pattern is invalid.
static void test_cre2_error_arg_invalid_pattern() {
    std::cout << "Test cre2_error_arg with an invalid pattern: ";

    // Create a CRE2 regex with an invalid pattern.
    cre2_regexp_t *re = cre2_new("(", 1, NULL);
    if (re == nullptr) {
        // If cre2_new fails to allocate the object for invalid input, we still report the situation.
        std::cerr << "ERROR: cre2_new returned null for invalid pattern; cannot test error_arg." << std::endl;
        return;
    }

    cre2_string_t arg{};
    cre2_error_arg(re, &arg);

    // Expect a non-empty error argument for an invalid pattern.
    bool non_empty = (arg.length > 0);
    expect_true(non_empty, "error_arg should be non-empty for invalid pattern");

    if (non_empty) {
        // Optional: print a snippet of the error_arg content for debugging purposes.
        std::string snippet(arg.data, std::min<size_t>(arg.length, 64));
        std::cout << " (arg snippet: \"" << snippet << "\")";
    }

    cre2_delete(re);
    std::cout << std::endl;
}

int main() {
    // Run tests sequentially. The lightweight framework allows tests to continue
    // despite individual assertion failures, providing broader coverage.
    test_cre2_error_arg_valid_pattern();
    test_cre2_error_arg_invalid_pattern();

    // Optional: summarize if desired (omitted to keep output concise).
    return 0;
}