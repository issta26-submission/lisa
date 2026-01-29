// Test suite for gen_portatom6 in gencode.c
// This test is written in C++11 without using GoogleTest.
// It focuses on verifying that gen_portatom6 forwards its parameters to gen_cmp
// with the correct constants (OR_TRAN_IPV6 and BPF_H).

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
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
#include <cstring>


// Step 1: Candidate Keywords (core dependencies of the focal method)
 // - gen_portatom6: the function under test
 // - gen_cmp: the underlying function it delegates to
 // - OR_TRAN_IPV6: the address-family/offset relation constant used by IPv6 path
 // - BPF_H: the size specifier passed to gen_cmp (likely "half-word"/16-bit)
 // - cstate: compiler_state_t pointer (stateful context for the BPF generator)
 // - off: offset parameter passed through
 // - v: value parameter passed through
 // The test asserts that gen_portatom6(cstate, off, v) == gen_cmp(cstate, OR_TRAN_IPV6, off, BPF_H, v)

extern "C" {
}

// A lightweight test harness that mirrors EXPECT-style assertions without terminating
// the test run on first failure, to maximize coverage.
static bool global_tests_passed = true;

#define EXPECT_EQ(a, b) do {                                           \
    auto _aa = (a);                                                      \
    auto _bb = (b);                                                      \
    if (!(_aa == _bb)) {                                                \
        std::cerr << "EXPECT_EQ failed: " #a " (" << _aa << ") != "       \
                  << #b << " (" << _bb << ")" << std::endl;            \
        global_tests_passed = false;                                    \
    }                                                                   \
} while (0)


// Helper to allocate a zero-initialized compiler_state_t instance.
// Returns nullptr on allocation failure.
static compiler_state_t* create_cstate() {
    compiler_state_t *cstate = (compiler_state_t*)calloc(1, sizeof(compiler_state_t));
    if (cstate == nullptr) {
        std::cerr << "Failed to allocate compiler_state_t" << std::endl;
        global_tests_passed = false;
    }
    return cstate;
}

// Helper to clean up compiler_state_t instance.
static void destroy_cstate(compiler_state_t* cstate) {
    if (cstate) {
        free(cstate);
    }
}

// Test 1: Forwarding correctness for multiple (off, v) pairs
// Ensure that gen_portatom6 forwards its arguments to gen_cmp with the exact
// constants OR_TRAN_IPV6 and BPF_H, i.e., result should match a direct call
// to gen_cmp with those constants.
static void test_portatom6_forwards_to_gen_cmp_ipv6() {
    // Prepare a fresh compiler state
    compiler_state_t* cstate = create_cstate();
    if (!cstate) return;

    // Define a set of representative test cases
    struct TestCase {
        int off;
        uint16_t v;
    } cases[] = {
        {0,     0x1234},
        {4,     0x0000},
        {8,     0xFFFF},
        {15,    0xABCD},
        {127,   0x1A2B}
    };

    const size_t num_cases = sizeof(cases) / sizeof(cases[0]);

    for (size_t i = 0; i < num_cases; ++i) {
        int off = cases[i].off;
        uint16_t v = cases[i].v;

        // Call the function under test
        int portatom6_res = gen_portatom6(cstate, off, v);

        // Compute the expected result by calling the underlying function directly
        int expected_res = gen_cmp(cstate, OR_TRAN_IPV6, off, BPF_H, v);

        // Compare results
        EXPECT_EQ(portatom6_res, expected_res);
    }

    destroy_cstate(cstate);
}

// Test 2: Sanity check with a single edge case to ensure no crashes on minimal inputs
// This also exercises the path with a small offset and a small value.
static void test_portatom6_edge_case_min_input() {
    compiler_state_t* cstate = create_cstate();
    if (!cstate) return;

    int off = 0;
    uint16_t v = 0;

    int res_portatom6 = gen_portatom6(cstate, off, v);
    int res_cmp = gen_cmp(cstate, OR_TRAN_IPV6, off, BPF_H, v);

    EXPECT_EQ(res_portatom6, res_cmp);

    destroy_cstate(cstate);
}

// Entry point for running all tests
static void run_all_tests() {
    test_portatom6_forwards_to_gen_cmp_ipv6();
    test_portatom6_edge_case_min_input();
}

// Main function as required when gtest is not used.
// It executes the test suite and returns non-zero when failures occurred.
int main() {
    run_all_tests();

    if (global_tests_passed) {
        std::cout << "All tests passed for gen_portatom6 forwarding behavior." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed for gen_portatom6 forwarding behavior." << std::endl;
        return 1;
    }
}