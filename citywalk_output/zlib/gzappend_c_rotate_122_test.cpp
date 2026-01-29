// Test suite for the focal method: rotate (located in gzappend.c)
// This test suite is written for C++11, without Google Test, and uses a small
// custom assertion mechanism that does not terminate on first failure.
// The tests assume the rotate function is accessible with C linkage:
//
//   extern "C" void rotate(unsigned char *list, unsigned len, unsigned rot);
 
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <array>


// Declare the focal method with C linkage to ensure correct linking with gzappend.c
extern "C" void rotate(unsigned char *list, unsigned len, unsigned rot);

// Lightweight non-terminating assertion macro for unsigned comparisons
static int g_failures = 0;
#define ASSERT_EQ_U(a, b, msg) do { if (static_cast<unsigned>(a) != static_cast<unsigned>(b)) { \
    std::cerr << "ASSERT FAILED: " << (msg) << " | expected " << (unsigned)(b) << ", got " << (unsigned)(a) << "\n"; \
    ++g_failures; } } while(0)

// Helper: print array for debugging
static void print_array(const unsigned char* arr, size_t n) {
    std::cout << "[";
    for (size_t i = 0; i < n; ++i) {
        if (i) std::cout << " ";
        std::cout << static_cast<unsigned>(arr[i]);
    }
    std::cout << "]";
}

// Test 1: len = 1 (degenerate case) should be a no-op
// Rationale: rotate should return immediately when len < 2.
static void test_rotate_len1_noop() {
    // Prepare a single-element list
    unsigned char data[1] = { 7 };
    rotate(data, 1, 0); // rot value should be ignored
    ASSERT_EQ_U(data[0], 7, "len=1 should remain unchanged (rot=0)");
}

// Test 2: rot = 0 should be a no-op (even for len > 1)
static void test_rotate_rot_zero_noop() {
    unsigned char data[5] = { 1, 2, 3, 4, 5 };
    unsigned char expected[5] = { 1, 2, 3, 4, 5 };
    rotate(data, 5, 0);
    ASSERT_EQ_U(std::memcmp(data, expected, 5), 0, "rot=0 should produce no change for len=5");
}

// Test 3: rot = 1 (simple left shift by one via rotate-right logic)
static void test_rotate_rot_one() {
    unsigned char data[5] = { 1, 2, 3, 4, 5 };
    unsigned char expected[5] = { 2, 3, 4, 5, 1 }; // right rotate by 1
    rotate(data, 5, 1);
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ_U(data[i], expected[i], "rot=1: element at index " + std::to_string(i));
    }
}

// Test 4: rot = len - 1 (right-rotate by len-1 which is equivalent to left-rotate by 1)
static void test_rotate_rot_len_minus_one() {
    unsigned char data[5] = { 1, 2, 3, 4, 5 };
    unsigned char expected[5] = { 2, 3, 4, 5, 1 }; // same as rot=1 for len=5
    rotate(data, 5, 4);
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ_U(data[i], expected[i], "rot=len-1: index " + std::to_string(i));
    }
}

// Test 5: rot normalization (rot >= len)
static void test_rotate_rot_normalization() {
    unsigned char data[5] = { 1, 2, 3, 4, 5 };
    unsigned char expected[5] = { 4, 5, 1, 2, 3 }; // right rotate by 2 (7 % 5 == 2)
    rotate(data, 5, 7);
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ_U(data[i], expected[i], "rot normalization (rot=7, len=5)");
    }
}

// Test 6: general rotation with len=6, rot=3 (verify multiple cycles behavior)
static void test_rotate_general_cycles() {
    unsigned char data[6] = { 1, 2, 3, 4, 5, 6 };
    unsigned char expected[6] = { 4, 5, 6, 1, 2, 3 }; // right rotate by 3
    rotate(data, 6, 3);
    for (size_t i = 0; i < 6; ++i) {
        ASSERT_EQ_U(data[i], expected[i], "rot=3 on len=6");
    }
}

// Run all tests
static void run_all_tests() {
    test_rotate_len1_noop();
    test_rotate_rot_zero_noop();
    test_rotate_rot_one();
    test_rotate_rot_len_minus_one();
    test_rotate_rot_normalization();
    test_rotate_general_cycles();
}

// Main entry: run tests and report summary
int main() {
    std::cout << "Starting rotate() unit tests (C-style focal method)..." << std::endl;

    run_all_tests();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}