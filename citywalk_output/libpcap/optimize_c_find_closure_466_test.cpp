// Test suite for the focal method: find_closure(opt_state_t *opt_state, struct block *root)
// This test uses a minimal harness without any external test framework (no GTest).
// It relies on the project headers to provide opt_state_t, struct block, and the macros
// SET_INSERT, SET_UNION, JT, JF, etc., as used by find_closure.
// The tests are designed to cover both branches of the conditional (JT != NULL and JT == NULL)
// and to verify that the closure bitsets are updated accordingly.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <cassert>
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


// Include the project's header to obtain correct type definitions and macros.
// This header is expected to declare opt_state_t, struct block, and find_closure.
extern "C" {
}

// Ensure the C function can be linked from C++
extern "C" void find_closure(opt_state_t *opt_state, struct block *root);

// Simple bit-check helper for 1-word closure (nodewords == 1)
static bool has_bit0(const unsigned int* set, int id) {
    // For nodewords == 1, the bit for id is located in set[0] at position id (mod 32).
    // We assume 32-bit words for the test environment.
    return (set[0] & (1u << (id % 32))) != 0;
}

// Test 1: Basic case where a single block without a true-branch (JT == NULL) is processed.
// Expected: the block's own id is inserted into its closure; no unions occur.
static bool test_find_closure_basic_no_jt() {
    // Prepare a minimal opt_state_t with 2 blocks and a single word per closure.
    opt_state_t ost;
    ost.n_blocks = 2;
    ost.nodewords = 1; // single 32-bit word per closure set

    // Allocate closure storage (used by all blocks via their own 'closure' arrays as well)
    ost.all_closure_sets = new unsigned int[ost.n_blocks * ost.nodewords];
    // Initialize to zero
    memset(ost.all_closure_sets, 0, ost.n_blocks * ost.nodewords * sizeof(*ost.all_closure_sets));

    // Allocate levels array with two levels (0 and 1)
    ost.levels = new struct block*[2];

    // Create blocks
    struct block b0; // level 0
    struct block b1; // level 1
    struct block root;

    // Initialize blocks
    b0.id = 0;
    b0.level = 0;
    b0.link = NULL;
    // Allocate closure memory for each block (1 word)
    b0.closure = new unsigned int[ost.nodewords];
    memset(b0.closure, 0, ost.nodewords * sizeof(*b0.closure));
    b0.JT = NULL; // No true-branch
    b0.JF = NULL;

    b1.id = 1;
    b1.level = 1;
    b1.link = NULL;
    b1.closure = new unsigned int[ost.nodewords];
    memset(b1.closure, 0, ost.nodewords * sizeof(*b1.closure));
    b1.JT = NULL;
    b1.JF = NULL;

    root.level = 1;

    // Level lists
    ost.levels[0] = &b0;
    ost.levels[1] = &b1;

    // Call the function under test
    find_closure(&ost, &root);

    // Check: b0 closure contains its own id (0)
    bool ok0 = has_bit0(b0.closure, 0);

    // Check: b1 closure contains its own id (1)
    bool ok1 = has_bit0(b1.closure, 1);

    // Cleanup
    delete[] ost.all_closure_sets;
    delete[] b0.closure;
    delete[] b1.closure;
    delete[] ost.levels;

    return ok0 && ok1;
}

// Test 2: Case where a block's true-branch JT is present.
// We expect a union into JT(b) closure and a separate union into JF(b) if present.
// For this test, we create two blocks: A (id=0) at level 0 and B (id=1) at level 1.
// A.JT points to B, so during processing A, B.closure should be updated with A.closure.
// After processing, B should also have its own id inserted.
static bool test_find_closure_with_jt_union() {
    // Prepare a minimal opt_state_t with 2 blocks and a single word per closure.
    opt_state_t ost;
    ost.n_blocks = 2;
    ost.nodewords = 1;

    ost.all_closure_sets = new unsigned int[ost.n_blocks * ost.nodewords];
    memset(ost.all_closure_sets, 0, ost.n_blocks * ost.nodewords * sizeof(*ost.all_closure_sets));

    ost.levels = new struct block*[2];

    struct block a; // level 0
    struct block b; // level 1
    struct block root;

    // Initialize blocks
    a.id = 0;
    a.level = 0;
    a.link = NULL;
    a.closure = new unsigned int[ost.nodewords];
    memset(a.closure, 0, ost.nodewords * sizeof(*a.closure));
    a.JT = &b;  // A's true-branch goes to B
    a.JF = NULL;

    b.id = 1;
    b.level = 1;
    b.link = NULL;
    b.closure = new unsigned int[ost.nodewords];
    memset(b.closure, 0, ost.nodewords * sizeof(*b.closure));
    b.JT = NULL;
    b.JF = NULL;

    root.level = 1;

    ost.levels[0] = &a;
    ost.levels[1] = &b;

    // Run
    find_closure(&ost, &root);

    // Expected:
    // - A.closure contains {0}
    // - B.closure contains {0, 1} due to union from A and its own insertion of 1.
    bool okA = has_bit0(a.closure, 0);
    bool okB0 = has_bit0(b.closure, 0);
    bool okB1 = has_bit0(b.closure, 1);

    // Cleanup
    delete[] ost.all_closure_sets;
    delete[] a.closure;
    delete[] b.closure;
    delete[] ost.levels;

    return okA && okB0 && okB1;
}

int main() {
    int failures = 0;

    std::cout << "Running test_find_closure_basic_no_jt...\n";
    if (test_find_closure_basic_no_jt()) {
        std::cout << "PASS: test_find_closure_basic_no_jt\n";
    } else {
        std::cout << "FAIL: test_find_closure_basic_no_jt\n";
        ++failures;
    }

    std::cout << "Running test_find_closure_with_jt_union...\n";
    if (test_find_closure_with_jt_union()) {
        std::cout << "PASS: test_find_closure_with_jt_union\n";
    } else {
        std::cout << "FAIL: test_find_closure_with_jt_union\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed.\n";
        return 1;
    }
}