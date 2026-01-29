// Unit test suite for the focal method propedom in optimize.c
// This test suite is designed for a C++11 project without Google Test.
// It uses a lightweight, non-terminating assertion mechanism and runs tests from main.
// The tests rely on the actual project headers to define opt_state_t, edge, and propedom.
// Extern "C" is used to ensure correct linkage for the C function under test.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <gencode.h>
#include <cstddef>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <pcap-types.h>


// Step 1 & 2: Include project headers to obtain real type definitions and the function prototype.
// We assume optimize.h provides the exact definitions of opt_state_t, struct edge, and propedom.
// Use extern "C" to prevent name mangling when calling a C function from C++.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) \
                  << " | expected: " << (b) << " actual: " << (a) << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

// Helper function to safely allocate a new edge with zeroed content.
// We rely on the actual project's edge structure being compatible with
// simple zero-initialization and member access in tests.
static edge* new_edge_zeroed() {
    edge* e = (edge*)std::calloc(1, sizeof(edge));
    return e;
}

// Test 1: When ep->succ is NULL, propedom should only insert ep->id into ep->edom.
// Expected: ep->edom contains a bit corresponding to ep->id.
static void test_propedom_no_succ_inserts_id() {
    // Initialize opt_state_t
    opt_state_t opt;
    std::memset(&opt, 0, sizeof(opt_state_t));
    // If edgewords is part of opt_state_t, it should be initialized by zeroing; 
    // the test expects that the operation uses edgewords only in the intersection path.

    // Create edge ep with id = 5 and ensure edom starts empty
    edge ep;
    std::memset(&ep, 0, sizeof(edge));
    ep.id = 5;

    // ep.succ is NULL, per test scenario
    ep.succ = NULL;

    // Call the function under test
    propedom(&opt, &ep);

    // Validate: ep.edom should have the bit corresponding to id set.
    // We assume ep.edom is an unsigned integer type suitable for bitwise operations.
    // The expression (1U << ep.id) should be present in ep.edom after the call.
    unsigned int expected_mask = (1U << (unsigned int)ep.id);
    unsigned int actual = ep.edom;
    EXPECT_EQ(actual, expected_mask, "ep.edom should contain bit for ep->id after propedom (no succ).");
}

// Test 2: When ep->succ is non-NULL, propedom should intersect ep->edom with succ->et.edom
// and succ->ef.edom using opt_state->edgewords. We verify that the intersections
// reflect the expected result given a controlled initial state.
static void test_propedom_with_succ_intersection() {
    // Initialize opt_state_t with a non-zero edgewords mask to exercise intersection.
    opt_state_t opt;
    std::memset(&opt, 0, sizeof(opt_state_t));
    // Assuming edgewords is a bitmask field inside opt_state_t
    opt.edgewords = 0x0F; // allow the lowest 4 bits to pass through the intersection

    // Create ep with a known edom (e.g., bit 3 set)
    edge ep;
    std::memset(&ep, 0, sizeof(edge));
    ep.id = 3;
    ep.edom = 0x08; // bit 3 set

    // Create succ edge with initial et.edom and ef.edom both zero
    edge *succ = new_edge_zeroed();
    // succ's et and ef are members; initialize them to zero
    succ->et.edom = 0x00;
    succ->ef.edom = 0x00;

    ep.succ = succ;

    // Call the function under test
    propedom(&opt, &ep);

    // Expected: succ->et.edom and succ->ef.edom should be ep.edom AND opt.edgewords
    unsigned int expected = ep.edom & opt.edgewords; // 0x08 & 0x0F == 0x08
    EXPECT_EQ(succ->et.edom, expected, "succ->et.edom should be intersection of ep->edom and edgewords.");
    EXPECT_EQ(succ->ef.edom, expected, "succ->ef.edom should be intersection of ep->edom and edgewords.");

    // Cleanup
    std::free(succ);
}

// Entry point for running tests
int main() {
    // Step 3: Run tests
    test_propedom_no_succ_inserts_id();
    test_propedom_with_succ_intersection();

    // Summary
    std::cout << "Propedom test suite finished. "
              << "Ran: " << g_tests_run
              << ", Failed: " << g_tests_failed
              << ".\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}