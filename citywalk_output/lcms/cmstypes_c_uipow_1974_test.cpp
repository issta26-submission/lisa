// Test suite for cmsUInt32Number uipow(n, a, b) without using Google Test.
// This test harness is designed to be compiled together with cmstypes.c
// and will run a set of unit tests covering true/false branches and edge cases.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <limits.h>


// Candidate type alias to match likely cmsUInt32Number definition.
// It is very commonly defined as unsigned int in the CMS/LittleCMS codebase.
typedef unsigned int cmsUInt32Number;

// Declaration of the focal function under test.
// We assume it uses the same signature as in cmstypes.c.
extern "C" cmsUInt32Number uipow(cmsUInt32Number n, cmsUInt32Number a, cmsUInt32Number b);

// Simple, non-terminating assertion helpers.
static int g_failures = 0;
#define EXPECT_EQ(x, y) do { \
    cmsUInt32Number _x = (cmsUInt32Number)(x); \
    cmsUInt32Number _y = (cmsUInt32Number)(y); \
    if (_x != _y) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " #x " (" << _x << ") != " #y " (" << _y << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(x) do { \
    bool _v = (bool)(x); \
    if (!_v) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": Expected true but got false" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FALSE(x) do { \
    bool _v = (bool)(x); \
    if (_v) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": Expected false but got true" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// Unit tests

// 1) a == 0 should cause immediate return 0 (regardless of n, b)
void test_case_a_zero_returns_zero() {
    // a == 0
    cmsUInt32Number res = uipow(5, 0, 10);
    EXPECT_EQ(res, 0u);
}

// 2) n == 0 should cause immediate return 0 (regardless of a, b)
void test_case_n_zero_returns_zero() {
    cmsUInt32Number res = uipow(0, 5, 10);
    EXPECT_EQ(res, 0u);
}

// 3) Basic non-overflow computation: n * (a^b) with small numbers
void test_case_basic_non_overflow() {
    // n=2, a=3, b=2 => a^b = 9; rc = 9*2 = 18
    cmsUInt32Number res = uipow(2, 3, 2);
    EXPECT_EQ(res, 18u);
}

// 4) Overflow detected inside the loop when computing a^b
// For example: n=1, a=2, b=32 should trigger overflow in the loop.
// The function returns (cmsUInt32Number) -1 which equals UINT_MAX.
void test_case_overflow_in_loop() {
    cmsUInt32Number res = uipow(1, 2, 32);
    EXPECT_EQ(res, UINT_MAX);
}

// 5) Overflow detected after the loop when computing rc = rv * n
// Choose values where a^b fits in the loop but rv*n overflows.
// Example: n=4, a=2, b=30 -> rv = 2^30; rc = rv*n overflows to 0 in 32-bit,
// thus rv != rc / n -> returns -1 (UINT_MAX)
void test_case_overflow_on_rc() {
    cmsUInt32Number res = uipow(4, 2, 30);
    EXPECT_EQ(res, UINT_MAX);
}

// 6) b == 0 should return n (provided n != 0 and a != 0)
void test_case_b_zero_returns_n() {
    cmsUInt32Number res = uipow(5, 3, 0);
    EXPECT_EQ(res, 5u);
}

// 7) a == 1 should return n (since a^b == 1)
void test_case_a_equals_one() {
    cmsUInt32Number res = uipow(7, 1, 5);
    EXPECT_EQ(res, 7u);
}

// 8) Larger non-overflow scenario with moderate numbers
// n=3, a=3, b=5 => a^b = 243; rc = 243*3 = 729
void test_case_larger_non_overflow() {
    cmsUInt32Number res = uipow(3, 3, 5);
    EXPECT_EQ(res, 729u);
}

// 9) Near-boundary multiplication without overflow
// n=65535, a=4, b=4 -> a^b = 256; rc = 256 * 65535 = 16776960
void test_case_near_boundary_no_overflow() {
    cmsUInt32Number res = uipow(65535u, 4u, 4u);
    // Note: a^b = 4^4 = 256; rc = 256 * 65535 = 16776960
    EXPECT_EQ(res, 16776960u);
}

// 10) Edge case with maximum a to force early overflow in the loop
// a = UINT_MAX, b = 1, n = 1 -> first multiplication overflows, return UINT_MAX
void test_case_max_a_overflow_loop() {
    cmsUInt32Number res = uipow(1, UINT_MAX, 1);
    EXPECT_EQ(res, UINT_MAX);
}

// Runner
void run_all_tests() {
    test_case_a_zero_returns_zero();
    test_case_n_zero_returns_zero();
    test_case_basic_non_overflow();
    test_case_overflow_in_loop();
    test_case_overflow_on_rc();
    test_case_b_zero_returns_n();
    test_case_a_equals_one();
    test_case_larger_non_overflow();
    test_case_near_boundary_no_overflow();
    test_case_max_a_overflow_loop();
}

int main() {
    run_all_tests();
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " test" << (g_failures > 1 ? "s" : "") << " failed.\n";
        return 1;
    }
}