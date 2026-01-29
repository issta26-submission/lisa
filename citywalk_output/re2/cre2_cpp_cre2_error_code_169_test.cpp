// Unit tests for cre2_error_code (wrapper around RE2::error_code)
// This test suite is handcrafted (no GoogleTest) and uses lightweight
// non-terminating assertions to exercise the focal method and related paths.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the C wrapper for CRE2 (as used by the focal function)

// Lightweight non-terminating assertion utilities
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_test_failures; \
        std::cerr << "[TEST_FAIL] " << (msg) << " (Condition: " #cond ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        ++g_test_failures; \
        std::cerr << "[TEST_FAIL] " << (msg) << " (expected: " << (b) \
                  << ", actual: " << (a) << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_NEQ(a, b, msg) do { \
    if((a) == (b)) { \
        ++g_test_failures; \
        std::cerr << "[TEST_FAIL] " << (msg) << " (unexpected: " << (a) << ")" \
                  << std::endl; \
    } \
} while(0)

// Helper to compute string length for patterns/texts
static int len(const char* s) { return s ? static_cast<int>(std::strlen(s)) : 0; }

// Test 1: cre2_error_code should be 0 for a valid compiled pattern
static void test_cre2_error_code_valid_pattern() {
    // Prepare options (disable logerrors to keep test output clean)
    cre2_options_t *opt = cre2_opt_new();
    EXPECT_TRUE(opt != nullptr, "cre2_opt_new() should return a non-null pointer");

    // Ensure option API is usable
    if (opt) {
        cre2_opt_set_log_errors(opt, 0);

        // Compile a simple valid pattern
        const char *pattern = "abc";
        int pattern_len = len(pattern);

        cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
        EXPECT_TRUE(re != nullptr, "cre2_new() should succeed for a valid pattern");

        if (re) {
            // The focal method: cre2_error_code should be 0 (no error)
            int code = cre2_error_code(re);
            EXPECT_EQ(code, 0, "cre2_error_code() should be 0 for a valid compiled pattern");

            // Cleanup
            cre2_delete(re);
        }

        cre2_opt_delete(opt);
    }
}

// Test 2: cre2_easy_match path exercises cre2_error_code usage when pattern is valid
static void test_cre2_easy_match_path_valid_and_no_match() {
    // Case A: pattern that exists in text -> expect a successful match (return 1)
    {
        const char *pattern = "hello";
        int pattern_len = len(pattern);
        const char *text = "say hello world";
        int text_len = len(text);

        cre2_string_t match;
        std::memset(&match, 0, sizeof(match));
        int nmatch = 1;

        int result = cre2_easy_match(pattern, pattern_len, text, text_len, &match, nmatch);
        EXPECT_EQ(result, 1, "cre2_easy_match should return 1 when a match exists");
    }

    // Case B: pattern not present in text -> expect no match (return 0)
    {
        const char *pattern = "absent";
        int pattern_len = len(pattern);
        const char *text = "hello world";
        int text_len = len(text);

        cre2_string_t match;
        std::memset(&match, 0, sizeof(match));
        int nmatch = 1;

        int result = cre2_easy_match(pattern, pattern_len, text, text_len, &match, nmatch);
        EXPECT_EQ(result, 0, "cre2_easy_match should return 0 when no match is found");
    }
}

// Test 3: Sanity of option and allocation flow used by cre2_error_code path
// This ensures resources are properly allocated and deallocated in normal use.
static void test_resource_management_flow() {
    cre2_options_t *opt = cre2_opt_new();
    EXPECT_TRUE(opt != nullptr, "cre2_opt_new() should not return null");
    if (!opt) return;

    cre2_opt_set_log_errors(opt, 0);

    const char *pattern = "(?:ab)c"; // a slightly more complex, yet valid pattern
    int pattern_len = len(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    EXPECT_TRUE(re != nullptr, "cre2_new() should succeed for a valid pattern in resource test");
    if (re) {
        int code = cre2_error_code(re);
        EXPECT_EQ(code, 0, "cre2_error_code() should be 0 for a valid pattern in resource test");

        cre2_delete(re);
        re = nullptr;
    }

    cre2_opt_delete(opt);
}

// Main test harness
int main() {
    std::printf("Starting CRE2 error_code unit tests...\n");

    test_cre2_error_code_valid_pattern();
    test_cre2_easy_match_path_valid_and_no_match();
    test_resource_management_flow();

    if (g_test_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::fprintf(stderr, "%d TEST FAILURE(S) DETECTED\n", g_test_failures);
        return 1;
    }
}