// C++11 test suite for the C function sqlite3TestHexToBin
// This test harness uses a lightweight, non-terminating assertion approach
// (no GTest). It relies only on the C++ standard library and the provided
// function under test.
//
// The tests cover:
// - Basic hex to binary conversion for two hex digits
// - Handling of the full byte (two hex digits) sequences
// - Odd number of hex digits (incomplete final nibble should yield 0 output length)
// - Ignoring non-hex digits (and behavior with mixed inputs)
// - Lowercase hex digits (expected to be ignored in this mapping)
// - Zero-length inputs
// - A longer, multi-byte conversion to ensure multiple bytes are produced

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Prototypes for the function under test.
// Ensure C linkage when compiling with a C++ compiler.
extern "C" int sqlite3TestHexToBin(const unsigned char *zIn, int N, unsigned char *aOut);

static int g_failures = 0;

// Simple non-terminating test assertion helpers
static inline void expect_true(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "TEST FAIL: " << msg << std::endl;
        ++g_failures;
    }
}

static inline void expect_eq_int(int a, int b, const char* msg)
{
    if (a != b) {
        std::cerr << "TEST FAIL: " << msg
                  << " (expected " << b << ", got " << a << ")" << std::endl;
        ++g_failures;
    }
}

static inline void expect_eq_bytes(const unsigned char* got, const unsigned char* expected, int n, const char* msg)
{
    if (std::memcmp(got, expected, n) != 0) {
        std::cerr << "TEST FAIL: " << msg << " - byte content mismatch." << std::endl;
        std::cerr << "  expected:";
        for (int i = 0; i < n; ++i) std::cerr << " " << std::hex << std::setw(2) << std::setfill('0') << int(expected[i]);
        std::cerr << std::dec << std::endl;
        std::cerr << "  got     :";
        for (int i = 0; i < n; ++i) std::cerr << " " << std::hex << std::setw(2) << std::setfill('0') << int(got[i]);
        std::cerr << std::dec << std::endl;
        ++g_failures;
    }
}

// Test 1: Basic two-hex-digit input "01" -> 0x01
static bool test_hex_to_bin_basic_two_digits()
{
    // Input: ASCII '0','1'
    const unsigned char in[] = { '0', '1' };
    const int N = 2;
    unsigned char out[4] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 1;
    const unsigned char expectedOut[] = { 0x01 };

    expect_eq_int(produced, expectedLen, "test_basic_two_digits: produced length should be 1");
    expect_eq_bytes(out, expectedOut, expectedLen, "test_basic_two_digits: output bytes should be 0x01");
    return (produced == expectedLen) && (produced == 1) && (std::memcmp(out, expectedOut, expectedLen) == 0);
}

// Test 2: Two hex digits forming 0xFF -> "FF"
static bool test_hex_to_bin_two_digits_ff()
{
    const unsigned char in[] = { 'F', 'F' };
    const int N = 2;
    unsigned char out[4] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 1;
    const unsigned char expectedOut[] = { 0xFF };

    expect_eq_int(produced, expectedLen, "test_ff_two_digits: produced length should be 1");
    expect_eq_bytes(out, expectedOut, expectedLen, "test_ff_two_digits: output bytes should be 0xFF");
    return (produced == expectedLen) && (produced == 1) && (std::memcmp(out, expectedOut, expectedLen) == 0);
}

// Test 3: Three pair hex digits "123456" -> 0x12 0x34 0x56
static bool test_hex_to_bin_multiple_bytes()
{
    const unsigned char in[] = { '1','2','3','4','5','6' };
    const int N = 6;
    unsigned char out[8] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 3;
    const unsigned char expectedOut[] = { 0x12, 0x34, 0x56 };

    expect_eq_int(produced, expectedLen, "test_multiple_bytes: produced length should be 3");
    expect_eq_bytes(out, expectedOut, expectedLen, "test_multiple_bytes: output bytes should be 0x12 0x34 0x56");
    return (produced == expectedLen) && (std::memcmp(out, expectedOut, expectedLen) == 0);
}

// Test 4: Lowercase hex digits are not mapped in the provided aMap; expect 0 bytes produced
static bool test_hex_to_bin_lowercase_ignored()
{
    const unsigned char in[] = { 'a', 'b', 'c' };
    const int N = 3;
    unsigned char out[4] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 0;

    expect_eq_int(produced, expectedLen, "test_lowercase_ignored: produced length should be 0 for lowercase hex");
    return (produced == 0);
}

// Test 5: Odd nibble sequence like "ABC" yields 0 output length because the final nibble is incomplete
static bool test_hex_to_bin_odd_nibbles()
{
    const unsigned char in[] = { 'A', 'B', 'C' };
    const int N = 3;
    unsigned char out[4] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 0;

    expect_eq_int(produced, expectedLen, "test_odd_nibbles: produced length should be 0 for odd nibble sequence");
    return (produced == 0);
}

// Test 6: Zero-length input returns 0
static bool test_hex_to_bin_zero_length()
{
    const unsigned char* in = nullptr;
    const int N = 0;
    unsigned char out[4] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 0;

    expect_eq_int(produced, expectedLen, "test_zero_length: produced length should be 0 for N=0");
    return (produced == 0);
}

// Test 7: Long input producing multiple bytes: "01020304" -> 0x01 0x02 0x03 0x04
static bool test_hex_to_bin_four_bytes_sequential()
{
    const unsigned char in[] = { '0','1','0','2','0','3','0','4' };
    const int N = 8;
    unsigned char out[8] = {0};
    int produced = sqlite3TestHexToBin(in, N, out);
    const int expectedLen = 4;
    const unsigned char expectedOut[] = { 0x01, 0x02, 0x03, 0x04 };

    expect_eq_int(produced, expectedLen, "test_four_bytes_sequential: produced length should be 4");
    expect_eq_bytes(out, expectedOut, expectedLen, "test_four_bytes_sequential: output should be 01 02 03 04");
    return (produced == expectedLen) && (std::memcmp(out, expectedOut, expectedLen) == 0);
}

int main()
{
    using std::cout;
    using std::endl;

    cout << "Starting sqlite3TestHexToBin unit tests (C++ harness)" << endl;

    bool all_passed = true;

    all_passed &= test_hex_to_bin_basic_two_digits();
    all_passed &= test_hex_to_bin_two_digits_ff();
    all_passed &= test_hex_to_bin_multiple_bytes();
    all_passed &= test_hex_to_bin_lowercase_ignored();
    all_passed &= test_hex_to_bin_odd_nibbles();
    all_passed &= test_hex_to_bin_zero_length();
    all_passed &= test_hex_to_bin_four_bytes_sequential();

    if (g_failures == 0) {
        cout << "All tests passed (" << (all_passed ? "OK" : "Some failures") << ")." << endl;
        return 0;
    } else {
        cout << g_failures << " test(s) failed." << endl;
        return 1;
    }
}