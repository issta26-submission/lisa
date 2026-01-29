// Lightweight C++11 test suite for the focal C function:
// pcapint_atomac48_x_xx_6_times(const char *s, uint8_t *addr)
// The implementation under test is assumed to be provided in the linked C source
// nametoaddr.c and exposed with C linkage.
//
// This test harness avoids Google Test, instead using a small, non-terminating
// assertion framework suitable for stepwise coverage of the function's behavior.
//
// Notes:
// - We declare the function with C linkage for direct invocation from C++.
// - We focus on both true and false branches of the parsing FSM, including
//   separator handling, hex digit parsing, and final acceptance.
// - We verify both the return value and the resulting 6-byte address where applicable.

#include <gencode.h>
#include <stdio.h>
#include <ethertype.h>
#include <iostream>
#include <errno.h>
#include <nametoaddr.h>
#include <iomanip>
#include <thread-local.h>
#include <cstdint>
#include <string>
#include <llc.h>
#include <pcap/namedb.h>
#include <diag-control.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <cstddef>
#include <os-proto.h>
#include <config.h>
#include <cstring>


// Forward declaration of the focal function (external C linkage)
extern "C" int pcapint_atomac48_x_xx_6_times(const char *s, uint8_t *addr);

// Simple non-terminating assertion framework
static int g_tests_run = 0;
static int g_tests_pass = 0;

// Print helper for bytes in hex
static void print_bytes(const uint8_t *addr, size_t n, std::ostream &os) {
    os << std::hex << std::setfill('0');
    for (size_t i = 0; i < n; ++i) {
        os << std::setw(2) << (unsigned)addr[i];
        if (i + 1 < n) os << " ";
    }
    os << std::dec;
}

// Generic equality assertion for scalar types
template <typename T>
static void expect_eq(const T &a, const T &b, const char *desc) {
    ++g_tests_run;
    if (a == b) {
        ++g_tests_pass;
    } else {
        std::cerr << "TEST FAIL: " << desc << "\n"
                  << "  Expected: " << b << "\n"
                  << "  Actual:   " << a << "\n";
    }
}

// Specialization for comparing 6-byte addresses
static void expect_addr_eq(const uint8_t *a, const uint8_t *b, size_t n, const char *desc) {
    ++g_tests_run;
    if (a != nullptr && b != nullptr && std::memcmp(a, b, n) == 0) {
        ++g_tests_pass;
    } else {
        std::cerr << "TEST FAIL: " << desc << "\n"
                  << "  Expected: ";
        print_bytes(b, n, std::cerr);
        std::cerr << "\n  Actual:   ";
        print_bytes(a, n, std::cerr);
        std::cerr << "\n";
    }
}

// A sentinel to initialize destination buffers for failure tests
static void init_addr(uint8_t *addr, size_t n, uint8_t val) {
    std::memset(addr, val, n);
}

// Test 1: Valid colon-separated input
static void test_valid_colon_separated() {
    const char input[] = "01:23:45:67:89:ab";
    uint8_t addr[6];
    init_addr(addr, 6, 0xFF);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 1, "rc should be 1 for valid colon-separated input");
    const uint8_t expected[6] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab };
    expect_addr_eq(addr, expected, 6, "addr should match expected bytes for colon input");
}

// Test 2: Valid dash-separated input
static void test_valid_dash_separated() {
    const char input[] = "01-23-45-67-89-ab";
    uint8_t addr[6];
    init_addr(addr, 6, 0xEE);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 1, "rc should be 1 for valid dash-separated input");
    const uint8_t expected[6] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab };
    expect_addr_eq(addr, expected, 6, "addr should match expected bytes for dash input");
}

// Test 3: Valid dot-separated input
static void test_valid_dot_separated() {
    const char input[] = "01.23.45.67.89.ab";
    uint8_t addr[6];
    init_addr(addr, 6, 0xAA);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 1, "rc should be 1 for valid dot-separated input");
    const uint8_t expected[6] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab };
    expect_addr_eq(addr, expected, 6, "addr should match expected bytes for dot input");
}

// Test 4: Uppercase hex digits in final byte
static void test_uppercase_final_byte() {
    const char input[] = "01:23:45:67:89:AB";
    uint8_t addr[6];
    init_addr(addr, 6, 0x00);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 1, "rc should be 1 for uppercase final byte");
    const uint8_t expected[6] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB };
    expect_addr_eq(addr, expected, 6, "addr should match expected bytes for uppercase final byte");
}

// Test 5: Insufficient digits (incomplete address)
static void test_insufficient_digits() {
    const char input[] = "01:23:45:67:89";
    uint8_t addr[6];
    init_addr(addr, 6, 0x11);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 0, "rc should be 0 for incomplete input (insufficient bytes)");
    // addr should remain unchanged
    const uint8_t expected[6] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
    expect_addr_eq(addr, expected, 6, "addr should remain unchanged on failure (incomplete input)");
}

// Test 6: Trailing extra data after 6 bytes
static void test_trailing_extra_after_bytes() {
    const char input[] = "01:23:45:67:89:ab:cd";
    uint8_t addr[6];
    init_addr(addr, 6, 0x22);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 0, "rc should be 0 when extra data is provided after 6 bytes");
    const uint8_t expected[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // Since the function should not modify addr on failure, we can't rely on a default.
    // However, we can still ensure the data did not erroneously become valid 6-byte sequence.
    // We compare to a known initial value.
    expect_addr_eq(addr, expected, 6, "addr should reflect initial values after failure (no modify)");
}

// Test 7: Inconsistent separator mid-stream
static void test_inconsistent_separator() {
    const char input[] = "01:23;45:67:89:ab"; // semicolon not part of seplist after first sep
    uint8_t addr[6];
    init_addr(addr, 6, 0x33);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 0, "rc should be 0 for inconsistent separator mid-stream");
    const uint8_t expected[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    expect_addr_eq(addr, expected, 6, "addr should remain unchanged on inconsistent separators");
}

// Test 8: No separators (invalid per FSM)
static void test_no_separators_invalid() {
    const char input[] = "0123456789ab";
    uint8_t addr[6];
    init_addr(addr, 6, 0x44);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 0, "rc should be 0 when no separators are used (invalid format)");
    const uint8_t expected[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    expect_addr_eq(addr, expected, 6, "addr should remain unchanged when invalid format");
}

// Test 9: Leading non-hex character
static void test_leading_non_hex() {
    const char input[] = "g1:23:45:67:89:ab";
    uint8_t addr[6];
    init_addr(addr, 6, 0x55);
    int rc = pcapint_atomac48_x_xx_6_times(input, addr);
    expect_eq(rc, 0, "rc should be 0 when input starts with non-hex digit");
    const uint8_t expected[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    expect_addr_eq(addr, expected, 6, "addr should remain unchanged on leading invalid input");
}

// Runner: execute all tests and print a summary
static void run_all_tests() {
    std::cout << "Starting tests for pcapint_atomac48_x_xx_6_times...\n";
    test_valid_colon_separated();
    test_valid_dash_separated();
    test_valid_dot_separated();
    test_uppercase_final_byte();
    test_insufficient_digits();
    test_trailing_extra_after_bytes();
    test_inconsistent_separator();
    test_no_separators_invalid();
    test_leading_non_hex();

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_pass
              << ", Failed: " << (g_tests_run - g_tests_pass) << "\n";
}

// Main: entry point for the test program
int main() {
    run_all_tests();
    return (g_tests_run == g_tests_pass) ? 0 : 1;
}