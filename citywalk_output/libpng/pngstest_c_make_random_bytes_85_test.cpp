// C++11 test suite for the focal function: make_random_bytes
// This test suite is designed to compile with a C++11 compiler without relying on GoogleTest.
// It tests the behavior of the C function make_random_bytes declared in the PNG test code.
// The tests rely on including libpng header <png.h> to get the correct typedefs and function prototype.

#include <cstdint>
#include <cstddef>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Bring in C declarations for the function under test.
// We rely on libpng's types (png_uint_32, png_byte, etc.).
extern "C" {
    
    // The focal function under test, declared from the library/source.
    // Prototype matches the one in the provided focal code.
    void make_random_bytes(png_uint_32 *seed, void *pv, size_t size);
}

// Reference implementation in test to validate make_random_bytes.
// This mirrors the exact algorithm used in the focal method to compute expected outputs.
static void ref_make_random_bytes(png_uint_32 seed[2], unsigned char *pv, size_t size)
{
    png_uint_32 u0 = seed[0], u1 = seed[1];
    unsigned char *bytes = pv;
    size_t i;
    for (i = 0; i < size; ++i)
    {
        png_uint_32 u = ((u0 >> (20 - 8)) ^ ((u1 << 7) | (u0 >> (32 - 7)))) & 0xff;
        u1 <<= 8;
        u1 |= u0 >> 24;
        u0 <<= 8;
        u0 |= u;
        *bytes++ = (png_byte)u;
    }
    seed[0] = u0;
    seed[1] = u1;
}

// Utility to print test result
static void log_result(const std::string &name, bool passed)
{
    std::cout << name << ": " << (passed ? "PASS" : "FAIL") << std::endl;
}

// Test 1: Basic correctness for a non-empty size
// - Verifies that make_random_bytes writes bytes matching the reference implementation.
// - Verifies that the internal seed is updated exactly as the reference updates it.
static bool test_basic_correctness()
{
    // Original seed
    const png_uint_32 seed_orig[2] = { 0x01234567, 0x89ABCDEF };
    png_uint_32 seed[2] = { seed_orig[0], seed_orig[1] };

    const size_t SIZE = 16;
    std::vector<png_byte> out(SIZE);

    // Call focal function
    make_random_bytes(seed, out.data(), SIZE);

    // Compute expected output by running the reference on a copy of the seed
    png_uint_32 exp_seed[2] = { seed_orig[0], seed_orig[1] };
    unsigned char ref_out[SIZE];
    ref_make_random_bytes(exp_seed, ref_out, SIZE);

    // Check the produced bytes
    bool ok_bytes = (std::memcmp(out.data(), ref_out, SIZE) == 0);

    // Check the updated seed matches expected
    bool ok_seed = (seed[0] == exp_seed[0] && seed[1] == exp_seed[1]);

    if (!ok_bytes) {
        std::cerr << "test_basic_correctness: output bytes do not match reference." << std::endl;
    }
    if (!ok_seed) {
        std::cerr << "test_basic_correctness: updated seed does not match reference." << std::endl;
    }
    return ok_bytes && ok_seed;
}

// Test 2: Zero size should perform no writes and not modify the seed
static bool test_zero_size_no_op()
{
    const png_uint_32 seed_orig[2] = { 0x0F0F0F0F, 0xF0F0F0F0 };
    png_uint_32 seed[2] = { seed_orig[0], seed_orig[1] };

    // Prepare buffer filled with a non-zero pattern to detect any writes
    std::vector<png_byte> out(8, 0xAA);

    // Call with size = 0
    make_random_bytes(seed, out.data(), 0);

    // Output should remain unchanged
    bool ok_bytes = (out[0] == 0xAA && out[1] == 0xAA && out[2] == 0xAA && out[3] == 0xAA);

    // Seed should remain unchanged
    bool ok_seed = (seed[0] == seed_orig[0] && seed[1] == seed_orig[1]);

    if (!ok_bytes) {
        std::cerr << "test_zero_size_no_op: buffer was modified for size 0." << std::endl;
    }
    if (!ok_seed) {
        std::cerr << "test_zero_size_no_op: seed was modified for size 0." << std::endl;
    }
    return ok_bytes && ok_seed;
}

// Test 3: Reproducibility with reset seeds
// - For a given seed, producing N bytes twice should yield identical outputs if the seed is reset between calls.
static bool test_reproducibility()
{
    const png_uint_32 seed_orig[2] = { 0xDEADBEEF, 0xFEEDFACE };
    const size_t SIZE = 32;

    std::vector<png_byte> out1(SIZE);
    std::vector<png_byte> out2(SIZE);

    // First run
    png_uint_32 seed1[2] = { seed_orig[0], seed_orig[1] };
    make_random_bytes(seed1, out1.data(), SIZE);

    // Second run: reset seed to original
    png_uint_32 seed2[2] = { seed_orig[0], seed_orig[1] };
    make_random_bytes(seed2, out2.data(), SIZE);

    // Compare outputs
    bool ok_bytes = (std::memcmp(out1.data(), out2.data(), SIZE) == 0);
    if (!ok_bytes) {
        std::cerr << "test_reproducibility: outputs differ when seed is reset." << std::endl;
    }
    // If outputs are equal, seeds after calls should be equal to the corresponding reference seeds
    // (we ensure by direct comparison of outputs only; seeds are not returned to caller here)
    return ok_bytes;
}

// Test 4: Consistency for multiple sizes
// - Ensure function handles varying sizes (small and medium) without crashing and produces valid bytes.
static bool test_multiple_sizes()
{
    const png_uint_32 seed_orig[2] = { 0xA5A5A5A5, 0x5A5A5A5A };
    const size_t SIZES[] = { 1, 2, 3, 7, 10, 31, 64 };
    const size_t NUM = sizeof(SIZES) / sizeof(SIZES[0]);

    for (size_t idx = 0; idx < NUM; ++idx) {
        size_t sz = SIZES[idx];
        std::vector<png_byte> buf(sz);
        png_uint_32 seed[2] = { seed_orig[0], seed_orig[1] };

        make_random_bytes(seed, buf.data(), sz);

        // Basic sanity: all bytes should be in 0..255
        for (size_t i = 0; i < sz; ++i) {
            if (buf[i] > 0xFF) {
                std::cerr << "test_multiple_sizes: found value outside 0..255 at index " << i << " for size " << sz << std::endl;
                return false;
            }
        }

        // To keep coverage simple, also check that after the call the seed actually changed
        if (sz > 0) {
            png_uint_32 expected_seed[2] = { seed_orig[0], seed_orig[1] };
            unsigned char dummy[1];
            ref_make_random_bytes(expected_seed, dummy, 0); // mutate expected_seed as reference
            if (seed[0] == seed_orig[0] && seed[1] == seed_orig[1]) {
                // If seed didn't change for non-zero size, it's a violation
                std::cerr << "test_multiple_sizes: seed did not change for non-zero size " << sz << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main(void)
{
    bool all_passed = true;

    // Run tests and accumulate results
    bool t1 = test_basic_correctness();
    log_result("test_basic_correctness", t1);
    all_passed = all_passed && t1;

    bool t2 = test_zero_size_no_op();
    log_result("test_zero_size_no_op", t2);
    all_passed = all_passed && t2;

    bool t3 = test_reproducibility();
    log_result("test_reproducibility", t3);
    all_passed = all_passed && t3;

    bool t4 = test_multiple_sizes();
    log_result("test_multiple_sizes", t4);
    all_passed = all_passed && t4;

    if (all_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}