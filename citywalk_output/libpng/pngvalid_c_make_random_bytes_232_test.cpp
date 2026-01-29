/*
Unit test suite for the focal method:
  make_random_bytes(png_uint_32 *seed, void *pv, size_t size)

Notes:
- This test harness is written in C++11 and does not rely on any testing framework.
- It calls the C function implemented in the PNG test code (make_random_bytes) via extern "C".
- A minimal reference implementation (ref_make_random_bytes and ref_final_seed) is provided
  to verify correctness deterministically against the original algorithm.
- Tests cover:
  1) size = 0 (no bytes produced; seeds unchanged)
  2) a small non-zero size (4 bytes)
  3) a larger non-zero size (16 bytes)
  4) edge case with all-zero seeds
- Each test includes explanatory comments and uses non-terminating assertions (collects failures
  and prints a summary at the end).

To compile (example):
  g++ -std=c++11 -I<path_to_libpng_include> -L<path_to_libpng_lib> -lpng -lpng16 -o test_make_random_bytes test_make_random_bytes.cpp
*/

#include <cstdint>
#include <cstddef>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Include libpng types for png_uint_32, png_bytep, etc.

// Declare the focal function from the C source (ensure C linkage)
extern "C" void make_random_bytes(png_uint_32 *seed, void *pv, size_t size);

// Small, self-contained reference implementations to validate behavior

// Reference: produce exactly the same bytes as make_random_bytes for given seed/size
static void ref_make_random_bytes(const png_uint_32 seed_in[2],
                                  unsigned char *out, size_t size)
{
    uint32_t u0 = seed_in[0], u1 = seed_in[1];
    for (size_t i = 0; i < size; ++i) {
        // u = ((u0 >> (20-8)) ^ ((u1 << 7) | (u0 >> (32-7)))) & 0xff;
        // 20-8 = 12, 32-7 = 25
        uint32_t u = ((u0 >> 12) ^ ((u1 << 7) | (u0 >> 25))) & 0xff;
        u1 <<= 8;
        u1 |= u0 >> 24;
        u0 <<= 8;
        u0 |= u;
        out[i] = (unsigned char)u;
    }
}

// Reference: compute the final seed after size iterations
static void ref_final_seed(const png_uint_32 seed_in[2], size_t size,
                           png_uint_32 seed_out[2])
{
    uint32_t u0 = seed_in[0], u1 = seed_in[1];
    for (size_t i = 0; i < size; ++i) {
        uint32_t u = ((u0 >> 12) ^ ((u1 << 7) | (u0 >> 25))) & 0xff;
        u1 <<= 8;
        u1 |= u0 >> 24;
        u0 <<= 8;
        u0 |= u;
    }
    seed_out[0] = u0;
    seed_out[1] = u1;
}

// Minimal assertion/logging helpers (non-terminating)
static int g_failures = 0;

static void log_failure(const char* test_name, const char* message,
                        const char* file, int line)
{
    std::cerr << "FAIL: " << test_name << " - " << message
              << " (" << file << ":" << line << ")\n";
    ++g_failures;
}

#define ASSERT_TRUE(cond, test_name, message) \
    do { \
        if (!(cond)) log_failure(test_name, message, __FILE__, __LINE__); \
    } while (0)

#define ASSERT_MEM_EQ(expected, actual, size, test_name) \
    do { \
        int _cmp = std::memcmp((expected), (actual), (size)); \
        if (_cmp != 0) { \
            log_failure(test_name, "memory contents differ", __FILE__, __LINE__); \
        } \
    } while (0)

#define ASSERT_EQ_UINT(a, b, test_name) \
    do { \
        if (static_cast<uint64_t>(a) != static_cast<uint64_t>(b)) { \
            std::cerr << "  " << test_name << ": expected " << b \
                      << ", got " << a << "\n"; \
            ++g_failures; \
        } \
    } while (0)


// Test 1: size = 0 should produce no bytes and leave seeds unchanged
static void test_make_random_bytes_size_zero()
{
    // Explain test
    // Ensure that providing a size of 0 does not modify output buffer (no writes)
    // and does not alter the seed values.
    const char* test_name = "test_make_random_bytes_size_zero";

    png_uint_32 seed[2] = {0x12345678u, 0x9abcdef0u};
    png_uint_32 seed_before[2] = {seed[0], seed[1]};

    // size = 0, output buffer can be NULL
    make_random_bytes(seed, nullptr, 0);

    // Seed should remain unchanged
    ASSERT_EQ_UINT(seed[0], seed_before[0], test_name);
    ASSERT_EQ_UINT(seed[1], seed_before[1], test_name);
}


// Test 2: size = 4 with a non-trivial seed
static void test_make_random_bytes_size_four()
{
    const char* test_name = "test_make_random_bytes_size_four";

    // Initial seed
    png_uint_32 seed_in[2] = {0x01234567u, 0x89abcdefu};
    png_uint_32 seed_copy[2] = {seed_in[0], seed_in[1]};

    unsigned char out[4];
    make_random_bytes(seed_copy, static_cast<void*>(out), 4);

    // Expected output using reference implementation
    unsigned char expected_out[4];
    ref_make_random_bytes(seed_in, expected_out, 4);

    ASSERT_MEM_EQ(expected_out, out, 4, test_name);

    // Expected final seeds after 4 iterations
    png_uint_32 expected_seed[2];
    ref_final_seed(seed_in, 4, expected_seed);
    ASSERT_EQ_UINT(seed_copy[0], expected_seed[0], test_name);
    ASSERT_EQ_UINT(seed_copy[1], expected_seed[1], test_name);
}


// Test 3: size = 16 with another non-trivial seed
static void test_make_random_bytes_size_sixteen()
{
    const char* test_name = "test_make_random_bytes_size_sixteen";

    // Initial seed
    png_uint_32 seed_in[2] = {0xdeadbeefu, 0xfeedfaceu};
    png_uint_32 seed_copy[2] = {seed_in[0], seed_in[1]};

    unsigned char out[16];
    make_random_bytes(seed_copy, static_cast<void*>(out), 16);

    // Expected output
    unsigned char expected_out[16];
    ref_make_random_bytes(seed_in, expected_out, 16);
    ASSERT_MEM_EQ(expected_out, out, 16, test_name);

    // Expected final seeds after 16 iterations
    png_uint_32 expected_seed[2];
    ref_final_seed(seed_in, 16, expected_seed);
    ASSERT_EQ_UINT(seed_copy[0], expected_seed[0], test_name);
    ASSERT_EQ_UINT(seed_copy[1], expected_seed[1], test_name);
}


// Test 4: edge case with all-zero seed
static void test_make_random_bytes_all_zero_seed()
{
    const char* test_name = "test_make_random_bytes_all_zero_seed";

    png_uint_32 seed_in[2] = {0u, 0u};
    png_uint_32 seed_copy[2] = {seed_in[0], seed_in[1]};

    unsigned char out[8];
    make_random_bytes(seed_copy, static_cast<void*>(out), 8);

    // Expect all outputs to be zero
    unsigned char expected_out[8];
    std::memset(expected_out, 0, 8);
    ASSERT_MEM_EQ(expected_out, out, 8, test_name);

    // Final seed should remain zero
    ASSERT_EQ_UINT(seed_copy[0], 0u, test_name);
    ASSERT_EQ_UINT(seed_copy[1], 0u, test_name);
}


// Main: run all tests and print a summary
int main(void)
{
    std::cout << "Starting unit tests for make_random_bytes...\n";

    test_make_random_bytes_size_zero();
    test_make_random_bytes_size_four();
    test_make_random_bytes_size_sixteen();
    test_make_random_bytes_all_zero_seed();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}