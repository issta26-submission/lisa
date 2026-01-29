// Test suite for adler32_combine_ (C implementation) using a lightweight C++11 harness.
// This file provides a set of non-terminating assertions to exercise the focal
// function and verify its behavior against a self-contained reference
// implementation of the Adler-32 combine logic.

// Note: The focal function is defined in adler32.c (extern "C" linkage assumed).
// We declare its prototype here to enable linkage from this C++ test harness.

#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>
#include <tuple>


// Forward declaration of the focal function under test.
// Adhere to C linkage to avoid name mangling when linking with C code.
extern "C" unsigned long adler32_combine_(unsigned long adler1, unsigned long adler2, long long len2);

// Lightweight non-terminating test assertion framework (no GTest).
static int g_tests_failed = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_EQ(actual, expected, msg) do { \
    if (!((actual) == (expected))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) \
                  << " | Expected: " << (expected) \
                  << "  Got: " << (actual) << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

using uLong = unsigned long;
using z_off64_t = long long;

// Reference implementation of the adler32_combine_ algorithm.
// This is a self-contained, portable C++ version used to verify the focal function.
// It mirrors the logic of the provided adler32_combine_ implementation, including
// the modulo reductions and final composition of sum1 and sum2.
static unsigned long adler32_combine_ref(unsigned long adler1, unsigned long adler2, long long len2) {
    const unsigned long BASE = 65521UL; // largest prime < 2^16
    if (len2 < 0)
        return 0xffffffffUL;

    // rem = len2 mod 63 (MOD63 macro in original code)
    unsigned long rem = static_cast<unsigned long>(len2 % 63);

    unsigned long sum1 = adler1 & 0xffff;
    unsigned long sum2 = rem * sum1;
    // MOD(sum2)
    sum2 %= BASE;

    sum1 += (adler2 & 0xffff) + BASE - 1;
    sum2 += ((adler1 >> 16) & 0xffff) + ((adler2 >> 16) & 0xffff) + BASE - rem;

    if (sum1 >= BASE) sum1 -= BASE;
    if (sum1 >= BASE) sum1 -= BASE;

    if (sum2 >= (BASE << 1)) sum2 -= (BASE << 1);
    if (sum2 >= BASE) sum2 -= BASE;

    // Return combined adler32: lower 16 bits = sum1, upper 16 bits = sum2
    return (sum1 & 0xffff) | (sum2 << 16);
}

// Test 1: Negative len2 should return 0xffffffff
// Verifies the invalid input path.
static void test_negative_len2() {
    // Various adler1/adler2 combos to ensure the path is consistently taken
    const std::vector<std::pair<unsigned long, unsigned long>> cases = {
        {1u, 2u},
        {0x12345678u, 0x9ABCDEF0u},
        {0xFFFFFFFFu, 0xFFFFFFFFu}
    };

    for (const auto &p : cases) {
        unsigned long a1 = p.first;
        unsigned long a2 = p.second;
        unsigned long expected = 0xffffffffUL;
        unsigned long actual = adler32_combine_(a1, a2, -1);
        EXPECT_EQ(actual, expected, "Negative len2 should yield 0xffffffff for a1=" +
                                    std::to_string(a1) + ", a2=" + std::to_string(a2));
    }
}

// Test 2: len2 = 0 should compute combined adler using only adler1 and adler2 with rem=0
static void test_len2_zero_matches_reference() {
    unsigned long a1 = 0x12345678u;
    unsigned long a2 = 0x9ABCDEF0u;
    long long len2 = 0;

    unsigned long expected = adler32_combine_ref(a1, a2, len2);
    unsigned long actual = adler32_combine_(a1, a2, len2);

    EXPECT_EQ(actual, expected, "len2=0 path should match reference computation");
}

// Test 3: len2 equals 63 (rem should be 0)
static void test_len63_rem_zero() {
    unsigned long a1 = 0x0FEDCBA9u;
    unsigned long a2 = 0x01234567u;
    long long len2 = 63; // rem = 0

    unsigned long expected = adler32_combine_ref(a1, a2, len2);
    unsigned long actual = adler32_combine_(a1, a2, len2);

    EXPECT_EQ(actual, expected, "len2=63 should use rem=0 path and match reference");
}

// Test 4: Large len2 to exercise multiple reductions
static void test_large_len2() {
    unsigned long a1 = 0x0A0B0C0Du;
    unsigned long a2 = 0x0E0F1011u;
    long long len2 = 123456789LL; // large positive length

    unsigned long expected = adler32_combine_ref(a1, a2, len2);
    unsigned long actual = adler32_combine_(a1, a2, len2);

    EXPECT_EQ(actual, expected, "Large len2 should match reference computation");
}

// Test 5: Extreme adler1/adler2 values to exercise bitwise handling
static void test_extreme_adler_values() {
    struct Case { unsigned long a1; unsigned long a2; long long len2; };
    const std::vector<Case> cases = {
        {0x00000000u, 0x00000000u, 0},
        {0xFFFFFFFFu, 0xFFFFFFFFu, 1},
        {0xFFFF0000u, 0x0000FFFFu, 1000},
        {0x80008000u, 0x7FFF7FFFu, 63},
    };

    for (const auto &c : cases) {
        unsigned long expected = adler32_combine_ref(c.a1, c.a2, c.len2);
        unsigned long actual = adler32_combine_(c.a1, c.a2, c.len2);
        EXPECT_EQ(actual, expected,
                  "Extreme adler values test with a1=" +
                  std::to_string(c.a1) + ", a2=" + std::to_string(c.a2) +
                  ", len2=" + std::to_string(c.len2));
    }
}

// Helper to run all tests and report summary
static void run_all_tests() {
    test_negative_len2();
    test_len2_zero_matches_reference();
    test_len63_rem_zero();
    test_large_len2();
    test_extreme_adler_values();

    if (g_tests_failed == 0) {
        std::cout << "All tests passed for adler32_combine_.\n";
    } else {
        std::cout << g_tests_failed << " test(s) FAILED for adler32_combine_.\n";
    }
}

int main() {
    // Namespace considerations: tests use standard C++11 and are self-contained.
    // They rely on the focal C function adler32_combine_ via C linkage.

    run_all_tests();

    // Return non-zero if any test failed to aid integration with build systems.
    return g_tests_failed ? 1 : 0;
}