// tificc_test.cpp
// A C++11 unit test harness for the focal function FromLabV2ToLabV4
// This test suite does not rely on Google Test and uses a lightweight
// test framework with non-terminating assertions to maximize coverage.

#include <lcms2_plugin.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <utils.h>
#include <tiffio.h>


// Focal function is defined in C (tificc.c). Use C linkage in C++ test.
extern "C" int FromLabV2ToLabV4(int x);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

static void log_pass(const std::string& msg) {
    std::cout << "TEST PASSED: " << msg << std::endl;
}

static void log_fail(const std::string& msg) {
    std::cerr << "TEST FAILED: " << msg << std::endl;
    ++g_failed;
}

static void EXPECT_EQ(int expected, int actual, const std::string& msg) {
    ++g_total;
    if (expected != actual) {
        log_fail(msg + " | expected=" + std::to_string(expected) + " actual=" + std::to_string(actual));
    } else {
        log_pass(msg);
    }
}

// Compute the expected result using the same mathematically derived formula
// for non-negative inputs while respecting the original clamp to 0xFFFF.
// Note: For non-negative x, FromLabV2ToLabV4 effectively computes
// a = floor(x * 257 / 256) and then clamps to 0xFFFF if a > 0xFFFF.
static int compute_expected(int x) {
    long long mult = static_cast<long long>(x) * 257LL;
    long long a = mult / 256; // equivalent to (x*257) >> 8 for non-negative x
    if (a > 0xFFFF) a = 0xFFFF;
    return static_cast<int>(a);
}

// Test 1: Basic range test for 0 <= x <= 255
// Rationale: For x in [0, 255], the computation should yield a == x
// and the function should return x without any clamping.
static void test_basic_range_0_to_255() {
    for (int x = 0; x <= 255; ++x) {
        int expected = compute_expected(x);
        int actual = FromLabV2ToLabV4(x);
        EXPECT_EQ(expected, actual, "FromLabV2ToLabV4(x) correctness for x=" + std::to_string(x));
        // Additional consistency check: for x in [0,255], the result should equal x
        EXPECT_EQ(x, actual, "FromLabV2ToLabV4(x) consistency check for x=" + std::to_string(x));
    }
}

// Test 2: Mixed values across ranges to exercise typical behavior
// Rationale: Verify a variety of inputs to exercise non-edge branches
static void test_mixed_values() {
    std::vector<int> inputs = {1, 256, 257, 258, 500, 999, 4096, 12345, 32767, 65533};
    for (int x : inputs) {
        int expected = compute_expected(x);
        int actual = FromLabV2ToLabV4(x);
        EXPECT_EQ(expected, actual, "FromLabV2ToLabV4(x) mixed test for x=" + std::to_string(x));
    }
}

// Test 3: Clamp path where a > 0xFFFF to ensure proper clamping to 0xFFFF
// Rationale: Inputs large enough to trigger the clamp behavior.
static void test_clamp_behavior_large_inputs() {
    std::vector<int> inputs = {65534, 65535, 70000, 100000, 200000};
    for (int x : inputs) {
        int expected = compute_expected(x);
        int actual = FromLabV2ToLabV4(x);
        EXPECT_EQ(expected, actual, "FromLabV2ToLabV4(x) clamp test for x=" + std::to_string(x));
    }
}

int main() {
    // Run all test groups
    test_basic_range_0_to_255();
    test_mixed_values();
    test_clamp_behavior_large_inputs();

    // Summary
    std::cout << "Total tests: " << g_total
              << ", Passed: " << (g_total - g_failed)
              << ", Failed: " << g_failed << std::endl;

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}