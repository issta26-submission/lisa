// Lightweight C++11 test suite for the focal main() behaviors in the cre2-based helpers.
// This test suite avoids GTest and uses a small, non-terminating assertion mechanism.
// It directly calls the C library (cre2) used by the focal method and validates
// the same scenarios demonstrated in the original main() function.
//
// Notes:
// - Tests are designed to run in a single process; failures are counted and reported
//   at the end, but execution continues to maximize coverage.
// - We exercise true branches (successful calls) and attempt to cover the spirit of
//   false branches by testing error-like conditions where feasible (e.g., rewrite
//   string failure path, invalid rewrite strings).
// - All tests are self-contained and do not rely on any private members or gmock/gtest.

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <cre2-test.h>
#include <cstdio>


// Include the C library header for CR2 (cre2). Guard for C linkage inside C++.
extern "C" {
}

// Global failure counter for the lightweight test harness.
static int g_failures = 0;

// Simple non-terminating assertion macro.
// On failure, logs message and increments the failure counter but does not exit.
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "Test failure: %s (line %d)\n", (msg), __LINE__); \
        ++g_failures; \
    } \
} while (0)

// Helper to print a summary at the end.
static void print_summary(void)
{
    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_failures);
    }
}

// Test 1: Quote meta characters
// Verifies that cre2_quote_meta properly escapes regex meta characters.
// This corresponds to the first block inside the focal main() function.
static void test_quote_meta_basic(void)
{
    // Prepare input pattern with characters that need escaping.
    const char *pattern = "1.5-2.0?";
    cre2_string_t original = {
        .data = pattern,
        .length = (strlen(pattern))
    };
    cre2_string_t quoted;
    int result = cre2_quote_meta(&quoted, &original);

    TEST_ASSERT(result == 0, "cre2_quote_meta should succeed (return 0)");

    // Expected escaped string: "1\.5\-2\.0\?"
    const char *expected = "1\\.5\\-2\\.0\\?";
    size_t expected_len = strlen(expected);

    TEST_ASSERT(quoted.length == expected_len, "Quoted length should match expected");
    if (quoted.length == expected_len) {
        int cmp = strncmp((const char *)quoted.data, expected, expected_len) == 0;
        TEST_ASSERT(cmp, "Quoted content should match expected escaped string");
    }

    free((void *)quoted.data);
}

// Test 2: Possible match range
// Verifies that cre2_possible_match_range returns a valid min/max range for a
// case-insensitive pattern. This mirrors the second block in the focal main().
static void test_possible_match_range(void)
{
    const char *pattern = "(?i)ABCdef";
    cre2_regexp_t *rex = cre2_new(pattern, strlen(pattern), NULL);
    TEST_ASSERT(rex != NULL, "cre2_new should succeed for valid pattern");

    cre2_string_t min, max;
    int result = cre2_possible_match_range(rex, &min, &max, 1024);

    TEST_ASSERT(result == 1, "cre2_possible_match_range should return 1 on success");

    // Expect min "ABCDEF" and max "abcdef" according to the original test.
    TEST_ASSERT(min.length == 6, "min length should be 6");
    if (min.length == 6) {
        int cmp_min = strncmp((const char *)min.data, "ABCDEF", 6) == 0;
        TEST_ASSERT(cmp_min, "min value should be 'ABCDEF'");
    }

    TEST_ASSERT(max.length == 6, "max length should be 6");
    if (max.length == 6) {
        int cmp_max = strncmp((const char *)max.data, "abcdef", 6) == 0;
        TEST_ASSERT(cmp_max, "max value should be 'abcdef'");
    }

    cre2_delete(rex);
    free((void *)min.data);
    free((void *)max.data);
}

// Test 3: Successful rewrite check
// Verifies that a valid rewrite string is accepted for the rewrite pattern.
// This corresponds to the third block in the focal main() function.
static void test_check_rewrite_string_success(void)
{
    const char *pattern = "a(b)c";
    const char *subst   = "def";

    cre2_regexp_t *rex = cre2_new(pattern, strlen(pattern), NULL);
    TEST_ASSERT(rex != NULL, "cre2_new should succeed for valid pattern");

    cre2_string_t rewrite = {
        .data   = (char *)subst,
        .length = strlen(subst)
    };
    cre2_string_t errmsg;
    int result = cre2_check_rewrite_string(rex, &rewrite, &errmsg);

    TEST_ASSERT(result == 1, "cre2_check_rewrite_string should succeed (return 1)");

    cre2_delete(rex);

    // No explicit content to verify for errmsg; ensure it's safe to ignore.
    if (errmsg.data != NULL) {
        free((void *)errmsg.data);
    }
}

// Test 4: Failed rewrite check (invalid backreference syntax)
// Verifies that an invalid rewrite string is rejected and that an error message
// is produced. This corresponds to the fourth block in the focal main() function.
static void test_check_rewrite_string_failure(void)
{
    const char *pattern = "a(b)c";
    const char *subst   = "\\1 \\2";

    cre2_regexp_t *rex = cre2_new(pattern, strlen(pattern), NULL);
    TEST_ASSERT(rex != NULL, "cre2_new should succeed for valid pattern");

    cre2_string_t rewrite = {
        .data   = (char *)subst,
        .length = strlen(subst)
    };
    cre2_string_t errmsg;
    int result = cre2_check_rewrite_string(rex, &rewrite, &errmsg);

    TEST_ASSERT(result == 0, "cre2_check_rewrite_string should fail (return 0) for invalid backrefs");

    // If an error message was produced, print it (as in the original test) and free.
    if (errmsg.data != NULL) {
        printf("error message: ");
        fwrite(errmsg.data, errmsg.length, 1, stdout);
        printf("\n");
        free((void *)errmsg.data);
    }

    cre2_delete(rex);
}

// Entry point for the test suite.
// Calls all test cases and prints a final summary.
// This mirrors the instruction to "call test methods from the main function" in environments
// where a test framework is not available.
int main(void)
{
    // Run all tests in sequence.
    test_quote_meta_basic();
    test_possible_match_range();
    test_check_rewrite_string_success();
    test_check_rewrite_string_failure();

    // Print summary for the lightweight test harness.
    print_summary();

    // Return non-zero if any test failed to aid integration with CI systems.
    return (g_failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}