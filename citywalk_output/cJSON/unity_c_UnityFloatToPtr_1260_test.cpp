// This test suite targets the UnityFloatToPtr function located in unity.c.
// It uses a small, self-contained C++11 test runner (no GTest) with
// minimal non-terminating assertions to maximize coverage.
// The tests rely on the C linkage of UnityFloatToPtr and Unity's typedef
// UNITY_INTERNAL_PTR defined in unity.h.

#include <stddef.h>
#include <cstring>
#include <unity.h>
#include <iostream>
#include <cstdint>


extern "C" {
// Prototype of the focal function under test (C linkage)
UNITY_INTERNAL_PTR UnityFloatToPtr(const float num);
}

// Simple non-terminating test harness
static int g_failures = 0;
#define EXPECT_FLOAT_EQ(actual, expected, msg) do { \
    if (!((actual) == (expected))) { \
        std::cerr << "FAIL: " << (msg) << " | expected: " << (expected) \
                  << ", got: " << (actual) << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if ((void*)(a) != (void*)(b)) { \
        std::cerr << "FAIL: " << (msg) << " | pointers differ: " \
                  << static_cast<void*>(a) << " != " << static_cast<void*>(b) \
                  << std::endl; \
        ++g_failures; \
    } \
} while (0)

static void test_UnityFloatToPtr_basic_usage() {
    // Test 1: Basic value round-trip for 1.0f
    // Purpose: Ensure UnityFloatToPtr writes 1.0f to the internal static memory
    // and returns a pointer to that memory which can be dereferenced to get 1.0f.
    UNITY_INTERNAL_PTR p1 = UnityFloatToPtr(1.0f);
    float val1 = *static_cast<float*>(p1);
    EXPECT_FLOAT_EQ(val1, 1.0f, "UnityFloatToPtr should store and return 1.0f");

    // Test 2: Round-trip for a negative value
    UNITY_INTERNAL_PTR p2 = UnityFloatToPtr(-2.5f);
    float val2 = *static_cast<float*>(p2);
    EXPECT_FLOAT_EQ(val2, -2.5f, "UnityFloatToPtr should store and return -2.5f");

    // Test 3: Pointer identity should remain stable across calls
    // The implementation uses a static UnityQuickCompare.f, so both calls
    // should return the same underlying memory address.
    EXPECT_PTR_EQ(p1, p2, "UnityFloatToPtr should return the same static memory address across calls");

    // Test 4: Additional value to broaden coverage
    UNITY_INTERNAL_PTR p3 = UnityFloatToPtr(0.75f);
    float val3 = *static_cast<float*>(p3);
    EXPECT_FLOAT_EQ(val3, 0.75f, "UnityFloatToPtr should store and return 0.75f");
    EXPECT_PTR_EQ(p1, p3, "Pointer identity should be stable for all invocations");
}

static void test_UnityFloatToPtr_negative_zero_bits_and_value() {
    // Test 5: Negative zero should preserve sign bit in memory,
    // while the numeric value when read may compare equal to +0.0f.
    UNITY_INTERNAL_PTR p = UnityFloatToPtr(-0.0f);
    float v = *static_cast<float*>(p);
    EXPECT_FLOAT_EQ(v, 0.0f, "Numeric value of -0.0f should be 0.0f when read back");

    // Check the exact bit pattern to verify the sign bit is set for -0.0f
    int32_t bits;
    std::memcpy(&bits, p, sizeof(bits));
    const int32_t NEG_ZERO_BITS = 0x80000000;
    if (bits != NEG_ZERO_BITS) {
        std::cerr << "FAIL: Sign bit check for -0.0f failed. Expected 0x"
                  << std::hex << NEG_ZERO_BITS << ", got 0x" << bits << std::dec << std::endl;
        ++g_failures;
    }
}

// Entry point: run all tests and report results.
// Tests are invoked directly from main (no external test runner).
int main() {
    test_UnityFloatToPtr_basic_usage();
    test_UnityFloatToPtr_negative_zero_bits_and_value();

    if (g_failures == 0) {
        std::cout << "All UnityFloatToPtr tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " UnityFloatToPtr test(s) failed." << std::endl;
        return g_failures;
    }
}