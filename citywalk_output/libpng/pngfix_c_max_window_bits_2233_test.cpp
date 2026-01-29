// Test suite for max_window_bits in pngfix.c
// Targeted for C++11, non-GTest, self-contained assertions with non-terminating checks.

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <iomanip>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


// We assume the implementation in pngfix.c exposes the function with C linkage.
// The exact user-defined type 'uarbc' is not exposed here, so we declare a compatible
// prototype using a pointer to 16-bit unsigned integers. This relies on the fact that
// within max_window_bits the parameter is indexed as size[0], i.e., treated as an array/pointer.
extern "C" std::uint16_t max_window_bits(const std::uint16_t* size, int ndigits);

// Simple non-terminating test harness (no abort on failure)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            ++g_failures; \
            std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
        } \
    } while (0)

static void test_ndigits_greater_than_one_always_returns_15() {
    // ndigits > 1 short-circuits to 15 regardless of the data size.
    std::uint16_t dummy = 0;
    std::uint16_t res = max_window_bits(&dummy, 2);
    EXPECT_TRUE(res == 15, "ndigits > 1 should return 15");
    // Also test with another value to ensure it's not dependent on 'size'
    dummy = 12345;
    res = max_window_bits(&dummy, 10); // still > 1
    EXPECT_TRUE(res == 15, "ndigits > 1 (even with cb) should return 15");
}

static void test_cb_boundaries() {
    // We'll test representative boundary values according to the function's logic.
    // cb > 16384 -> 15
    // 16384 >= cb > 8192 -> 14
    // 8192 >= cb > 4096  -> 13
    // 4096 >= cb > 2048  -> 12
    // 2048 >= cb > 1024  -> 11
    // 1024 >= cb > 512    -> 10
    // 512 >= cb > 256     -> 9
    // 256 >= cb             -> 8
    std::uint16_t size = 16385;
    EXPECT_TRUE(max_window_bits(&size, 1) == 15, "cb=16385 should yield 15");

    size = 16384;
    EXPECT_TRUE(max_window_bits(&size, 1) == 14, "cb=16384 should yield 14");

    size = 8193;
    EXPECT_TRUE(max_window_bits(&size, 1) == 14, "cb=8193 should yield 14");

    size = 8192;
    EXPECT_TRUE(max_window_bits(&size, 1) == 13, "cb=8192 should yield 13");

    size = 4097;
    EXPECT_TRUE(max_window_bits(&size, 1) == 13, "cb=4097 should yield 13");

    size = 4096;
    EXPECT_TRUE(max_window_bits(&size, 1) == 12, "cb=4096 should yield 12");

    size = 2049;
    EXPECT_TRUE(max_window_bits(&size, 1) == 12, "cb=2049 should yield 12");

    size = 2048;
    EXPECT_TRUE(max_window_bits(&size, 1) == 11, "cb=2048 should yield 11");

    size = 1025;
    EXPECT_TRUE(max_window_bits(&size, 1) == 11, "cb=1025 should yield 11");

    size = 1024;
    EXPECT_TRUE(max_window_bits(&size, 1) == 10, "cb=1024 should yield 10");

    size = 513;
    EXPECT_TRUE(max_window_bits(&size, 1) == 10, "cb=513 should yield 10");

    size = 512;
    EXPECT_TRUE(max_window_bits(&size, 1) == 9, "cb=512 should yield 9");

    size = 257;
    EXPECT_TRUE(max_window_bits(&size, 1) == 9, "cb=257 should yield 9");

    size = 256;
    EXPECT_TRUE(max_window_bits(&size, 1) == 8, "cb=256 should yield 8");

    size = 0;
    EXPECT_TRUE(max_window_bits(&size, 1) == 8, "cb=0 should yield 8");
}

// Entry point for running tests
int main() {
    std::cout << "Running max_window_bits unit tests (non-GTest, C++11)\n";

    test_ndigits_greater_than_one_always_returns_15();
    test_cb_boundaries();

    if (g_failures == 0) {
        std::cout << "[PASS] All tests for max_window_bits passed.\n";
        return 0;
    } else {
        std::cerr << "[FAIL] " << g_failures << " test(s) failed for max_window_bits.\n";
        return 1;
    }
}