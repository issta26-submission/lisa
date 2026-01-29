// test_crc_init_4.cpp
// A lightweight C++11 test harness for the focal function crc_init_4
// in pngfix.c. The test uses a runtime-generated CRC-32 table (polynomial
// 0xEDB88320) as a reference implementation to verify the library's
// crc_init_4 behavior. This approach avoids hard-coding the 256-entry table
// and remains compatible with the common CRC-32 table used by zlib/libpng.
//
// Notes:
// - This test is designed to be compiled and linked with the real pngfix.c
//   source so that crc_init_4 resolves to the genuine implementation.
// - We avoid GTest; instead we implement a small, non-terminating assertion
//   style: we print per-test results and return a non-zero exit code if any
//   test fails.

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Declaration of the focal function from the C source under test.
// We assume the usual libpng typedefs map to uint32_t in practice.
extern "C" uint32_t crc_init_4(uint32_t value);

// Generate the standard CRC-32 table for polynomial 0xEDB88320.
static void generate_crc32_table(uint32_t table[256])
{
    const uint32_t POLY = 0xEDB88320u;
    for (int i = 0; i < 256; ++i) {
        uint32_t c = static_cast<uint32_t>(i);
        for (int j = 0; j < 8; ++j) {
            if (c & 1)
                c = (c >> 1) ^ POLY;
            else
                c >>= 1;
        }
        table[i] = c;
    }
}

// Reference implementation of crc_init_4 using the generated table.
// This mirrors the exact computation performed by the focal method.
static uint32_t crc_init_4_ref(uint32_t value, const uint32_t table[256])
{
    uint32_t crc = table[(~value >> 24) & 0xFF] ^ 0xFFFFFFu;
    crc = table[(crc ^ (value >> 16)) & 0xFF] ^ (crc >> 8);
    crc = table[(crc ^ (value >> 8)) & 0xFF] ^ (crc >> 8);
    return table[(crc ^ value) & 0xFF] ^ (crc >> 8);
}

// Convenience wrapper to compute expected value for a given input using a
// freshly generated CRC-32 table.
static uint32_t compute_expected(uint32_t value)
{
    uint32_t table[256];
    generate_crc32_table(table);
    return crc_init_4_ref(value, table);
}

// Test values covering edge cases and typical scenarios.
static const uint32_t test_values[] = {
    0x00000000u,       // all zeros
    0x00000001u,       // small value
    0x12345678u,       // typical value
    0x9ABCDEF0u,        // mid-range value
    0xFFFFFFFFu,       // all ones
    0x0000FFFFu,       // lower half ones
    0x00FF00FFu,       // mixed pattern
    0x80000000u,       // high bit set
    0xDEADBEEFu,        // arbitrary value
    0xA5A5A5A5u        // repeating pattern
};

static void run_single_test(size_t idx, uint32_t value, int &failures)
{
    uint32_t expected = compute_expected(value);
    uint32_t actual = crc_init_4(value);

    if (expected != actual) {
        // Explanatory output for failed test (non-terminating assertion style)
        printf("TEST %zu FAILED: value=0x%08X, expected=0x%08X, actual=0x%08X\n",
               idx, value, expected, actual);
        ++failures;
    } else {
        // Non-failing pass message for coverage trace
        printf("TEST %zu PASSED: value=0x%08X\n", idx, value);
    }
}

int main(void)
{
    // Describe what we test
    printf("Starting test suite for crc_init_4(value) in pngfix.c\n");

    int failures = 0;
    const size_t N = sizeof(test_values) / sizeof(test_values[0]);

    // Iterate test cases
    for (size_t i = 0; i < N; ++i) {
        run_single_test(i, test_values[i], failures);
    }

    if (failures > 0) {
        printf("CRC_INIT_4 TEST SUMMARY: %d failure(s) detected.\n", failures);
        return 1;
    } else {
        printf("CRC_INIT_4 TEST SUMMARY: All tests PASSED.\n");
        return 0;
    }
}