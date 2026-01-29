// C++11 unit test suite for the focal function pcapint_atoan in nametoaddr.c
// This test harness does not rely on GTest or any external testing framework.
// It uses a lightweight, non-terminating assertion style and prints PASS/FAIL per test.

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <cstdint>
#include <gencode.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <ethertype.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <llc.h>


// Declaration of the focal C function under test.
// The project compiles the rest of the codebase; we link against the existing object(s).
extern "C" int pcapint_atoan(const char *s, uint8_t *addr);

//
// Helper: prints concise test result
//
static bool report_test(const char* name, bool ok) {
    if (ok) {
        std::printf("PASS: %s\n", name);
    } else {
        std::printf("FAIL: %s\n", name);
    }
    return ok;
}

//
// Test 1: Valid single-hex-digit after '$' (e.g., "$A" -> 0x0A, returns 1)
// Domain knowledge alignment: ensure single-digit hex path initializes correctly.
// The FSM accepts 1 or 2 hex digits after '$' and end-of-string.
//
static bool test_pcapint_atoan_single_hex() {
    const char* s = "$A";
    uint8_t addr = 0xFF; // sentinel to verify write on success
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 1) && (addr == 0x0A);
    return ok;
}

//
// Test 2: Valid two-hex-digit after '$' (e.g., "$1F" -> 0x1F, returns 1)
// Ensures proper accumulation of two digits via the FSM (HEX1 -> HEX2).
//
static bool test_pcapint_atoan_two_digits() {
    const char* s = "$1F";
    uint8_t addr = 0x00;
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 1) && (addr == 0x1F);
    return ok;
}

//
// Test 3: Invalid input missing leading '$' (e.g., "A1" should fail, addr unchanged)
// This tests the START predicate behavior.
// 
static bool test_pcapint_atoan_invalid_no_dollar() {
    const char* s = "A1";
    uint8_t addr = 0x55; // ensure it is not overwritten on failure
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 0) && (addr == 0x55);
    return ok;
}

//
// Test 4: Invalid input with only '$' (no hex digits) -> should fail, addr unchanged
//
static bool test_pcapint_atoan_invalid_no_hex_after_dollar() {
    const char* s = "$";
    uint8_t addr = 0xAA;
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 0) && (addr == 0xAA);
    return ok;
}

//
// Test 5: Invalid input with three hex digits after '$' (e.g., "$1A3" should fail)
// The FSM rejects after HEX2 on receiving a third digit.
// 
static bool test_pcapint_atoan_invalid_three_digits() {
    const char* s = "$1A3";
    uint8_t addr = 0x00;
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 0) && (addr == 0x00);
    return ok;
}

//
// Test 6: Invalid input with non-hex following '$' (e.g., "$G" should fail)
// Tests PCAP_ISXDIGIT predicate usage for the first hex digit.
//
static bool test_pcapint_atoan_invalid_non_hex_after_dollar() {
    const char* s = "$G";
    uint8_t addr = 0x01;
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 0) && (addr == 0x01);
    return ok;
}

//
// Test 7: Valid single-digit after '$' with end-of-string (e.g., "$0" -> 0x00)
//
static bool test_pcapint_atoan_single_zero() {
    const char* s = "$0";
    uint8_t addr = 0x12;
    int r = pcapint_atoan(s, &addr);
    bool ok = (r == 1) && (addr == 0x00);
    return ok;
}

//
// Main: run all tests, summarize results, return non-zero if any test failed.
// The tests are designed to execute in a standalone manner without terminating the program on failure.
//
int main() {
    bool all_passed = true;

    all_passed &= report_test("pcapint_atoan_single_hex (\"$A\" -> 0x0A)", test_pcapint_atoan_single_hex());
    all_passed &= report_test("pcapint_atoan_two_digits (\"$1F\" -> 0x1F)", test_pcapint_atoan_two_digits());
    all_passed &= report_test("pcapint_atoan_invalid_no_dollar (\"A1\")", test_pcapint_atoan_invalid_no_dollar());
    all_passed &= report_test("pcapint_atoan_invalid_no_hex_after_dollar (\"$\")", test_pcapint_atoan_invalid_no_hex_after_dollar());
    all_passed &= report_test("pcapint_atoan_invalid_three_digits (\"$1A3\")", test_pcapint_atoan_invalid_three_digits());
    all_passed &= report_test("pcapint_atoan_invalid_non_hex_after_dollar (\"$G\")", test_pcapint_atoan_invalid_non_hex_after_dollar());
    all_passed &= report_test("pcapint_atoan_single_zero (\"$0\" -> 0x00)", test_pcapint_atoan_single_zero());

    if (!all_passed) {
        std::printf("Some tests FAILED. Review the logs above for details.\n");
        return 1;
    }
    std::printf("All tests PASSED.\n");
    return 0;
}