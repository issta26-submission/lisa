#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cre2-test.h>
#include <vector>
#include <iostream>


// This test suite exercises the focal CRE2-based "main" behavior by re-implementing
// representative test cases in C++11, focusing on the CRE2 C API functions
// (cre2_full_match and cre2_full_match_re). It avoids GTest and uses a small,
// non-terminating assertion approach to maximize code coverage across true/false
// branches.

namespace cre2_test_suite {

// Global counter for failed assertions
static int g_failures = 0;

// Non-terminating assertion helper
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            ++g_failures; \
            std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
        } \
    } while (0)

// Utility to reset test state
static void reset_state() {
    g_failures = 0;
}

// Test 1: Basic full_match (no capture groups) should succeed
// Pattern: "ci.*ut" on text: "ciao salut"
static void test_full_match_no_capture() {
    const char *pattern = "ci.*ut";
    const char *text    = "ciao salut";

    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int result = cre2_full_match(pattern, &input, NULL, 0);

    ASSERT_TRUE(result != 0, "cre2_full_match should succeed for pattern 'ci.*ut' with text 'ciao salut'");

    // Ensure input data remains unchanged (points to original text)
    ASSERT_TRUE(std::strncmp(text, input.data, input.length) == 0,
                "input data should remain equal to original text after match");
}

// Test 2: Full_match with one subexpression (capturing group)
// Pattern: "(ciao) salut" on text: "ciao salut"
static void test_full_match_one_capture() {
    const char *pattern = "(ciao) salut";
    const char *text    = "ciao salut";

    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int nmatch = 1;
    std::vector<cre2_string_t> match(nmatch);
    // Initialize match entries
    for (int i = 0; i < nmatch; ++i) {
        match[i].data = nullptr;
        match[i].length = 0;
    }

    int result = cre2_full_match(pattern, &input, &match[0], nmatch);

    ASSERT_TRUE(result != 0, "cre2_full_match should succeed for one-capture pattern");
    ASSERT_TRUE(std::strncmp(text, input.data, input.length) == 0,
                "input data should remain equal to original text after match");

    // Validate captured group content
    if (nmatch > 0 && match[0].data != nullptr) {
        ASSERT_TRUE(std::strncmp("ciao", match[0].data, match[0].length) == 0,
                    "captured group 0 should be 'ciao'");
    } else {
        ASSERT_TRUE(false, "captured group 0 should be populated");
    }
}

// Test 3: Full_match with two subexpressions
// Pattern: "(ciao) (salut)" on text: "ciao salut"
static void test_full_match_two_captures() {
    const char *pattern = "(ciao) (salut)";
    const char *text    = "ciao salut";

    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int nmatch = 2;
    std::vector<cre2_string_t> match(nmatch);
    for (int i = 0; i < nmatch; ++i) {
        match[i].data = nullptr;
        match[i].length = 0;
    }

    int result = cre2_full_match(pattern, &input, &match[0], nmatch);

    ASSERT_TRUE(result != 0, "cre2_full_match should succeed for two-capture pattern");
    ASSERT_TRUE(std::strncmp(text, input.data, input.length) == 0,
                "input data should remain equal to original text after match");

    // Validate captured groups
    if (match[0].data != nullptr) {
        ASSERT_TRUE(std::strncmp("ciao", match[0].data, match[0].length) == 0,
                    "captured group 0 should be 'ciao'");
    } else {
        ASSERT_TRUE(false, "captured group 0 should be populated");
    }

    if (match[1].data != nullptr) {
        ASSERT_TRUE(std::strncmp("salut", match[1].data, match[1].length) == 0,
                    "captured group 1 should be 'salut'");
    } else {
        ASSERT_TRUE(false, "captured group 1 should be populated");
    }
}

// Test 4: Failure case for basic full_match (no capture groups)
// Pattern: "ci.*ut" on text: "ciao hello" should fail
static void test_full_match_failure_no_capture() {
    const char *pattern = "ci.*ut";
    const char *text    = "ciao hello";

    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int result = cre2_full_match(pattern, &input, NULL, 0);

    ASSERT_TRUE(result == 0, "cre2_full_match should fail for non-matching text 'ciao hello'");
}

// Test 5: Failure case for capturing group with mismatched text
// Pattern: "(ciao) salut" on text: "ciao hello" should fail
static void test_full_match_failure_capture() {
    const char *pattern = "(ciao) salut";
    const char *text    = "ciao hello";

    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int nmatch = 1;
    std::vector<cre2_string_t> match(nmatch);
    for (int i = 0; i < nmatch; ++i) {
        match[i].data = nullptr;
        match[i].length = 0;
    }

    int result = cre2_full_match(pattern, &input, &match[0], nmatch);

    // Expect failure due to text mismatch
    ASSERT_TRUE(result == 0, "cre2_full_match should fail when text does not match capturing pattern");
}

// Test 6: Basic full_match_re (no capture) using CRE2 re object
// Pattern: "ci.*ut" on text: "ciao salut"
static void test_full_match_re_no_capture() {
    const char *pattern = "ci.*ut";
    const char *text    = "ciao salut";

    cre2_regexp_t *rex = cre2_new(pattern, strlen(pattern), NULL);
    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int result = cre2_full_match_re(rex, &input, NULL, 0);

    cre2_delete(rex);

    ASSERT_TRUE(result != 0, "cre2_full_match_re should succeed for pattern 'ci.*ut' with text 'ciao salut'");
    ASSERT_TRUE(std::strncmp(text, input.data, input.length) == 0,
                "input data should remain equal to original text after match_re");
}

// Test 7: Full_match_re with one capture
// Pattern: "(ciao) salut" on text: "ciao salut"
static void test_full_match_re_one_capture() {
    const char *pattern = "(ciao) salut";
    const char *text    = "ciao salut";

    cre2_regexp_t *rex = cre2_new(pattern, strlen(pattern), NULL);
    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int nmatch = 1;
    std::vector<cre2_string_t> match(nmatch);
    for (int i = 0; i < nmatch; ++i) {
        match[i].data = nullptr;
        match[i].length = 0;
    }

    int result = cre2_full_match_re(rex, &input, &match[0], nmatch);

    cre2_delete(rex);

    ASSERT_TRUE(result != 0, "cre2_full_match_re should succeed for one-capture with RE");
    ASSERT_TRUE(std::strncmp(text, input.data, input.length) == 0,
                "input data should remain equal to original text after match_re with one capture");

    if (match[0].data != nullptr) {
        ASSERT_TRUE(std::strncmp("ciao", match[0].data, match[0].length) == 0,
                    "captured group 0 (RE) should be 'ciao'");
    } else {
        ASSERT_TRUE(false, "captured group 0 (RE) should be populated");
    }
}

// Test 8: Full_match_re with two captures
// Pattern: "(ciao) (salut)" on text: "ciao salut"
static void test_full_match_re_two_captures() {
    const char *pattern = "(ciao) (salut)";
    const char *text    = "ciao salut";

    cre2_regexp_t *rex = cre2_new(pattern, strlen(pattern), NULL);
    cre2_string_t input;
    input.data = text;
    input.length = strlen(text);

    int nmatch = 2;
    std::vector<cre2_string_t> match(nmatch);
    for (int i = 0; i < nmatch; ++i) {
        match[i].data = nullptr;
        match[i].length = 0;
    }

    int result = cre2_full_match_re(rex, &input, &match[0], nmatch);

    cre2_delete(rex);

    ASSERT_TRUE(result != 0, "cre2_full_match_re should succeed for two captures");
    ASSERT_TRUE(std::strncmp(text, input.data, input.length) == 0,
                "input data should remain equal to original text after match_re (two captures)");

    if (match[0].data != nullptr) {
        ASSERT_TRUE(std::strncmp("ciao", match[0].data, match[0].length) == 0,
                    "captured group 0 (RE) should be 'ciao'");
    } else {
        ASSERT_TRUE(false, "captured group 0 (RE) should be populated");
    }

    if (match[1].data != nullptr) {
        ASSERT_TRUE(std::strncmp("salut", match[1].data, match[1].length) == 0,
                    "captured group 1 (RE) should be 'salut'");
    } else {
        ASSERT_TRUE(false, "captured group 1 (RE) should be populated");
    }
}

// Test runner: executes all tests and returns number of failures
static int run_all_tests() {
    reset_state();

    test_full_match_no_capture();
    test_full_match_one_capture();
    test_full_match_two_captures();
    test_full_match_failure_no_capture();
    test_full_match_failure_capture();

    test_full_match_re_no_capture();
    test_full_match_re_one_capture();
    test_full_match_re_two_captures();

    return g_failures;
}

} // namespace cre2_test_suite

int main() {
    int failures = cre2_test_suite::run_all_tests();
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cerr << failures << " TEST(S) FAILED" << std::endl;
    }
    return failures;
}