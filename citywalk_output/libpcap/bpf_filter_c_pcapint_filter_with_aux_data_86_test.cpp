/*
Unit test suite for the focal method:
  pcapint_filter_with_aux_data(...)
The tests are designed to run without GoogleTest (GTest) and rely only on
the C/C++ standard library plus the project's existing C components.

Notes and design decisions:
- We target direct calls to the real function provided by the project.
- We rely on Linux's BPF definitions available via <linux/filter.h> for
  opcodes (BPF_RET, BPF_K, BPF_A, BPF_LD, BPF_W, BPF_ABS, etc.).
- We craft minimal BPF instruction sequences where the last instruction in the
  program is the one executed first by the interpreter (as the real code does:
  it starts by executing the last instruction, then moves forward). This helps us
  exercise simple return paths deterministically.
- We cover:
  - Null program pointer (pc == 0) -> returns (u_int)-1.
  - Simple return using BPF_RET|BPF_K (return constant).
  - Return using BPF_RET|BPF_A (return accumulator A; initial A = 0).
  - Error path for BPF_LD|BPF_W|BPF_ABS with an out-of-bounds offset (k > buflen).
- Static analysis hints: we don’t mock private internals; we call the actual function
  and verify expected outputs. The tests keep aux_data as NULL to avoid VLAN tag handling
  paths unless explicitly exercising those branches.

To compile and run:
- Build with a typical Linux toolchain where <linux/filter.h> is available.
- Link the test object with the project’s bpf_filter.o (or compile and link with
  the source that contains pcapint_filter_with_aux_data).
- Run the produced executable; it will print test results to stdout.

Code:

*/

#include <diag-control.h>
#include <pcap/pcap-inttypes.h>
#include <sys/types.h>
#include <linux/types.h>
#include <sys/time.h>
#include <cstdint>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <pcap-int.h>
#include <stdlib.h>
#include <config.h>
#include <extract.h>
#include <cstring>
#include <sys/param.h>
#include <iostream>
#include <pcap-types.h>


// Include linux BPF definitions to construct test programs.
// These provide struct bpf_insn and BPF_* constants used by the target function.

// Forward declaration of the focal function (C linkage to avoid name mangling in C++)
extern "C" {
    // The exact signature (as provided in the project). We mirror types used by the function.
    // Note: The project's actual types may differ in headers; we rely on typical Linux definitions.
    unsigned int pcapint_filter_with_aux_data(const struct bpf_insn *pc,
                                              const unsigned char *p,
                                              unsigned int wirelen,
                                              unsigned int buflen,
                                              const struct pcap_bpf_aux_data *aux_data);
}

// Lightweight typedefs to match test file expectations across translation units
typedef unsigned char u_char;
typedef unsigned int u_int;

// Minimal aux data structure as used by the focal function for VLAN tag paths.
// We only populate fields required by the tested branches (vlan_tag and vlan_tag_present).
struct pcap_bpf_aux_data {
    uint32_t vlan_tag;
    uint32_t vlan_tag_present;
};

// Candidate keyword mapping (Step 1) visibility for test generation
// - pcapint_filter_with_aux_data
// - BPF_RET, BPF_K, BPF_A
// - BPF_LD, BPF_W, BPF_ABS
// - BPF_LD|BPF_W|BPF_ABS path (out-of-bounds check)
// - aux_data VLAN fields (vlan_tag, vlan_tag_present)

static int tests_passed = 0;
static int tests_failed = 0;

#define LOG_PASS(msg) do { std::cout << "[PASS] " << msg << "\n"; } while (0)
#define LOG_FAIL(msg) do { std::cerr << "[FAIL] " << msg << "\n"; } while (0)

// Simple assertion helper with non-terminating behavior (per domain knowledge)
#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) == (actual)) { LOG_PASS(msg); ++tests_passed; } \
    else { LOG_FAIL((std::string(msg) + " (expected: " + std::to_string(expected) + ", actual: " + std::to_string(actual) + ")").c_str()); ++tests_failed; } \
} while (0)

// Test 1: pc == NULL should return (u_int)-1
void test_pc_null() {
    unsigned char data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    unsigned int res = pcapint_filter_with_aux_data(nullptr, data, 8, 8, nullptr);
    // (u_int)-1 as unsigned wraparound. Use explicit cast for clarity in comparison.
    ASSERT_EQ(static_cast<unsigned int>(-1), res, "pc == NULL should return (u_int)-1");
}

// Test 2: Simple return via BPF_RET|BPF_K
// Program layout:
//  prog[0] = dummy (not executed directly in this harness)
//  prog[1] = BPF_RET|BPF_K with k = 123
// The interpreter executes the last instruction first, hence this returns 123.
void test_ret_k_returns_constant() {
    struct bpf_insn prog[2];
    // dummy instruction (not used in this test, but present to satisfy array length)
    prog[0].code = 0;
    prog[0].jt = 0;
    prog[0].jf = 0;
    prog[0].k = 0;

    prog[1].code = BPF_RET | BPF_K;
    prog[1].jt = 0;
    prog[1].jf = 0;
    prog[1].k = 123; // expected return value

    unsigned char payload[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    unsigned int res = pcapint_filter_with_aux_data(prog, payload, 4, 4, nullptr);
    ASSERT_EQ(123u, res, "BPF_RET|BPF_K should return k value (123)");
}

// Test 3: Simple return via BPF_RET|BPF_A
// A starts at 0. The function should return A (0) when the last instruction is BPF_RET|BPF_A.
void test_ret_a_returns_accumulator() {
    struct bpf_insn prog[2];
    prog[0].code = 0;
    prog[0].jt = 0;
    prog[0].jf = 0;
    prog[0].k = 0;

    prog[1].code = BPF_RET | BPF_A;
    prog[1].jt = 0;
    prog[1].jf = 0;
    prog[1].k = 0; // ignored for BPF_A

    unsigned char payload[4] = {0x11, 0x22, 0x33, 0x44};
    unsigned int res = pcapint_filter_with_aux_data(prog, payload, 4, 4, nullptr);
    // A is initialized to 0, so expect 0
    ASSERT_EQ(0u, res, "BPF_RET|BPF_A should return A (initialized to 0)");
}

// Test 4: BPF_LD|BPF_W|BPF_ABS with an out-of-bounds offset should return 0
// This covers the error path inside the LD_ABS branch.
void test_ld_w_abs_out_of_bounds_returns_zero() {
    struct bpf_insn prog[2];
    prog[0].code = 0;
    prog[0].jt = 0;
    prog[0].jf = 0;
    prog[0].k = 0;

    // Intentionally out-of-bounds k to trigger the "k > buflen" path
    prog[1].code = BPF_LD | BPF_W | BPF_ABS;
    prog[1].jt = 0;
    prog[1].jf = 0;
    prog[1].k = 10; // buflen will be set to 4 in test, so this is out-of-bounds

    unsigned char payload[4] = {0x01, 0x02, 0x03, 0x04};
    unsigned int res = pcapint_filter_with_aux_data(prog, payload, 4, 4, nullptr);
    // Should return 0 due to out-of-bounds guard
    ASSERT_EQ(0u, res, "BPF_LD|BPF_W|BPF_ABS with k out of bounds should return 0");
}

int main() {
    test_pc_null();
    test_ret_k_returns_constant();
    test_ret_a_returns_accumulator();
    test_ld_w_abs_out_of_bounds_returns_zero();

    std::cout << "\nTest Summary: " << tests_passed << " passed, "
              << tests_failed << " failed." << std::endl;

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}