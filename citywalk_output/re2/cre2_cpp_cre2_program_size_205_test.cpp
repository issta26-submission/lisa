// Test suite for cre2_program_size (C API wrapper around RE2)
// This test does not use Google Test; it uses lightweight, non-terminating
// assertions and a simple main function to run tests as required.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Lightweight test failure counter
static int g_test_failures = 0;

// Small non-terminating assertion helpers
#define EXPECT_EQ(a, b) do { \
    auto _a = (a); auto _b = (b); \
    if (!(_a == _b)) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " != " << #b \
                  << " (" << _a << " vs " << _b << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: condition " << #cond \
                  << " is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while (0)

// Helper: run cre2_program_size and compare to RE2::ProgramSize() for a given pattern
static void test_program_size_matches_for_pattern(const char* pattern) {
    // Prepare C API RE2 wrapper for the same pattern
    size_t len = std::strlen(pattern);
    cre2_options_t* opt = cre2_opt_new();
    cre2_regexp_t* re = cre2_new(pattern, (int)len, opt);

    // Call the focal function
    size_t size_from_cre2 = cre2_program_size(re);

    // Compute expected value using the underlying C++ RE2
    re2::RE2 re_cpp(pattern);
    size_t expected_size = re_cpp.ProgramSize();

    // Compare (non-terminating, continue on mismatch)
    EXPECT_EQ(expected_size, size_from_cre2);

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
}

// Test 1: Basic pattern matching size equality with a simple literal
static void test_cre2_program_size_basic_pattern() {
    // Simple pattern
    test_program_size_matches_for_pattern("abc");
}

// Test 2: Empty pattern should still produce a valid program size
static void test_cre2_program_size_empty_pattern() {
    test_program_size_matches_for_pattern("");
}

// Test 3: Longer pattern to exercise larger program size
static void test_cre2_program_size_long_pattern() {
    std::string long_pat(256, 'a'); // 256 'a's
    test_program_size_matches_for_pattern(long_pat.c_str());
}

// Test 4: Pattern with metacharacters to ensure handling of more complex expressions
static void test_cre2_program_size_complex_pattern() {
    // A moderately complex pattern
    test_program_size_matches_for_pattern("(ab|cd)+e?colou?r");
}

// Test 5: Reuse with different options to ensure consistent results
static void test_cre2_program_size_consistency_across_options() {
    const char* pattern = "consistency_test_pattern";
    size_t len = std::strlen(pattern);

    cre2_options_t* opt1 = cre2_opt_new();
    cre2_regexp_t* re1 = cre2_new(pattern, (int)len, opt1);
    size_t size1 = cre2_program_size(re1);

    cre2_options_t* opt2 = cre2_opt_new();
    cre2_regexp_t* re2 = cre2_new(pattern, (int)len, opt2);
    size_t size2 = cre2_program_size(re2);

    // Both should be equal for same pattern with default options
    EXPECT_EQ(size1, size2);

    cre2_delete(re1);
    cre2_opt_delete(opt1);
    cre2_delete(re2);
    cre2_opt_delete(opt2);
}

// Main function: call test routines from main (as required in the domain knowledge)
int main() {
    // Run tests
    test_cre2_program_size_basic_pattern();
    test_cre2_program_size_empty_pattern();
    test_cre2_program_size_long_pattern();
    test_cre2_program_size_complex_pattern();
    test_cre2_program_size_consistency_across_options();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All cre2_program_size tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " cre2_program_size tests failed.\n";
        return 1;
    }
}