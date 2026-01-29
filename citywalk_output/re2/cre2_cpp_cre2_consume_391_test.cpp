// Unit tests for cre2_consume (focal method) in cre2.cpp
// Note: This test suite uses a lightweight, self-contained test harness
// (no Google Test). It targets the cre2 C API as exposed by cre2.h and
// exercises the behavior of the ConsumeN variant by using a simple
// pattern and input. The goal is to validate true/false branches and
// side effects on input text, while keeping tests executable under C++11.
//
// Assumptions based on the focal code snippet:
// - cre2_consume has a signature similar to:
//     int cre2_consume(cre2_regexp_t *re, cre2_string_t *text, int n);
// - It attempts to match the pattern at the start of text and, if a match
//   is found whose consumed portion length is <= n, it mutates text to
//   refer to the remaining portion after the consumed match and returns 1.
// - If no match is found or the match length exceeds n, it returns 0 and may
//   still mutate text to reflect the original content (implementation dependent).
// - cre2_string_t contains fields: char* data; int length;
// - Memory management is the caller's responsibility to free allocated memory.
//
// The tests below attempt to verify:
// 1) A successful consume with exact N length and correct mutation of text.
// 2) A failed consume due to N too small (no mutation and return 0).
// 3) A no-match scenario (return 0 and input remains as-is).

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the CRE2 C API (provided by the project)

static int g_total = 0;
static int g_pass = 0;
static int g_fail = 0;

// Lightweight assertion helpers that log without terminating the test run
#define TEST_ASSERT(cond, name) do { if (cond) { ++g_pass; } else { ++g_fail; fprintf(stderr, "Test failed: %s\n", name); } ++g_total; } while(0)
#define TEST_PASS(name) do { ++g_pass; ++g_total; printf("[PASS] %s\n", name); } while(0)
#define TEST_FAIL(name) do { ++g_fail; ++g_total; fprintf(stderr, "[FAIL] %s\n", name); } while(0)

// Helper to safely copy the current text content for comparison
static std::string read_text_content(const cre2_string_t &text) {
    if (text.data && text.length >= 0) {
        return std::string(text.data, text.length);
    }
    return std::string();
}

// Test 1: Basic consume scenario with exact N and straightforward digits pattern
bool test_cre2_consume_basic() {
    // Prepare pattern and input
    const char *pattern = "([0-9]+)"; // capture one or more digits
    int pattern_len = (int)strlen(pattern);

    // Create options and regex
    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re == nullptr) {
        cre2_opt_delete(opt);
        return false;
    }

    // Input text "123abc" -> we will consume "123" (length 3) with N = 3
    char *buffer = (char*)malloc(256);
    strcpy(buffer, "123abc"); // ensure null-terminated writable buffer

    cre2_string_t text;
    text.data = buffer;
    text.length = (int)strlen(buffer);

    int N = 3;
    int ret = cre2_consume(re, &text, N);

    // Capture the content after consumption for verification
    std::string after_consumption = read_text_content(text);

    // Expected: a successful consume (ret == 1) and remaining text is "abc"
    bool ok = (ret == 1) && (after_consumption == "abc");

    // Cleanup
    free(buffer);               // if text.data points inside this buffer, it's safe to free after copying the content above
    cre2_delete(re);
    cre2_opt_delete(opt);

    return ok;
}

// Test 2: Consume should fail when N is too small to cover the match length
bool test_cre2_consume_n_too_small() {
    // Pattern the same as Test 1
    const char *pattern = "([0-9]+)";
    int pattern_len = (int)strlen(pattern);

    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re == nullptr) {
        cre2_opt_delete(opt);
        return false;
    }

    // Input text "123abc" and N = 2 (too small to consume the matched "123")
    char *buffer = (char*)malloc(256);
    strcpy(buffer, "123abc");

    cre2_string_t text;
    text.data = buffer;
    text.length = (int)strlen(buffer;

    // NOTE: The length computation above intentionally uses the current string length.
    int N = 2;
    int ret = cre2_consume(re, &text, N);

    // Read content after call
    std::string after_consumption = read_text_content(text);

    // Expected: ret == 0 (no consume), but content may or may not have been mutated
    // We conservatively expect the content to still reflect the original string "123abc".
    bool ok = (ret == 0) && (after_consumption == "123abc");

    // Cleanup
    free(buffer);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return ok;
}

// Test 3: No-match scenario should return 0 and input should remain as-is
bool test_cre2_consume_no_match() {
    // Pattern that won't match the input
    const char *pattern = "(XYZ)";
    int pattern_len = (int)strlen(pattern);

    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re == nullptr) {
        cre2_opt_delete(opt);
        return false;
    }

    // Input text "abcdef" (no digits or XYZ)
    char *buffer = (char*)malloc(256);
    strcpy(buffer, "abcdef");

    cre2_string_t text;
    text.data = buffer;
    text.length = (int)strlen(buffer);

    int N = 5;
    int ret = cre2_consume(re, &text, N);

    // Read content after call
    std::string after_consumption = read_text_content(text);

    // Expected: ret == 0 and input content unchanged
    bool ok = (ret == 0) && (after_consumption == "abcdef");

    // Cleanup
    free(buffer);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return ok;
}

// Simple test runner
int main() {
    printf("Running unit tests for cre2_consume (focal method)...\n");

    if (test_cre2_consume_basic()) {
        TEST_PASS("test_cre2_consume_basic");
    } else {
        TEST_FAIL("test_cre2_consume_basic");
    }

    if (test_cre2_consume_n_too_small()) {
        TEST_PASS("test_cre2_consume_n_too_small");
    } else {
        TEST_FAIL("test_cre2_consume_n_too_small");
    }

    if (test_cre2_consume_no_match()) {
        TEST_PASS("test_cre2_consume_no_match");
    } else {
        TEST_FAIL("test_cre2_consume_no_match");
    }

    // Summary
    printf("Tests run: %d, Passed: %d, Failed: %d\n", g_total, g_pass, g_fail);
    if (g_fail > 0) {
        return 1; // non-zero exit code indicates test failures
    }
    return 0;
}