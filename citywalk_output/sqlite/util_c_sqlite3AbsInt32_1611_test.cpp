// Test suite for sqlite3AbsInt32 implemented in util.c
// This test is designed to be compiled with a C++11 compiler without GoogleTest.
// It uses a lightweight, non-terminating assertion framework to maximize code coverage.

#include <math.h>
#include <climits>
#include <limits>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>

extern "C" int sqlite3AbsInt32(int x); // function under test (C linkage)

// Lightweight non-terminating assertion macro.
// It records failures but continues executing to improve coverage.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_EQ(val, exp) do { \
    ++g_total_tests; \
    auto _v = (val); \
    auto _e = (exp); \
    if (_v != _e) { \
        ++g_failed_tests; \
        std::cerr << "ASSERT FAILED at line " << __LINE__ << ": " \
                  << "actual(" << _v << ") != expected(" << _e << ")." \
                  << std::endl; \
    } \
} while(0)

// Optional helper to print summary of tests.
static void print_summary() {
    std::cout << "Test Summary: " << (g_total_tests - g_failed_tests)
              << " / " << g_total_tests << " tests passed."
              << std::endl;
    if (g_failed_tests > 0) {
        std::cerr << g_failed_tests << " test(s) FAILED." << std::endl;
    } else {
        std::cout << "All tests PASSED." << std::endl;
    }
}

int main() {
    // Candidate Keywords (from Step 1)
    // sqlite3AbsInt32, INT_MIN, INT_MAX, 0x80000000 path (special-case for int min),
    // abs-like behavior for negative numbers, non-negative numbers remain unchanged.

    // Test 1: Non-negative inputs should be returned as-is.
    // true branch: x >= 0
    ASSERT_EQ(sqlite3AbsInt32(0), 0);
    ASSERT_EQ(sqlite3AbsInt32(5), 5);
    ASSERT_EQ(sqlite3AbsInt32(12345), 12345);
    ASSERT_EQ(sqlite3AbsInt32(INT_MAX), INT_MAX);

    // Test 2: Negative inputs (not INT_MIN) should be negated.
    // false branch before min-case: x < 0 and x != INT_MIN
    ASSERT_EQ(sqlite3AbsInt32(-1), 1);
    ASSERT_EQ(sqlite3AbsInt32(-3), 3);
    ASSERT_EQ(sqlite3AbsInt32(-INT_MAX), INT_MAX); // -(-2147483647) = 2147483647
    // Also test a mid-range negative value
    ASSERT_EQ(sqlite3AbsInt32(-123456), 123456);

    // Test 3: Special-case when x is INT_MIN (the implementation uses 0x80000000 cast to int)
    // In two's complement systems, INT_MIN == (int)0x80000000
    // The function should return INT_MAX in this case.
    ASSERT_EQ(sqlite3AbsInt32(INT_MIN), INT_MAX);

    // Summary
    print_summary();
    return (g_failed_tests == 0) ? 0 : 1;
}