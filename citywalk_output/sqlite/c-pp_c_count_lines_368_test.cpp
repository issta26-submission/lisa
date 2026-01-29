/*
  Unit test suite for the focal function:
  unsigned count_lines(unsigned char const * zFrom, unsigned char const * zTo);

  This test suite is implemented in C++11 without using GoogleTest.
  It exercises typical and edge scenarios by calling the C function
  from the provided c-pp.c file. The test framework is minimal and
  non-terminating: it reports failures but continues executing.
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Declaration of the focal C function (to be linked from c-pp.c)
extern "C" unsigned count_lines(unsigned char const * zFrom, unsigned char const * zTo);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// EXPECT_EQ macro: records a failure but does not abort tests
#define EXPECT_EQ(actual, expected) do {                               \
    ++g_total;                                                           \
    auto _act = (actual);                                                \
    auto _exp = (expected);                                              \
    if (!(_act == _exp)) {                                             \
        std::cerr << "Test failure at " << __FILE__ << ":" << __LINE__   \
                  << " - actual: " << _act                                   \
                  << ", expected: " << _exp << std::endl;                 \
        ++g_failed;                                                    \
    }                                                                    \
} while(0)

// Test 1: No newline characters in the range
// Rationale: The function should return 0 when there are no '\n' characters.
void test_no_newlines() {
    // "abcdef" has no newline characters
    const char s[] = "abcdef";
    unsigned count = count_lines(reinterpret_cast<unsigned char const*>(s),
                                 reinterpret_cast<unsigned char const*>(s) + strlen(s));
    EXPECT_EQ(count, 0u);
}

// Test 2: Empty range (zFrom == zTo)
// Rationale: Count should be 0 for an empty range, while still honoring the non-null preconditions.
void test_empty_range() {
    const char s[] = ""; // empty string
    unsigned count = count_lines(reinterpret_cast<unsigned char const*>(s),
                                 reinterpret_cast<unsigned char const*>(s) + strlen(s));
    EXPECT_EQ(count, 0u);
}

// Test 3: Single newline at the end
// Rationale: A newline at the last position before zTo should be counted exactly once.
void test_single_newline_end() {
    const char s[] = "hello\n";
    unsigned count = count_lines(reinterpret_cast<unsigned char const*>(s),
                                 reinterpret_cast<unsigned char const*>(s) + strlen(s));
    EXPECT_EQ(count, 1u);
}

// Test 4: Multiple newlines scattered in the range
// Rationale: All newline occurrences within the range should be counted.
void test_multiple_newlines() {
    // "a\nb\n\nc" -> 3 newline characters
    const char s[] = "a\nb\n\nc";
    unsigned count = count_lines(reinterpret_cast<unsigned char const*>(s),
                                 reinterpret_cast<unsigned char const*>(s) + strlen(s));
    EXPECT_EQ(count, 3u);
}

// Test 5: All characters are newlines
// Rationale: Range consisting entirely of newline characters should return its length.
void test_all_newlines() {
    const char s[] = "\n\n\n\n\n"; // 5 newlines
    unsigned count = count_lines(reinterpret_cast<unsigned char const*>(s),
                                 reinterpret_cast<unsigned char const*>(s) + strlen(s));
    EXPECT_EQ(count, 5u);
}

// Run all tests and report summary
void run_all_tests() {
    test_no_newlines();
    test_empty_range();
    test_single_newline_end();
    test_multiple_newlines();
    test_all_newlines();
}

int main(int argc, char const * const *argv) {
    (void)argc; (void)argv; // avoid unused parameter warnings
    run_all_tests();

    if (g_failed == 0) {
        std::cout << "All tests passed. Total tests: " << g_total << std::endl;
        return 0;
    } else {
        std::cerr << g_failed << " test(s) failed out of " << g_total << std::endl;
        return 1;
    }
}