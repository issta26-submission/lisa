// Test suite for cre2_easy_match (C++11, no GoogleTest)
// Purpose: exercise cre2_easy_match with typical and edge cases
// Notes:
// - This test relies on the CRE2 C API exposed in cre2.h
// - No GTest; a lightweight test harness is implemented
// - We focus on true/false branches of conditional logic inside cre2_easy_match
// - We avoid private members and static internals; only the public cre2_easy_match is exercised

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static std::vector<std::string> g_failures;

#define T_ASSERT(cond, msg) do { if(!(cond)) { g_failures.push_back(msg); return false; } } while(0)

// Helper to convert a cre2_string_t to std::string (ptr may be NULL for zero-length)
static std::string to_std_string(const cre2_string_t &s) {
    if (s.ptr == nullptr || s.len <= 0) return std::string();
    return std::string(s.ptr, s.ptr + s.len);
}

// Test 1: Basic positive match - pattern "abc" in text "xxabcxx" -> should match "abc"
static bool test_basic_match() {
    const char *pattern = "abc";
    int pattern_len = 3;
    const char *text = "xxabcxx";
    int text_len = 7;

    cre2_string_t match[2];
    int nmatch = 2;

    int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, nmatch);

    T_ASSERT(rv == 1, "test_basic_match: expected rv == 1 for a successful match");

    std::string m0 = to_std_string(match[0]);
    T_ASSERT(m0 == "abc" && match[0].len == 3, "test_basic_match: expected full match to be 'abc' with length 3");

    // Optional capture group 1 not used here; ensure function returns valid data when nmatch >= 2
    // Since pattern has no capturing group, match[1] should be empty
    if (nmatch >= 2) {
        std::string m1 = to_std_string(match[1]);
        T_ASSERT(m1 == "" , "test_basic_match: expected group 1 to be empty for pattern without capturing groups");
    }

    return true;
}

// Test 2: No-match path - pattern "def" in text "abc" -> should return 0
static bool test_no_match() {
    const char *pattern = "def";
    int pattern_len = 3;
    const char *text = "abc";
    int text_len = 3;

    cre2_string_t match[1];
    int nmatch = 1;

    int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, nmatch);
    T_ASSERT(rv == 0, "test_no_match: expected rv == 0 when there is no match");
    return true;
}

// Test 3: Invalid pattern that should cause rex creation to fail or error code non-zero
// Using an obviously invalid regex: unclosed parenthesis
static bool test_invalid_regex_syntax() {
    const char *pattern = "("; // invalid syntax
    int pattern_len = 1;
    const char *text = "anything";
    int text_len = 8;

    cre2_string_t match[1];
    int nmatch = 1;

    int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, nmatch);
    // CRE2 behavior may either return 2 (wrong regexp) or 0/1 depending on internal error handling.
    // Our expectation is that it does not crash and returns 2 to indicate bad regex.
    T_ASSERT(rv == 2, "test_invalid_regex_syntax: expected rv == 2 for invalid regex syntax");
    return true;
}

// Test 4: Valid capturing groups - pattern "(abc)" should populate full match and group 1
static bool test_capture_group() {
    const char *pattern = "(abc)";
    int pattern_len = 5;
    const char *text = "zzzabczzz";
    int text_len = 9;

    cre2_string_t match[2];
    int nmatch = 2;

    int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, nmatch);
    T_ASSERT(rv == 1, "test_capture_group: expected rv == 1 for a successful match with capture");

    std::string full = to_std_string(match[0]);
    std::string group1 = to_std_string(match[1]);

    T_ASSERT(full == "abc" && match[0].len == 3, "test_capture_group: full match should be 'abc' of length 3");
    T_ASSERT(group1 == "abc" && match[1].len == 3, "test_capture_group: capturing group 1 should be 'abc' of length 3");

    return true;
}

// Test 5: Empty text - pattern "a" with zero-length text should yield no match (rv == 0)
static bool test_empty_text() {
    const char *pattern = "a";
    int pattern_len = 1;
    const char *text = "";
    int text_len = 0;

    cre2_string_t match[1];
    int nmatch = 1;

    int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, nmatch);
    T_ASSERT(rv == 0, "test_empty_text: expected rv == 0 when text is empty");
    return true;
}

// Test 6: Multiple test calls to ensure static-like behavior is not required; ensure cleanup paths
static bool test_multiple_calls_cleanup() {
    // First call succeeds
    {
        const char *pattern = "abc";
        int pattern_len = 3;
        const char *text = "abc";
        int text_len = 3;
        cre2_string_t match[2];
        int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, 2);
        T_ASSERT(rv == 1, "test_multiple_calls_cleanup: first call should succeed");
    }
    // Second call with different inputs to ensure there is no side-effect bleed-through
    {
        const char *pattern = "def";
        int pattern_len = 3;
        const char *text = "abcdef";
        int text_len = 6;
        cre2_string_t match[2];
        int rv = cre2_easy_match(pattern, pattern_len, text, text_len, match, 2);
        // 'def' appears starting at index 3 in "abcdef" => should match
        T_ASSERT(rv == 1, "test_multiple_calls_cleanup: second call should also succeed and find a match");
        std::string m0 = to_std_string(match[0]);
        T_ASSERT(m0 == "def" && match[0].len == 3, "test_multiple_calls_cleanup: captured full match should be 'def'");
    }
    return true;
}

// Helper to run all tests
typedef bool (*TestFn)();
struct TestCase { const char *name; TestFn fn; };

static TestCase g_tests[] = {
    {"BasicMatch", &test_basic_match},
    {"NoMatch", &test_no_match},
    {"InvalidRegexSyntax", &test_invalid_regex_syntax},
    {"CaptureGroup", &test_capture_group},
    {"EmptyText", &test_empty_text},
    {"MultipleCallsCleanup", &test_multiple_calls_cleanup},
};

int main() {
    int total = sizeof(g_tests) / sizeof(g_tests[0]);
    int passed = 0;

    std::cerr << "Running " << total << " tests for cre2_easy_match...\n";

    for (size_t i = 0; i < total; ++i) {
        const char* tname = g_tests[i].name;
        bool result = g_tests[i].fn();
        if (result) {
            ++passed;
            std::cerr << "[PASS] " << tname << "\n";
        } else {
            std::cerr << "[FAIL] " << tname << "\n";
            // Print captured failures (if any) from global log
        }
    }

    if (!g_failures.empty()) {
        std::cerr << "\nFailure details:\n";
        for (const auto &s : g_failures) {
            std::cerr << "  - " << s << "\n";
        }
    }

    std::cerr << "\nSummary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}