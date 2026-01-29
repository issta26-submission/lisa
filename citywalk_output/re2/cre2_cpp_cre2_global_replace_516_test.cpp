/*
  Lightweight unit test suite for cre2_global_replace (no GTest)
  - Uses a small in-house assertion approach to keep test suite self-contained.
  - Exercises multiple scenarios to increase coverage for cre2_global_replace.
  - Assumes CRE2 library is available and cre2.h provides cre2_string_t and cre2_global_replace.
  - Test design follows the provided step-by-step reasoning and domain knowledge.
*/

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Global container to accumulate failure messages without terminating tests early
static std::vector<std::string> g_failures;

// Helper to convert a C-style string with given length to std::string
static std::string make_string(const char* data, int length) {
    if (data == nullptr || length < 0) return std::string();
    return std::string(data, length);
}

// Test 1: Basic single-character replacement with same length (b -> X)
static bool test_basic_single_char_replacement() {
    bool ok = true;

    // text: "abcabc" (length=6), rewrite: "X" (length=1)
    cre2_string_t text;
    cre2_string_t rewrite;

    text.data = const_cast<char*>("abcabc"); // string literal; non-owning
    text.length = 6;

    rewrite.data = const_cast<char*>("X"); // single-character rewrite
    rewrite.length = 1;

    int ret = cre2_global_replace("b", &text, &rewrite);

    // Expect 2 replacements: "aXcXaXc"
    if (ret != 2) {
        g_failures.push_back("test_basic_single_char_replacement: expected 2 replacements, got " + std::to_string(ret));
        ok = false;
    }

    if (text.length != 6) {
        g_failures.push_back("test_basic_single_char_replacement: expected length 6, got " + std::to_string(text.length));
        ok = false;
    }

    std::string result = make_string(text.data, text.length);
    if (result != "aXcXaXc") {
        g_failures.push_back("test_basic_single_char_replacement: expected 'aXcXaXc', got '" + result + "'");
        ok = false;
    }

    // Free the newly allocated buffer to avoid leaks
    if (text.data) {
        free(text.data);
        text.data = nullptr;
    }

    return ok;
}

// Test 2: No matches found (ret should be 0; content unchanged)
static bool test_no_match() {
    bool ok = true;

    // text: "hello" (length=5), rewrite: "X" (length=1)
    cre2_string_t text;
    cre2_string_t rewrite;

    text.data = const_cast<char*>("hello");
    text.length = 5;

    rewrite.data = const_cast<char*>("X");
    rewrite.length = 1;

    int ret = cre2_global_replace("z", &text, &rewrite);

    // Expect 0 replacements and unchanged content
    if (ret != 0) {
        g_failures.push_back("test_no_match: expected 0 replacements, got " + std::to_string(ret));
        ok = false;
    }

    if (text.length != 5) {
        g_failures.push_back("test_no_match: expected length 5, got " + std::to_string(text.length));
        ok = false;
    }

    std::string result = make_string(text.data, text.length);
    if (result != "hello") {
        g_failures.push_back("test_no_match: expected 'hello', got '" + result + "'");
        ok = false;
    }

    if (text.data) {
        free(text.data);
        text.data = nullptr;
    }

    return ok;
}

// Test 3: Replacement expands length (a -> zz) over "aaa" -> "zzzzzz"
static bool test_expanding_rewrite() {
    bool ok = true;

    cre2_string_t text;
    cre2_string_t rewrite;

    text.data = const_cast<char*>("aaa");
    text.length = 3;

    rewrite.data = const_cast<char*>("zz");
    rewrite.length = 2;

    int ret = cre2_global_replace("a", &text, &rewrite);

    // Expect 3 replacements
    if (ret != 3) {
        g_failures.push_back("test_expanding_rewrite: expected 3 replacements, got " + std::to_string(ret));
        ok = false;
    }

    if (text.length != 6) {
        g_failures.push_back("test_expanding_rewrite: expected length 6, got " + std::to_string(text.length));
        ok = false;
    }

    std::string result = make_string(text.data, text.length);
    if (result != "zzzzzz") {
        g_failures.push_back("test_expanding_rewrite: expected 'zzzzzz', got '" + result + "'");
        ok = false;
    }

    if (text.data) {
        free(text.data);
        text.data = nullptr;
    }

    return ok;
}

// Test 4: Multiple occurrences with multi-character rewrite (ab -> X)
static bool test_multiple_occurrences_multi_char_rewrite() {
    bool ok = true;

    cre2_string_t text;
    cre2_string_t rewrite;

    text.data = const_cast<char*>("abab");
    text.length = 4;

    rewrite.data = const_cast<char*>("X");
    rewrite.length = 1;

    int ret = cre2_global_replace("ab", &text, &rewrite);

    // Expect 2 replacements
    if (ret != 2) {
        g_failures.push_back("test_multiple_occurrences_multi_char_rewrite: expected 2 replacements, got " + std::to_string(ret));
        ok = false;
    }

    if (text.length != 2) {
        g_failures.push_back("test_multiple_occurrences_multi_char_rewrite: expected length 2, got " + std::to_string(text.length));
        ok = false;
    }

    std::string result = make_string(text.data, text.length);
    if (result != "XX") {
        g_failures.push_back("test_multiple_occurrences_multi_char_rewrite: expected 'XX', got '" + result + "'");
        ok = false;
    }

    if (text.data) {
        free(text.data);
        text.data = nullptr;
    }

    return ok;
}

// Test 5: Multi-character rewrite increasing length (aa -> bbbb)
static bool test_multi_char_rewrite_increase() {
    bool ok = true;

    cre2_string_t text;
    cre2_string_t rewrite;

    text.data = const_cast<char*>("aa");
    text.length = 2;

    rewrite.data = const_cast<char*>("bb");
    rewrite.length = 2;

    int ret = cre2_global_replace("a", &text, &rewrite);

    // Expect 2 replacements
    if (ret != 2) {
        g_failures.push_back("test_multi_char_rewrite_increase: expected 2 replacements, got " + std::to_string(ret));
        ok = false;
    }

    if (text.length != 4) {
        g_failures.push_back("test_multi_char_rewrite_increase: expected length 4, got " + std::to_string(text.length));
        ok = false;
    }

    std::string result = make_string(text.data, text.length);
    if (result != "bbbb") {
        g_failures.push_back("test_multi_char_rewrite_increase: expected 'bbbb', got '" + result + "'");
        ok = false;
    }

    if (text.data) {
        free(text.data);
        text.data = nullptr;
    }

    return ok;
}

// Runner
int main() {
    // Clear any previous failures (in case of reruns)
    g_failures.clear();

    bool pass1 = test_basic_single_char_replacement();
    bool pass2 = test_no_match();
    bool pass3 = test_expanding_rewrite();
    bool pass4 = test_multiple_occurrences_multi_char_rewrite();
    bool pass5 = test_multi_char_rewrite_increase();

    // Aggregate results
    bool all_passed = pass1 && pass2 && pass3 && pass4 && pass5;

    if (all_passed && g_failures.empty()) {
        std::cout << "[OK] All tests passed for cre2_global_replace." << std::endl;
        return 0;
    } else {
        std::cout << "[FAILED] cre2_global_replace test suite results:" << std::endl;
        // Report detailed failures
        for (const auto &m : g_failures) {
            std::cout << "  - " << m << std::endl;
        }
        // Even if some tests pass, we report failure if any test failed
        return 1;
    }
}