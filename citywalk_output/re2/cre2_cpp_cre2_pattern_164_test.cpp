/*
Step 1 - Candidate Keywords (derived from the focal method)
- cre2_pattern: the function under test
- cre2_regexp_t: the type representing a compiled regex (opaque to callers)
- TO_CONST_RE2: a macro used to access the underlying re2::RE2 object
- re2: the underlying library providing RE2::Pattern functionality
- pattern(): the RE2 method that returns the stored pattern as std::string
- c_str(): the C-style string accessor for the underlying std::string
- re: a cre2_regexp_t instance
- cre2_new/cre2_delete: lifecycle management for cre2_regexp_t
- cre2_options_t: options structure (passed as nullptr in tests for default behavior)
- The test should validate content integrity of the pattern string returned by cre2_pattern
- Lifetime considerations: the returned pointer must remain valid while the underlying re object exists
- Non-GTest approach: we implement a lightweight test harness with non-terminating checks
*/

/*
Step 2 & 3 - Unit Test Suite for cre2_pattern
- Tests rely only on the public cre2 API (cre2_new, cre2_pattern, cre2_delete)
- Coverage goals:
  - Basic pattern retrieval matches input
  - Patterns with special characters are retrieved correctly
  - Consistency: repeated calls yield identical content
  - Handling of a longer pattern retains content integrity
- Implements a small test framework (EXPECT_* macros) to avoid terminating on first failure
- Uses plain C/C++ standard library; no GTest or Google Mock
*/

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <re2/set.h>


// Lightweight test harness: non-terminating assertions to maximize coverage
static int g_test_failures = 0;

#define EXPECT_NOT_NULL(x) do { \
    if ((x) == nullptr) { \
        fprintf(stderr, "EXPECT_NOT_NULL failed: %s is NULL at %s:%d\n", #x, __FILE__, __LINE__); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_EQ_STR(a, b) do { \
    if ((a) == nullptr || (b) == nullptr) { \
        fprintf(stderr, "EXPECT_EQ_STR failed: NULL pointer involved at %s:%d\n", __FILE__, __LINE__); \
        ++g_test_failures; \
    } else if (std::strcmp((a), (b)) != 0) { \
        fprintf(stderr, "EXPECT_EQ_STR failed: \"%s\" != \"%s\" at %s:%d\n", (a), (b), __FILE__, __LINE__); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_TRUE(x) do { \
    if (!(x)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s is false at %s:%d\n", #x, __FILE__, __LINE__); \
        ++g_test_failures; \
    } \
} while (0)

// Test 1: Basic pattern retrieval
static void test_cre2_pattern_basic() {
    // Given a simple pattern, cre2_new should create a valid regex object.
    const char *pattern = "abc";
    cre2_regexp_t *re = cre2_new(pattern, (int)std::strlen(pattern), nullptr);
    EXPECT_NOT_NULL(re);

    // When querying the pattern via cre2_pattern, we expect the content to match the input.
    const char *ret = cre2_pattern(re);
    EXPECT_NOT_NULL(ret);
    EXPECT_EQ_STR(ret, pattern);

    // Cleanup
    cre2_delete(re);
}

// Test 2: Pattern with special characters
static void test_cre2_pattern_special_chars() {
    const char *pattern = "(a|b)*c";
    cre2_regexp_t *re = cre2_new(pattern, (int)std::strlen(pattern), nullptr);
    EXPECT_NOT_NULL(re);

    const char *ret = cre2_pattern(re);
    EXPECT_NOT_NULL(ret);
    EXPECT_EQ_STR(ret, pattern);

    cre2_delete(re);
}

// Test 3: Consistency across multiple invocations
static void test_cre2_pattern_consistency() {
    const char *pattern = "hello_world-123";
    cre2_regexp_t *re = cre2_new(pattern, (int)std::strlen(pattern), nullptr);
    EXPECT_NOT_NULL(re);

    const char *ret1 = cre2_pattern(re);
    const char *ret2 = cre2_pattern(re);
    EXPECT_NOT_NULL(ret1);
    EXPECT_NOT_NULL(ret2);

    // Content should be identical across calls
    EXPECT_EQ_STR(ret1, ret2);
    EXPECT_EQ_STR(ret1, pattern);

    cre2_delete(re);
}

// Test 4: Long pattern to exercise larger internal storage
static void test_cre2_pattern_long() {
    // Build a reasonably long pattern (non-empty)
    std::string long_pattern(256, 'x'); // 256 'x' characters
    cre2_regexp_t *re = cre2_new(long_pattern.c_str(), (int)long_pattern.size(), nullptr);
    EXPECT_NOT_NULL(re);

    const char *ret = cre2_pattern(re);
    EXPECT_NOT_NULL(ret);
    EXPECT_EQ_STR(ret, long_pattern.c_str());

    cre2_delete(re);
}

// Simple test runner
static void run_all_tests() {
    test_cre2_pattern_basic();
    test_cre2_pattern_special_chars();
    test_cre2_pattern_consistency();
    test_cre2_pattern_long();
}

// Entry point
int main() {
    run_all_tests();

    if (g_test_failures > 0) {
        fprintf(stderr, "Unit tests completed with %d failure(s).\n", g_test_failures);
        return 1;
    } else {
        printf("All unit tests passed.\n");
        return 0;
    }
}