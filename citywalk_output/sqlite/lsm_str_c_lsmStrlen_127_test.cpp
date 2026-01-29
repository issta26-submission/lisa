// Test suite for the focal method lsmStrlen in lsm_str.c
// - This test harness is written in C++11 without Google Test.
// - It uses lightweight EXPECT-like macros that do not terminate the test run on failure.
// - It relies on the C linkage for the lsmStrlen function to be linked with the C source.

// Extern "C" to ensure proper linkage with the C implementation of lsmStrlen
extern "C" int lsmStrlen(const char *zName);

#include <lsmInt.h>
#include <iostream>


// Lightweight, non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Expect equality between two values
#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    auto _actual = (actual); \
    auto _expected = (expected); \
    if ( _actual != _expected ) { \
        ++g_failed_tests; \
        std::cout << "[FAIL] " << msg \
                  << " | expected: " << _expected \
                  << ", actual: " << _actual << std::endl; \
    } \
} while(0)

// Expect a boolean condition to be true
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cout << "[FAIL] " << msg << " | condition is false" << std::endl; \
    } \
} while(0)

// Expect a boolean condition to be false
#define EXPECT_FALSE(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { \
        ++g_failed_tests; \
        std::cout << "[FAIL] " << msg << " | condition is true" << std::endl; \
    } \
} while(0)

/*
  Tests for lsmStrlen:
  - Basic ASCII strings
  - Empty string
  - String with embedded null character (should stop at first \0)
  - Very long string to exercise counting loop
*/

// Test 1: Basic ASCII string
// Purpose: Verify that lsmStrlen returns the correct length for a typical string.
static void test_lsmStrlen_basic() {
    const char *s = "Hello";
    int len = lsmStrlen(s);
    EXPECT_EQ(len, 5, "lsmStrlen(\"Hello\") should return 5");
}

// Test 2: Empty string
// Purpose: Ensure zero length is returned for an empty string.
static void test_lsmStrlen_empty() {
    const char *s = "";
    int len = lsmStrlen(s);
    EXPECT_EQ(len, 0, "lsmStrlen(\"\") should return 0");
}

// Test 3: Embedded null within the array (not a C-string termination in source)
// Purpose: Verify that counting stops at the first null character, as in standard C strings.
static void test_lsmStrlen_embedded_null() {
    char s_with_null[] = { 'A', 'B', '\0', 'C', 'D', 0 }; // length should be 2
    int len = lsmStrlen(s_with_null);
    EXPECT_EQ(len, 2, "lsmStrlen with embedded null should count up to first '\\0' (2)");
}

// Test 4: Very long string to validate counting loop robustness
static void test_lsmStrlen_long() {
    // "The quick brown fox jumps over the lazy dog" has length 43
    const char *s = "The quick brown fox jumps over the lazy dog";
    int len = lsmStrlen(s);
    EXPECT_EQ(len, 43, "lsmStrlen(long pangram) should return 43");
}

// Run all tests and summarize results
static void run_all_tests() {
    test_lsmStrlen_basic();
    test_lsmStrlen_empty();
    test_lsmStrlen_embedded_null();
    test_lsmStrlen_long();

    std::cout << "lsmStrlen test summary: "
              << "Total=" << g_total_tests
              << ", Passed=" << (g_total_tests - g_failed_tests)
              << ", Failed=" << g_failed_tests
              << std::endl;
}

// Main function as the entry point for test execution
int main() {
    run_all_tests();
    // Return non-zero if any test failed to signal issues in CI systems that check exit codes
    return (g_failed_tests > 0) ? 1 : 0;
}