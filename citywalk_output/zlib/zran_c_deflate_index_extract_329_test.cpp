/*
 * Unit test suite for deflate_index_extract (zran.c) using a lightweight,
 * non-GTest C++11 harness. The tests focus on validating the early-return
 * predicates and boundary conditions to maximize coverage without requiring
 * actual zlib inflate execution.
 *
 * Tests implemented:
 * 1) NULL index returns Z_STREAM_ERROR
 * 2) index.have < 1 returns Z_STREAM_ERROR
 * 3) index.list[0].out != 0 returns Z_STREAM_ERROR
 * 4) len == 0 returns 0 (with a valid index)
 * 5) offset < 0 returns 0 (with a valid index)
 * 6) offset >= index.length returns 0 (with a valid index)
 *
 * Notes:
 * - We purposely avoid scenarios that require actual inflation (which would
 *   require valid compressed data and a functioning in stream).
 * - The test harness prints PASS/FAIL messages and does not terminate on failures.
 * - We rely on zlib.h and zran.h headers being available in the include path.
 * - This file is intended to be compiled together with zran.c (the focal method)
 *   in a C++11 capable project (without GTest).
 */

#include <cstdio>
#include <limits.h>
#include <string.h>
#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zran.h>


// Provide C linkage for the focal function under test.
extern "C" {
    ptrdiff_t deflate_index_extract(FILE *in, struct deflate_index *index,
                                    off_t offset, unsigned char *buf, size_t len);
}

// Lightweight test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion helper
#define TEST_PASS_MSG(msg) \
    do { std::cout << "[PASS] " msg << std::endl; } while (0)

#define TEST_FAIL_MSG(msg) \
    do { std::cerr << "[FAIL] " msg << std::endl; g_failed_tests++; } while (0)

// Helper macros for basic numeric assertions (prints helpful messages)
#define ASSERT_EQ(actual, expected, label)                               \
    do {                                                                  \
        ++g_total_tests;                                                  \
        if ((actual) != (expected)) {                                   \
            std::cerr << "[ASSERT_FAIL] " label                          \
                      << " (got " << static_cast<long long>(actual)      \
                      << ", expected " << static_cast<long long>(expected)\
                      << ")" << std::endl;                               \
            ++g_failed_tests;                                           \
        } else {                                                        \
            TEST_PASS_MSG(("[" label "] " #actual " == " #expected));    \
        }                                                               \
    } while (0)

#define ASSERT_TRUE(cond, label)                                          \
    do {                                                                  \
        ++g_total_tests;                                                  \
        if (!(cond)) {                                                  \
            std::cerr << "[ASSERT_FAIL] " label                          \
                      << " (condition is false)" << std::endl;          \
            ++g_failed_tests;                                           \
        } else {                                                        \
            TEST_PASS_MSG(("[" label "] condition is true"));            \
        }                                                               \
    } while (0)

// Test 1: NULL index should return Z_STREAM_ERROR
static void test_deflate_index_extract_null_index() {
    ++g_total_tests;
    ptrdiff_t ret = deflate_index_extract(nullptr, nullptr, 0, nullptr, 1);
    ASSERT_EQ(ret, Z_STREAM_ERROR, "deflate_index_extract(NULL, NULL, 0, NULL, 1) should be Z_STREAM_ERROR");
}

// Test 2: index.have < 1 should return Z_STREAM_ERROR
static void test_deflate_index_extract_have_lt_one() {
    ++g_total_tests;
    struct deflate_index idx;
    idx.have = 0;            // less than 1
    idx.length = 0;
    idx.list = NULL;
    idx.mode = 0;
    ptrdiff_t ret = deflate_index_extract(nullptr, &idx, 0, nullptr, 1);
    ASSERT_EQ(ret, Z_STREAM_ERROR, "deflate_index_extract(index.have<1) should be Z_STREAM_ERROR");
}

// Test 3: index.list[0].out != 0 should return Z_STREAM_ERROR
static void test_deflate_index_extract_out_not_zero() {
    ++g_total_tests;
    struct deflate_index idx;
    idx.have = 1;

    // Prepare a single point_t with non-zero 'out' value
    point_t pt;
    pt.in = 0;
    pt.out = 5;      // non-zero -> failure at initial predicate
    pt.bits = 0;
    pt.window = NULL;

    idx.list = &pt;
    idx.length = 100;
    idx.mode = 0;

    ptrdiff_t ret = deflate_index_extract(nullptr, &idx, 0, nullptr, 4);
    ASSERT_EQ(ret, Z_STREAM_ERROR, "deflate_index_extract(list[0].out != 0) should be Z_STREAM_ERROR");
}

// Test 4: len == 0 should return 0 (with a valid index)
static void test_deflate_index_extract_len_zero() {
    ++g_total_tests;
    struct deflate_index idx;
    idx.have = 1;

    point_t pt;
    pt.in = 0;
    pt.out = 0;      // valid (out == 0) for pre-check
    pt.bits = 0;
    pt.window = NULL;

    idx.list = &pt;
    idx.length = 100;
    idx.mode = 0;

    ptrdiff_t ret = deflate_index_extract(nullptr, &idx, 0, nullptr, 0);
    ASSERT_EQ(ret, 0, "deflate_index_extract(len==0) should return 0");
}

// Test 5: offset < 0 should return 0 (with a valid index)
static void test_deflate_index_extract_offset_negative() {
    ++g_total_tests;
    struct deflate_index idx;
    idx.have = 1;

    point_t pt;
    pt.in = 0;
    pt.out = 0;
    pt.bits = 0;
    pt.window = NULL;

    idx.list = &pt;
    idx.length = 100;
    idx.mode = 0;

    ptrdiff_t ret = deflate_index_extract(nullptr, &idx, -1, nullptr, 4);
    ASSERT_EQ(ret, 0, "deflate_index_extract(offset<0) should return 0");
}

// Test 6: offset >= index.length should return 0 (with a valid index)
static void test_deflate_index_extract_offset_out_of_bounds() {
    ++g_total_tests;
    struct deflate_index idx;
    idx.have = 1;

    point_t pt;
    pt.in = 0;
    pt.out = 0;
    pt.bits = 0;
    pt.window = NULL;

    idx.list = &pt;
    idx.length = 10;
    idx.mode = 0;

    ptrdiff_t ret = deflate_index_extract(nullptr, &idx, 10, nullptr, 4);
    ASSERT_EQ(ret, 0, "deflate_index_extract(offset==length) should return 0");
}

// Entry point to run tests
int main() {
    test_deflate_index_extract_null_index();
    test_deflate_index_extract_have_lt_one();
    test_deflate_index_extract_out_not_zero();
    test_deflate_index_extract_len_zero();
    test_deflate_index_extract_offset_negative();
    test_deflate_index_extract_offset_out_of_bounds();

    if (g_failed_tests == 0) {
        std::cout << "[SUMMARY] All tests passed. Total: " << g_total_tests << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] Tests failed: " << g_failed_tests
                  << " / " << g_total_tests << std::endl;
        return 1;
    }
}