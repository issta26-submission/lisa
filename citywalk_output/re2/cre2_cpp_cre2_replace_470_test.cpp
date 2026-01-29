// Simple C++11 unit tests for cre2_replace (no Google Test)
// Test harness is self-contained and uses manual EXPECT-like checks.
// This file exercises cre2_replace function from cre2.cpp by feeding known
// inputs and verifying outputs, including memory updates.

// Steps followed (as per instructions):
// - Step 1: Analyze cre2_replace dependencies: uses std::string, RE2::Replace,
//           cre2_string_t (text_and_target, rewrite), and memory management.
// - Step 2: Generate tests for basic success, no-match, and replacement with empty rewrite.
// - Step 3: Use domain knowledge to craft robust cases and ensure memory management.
// Note: We intentionally cover both true (replacement occurred) and false (no match) branches.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Global failure counter for non-terminating test reporting
static int g_failures = 0;

// Lightweight non-terminating expectations (do not abort on failure)
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        ++g_failures;
        std::cerr << "EXPECT_TRUE FAILED: " << msg << std::endl;
    }
}

static void expect_int_eq(int expected, int actual, const char* msg) {
    if (expected != actual) {
        ++g_failures;
        std::cerr << "EXPECT_EQ INT FAILED: " << msg
                  << " | expected: " << expected << " actual: " << actual << std::endl;
    }
}

static void expect_str_eq(const std::string& a, const std::string& b, const char* msg) {
    if (a != b) {
        ++g_failures;
        std::cerr << "EXPECT_EQ STR FAILED: " << msg
                  << " | expected: \"" << b << "\" actual: \"" << a << "\"" << std::endl;
    }
}

// Helper to build cre2_string_t from std::string (data must be modifiable if the test intends to free/alter)
static cre2_string_t build_cre2_string(const std::string& s) {
    cre2_string_t st;
    // Allocate modifiable memory for the test
    char* buf = (char*)std::malloc(s.size() + 1);
    if (buf) {
        std::memcpy(buf, s.data(), s.size());
        buf[s.size()] = '\0';
        st.data = buf;
        st.length = static_cast<int>(s.size());
    } else {
        st.data = nullptr;
        st.length = 0;
    }
    return st;
}

// Cleanup helper
static void free_cre2_string(cre2_string_t& s) {
    if (s.data) {
        std::free(s.data);
        s.data = nullptr;
        s.length = 0;
    }
}

// Test 1: Simple replacement - ensure content changes and length updates, return value is 1
// Input: pattern = "cat", text = "the cat sat", rewrite = "dog"
// Expected: "the dog sat", length updated to new length, return 1
static void test_cre2_replace_simple_replacement() {
    // Prepare input
    cre2_string_t text_and_target = build_cre2_string("the cat sat");
    cre2_string_t rewrite = build_cre2_string("dog");

    const char* pattern = "cat";

    // Act
    int result = cre2_replace(pattern, &text_and_target, &rewrite);

    // Assert
    std::string updated(reinterpret_cast<char*>(text_and_target.data), text_and_target.length);
    expect_int_eq(1, result, "Replacement should return 1 when a match is found.");
    expect_str_eq(updated, "the dog sat", "Simple replacement should yield 'the dog sat'.");

    // Cleanup
    free_cre2_string(text_and_target);
    free_cre2_string(rewrite);
}

// Test 2: No-match scenario - ensure content remains unchanged, return value is 0
// Input: pattern = "zebra", text = "the cat sat", rewrite = "dog"
// Expected: unchanged "the cat sat", length unchanged, return 0
static void test_cre2_replace_no_match() {
    // Prepare input
    cre2_string_t text_and_target = build_cre2_string("the cat sat");
    cre2_string_t rewrite = build_cre2_string("dog");

    const char* pattern = "zebra";

    // Act
    int result = cre2_replace(pattern, &text_and_target, &rewrite);

    // Assert
    std::string updated(reinterpret_cast<char*>(text_and_target.data), text_and_target.length);
    expect_int_eq(0, result, "Replacement should return 0 when no match is found.");
    expect_str_eq(updated, "the cat sat", "No-match should leave text unchanged.");

    // Cleanup
    free_cre2_string(text_and_target);
    free_cre2_string(rewrite);
}

// Test 3: Replacement with empty rewrite - delete matched portion
// Input: pattern = "cd", text = "abcdef", rewrite = "" (empty)
// Expected: "abef", return 1
static void test_cre2_replace_empty_rewrite() {
    // Prepare input
    cre2_string_t text_and_target = build_cre2_string("abcdef");
    cre2_string_t rewrite = build_cre2_string(""); // empty rewrite

    const char* pattern = "cd";

    // Act
    int result = cre2_replace(pattern, &text_and_target, &rewrite);

    // Assert
    std::string updated(reinterpret_cast<char*>(text_and_target.data), text_and_target.length);
    expect_int_eq(1, result, "Replacement with empty rewrite should return 1.");
    expect_str_eq(updated, "abef", "Replacement with empty rewrite should yield 'abef'.");

    // Cleanup
    free_cre2_string(text_and_target);
    free_cre2_string(rewrite);
}

// Test 4: Ensure only first occurrence is replaced (RE2::Replace replaces first match by default)
// Input: pattern = "cat", text = "cat and cat", rewrite = "dog"
// Expected: "dog and cat"
static void test_cre2_replace_first_only() {
    // Prepare input
    cre2_string_t text_and_target = build_cre2_string("cat and cat");
    cre2_string_t rewrite = build_cre2_string("dog");

    const char* pattern = "cat";

    // Act
    int result = cre2_replace(pattern, &text_and_target, &rewrite);

    // Assert
    std::string updated(reinterpret_cast<char*>(text_and_target.data), text_and_target.length);
    expect_int_eq(1, result, "Replacement should return 1 for first-match replacement.");
    expect_str_eq(updated, "dog and cat", "Only the first occurrence should be replaced.");

    // Cleanup
    free_cre2_string(text_and_target);
    free_cre2_string(rewrite);
}

// Run all tests and print summary
int main() {
    test_cre2_replace_simple_replacement();
    test_cre2_replace_no_match();
    test_cre2_replace_empty_rewrite();
    test_cre2_replace_first_only();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
    }

    // Return nonzero if any test failed to aid automated tooling
    return g_failures;
}