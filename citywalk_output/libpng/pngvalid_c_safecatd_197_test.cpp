/*
Unit Test Suite for safecatd (C function) using C++11

Overview:
- Purpose: Validate safecatd(char *buffer, size_t bufsize, size_t pos, double d, int precision)
  which formats the double d using "%.*f" with the given precision and then delegates
  to safecat(buffer, bufsize, pos, number).

- Approach:
  - Use a minimal, self-contained C++ test harness (no GTest).
  - Treat safecat and safecatd as C functions exposed to C++ via extern "C".
  - Initialize buffers with zeros to have well-defined string semantics.
  - Test true branches by verifying the exact formatted string is placed at the expected position.
  - Tests focus on:
    1) Basic formatting correctness with positive numbers and finite precision.
    2) Writing at an offset (pos > 0) while preserving existing content.
    3) Negative numbers with various precisions, including precision 0.

Notes:
- The tests assume safecat returns a size_t (as it delegates to safecat) and that
  the buffer is treated as a C-string after safecatd is called with pos = 0.
- The actual safecat implementation must be linked into the test binary (as provided by pngvalid.c or its compiled object).
- We provide explanatory comments for each test case.

Compile (example):
g++ -std=c++11 -O2 -Wall -Wextra -Wno-sign-compare tests_safecatd.cpp -o test_safecatd -lpng -lz

Depending on your build system, you may need to link in the C file that defines safecat/safecatd
or compile it into the same translation unit.
*/

#include <cstddef>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Forward declarations of the C functions to be tested.
// Assuming the functions use C linkage in the PNG codebase.
extern "C" {
    // The actual signature may return size_t. We assume size_t here to match typical usage.
    size_t safecat(char *buffer, size_t bufsize, size_t pos, const char *cat);
    size_t safecatd(char *buffer, size_t bufsize, size_t pos, double d, int precision);
}

// Simple test framework (minimal, no external testing libraries)
static int g_failures = 0;

#define ASSERT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "Assertion failed: " #cond " in " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define ASSERT_STR_EQ(actual, expected) \
    do { \
        if(std::strcmp((actual), (expected)) != 0) { \
            std::cerr << "Assertion failed: expected \"" << (expected) \
                      << "\" but got \"" << (actual) \
                      << "\" in " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)


// Test 1: Basic formatting at position 0 with positive number and precision
// Expectation: buffer becomes the exact string formatted by "%.*f" with given precision.
void test_safecatd_basic_positive_formatting() {
    // Candidate Keywords: buffer, pos, precision, d, number string, "%.*f", safecatd
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));

    double input = 3.14159;
    int precision = 2; // expect "3.14"

    size_t ret = safecatd(buffer, sizeof(buffer), 0, input, precision);

    // The immediate expectation is that the buffer now begins with "3.14"
    ASSERT_STR_EQ(buffer, "3.14");

    // Optional: ensure a return value is produced (non-zero indicates some positioning)
    // We don't strictly rely on ret value for correctness, but it provides extra safety.
    ASSERT_TRUE(ret > 0);
}

// Test 2: Writing at an offset (pos > 0) should not clobber initial content
// Expectation: buffer starts with pre-filled content (e.g., "AB") and the formatted
// number is written right after at position pos.
void test_safecatd_write_at_offset_preserves_prefix() {
    // Candidate Keywords: buffer, pos, d, precision, number, offset write
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));

    // Pre-fill prefix
    buffer[0] = 'A';
    buffer[1] = 'B';
    buffer[2] = '\0'; // keep as C-string terminator after "AB" for clarity

    double input = 1.0;
    int precision = 0; // expect "1" with no decimal part

    size_t ret = safecatd(buffer, sizeof(buffer), 2, input, precision);

    // After writing at offset 2, the buffer should be "AB" followed by "1" and terminator
    ASSERT_STR_EQ(buffer, "AB1");

    // Basic sanity on return value
    ASSERT_TRUE(ret > 2);
}

// Test 3: Negative numbers with decimal precision (e.g., -2.50 with precision 2)
void test_safecatd_negative_number_with_precision() {
    // Candidate Keywords: negative d, precision handling, format sign
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));

    double input = -2.5;
    int precision = 2; // expect "-2.50"

    size_t ret = safecatd(buffer, sizeof(buffer), 0, input, precision);

    ASSERT_STR_EQ(buffer, "-2.50");
    ASSERT_TRUE(ret > 0);
}

// Test 4: Precision zero formatting
// Ensure that with precision 0, fractional part is omitted and number is rounded appropriately
void test_safecatd_precision_zero() {
    // Candidate Keywords: precision 0 formatting, rounding behavior
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));

    double input = 5.9;
    int precision = 0; // expect "6" due to rounding of 5.9

    size_t ret = safecatd(buffer, sizeof(buffer), 0, input, precision);

    // Depending on rounding behavior, expect "6"
    ASSERT_STR_EQ(buffer, "6");
    ASSERT_TRUE(ret > 0);
}

void run_all_tests() {
    std::cout << "Starting safecatd test suite...\n";

    test_safecatd_basic_positive_formatting();
    test_safecatd_write_at_offset_preserves_prefix();
    test_safecatd_negative_number_with_precision();
    test_safecatd_precision_zero();

    if(g_failures == 0) {
        std::cout << "All safecatd tests passed.\n";
    } else {
        std::cerr << g_failures << " safecatd test(s) failed.\n";
    }
}

int main() {
    run_all_tests();
    return g_failures ? 1 : 0;
}