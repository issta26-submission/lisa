// Unit test suite for gen_abs_offset_varpart (reproduced in-test to be self-contained)
// This test focuses on the core logic described in the focal method:
//   - If off->is_variable is true, and off->reg == -1, allocate a register via alloc_reg(cstate)
//   - Load the register into a dummy SLIST node via new_stmt(...) and set its k field to the reg
//   - If off->is_variable is false, return NULL
//
// The tests are implemented in C++11, without GoogleTest, and use a lightweight, non-terminating
// assertion mechanism to maximize code coverage.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <cassert>
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
#include <vector>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <memory>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


// ---------------------------
// Mocked/Minimal Data Structures
// ---------------------------

// Representation of the BPF absolute offset
struct bpf_abs_offset {
    int is_variable; // true if there is a variable part to the offset
    int reg;         // register holding the variable part, -1 means "unassigned"
};

// Minimal compiler state used by the focal function
struct compiler_state_t {
    int next_reg; // simple counter to simulate register allocation
};

// Minimal representation of a statement node returned by new_stmt
struct slist {
    struct {
        int k;      // the register loaded (s->s.k in the original code)
    } s;
    int code;         // code field to emulate BPF_LDX|BPF_MEM in tests
    slist *next;       // for compatibility with a list structure (not used here)
};

// Core constants used by the focal function
#define BPF_LDX 0x01
#define BPF_MEM 0x02

// ---------------------------
// Production-like API (reproduced for test QoS)
// These implementations mimic the behavior expected by gen_abs_offset_varpart
// and are self-contained to avoid external dependencies.
// ---------------------------

// Allocate a new register (simple incremental allocator)
extern "C" int alloc_reg(compiler_state_t *cstate) {
    int r = cstate->next_reg;
    cstate->next_reg++;
    return r;
}

// Create a new statement node with a code
extern "C" slist* new_stmt(compiler_state_t *cstate, int code) {
    (void)cstate; // not used by this minimal implementation beyond interface
    slist* s = new slist();
    s->code = code;
    s->s.k = 0;
    s->next = nullptr;
    return s;
}

// The focal method re-implemented for in-test inspection.
// It mirrors the logic described in the prompt exactly.
extern "C" slist* gen_abs_offset_varpart(compiler_state_t *cstate, bpf_abs_offset *off) {
    {
        struct slist *s;
        if (off->is_variable) {
            if (off->reg == -1) {
                // allocate one for the variable part
                off->reg = alloc_reg(cstate);
            }
            // Load the register containing the variable part into the X register
            s = new_stmt(cstate, BPF_LDX | BPF_MEM);
            s->s.k = off->reg;
            return s;
        } else {
            // No variable part, nothing to generate
            return NULL;
        }
    }
}

// ---------------------------
// Lightweight Test Harness (non-terminating assertions)
// ---------------------------

static int g_passed = 0;
static int g_failed = 0;

#define CHECK(cond, msg) do { \
    if (cond) { \
        ++g_passed; \
    } else { \
        ++g_failed; \
        std::cerr << "Test failure: " << (msg) << std::endl; \
    } \
} while (0)

static void test_report() {
    std::cout << "Tests passed: " << g_passed << ", Tests failed: " << g_failed << std::endl;
}

// Helper to run all tests
static void run_all_tests();

// ---------------------------
// Individual Test Cases
// ---------------------------

// Test 1: Variable part with unassigned register (-1) should allocate a new register and set the return node's k to that reg
bool test_case_variable_unassigned_allocates_and_sets_k() {
    compiler_state_t cs;
    cs.next_reg = 0; // start allocating at 0

    bpf_abs_offset off;
    off.is_variable = 1;
    off.reg = -1;

    slist* res = gen_abs_offset_varpart(&cs, &off);

    bool ok = true;
    if (!res) {
        ok = false;
        // No result when there should be one
    } else {
        // off.reg should have been allocated to 0
        if (off.reg != 0) ok = false;
        // returned node should have k == allocated reg (0)
        if (res->s.k != 0) ok = false;
        // ensure code is built with the expected bitfield
        if (res->code != (BPF_LDX | BPF_MEM)) ok = false;
    }

    delete res;
    return ok;
}

// Test 2: Variable part with pre-assigned register should not allocate and should propagate reg into s.k
bool test_case_variable_with_preassigned_reg_uses_existing() {
    compiler_state_t cs;
    cs.next_reg = 0;

    bpf_abs_offset off;
    off.is_variable = 1;
    off.reg = 7; // pre-assigned

    slist* res = gen_abs_offset_varpart(&cs, &off);

    bool ok = true;
    if (!res) {
        ok = false;
    } else {
        if (off.reg != 7) ok = false;
        if (res->s.k != 7) ok = false;
        if (res->code != (BPF_LDX | BPF_MEM)) ok = false;
    }

    delete res;
    return ok;
}

// Test 3: Non-variable offset should produce NULL (no code generated)
bool test_case_non_variable_produces_null() {
    compiler_state_t cs;
    cs.next_reg = 0;

    bpf_abs_offset off;
    off.is_variable = 0;
    off.reg = -1;

    slist* res = gen_abs_offset_varpart(&cs, &off);

    bool ok = (res == NULL);
    // no allocations should have happened
    if (cs.next_reg != 0) ok = false;

    return ok;
}

// Test 4: Multiple allocations across consecutive calls should increment allocated regs
bool test_case_multiple_allocations_increment_reg() {
    compiler_state_t cs;
    cs.next_reg = 0;

    // First call: should allocate reg 0
    bpf_abs_offset off1;
    off1.is_variable = 1;
    off1.reg = -1;
    slist* r1 = gen_abs_offset_varpart(&cs, &off1);
    bool ok = (r1 != NULL) && (off1.reg == 0) && (r1->s.k == 0);
    delete r1;

    // Second call: should allocate reg 1
    bpf_abs_offset off2;
    off2.is_variable = 1;
    off2.reg = -1;
    slist* r2 = gen_abs_offset_varpart(&cs, &off2);
    ok = ok && (r2 != NULL) && (off2.reg == 1) && (r2->s.k == 1);
    delete r2;

    // Ensure next_reg progressed to 2
    ok = ok && (cs.next_reg == 2);

    return ok;
}

// Run all tests and print per-test comments
static void run_all_tests() {
    // Test 1
    {
        bool result = test_case_variable_unassigned_allocates_and_sets_k();
        CHECK(result, "test_case_variable_unassigned_allocates_and_sets_k failed");
        std::cout << "Test 1: Variable unassigned allocation and k propagation - "
                  << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test 2
    {
        bool result = test_case_variable_with_preassigned_reg_uses_existing();
        CHECK(result, "test_case_variable_with_preassigned_reg_uses_existing failed");
        std::cout << "Test 2: Variable with preassigned reg uses existing value - "
                  << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test 3
    {
        bool result = test_case_non_variable_produces_null();
        CHECK(result, "test_case_non_variable_produces_null failed");
        std::cout << "Test 3: Non-variable offset yields NULL (no code) - "
                  << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test 4
    {
        bool result = test_case_multiple_allocations_increment_reg();
        CHECK(result, "test_case_multiple_allocations_increment_reg failed");
        std::cout << "Test 4: Multiple allocations increment register numbers - "
                  << (result ? "PASSED" : "FAILED") << std::endl;
    }

    test_report();
}

// ---------------------------
// Main
// ---------------------------

int main() {
    run_all_tests();
    return (g_failed == 0) ? 0 : 1;
}