// Unit test suite for cre2_possible_match_range (cre2.cpp)
// This test suite is designed to work without GTest, using a lightweight
// custom test harness with non-terminating assertions (prints failures
// but continues execution). It targets the CRE2-based wrapper defined in
// cre2.h and exercises the true/false branches of cre2_possible_match_range.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Simple non-terminating test harness
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg) do { \
    if (cond) { \
        ++g_pass; \
    } else { \
        ++g_fail; \
        std::cerr << "TEST_FAIL: " << (msg) << std::endl; \
    } \
} while (0)

// Test 1: Verify true-branch execution of cre2_possible_match_range
// - Creates a simple alternation pattern that should produce a possible match range.
// - Ensures that PossibleMatchRange returns true and that min/max strings are allocated
//   and populated with non-null, non-empty data.
// - Verifies basic memory handling (min.data and max.data are freed after use).
void test_true_path_possible_match_range() {
    // Arrange: create a simple regexp that is known to be matchable
    cre2_regexp_t * rex = cre2_new("a|b", 3, NULL);
    CHECK(rex != NULL, "cre2_new failed to create regexp for pattern 'a|b'");

    // Act: invoke cre2_possible_match_range
    cre2_string_t min = { NULL, 0 };
    cre2_string_t max = { NULL, 0 };
    int maxlen = 100;  // typical non-trivial max length
    int res = cre2_possible_match_range(rex, &min, &max, maxlen);

    // Assert: retval should be 1 (true) and both min/max data should be allocated
    CHECK(res == 1, "cre2_possible_match_range should return 1 for a matchable pattern");

    if (res == 1) {
        CHECK(min.data != NULL, "min.data should be allocated when retval is true");
        CHECK(max.data != NULL, "max.data should be allocated when retval is true");
        // Also verify that the lengths correspond to the allocated buffers
        CHECK(min.length > 0, "min.length should be > 0 when data is allocated");
        CHECK(max.length > 0, "max.length should be > 0 when data is allocated");

        // Optional: inspect strings (null-terminated) via std::string wrapper
        std::string min_str(min.data, min.length);
        std::string max_str(max.data, max.length);
        std::cout << "TestTruePath: MIN=\"" << min_str << "\" MAX=\"" << max_str << "\"\n";

        // Clean up allocated buffers
        free((void *)min.data);
        free((void *)max.data);
        min.data = NULL;
        max.data = NULL;
    }

    // Cleanup: delete the regexp
    cre2_delete(rex);
}

// Test 2: Verify false-branch execution of cre2_possible_match_range
// - Use a conservative maxlen (0) to exercise the else-branch where PossibleMatchRange
//   indicates no possible match within the given length.
// - Ensures that the function returns 0 and does not attempt to allocate memory.
void test_false_path_possible_match_range() {
    // Arrange: create a simple regexp
    cre2_regexp_t * rex = cre2_new("a", 1, NULL);
    CHECK(rex != NULL, "cre2_new failed to create regexp for pattern 'a'");

    // Act: invoke cre2_possible_match_range with a maxlen that should result in no possible match
    cre2_string_t min = { NULL, 0 };
    cre2_string_t max = { NULL, 0 };
    int maxlen = 0;  // likely to trigger false branch (no possible match within length 0)

    int res = cre2_possible_match_range(rex, &min, &max, maxlen);

    // Assert: retval should be 0 (no possible match within maxlen)
    CHECK(res == 0, "cre2_possible_match_range should return 0 when no match is possible within maxlen");

    // Since retval is 0, min/max should not have been allocated; ensure no dangling pointers
    if (min.data != NULL || max.data != NULL) {
        // If allocations happened, clean them to avoid leaks and report if it happened unexpectedly
        if (min.data) free((void*)min.data);
        if (max.data) free((void*)max.data);
        min.data = NULL;
        max.data = NULL;
        CHECK(false, "min/max buffers should not be allocated when retval is 0");
    } else {
        CHECK(true, "min/max buffers not allocated as expected for false path");
    }

    // Cleanup
    cre2_delete(rex);
}

// Main test runner
int main() {
    std::cout << "Starting cre2_possible_match_range unit tests (no GTest)\n";

    test_true_path_possible_match_range();
    test_false_path_possible_match_range();

    std::cout << "Tests completed. Passed: " << g_pass << ", Failed: " << g_fail << "\n";

    // Return non-zero if any test failed
    return (g_fail != 0) ? 1 : 0;
}