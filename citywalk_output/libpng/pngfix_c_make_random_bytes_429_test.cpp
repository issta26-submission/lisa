// pngfix_tests.cpp
// Lightweight C++11 test suite for the C function make_random_bytes
// without using Google Test. Provides minimal test harness and
// explanatory comments for each test case.
//
// Notes:
// - This test relies on libpng's type definitions (png_uint_32, png_byte, etc.)
// - The tests implement a small reference (pure C++) version of the
//   make_random_bytes algorithm to compare against the production function.
// - Main() runs all tests and reports summary results.
// - All tests aim to exercise true/false branches and edge cases where feasible.

#include <cstdint>
#include <cassert>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


extern "C" {
  // Production function under test (from pngfix.c)
  // Signature matches the C declaration in the source.
  void make_random_bytes(png_uint_32 *seed, void *pv, size_t size);
}

// libpng type aliases. Ensure we include libpng headers.

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_BEGIN(Name) static void Name(); static void Name##_run(); static int Name##_total = 0; static int Name##_failed = 0; \
  static void Name##_run() { Name(); } \
  static void NAME_##__COUNTER__##_register() { } \
  static void Name() { /* test body will be defined below */ } // Placeholder for readability

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        fprintf(stderr, "TEST FAILED: %s\n", msg); \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if((a) != (b)) { \
        fprintf(stderr, "TEST FAILED: %s | expected %lld, got %lld\n", msg, \
                static_cast<long long>(a), static_cast<long long>(b)); \
        ++g_failed_tests; \
    } \
} while(0)

// Reference implementation of make_random_bytes for validation
static void ref_make_random_bytes(png_uint_32 *seed, void *pv, size_t size)
{
    png_uint_32 u0 = seed[0], u1 = seed[1];
    png_bytep bytes = (png_bytep)pv;
    for (size_t i = 0; i < size; ++i)
    {
        // Reproduce the exact same bit-twiddling as production code
        png_uint_32 u = ((u0 >> (20 - 8)) ^ ((u1 << 7) | (u0 >> (32 - 7)))) & 0xff;
        u1 <<= 8;
        u1 |= u0 >> 24;
        u0 <<= 8;
        u0 |= u;
        bytes[i] = (png_byte)u;
    }
    seed[0] = u0;
    seed[1] = u1;
}

// Test 1: Reproducibility between production and reference implementations
// - Start from a known seed, generate N bytes, then regenerate with a fresh seed
//   and compare outputs for exact match.
static void test_reproducibility()
{
    // Seed and buffers
    png_uint_32 seed_prod[2] = { 0x01234567, 0x89abcdef };
    const size_t N = 32;
    unsigned char prod_buf[N];
    unsigned char ref_buf[N];

    // Produce using the actual function
    make_random_bytes(seed_prod, prod_buf, N);

    // Produce using the reference implementation on a fresh seed
    png_uint_32 seed_ref[2] = { 0x01234567, 0x89abcdef };
    ref_make_random_bytes(seed_ref, ref_buf, N);

    // Compare outputs
    for (size_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(prod_buf[i], ref_buf[i], "Output byte mismatch between production and reference");
    }

    // Seed should be updated identically after the run
    EXPECT_EQ(seed_prod[0], seed_ref[0], "Final seed[0] mismatch after generation");
    EXPECT_EQ(seed_prod[1], seed_ref[1], "Final seed[1] mismatch after generation");
}

// Test 2: Size = 0 should not modify output buffer or seed
static void test_size_zero()
{
    png_uint_32 seed[2] = { 0xAABBCCDD, 0x11223344 };
    unsigned char dst[8];
    // Initialize destination with a known pattern
    for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = static_cast<unsigned char>(0xFF);

    // Call with size 0; both buffer and seed should remain unchanged
    png_uint_32 seed_before_0[2] = { seed[0], seed[1] };
    make_random_bytes(seed, dst, 0);

    // Buffer unchanged
    for (size_t i = 0; i < sizeof(dst); ++i)
    {
        EXPECT_EQ(dst[i], 0xFF, "Buffer modified when size == 0");
    }

    // Seed unchanged
    EXPECT_EQ(seed[0], seed_before_0[0], "Seed[0] changed when size == 0");
    EXPECT_EQ(seed[1], seed_before_0[1], "Seed[1] changed when size == 0");
}

// Test 3: Consecutive calls using the same seed should match two-step reference
static void test_consecutive_calls()
{
    // First call and second call on the same seed
    png_uint_32 seed_prod[2] = { 0xCAFEBABE, 0xDEADBEEF };
    unsigned char buf_prod1[10];
    unsigned char buf_prod2[6];

    make_random_bytes(seed_prod, buf_prod1, 10);
    make_random_bytes(seed_prod, buf_prod2, 6);

    // Reference implementation: two consecutive calls starting from same initial seed
    png_uint_32 seed_ref[2] = { 0xCAFEBABE, 0xDEADBEEF };
    unsigned char ref_buf1[10];
    unsigned char ref_buf2[6];

    ref_make_random_bytes(seed_ref, ref_buf1, 10);
    ref_make_random_bytes(seed_ref, ref_buf2, 6);

    // Compare first chunk
    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_EQ(buf_prod1[i], ref_buf1[i], "First chunk mismatch between production and reference");
    }
    // Compare second chunk
    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(buf_prod2[i], ref_buf2[i], "Second chunk mismatch between production and reference");
    }

    // Seeds after both calls should match
    EXPECT_EQ(seed_prod[0], seed_ref[0], "Final seed[0] mismatch after consecutive runs");
    EXPECT_EQ(seed_prod[1], seed_ref[1], "Final seed[1] mismatch after consecutive runs");
}

// Entry point to run all tests
int main()
{
    // Run tests
    test_reproducibility();
    test_size_zero();
    test_consecutive_calls();

    // Summary
    printf("Total tests executed: %d\n", g_total_tests);
    if (g_failed_tests == 0)
    {
        printf("ALL TESTS PASSED\n");
        return 0;
    }
    else
    {
        printf("TESTS FAILED: %d\n", g_failed_tests);
        return 1;
    }
}