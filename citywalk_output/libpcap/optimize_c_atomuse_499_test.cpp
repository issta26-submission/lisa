/*
  C++11 test suite for the focal function: atomuse(struct stmt *s)

  Notes:
  - The tests rely on the public interfaces and macros from the project
    (e.g., optimize.h, BPF_* constants, A_ATOM, X_ATOM, AX_ATOM, NOP, etc.).
  - We implement a lightweight test harness (no GoogleTest) that runs all tests
    from main. Tests use non-terminating assertions: they report pass/fail but
    do not abort the test run, so we gain higher coverage across tests.
  - Each test includes explanatory comments describing what branch/condition is exercised.
  - The tests assume the project defines struct stmt { int code; bpf_u_int32 k; } and
    the BPF-related macros used by atomuse (NOP, BPF_CLASS, BPF_RET, BPF_LD,
    BPF_LDX, BPF_IND, BPF_MEM, BPF_ST, BPF_STX, BPF_JMP, BPF_ALU, BPF_MISC,
    BPF_SRC, BPF_X, BPF_A, BPF_TXA, A_ATOM, X_ATOM, AX_ATOM, etc.).
  - The test calls the function via C linkage: extern "C" int atomuse(struct stmt *s);
*/

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdint>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Forward declaration of the focal function with C linkage.
// The actual signature is provided by the project's headers (optimize.h).
extern "C" int atomuse(struct stmt *s);

// Include project headers to ensure proper macro/struct definitions.
// It must be the same headers used by the focal code.

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void report_result(const char* test_name, bool passed, const char* details = nullptr) {
    ++tests_run;
    if (passed) {
        ++tests_passed;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        ++tests_failed;
        std::cout << "[FAIL] " << test_name;
        if (details) std::cout << " - " << details;
        std::cout << "\n";
    }
}

// Helper to create a dummy stmt with given code and k.
// We rely on the project definition: struct stmt { int code; bpf_u_int32 k; };
static struct stmt make_stmt(int code, uint32_t k = 0) {
    struct stmt s;
    s.code = code;
    s.k = k;
    return s;
}

// Test 1: NOP should yield -1
static bool test_atomuse_nop_returns_minus_one() {
    struct stmt s = make_stmt(NOP, 0);
    int res = atomuse(&s);
    return (res == -1);
}

// Test 2: RET with A should yield A_ATOM
static bool test_atomuse_ret_with_A_returns_A_ATOM() {
    // Construct a RET instruction carrying A as the return value.
    // We combine the RET class with A as the RVAL bits if BPF_* definitions place A at the RVAL field.
    int code = BPF_RET | BPF_A;
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == A_ATOM);
}

// Test 2b: RET with X should yield X_ATOM
static bool test_atomuse_ret_with_X_returns_X_ATOM() {
    int code = BPF_RET | BPF_X;
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == X_ATOM);
}

// Test 3: LD with IND should yield X_ATOM
static bool test_atomuse_ld_ind_returns_X_ATOM() {
    int code = BPF_LD | BPF_IND;
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == X_ATOM);
}

// Test 4: LD with MEM should yield k value
static bool test_atomuse_ld_mem_returns_k() {
    uint32_t kval = 1234;
    int code = BPF_LD | BPF_MEM;
    struct stmt s = make_stmt(code, kval);
    int res = atomuse(&s);
    // The function returns (int)s->k when MEM, so cast to int should match kval
    return (static_cast<uint32_t>(res) == kval);
}

// Test 5: ST should yield A_ATOM
static bool test_atomuse_st_returns_A_ATOM() {
    int code = BPF_ST;
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == A_ATOM);
}

// Test 6: STX should yield X_ATOM
static bool test_atomuse_stx_returns_X_ATOM() {
    int code = BPF_STX;
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == X_ATOM);
}

// Test 7: JMP/ALU with SRC == X should yield AX_ATOM
static bool test_atomuse_jmpalu_srcx_returns_AX_ATOM() {
    int code = BPF_JMP | BPF_X; // ensure SRC is X
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == AX_ATOM);
}

// Test 8: MISC with TXA should yield X_ATOM
static bool test_atomuse_misc_txa_returns_X_ATOM() {
    int code = BPF_MISC | BPF_TXA;
    struct stmt s = make_stmt(code, 0);
    int res = atomuse(&s);
    return (res == X_ATOM);
}

// Test 9: MISC with non-TXA should yield A_ATOM (coverage for else branch)
static bool test_atomuse_misc_not_txa_returns_A_ATOM() {
    int code = BPF_MISC;          // MISC with a non-TXA opcode
    struct stmt s = make_stmt(code, 0);
    // To simulate non-TXA, ensure BPF_MISCOP(c) != BPF_TXA.
    // This relies on BPF_MISCOP macro returning a value based on code.
    int res = atomuse(&s);
    // If the code's MISCOP is not TXA, expect A_ATOM as per function logic.
    return (res == A_ATOM);
}

// Main: run all tests
int main() {
    // Run tests and accumulate results. Non-terminating assertions allow us to
    // exercise multiple branches in a single run.
    report_result("atomuse_nop_returns_minus_one", test_atomuse_nop_returns_minus_one(),
                  "NOP should produce -1");
    report_result("atomuse_ret_with_A_returns_A_ATOM", test_atomuse_ret_with_A_returns_A_ATOM(),
                  "RET with A should yield A_ATOM");
    report_result("atomuse_ret_with_X_returns_X_ATOM", test_atomuse_ret_with_X_returns_X_ATOM(),
                  "RET with X should yield X_ATOM");
    report_result("atomuse_ld_ind_returns_X_ATOM", test_atomuse_ld_ind_returns_X_ATOM(),
                  "LD IND should yield X_ATOM");
    report_result("atomuse_ld_mem_returns_k", test_atomuse_ld_mem_returns_k(),
                  "LD MEM should return s->k as int");
    report_result("atomuse_st_returns_A_ATOM", test_atomuse_st_returns_A_ATOM(),
                  "ST should yield A_ATOM");
    report_result("atomuse_stx_returns_X_ATOM", test_atomuse_stx_returns_X_ATOM(),
                  "STX should yield X_ATOM");
    report_result("atomuse_jmpalu_srcx_returns_AX_ATOM", test_atomuse_jmpalu_srcx_returns_AX_ATOM(),
                  "JMP/ALU with SRC X should yield AX_ATOM");
    report_result("atomuse_misc_txa_returns_X_ATOM", test_atomuse_misc_txa_returns_X_ATOM(),
                  "MISC TXA should yield X_ATOM");
    report_result("atomuse_misc_not_txa_returns_A_ATOM", test_atomuse_misc_not_txa_returns_A_ATOM(),
                  "MISC non-TXA should yield A_ATOM");

    // Summary
    std::cout << "\nTest summary: " << tests_passed << " passed, "
              << tests_failed << " failed, out of " << tests_run << " tests.\n";

    // Return non-zero if any test failed to indicate failure in automation
    return (tests_failed == 0) ? 0 : 1;
}