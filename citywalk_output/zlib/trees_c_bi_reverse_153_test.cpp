// bi_reverse_test.cpp
// C++11 unit test suite for the focal method bi_reverse in trees.c
// This test is designed to be compiled alongside the C sources (e.g., trees.c)
// and linked using C linkage for bi_reverse.
// No GTest is used; a lightweight test harness with non-terminating assertions is provided.

#include <iostream>
#include <deflate.h>
#include <cstdint>


// Forward declaration of the focal function under test.
// We assume file-local (static) keyword 'local' is defined away in the build (as in the
// provided code snippet) so that bi_reverse has external linkage for testing.
// If your build defines 'local' as static, ensure the symbol is exposed for testing.
extern "C" unsigned bi_reverse(unsigned code, int len);

// Reference implementation (cracker-functional baseline) to validate bi_reverse.
// It reverses the lower 'len' bits of 'code' (up to 32 bits).
static unsigned bi_reverse_ref(unsigned code, int len)
{
    if (len <= 0) return 0;
    unsigned res = 0;
    // Only the lower 32 bits are meaningful for unsigned int
    int max_bits = (len < 32) ? len : 32;
    for (int i = 0; i < max_bits; ++i) {
        if (code & (1u << i)) {
            res |= 1u << (len - 1 - i);
        }
    }
    // If len > 32, higher bits of the result cannot be represented in 32-bit unsigned
    // and are effectively ignored; mirror the behavior by returning the computed value.
    return res;
}

// Lightweight testing framework (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion: checks that a and b are equal, logs failure but continues.
static void expect_equal_uint(unsigned a, unsigned b, const char* msg)
{
    ++g_total;
    if (a != b) {
        ++g_failed;
        std::cerr << "FAIL: " << msg
                  << " | expected " << b << ", got " << a << std::endl;
    } else {
        // Optional: Uncomment to enable pass logging
        // std::cout << "PASS: " << msg << std::endl;
    }
}

// Test 1: Basic known values to verify correctness on small, tangible cases.
// - bi_reverse(13, 4) should reverse the lower 4 bits of 13 (0b1101) -> 0b1011 (11).
// - bi_reverse(3, 4) should reverse 0b0011 -> 0b1100 (12).
// - bi_reverse(5, 1) should return the LSB of 5, i.e., 1.
static void test_bi_reverse_basic_cases()
{
    // 13 (0b1101) with len=4 -> expected 11 (0b1011)
    expect_equal_uint(bi_reverse(13u, 4), bi_reverse_ref(13u, 4),
                      "bi_reverse(13,4) should reverse lower 4 bits (expected 11)");
    // 3 (0b0011) with len=4 -> expected 12 (0b1100)
    expect_equal_uint(bi_reverse(3u, 4), bi_reverse_ref(3u, 4),
                      "bi_reverse(3,4) should reverse lower 4 bits (expected 12)");
    // len=1 should return LSB
    expect_equal_uint(bi_reverse(5u, 1), bi_reverse_ref(5u, 1),
                      "bi_reverse(5,1) should return LSB of code (expected 1)");
}

// Test 2: Additional cases to exercise edge conditions and larger bit widths.
// - 0xF0F0F0F0 reversed across 32 bits should be 0x0F0F0F0F.
// - 0x12345678 with len=8 should reverse lower 8 bits to 0x1E.
// - 0xFFFFFFFF with len=16 should remain 0xFFFF when considering 16 bits.
// - 0xAAAAAAAA with len=4 tests nibble reversal behavior on a repeating pattern.
static void test_bi_reverse_more_cases()
{
    // 0 -> 0 for 32 bits
    expect_equal_uint(bi_reverse(0u, 32), bi_reverse_ref(0u, 32),
                      "bi_reverse(0,32) should be 0");
    // Reversing 32 bits of 0xF0F0F0F0 -> 0x0F0F0F0F
    expect_equal_uint(bi_reverse(0xF0F0F0F0u, 32), bi_reverse_ref(0xF0F0F0F0u, 32),
                      "bi_reverse(0xF0F0F0F0,32) should be 0x0F0F0F0F");
    // Lower 8 bits of 0x12345678 reversed -> 0x1E
    expect_equal_uint(bi_reverse(0x12345678u, 8), bi_reverse_ref(0x12345678u, 8),
                      "bi_reverse(0x12345678,8) should reverse lower 8 bits (expected 0x1E)");
    // Upper bound on 16-bit reversal: 0xFFFFFFFF reversed for 16 bits -> 0xFFFF
    expect_equal_uint(bi_reverse(0xFFFFFFFFu, 16), bi_reverse_ref(0xFFFFFFFFu, 16),
                      "bi_reverse(0xFFFFFFFF,16) should be 0xFFFF");
    // Pattern test on 4 bits
    expect_equal_uint(bi_reverse(0xAAAAAAAAu, 4), bi_reverse_ref(0xAAAAAAAAu, 4),
                      "bi_reverse(0xAAAAAAAA,4) should reverse lower 4 bits");
}

// Test 3: Property-based test to verify the involutive property of bit reversal.
// For multiple codes and lengths, reversing twice should yield the original lower 'len' bits.
static void test_bi_reverse_property()
{
    unsigned codes[] = {0x0u, 0x1u, 0x3u, 0x5u, 0xA5A5A5A5u, 0xFFFFFFFFu, 0x1234ABCDu};
    int lenses[] = {1, 2, 3, 4, 8, 12, 16, 20, 24, 32};

    for (unsigned c : codes) {
        for (int len : lenses) {
            if (len <= 0) continue;
            unsigned first = bi_reverse(c, len);
            unsigned second = bi_reverse(first, len);
            unsigned mask = (len >= 32) ? 0xFFFFFFFFu : ((1u << len) - 1);
            // The double reversal should equal the lower 'len' bits of the original code
            expect_equal_uint(second & mask, c & mask,
                              "bi_reverse twice should yield original lower len bits");
        }
    }
}

// Entry point: runs all tests and reports summary.
// Uses non-terminating assertions: tests continue after a failure to maximize coverage.
int main()
{
    std::cout << "Starting bi_reverse test suite (C linkage) ..." << std::endl;

    test_bi_reverse_basic_cases();
    test_bi_reverse_more_cases();
    test_bi_reverse_property();

    if (g_failed > 0) {
        std::cerr << "bi_reverse tests completed with failures: "
                  << g_failed << " / " << g_total << std::endl;
        return 1;
    } else {
        std::cout << "bi_reverse tests passed: " << g_total << " tests." << std::endl;
        return 0;
    }
}