// Test suite for the focal method main behavior by exercising CRE2 partial matching APIs.
// This test targets the same functional space as the provided C test (test-partial-match.c)
// but implemented in C++11 without GoogleTest. It uses the CRE2 C API directly from C++.
//
// Notes:
// - We avoid relying on private/internal state. We verify outcomes via return codes
//   and by inspecting the captured match substrings when provided.
// - We exercise both cre2_partial_match and cre2_partial_match_re paths.
// - We cover success and failure branches, including various capture group scenarios.
// - We compile against C++11 and link against the CRE2 library (e.g., -lcre2).
//
// Candidate Keywords (Step 1): cre2_partial_match, cre2_partial_match_re, cre2_new, cre2_delete,
// cre2_string_t, cre2_regexp_t, match entries, patterns with and without parentheses,
// success/failure branches, static helpers, input text/current input, non-terminating tests.
//
// Domain knowledge considerations (Step 3): Use only standard library to report pass/fail,
// call test methods from main, avoid terminating tests prematurely, and ensure coverage
// of true/false branches for each predicate.

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cre2-test.h>
#include <vector>
#include <iostream>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple test harness: accumulate results and print a summary.
// We avoid terminating the test suite on first failure to maximize coverage.

static bool test_basic_no_parenthesis() {
    // Test: success case with no capturing groups using cre2_partial_match
    // Pattern ci.*ut should match inside the provided text.
    const char *pattern = "ci.*ut";
    const char *text = "pre ciao salut post";

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int result = cre2_partial_match(pattern, &input, NULL, 0);
    if (result == 0) {
        std::cerr << "[FAIL] test_basic_no_parenthesis: expected success (non-zero result).\n";
        return false;
    }
    // We do not rely on input pointer arithmetic here; the library handles the semantics.
    // At least ensure we did not crash and returned a non-zero result.
    return true;
}

static bool test_single_group() {
    // Test: success with one capturing group and one match entry
    const char *pattern = "(ciao) salut";
    const char *text = "ciao salut";

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int nmatch = 1;
    std::vector<cre2_string_t> match(nmatch);

    int result = cre2_partial_match(pattern, &input, match.data(), nmatch);
    if (result == 0) {
        std::cerr << "[FAIL] test_single_group: expected success (non-zero result).\n";
        return false;
    }
    // Validate the captured group content
    if (match[0].data == nullptr || match[0].length == 0) {
        std::cerr << "[FAIL] test_single_group: expected non-empty match for group 0.\n";
        return false;
    }
    if (std::strncmp("ciao", match[0].data, (size_t)match[0].length) != 0) {
        std::cerr << "[FAIL] test_single_group: expected capture to be 'ciao'.\n";
        return false;
    }
    return true;
}

static bool test_two_groups_two_matches() {
    // Test: two capturing groups, two match entries
    const char *pattern = "(ciao) (salut)";
    const char *text = "ciao salut";

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int nmatch = 2;
    std::vector<cre2_string_t> match(nmatch);

    int result = cre2_partial_match(pattern, &input, match.data(), nmatch);
    if (result == 0) {
        std::cerr << "[FAIL] test_two_groups_two_matches: expected success (non-zero).\n";
        return false;
    }

    if (match[0].data == nullptr || match[0].length == 0 ||
        match[1].data == nullptr || match[1].length == 0) {
        std::cerr << "[FAIL] test_two_groups_two_matches: expected non-empty captures.\n";
        return false;
    }

    if (std::strncmp("ciao", match[0].data, (size_t)match[0].length) != 0) {
        std::cerr << "[FAIL] test_two_groups_two_matches: first capture should be 'ciao'.\n";
        return false;
    }
    if (std::strncmp("salut", match[1].data, (size_t)match[1].length) != 0) {
        std::cerr << "[FAIL] test_two_groups_two_matches: second capture should be 'salut'.\n";
        return false;
    }
    return true;
}

static bool test_failure_no_match_no_capture() {
    // Test: failure case for pattern ci.*ut on a text that doesn't end with 'ut' to match
    const char *pattern = "ci.*ut";
    const char *text = "ciao hello";

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int result = cre2_partial_match(pattern, &input, NULL, 0);
    if (result != 0) {
        std::cerr << "[FAIL] test_failure_no_match_no_capture: expected failure (result == 0).\n";
        return false;
    }
    return true;
}

static bool test_wrong_regexp_specification() {
    // Test: wrong regexp specification should yield non-zero (error) on compile/parse
    const char *pattern = "cia(o salut"; // malformed
    const char *text = "ciao hello";

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int nmatch = 1;
    std::vector<cre2_string_t> match(nmatch);

    int result = cre2_partial_match(pattern, &input, match.data(), nmatch);
    if (result == 0) {
        std::cerr << "[FAIL] test_wrong_regexp_specification: expected non-zero error for bad pattern.\n";
        return false;
    }
    return true;
}

static bool test_re_path_single_group() {
    // Test: using cre2_new + cre2_partial_match_re with single capture
    const char *pattern = "(ciao) salut";
    const char *text = "ciao salut";

    cre2_regexp_t *rex = cre2_new(pattern, (int)std::strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "[FAIL] test_re_path_single_group: cre2_new returned NULL.\n";
        return false;
    }

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int nmatch = 1;
    std::vector<cre2_string_t> match(nmatch);

    int result = cre2_partial_match_re(rex, &input, match.data(), nmatch);
    cre2_delete(rex);
    if (result == 0) {
        std::cerr << "[FAIL] test_re_path_single_group: expected success (non-zero).\n";
        return false;
    }

    if (match[0].data == nullptr || match[0].length == 0) {
        std::cerr << "[FAIL] test_re_path_single_group: expected non-empty capture for group 0.\n";
        return false;
    }
    if (std::strncmp("ciao", match[0].data, (size_t)match[0].length) != 0) {
        std::cerr << "[FAIL] test_re_path_single_group: capture should be 'ciao'.\n";
        return false;
    }
    return true;
}

static bool test_re_path_two_groups() {
    // Test: using cre2_new + cre2_partial_match_re with two captures and two matches
    const char *pattern = "(ciao) (salut)";
    const char *text = "ciao salut";

    cre2_regexp_t *rex = cre2_new(pattern, (int)std::strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "[FAIL] test_re_path_two_groups: cre2_new failed.\n";
        return false;
    }

    cre2_string_t input;
    input.data = text;
    input.length = (size_t)std::strlen(text);

    int nmatch = 2;
    std::vector<cre2_string_t> match(nmatch);

    int result = cre2_partial_match_re(rex, &input, match.data(), nmatch);
    cre2_delete(rex);
    if (result == 0) {
        std::cerr << "[FAIL] test_re_path_two_groups: expected success (non-zero).\n";
        return false;
    }

    if (match[0].data == nullptr || match[0].length == 0 ||
        match[1].data == nullptr || match[1].length == 0) {
        std::cerr << "[FAIL] test_re_path_two_groups: captured groups should be non-empty.\n";
        return false;
    }

    if (std::strncmp("ciao", match[0].data, (size_t)match[0].length) != 0) {
        std::cerr << "[FAIL] test_re_path_two_groups: first capture should be 'ciao'.\n";
        return false;
    }
    // Second capture should be "salut"
    if (std::strncmp("salut", match[1].data, (size_t)match[1].length) != 0) {
        std::cerr << "[FAIL] test_re_path_two_groups: second capture should be 'salut'.\n";
        return false;
    }
    return true;
}

int main() {
    // Run all tests
    int total = 0;
    int failed = 0;

    std::cout << "Starting CRE2 partial-match unit tests (C API via C++ harness)..." << std::endl;

    auto run = [&](const char* name, bool (*fn)()) {
        total++;
        bool ok = fn();
        if (ok) {
            std::cout << "[PASS] " << name << std::endl;
        } else {
            failed++;
            std::cout << "[FAIL] " << name << std::endl;
        }
    };

    run("test_basic_no_parenthesis", test_basic_no_parenthesis);
    run("test_single_group", test_single_group);
    run("test_two_groups_two_matches", test_two_groups_two_matches);
    run("test_failure_no_match_no_capture", test_failure_no_match_no_capture);
    run("test_wrong_regexp_specification", test_wrong_regexp_specification);
    run("test_re_path_single_group", test_re_path_single_group);
    run("test_re_path_two_groups", test_re_path_two_groups);

    std::cout << "Test summary: " << total << " tests, " << failed << " failures." << std::endl;

    return (failed == 0) ? 0 : 1;
}