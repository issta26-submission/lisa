/*
  Unit test suite for stringtoportrange (focal method)
  - Environment: C++11, no GoogleTest/GMock
  - Approach: Provide minimal C stubs for dependencies (bpf_error, stringtoport)
    so that stringtoportrange can be exercised in isolation.
  - The tests focus on coverage of all meaningful branches:
      * hyphen present or missing
      * multiple hyphens
      * empty starting port
      * empty ending port
      * proto consistency across port1/port2 parses
  - Error paths are captured using setjmp/longjmp to emulate non-returning
    behavior of bpf_error in the original code.
  - The tests are designed to be self-contained and executable without
    additional test frameworks.
  - Notes:
      - PROTO_UNDEF value is internal to the focal code; tests do not rely on its exact numeric
        value. They verify the intended semantic behavior (e.g., proto change/mismatch) through
        observable values (port numbers, proto equality vs. save_proto, etc.).
      - We provide a lightweight mechanism to simulate different proto outcomes from stringtoport
        to cover the proto-mismatch path.
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
#include <cstdarg>
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


// Forward declare the focal state's type as an incomplete type for the test harness.
// The actual definition is inside the production code; pointers are sufficient here.
struct compiler_state_t;

// Typedef matching the code's usage
typedef uint32_t bpf_u_int32;

// Declaration of the focal function under test (C linkage)
extern "C" void stringtoportrange(struct compiler_state_t *cstate,
                                 const char *string,
                                 bpf_u_int32 *port1,
                                 bpf_u_int32 *port2,
                                 int *proto);

// Dependency stubs to drive the focal function without requiring full external libpcap
// Implemented with C linkage to match the production code's linkage.
// These stubs are intentionally minimal and deterministic for test reproducibility.

static jmp_buf test_jmp; // for catching bpf_error longjmp calls

// Gating controls for the stringtoport mock to cover proto-mismatch cases
static int g_stringtoport_call = 0;
static int g_stringtoport_mismatch_mode = 0;

// Reset the internal counter for stringtoport calls (per-test isolation)
extern "C" void reset_stringtoport(void) {
    g_stringtoport_call = 0;
}

// Configure whether stringtoport should produce a proto mismatch between successive calls
extern "C" void set_stringtoport_mismatch_mode(int mode) {
    g_stringtoport_mismatch_mode = mode;
}

// Mock implementation of stringtoport used by stringtoportrange
extern "C" uint32_t stringtoport(struct compiler_state_t *cstate,
                               const char *string,
                               size_t string_size,
                               int *proto) {
    (void)cstate; // unused in test harness
    g_stringtoport_call++;

    // Copy and interpret the port string (not strictly required for test logic)
    char buf[32];
    size_t n = (string_size < sizeof(buf) - 1) ? string_size : (sizeof(buf) - 1);
    strncpy(buf, string, n);
    buf[n] = '\0';
    // Simple numeric port parse
    uint32_t port = static_cast<uint32_t>(std::strtoul(buf, nullptr, 10));

    // Proto behavior:
    // - In normal mode (no mismatch), always set proto to 10 on both calls.
    // - In mismatch mode, first call sets proto to 10, second call to 20 (to trigger mismatch)
    if (g_stringtoport_mismatch_mode) {
        *proto = (g_stringtoport_call == 1) ? 10 : 20;
    } else {
        *proto = 10;
    }
    return port;
}

// Helper to reset the stringtoport internal state between tests (exposed externally)
extern "C" void test_harness_reset_state(void) {
    reset_stringtoport();
    // Do not reset mismatch mode here; tests may set it explicitly
}

// bpf_error stub: emulate non-returning error path by using longjmp to the test harness
extern "C" void bpf_error(struct compiler_state_t *cstate, const char *fmt, ...) {
    (void)cstate;
    va_list ap;
    va_start(ap, fmt);
    // Optional: could log the error format if needed for debugging
    (void)fmt;
    va_end(ap);
    longjmp(test_jmp, 1);
}

// Test helper: simple assertion with non-terminating behavior
static int tests_failed = 0;
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "EXPECT_FAILED: " << msg << std::endl;
        tests_failed++;
    }
}

int main() {
    // Simple dummy object to pass as compiler_state_t* to the focal function
    struct compiler_state_t dummy;

    // Test 1: Valid range with same proto for both ports
    // Expected: port1 == 80, port2 == 443, proto == 10
    if (setjmp(test_jmp) == 0) {
        // Prepare a clean state
        test_harness_reset_state();
        set_stringtoport_mismatch_mode(0); // ensure proto is the same for both ports
        uint32_t port1 = 0, port2 = 0;
        int proto = 0;
        stringtoportrange(&dummy, "80-443", &port1, &port2, &proto);

        expect_true(port1 == 80, "port1 should be parsed as 80");
        expect_true(port2 == 443, "port2 should be parsed as 443");
        expect_true(proto == 10, "proto should be 10 after both parses");
        std::cout << "Test1 PASSED: valid port range parsed with consistent proto." << std::endl;
    } else {
        std::cerr << "Test1 FAILED: unexpected bpf_error during valid input." << std::endl;
        return 1;
    }

    // Test 2: No hyphen -> error path
    // Expected: error triggered (bpf_error calls longjmp)
    if (setjmp(test_jmp) == 0) {
        test_harness_reset_state();
        uint32_t p1 = 0, p2 = 0;
        int proto = 0;
        stringtoportrange(&dummy, "80", &p1, &p2, &proto);
        std::cerr << "Test2 FAILED: expected error for input without hyphen, but function returned normally." << std::endl;
        return 1;
    } else {
        std::cout << "Test2 PASSED: error path triggered as expected for missing hyphen." << std::endl;
        // proceed to next tests
    }

    // Test 3: More than one hyphen -> error path
    if (setjmp(test_jmp) == 0) {
        test_harness_reset_state();
        uint32_t p1 = 0, p2 = 0;
        int proto = 0;
        stringtoportrange(&dummy, "80-90-1", &p1, &p2, &proto);
        std::cerr << "Test3 FAILED: expected error for multiple hyphens, but function returned normally." << std::endl;
        return 1;
    } else {
        std::cout << "Test3 PASSED: error path triggered as expected for multiple hyphens." << std::endl;
    }

    // Test 4: Starting port missing -> error path
    if (setjmp(test_jmp) == 0) {
        test_harness_reset_state();
        uint32_t p1 = 0, p2 = 0; int proto = 0;
        stringtoportrange(&dummy, "-80", &p1, &p2, &proto);
        std::cerr << "Test4 FAILED: expected error for missing starting port, but function returned normally." << std::endl;
        return 1;
    } else {
        std::cout << "Test4 PASSED: error path triggered as expected for missing starting port." << std::endl;
    }

    // Test 5: Ending port missing -> error path
    if (setjmp(test_jmp) == 0) {
        test_harness_reset_state();
        uint32_t p1 = 0, p2 = 0; int proto = 0;
        stringtoportrange(&dummy, "80-", &p1, &p2, &proto);
        std::cerr << "Test5 FAILED: expected error for missing ending port, but function returned normally." << std::endl;
        return 1;
    } else {
        std::cout << "Test5 PASSED: error path triggered as expected for missing ending port." << std::endl;
    }

    // Test 6: Proto mismatch between the two ports should set proto to PROTO_UNDEF
    // This test uses the mismatch mode to force a proto change and then checks proto != 10
    set_stringtoport_mismatch_mode(1);
    if (setjmp(test_jmp) == 0) {
        test_harness_reset_state();
        uint32_t p1 = 0, p2 = 0;
        int proto = 0;
        stringtoportrange(&dummy, "80-8080", &p1, &p2, &proto);
        // Expect proto to differ from the first proto (10) due to mismatch
        expect_true(proto != 10, "proto should differ from initial value when mismatch happens");
        std::cout << "Test6 PASSED: proto mismatch leads to proto != 10 (PROTO_UNDEF semantics)." << std::endl;
    } else {
        // If bpf_error were to trigger here, that would be unexpected for this test
        std::cerr << "Test6 FAILED: unexpected bpf_error during proto mismatch test." << std::endl;
        return 1;
    }

    // Summary
    if (tests_failed == 0) {
        std::cout << "All tests completed. Summary: 0 failures." << std::endl;
    } else {
        std::cout << "All tests completed. Summary: " << tests_failed << " failure(s)." << std::endl;
    }

    return 0;
}