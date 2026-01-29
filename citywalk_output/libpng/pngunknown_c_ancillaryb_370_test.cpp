// Test suite for the focal method ancillaryb in pngunknown.c
// This test is a lightweight, GTest-free C++11 test harness.
// It exercises ancillaryb by feeding known 4-byte names and checking deterministic
// behavior and inter-input distinctions. It avoids terminating on failure and reports
// a summary at the end.
//
// Step 1 (Conceptual): Candidate keywords for the method under test
// - ancillaryb: the function under test
// - PNG_U32: macro used to pack four bytes into a 32-bit value
// - PNG_CHUNK_ANCILLARY: macro used to classify the 32-bit value as an ancillary chunk
// - png_byte: byte type (typically unsigned char)
// - name[0], name[1], name[2], name[3]: input bytes used to create the 32-bit value
// - Unknown chunk handling in PNG (ancillary vs critical) is influenced by four input bytes
//
// Step 2 (Unit Test Generation): A minimal suite focusing on ancillaryb behavior.
// Step 3 (Test Case Refinement): Deterministic outputs, cross-input distinctions, and
// non-terminating assertions are used to maximize test coverage.

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>


// The production code exposes a function with C linkage.
// We declare it here to enable linking against pngunknown.c without needing its headers.
// It is assumed that the actual signature matches: png_uint_32 ancillaryb(const png_byte *name);
extern "C" uint32_t ancillaryb(const unsigned char *name);

// Simple non-GTest test harness (non-terminating assertions).
static int g_passed = 0;
static int g_failed = 0;

// Log a passing result
static void t_pass(const char* test_name) {
    ++g_passed;
    // Optional: print per-test pass (comment out if too verbose)
    // fprintf(stdout, "PASSED: %s\n", test_name);
}

// Log a failing result with a message
static void t_fail(const char* test_name, const char* msg) {
    ++g_failed;
    fprintf(stderr, "FAILED: %s -> %s\n", test_name, msg);
}

// Assert equality for 32-bit unsigned integers (non-terminating)
static void t_assert_eq_u32(const char* test_name, uint32_t a, uint32_t b) {
    if (a == b) {
        t_pass(test_name);
    } else {
        char buf[256];
        snprintf(buf, sizeof(buf),
                 "expected %u, got %u", (unsigned)a, (unsigned)b);
        t_fail(test_name, buf);
    }
}

// Assert inequality for 32-bit unsigned integers (non-terminating)
static void t_assert_ne_u32(const char* test_name, uint32_t a, uint32_t b) {
    if (a != b) {
        t_pass(test_name);
    } else {
        char buf[256];
        snprintf(buf, sizeof(buf),
                 "unexpected equality: both are %u", (unsigned)a);
        t_fail(test_name, buf);
    }
}

// Test 1: Deterministic behavior on identical inputs
// - The same 4-byte name should always yield the same output.
static void test_ancillaryb_deterministic_same_input() {
    const unsigned char name[4] = {0x00, 0x00, 0x00, 0x00};
    uint32_t r1 = ancillaryb(name);
    uint32_t r2 = ancillaryb(name);
    t_assert_eq_u32("ancillaryb_deterministic_same_input", r1, r2);
}

// Test 2: Distinct inputs should (likely) map to distinct outputs
// - Use a set of varied inputs to exercise the mapping.
static void test_ancillaryb_distinct_inputs_basic() {
    const unsigned char n0[4] = {0x00, 0x00, 0x00, 0x00};
    const unsigned char n1[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    const unsigned char n2[4] = {'A','B','C','D'};
    const unsigned char n3[4] = {'a','b','c','d'};

    uint32_t r0 = ancillaryb(n0);
    uint32_t r1 = ancillaryb(n1);
    uint32_t r2 = ancillaryb(n2);
    uint32_t r3 = ancillaryb(n3);

    t_assert_ne_u32("ancillaryb_distinct_inputs_basic: n0 vs n1", r0, r1);
    t_assert_ne_u32("ancillaryb_distinct_inputs_basic: n0 vs n2", r0, r2);
    t_assert_ne_u32("ancillaryb_distinct_inputs_basic: n0 vs n3", r0, r3);
    t_assert_ne_u32("ancillaryb_distinct_inputs_basic: n1 vs n2", r1, r2);
    t_assert_ne_u32("ancillaryb_distinct_inputs_basic: n1 vs n3", r1, r3);
    t_assert_ne_u32("ancillaryb_distinct_inputs_basic: n2 vs n3", r2, r3);
}

// Test 3: Case sensitivity effect
// - Compare an all-lowercase name to its all-uppercase counterpart to ensure
//   the function's output reflects ASCII case information.
static void test_ancillaryb_case_sensitivity_effect() {
    const unsigned char lower[4] = {'a','b','c','d'};
    const unsigned char upper[4] = {'A','B','C','D'};

    uint32_t rl = ancillaryb(lower);
    uint32_t ru = ancillaryb(upper);

    t_assert_ne_u32("ancillaryb_case_sensitivity_effect: lower vs upper", rl, ru);
}

// Test 4: Reproducibility across multiple calls with different but related inputs
// - Slightly varying inputs should produce different outputs.
static void test_ancillaryb_intermingling_inputs() {
    const unsigned char x1[4] = {0x01, 0x02, 0x03, 0x04};
    const unsigned char x2[4] = {0x01, 0x02, 0x03, 0x05}; // last byte changed

    uint32_t r1 = ancillaryb(x1);
    uint32_t r2 = ancillaryb(x2);

    t_assert_ne_u32("ancillaryb_intermingling_inputs: last byte variation", r1, r2);
}

// Run all tests and print a summary
int main(void) {
    // Explain to the user what is tested
    fprintf(stdout, "Starting tests for ancillaryb (pngunknown.c)\n");

    test_ancillaryb_deterministic_same_input();
    test_ancillaryb_distinct_inputs_basic();
    test_ancillaryb_case_sensitivity_effect();
    test_ancillaryb_intermingling_inputs();

    // Summary
    fprintf(stdout, "Tests completed. Passed: %d, Failed: %d\n",
            g_passed, g_failed);

    // Non-terminating test: return non-zero if any test failed
    return (g_failed != 0) ? 1 : 0;
}