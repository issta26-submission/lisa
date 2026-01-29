// Unit test suite for sqlite3Atoi64 (located in util.c)
// This test harness is a lightweight C++11 test suite (no Google Test) that
// exercises various code paths of sqlite3Atoi64 using a minimal in-file test
// framework. It relies on the project-provided dependencies (sqlite3Isspace,
// sqlite3Atoi64, constants SQLITE_UTF8/SQLITE_UTF16LE/SQLITE_UTF16BE, etc.)
// but keeps the tests self-contained and non-terminating on failure.
//
// Important notes (mapped to the provided <DOMAIN_KNOWLEDGE> rules):
// - We import necessary dependencies with correct paths (sqlite3.h) to obtain
//   SQLITE_UTF8, etc., and declare the sqlite3Atoi64 prototype for linkage.
// - We cover true/false branches of predicates where feasible for utf8 input.
// - We access non-static symbols from the test (extern "C" linkage for C APIs).
// - We use only the C++ standard library and provided project methods.
// - We do not rely on GoogleTest; we implement a tiny assertion framework.
// - We distinguish between value and address expectations via proper types.
// - All tests run from main(); static facilities inside sqlite3Atoi64 are not
//   mocked; we exercise the observable behavior through public API.
// - The tests focus on the focal function and its primary code paths (UTF8 path
//   and edge cases like spaces, signs, leading zeros, overflow, etc.).
//
// How to build (example):
//   g++ -std=c++11 -I<path_to_sqlite_headers> -c test_sqlite3Atoi64.cpp
//   g++ -std=c++11 test_sqlite3Atoi64.o -L<path_to_sqlite_lib> -lsqlite3 -o test_sqlite3Atoi64
// Run: ./test_sqlite3Atoi64

#include <math.h>
#include <climits>
#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal C function under test.
// We assume that the project provides the actual implementation in util.c and
// that the calling convention is C linkage.
extern "C" int sqlite3Atoi64(const char *zNum, long long *pNum, int length, unsigned char enc);

// Include sqlite3.h to obtain necessary constants/macros such as encodings.

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
#define TEST_FAIL_MSG_PREFIX "Test failure"
#define EXPECT_EQ(actual, expected, msg) do { \
    if((actual) != (expected)) { \
        std::cerr << "FAIL: " << (msg) << " | expected: " << (expected) \
                  << ", got: " << (actual) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_LL(actual, expected, msg) EXPECT_EQ((long long)(actual), (long long)(expected), (msg))
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { std::cerr << "FAIL: " << (msg) << std::endl; ++g_failures; } } while(0)


// Test 1: Basic positive number parsing in UTF-8
// Input: "12345" -> pNum = 12345, rc = 0
void test_basic_positive() {
    long long v = 0;
    const char *s = "12345";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for basic positive number (UTF-8)");
    EXPECT_EQ_LL(v, 12345LL, "value for basic positive number (UTF-8)");
}

// Test 2: Leading spaces should be skipped
// Input: "   987" -> pNum = 987, rc = 0
void test_leading_spaces() {
    long long v = 0;
    const char *s = "   987";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for leading spaces");
    EXPECT_EQ_LL(v, 987LL, "value after skipping spaces");
}

// Test 3: Negative sign handling
// Input: "-42" -> pNum = -42, rc = 0
void test_negative_sign() {
    long long v = 0;
    const char *s = "-42";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for negative sign");
    EXPECT_EQ_LL(v, -42LL, "value for negative sign");
}

// Test 4: Plus sign handling
// Input: "+7" -> pNum = 7, rc = 0
void test_plus_sign() {
    long long v = 0;
    const char *s = "+7";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for plus sign");
    EXPECT_EQ_LL(v, 7LL, "value for plus sign");
}

// Test 5: Leading zeros should not affect value
// Input: "000123" -> pNum = 123, rc = 0
void test_leading_zeros() {
    long long v = 0;
    const char *s = "000123";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for leading zeros");
    EXPECT_EQ_LL(v, 123LL, "value for leading zeros");
}

// Test 6: Overflow positive (2^63) -> rc = 2
// Input: "9223372036854775808" -> overflow
void test_overflow_positive() {
    long long v = 0;
    const char *s = "9223372036854775808"; // 19 digits, one more than max
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    // Expect overflow return code 2
    EXPECT_EQ_LL(rc, 2, "rc for overflow positive");
    // Value should be clamped to LARGEST_INT64
    EXPECT_EQ_LL(v, LLONG_MAX, "clamped value on overflow (positive)");
}

// Test 7: Minimum negative value
// Input: "-9223372036854775808" -> rc = 0, pNum = LLONG_MIN
void test_min_negative() {
    long long v = 0;
    const char *s = "-9223372036854775808"; // min int64
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for min negative");
    EXPECT_EQ_LL(v, LLONG_MIN, "value equals LLONG_MIN");
}

// Test 8: Empty string (no digits) -> rc = -1
void test_no_digits() {
    long long v = 0;
    const char *s = "";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, -1, "rc for empty input (no digits)");
}

// Test 9: Trailing non-space characters after digits -> rc = 1
// Input: "123a" -> rc = 1
void test_trailing_non_digit_after_digits() {
    long long v = 0;
    const char *s = "123a";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 1, "rc for trailing non-digit after digits");
    EXPECT_EQ_LL(v, 123LL, "value for trailing non-digit after digits");
}

// Test 10: Trailing spaces after digits should be allowed (rc == 0)
void test_trailing_spaces_ok() {
    long long v = 0;
    const char *s = "123   ";
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 0, "rc for trailing spaces after digits");
    EXPECT_EQ_LL(v, 123LL, "value for digits with trailing spaces");
}

// Test 11: Overflow negative (beyond LLONG_MIN) -> rc = 2
// Input: "-18446744073709551616" (1 more than max unsigned 64, negative path)
// This number has 20 digits; parsing should overflow and return 2.
void test_overflow_negative() {
    long long v = 0;
    const char *s = "-18446744073709551616"; // 20 digits, negative overflow
    int rc = sqlite3Atoi64(s, &v, (int)strlen(s), SQLITE_UTF8);
    EXPECT_EQ_LL(rc, 2, "rc for overflow negative (too small)");
    EXPECT_EQ_LL(v, LLONG_MIN, "value on overflow negative (clamped to LLONG_MIN)");
}

// Runner to execute all tests
void run_all_tests() {
    test_basic_positive();
    test_leading_spaces();
    test_negative_sign();
    test_plus_sign();
    test_leading_zeros();
    test_overflow_positive();
    test_min_negative();
    test_no_digits();
    test_trailing_non_digit_after_digits();
    test_trailing_spaces_ok();
    test_overflow_negative();
}

int main() {
    // Explain what is being tested (in code comments above)
    std::cout << "sqlite3Atoi64 unit tests starting...\n";

    run_all_tests();

    if(g_failures == 0) {
        std::cout << "All sqlite3Atoi64 tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " sqlite3Atoi64 test(s) failed.\n";
        return 1;
    }
}