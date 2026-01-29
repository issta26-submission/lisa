// Test suite for the focal method read_wh in genpng.c
// This test suite is designed to be compiled with C++11 and linked against the
// project's genpng.c (which defines read_wh) and the project's png.h.
// It avoids using GTest and uses a lightweight, non-terminating assertion system.

#include <cstdint>
#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Step 1: Bring in the PNG type definitions and the function under test.
// We declare the function with C linkage to match the expected symbol in the C file.
extern "C" {
}

// Declaration of the focal function under test (C linkage)
extern "C" png_uint_32 read_wh(const char *name, const char *str);

// Lightweight assertion mechanism (non-terminating)
static int g_fail_count = 0;
#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " (" << static_cast<long long>(a) \
                  << ") != " << #b << " (" << static_cast<long long>(b) << ")\n"; \
        ++g_fail_count; \
    } \
} while (0)

#define TEST_OK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST_FAIL: " << (msg) << "\n"; \
        ++g_fail_count; \
    } \
} while (0)

/*
Candidate Keywords (Step 1 insights):
- strtoul: standard C function used to convert string to unsigned long
- ep (end pointer): tracks the end position after conversion
- ul: the converted unsigned long value
- 0x7fffffff: upper bound for valid values
- name, str: inputs to read_wh
- fprintf(stderr, ...): error reporting path (not exercised with exit in valid tests)
- exit(1): termination path for invalid input (not triggered by valid tests)
These keywords reflect the function's core dependency on string-to-number parsing with
range validation and error signaling.
*/

// Test 1: Basic valid input "1" should return 1 without exiting.
bool test_read_wh_basic_one() {
    // Purpose: Verify that a simple, valid numeric string is parsed correctly
    png_uint_32 v = read_wh("width", "1");
    EXPECT_EQ(v, (png_uint_32)1);
    TEST_OK(v == 1, "read_wh should return 1 for input \"1\"");
    return g_fail_count == 0;
}

// Test 2: Leading zeros should be handled correctly by strtoul.
// Purpose: Ensure that numbers with leading zeros (e.g., "0005") are parsed as 5.
bool test_read_wh_leading_zeros() {
    png_uint_32 v = read_wh("width", "0005");
    EXPECT_EQ(v, (png_uint_32)5);
    TEST_OK(v == 5, "read_wh should return 5 for input \"0005\"");
    return g_fail_count == 0;
}

// Test 3: Maximum valid value (0x7fffffff) should be accepted and returned.
// Purpose: Validate upper-bound boundary handling.
bool test_read_wh_max_bound() {
    png_uint_32 v = read_wh("height", "2147483647"); // 0x7FFFFFFF
    EXPECT_EQ(v, (png_uint_32)2147483647);
    TEST_OK(v == 2147483647, "read_wh should return 2147483647 for input \"2147483647\"");
    return g_fail_count == 0;
}

// Main entry point for tests
int main() {
    // Run tests in sequence
    // Note: We intentionally avoid tests that would call exit(1) by using only valid inputs.
    test_read_wh_basic_one();
    test_read_wh_leading_zeros();
    test_read_wh_max_bound();

    // Summary
    if (g_fail_count > 0) {
        std::cerr << "Total failing tests: " << g_fail_count << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}