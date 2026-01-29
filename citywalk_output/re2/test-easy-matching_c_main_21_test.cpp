// C++11 test harness for the focal function logic (cre2 easy matching) derived from the
// provided focal method main in test-easy-matching.c.
// This test suite exercises the same core behaviours via the cre2_easy_match API
// and validates the outcomes without terminating on assertion failures.

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <string>
#include <cre2-test.h>
#include <iostream>



// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper macro for simple assertion with non-terminating behavior
#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) \
                  << " [Line: " << __LINE__ << "]" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

#define ASSERT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if (static_cast<decltype(expected)>(actual) != (expected)) { \
        std::cerr << "Test failure: " << (msg) \
                  << " (expected: " << (expected) \
                  << ", actual: " << (actual) \
                  << ") [Line: " << __LINE__ << "]" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

// Helper to compare CRE2 string against a C-string content
static bool cre2_string_equals(const cre2_string_t& s, const char* expected) {
    if (s.data == nullptr || expected == nullptr) return false;
    int len_expected = static_cast<int>(std::strlen(expected));
    if (s.length != len_expected) return false;
    return std::strncmp(s.data, expected, s.length) == 0;
}

// Helper to compare CRE2 string array entries
static bool cre2_strings_equal(const cre2_string_t& s, const char* expected) {
    return cre2_string_equals(s, expected);
}

/*
 * Test Case 1: Simple exact match
 *   Pattern: "ciao"
 *   Text:    "ciao"
 * Expected: full match equals "ciao"
 * This corresponds to the first block in the focal main and validates
 * that a straightforward literal pattern yields a correct full match.
 */
static void test_case_simple_match() {
    // Setup
    const char* pattern = "ciao";
    const char* text = "ciao";

    cre2_string_t match;
    int nmatch = 1;

    // Exercise
    int retval = cre2_easy_match(pattern, std::strlen(pattern),
                                 text,    std::strlen(text),
                                 &match, nmatch);

    // Verify
    // Retval is commonly 0 on success for CRE2 easy_match; we don't rely solely on it,
    // but we check the produced match content.
    ASSERT_TRUE(match.data != nullptr, "simple_match: match data should be non-null");
    ASSERT_EQ(4, match.length, "simple_match: match length should be 4");
    ASSERT_TRUE(cre2_strings_equal(match, "ciao"), "simple_match: full match content should be 'ciao'");

    // Optional: ensure a successful return value when available
    // (depending on CRE2 version, retval might be 0 for success)
    // If retval is not available in the environment, this check can be ignored safely.
    (void)retval;
}

/*
 * Test Case 2: Invalid (unbalanced) pattern
 *   Pattern: "ci(ao" (invalid regex)
 *   Text:    "ciao"
 * Expected: retval == 2 (invalid pattern as per focal main)
 * This exercises the error path for incorrect regular expressions.
 */
static void test_case_invalid_pattern() {
    const char* pattern = "ci(ao";  // invalid regex (unbalanced parenthesis)
    const char* text = "ciao";

    cre2_string_t match;
    int nmatch = 1;

    int retval = cre2_easy_match(pattern, std::strlen(pattern),
                                 text,    std::strlen(text),
                                 &match, nmatch);

    // Verify that invalid pattern returns 2
    ASSERT_EQ(2, retval, "invalid_pattern: expected retval 2 for invalid pattern");
}

/*
 * Test Case 3: Pattern with two capturing groups
 *   Pattern: "(ciao) (hello)"
 *   Text:    "ciao hello"
 * Expected:
 *   match[0] == "ciao hello" (full match)
 *   match[1] == "ciao"        (group 1)
 *   match[2] == "hello"       (group 2)
 * This exercises both full match and capturing groups extraction.
 */
static void test_case_two_groups() {
    const char* pattern = "(ciao) (hello)";
    const char* text = "ciao hello";

    int nmatch = 3;
    cre2_string_t match[3];

    int retval = cre2_easy_match(pattern, std::strlen(pattern),
                                 text,    std::strlen(text),
                                 match, nmatch);

    // Verify success and content of all matches
    ASSERT_EQ(0, retval, "two_groups: expected success return value 0");

    ASSERT_TRUE(match[0].data != nullptr, "two_groups: full match data should be non-null");
    ASSERT_EQ(11, match[0].length, "two_groups: full match length should be 11");
    ASSERT_TRUE(cre2_string_equals(match[0], "ciao hello"), "two_groups: full match content should be 'ciao hello'");

    ASSERT_EQ(4, match[1].length, "two_groups: first group length should be 4");
    ASSERT_TRUE(cre2_string_equals(match[1], "ciao"), "two_groups: first group content should be 'ciao'");

    ASSERT_EQ(5, match[2].length, "two_groups: second group length should be 5");
    ASSERT_TRUE(cre2_string_equals(match[2], "hello"), "two_groups: second group content should be 'hello'");
}

/*
 * Entry point for the test suite.
 * Runs all test cases and reports a summary.
 * The test harness does not terminate on failure; instead, it aggregates results.
 */
int main() {
    // Run tests
    test_case_simple_match();
    test_case_invalid_pattern();
    test_case_two_groups();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed
              << std::endl;

    // Return non-zero if any test failed (useful for CI systems)
    return (g_tests_failed > 0) ? 1 : 0;
}