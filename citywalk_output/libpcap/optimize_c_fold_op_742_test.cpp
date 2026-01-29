/*
  Unit test suite for the focal method fold_op in optimize.c
  - Target environment: C++11, integrated with the project's C code (libpcap-like BPF optimizer)
  - No Google Test; a lightweight, self-contained test harness is provided.
  - The tests exercise multiple operator branches (ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, LSH, RSH)
  - Each test initializes an opt_state_t instance and a stmt instance, then calls fold_op
  - All assertions are non-terminating; failures are reported but do not abort the test run
  - Static member demonstration included to align with domain knowledge guidelines
*/

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <vector>
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
#include <string>
#include <iostream>
#include <pcap-types.h>


// Forward declaration of the C API exported by optimize.c
// Include the real project header if available in your environment.
// The test harness assumes the project provides the following types/macros:
// - typedef unsigned int bpf_u_int32;
// - typedef struct opt_state_t opt_state_t;
// - typedef struct stmt { unsigned int code; unsigned int k; } stmt; (actual
//   definition comes from project headers; the test uses the real type through the
//   project's headers when compiled in your environment.)
extern "C" {
//                         BPF_OP macro, and the fold_op function.
// If your project organizes headers differently, adjust the include accordingly.
}

// Lightweight test harness (non-terminating assertions)
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAILED] " << msg << std::endl; \
        g_passed = false; \
    } \
} while (0)

static bool g_passed = true;

// Helper: reset state for each test
static void reset_state(opt_state_t &opt, stmt &s) {
    // Best effort reset: zero out the provided structures to a clean slate.
    // Rely on the actual types' semantics in the project environment.
    std::memset(&opt, 0, sizeof(opt_state_t));
    std::memset(&s,   0, sizeof(stmt));
}

// Expose a minimal wrapper to the focal function for C linkage when linking from C++
extern "C" {
    void fold_op(opt_state_t *opt_state, struct stmt *s, bpf_u_int32 v0, bpf_u_int32 v1);
}

// Static member demonstration (domain knowledge)
class StaticDemo {
public:
    static int static_counter;
    static void reset() { static_counter = 0; }
    static int increment() { return ++static_counter; }
};
int StaticDemo::static_counter = 0;

/*
  Test 1: Addition path (BPF_ADD)
  - a = v0, b = v1
  - Expect s->k == a + b
*/
static void test_fold_op_add() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    // Prepare vmap constants for v0 and v1
    // Assumes opt_state_t.vmap[] exists and has a member const_val
    // The actual project header defines these; use the real fields in your environment.
    opt.vmap[0].const_val = 2; // v0
    opt.vmap[1].const_val = 3; // v1

    // Set the operation to ADD
    s.code = BPF_ADD; // Macro provided by optimize.h
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 5, "fold_op ADD: expected 2 + 3 = 5");
    TEST_ASSERT(s.code == (BPF_LD|BPF_IMM), "fold_op ADD: s->code should be updated to BPF_LD|BPF_IMM");
}

/*
  Test 2: Subtraction path (BPF_SUB)
  - a = 7, b = 4
  - Expect s->k == a - b
*/
static void test_fold_op_sub() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 7; // v0
    opt.vmap[1].const_val = 4; // v1

    s.code = BPF_SUB;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 3, "fold_op SUB: expected 7 - 4 = 3");
}

/*
  Test 3: Multiplication path (BPF_MUL)
  - a = 6, b = 4
  - Expect s->k == a * b
*/
static void test_fold_op_mul() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 6;
    opt.vmap[1].const_val = 4;

    s.code = BPF_MUL;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 24, "fold_op MUL: expected 6 * 4 = 24");
}

/*
  Test 4: Division path (BPF_DIV)
  - a = 20, b = 5
  - Expect s->k == a / b
*/
static void test_fold_op_div() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 20;
    opt.vmap[1].const_val = 5;

    s.code = BPF_DIV;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 4, "fold_op DIV: expected 20 / 5 = 4");
}

/*
  Test 5: Modulus path (BPF_MOD)
  - a = 20, b = 3
  - Expect s->k == a % b
*/
static void test_fold_op_mod() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 20;
    opt.vmap[1].const_val = 3;

    s.code = BPF_MOD;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 2, "fold_op MOD: expected 20 % 3 = 2");
}

/*
  Test 6: AND path (BPF_AND)
  - a = 0b1010 (10), b = 0b1100 (12)
  - Expect s->k == a & b
*/
static void test_fold_op_and() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 10;
    opt.vmap[1].const_val = 12;

    s.code = BPF_AND;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == (10 & 12), "fold_op AND: bitwise AND result mismatch");
}

/*
  Test 7: OR path (BPF_OR)
  - a = 0b1010 (10), b = 0b1100 (12)
  - Expect s->k == a | b
*/
static void test_fold_op_or() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 10;
    opt.vmap[1].const_val = 12;

    s.code = BPF_OR;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == (10 | 12), "fold_op OR: bitwise OR result mismatch");
}

/*
  Test 8: XOR path (BPF_XOR)
  - a = 0b1010 (10), b = 0b1100 (12)
  - Expect s->k == a ^ b
*/
static void test_fold_op_xor() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 10;
    opt.vmap[1].const_val = 12;

    s.code = BPF_XOR;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == (10 ^ 12), "fold_op XOR: bitwise XOR result mismatch");
}

/*
  Test 9: Left shift path with b < 32 (BPF_LSH)
  - a = 1, b = 3
  - Expect s->k == a << b
*/
static void test_fold_op_lsh_lt32() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 1;
    opt.vmap[1].const_val = 3;

    s.code = BPF_LSH;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == (1 << 3), "fold_op LSH (<32): 1 << 3 should be 8");
}

/*
  Test 10: Left shift path with b >= 32 (BPF_LSH)
  - a = 1, b = 32
  - Expect a becomes 0 per implementation
*/
static void test_fold_op_lsh_ge32() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 1;
    opt.vmap[1].const_val = 32;

    s.code = BPF_LSH;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 0, "fold_op LSH (>=32): 1 << 32 should yield 0 per policy");
}

/*
  Test 11: Right shift path with b < 32 (BPF_RSH)
  - a = 8, b = 2
  - Expect s->k == a >> b
*/
static void test_fold_op_rsh_lt32() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 8;
    opt.vmap[1].const_val = 2;

    s.code = BPF_RSH;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == (8 >> 2), "fold_op RSH (<32): 8 >> 2 should be 2");
}

/*
  Test 12: Right shift path with b >= 32 (BPF_RSH)
  - a = 8, b = 32
  - Expect a becomes 0 per implementation
*/
static void test_fold_op_rsh_ge32() {
    opt_state_t opt;
    stmt s;
    reset_state(opt, s);

    opt.vmap[0].const_val = 8;
    opt.vmap[1].const_val = 32;

    s.code = BPF_RSH;
    fold_op(&opt, &s, 0, 1);

    TEST_ASSERT(s.k == 0, "fold_op RSH (>=32): 8 >> 32 should yield 0 per policy");
}

/*
  Runner: execute all tests and summarize
*/
int main() {
    // Initialize static example to demonstrate “static member” concept
    StaticDemo::reset();

    // Run all tests; each test updates g_passed accordingly via TEST_ASSERT
    test_fold_op_add();
    test_fold_op_sub();
    test_fold_op_mul();
    test_fold_op_div();
    test_fold_op_mod();
    test_fold_op_and();
    test_fold_op_or();
    test_fold_op_xor();
    test_fold_op_lsh_lt32();
    test_fold_op_lsh_ge32();
    test_fold_op_rsh_lt32();
    test_fold_op_rsh_ge32();

    // Report summary
    if (g_passed) {
        std::cout << "[TEST SUCCEEDED] All fold_op tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[TEST FAILED] Some fold_op tests failed. See messages above for details." << std::endl;
        return 1;
    }
}