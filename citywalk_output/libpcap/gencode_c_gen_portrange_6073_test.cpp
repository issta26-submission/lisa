// Unit test suite for focal method: gen_portrange in gencode.c
// Language: C++11
// Testing framework: lightweight in-file test harness (no GoogleTest)
// Notes:
// - This test calls the actual gen_portrange function from the project's C sources.
// - It uses extern "C" to link with C code.
// - It aims to exercise true/false branches of the switch by passing each supported dir value.
// - Each test is annotated with comments explaining its purpose and expected behavior.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <cstdint>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


// Bring in the C declarations. Use extern "C" to prevent name mangling when linked with C code.
extern "C" {
}

// Lightweight test harness state
static int g_passed = 0;
static int g_failed = 0;

// Simple non-terminating assertion macro
#define TEST_ASSERT(cond, testname) do { \
    if (cond) { \
        ++g_passed; \
        printf("[OK] %s\n", testname); \
    } else { \
        ++g_failed; \
        fprintf(stderr, "[FAIL] %s\n", testname); \
    } \
} while (0)

// Convenience: test that a pointer is non-null
static void expect_not_null(const void* p, const char* testname) {
    TEST_ASSERT(p != nullptr, testname);
}

// Test 1: Verify behavior when dir == Q_SRC
// Purpose: Ensure gen_portrange executes the Q_SRC branch and returns a non-null block chain.
// This covers the true path for the Src-port range variant.
static void test_portrange_src() {
    const char* testname = "gen_portrange_src (Q_SRC) returns non-null block";
    // Allocate a minimal compiler_state_t to satisfy the callee's expectations.
    compiler_state_t *cstate = static_cast<compiler_state_t*>(calloc(1, sizeof(compiler_state_t)));
    if (!cstate) {
        fprintf(stderr, "Memory allocation failed for compiler_state_t in test_portrange_src\n");
        ++g_failed;
        return;
    }

    // Use representative values for port1/port2 and a typical protocol value (e.g., 6 for TCP).
    uint16_t port1 = 1000;
    uint16_t port2 = 2000;
    int proto = 6;      // TCP
    int dir = Q_SRC;      // Src predicate

    // Call the focal function
    struct block *result = gen_portrange(cstate, port1, port2, proto, dir);

    // Validate: result should be non-null
    expect_not_null(result, testname);

    // Cleanup
    free(cstate);
}

// Test 2: Verify behavior when dir == Q_DST
// Purpose: Ensure gen_portrange executes the Q_DST branch and returns a non-null block.
// This covers the true path for the Dst-port range variant.
static void test_portrange_dst() {
    const char* testname = "gen_portrange_dst (Q_DST) returns non-null block";
    compiler_state_t *cstate = static_cast<compiler_state_t*>(calloc(1, sizeof(compiler_state_t)));
    if (!cstate) {
        fprintf(stderr, "Memory allocation failed for compiler_state_t in test_portrange_dst\n");
        ++g_failed;
        return;
    }

    uint16_t port1 = 3000;
    uint16_t port2 = 4000;
    int proto = 17;       // UDP example
    int dir = Q_DST;        // Dst predicate

    struct block *result = gen_portrange(cstate, port1, port2, proto, dir);
    expect_not_null(result, testname);

    free(cstate);
}

// Test 3: Verify behavior when dir == Q_AND
// Purpose: Ensure gen_portrange executes the Q_AND branch, which combines two portrangeatom calls
// via gen_and, returning a non-null block.
static void test_portrange_and() {
    const char* testname = "gen_portrange_and (Q_AND) returns non-null block";
    compiler_state_t *cstate = static_cast<compiler_state_t*>(calloc(1, sizeof(compiler_state_t)));
    if (!cstate) {
        fprintf(stderr, "Memory allocation failed for compiler_state_t in test_portrange_and\n");
        ++g_failed;
        return;
    }

    uint16_t port1 = 100;
    uint16_t port2 = 65535;
    int proto = 6;        // TCP
    int dir = Q_AND;        // And predicate

    struct block *result = gen_portrange(cstate, port1, port2, proto, dir);
    expect_not_null(result, testname);

    free(cstate);
}

// Test 4: Verify behavior when dir == Q_DEFAULT
// Purpose: Ensure gen_portrange executes the Q_DEFAULT branch, which uses Q_OR logic internally.
// This covers the combined path for the default/or scenario.
static void test_portrange_default_or() {
    const char* testname = "gen_portrange_default_or (Q_DEFAULT) returns non-null block";
    compiler_state_t *cstate = static_cast<compiler_state_t*>(calloc(1, sizeof(compiler_state_t)));
    if (!cstate) {
        fprintf(stderr, "Memory allocation failed for compiler_state_t in test_portrange_default_or\n");
        ++g_failed;
        return;
    }

    uint16_t port1 = 50;
    uint16_t port2 = 150;
    int proto = 1;          // ICMP (example)
    int dir = Q_DEFAULT;      // Default/Or predicate

    struct block *result = gen_portrange(cstate, port1, port2, proto, dir);
    expect_not_null(result, testname);

    free(cstate);
}

// Test 5: Verify behavior when dir == Q_OR
// Purpose: Ensure gen_portrange executes the Q_OR branch (similar to Q_DEFAULT in implementation)
// and returns a non-null block.
static void test_portrange_or() {
    const char* testname = "gen_portrange_or (Q_OR) returns non-null block";
    compiler_state_t *cstate = static_cast<compiler_state_t*>(calloc(1, sizeof(compiler_state_t)));
    if (!cstate) {
        fprintf(stderr, "Memory allocation failed for compiler_state_t in test_portrange_or\n");
        ++g_failed;
        return;
    }

    uint16_t port1 = 1234;
    uint16_t port2 = 5678;
    int proto = 6;          // TCP
    int dir = Q_OR;           // Or predicate

    struct block *result = gen_portrange(cstate, port1, port2, proto, dir);
    expect_not_null(result, testname);

    free(cstate);
}

// Main: Run all tests and print a short summary
int main() {
    printf("Starting test suite for gen_portrange...\n");

    // Run tests covering all switch branches
    test_portrange_src();
    test_portrange_dst();
    test_portrange_and();
    test_portrange_default_or();
    test_portrange_or();

    // Summary
    printf("Test results: %d passed, %d failed\n", g_passed, g_failed);
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}