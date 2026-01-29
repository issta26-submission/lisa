// Minimal C++11 unit test suite for cre2_global_replace_re (cre2.cpp)
// This test suite does not rely on GTest; it uses lightweight non-terminating checks.
// The tests are designed to cover true/false branches, regex and literal patterns,
// and memory handling of cre2_string_t inputs/outputs.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include CRE2 headers as per FOCAL_CLASS_DEP

// Forward declaration of function under test (as provided by cre2.cpp)
extern "C" int cre2_global_replace_re (cre2_regexp_t * rex, cre2_string_t * text_and_target, cre2_string_t * rewrite);

// Simple global test failure counter
static int g_test_failures = 0;

// Non-terminating assertion helper for string equality
static void assert_eq_string(const std::string& expected, const cre2_string_t& actual) {
    std::string actual_str(actual.data, actual.length);
    if (expected != actual_str) {
        std::cerr << "TEST FAILURE: expected \"" << expected
                  << "\", got \"" << actual_str << "\"\n";
        ++g_test_failures;
    }
}

// Non-terminating assertion helper for integer equality
static void assert_eq_int(int expected, int actual, const char* test_name) {
    if (expected != actual) {
        std::cerr << "TEST FAILURE: " << test_name
                  << " - expected int " << expected
                  << ", got " << actual << "\n";
        ++g_test_failures;
    }
}

// Helper to create a cre2_string_t from a C string (with given length)
static cre2_string_t make_cre2_string(const char* s, int len) {
    cre2_string_t st;
    st.data = const_cast<char*>(s);
    st.length = len;
    return st;
}

// Test 1: Basic literal replacement using global replace
static void test_cre2_global_replace_re_basic() {
    const char* pattern = "cat";
    cre2_regexp_t* rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "TEST FAILURE: could not compile regex pattern '" << pattern << "'\n";
        ++g_test_failures;
        return;
    }

    // Text to operate on
    const char* text_c = "the cat sat on the cat";
    char* text_buf = strdup(text_c);
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    // Rewrite string
    const char* rewrite_c = "dog";
    cre2_string_t rewrite = { (char*)rewrite_c, (int)strlen(rewrite_c) };

    int ret = cre2_global_replace_re(rex, &text, &rewrite);

    // Expected: "the dog sat on the dog"
    std::string expected = "the dog sat on the dog";
    assert_eq_int(2, ret, "basic_replacement_retval");
    assert_eq_string(expected, text);

    // Cleanup
    free(text.data);
    cre2_delete(rex);
}

// Test 2: No-match scenario should return 0 and keep text unchanged
static void test_cre2_global_replace_re_no_match() {
    const char* pattern = "zzz";
    cre2_regexp_t* rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "TEST FAILURE: could not compile regex pattern '" << pattern << "'\n";
        ++g_test_failures;
        return;
    }

    const char* text_c = "abc abc";
    char* text_buf = strdup(text_c);
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    const char* rewrite_c = "X";
    cre2_string_t rewrite = { (char*)rewrite_c, (int)strlen(rewrite_c) };

    int ret = cre2_global_replace_re(rex, &text, &rewrite);

    std::string expected = "abc abc";
    assert_eq_int(0, ret, "no_match_retval");
    assert_eq_string(expected, text);

    free(text.data);
    cre2_delete(rex);
}

// Test 3: Empty input text should still allocate a buffer and return 0
static void test_cre2_global_replace_re_empty_input() {
    const char* pattern = "a";
    cre2_regexp_t* rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "TEST FAILURE: could not compile regex pattern '" << pattern << "'\n";
        ++g_test_failures;
        return;
    }

    // Empty text
    const char* text_c = "";
    char* text_buf = strdup(text_c);
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    const char* rewrite_c = "X";
    cre2_string_t rewrite = { (char*)rewrite_c, (int)strlen(rewrite_c) };

    int ret = cre2_global_replace_re(rex, &text, &rewrite);

    // Expect empty result with length 0
    assert_eq_int(0, ret, "empty_input_retval");
    std::string expected = "";
    assert_eq_string(expected, text);

    free(text.data);
    cre2_delete(rex);
}

// Test 4: Regex pattern with a+b+ matching multiple times
static void test_cre2_global_replace_re_regex_repeats() {
    const char* pattern = "a+b+";
    cre2_regexp_t* rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "TEST FAILURE: could not compile regex pattern '" << pattern << "'\n";
        ++g_test_failures;
        return;
    }

    const char* text_c = "aaabbbaaabbb";
    char* text_buf = strdup(text_c);
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    const char* rewrite_c = "Z";
    cre2_string_t rewrite = { (char*)rewrite_c, (int)strlen(rewrite_c) };

    int ret = cre2_global_replace_re(rex, &text, &rewrite);

    // Expected: "ZZ"
    std::string expected = "ZZ";
    assert_eq_int(2, ret, "regex_repeats_retval");
    assert_eq_string(expected, text);

    free(text.data);
    cre2_delete(rex);
}

// Test 5: Regex with alternatives (cat|dog) replacing three occurrences
static void test_cre2_global_replace_re_alternatives() {
    const char* pattern = "cat|dog";
    cre2_regexp_t* rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (rex == NULL) {
        std::cerr << "TEST FAILURE: could not compile regex pattern '" << pattern << "'\n";
        ++g_test_failures;
        return;
    }

    const char* text_c = "cat and dog; cat";
    char* text_buf = strdup(text_c);
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    const char* rewrite_c = "X";
    cre2_string_t rewrite = { (char*)rewrite_c, (int)strlen(rewrite_c) };

    int ret = cre2_global_replace_re(rex, &text, &rewrite);

    // Expected: "X and X; X"
    std::string expected = "X and X; X";
    assert_eq_int(3, ret, "alternatives_retval");
    assert_eq_string(expected, text);

    free(text.data);
    cre2_delete(rex);
}

// Main: run all tests
int main() {
    test_cre2_global_replace_re_basic();
    test_cre2_global_replace_re_no_match();
    test_cre2_global_replace_re_empty_input();
    test_cre2_global_replace_re_regex_repeats();
    test_cre2_global_replace_re_alternatives();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED\n";
        return 1;
    }
}