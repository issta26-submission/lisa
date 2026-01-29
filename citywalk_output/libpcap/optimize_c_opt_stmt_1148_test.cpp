// C++11 test suite for the focal method opt_stmt (C code) from optimize.c
// This test harness assumes the project headers provide the C types and
// constants used by opt_stmt (e.g., opt_state_t, struct stmt, BPF_* constants,
// A_ATOM, X_ATOM, K(), F(), vstore(), etc.).
// The tests are written to be non-terminating (do not call exit on failure)
// and rely on a small internal reporting mechanism.
// Note: This file is intended to be compiled and linked with the project sources.
// It uses a lightweight, self-contained test runner (no GoogleTest).

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
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


// Bring in C declarations. If the project uses C headers with guards, these
// extern "C" wrappers ensure proper linkage when compiled as C++.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
#define TEST_ASSERT(cond, msg)                          \
    do {                                                \
        if (!(cond)) {                                  \
            std::cerr << "ASSERT FAILED: " << msg      \
                      << "  (in " << __FILE__           \
                      << ":" << __LINE__ << ")" << std::endl; \
            return false;                               \
        }                                               \
    } while (0)

///////////////////////////////////////////////////////////////////////////////
// Helpers to dump and reason about results (for debugging if needed)
// These helpers are intentionally simple to keep the tests focused on behavior.
static void dump_uint32(bpf_u_int32 v) {
    std::cout << "0x" << std::hex << v << std::dec;
}

///////////////////////////////////////////////////////////////////////////////
// Test 1: LD IMM should place the immediate constant into A_ATOM (val[A_ATOM])
// This validates the straightforward IMM path in opt_stmt.
bool test_opt_stmt_ld_imm_constant() {
    // Arrange
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    struct stmt s;
    memset(&s, 0, sizeof(s));

    // Value array after op; ensure a clean slate
    bpf_u_int32 val[4];
    memset(val, 0, sizeof(val));

    // Configure the statement to take the IMM path
    s.code = BPF_LD | BPF_IMM;
    s.k = 123; // immediate value

    int alter = 0; // no alteration of optimizer state required for this path

    // Act
    opt_stmt(&opt_state, &s, val, alter);

    // Assert
    // val[A_ATOM] should be K(123). In typical BPF code, K(x) returns x.
    TEST_ASSERT(val[A_ATOM] == K(123),
                "LD IMM path should store immediate constant into val[A_ATOM]");

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Test 2: LD IND with a constant in vmap should transform to ABS + SIZE and set done flag.
// This exercises the path where alter is true and vmap[v].is_const is true.
bool test_opt_stmt_ld_ind_const_transform() {
    // Arrange
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    // Prepare vmap so that v = val[X_ATOM] is treated as a constant
    // We assume vmap is indexable by an integer; set index 7 to be a const (value 5)
    opt_state.vmap[7].is_const = 1;
    opt_state.vmap[7].const_val = 5;

    struct stmt s;
    memset(&s, 0, sizeof(s));

    bpf_u_int32 val[4];
    memset(val, 0, sizeof(val));

    // Start with an LD IND of size W
    // The exact size macro BPF_W is used in the SIZE() calculation
    s.code = BPF_LD | BPF_IND | BPF_W;
    s.k = 0;
    val[X_ATOM] = 7; // v = val[X_ATOM] -> 7

    int alter = 1;

    // Act
    opt_stmt(&opt_state, &s, val, alter);

    // Assert
    // Expect s->code to have been transformed to BPF_LD|BPF_ABS|BPF_W
    TEST_ASSERT(s.code == (BPF_LD | BPF_ABS | BPF_W),
                "LD IND with const should transform to ABS with correct size.");
    // And optimizer state should indicate no further non-branch movement was performed
    TEST_ASSERT(opt_state.done == 0,
                "opt_state.done should be cleared (set to 0) after transformation.");

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Test 3: ALU NEG with const accumulator should negate via unsigned arithmetic.
// When alter is true and the A_ATOM value comes from a const in vmap, the code
// path updates s->k and val[A_ATOM] accordingly without calling F().
bool test_opt_stmt_alu_neg_const_operand() {
    // Arrange
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    // Configure vmap so that val[A_ATOM] refers to a const value
    // We'll set index 1 to be a const (value 10)
    opt_state.vmap[1].is_const = 1;
    opt_state.vmap[1].const_val = 10;

    struct stmt s;
    memset(&s, 0, sizeof(s));

    bpf_u_int32 val[4];
    memset(val, 0, sizeof(val));

    // Start with ALU NEG
    s.code = BPF_ALU | BPF_NEG;
    val[A_ATOM] = 1; // index into vmap -> const 10

    int alter = 1;

    // Act
    opt_stmt(&opt_state, &s, val, alter);

    // Assert
    // Expect s->code to remain ALU NEG and val[A_ATOM] to be 0U - const_val
    TEST_ASSERT(s.code == (BPF_ALU | BPF_NEG),
                "ALU NEG path should remain unchanged in code for const case.");
    TEST_ASSERT(val[A_ATOM] == K(0U - opt_state.vmap[1].const_val),
                "ALU NEG with const accumulator should negate using unsigned arithmetic.");

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Test 4: LD MEM with a constant in vmap should optimize to IMM and load the const value.
// This ensures that an optimizer-controlled constant memory load is folded properly.
bool test_opt_stmt_ld_mem_const_transform() {
    // Arrange
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    // s->k will index into val; make val[3] const = 7
    opt_state.vmap[2].is_const = 1;
    opt_state.vmap[2].const_val = 7;

    struct stmt s;
    memset(&s, 0, sizeof(s));

    bpf_u_int32 val[4];
    memset(val, 0, sizeof(val));

    // s.k = 3; v = val[3] => 2 (we'll set val[3] below)
    s.code = BPF_LD | BPF_MEM;
    s.k = 3;
    val[3] = 2; // v becomes 2, which is used to look up const_val in vmap

    int alter = 1;

    // Act
    opt_stmt(&opt_state, &s, val, alter);

    // Assert
    // Expect s.code to have transformed to IMM (BPF_LD|BPF_IMM)
    TEST_ASSERT(s.code == (BPF_LD | BPF_IMM),
                "LD MEM with const should transform to IMM path.");
    // k should be set to the const value (7)
    TEST_ASSERT(s.k == 7,
                "s.k should be updated to the const value used for IMM load.");
    // done flag should be reset
    TEST_ASSERT(opt_state.done == 0,
                "opt_state.done should be 0 after folding into IMM.");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Test harness
int main() {
    int tests_passed = 0;
    int tests_total = 0;

    std::cout << "Running tests for opt_stmt in optimize.c (C++11 test harness)" << std::endl;

    // Run Test 1
    ++tests_total;
    if (test_opt_stmt_ld_imm_constant()) {
        std::cout << "[PASS] test_opt_stmt_ld_imm_constant" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] test_opt_stmt_ld_imm_constant" << std::endl;
    }

    // Run Test 2
    ++tests_total;
    if (test_opt_stmt_ld_ind_const_transform()) {
        std::cout << "[PASS] test_opt_stmt_ld_ind_const_transform" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] test_opt_stmt_ld_ind_const_transform" << std::endl;
    }

    // Run Test 3
    ++tests_total;
    if (test_opt_stmt_alu_neg_const_operand()) {
        std::cout << "[PASS] test_opt_stmt_alu_neg_const_operand" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] test_opt_stmt_alu_neg_const_operand" << std::endl;
    }

    // Run Test 4
    ++tests_total;
    if (test_opt_stmt_ld_mem_const_transform()) {
        std::cout << "[PASS] test_opt_stmt_ld_mem_const_transform" << std::endl;
        ++tests_passed;
    } else {
        std::cout << "[FAIL] test_opt_stmt_ld_mem_const_transform" << std::endl;
    }

    // Summary
    std::cout << "Test results: " << tests_passed << " / " << tests_total << " passed." << std::endl;

    // Do not call exit on failure; return status code 0 (success) or 1 (any failure)
    return (tests_passed == tests_total) ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////
// Notes for maintainers (Domain Knowledge alignment)
// - The tests focus on exercising true/false branches for specific predicates within
//   opt_stmt, notably:
//   - IMM path (true, simple store into A_ATOM)
//   - IND path with const vmap entry (transformation to ABS/size, done flag flip)
//   - ALU NEG path with const accumulator (unsigned negation via 0U - value)
//   - LD MEM path with const folding into IMM (k updated to const_val, done flag)
// - Static helper state (e.g., vmap, done) is manipulated directly to trigger
//   the branch predicates. Static members in the production code are accessed
//   via the class/struct namespace provided by the headers, as per the
//   <DOMAIN_KNOWLEDGE> requirement.
// - This test suite uses non-terminating assertions (via TEST_ASSERT) to ensure
//   all code paths execute for coverage purposes.
// - The tests avoid private/internal methods; they exercise opt_stmt through its
//   public interface and shared state structures defined in the headers.
// - All tests run from main, as gtest is not allowed per the domain knowledge.
// - The tests assume the availability and correctness of K(), F(), vstore(), and
//   related macros as provided by the project headers. If any of these APIs differ
//   in your environment, adjust the expectations accordingly.