// pufftest.cpp
// Unit test suite for the focal method: bythirds(size_t size)
// This test suite is written for C++11, without GTest, using a lightweight
// in-source test harness with non-terminating assertions to maximize coverage.

#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <puff.h>
#include <stdlib.h>


// Replicated focal method under test (mirrors the logic in the provided snippet).
static size_t bythirds(size_t size)
{
    int n;
    size_t m;
    m = size;
    for (n = 0; m; n++)
        m >>= 1;
    if (n < 3)
        return size + 1;
    n -= 3;
    m = size >> n;
    m += m == 6 ? 2 : 1;
    m <<= n;
    return m > size ? m : (size_t)(-1);
}

// Simple test harness (non-terminating assertions): keeps running even on failures.
static int g_failures = 0;

#define EXPECT_EQ(expected, actual) do { \
    if ((size_t)(actual) != (size_t)(expected)) { \
        std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << "  expected=" << (expected) \
                  << ", actual=" << (actual) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << "  condition failed: " #cond << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: True branch of the first predicate (n < 3).
// This exercises inputs where the highest set bit is in the lowest two bits.
static void test_bythirds_small_boundaries()
{
    // For sizes 0 to 3, the function should return size + 1.
    // This confirms the early-return path (n < 3) is covered.
    for (size_t s = 0; s <= 3; ++s) {
        size_t expected = s + 1;
        size_t actual = bythirds(s);
        EXPECT_EQ(expected, actual);
    }
}

// Test 2: False branch of the first predicate (n >= 3) and
// verification of both internal branches within that path.
// Also ensures the m == 6 path is exercised for at least one input.
static void test_bythirds_known_values()
{
    // Known values derived from the focal implementation logic:
    // s: expected bythirds(s)
    // 4 -> 5, 5 -> 6, 6 -> 8, 7 -> 8, 8 -> 10, 9 -> 10,
    // 10 -> 12, 11 -> 12, 12 -> 16, 13 -> 16, 14 -> 16, 15 -> 16, 16 -> 20
    struct Pair { size_t input; size_t expected; };
    const Pair tests[] = {
        {4, 5},
        {5, 6},
        {6, 8},
        {7, 8},
        {8, 10},
        {9, 10},
        {10, 12},
        {11, 12},
        {12, 16}, // m == 6 path (size >> n' == 6) occurs for s=12
        {13, 16},
        {14, 16},
        {15, 16},
        {16, 20}
    };

    for (const auto &t : tests) {
        size_t actual = bythirds(t.input);
        EXPECT_EQ(t.expected, actual);
    }

    // Additional explicit checks to document specific branches:
    // Ensure path where m != 6 (e.g., s = 10) yields correct result 12.
    {
        size_t s = 10;
        size_t expected = 12; // computed in Step-Understanding analysis
        size_t actual = bythirds(s);
        EXPECT_EQ(expected, actual);
    }
    // Ensure path where m == 6 could be encountered in inputs like s = 12
    // and still produces a valid output > size.
    {
        size_t s = 12;
        size_t expected = 16;
        size_t actual = bythirds(s);
        EXPECT_EQ(expected, actual);
    }
}

// Test 3: Coverage of a few larger representative inputs to ensure
// stability across several bit-length ranges.
static void test_bythirds_large_inputs()
{
    // Quick sanity checks on larger powers of two and near-powers:
    // 32 -> 40, 64 -> 128? (computed below via the implemented logic)
    // The exact values are derived from the algorithm; here we verify
    // that results are strictly greater than the input for these cases.
    size_t inputs[] = {32, 64, 128, 256};
    for (size_t s : inputs) {
        size_t res = bythirds(s);
        EXPECT_TRUE(res > s);
    }
}

// Entry point for running all tests.
int main()
{
    // Run tests in a logical order to ensure coverage of all branches.
    test_bythirds_small_boundaries();   // True branch of first if
    test_bythirds_known_values();       // False branch of first if, includes m==6 path
    test_bythirds_large_inputs();       // Large inputs exercising larger n values

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
    }
    return g_failures;
}