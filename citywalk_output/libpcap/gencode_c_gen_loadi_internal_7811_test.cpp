// C++11 test suite for the focal method gen_loadi_internal in gencode.c
// This test harness uses a lightweight, non-GTest style approach suitable for
// integration in environments where Google Test is not allowed.
// It relies on the project headers and implementations (gencode.h and the
// corresponding C sources) to provide the actual structures and functions
// (compiler_state_t, arth, slist, newchunk, alloc_reg, new_stmt, etc.).
//
// Test goals:
// - Validate that gen_loadi_internal correctly allocates an arth object.
// - Validate that the first statement carries the immediate value (val) in k.
// - Validate that the second statement carries the allocated register in k.
// - Validate that the returned arth's regno matches the register used in the
//   second statement.
// - Exercise multiple input values to increase coverage.

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


// Include the project's C headers with C linkage to ensure correct symbol wiring.
extern "C" {
}

// Helper macro-like functions for simple reporting
static void report_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}
static void report_fail(const char* test_name, const char* reason) {
    std::cerr << "[FAIL] " << test_name << " - " << reason << std::endl;
}

// Test 1: Basic sanity check for gen_loadi_internal with a typical value
// - Ensure a is non-null.
// - Ensure a->s is non-null and a->s->s.k equals the input val.
// - Ensure a->s->next is non-null and a->s->next->s.k equals a->regno.
// - Ensure the function allocates a register and stores it in a->regno.
static bool test_gen_loadi_internal_basic() {
    const char* test_name = "test_gen_loadi_internal_basic";

    // Prepare compiler state
    compiler_state_t *cstate = (compiler_state_t*)calloc(1, sizeof(compiler_state_t));
    if (!cstate) {
        report_fail(test_name, "failed to allocate compiler_state_t");
        return false;
    }
    initchunks(cstate);

    // Test input
    const bpf_u_int32 test_val = 12345u; // arbitrary non-zero value

    // Call the function under test
    struct arth *a = gen_loadi_internal(cstate, test_val);

    // Validate results
    bool ok = true;
    if (!a) {
        report_fail(test_name, "gen_loadi_internal returned NULL arth");
        ok = false;
    } else {
        // a should contain a non-null s-list
        if (!a->s) {
            report_fail(test_name, "a->s is NULL");
            ok = false;
        } else {
            // First statement's k should be equal to test_val
            if (a->s->s.k != test_val) {
                report_fail(test_name, "first statement k does not equal input value val");
                ok = false;
            }
            // Second statement should exist and carry the register in k
            if (!a->s->next) {
                report_fail(test_name, "second statement (a->s->next) is NULL");
                ok = false;
            } else {
                const int reg_from_second = a->s->next->s.k;
                if (reg_from_second != a->regno) {
                    report_fail(test_name, "reg number in second statement does not match a->regno");
                    ok = false;
                }
            }
        }
    }

    // Cleanup
    freechunks(cstate);
    free(cstate);

    if (ok) report_pass(test_name);
    return ok;
}

// Test 2: Verify behavior with another value (boundary and distinct value)
static bool test_gen_loadi_internal_other_value() {
    const char* test_name = "test_gen_loadi_internal_other_value";

    compiler_state_t *cstate = (compiler_state_t*)calloc(1, sizeof(compiler_state_t));
    if (!cstate) {
        report_fail(test_name, "failed to allocate compiler_state_t");
        return false;
    }
    initchunks(cstate);

    const bpf_u_int32 test_val = 0xFFFFFFFFu; // max 32-bit value to exercise edge case

    struct arth *a = gen_loadi_internal(cstate, test_val);

    bool ok = true;
    if (!a) {
        report_fail(test_name, "gen_loadi_internal returned NULL arth");
        ok = false;
    } else {
        if (!a->s) {
            report_fail(test_name, "a->s is NULL");
            ok = false;
        } else {
            if (a->s->s.k != test_val) {
                report_fail(test_name, "first statement k does not equal input val (0xFFFFFFFF)");
                ok = false;
            }
            if (!a->s->next) {
                report_fail(test_name, "second statement (a->s->next) is NULL");
                ok = false;
            } else {
                const int reg_from_second = a->s->next->s.k;
                if (reg_from_second != a->regno) {
                    report_fail(test_name, "reg number in second statement does not match a->regno (edge case)");
                    ok = false;
                }
            }
        }
    }

    freechunks(cstate);
    free(cstate);

    if (ok) report_pass(test_name);
    return ok;
}

// Test 3: Multiple allocations check (ensure successive calls allocate distinct regs)
// This test ensures state resets after a new cstate per test, not across tests.
// It mainly validates that the API remains stable across repeated calls.
static bool test_gen_loadi_internal_multiple_calls_distinct_regs() {
    const char* test_name = "test_gen_loadi_internal_multiple_calls_distinct_regs";

    compiler_state_t *cstate = (compiler_state_t*)calloc(1, sizeof(compiler_state_t));
    if (!cstate) {
        report_fail(test_name, "failed to allocate compiler_state_t");
        return false;
    }
    initchunks(cstate);

    const int NUM_CALLS = 3;
    bool ok = true;
    bpf_u_int32 vals[NUM_CALLS] = { 1, 2, 3 };
    int reg_seen[NUM_CALLS];

    struct arth *as[NUM_CALLS];

    for (int i = 0; i < NUM_CALLS; ++i) {
        as[i] = gen_loadi_internal(cstate, vals[i]);
        if (!as[i] || !as[i]->s) {
            report_fail(test_name, "internal call returned NULL or missing s-list");
            ok = false;
            break;
        }
        reg_seen[i] = as[i]->regno;
        // Basic sanity checks
        if (as[i]->s->s.k != vals[i]) {
            report_fail(test_name, "first statement k does not equal input val on iteration");
            ok = false;
            break;
        }
        if (!as[i]->s->next) {
            report_fail(test_name, "second statement (a->s->next) is NULL on iteration");
            ok = false;
            break;
        }
        if (as[i]->s->next->s.k != as[i]->regno) {
            report_fail(test_name, "reg number in second statement does not match a->regno on iteration");
            ok = false;
            break;
        }
    }

    // Ensure that regs are distinct for these 3 successive allocations
    if (ok) {
        if (reg_seen[0] == reg_seen[1] || reg_seen[1] == reg_seen[2] || reg_seen[0] == reg_seen[2]) {
            report_fail(test_name, "register allocations are not distinct across multiple calls");
            ok = false;
        } else {
            report_pass(test_name);
        }
    } else {
        // Failure already reported above; still attempt to cleanup
    }

    freechunks(cstate);
    free(cstate);

    return ok;
}

// Main test runner
int main() {
    std::cout << "Running unit tests for gen_loadi_internal (gencode.c) using C++11 test harness...\n";

    int total = 0;
    int passed = 0;

    if (test_gen_loadi_internal_basic()) { ++passed; }
    ++total;

    if (test_gen_loadi_internal_other_value()) { ++passed; }
    ++total;

    if (test_gen_loadi_internal_multiple_calls_distinct_regs()) { ++passed; }
    ++total;

    std::cout << "Test summary: " << passed << "/" << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}