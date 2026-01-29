// Unit test suite for sqlite3TestBinToHex (C function) using C++11.
// The tests are designed to be executable without a testing framework like GoogleTest.
// They exercise various inputs to validate correct hex encoding and proper buffer handling.
// The implementation of sqlite3TestBinToHex is provided here to enable self-contained testing.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Provide the focal function under test.
// The original function lives in test_hexio.c; we reproduce it here for a self-contained test.
extern "C" void sqlite3TestBinToHex(unsigned char *zBuf, int N){
  const unsigned char zHex[] = "0123456789ABCDEF";
  int i, j;
  unsigned char c;
  i = N*2;
  zBuf[i--] = 0;
  for(j=N-1; j>=0; j--){
    c = zBuf[j];
    zBuf[i--] = zHex[c&0xf];
    zBuf[i--] = zHex[c>>4];
  }
  assert( i==-1 );
}

// ---------------- Test Infrastructure ----------------

// Simple test harness using C++11 only (no gtest). Each test returns true on success.
static bool test_passed;
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to run and report a single test
#define RUN_TEST(TEST_FN) do {                                         \
    g_total_tests++;                                                     \
    bool ok = (TEST_FN)();                                                \
    if (ok) {                                                           \
        std::cout << "[PASS] " #TEST_FN "\n";                          \
    } else {                                                            \
        g_failed_tests++;                                             \
        std::cout << "[FAIL] " #TEST_FN "\n";                          \
    }                                                                   \
} while(0)

// ----------------- Individual Tests -----------------

// Test 1: N = 0 should produce just the terminator and no digits.
bool test_bin_to_hex_N0() {
    // Buffer size for N=0 is 1 (2*N + 1)
    std::vector<unsigned char> buf(1);
    // Initialize (not strictly necessary, but explicit)
    buf[0] = 0;
    sqlite3TestBinToHex(buf.data(), 0);
    // Expect buf[0] == 0 (terminator)
    if (buf[0] != 0) {
        std::cerr << "test_bin_to_hex_N0: expected terminator at buf[0], got " << int(buf[0]) << "\n";
        return false;
    }
    return true;
}

// Test 2: N = 1 with a representative byte 0x5A should yield "5A"
bool test_bin_to_hex_N1() {
    const int N = 1;
    std::vector<unsigned char> buf(2*N + 1);
    buf[0] = 0x5A; // input byte
    // Ensure the rest is non-retained
    buf[1] = 0;
    buf[2] = 0;
    sqlite3TestBinToHex(buf.data(), N);
    // Expect "5A" + '\0'
    if (buf[0] != '5' || buf[1] != 'A' || buf[2] != 0) {
        std::cerr << "test_bin_to_hex_N1: expected result '5A' with terminator, got '"
                  << buf[0] << buf[1] << "' and terminator " << int(buf[2]) << "\n";
        return false;
    }
    return true;
}

// Test 3: N = 2 with bytes {0x00, 0xFF} should yield "00FF"
bool test_bin_to_hex_N2_mixed() {
    const int N = 2;
    std::vector<unsigned char> buf(2*N + 1);
    buf[0] = 0x00;
    buf[1] = 0xFF;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    sqlite3TestBinToHex(buf.data(), N);
    // Expect "00FF" + '\0'
    if (buf[0] != '0' || buf[1] != '0' || buf[2] != 'F' || buf[3] != 'F' || buf[4] != 0) {
        std::cerr << "test_bin_to_hex_N2_mixed: expected '00FF' with terminator, got: "
                  << static_cast<char>(buf[0]) << static_cast<char>(buf[1])
                  << static_cast<char>(buf[2]) << static_cast<char>(buf[3])
                  << " terminator=" << int(buf[4]) << "\n";
        return false;
    }
    return true;
}

// Test 4: N = 3 with bytes {0x12, 0x34, 0x56} should yield "123456"
bool test_bin_to_hex_N3() {
    const int N = 3;
    std::vector<unsigned char> buf(2*N + 1);
    buf[0] = 0x12;
    buf[1] = 0x34;
    buf[2] = 0x56;
    // Initialize rest
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 0;
    sqlite3TestBinToHex(buf.data(), N);
    // Expect "12" "34" "56" + '\0'
    if (buf[0] != '1' || buf[1] != '2' ||
        buf[2] != '3' || buf[3] != '4' ||
        buf[4] != '5' || buf[5] != '6' || buf[6] != 0) {
        std::cerr << "test_bin_to_hex_N3: expected '123456' with terminator, got: "
                  << buf[0] << buf[1] << buf[2] << buf[3] << buf[4] << buf[5]
                  << " terminator=" << int(buf[6]) << "\n";
        return false;
    }
    return true;
}

// Test 5: N = 5 with a mixed set including 0x01, 0x23, 0x45, 0x67, 0x89 should yield "0123456789"
bool test_bin_to_hex_N5() {
    const int N = 5;
    std::vector<unsigned char> buf(2*N + 1);
    unsigned char inputs[N] = {0x01, 0x23, 0x45, 0x67, 0x89};
    for (int i = 0; i < N; ++i) buf[i] = inputs[i];
    // Ensure terminator space
    for (int i = 0; i < 2*N+1; ++i) if (i >= N) buf[i] = 0;
    sqlite3TestBinToHex(buf.data(), N);
    // Expect "0123456789"
    const char expected[] = "0123456789";
    bool ok = std::strncmp(reinterpret_cast<char*>(buf.data()), expected, 2*N) == 0 && buf[2*N] == 0;
    if (!ok) {
        std::cerr << "test_bin_to_hex_N5: expected \"" << expected << "\" with terminator, got '";
        std::string out(reinterpret_cast<char*>(buf.data()));
        std::cerr << out << "'\n";
    }
    return ok;
}

// ---------------------- Main ----------------------

int main() {
    std::cout << "Starting sqlite3TestBinToHex test suite (C++11)..." << std::endl;

    // Run individual tests
    RUN_TEST(test_bin_to_hex_N0);
    RUN_TEST(test_bin_to_hex_N1);
    RUN_TEST(test_bin_to_hex_N2_mixed);
    RUN_TEST(test_bin_to_hex_N3);
    RUN_TEST(test_bin_to_hex_N5);

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}