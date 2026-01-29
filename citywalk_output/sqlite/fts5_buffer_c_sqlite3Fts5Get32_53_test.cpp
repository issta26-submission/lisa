/*
  Unit tests for sqlite3Fts5Get32(const u8 *aBuf)

  Approach:
  - Provide a lightweight C++11 test harness (no GTest) that directly links to the C function
    sqlite3Fts5Get32 (extern "C" to prevent name mangling).
  - Cover multiple input scenarios to verify correct 32-bit reconstruction and signed casting:
      - Normal big-endian-like construction (0x12 0x34 0x56 0x78 -> 0x12345678)
      - All zeros
      - All ones (0xFFFFFFFF -> -1 as int)
      - High-bit set scenario to ensure correct negative result when cast to int
      - Arbitrary values to ensure general correctness
  - Use a simple non-terminating assertion helper that records failures and continues
    execution to maximize coverage.

  Notes:
  - This test relies on the function being available in the linked project (as defined in
    <FOCAL_CLASS_DEP>), with the appropriate C linkage. We declare it here with extern "C".
  - The tests are designed to be portable across platforms due to explicit use of uint8_t
    and fixed-width literals.
*/

#include <cstdio>
#include <cstdint>
#include <fts5Int.h>


// Import the focal function under test with C linkage
extern "C" int sqlite3Fts5Get32(const uint8_t *aBuf);

static int g_failures = 0;

// Lightweight assertion helper: non-terminating, reports failures but continues.
static void expect_eq_int(const char* desc, int got, int expected) {
    if (got != expected) {
        printf("FAIL: %s - expected %d, got %d\n", desc, expected, got);
        ++g_failures;
    } else {
        printf("PASS: %s\n", desc);
    }
}

// Test 1: Basic case with a typical sequence, verifying correct assembly of bytes into
// a 32-bit value as per the function's logic.
static void test_get32_basic() {
    // Candidate Keywords: aBuf, [0], [1], [2], [3], endianness, 0x12 0x34 0x56 0x78
    uint8_t aBuf[4] = {0x12, 0x34, 0x56, 0x78};
    int val = sqlite3Fts5Get32(aBuf);
    expect_eq_int("basic_big_endian_construction (0x12 0x34 0x56 0x78)", val, 0x12345678);
}

// Test 2: All zeros should yield zero.
static void test_get32_all_zeros() {
    uint8_t aBuf[4] = {0x00, 0x00, 0x00, 0x00};
    int val = sqlite3Fts5Get32(aBuf);
    expect_eq_int("all_zeros", val, 0);
}

// Test 3: All bytes 0xFF should yield -1 when interpreted as signed int after casting.
static void test_get32_all_ones() {
    uint8_t aBuf[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    int val = sqlite3Fts5Get32(aBuf);
    expect_eq_int("all_ones", val, -1); // 0xFFFFFFFF interpreted as signed int
}

// Test 4: High bit set in the most significant byte should produce a negative int.
// Verifies correct casting behavior of the final value.
static void test_get32_mixed_sign() {
    // 0x80000001 -> negative when cast to signed int
    uint8_t aBuf[4] = {0x80, 0x00, 0x00, 0x01};
    int val = sqlite3Fts5Get32(aBuf);
    expect_eq_int("mixed_sign_high_bit_set", val, (int)0x80000001);
}

// Test 5: Arbitrary values to ensure general correctness beyond basic or edge cases.
static void test_get32_arbitrary2() {
    uint8_t aBuf[4] = {0x7F, 0xAA, 0x55, 0x01};
    int val = sqlite3Fts5Get32(aBuf);
    expect_eq_int("arbitrary2", val, (int)0x7FAA5501);
}

// Entry point: run all tests and report total failures.
int main() {
    // Run test cases
    test_get32_basic();
    test_get32_all_zeros();
    test_get32_all_ones();
    test_get32_mixed_sign();
    test_get32_arbitrary2();

    printf("Total failures: %d\n", g_failures);
    // Return non-zero if any test failed
    return g_failures;
}