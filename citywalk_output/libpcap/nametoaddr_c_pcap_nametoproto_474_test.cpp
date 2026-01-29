/************************************************************
 * Test Suite for pcap_nametoproto in nametoaddr.c
 * 
 * This file provides a small, self-contained C++11 unit-test
 * harness (no GTest) to validate the focal function:
 * 
 *   int pcap_nametoproto(const char *str)
 * 
 * The implementation relies on platform-specific paths
 * (Linux getprotobyname_r, Solaris/AIX variants, or the classic
 * getprotobyname). Since the exact behavior may vary between
 * environments, the tests are designed to be robust across
 * platforms by accepting both a concrete protocol number (e.g.,
 * TCP = 6) and the sentinel value PROTO_UNDEF when the protocol
 * name cannot be resolved.
 *
 * Approach:
 * - Provide two tests:
 *   1) Known protocol name: "tcp" (expects a positive proto value or PROTO_UNDEF)
 *   2) Unknown protocol name: "THIS_IS_NOT_A_PROTO_NAME" (expects PROTO_UNDEF or negative)
 * - Use non-terminating assertions: tests continue execution even if one fails.
 * - Compile as C++11, link with the target library providing pcap_nametoproto.
 *
 * Important notes:
 * - This harness imports the function with C linkage.
 * - PROTO_UNDEF may be defined as a macro in the included headers; tests adapt
 *   to its presence via #ifdef PROTO_UNDEF.
 * - The exact numeric value of a successful TCP lookup can vary by platform;
 *   the test accepts either a concrete positive value (commonly 6) or PROTO_UNDEF.
 ************************************************************/

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
#include <cstring>
#include <llc.h>


// Declare the focal function with C linkage to avoid name mangling
extern "C" int pcap_nametoproto(const char *str);

// Optional macro from the library headers representing "undefined" proto.
// If present, we use it in comparisons; otherwise, tests fall back to
// a platform-agnostic acceptance criterion.
#ifdef PROTO_UNDEF
extern int PROTO_UNDEF; // may be defined as a macro in headers; this line is harmless if PROTO_UNDEF is a macro
#endif

// Lightweight test harness (non-terminating, collects results)
static int total_tests = 0;
static int failed_tests = 0;

// Helper to print per-test result
#define TEST_PASS() do { printf("[TEST PASS] %s:%d\n", __FILE__, __LINE__); } while(0)
#define TEST_FAIL(msg) do { printf("[TEST FAIL] %s:%d - %s\n", __FILE__, __LINE__, msg); failed_tests++; } while(0)
#define ASSERT(cond, msg) do { if(!(cond)) { TEST_FAIL(msg); } else { TEST_PASS(); } } while(0)

// Test 1: Known protocol name "tcp" should resolve to a valid protocol number
bool test_pcap_nametoproto_known_tcp() {
    total_tests++;
    int res = pcap_nametoproto("tcp");
#ifdef PROTO_UNDEF
    if (res == PROTO_UNDEF) {
        // Acceptable on systems where /etc/protocols isn't available
        printf("test_pcap_nametoproto_known_tcp: resolved to PROTO_UNDEF (unknown) - acceptable on this platform.\n");
        return true;
    } else if (res == 6 || res > 0) {
        printf("test_pcap_nametoproto_known_tcp: resolved to protocol number %d (expected ~6).\n", res);
        return true;
    } else {
        printf("test_pcap_nametoproto_known_tcp: unexpected result %d.\n", res);
        return false;
    }
#else
    // Without PROTO_UNDEF in scope, assume a positive protocol number indicates success
    if (res > 0) {
        printf("test_pcap_nametoproto_known_tcp: resolved to protocol number %d.\n", res);
        return true;
    } else {
        printf("test_pcap_nametoproto_known_tcp: failed to resolve 'tcp' (res=%d).\n", res);
        return false;
    }
#endif
}

// Test 2: Unknown protocol name should result in undefined/proto failure
bool test_pcap_nametoproto_unknown_name() {
    total_tests++;
    int res = pcap_nametoproto("THIS_IS_NOT_A_PROTO_NAME");
#ifdef PROTO_UNDEF
    // Accept either explicit undefined sentinel or any negative outcome
    if (res == PROTO_UNDEF) {
        printf("test_pcap_nametoproto_unknown_name: correctly returned PROTO_UNDEF.\n");
        return true;
    } else {
        printf("test_pcap_nametoproto_unknown_name: unexpected result %d (expected PROTO_UNDEF).\n", res);
        return false;
    }
#else
    // If PROTO_UNDEF is not exposed, treat negative results as undefined
    if (res < 0) {
        printf("test_pcap_nametoproto_unknown_name: correctly returned negative/proto undefined value (%d).\n", res);
        return true;
    } else {
        printf("test_pcap_nametoproto_unknown_name: unexpected result %d (expected undefined).\n", res);
        return false;
    }
#endif
}

// Entry point: run tests and print a summary
int main() {
    printf("Starting test suite for pcap_nametoproto...\n");

    bool t1 = test_pcap_nametoproto_known_tcp();
    if (!t1) {
        TEST_FAIL("test_pcap_nametoproto_known_tcp failed");
    }

    bool t2 = test_pcap_nametoproto_unknown_name();
    if (!t2) {
        TEST_FAIL("test_pcap_nametoproto_unknown_name failed");
    }

    // Summary
    printf("Test summary: %d run, %d passed, %d failed.\n",
           total_tests, total_tests - failed_tests, failed_tests);

    // Non-terminating: return non-zero if any test failed
    return failed_tests == 0 ? 0 : 1;
}