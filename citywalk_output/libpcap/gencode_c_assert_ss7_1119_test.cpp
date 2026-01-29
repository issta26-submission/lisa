// C++ test suite for the focal method assert_ss7 in gencode.c
// This test suite is designed to be compiled along with gencode.c and uses
// a lightweight, non-terminating assertion mechanism (no GTest) as requested.
// It overrides the C function bpf_error to observe call behavior.

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
#include <string>
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


// Domain knowledge hints (mapped to Candidate Keywords):
// - Dependencies: compiler_state_t, fields linktype, off_sio, off_opc, off_dpc, off_sls
// - SS7-specific linktypes: DLT_MTP2, DLT_ERF, DLT_MTP2_WITH_PHDR
// - OFFSET_NOT_SET sentinel used to determine if offsets are set
// - bpf_error function is used to report errors
// - Keyword kw passed to assert_ss7 is interpolated into the error message
// These elements guide our test coverage:
static bool g_error_seen = false;
static std::string g_last_fmt;

// Variadic C linkage override: capture whenever bpf_error is invoked by the code under test.
// We intentionally ignore the variadic arguments (as tests focus on invocation and formatting).
extern "C" void bpf_error(compiler_state_t *cstate, const char *fmt, ...) {
    (void)cstate;      // unused in tests, but kept to mirror real signature
    g_error_seen = true;
    if (fmt) {
        g_last_fmt = fmt;
    } else {
        g_last_fmt.clear();
    }
}

// Helper: reset error state before each test
static void reset_error_state() {
    g_error_seen = false;
    g_last_fmt.clear();
}

// Helper: simple non-terminating assertion logging
static int tests_run = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) << " (line " << __LINE__ << ")\n"; \
        ++tests_failed; \
    } \
} while (0)

// Test 1: For a non-SS7 linktype (not MTP2/ERF/MTP2_WITH_PHDR), assert_ss7 should
// always call bpf_error and include the kw in the message.
static void test_assert_ss7_non_ss7_linktype_errors() {
    reset_error_state();
    compiler_state_t cs;

    // Choose a linktype not among SS7-specific cases.
    // Use a concrete integer that is unlikely to match the switch cases.
    cs.linktype = 9999;

    // Offsets can be in any state; they should not affect the behavior here.
    cs.off_sio = OFFSET_NOT_SET;
    cs.off_opc = OFFSET_NOT_SET;
    cs.off_dpc = OFFSET_NOT_SET;
    cs.off_sls = OFFSET_NOT_SET;

    const char *kw = "non-ss7-test-keyword";

    // Exercise
    assert_ss7(&cs, kw);

    // Verify: an error must have been reported
    TEST_ASSERT(g_error_seen, "bpf_error should be invoked for non-SS7 linktypes");

    // Verify: the keyword should appear in the formatted message
    TEST_ASSERT(!g_last_fmt.empty(), "bpf_error should receive a format string");
    TEST_ASSERT(std::string(g_last_fmt).find(kw) != std::string::npos,
                "bpf_error format should include the keyword");
}

// Test 2: For an SS7 linktype with all four offsets set, assert_ss7 should return
// without calling bpf_error (i.e., no error is emitted).
static void test_assert_ss7_ss7_all_offsets_set_no_error() {
    reset_error_state();
    compiler_state_t cs;

    // SS7 linktype case
    cs.linktype = DLT_MTP2;

    // All offsets set (not OFFSET_NOT_SET)
    cs.off_sio = 1;
    cs.off_opc = 2;
    cs.off_dpc = 3;
    cs.off_sls = 4;

    const char *kw = "ss7-offsets-all-set";

    // Exercise
    assert_ss7(&cs, kw);

    // Verify: no error must have been reported
    TEST_ASSERT(!g_error_seen, "No error should be reported when all SS7 offsets are set");
    // Also verify that the keyword was not forced into an error path
    // (since no error path is taken, last_fmt should remain empty or unchanged)
}

// Test 3: For an SS7 linktype with not all offsets set, assert_ss7 should emit an error.
// This covers the true-branch condition that prevents the early return.
static void test_assert_ss7_ss7_incomplete_offsets_error() {
    reset_error_state();
    compiler_state_t cs;

    // SS7 linktype
    cs.linktype = DLT_MTP2_WITH_PHDR;

    // Not all offsets set: only off_sio is set, others remain OFFSET_NOT_SET
    cs.off_sio = 1;
    cs.off_opc = OFFSET_NOT_SET;
    cs.off_dpc = OFFSET_NOT_SET;
    cs.off_sls = OFFSET_NOT_SET;

    const char *kw = "ss7-incomplete-offsets";

    // Exercise
    assert_ss7(&cs, kw);

    // Verify: an error must have been reported
    TEST_ASSERT(g_error_seen, "bpf_error should be invoked when not all SS7 offsets are set");

    // Verify: keyword present in error format
    TEST_ASSERT(!g_last_fmt.empty(), "bpf_error should receive a format string");
    TEST_ASSERT(std::string(g_last_fmt).find(kw) != std::string::npos,
                "bpf_error format should include the keyword");
}

// Main runner
int main() {
    // Run tests
    test_assert_ss7_non_ss7_linktype_errors();
    test_assert_ss7_ss7_all_offsets_set_no_error();
    test_assert_ss7_ss7_incomplete_offsets_error();

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";

    // Return non-zero if any test failed
    return tests_failed ? 1 : 0;
}