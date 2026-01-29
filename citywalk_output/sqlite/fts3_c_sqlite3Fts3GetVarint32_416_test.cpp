/*
  Lightweight C++11 unit test suite for sqlite3Fts3GetVarint32 (fts3.c)
  - No external test framework (no GoogleTest/GTest). 
  - Uses a small, non-terminating assertion style suitable for continuous execution.
  - Focuses on key input encoding/decoding behavior of sqlite3Fts3GetVarint32.
  - Test vectors are crafted to cover edge cases and typical values within 32-bit range.
  - The test harness is self-contained and relies on the function being linked from fts3.c.
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal function (C linkage)
extern "C" int sqlite3Fts3GetVarint32(const char *p, int *pi);

/*
  Global test failure counter.
  Non-terminating assertions: test execution continues after a failure.
*/
static int g_failures = 0;

/*
  Helper: perform a single decode test for a given value.
  Encoding scheme (as used by sqlite3Fts3GetVarint32 in 5-byte form):
    - First 4 bytes carry the low 28 bits (7 bits per byte, MSB as continuation flag).
    - 5th byte carries the top 3 bits of the 31-bit result in its lower 3 bits.
    - The 5-byte sequence ends with the 5th byte having its continuation bit not set.
  We construct a buffer accordingly and verify that:
    ret == 5 and decoded value equals the original input.
*/
static void test_varint_decode(uint32_t value) {
    // Prepare 5-byte buffer encoding 'value' (lower 28 bits in first 4 bytes, top 3 bits in last byte)
    unsigned char buf[5];
    uint32_t low28 = value & 0x0FFFFFFF;
    buf[0] = static_cast<unsigned char>(low28 & 0x7F) | 0x80;
    buf[1] = static_cast<unsigned char>((low28 >> 7) & 0x7F) | 0x80;
    buf[2] = static_cast<unsigned char>((low28 >> 14) & 0x7F) | 0x80;
    buf[3] = static_cast<unsigned char>((low28 >> 21) & 0x7F) | 0x80;
    buf[4] = static_cast<unsigned char>((value >> 28) & 0x07); // top 3 bits

    int out = -1;
    int ret = sqlite3Fts3GetVarint32(reinterpret_cast<const char*>(buf), &out);

    if (ret == 5 && out == static_cast<int>(value)) {
        std::cout << "PASS: sqlite3Fts3GetVarint32 decoded value " << value << " correctly (ret=" << ret << ").\n";
    } else {
        ++g_failures;
        std::cerr << "FAIL: sqlite3Fts3GetVarint32 decode of value " << value
                  << " -> ret=" << ret << ", out=" << out << "\n";
    }
}

/*
  Test 1: Decode zero
  - Expect ret == 5
  - Expect decoded value == 0
*/
static void test_varint_zero() {
    test_varint_decode(0);
}

/*
  Test 2: Decode one
  - Ensure simple non-zero with low bits is handled.
*/
static void test_varint_one() {
    test_varint_decode(1);
}

/*
  Test 3: Decode a value with top 3 bits non-zero (mid-range)
  - Checks top bits encoding path (bits 28-30).
*/
static void test_varint_mid_topbits() {
    test_varint_decode(0x10000000); // binary: top 3 bits = 001
}

/*
  Test 4: Decode maximum 31-bit value
  - 0x7FFFFFFF should map to 28 low bits all 1s and top 3 bits = 0x07
*/
static void test_varint_max() {
    test_varint_decode(0x7FFFFFFF);
}

/*
  Test 5: Decode a random non-trivial value
  - Validates general correctness for a non-boundary example.
*/
static void test_varint_random() {
    test_varint_decode(0x01234567);
}

/*
  Runner: execute all tests and report final status.
  - Returns 0 on success (no failures), non-zero if any test failed.
*/
static void run_all_tests() {
    std::cout << "Starting sqlite3Fts3GetVarint32 tests...\n";

    test_varint_zero();
    test_varint_one();
    test_varint_mid_topbits();
    test_varint_max();
    test_varint_random();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
    }
}

int main() {
    run_all_tests();
    return g_failures;
}