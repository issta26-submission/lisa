// Unit test suite for the focal method deadstmt in optimize.c
// This test suite targets high-coverage scenarios for deadstmt
// without using GTest. It uses a lightweight, self-contained
// assertion framework and runs under C++11.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <cstdio>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <pcap-types.h>


// Include the project header that declares the focal function
// and the necessary dependent structures.
// We assume the project provides opt_state_t, struct stmt,
// and constants like AX_ATOM, X_ATOM, A_ATOM, NOP, and the
// function prototype for deadstmt.
extern "C" {
}

// Lightweight assertion helpers
static inline void tmsg(const char* msg) {
    fprintf(stderr, "TEST: %s\n", msg);
}

// Simple assertion macro (non-terminating: prints fail but continues)
#define ASSERT(cond, msg) do { \
    if (!(cond)) { tmsg(msg); return false; } \
} while(0)


// Helper to create a new stmt node with a given initial code
static struct stmt* new_stmt(int init_code) {
    struct stmt* s = (struct stmt*)malloc(sizeof(struct stmt));
    if (!s) {
        tmsg("Memory allocation failed for stmt");
        return nullptr;
    }
    // Initialize with default fields; the real project may have more fields,
    // but deadstmt only touches 'code' in the testable paths.
    memset(s, 0, sizeof(struct stmt));
    s->code = init_code;
    return s;
}

// Helper to free a stmt (and avoid leaks in tests)
static void free_stmt(struct stmt* s) {
    if (s) free(s);
}


// Test 1: AX_ATOM path with non-NULL last[AX_ATOM] to verify
// - last[X_ATOM] and last[A_ATOM] are cleared (set to NULL)
// - last[AX_ATOM] is updated to point to s
// - The pre-existing last[AX_ATOM] node receives code = NOP (demonstrating
//   the immediate "NOP" write when atom == AX_ATOM and there is a prior
//   non-NULL last[AX_ATOM] through the second phase).
static bool test_deadstmt_AX_ATOM_clears_and_NOP() {
    tmsg("Test 1: AX_ATOM path clears last[X_ATOM]/last[A_ATOM] and marks NOP on prior AX_ATOM");

    // Prepare state
    opt_state_t state;
    memset(&state, 0, sizeof(state));
    state.done = 1;
    state.non_branch_movement_performed = 0;

    // Prepare s
    struct stmt* s = new_stmt(0);
    if (!s) return false;

    // Prepare last array with indices we will use.
    // We assume the project defines AX_ATOM, X_ATOM, A_ATOM as valid indices.
    const int LAST_SIZE = 32; // ample to cover atom indices
    struct stmt* last[LAST_SIZE];
    for (int i = 0; i < LAST_SIZE; ++i) last[i] = nullptr;

    // Create sentinel for AX_ATOM that will be overwritten by the test.
    struct stmt* sentinel_AX = new_stmt(-999); // arbitrary initial code
    if (!sentinel_AX) { free_stmt(s); return false; }

    // Pre-load last array to reflect a non-NULL AX_ATOM case
    last[AX_ATOM] = sentinel_AX;
    // Also provide non-NULL last for the X and A atoms to ensure they are cleared
    last[X_ATOM] = new_stmt(-1);
    last[A_ATOM] = new_stmt(-2);
    if (!last[X_ATOM] || !last[A_ATOM]) {
        free_stmt(s);
        free_stmt(sentinel_AX);
        return false;
    }

    // Call the function under test
    deadstmt(&state, s, last);

    // Assertions:
    // - last[X_ATOM] and last[A_ATOM] should be cleared (NULL)
    ASSERT(last[X_ATOM] == nullptr, "deadstmt AX_ATOM: last[X_ATOM] should be NULL");
    ASSERT(last[A_ATOM] == nullptr, "deadstmt AX_ATOM: last[A_ATOM] should be NULL");

    // - last[AX_ATOM] should point to s
    ASSERT(last[AX_ATOM] == s, "deadstmt AX_ATOM: last[AX_ATOM] should be updated to s");

    // - The pre-existing sentinel_AX should have had its code set to NOP
    ASSERT(sentinel_AX->code == NOP, "deadstmt AX_ATOM: sentinel AX atom's code should be set to NOP");

    // Cleanup
    free_stmt(s);
    free_stmt(sentinel_AX);
    free_stmt(last[X_ATOM]);
    free_stmt(last[A_ATOM]);
    return true;
}


// Test 2: AX_ATOM path with NULL last[AX_ATOM] to verify
// - only last[AX_ATOM] is updated to s; no NOP mutation performed
static bool test_deadstmt_AX_ATOM_NULL_last() {
    tmsg("Test 2: AX_ATOM path with NULL last[AX_ATOM] should set last[AX_ATOM] to s and not touch NOP on a prior AX entry (since none existed)");

    opt_state_t state;
    memset(&state, 0, sizeof(state));

    struct stmt* s = new_stmt(0);
    if (!s) return false;

    const int LAST_SIZE = 32;
    struct stmt* last[LAST_SIZE];
    for (int i = 0; i < LAST_SIZE; ++i) last[i] = nullptr;

    // Ensure there is a non-NULL AX_ATOM before, but here we specifically set
    // it to NULL to isolate behavior when there is no prior AX_ATOM holder.
    last[AX_ATOM] = nullptr;
    // Other atoms can be non-NULL to ensure they are not unexpectedly affected
    last[X_ATOM] = new_stmt(-5);
    last[A_ATOM] = new_stmt(-6);
    if (!last[X_ATOM] || !last[A_ATOM]) {
        free_stmt(s);
        return false;
    }

    // Call the function under test
    deadstmt(&state, s, last);

    // Assertions:
    // - last[AX_ATOM] should be updated to point to s
    ASSERT(last[AX_ATOM] == s, "deadstmt AX_ATOM NULL: last[AX_ATOM] should be updated to s");

    // Since there was no prior AX_ATOM, there should be no NOP mutation
    // of any prior AX_ATOM entry (there was none). We can still verify that
    // the previously allocated last[X_ATOM] and last[A_ATOM] retain their codes.
    // They should remain unchanged because the code path for AX_ATOM should not affect them.
    ASSERT(last[X_ATOM]->code == -5, "deadstmt AX_ATOM NULL: last[X_ATOM] should remain unchanged");
    ASSERT(last[A_ATOM]->code == -6, "deadstmt AX_ATOM NULL: last[A_ATOM] should remain unchanged");

    // Cleanup
    free_stmt(s);
    free_stmt(last[X_ATOM]);
    free_stmt(last[A_ATOM]);
    return true;
}


// Test 3: atomdef path when atomdef(s) returns a valid atom and last[atom] is non-NULL
// This asserts that last[atom]->code is set to NOP and opt_state->done is set to 0,
// and opt_state->non_branch_movement_performed is set to 1, and last[atom] is updated to s.
static bool test_deadstmt_atomdef_sets_NOP_and_flags() {
    tmsg("Test 3: atomdef path updates NOP and optimizer flags when last[atom] is non-NULL");

    opt_state_t state;
    memset(&state, 0, sizeof(state));
    state.done = 1;
    state.non_branch_movement_performed = 0;

    // Prepare s
    struct stmt* s = new_stmt(0);
    if (!s) return false;

    // Prepare last array
    const int LAST_SIZE = 32;
    struct stmt* last[LAST_SIZE];
    for (int i = 0; i < LAST_SIZE; ++i) last[i] = nullptr;

    // Choose an atom value that will be used in both phases:
    // - atomuse(s) should yield a non-AX_ATOM value (we'll try to hit atomdef path)
    // - atomdef(s) should return the same atom value to exercise the "if (last[atom])" branch
    // We set up an initial non-NULL last[ATOM] to be manipulated by the second phase.
    int target_atom = 3; // arbitrary within bounds; assume within LAST_SIZE
    if (target_atom >= LAST_SIZE) {
        target_atom = AX_ATOM; // fallback if bound differs
    }

    last[target_atom] = new_stmt(111); // a non-NULL node that should become NOP

    // Also ensure the AX_ATOM path does not interfere; keep it NULL for this test
    last[AX_ATOM] = nullptr;

    // We expect during deadstmt: atomuse(s) -> some non-AX_ATOM (equal to target_atom),
    // so last[target_atom] is set to 0, then atomdef(s) -> returns the same target_atom and
    // last[target_atom] is non-NULL (the sentinel), so its code becomes NOP.
    // Finally, last[target_atom] becomes s.

    deadstmt(&state, s, last);

    // Assertions
    ASSERT(state.done == 0, "deadstmt atomdef path: opt_state->done should be 0");
    ASSERT(state.non_branch_movement_performed == 1, "deadstmt atomdef path: non_branch_movement_performed should be 1");
    ASSERT(last[target_atom] == s, "deadstmt atomdef path: last[target_atom] should be updated to s");
    ASSERT((last[target_atom]->code) == NOP, "deadstmt atomdef path: target_atom node's code should be set to NOP");

    // Cleanup
    free_stmt(s);
    free_stmt(last[target_atom]);
    return true;
}


// Test harness: run all tests and report summary
int main() {
    int tests_passed = 0;
    int tests_total = 0;

#define RUN_TEST(T) do { \
    ++tests_total; \
    if (T()) { \
        ++tests_passed; \
        printf("[OK]  %s\n", #T); \
    } else { \
        printf("[FAIL] %s\n", #T); \
    } \
} while(0)

    // Run individual tests
    RUN_TEST(test_deadstmt_AX_ATOM_clears_and_NOP);
    RUN_TEST(test_deadstmt_AX_ATOM_NULL_last);
    RUN_TEST(test_deadstmt_atomdef_sets_NOP_and_flags);

    // Summary
    printf("Summary: %d/%d tests passed.\n", tests_passed, tests_total);
    return (tests_passed == tests_total) ? 0 : 1;
}