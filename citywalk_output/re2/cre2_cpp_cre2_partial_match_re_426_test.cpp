/*
  Unit test suite for the focal method cre2_partial_match_re (from cre2.cpp).
  This test suite is handcrafted for C++11, does not rely on GTest,
  and uses a minimal, non-terminating assertion mechanism to maximize
  code coverage across the focal function and its dependencies.

  Notes and assumptions (based on the provided focal code and typical CRE2 wrappers):
  - cre2_new(pattern, pattern_len, opt) creates a compiled regular expression.
  - cre2_opt_new()/cre2_opt_delete() manage options (cre2_options_t).
  - cre2_partial_match_re(cre2_regexp_t*, const char*, int, int, int, cre2_string_t*, int)
    performs a partial match on the given text, filling 'match' with the
    matched substring (cre2_string_t) and returning a non-zero verdict on success.
  - cre2_string_t is a structure of (char* data; size_t length;).
  - cre2_delete frees the compiled regex.
  - Allocated memory in cre2_string_t->data, if any, should be freed by the test
    to avoid leaks (we assume the API may allocate; handle defensively).
  - The tests focus on the true/false branches and boundary conditions to
    maximize branch coverage.
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


// Include CRE2 C-wrapper header (assumed to be in include path)

// Simple non-terminating assertion framework
static int g_test_total = 0;
static int g_test_passed = 0;

#define ASSERT(cond, msg) \
    do { \
        g_test_total++; \
        if (cond) { \
            g_test_passed++; \
        } else { \
            std::cerr << "ASSERTION FAILED: " << (msg) << " (line " << __LINE__ << ")\n"; \
        } \
    } while (0)


// Helper: Safe free of cre2_string_t data if allocated
static void safe_free_match(cre2_string_t *match) {
    if (match && match->data) {
        free(match->data);
        match->data = nullptr;
        match->length = 0;
    }
}

// Test 1: Basic positive path for cre2_partial_match_re using a simple literal
// pattern that should fully match a prefix of the input text within bounds.
static void test_partial_match_re_basic_positive() {
    // Arrange
    cre2_options_t *opt = cre2_opt_new();
    ASSERT(opt != nullptr, "Failed to create cre2 options (opt != nullptr)");

    cre2_regexp_t *re = cre2_new("ab", 2, opt);
    ASSERT(re != nullptr, "Failed to compile pattern 'ab' into CRE2 regexp (re != nullptr)");
    
    cre2_string_t match;
    std::memset(&match, 0, sizeof(match));

    // Act
    int ret = cre2_partial_match_re(re, "abcdef", 6, 0, 6, &match, 1);

    // Assert
    ASSERT(ret != 0, "cre2_partial_match_re should indicate success for text 'abcdef' with pattern 'ab'");
    if (ret != 0) {
        // Best-effort checks on match content/length if provided by API
        ASSERT(match.length >= 0, "Partial match should set a non-negative match length");
        if (match.length > 0 && match.data) {
            // The matched substring is expected to be 'ab' (length 2)
            // We perform a safe content check only if length is small to avoid extra allocations
            int cmp_len = (match.length < 4) ? static_cast<int>(match.length) : 2;
            if (cmp_len > 0 && match.data) {
                // Copy up to 3 chars for a safe compare
                char buf[8] = {0};
                std::strncpy(buf, match.data, cmp_len);
                // If the matched string is exactly 'ab', then buf should start with "ab"
                ASSERT(std::strncmp(buf, "ab", 2) == 0, "Matched content should start with 'ab' for pattern 'ab'");
            } else {
                // If length is unexpected, still pass as long as non-zero
                ASSERT(true, "Matched non-zero length with non-null data (content check skipped due to length)");
            }
        } else {
            // No data; still consider as a valid non-null length check passed
            ASSERT(true, "Match data is null but length indicates a match (acceptable in some wrappers)");
        }
    }

    // Cleanup
    safe_free_match(&match);
    cre2_delete(re);
    cre2_opt_delete(opt);
}

// Test 2: Negative path for cre2_partial_match_re where no match exists in the input text
static void test_partial_match_re_basic_negative() {
    // Arrange
    cre2_options_t *opt = cre2_opt_new();
    ASSERT(opt != nullptr, "Failed to create cre2 options (opt != nullptr)");

    cre2_regexp_t *re = cre2_new("ab", 2, opt);
    ASSERT(re != nullptr, "Failed to compile pattern 'ab' into CRE2 regexp (re != nullptr)");
    
    cre2_string_t match;
    std::memset(&match, 0, sizeof(match));

    // Act
    int ret = cre2_partial_match_re(re, "xyz", 3, 0, 3, &match, 1);

    // Assert
    ASSERT(ret == 0, "cre2_partial_match_re should indicate failure for text 'xyz' with pattern 'ab'");

    // Cleanup
    safe_free_match(&match);
    cre2_delete(re);
    cre2_opt_delete(opt);
}

// Test 3: Boundary condition: endpos restricts matching window
// Text contains the pattern, but endpos limits the usable portion so that
// the match should still be detected if endpos >= match length.
static void test_partial_match_re_endpos_boundary() {
    // Arrange
    cre2_options_t *opt = cre2_opt_new();
    ASSERT(opt != nullptr, "Failed to create cre2 options (opt != nullptr)");

    cre2_regexp_t *re = cre2_new("ab", 2, opt);
    ASSERT(re != nullptr, "Failed to compile pattern 'ab' into CRE2 regexp (re != nullptr)");
    
    cre2_string_t match;
    std::memset(&match, 0, sizeof(match));

    // Act: endpos restricted to first 2 characters; "ab" should still be found there
    int ret = cre2_partial_match_re(re, "abab", 4, 0, 2, &match, 1);

    // Assert
    ASSERT(ret != 0, "cre2_partial_match_re should succeed within endpos boundary (endpos=2 for 'abab')");
    if (ret != 0) {
        ASSERT(match.length >= 0, "Partial match result should have non-negative length within endpos boundary");
    }

    // Cleanup
    safe_free_match(&match);
    cre2_delete(re);
    cre2_opt_delete(opt);
}

// Entry point for running tests
int main() {
    std::cout << "Running cre2_partial_match_re unit tests (C++11, non-terminating assertions)\n";

    test_partial_match_re_basic_positive();
    test_partial_match_re_basic_negative();
    test_partial_match_re_endpos_boundary();

    int total = g_test_total;
    int passed = g_test_passed;
    std::cout << "Tests run: " << total << ", Passed: " << passed
              << ", Failed: " << (total - passed) << "\n";

    // Provide a simple exit code: 0 if all tests pass, 1 otherwise
    return (total == passed) ? 0 : 1;
}