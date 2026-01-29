// C++11 unit tests for the focal method 'construct' inside blast.c
// Approach: include blast.c in a controlled way so that 'construct' becomes
// a publicly linkable symbol within this translation unit (by redefining 'local'
// to nothing before inclusion). This allows direct testing from C++.
//
// Rationale comments are provided for clarity. Non-terminating assertions are used
// (they only report failures but do not abort the test run).

#include <cstring>
#include <stddef.h>
#include <blast.h>
#include <setjmp.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <blast.c>
#include <stdlib.h>


// Expose the focal C function by compiling blast.c in this translation unit.
// We redefine the macro 'local' (used inside blast.c) to nothing so that the
// functions/classes that would otherwise be static become globally accessible.
#define local
#undef local

// Simple non-terminating test harness (no GTest/GMock dependencies)
static int total_tests = 0;
static int failed_tests = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        ++total_tests; \
        if (!(cond)) { \
            ++failed_tests; \
            std::cerr << "TEST FAILURE: " << (msg) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        } \
    } while (0)

// Helper to set up a huffman instance backed by user-provided arrays
struct HuffmanWrapper {
    short *count;
    short *symbol;
};

// Test 1: Complete zero-length set should return 0 and reflect count[0] == n
// rep encodes: len = 0, left = 1, so a single symbol with length 0
void test_construct_complete_zero_set() {
    // Prepare storage for huffman fields
    short count_storage[256];
    short symbol_storage[256];
    std::memset(count_storage, 0, sizeof(count_storage));
    std::memset(symbol_storage, 0, sizeof(symbol_storage));

    struct huffman h;
    h.count = count_storage;
    h.symbol = symbol_storage;

    // rep: [0x00] -> one symbol with length 0
    unsigned char rep[1] = { 0x00 };
    int n = 1;

    int ret = construct(&h, rep, n);

    TEST_ASSERT(ret == 0, "Construct should return 0 for complete set (single length 0)");
    TEST_ASSERT(count_storage[0] == 1, "count[0] should be equal to n after processing (complete zero-length set)");
    // No need to check symbol array for this specific case
}

// Test 2: Over-subscribed set (negative return expected)
// rep: three symbols with length 1 (each encoded as 0x01, left=1)
void test_construct_over_subscribed() {
    short count_storage[256];
    short symbol_storage[256];
    std::memset(count_storage, 0, sizeof(count_storage));
    std::memset(symbol_storage, 0, sizeof(symbol_storage));

    struct huffman h;
    h.count = count_storage;
    h.symbol = symbol_storage;

    // rep: [0x01, 0x01, 0x01] -> n=3, all length 1
    unsigned char rep[3] = { 0x01, 0x01, 0x01 };
    int n = 3;

    int ret = construct(&h, rep, n);

    TEST_ASSERT(ret < 0, "Over-subscribed lengths should yield a negative return value");
}

// Test 3: Normal case with multiple lengths; expect 0 (complete set) and correct counts/symbols
void test_construct_normal_complete() {
    short count_storage[256];
    short symbol_storage[256];
    std::memset(count_storage, 0, sizeof(count_storage));
    std::memset(symbol_storage, 0, sizeof(symbol_storage));

    struct huffman h;
    h.count = count_storage;
    h.symbol = symbol_storage;

    // rep: [0x01, 0x03] => n=2, lengths 1 and 3
    unsigned char rep[2] = { 0x01, 0x03 };
    int n = 2;

    int ret = construct(&h, rep, n);

    TEST_ASSERT(ret == 0, "Normal complete set should return 0");
    // Expect one code of length 1 and one of length 3
    TEST_ASSERT(count_storage[1] == 1, "count[1] should be 1");
    TEST_ASSERT(count_storage[3] == 1, "count[3] should be 1");
    // Symbol table should reflect ordering by length: first symbol with len 1, then len 3
    // We deduced that symbol[0] should be 0 (the first symbol with length 1) and symbol[1] should be 1
    TEST_ASSERT(symbol_storage[0] == 0, "First symbol should be 0 (length 1)");
    TEST_ASSERT(symbol_storage[1] == 1, "Second symbol should be 1 (length 3)");
}

// Test 4: Incomplete set scenario (non-zero return)
void test_construct_incomplete_set() {
    short count_storage[256];
    short symbol_storage[256];
    std::memset(count_storage, 0, sizeof(count_storage));
    std::memset(symbol_storage, 0, sizeof(symbol_storage));

    struct huffman h;
    h.count = count_storage;
    h.symbol = symbol_storage;

    // rep: [0x01] -> single symbol with length 1
    unsigned char rep[1] = { 0x01 };
    int n = 1;

    int ret = construct(&h, rep, n);

    TEST_ASSERT(ret > 0, "Incomplete set should return a positive value indicating leftover codes (non-zero)");
}

int main() {
    // Run tests
    test_construct_complete_zero_set();
    test_construct_over_subscribed();
    test_construct_normal_complete();
    test_construct_incomplete_set();

    // Summary
    std::cout << "Total tests run: " << total_tests << std::endl;
    if (failed_tests > 0) {
        std::cout << "Tests failed: " << failed_tests << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }

    return failed_tests;
}