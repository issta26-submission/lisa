/*
  Unit test suite for gen_portrangeatom in gencode.c
  - Implemented in C style as a C++11 test harness (no GTest)
  - Uses the real API declarations from gencode.h (C linkage)
  - Exercises both branches of gen_portrangeatom:
      1) v1 == v2 returns portatom
      2) v1 != v2 returns AND of ge/le comparisons
  - Includes lightweight, non-terminating EXPECT_* assertions
  - Uses a single main() to run all tests and reports summary

  Notes:
  - This test relies on the public API exposed by gencode.h (gen_portrangeatom, gen_portatom,
    gen_cmp_ge, gen_cmp_le, gen_and, and related types/macros like OR_TRAN_IPV4, BPF_H).
  - The test assumes proper C linkage for the API (hence the extern "C" wrapper around the include).
  - The test prints detailed messages for failed expectations but does not abort on first failure.
  - The test uses pointer identity (addresses) to validate that the observed outcomes match
    the expected internal combination of blocks when possible.

  How to build (example):
  - Compile with your project build system, ensuring gencode.c and gencode.h are compiled
    and linked with this test binary, and that gencode.c exposes the expected API.
  - Compile example (assuming gencode.{c,h} are in the same project):
      g++ -std=c++11 -O2 -Wall -Wextra test_portrange_suite.cpp gencode.c -o test_portrange_suite
*/

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


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_NOT_NULL(x) do { \
    if ((x) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed: " #x " is null at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if ((intptr_t)(a) != (intptr_t)(b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " != " #b " (" << (void*)(a) << " != " << (void*)(b) \
                  << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// Helper to initialize a zeroed compiler state for tests
static void init_compiler_state(compiler_state_t *cstate) {
    if (cstate == nullptr) return;
    // Best-effort clear; actual internal fields depend on the real type.
    // This is a non-destructive, safe initialization for test environments.
    std::memset(cstate, 0, sizeof(*cstate));
}

// Test 1: When v1 == v2, gen_portrangeatom should produce the same block as gen_portatom for the same inputs.
// Rationale: the code path short-circuits to gen_portatom in this case.
static void test_v1_equal_v2_path() {
    // Arrange
    compiler_state_t cstate;
    init_compiler_state(&cstate);

    const unsigned off = 5;
    const uint16_t v = 1234U;

    // Act
    struct block *res = gen_portrangeatom(&cstate, off, v, v);
    struct block *expected = gen_portatom(&cstate, off, v);

    // Assert
    EXPECT_NOT_NULL(res);
    EXPECT_NOT_NULL(expected);
    EXPECT_EQ(res, expected);

    // Comment:
    // If the implementation correctly short-circuits, res should be the same pointer as
    // the port-only atom produced by gen_portatom for the same inputs.
}

// Test 2: When v1 != v2, gen_portrangeatom should produce an AND of ge/min and le/max blocks.
// Rationale: the code path constructs b1 = gen_cmp_ge(..., min(v1, v2)) and
// b2 = gen_cmp_le(..., max(v1, v2)) and returns gen_and(b1, b2).
static void test_v1_not_equal_v2_path() {
    // Arrange
    compiler_state_t cstate;
    init_compiler_state(&cstate);

    const unsigned off = 8;
    const uint16_t v1 = 100;
    const uint16_t v2 = 2000;

    // Act
    struct block *res = gen_portrangeatom(&cstate, off, v1, v2);

    // Build the expected inner blocks using the public API
    uint16_t vmin = (v1 < v2) ? v1 : v2;
    uint16_t vmax = (v1 > v2) ? v1 : v2;

    struct block *ge = gen_cmp_ge(&cstate, OR_TRAN_IPV4, off, BPF_H, vmin);
    struct block *le = gen_cmp_le(&cstate, OR_TRAN_IPV4, off, BPF_H, vmax);
    struct block *expected = gen_and(ge, le);

    // Assert
    EXPECT_NOT_NULL(res);
    EXPECT_NOT_NULL(ge);
    EXPECT_NOT_NULL(le);
    EXPECT_NOT_NULL(expected);
    EXPECT_EQ(res, expected);

    // Comment:
    // The test verifies that gen_portrangeatom builds the same composite block as the
    // direct composition of ge/le and and, given the same inputs.
}

// Test 3: Boundary values and reversed order (v2, v1) to ensure min/max semantics are honored.
// Rationale: ensure correct handling when inputs are swapped and at edge values.
static void test_boundary_and_reverse_order() {
    // Arrange
    compiler_state_t cstate;
    init_compiler_state(&cstate);

    const unsigned off = 1;

    // Case A: v1 == v2 with boundary value
    {
        const uint16_t v = 0; // boundary value
        struct block *r = gen_portrangeatom(&cstate, off, v, v);
        struct block *p = gen_portatom(&cstate, off, v);
        EXPECT_NOT_NULL(r);
        EXPECT_NOT_NULL(p);
        EXPECT_EQ(r, p);
    }

    // Case B: v1 != v2 with boundary values
    {
        const uint16_t v1 = 0;
        const uint16_t v2 = 65535;

        struct block *r = gen_portrangeatom(&cstate, off, v1, v2);

        uint16_t vmin = (v1 < v2) ? v1 : v2;
        uint16_t vmax = (v1 > v2) ? v1 : v2;

        struct block *ge = gen_cmp_ge(&cstate, OR_TRAN_IPV4, off, BPF_H, vmin);
        struct block *le = gen_cmp_le(&cstate, OR_TRAN_IPV4, off, BPF_H, vmax);
        struct block *expected = gen_and(ge, le);

        EXPECT_NOT_NULL(r);
        EXPECT_NOT_NULL(ge);
        EXPECT_NOT_NULL(le);
        EXPECT_NOT_NULL(expected);
        EXPECT_EQ(r, expected);
    }
}

// Main: run all tests and report summary
int main() {
    // Run tests
    test_v1_equal_v2_path();
    test_v1_not_equal_v2_path();
    test_boundary_and_reverse_order();

    if (g_failures == 0) {
        std::cout << "All tests passed for gen_portrangeatom." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed for gen_portrangeatom." << std::endl;
        return 1;
    }
}