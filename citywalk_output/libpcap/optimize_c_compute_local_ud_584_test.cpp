/*
 * Lightweight C++11 unit test suite for the focal method:
 *   compute_local_ud(struct block *b)
 *
 * Notes:
 * - This test harness is designed to work without GoogleTest or other external
 *   test frameworks. It uses a small, non-terminating assertion mechanism
 *   that reports failures but continues execution to improve coverage.
 * - It relies on the project headers (e.g., optimize.h) that define the
 *   necessary data structures and macros (struct block, struct slist, struct stmt,
 *   NOP, AX_ATOM, X_ATOM, A_ATOM, N_ATOMS, ATOMELEM, ATOMMASK, BPF_CLASS, etc.)
 * - The tests focus on creating minimal block configurations to exercise the
 *   function, asserting on outputs where feasible (e.g., def, kill, in_use).
 * - Since internal atomuse/atomdef implementations are part of the focal code,
 *   the tests aim to exercise safe, deterministic configurations (e.g., blocks
 *   with no statements, or a statement set to NOP) to avoid triggering abort()
 *   paths while still validating that the function completes and sets its output
 *   fields to sane values.
 *
 * To run:
 * - Include this file in your build alongside the codebase, or compile and link
 *   it with the same source tree that provides optimize.h and the implementation
 *   of compute_local_ud.
 * - Ensure C linkage for the focal function is compatible with the C++ test file.
 *
 * This file outputs simple diagnostic messages to stdout for failing tests and
 * a final summary of test results.
 */

#include <gencode.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <optimize.h>
#include <cstdio>
#include <setjmp.h>
#include <string>
#include <diag-control.h>
#include <vector>
#include <limits.h>
#include <functional>
#include <stdlib.h>
#include <pcap-int.h>
#include <pcap-types.h>
#include <string.h>
#include <memory.h>
#include <os-proto.h>
#include <config.h>
#include <cstring>


extern "C" {
    // Forward declaration of the focal function from optimize.c
    // The actual signature is: void compute_local_ud(struct block *b);
    // It takes a pointer to a block and mutates its def, kill, in_use fields.
    void compute_local_ud(struct block *b);
}

// Include project-specific definitions for types/macros used by the focal method.
// This provides struct block, struct slist, struct stmt, and constants like NOP, etc.

//
// Lightweight test framework (non-terminating assertions)
//

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __func__ << ": " << msg << " (" #cond ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

#define TESTCASE(name) static void name()

// Utility: zero-initialize a POD block object safely
template <typename T>
static void zero_struct(T *p) {
    if (p) {
        std::memset(p, 0, sizeof(T));
    }
}

// Helper to print summary
static void print_summary() {
    std::cout << "Test Summary: " << g_tests_run << " run, "
              << g_tests_failed << " failed." << std::endl;
}

// Test 1: No statements in the block.
// Rationale: exercise the path where the for-loop is skipped and ensure
// that the function does not crash and yields zeroed def/in_use/kill.
TESTCASE(test_compute_local_ud_no_stmts) {
    // Prepare a block with no statements
    struct block b;
    zero_struct(&b);

    // Explicitly set an empty list of statements
    b.stmts = NULL;
    // Root/Block header statement (b.s) to a non-JMP non-NOP class is not required
    // for this test since there are no statements; ensure it's at least defined.
    b.s.code = NOP; // Make sure the block header itself is a NOP for safety

    // Call the focal function
    compute_local_ud(&b);

    // Validate post-conditions: def, kill, in_use should be zeroed
    TEST_ASSERT(b.def == 0, "def should be 0 when there are no statements");
    TEST_ASSERT(b.kill == 0, "kill should be 0 when there are no statements");
    TEST_ASSERT(b.in_use == 0, "in_use should be 0 when there are no statements");
}

// Test 2: Single NOP statement in the block's statement list.
// Rationale: ensures that non-empty stmts contributes no extra definitions
// or uses when statements themselves are NOPs, and that the loop handles
// non-executable statements gracefully.
TESTCASE(test_compute_local_ud_with_single_nop_stmt) {
    // Prepare a block with one NOP statement
    struct block b;
    zero_struct(&b);

    // Allocate a single slist node representing a NOP statement
    static struct slist s_node;
    s_node.next = NULL;
    // The inner stmt (s) must have a code field; set to a value that
    // represents a non-NOP instruction to exercise the branch where
    // s->s.code != NOP (but the NOP itself should be skipped by the loop).
    // However, we still set its code to NOP to avoid introducing complex
    // atomuse/atomdef interactions in this test.
    s_node.s.code = NOP;

    // Link the single statement into the block
    b.stmts = &s_node;

    // Root block header still set to NOP
    b.s.code = NOP;

    // Run the function
    compute_local_ud(&b);

    // The for-loop will skip the only statement due to NOP; after completion,
    // the block's fields should remain zeroed except potential side effects
    // from the outer scope (which we expect to be 0 here as well).
    TEST_ASSERT(b.def == 0, "def should be 0 when only NOP statements exist");
    TEST_ASSERT(b.kill == 0, "kill should be 0 when only NOP statements exist");
    TEST_ASSERT(b.in_use == 0, "in_use should be 0 when only NOP statements exist");
}

//
// Main entry: run tests
//
int main() {
    // Run test cases
    test_compute_local_ud_no_stmts();
    test_compute_local_ud_with_single_nop_stmt();

    // Output summary
    print_summary();

    // Return 0 to indicate the harness itself ran to completion.
    // Individual test results are reported via TEST_ASSERT messages.
    return (g_tests_failed == 0) ? 0 : 1;
}