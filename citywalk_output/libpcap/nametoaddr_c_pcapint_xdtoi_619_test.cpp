/*
Unit Test Suite for focal method: pcapint_xdtoi(const u_char c)

Step 1 – Program Understanding (Keywords)
- Focus: pcapint_xdtoi converts a single hex character to its numeric nibble value.
- Core paths:
  - True branch: digits '0'..'9' -> 0..9
  - True branch: lowercase 'a'..'f' -> 10..15
  - Else branch (covers uppercase 'A'..'Z' and any other char): c - 'A' + 10
- Important edge/branch coverage:
  - Boundary digits: '0', '9'
  - Boundary lowercase hex: 'a', 'f'
  - Boundary uppercase hex: 'A', 'F'
  - Other uppercase letters: 'B', 'Z'
  - Other non-hex chars: '?', 'g'
- Dependencies: None beyond the focal function; test will declare the function as extern "C".
- Static considerations: none in this focal function; no static member exposure required.

Step 2 – Unit Test Generation (Test Plan)
- Validate all branches with representative inputs:
  - Digit path: '0' and '9'
  - Lowercase hex path: 'a' and 'f'
  - Uppercase hex path: 'A' and 'F'
  - Other uppercase letters to exercise else path: 'B', 'Z'
  - Non-hex characters to exercise else path with non-letter: '?' and 'g'
- Output style: non-terminating assertions; do not abort on first failure.
- Language: C++11, no GTest; a lightweight in-file test harness is provided.

Step 3 – Test Case Refinement (Domain Knowledge)
- Use straightforward integer comparisons; print PASS for successful tests and FAIL with details for failures.
- Access the focal C function from C++ via extern "C".
- All tests are self-contained and do not rely on private state or external mocks.

Below is the test code. Compile with the C source (nametoaddr.c) linked in, ensuring the symbol pcapint_xdtoi is available.

*/

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <cstdlib>
#include <gencode.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <ethertype.h>
#include <config.h>
#include <pcap-int.h>
#include <llc.h>


// Step 2: Expose the focal C function to C++
// The function is defined in nametoaddr.c as:
//     pcapint_xdtoi(const u_char c)
extern "C" unsigned char pcapint_xdtoi(const unsigned char c);

// Global failure counter to support non-terminating test execution
static int g_failures = 0;

// Helper: report integer equality result
static void expect_eq_int(const char* test_name, int expected, int actual) {
    if (expected != actual) {
        std::fprintf(stderr, "[FAIL] %s: expected %d, got %d\n", test_name, expected, actual);
        ++g_failures;
    } else {
        std::printf("[PASS] %s\n", test_name);
    }
}

/*
Unit Tests for pcapint_xdtoi
- Test 1: Digit branch for '0'
- Test 2: Digit branch for '9'
- Test 3: Lowercase hex branch for 'a'
- Test 4: Lowercase hex branch for 'f'
- Test 5: Uppercase hex branch for 'A'
- Test 6: Uppercase hex branch for 'F'
- Test 7: Uppercase else branch for 'B'
- Test 8: Uppercase else branch for 'Z'
- Test 9: Else branch for non-hex character '?'
- Test 10: Else branch for non-hex lowercase-ish 'g'
*/

// Test 1: '0' -> 0
static void test_pcapint_xdtoi_digit_0() {
    unsigned char r = pcapint_xdtoi((unsigned char)'0');
    expect_eq_int("pcapint_xdtoi('0')", 0, (int)r);
}

// Test 2: '9' -> 9
static void test_pcapint_xdtoi_digit_9() {
    unsigned char r = pcapint_xdtoi((unsigned char)'9');
    expect_eq_int("pcapint_xdtoi('9')", 9, (int)r);
}

// Test 3: 'a' -> 10
static void test_pcapint_xdtoi_lower_a() {
    unsigned char r = pcapint_xdtoi((unsigned char)'a');
    expect_eq_int("pcapint_xdtoi('a')", 10, (int)r);
}

// Test 4: 'f' -> 15
static void test_pcapint_xdtoi_lower_f() {
    unsigned char r = pcapint_xdtoi((unsigned char)'f');
    expect_eq_int("pcapint_xdtoi('f')", 15, (int)r);
}

// Test 5: 'A' -> 10
static void test_pcapint_xdtoi_upper_A() {
    unsigned char r = pcapint_xdtoi((unsigned char)'A');
    expect_eq_int("pcapint_xdtoi('A')", 10, (int)r);
}

// Test 6: 'F' -> 15
static void test_pcapint_xdtoi_upper_F() {
    unsigned char r = pcapint_xdtoi((unsigned char)'F');
    expect_eq_int("pcapint_xdtoi('F')", 15, (int)r);
}

// Test 7: 'B' -> 11 (upper-case non-hex path)
static void test_pcapint_xdtoi_upper_B() {
    unsigned char r = pcapint_xdtoi((unsigned char)'B');
    // 'B' -> (66 - 65) + 10 = 11
    expect_eq_int("pcapint_xdtoi('B')", 11, (int)r);
}

// Test 8: 'Z' -> 35 (upper-case non-hex path)
static void test_pcapint_xdtoi_upper_Z() {
    unsigned char r = pcapint_xdtoi((unsigned char)'Z');
    // 'Z' -> (90 - 65) + 10 = 25 + 10 = 35
    expect_eq_int("pcapint_xdtoi('Z')", 35, (int)r);
}

// Test 9: '?' -> 8 (non-hex non-letter path; 63 - 65 + 10 = 8)
static void test_pcapint_xdtoi_question() {
    unsigned char r = pcapint_xdtoi((unsigned char)'?');
    expect_eq_int("pcapint_xdtoi('?')", 8, (int)r);
}

// Test 10: 'g' -> 48 (non-hex lowercase beyond 'f'; 103 - 65 + 10 = 48)
static void test_pcapint_xdtoi_lower_g() {
    unsigned char r = pcapint_xdtoi((unsigned char)'g');
    expect_eq_int("pcapint_xdtoi('g')", 48, (int)r);
}

// Simple test runner
int main() {
    // Run all tests
    test_pcapint_xdtoi_digit_0();
    test_pcapint_xdtoi_digit_9();
    test_pcapint_xdtoi_lower_a();
    test_pcapint_xdtoi_lower_f();
    test_pcapint_xdtoi_upper_A();
    test_pcapint_xdtoi_upper_F();
    test_pcapint_xdtoi_upper_B();
    test_pcapint_xdtoi_upper_Z();
    test_pcapint_xdtoi_question();
    test_pcapint_xdtoi_lower_g();

    // Summary
    if (g_failures == 0) {
        std::printf("[SUMMARY] All tests PASSED\n");
    } else {
        std::fprintf(stderr, "[SUMMARY] Total failures: %d\n", g_failures);
    }

    return g_failures;
}