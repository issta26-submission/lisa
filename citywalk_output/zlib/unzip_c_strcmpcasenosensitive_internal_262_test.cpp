// Unit test suite for strcmpcasenosensitive_internal
// ---------------------------------------------------
// This test suite is written in standard C++11 (no GoogleTest) and
// tests the behavior of strcmpcasenosensitive_internal by re-implementing
// its logic in this translation unit (as a non-static function for testing purposes).
//
// The tests cover:
// - Case-insensitive comparison across letters (lowercase to uppercase).
// - End-of-string behavior when one string ends before the other.
// - Mixed content (letters and digits) to ensure non-letter characters are handled correctly.
// - Various equality and inequality scenarios to exercise all predicate branches.

#include <cstring>
#include <string.h>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Implementation under test (copied here to enable testing in a single TU).
// In the real project this function is static within unzip.c; for test purposes
// we expose an equivalent function in this translation unit.
int strcmpcasenosensitive_internal(const char* fileName1, const char* fileName2) {
    for (;;)
    {
        char c1=*(fileName1++);
        char c2=*(fileName2++);
        if ((c1>='a') && (c1<='z'))
            c1 -= 0x20;
        if ((c2>='a') && (c2<='z'))
            c2 -= 0x20;
        if (c1=='\0')
            return ((c2=='\0') ? 0 : -1);
        if (c2=='\0')
            return 1;
        if (c1<c2)
            return -1;
        if (c1>c2)
            return 1;
    }
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define CHECK_EQ(actual, expected, testName) do { \
    ++g_tests_run; \
    int _act = (actual); \
    int _exp = (expected); \
    if (_act != _exp) { \
        std::cerr << "[FAILED] " << testName \
                  << ": expected " << _exp << " but got " << _act << "\n"; \
        ++g_tests_failed; \
    } else { \
        std::cout << "[PASSED] " << testName << "\n"; \
    } \
} while (0)


// Test 1: Equal strings with different cases should compare as equal (case-insensitive)
void test_equal_strings_case_insensitive() {
    // both strings contain same letters ignoring case
    const char* s1 = "abc";
    const char* s2 = "ABC";
    int r = strcmpcasenosensitive_internal(s1, s2);
    CHECK_EQ(r, 0, "Equal strings with differing case -> 0");
}

// Test 2: First char greater after case-insensitive normalization
void test_first_char_greater_after_normalization() {
    // 'b' vs 'a' -> after normalization 'B' > 'A' => 1
    CHECK_EQ(strcmpcasenosensitive_internal("b", "a"), 1, "First char greater after normalization -> 1");
    // also ensure mixed case first char
    CHECK_EQ(strcmpcasenosensitive_internal("B", "a"), 1, "First char 'B' vs 'a' -> 1");
}

// Test 3: First string ends earlier than second string
void test_end_of_string_first_earlier() {
    // "abc" vs "abcd" -> first ends earlier -> -? Wait logic: if c2 != '\0' at end of s1, returns -1 only when c1=='\0' and c2 != '\0'
    // Thus "abc" vs "abcd" should yield -1
    CHECK_EQ(strcmpcasenosensitive_internal("abc", "abcd"), -1, "End-of-string: first shorter -> -1");
}

// Test 4: Second string ends earlier than first string
void test_end_of_string_second_earlier() {
    // "abcd" vs "abc" -> second ends earlier -> 1
    CHECK_EQ(strcmpcasenosensitive_internal("abcd", "abc"), 1, "End-of-string: second shorter -> 1");
}

// Test 5: Character-by-character difference after some equal prefix
void test_prefix_then_difference() {
    // "abc" vs "abd" -> compare 'c' vs 'd' -> -1
    CHECK_EQ(strcmpcasenosensitive_internal("abc", "abd"), -1, "Difference after equal prefix -> -1");
}

// Test 6: Equal strings with mixed cases across full length
void test_equal_strings_mixed_case_full() {
    // "AbCdEf" vs "aBcDeF" -> should be equal
    CHECK_EQ(strcmpcasenosensitive_internal("AbCdEf", "aBcDeF"), 0, "Full-length equal with mixed case -> 0");
}

// Test 7: Non-letter characters should compare using their ASCII values (case-insensitive has no effect)
void test_non_letters_equal() {
    // digits and punctuation should compare based on their codes
    CHECK_EQ(strcmpcasenosensitive_internal("file1.txt", "FILE1.TXT"), 0, "Non-letter comparison case-insensitive -> 0");
}

// Test 8: Mixed letters and digits where digits determine order
void test_letters_and_digits_order() {
    // "a1b" vs "a2b" -> compare '1' (49) vs '2' (50) after normalization, so -1
    CHECK_EQ(strcmpcasenosensitive_internal("a1b", "a2b"), -1, "Letters-digits order -> -1");
}

// Test 9: Long strings with later difference
void test_long_strings_late_difference() {
    // "abcdefghij" vs "abcdefghik" -> at position 9 ('j' vs 'k'), 'J' (74) vs 'K' (75) => -1
    CHECK_EQ(strcmpcasenosensitive_internal("abcdefghij", "abcdefghik"), -1, "Long strings difference late -> -1");
}

// Test 10: Both strings identical to ensure 0
void test_identical_strings() {
    CHECK_EQ(strcmpcasenosensitive_internal("same", "same"), 0, "Identical strings -> 0");
}

// Test 11: Edge case with upper/lower inside longer strings
void test_mixed_case_longer_strings() {
    // "SomeLongName123" vs "somelongname123" -> equal ignoring case
    CHECK_EQ(strcmpcasenosensitive_internal("SomeLongName123", "somelongname123"), 0, "Mixed case long strings -> 0");
}

// Runner
int main() {
    std::cout << "Running strcmpcasenosensitive_internal unit tests (non-terminating assertions):\n";

    test_equal_strings_case_insensitive();
    test_first_char_greater_after_normalization();
    test_end_of_string_first_earlier();
    test_end_of_string_second_earlier();
    test_prefix_then_difference();
    test_equal_strings_mixed_case_full();
    test_non_letters_equal();
    test_letters_and_digits_order();
    test_long_strings_late_difference();
    test_identical_strings();
    test_mixed_case_longer_strings();

    std::cout << "\nTest Summary: Run=" << g_tests_run << "  Failed=" << g_tests_failed << "\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}