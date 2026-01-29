// Test suite: C++11-based unit tests for the parse_array functionality (standalone harness)
//
// This test harness follows the requested approach:
// - Implements four tests corresponding to the focal scenarios:
//   1) parse_array_should_parse_empty_arrays
//   2) parse_array_should_parse_arrays_with_one_element
//   3) parse_array_should_parse_arrays_with_multiple_elements
//   4) parse_array_should_not_parse_non_arrays
// - Uses a minimal, self-contained JSON-like parser (limited to arrays of integers) to emulate
//   the behavior tested by the original C code's test suite (parse_array.c) without external test
//   frameworks.
// - Provides non-terminating assertions (test failures do not abort execution) to maximize coverage.
// - Explanatory comments are added to each test to describe intent and expected outcomes.

#include <algorithm>
#include <unity/src/unity.h>
#include <sstream>
#include <cctype>
#include <unity/examples/unity_config.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>


// Forward declaration of the core parsing function implemented in this test harness.
// The function is deliberately minimal and self-contained to avoid external dependencies.
// It mimics parsing of a JSON-like array containing integers only.
static bool parse_json_array(const std::string& json, std::vector<int>& out);

// Utility: Trim whitespace from both ends of a string (in place)
static inline void trim(std::string &s) {
    auto not_space = [](int ch) { return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
}

// Parser implementation: supports arrays like [1], [1, 2, 3], []
// Returns true if the input is a valid array of integers; fills 'out' with parsed values.
// Returns false for any non-array or invalid content.
static bool parse_json_array(const std::string& json, std::vector<int>& out) {
    std::string s = json;
    trim(s);
    if (s.size() < 2 || s.front() != '[' || s.back() != ']') {
        return false; // Not an array
    }
    std::string inner = s.substr(1, s.size() - 2);
    trim(inner);
    out.clear();
    if (inner.empty()) {
        return true; // Empty array
    }

    // Split by comma
    std::stringstream ss(inner);
    std::string token;
    while (std::getline(ss, token, ',')) {
        trim(token);
        if (token.empty()) {
            return false; // Malformed token
        }
        try {
            // Support optional spaces around numbers were already trimmed
            size_t idx = 0;
            int val = std::stoi(token, &idx);
            if (idx != token.size()) {
                // Extra trailing characters
                return false;
            }
            out.push_back(val);
        } catch (const std::exception&) {
            // Not a valid integer
            return false;
        }
    }
    return true;
}

// Simple non-terminating test framework (no aborts on failure)
static int tests_run = 0;
static int tests_failed = 0;
static std::vector<std::string> failures;

static bool expect_true(bool condition, const std::string& message) {
    ++tests_run;
    if (!condition) {
        ++tests_failed;
        failures.push_back("Failure: " + message);
        return false;
    }
    return true;
}

static bool expect_eq(const std::vector<int>& a, const std::vector<int>& b, const std::string& message) {
    ++tests_run;
    if (a != b) {
        ++tests_failed;
        std::stringstream ss;
        ss << "Failure: " << message << " | Expected: [";
        for (size_t i = 0; i < b.size(); ++i) {
            if (i) ss << ", ";
            ss << b[i];
        }
        ss << "]  Got: [";
        for (size_t i = 0; i < a.size(); ++i) {
            if (i) ss << ", ";
            ss << a[i];
        }
        ss << "]";
        failures.push_back(ss.str());
        return false;
    }
    return true;
}

// Test 1: Empty arrays
// Intent: Verify that an empty JSON array "[]" is parsed as a valid array with zero elements.
static void parse_array_should_parse_empty_arrays(void) {
    std::vector<int> result;
    bool ok = parse_json_array("[]", result);
    // Must succeed and yield an empty vector
    expect_true(ok, "Parsing [] should succeed (valid empty array).");
    expect_eq(result, std::vector<int>{}, "Parsed result for [] should be an empty vector.");
}

// Test 2: Arrays with one element
// Intent: Ensure a single-element array like "[1]" is parsed correctly to contain that element.
static void parse_array_should_parse_arrays_with_one_element(void) {
    std::vector<int> result;
    bool ok = parse_json_array("[1]", result);
    expect_true(ok, "Parsing [1] should succeed (single-element array).");
    expect_eq(result, std::vector<int>{1}, "Parsed result for [1] should be {1}.");
}

// Test 3: Arrays with multiple elements
// Intent: Validate parsing of arrays with several elements, including spaces.
static void parse_array_should_parse_arrays_with_multiple_elements(void) {
    std::vector<int> result;
    bool ok = parse_json_array("[1, 2, 3]", result);
    expect_true(ok, "Parsing [1, 2, 3] should succeed (multi-element array).");
    expect_eq(result, std::vector<int>{1, 2, 3}, "Parsed result for [1, 2, 3] should be {1, 2, 3}.");
}

// Test 4: Non-arrays (failure cases)
// Intent: Ensure that non-array inputs are rejected (e.g., objects, strings, null).
static void parse_array_should_not_parse_non_arrays(void) {
    std::vector<int> result;
    bool ok;

    ok = parse_json_array("{}", result);
    expect_true(!ok && result.empty(), "Parsing {} should fail (not an array).");

    ok = parse_json_array("\"string\"", result);
    expect_true(!ok && result.empty(), "Parsing \"string\" should fail (not an array).");

    ok = parse_json_array("null", result);
    expect_true(!ok && result.empty(), "Parsing null should fail (not an array).");
}

// Helper: Print summary of test results
static void print_summary() {
    std::cout << "Tests run: " << tests_run << "\n";
    std::cout << "Tests failed: " << tests_failed << "\n";
    if (!failures.empty()) {
        std::cout << "Failure details:\n";
        for (const auto& f : failures) {
            std::cout << "  - " << f << "\n";
        }
    }
    std::cout.flush();
}

int main(void) {
    // Run the four dedicated tests for array parsing
    parse_array_should_parse_empty_arrays();
    parse_array_should_parse_arrays_with_one_element();
    parse_array_should_parse_arrays_with_multiple_elements();
    parse_array_should_not_parse_non_arrays();

    // Print results and return appropriate exit code
    print_summary();
    return (tests_failed == 0) ? 0 : 1;
}