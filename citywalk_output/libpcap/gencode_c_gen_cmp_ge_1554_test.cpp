// Test suite for the focal method gen_cmp_ge in gencode.c
// This test uses a linker-wrapped interception to replace the underlying
// gen_ncmp call with a test probe, allowing verification of the behavior of
// gen_cmp_ge without requiring full internal state setup.
//
// Build notes (to be executed by the user, not part of the code):
// - Compile gencode.c into a library (e.g., libgencode.a)
// - Build this test file with a linker wrap so that all calls to gen_ncmp are
//   redirected to the test wrapper. Example (Linux with GCC/LD):
//     g++ -O2 -std=c++11 -c test_gen_cmp_ge.cpp -o test_gen_cmp_ge.o
//     ar rcs libgencode.a <object files of gencode.c>
//     g++ -O2 -std=c++11 -I/path/to/headers \
//         test_gen_cmp_ge.cpp -L. -lgencode \
//         -Wl,--wrap=gen_ncmp -o test_gen_cmp_ge
// - Run: ./test_gen_cmp_ge
//
// The approach relies on the linker wrapper to intercept the gen_ncmp call made
// by gen_cmp_ge. The wrapper records the parameters (mask, jtype, reverse, v)
// passed to gen_ncmp and returns a fixed sentinel value so that we can verify
// that gen_cmp_ge propagates the wrapper's return value correctly.

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


// Bring in the production declarations. This provides the exact types used by
// gen_cmp_ge and the underlying gen_ncmp implementation.
extern "C" {
}

// Global capture variables updated by the wrapper intercepting gen_ncmp.
// These allow the tests to validate that gen_cmp_ge forwards the right values
// to gen_ncmp (via the wrapper).
static unsigned int g_last_mask = 0;
static int g_last_jtype = -1;
static int g_last_reverse = -1;
static unsigned int g_last_v = 0;

// Wrapper for gen_ncmp selected by the linker wrap mechanism.
// The wrapper records the input arguments and returns a sentinel value.
// Note: We do not call the real gen_ncmp here to avoid requiring a valid
// compiler state (cstate) initialization for tests.
extern "C" int __wrap_gen_ncmp(compiler_state_t *cstate,
                              enum e_offrel offrel,
                              unsigned int offset,
                              unsigned int size,
                              unsigned int mask,
                              int jtype,
                              int reverse,
                              unsigned int v)
{
    // Capture the parameters to verify later in tests.
    (void)offrel;   // unused in capture, but kept to reflect actual signature
    (void)offset;
    (void)size;
    g_last_mask   = mask;
    g_last_jtype  = jtype;
    g_last_reverse= reverse;
    g_last_v      = v;

    // Return a sentinel value to indicate that the wrapper was invoked.
    // This allows us to verify that gen_cmp_ge forwards the return value
    // from gen_ncmp (via the wrapper) to its caller.
    return 0xdeadbeef;
}

// Helper to reset captured values before each test
static void reset_capture() {
    g_last_mask    = 0;
    g_last_jtype   = -1;
    g_last_reverse = -1;
    g_last_v       = 0;
}

// Test 1: Basic interception validation
// - Purpose: Ensure that gen_cmp_ge calls gen_ncmp with the expected fixed
//   parameters: mask = 0xffffffff, reverse = 0, and forwards v.
// - Rationale: If gen_cmp_ge delegates to gen_ncmp with these exact constants,
//   the wrapper should capture those values. The test also asserts that the
//   return value from gen_cmp_ge equals the sentinel from the wrapper.
static bool test_gen_cmp_ge_basic_intercept() {
    reset_capture();

    // Use a dummy cstate pointer (tests do not require a real compiler state)
    compiler_state_t *dummy = NULL;
    // Use an arbitrary but valid-looking offrel
    enum e_offrel offrel = (enum e_offrel)0;
    unsigned int offset = 5;
    unsigned int size = 16;
    unsigned int v = 0x12345678u;

    int ret = gen_cmp_ge(dummy, offrel, offset, size, v);

    // The wrapper returns 0xdeadbeef, so gen_cmp_ge should propagate that value.
    if (ret != 0xdeadbeef) {
        std::cerr << "[Test 1] Expected sentinel return (0xdeadbeef), got " << std::hex << ret << std::dec << "\n";
        return false;
    }

    // Validate captured parameters
    if (g_last_mask != 0xffffffffu) {
        std::cerr << "[Test 1] Expected mask 0xffffffff, got " << std::hex << g_last_mask << std::dec << "\n";
        return false;
    }
    // jtype should correspond to a non-zero value (exact macro value is not asserted here
    // to avoid depending on a particular BPF macro value in the test).
    if (g_last_jtype == 0) {
        std::cerr << "[Test 1] Expected non-zero jtype, got 0\n";
        return false;
    }
    if (g_last_reverse != 0) {
        std::cerr << "[Test 1] Expected reverse 0, got " << g_last_reverse << "\n";
        return false;
    }
    if (g_last_v != v) {
        std::cerr << "[Test 1] Expected v " << std::hex << v << ", got " << g_last_v << std::dec << "\n";
        return false;
    }

    return true;
}

// Test 2: Parameter propagation with multiple invocations
// - Purpose: Ensure that subsequent calls update the wrapper-captured values,
//   and that gen_cmp_ge consistently forwards the latest v value.
static bool test_gen_cmp_ge_param_update() {
    reset_capture();

    compiler_state_t *dummy = NULL;
    enum e_offrel offrel = (enum e_offrel)0;

    // First invocation with v = 0x1
    int ret1 = gen_cmp_ge(dummy, offrel, 0, 2, 0x1);
    if (ret1 != 0xdeadbeef) {
        std::cerr << "[Test 2] First invocation: unexpected return " << std::hex << ret1 << std::dec << "\n";
        return false;
    }
    if (g_last_v != 0x1) {
        std::cerr << "[Test 2] First invocation: expected v 0x1, got " << g_last_v << "\n";
        return false;
    }

    // Second invocation with a different v
    int ret2 = gen_cmp_ge(dummy, offrel, 0, 2, 0x2);
    if (ret2 != 0xdeadbeef) {
        std::cerr << "[Test 2] Second invocation: unexpected return " << std::hex << ret2 << std::dec << "\n";
        return false;
    }
    if (g_last_v != 0x2) {
        std::cerr << "[Test 2] Second invocation: expected v 0x2, got " << g_last_v << "\n";
        return false;
    }

    return true;
}

// Main test runner
int main() {
    bool ok = true;
    int failures = 0;

    // Run Test 1
    if (test_gen_cmp_ge_basic_intercept()) {
        std::cout << "[OK] Test 1: Basic intercept validation passed.\n";
    } else {
        std::cerr << "[FAILED] Test 1: Basic intercept validation failed.\n";
        failures++;
    }

    // Run Test 2
    if (test_gen_cmp_ge_param_update()) {
        std::cout << "[OK] Test 2: Parameter propagation across calls passed.\n";
    } else {
        std::cerr << "[FAILED] Test 2: Parameter propagation across calls failed.\n";
        failures++;
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
}