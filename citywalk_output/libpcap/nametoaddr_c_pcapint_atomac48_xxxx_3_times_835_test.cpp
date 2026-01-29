/*
Unit test suite for focal method:
  int pcapint_atomac48_xxxx_3_times(const char *s, uint8_t *addr)

Notes:
- This C/C++-callable function parses a very specific 14-char string format:
  "XXXX.XXXX.XXXX" where each X is a hex digit. It fills 6 bytes (addr[0..5])
  with pairs of hex digits: (s[0],s[1]) -> addr[0], (s[2],s[3]) -> addr[1],
  (s[5],s[6]) -> addr[2], (s[7],s[8]) -> addr[3], (s[10],s[11]) -> addr[4],
  (s[12],s[13]) -> addr[5]. Returns 1 on success, 0 otherwise.

- The test suite is written in C++11, without GTest, and uses a small, non-terminating
  assertion mechanism (EXPECT-style) implemented inside the test code itself.

- We link against the existing C implementation (nametoaddr.c and dependencies) by
  declaring the function with extern "C" linkage.

- The test covers:
  - Positive case with mixed-case hex digits
  - Positive case with a different valid hex payload
  - Several negative cases covering length mismatch, non-hex characters, and wrong separators

Candidate Keywords and Dependencies (derived from the focal method and class dependencies):
- Hex-digit recognition: PCAP_ISXDIGIT, pcapint_xdtoi
- Parsing structure: delimiter '.', exact length 14
- Address assembly: addr[0..5] composition from sequential hex pairs
- Return semantics: 1 on success, 0 on failure
- Static/extern linkage: functions declared with C linkage when called from C++

This test suite assumes the project builds under C++11 and that the C function pcapint_atomac48_xxxx_3_times is linkable.

*/

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <cstdint>
#include <gencode.h>
#include <cstddef>
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


// Linkage to the focal C function under test
#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test
int pcapint_atomac48_xxxx_3_times(const char *s, uint8_t *addr);
#ifdef __cplusplus
}
#endif

// Minimal, non-terminating test assertions (EXPECT-style) implemented for this suite
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if ((expected) != (actual)) { \
        printf("EXPECT_FAIL: %s. Expected %ld, got %ld\n", (msg), (long)(expected), (long)(actual)); \
        ++g_tests_failed; \
    } else { \
        printf("EXPECT_PASS: %s\n", (msg)); \
    } \
} while (0)

#define EXPECT_MEMEQ(expected, actual, len, msg) do { \
    ++g_tests_run; \
    if (std::memcmp((expected), (actual), (len)) != 0) { \
        printf("EXPECT_FAIL: %s. Memory blocks differ.\n", (msg)); \
        ++g_tests_failed; \
    } else { \
        printf("EXPECT_PASS: %s\n", (msg)); \
    } \
} while (0)

// Helper to print a byte array (for debugging if needed)
static void print_bytes(const uint8_t* b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%02X", b[i]);
    }
    printf("\n");
}

// Test harness: run one test case
static void run_case(const char* name,
                     const char* s,
                     const uint8_t* expected_addr,
                     int expect_ret)
{
    uint8_t addr[6];
    // Initialize addr with a non-zero pattern to detect unintended writes
    for (size_t i = 0; i < 6; ++i) addr[i] = 0xAA;
    int ret = pcapint_atomac48_xxxx_3_times(s, addr);
    char msg[256];
    snprintf(msg, sizeof(msg), "Case '%s' return value", name);
    EXPECT_EQ(expect_ret, ret, msg);

    if (expect_ret == 1) {
        snprintf(msg, sizeof(msg), "Case '%s' address bytes match", name);
        EXPECT_MEMEQ(expected_addr, addr, 6, msg);
    } else {
        // For negative cases, we also verify that addr was not modified (remains AA)
        uint8_t expected_all_aa[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
        snprintf(msg, sizeof(msg), "Case '%s' no modification to addr", name);
        EXPECT_MEMEQ(expected_all_aa, addr, 6, msg);
    }
}

int main() {
    // Step 2: Unit Test Generation
    // Positive test 1: "1a2b.3c4d.5e6f" -> 0x1a 0x2b 0x3c 0x4d 0x5e 0x6f
    const char test1_s[] = "1a2b.3c4d.5e6f";
    const uint8_t test1_exp[6] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F};
    run_case("Valid mixed-case hex string 1", test1_s, test1_exp, 1);

    // Positive test 2: "A1B2.CD4D.5EFF" -> 0xA1 0xB2 0xCD 0x4D 0x5E 0xFF
    const char test2_s[] = "A1B2.CD4D.5EFF";
    const uint8_t test2_exp[6] = {0xA1, 0xB2, 0xCD, 0x4D, 0x5E, 0xFF};
    run_case("Valid mixed-case hex string 2", test2_s, test2_exp, 1);

    // Negative test 1: Wrong length
    // 15 chars: should fail, and not modify addr
    const char test3_s[] = "1a2b.3c4d.5e6fX"; // length 15
    const uint8_t test3_exp[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // not used on success
    run_case("Invalid length", test3_s, test3_exp, 0);

    // Negative test 2: Non-hex character in hex region
    const char test4_s[] = "1g2b.3c4d.5e6f"; // 'g' is not hex
    run_case("Non-hex character in hex region", test4_s, test3_exp, 0);

    // Negative test 3: Wrong separators
    const char test5_s[] = "1a2b,3c4d.5e6f"; // first separator is comma
    run_case("Wrong separator character", test5_s, test3_exp, 0);

    // Summary
    printf("\nTest run complete. Total: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    return g_tests_failed ? 1 : 0;
}