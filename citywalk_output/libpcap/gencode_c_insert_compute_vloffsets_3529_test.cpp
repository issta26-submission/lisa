// Test suite for insert_compute_vloffsets in gencode.c
// Purpose: provide a set of C++11-based unit tests without GTest.
// The tests exercise key conditional branches and state mutations
// in the focal method, using a minimal harness with non-terminating
// assertions to maximize coverage.
//
// Note: This harness assumes the project provides the C declarations
// in gencode.h (and related headers) with C linkage. We invoke
// the focal function exactly as it would be in the production code.

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


// Include the project's C headers under C linkage.
extern "C" {
}

// Lightweight non-terminating assertion macro
// Reports failures but allows test to continue.
#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "[TEST_FAIL] " << __FUNCTION__ \
                      << " - Expected true: " #cond << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_FALSE(cond) \
    do { \
        if((cond)) { \
            std::cerr << "[TEST_FAIL] " << __FUNCTION__ \
                      << " - Expected false: " #cond << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_NOT Equal  // Reserved to avoid accidental misuse in this harness

// Global counters for test results
static int g_total_tests = 0;
static int g_test_failures = 0;

// Helper to reset a compiler_state_t structure to a clean state
static void reset_compiler_state(compiler_state_t *cstate) {
    if (!cstate) return;
    // Best-effort reset of the structure; zeroed memory is a reasonable
    // default for test scenarios where fields will be explicitly set.
    std::memset(cstate, 0, sizeof(*cstate));
}

// Helper to reset a block structure
static void reset_block(struct block *b) {
    if (!b) return;
    std::memset(b, 0, sizeof(*b));
}

// Test 1: True-branch of the first conditional should allocate a register
// for the link header when a payload offset is already allocated and
// the link header has a variable portion but no register yet.
static void test_branch_allocates_off_linkhdr_reg() {
    // Setup: off_linkpl.reg != -1 (payload offset present)
    //         off_linkhdr.is_variable == true
    //         off_linkhdr.reg == -1 (not yet allocated)
    // Outer type can be any (we focus on the first if)
    compiler_state_t cstate;
    struct block b;

    reset_compiler_state(&cstate);
    reset_block(&b);

    // Initialize the specific fields used by the first if
    cstate.off_linkpl.reg = 5;
    cstate.off_linkhdr.is_variable = 1;  // true
    cstate.off_linkhdr.reg = -1;          // not allocated yet
    cstate.outermostlinktype = DLT_IEEE802_11; // arbitrary non-default
    cstate.is_vlan_vloffset = false;         // VLAN not involved for this test

    // Execute the focal method
    insert_compute_vloffsets(&cstate, &b);

    // Verify: reg should have been allocated (i.e., not -1)
    EXPECT_TRUE(cstate.off_linkhdr.reg != -1);

    ++g_total_tests;
}

// Test 2: VLAN-related offsets should cause allocation of both required registers
// when VLAN offset handling is enabled and no registers exist yet.
static void test_vlan_offsets_allocate_both_regs() {
    compiler_state_t cstate;
    struct block b;

    reset_compiler_state(&cstate);
    reset_block(&b);

    // VLAN-related mode ON
    cstate.is_vlan_vloffset = true;
    cstate.outermostlinktype = DLT_UNKNOWN; // ensure VLAN path runs
    // Start with no registers allocated
    cstate.off_linkpl.reg = -1;
    cstate.off_linktype.reg = -1;

    // Execute
    insert_compute_vloffsets(&cstate, &b);

    // Verify: both registers should be allocated as a result of VLAN path
    EXPECT_TRUE(cstate.off_linkpl.reg != -1);
    EXPECT_TRUE(cstate.off_linktype.reg != -1);

    ++g_total_tests;
}

// Test 3: VLAN path should allocate only the missing register when one already exists.
// Specifically, off_linkpl.reg pre-allocated, off_linktype.reg not allocated yet.
static void test_vlan_offsets_allocate_only_missing_type_reg() {
    compiler_state_t cstate;
    struct block b;

    reset_compiler_state(&cstate);
    reset_block(&b);

    cstate.is_vlan_vloffset = true;
    cstate.outermostlinktype = DLT_UNKNOWN;
    // Pre-allocate payload offset reg, simulate partial initialization
    cstate.off_linkpl.reg = 7;
    cstate.off_linktype.reg = -1;

    insert_compute_vloffsets(&cstate, &b);

    // Verify: off_linktype.reg should now be allocated
    EXPECT_TRUE(cstate.off_linktype.reg != -1);
    // off_linkpl.reg should remain as allocated (non -1)
    EXPECT_TRUE(cstate.off_linkpl.reg != -1);

    ++g_total_tests;
}

// Test 4: When VLAN_offset flag is false and conditionally dependent fields are unset,
// no extra allocations should occur beyond expected defaults.
static void test_no_allocation_when_no_vlan_and_no_prior_offsets() {
    compiler_state_t cstate;
    struct block b;

    reset_compiler_state(&cstate);
    reset_block(&b);

    // No VLAN offsets requested
    cstate.is_vlan_vloffset = false;
    // Ensure no pre-allocated payload or header registers
    cstate.off_linkpl.reg = -1;
    cstate.off_linkhdr.reg = -1;
    cstate.off_linktype.reg = -1;
    cstate.outermostlinktype = DLT_UNKNOWN;

    insert_compute_vloffsets(&cstate, &b);

    // Verify: none should be allocated
    EXPECT_TRUE(cstate.off_linkpl.reg == -1);
    EXPECT_TRUE(cstate.off_linkhdr.reg == -1 || cstate.off_linkhdr.reg == -1); // hdr may remain -1
    EXPECT_TRUE(cstate.off_linktype.reg == -1);

    ++g_total_tests;
}

// Entry point for running tests
int main() {
    // Run all tests
    test_branch_allocates_off_linkhdr_reg();
    test_vlan_offsets_allocate_both_regs();
    test_vlan_offsets_allocate_only_missing_type_reg();
    test_no_allocation_when_no_vlan_and_no_prior_offsets();

    // Summary
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_test_failures
              << std::endl;

    // Return non-zero if any test failed
    return (g_test_failures != 0) ? 1 : 0;
}