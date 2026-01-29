// C++11 compliant unit test suite for the focal method:
// sqlite3StrIHash in util.c
// The test avoids any test framework (GTest) and uses a lightweight harness.
// It relies on the external C linkage of sqlite3StrIHash and the external
// UpperToLower array that is defined in the project under test.

#include <math.h>
#include <string>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Declare the function under test with C linkage
extern "C" unsigned char sqlite3StrIHash(const char *z);

// Declare the external array used for hashing (case mapping)
extern "C" unsigned char UpperToLower[];

// Lightweight test harness
static int g_failures = 0;

#define TEST_EXPECT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_FAIL: " << msg << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define TEST_EXPECT_EQ(actual, expected, msg) \
    do { \
        if((actual) != (expected)) { \
            std::cerr << "EXPECT_EQ_FAIL: " << msg \
                      << " | expected: " << int(expected) \
                      << " actual: " << int(actual) << "\n"; \
            ++g_failures; \
        } \
    } while(0)

// Helper to compute expected hash using the exported UpperToLower array.
// This mirrors the logic inside sqlite3StrIHash.
static unsigned char compute_expected_hash(const std::string &s) {
    unsigned char h = 0;
    for (unsigned char ch : s) {
        h += UpperToLower[ch];
    }
    return h;
}

// Test 1: Null pointer should return 0
static void test_null_pointer() {
    // Follows the contract: if z == 0, return 0
    unsigned char res = sqlite3StrIHash(nullptr);
    TEST_EXPECT_EQ(res, 0, "sqlite3StrIHash(nullptr) should return 0");
}

// Test 2: Empty string should return 0
static void test_empty_string() {
    const char *empty = "";
    unsigned char res = sqlite3StrIHash(empty);
    TEST_EXPECT_EQ(res, 0, "sqlite3StrIHash(\"\") should return 0");
}

// Test 3: Basic string hashing with mixed characters
static void test_basic_hash() {
    std::string s = "ABCdef"; // mix of upper and lower
    unsigned char res = sqlite3StrIHash(s.c_str());
    unsigned char expected = compute_expected_hash(s);
    TEST_EXPECT_EQ(res, expected, "Basic mixed string hash should match expected from UpperToLower mapping");
}

// Test 4: Hashing with non-letter characters to ensure loop handles all chars
static void test_non_letter_chars() {
    std::string s = "Hello, World! 123";
    unsigned char res = sqlite3StrIHash(s.c_str());
    unsigned char expected = compute_expected_hash(s);
    TEST_EXPECT_EQ(res, expected, "Hash with spaces punctuation digits should use UpperToLower for each char");
}

// Test 5: Hash wraparound behavior (32-bit to 8-bit wrap)
// Create 256 identical characters to exercise modulo 256
static void test_wraparound_behavior() {
    std::string s;
    s.assign(256, 'A'); // 256 repetitions of 'A'
    unsigned char res = sqlite3StrIHash(s.c_str());
    unsigned char expected = compute_expected_hash(s);
    TEST_EXPECT_EQ(res, expected, "Hash should wrap around modulo 256 for 256 identical chars");
}

// Test 6: Very long string to exercise performance characteristics (sanity check)
static void test_long_string() {
    // Build a long but simple string consisting of two characters repeating
    std::string part = "AZ";
    std::string s;
    s.reserve(1024);
    for (int i = 0; i < 128; ++i) s += part; // 128 repetitions of "AZ" -> 256 chars
    unsigned char res = sqlite3StrIHash(s.c_str());
    unsigned char expected = compute_expected_hash(s);
    TEST_EXPECT_EQ(res, expected, "Long string hash should match expected value from UpperToLower mapping");
}

// Entry point calling all tests
int main() {
    // Run tests in a simple, ordered fashion to maximize code coverage
    test_null_pointer();
    test_empty_string();
    test_basic_hash();
    test_non_letter_chars();
    test_wraparound_behavior();
    test_long_string();

    if (g_failures == 0) {
        std::cout << "All tests passed\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed\n";
        return 1;
    }
}