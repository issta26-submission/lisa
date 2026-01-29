// Test suite for the focal method gen_geneve_offsets in gencode.c
// This test harness is written in C++11 without using any testing framework.
// It performs non-terminating checks (console reports) and returns a non-zero
// exit code if any test fails. The tests rely on the project-provided headers
// and types (e.g., compiler_state_t, slist) and therefore must be built
// together with the original project sources.

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


// Import C headers in a C++ translation unit
extern "C" {
}

// Simple non-terminating expectation helpers
static void expect_true(bool cond, const char* msg, bool &ok) {
    if (!cond) {
        std::cerr << "Expectation failed: " << msg << std::endl;
        ok = false;
    }
}

static void log_header(const char* name) {
    std::cout << "=== Running test: " << name << " ===" << std::endl;
}

// Test 1: Baseline behavior with zeroed compiler_state
// Intent:
// - Ensure gen_geneve_offsets returns a non-NULL instruction list.
// - Verify that critical internal state fields are initialized/updated as the code
//   path dictates (off_linktype and off_linkpl are marked variable and have reg allocated).
// - Verify that a clean, zeroed state leaves off_nl at 0 after generation.
static bool test_gen_geneve_offsets_baseline() {
    log_header("test_gen_geneve_offsets_baseline");

    bool ok = true;

    // Zero-initialized compiler state
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Call the focal function
    struct slist *head = gen_geneve_offsets(&cstate);

    // Basic sanity: non-NULL return
    expect_true(head != nullptr, "gen_geneve_offsets should not return NULL", ok);

    // Validate that off_linktype is marked as a variable and has a reg allocated
    // These fields are touched by gen_geneve_offsets during its setup.
    expect_true(cstate.off_linktype.is_variable == 1,
                "off_linktype should be marked as variable", ok);
    // constant_part should be zero for variable link type
    expect_true(cstate.off_linktype.constant_part == 0,
                "off_linktype.constant_part should be 0", ok);
    // a valid register should be allocated
    expect_true(cstate.off_linktype.reg >= 0,
                "off_linktype.reg should be non-negative", ok);

    // Validate that off_linkpl is marked as a variable with a valid reg
    expect_true(cstate.off_linkpl.is_variable == 1,
                "off_linkpl should be marked as variable", ok);
    expect_true(cstate.off_linkpl.constant_part == 0,
                "off_linkpl.constant_part should be 0", ok);
    expect_true(cstate.off_linkpl.reg >= 0,
                "off_linkpl.reg should be non-negative", ok);

    // After generation, off_nl should be reset to 0
    expect_true(cstate.off_nl == 0,
                "off_nl should be reset to 0 after generation", ok);

    return ok;
}

// Test 2: Behavior when an offset prefix is pre-set (non-default off_nl/constant_part)
// Intent:
// - Initialize specific fields to non-zero values to exercise a different path.
// - Ensure that the function still returns a valid instruction list and that internal
//   state is properly initialized (registration for the offsets remains functional).
static bool test_gen_geneve_offsets_with_offset() {
    log_header("test_gen_geneve_offsets_with_offset");

    bool ok = true;

    // Prepare compiler state with non-default offset prefix
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Introduce non-zero values to exercise different offset calculations
    cstate.off_linkpl.constant_part = 1;  // non-zero base
    cstate.off_nl = 5;                   // non-zero network header length placeholder

    struct slist *head = gen_geneve_offsets(&cstate);

    // Basic sanity: non-NULL return
    expect_true(head != nullptr, "gen_geneve_offsets should not return NULL with non-zero offsets", ok);

    // Validate that off_linktype and off_linkpl are still initialized as in baseline
    expect_true(cstate.off_linktype.is_variable == 1,
                "off_linktype should be marked as variable when offsets pre-set", ok);
    expect_true(cstate.off_linktype.constant_part == 0,
                "off_linktype.constant_part should still be 0", ok);
    expect_true(cstate.off_linktype.reg >= 0,
                "off_linktype.reg should be non-negative when offsets pre-set", ok);

    expect_true(cstate.off_linkpl.is_variable == 1,
                "off_linkpl should be marked as variable when offsets pre-set", ok);
    expect_true(cstate.off_linkpl.constant_part == 0,
                "off_linkpl.constant_part should be 0 (reg driven)", ok);
    expect_true(cstate.off_linkpl.reg >= 0,
                "off_linkpl.reg should be non-negative when offsets pre-set", ok);

    // off_nl should be reset to 0 after generation regardless of input
    expect_true(cstate.off_nl == 0,
                "off_nl should be reset to 0 after generation (even with pre-set offsets)", ok);

    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    // Run tests
    if (test_gen_geneve_offsets_baseline()) {
        std::cout << "[PASS] test_gen_geneve_offsets_baseline" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] test_gen_geneve_offsets_baseline" << std::endl;
    }
    ++total;

    if (test_gen_geneve_offsets_with_offset()) {
        std::cout << "[PASS] test_gen_geneve_offsets_with_offset" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] test_gen_geneve_offsets_with_offset" << std::endl;
    }
    ++total;

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Non-terminating: return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}