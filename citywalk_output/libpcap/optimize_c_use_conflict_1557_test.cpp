// C++11 test suite for the focal C function: use_conflict(struct block *b, struct block *succ)
// This test harness uses a lightweight, non-terminating assertion approach (EXPECT_* macros)
// to verify both true and false branches of the predicate inside use_conflict.
// The tests rely on project-provided types/macros (struct block, N_ATOMS, ATOMELEM, etc.)
// and link against the existing optimize.c implementation.
// Explanatory comments are provided for each test case.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <cstdint>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <cstdio>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <pcap-types.h>


// Include the project's optimize header to obtain the correct type definitions
// and the prototype for use_conflict. If your project uses a different header name,
// adjust accordingly (e.g., #include "optimize.h").
extern "C" {
}

// Lightweight test harness: non-terminating EXPECT_* assertions
static int test_failures = 0;

#define LOG_FAIL(msg) \
    do { fprintf(stderr, "TEST FAILURE: %s:%d: %s\n", __FILE__, __LINE__, msg); ++test_failures; } while(0)

#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { LOG_FAIL("EXPECT_TRUE failed: " #cond); } } while(0)

#define EXPECT_FALSE(cond) \
    do { if ((cond)) { LOG_FAIL("EXPECT_FALSE failed: " #cond); } } while(0)

#define EXPECT_EQ(a, b) \
    do { auto _a = (a); auto _b = (b); if (!(_a == _b)) { fprintf(stderr, "TEST FAILURE: EXPECT_EQ failed: %s == %s (%lld != %lld)\n", #a, #b, (long long)_a, (long long)_b); ++test_failures; } } while(0)

#define EXPECT_NEQ(a, b) \
    do { auto _a = (a); auto _b = (b); if (_a == _b) { fprintf(stderr, "TEST FAILURE: EXPECT_NEQ failed: %s != %s (both are %lld)\n", #a, #b, (long long)_a); ++test_failures; } } while(0)

// Helper: initialize both blocks with zeroed values for all atoms
static void init_blocks_zero(struct block* b, struct block* succ)
{
    // Assumes N_ATOMS is defined by the project headers
    for (int i = 0; i < N_ATOMS; ++i) {
        b->val[i] = 0;
        succ->val[i] = 0;
    }
    // out_use for succ is left to be set explicitly by each test
}

// Test 1: When succ->out_use is 0, use_conflict should return 0 (no conflict check executed)
static void test_use_conflict_no_usage()
{
    // Setup
    struct block b;
    struct block succ;
    init_blocks_zero(&b, &succ);
    succ.out_use = 0; // no atoms used by successor

    // Exercise
    int res = use_conflict(&b, &succ);

    // Verify
    EXPECT_EQ(res, 0);
}

// Test 2: No conflict when all atoms in use have identical values in b and succ
static void test_use_conflict_all_equal_for_used_atoms()
{
    // Setup
    struct block b;
    struct block succ;
    init_blocks_zero(&b, &succ);

    // Choose a nontrivial set of atoms to consider as "used"
    // Example: use atoms 0, 2, and 3 (bitmask depends on N_ATOMS; using typical bit operations)
    succ.out_use = 0;
    succ.out_use |= (1u << 0);
    succ.out_use |= (1u << 2);
    succ.out_use |= (1u << 3);

    // Set equal values for all atoms that are marked as used
    for (int i = 0; i < N_ATOMS; ++i) {
        b.val[i] = i;
        succ.val[i] = i;
    }

    // Exercise
    int res = use_conflict(&b, &succ);

    // Verify: since used atoms have identical values, there is no conflict
    EXPECT_EQ(res, 0);
}

// Test 3: Conflict arises when at least one used atom has differing values between b and succ
static void test_use_conflict_conflict_due_to_diff_in_used_atom()
{
    // Setup
    struct block b;
    struct block succ;
    init_blocks_zero(&b, &succ);

    // Use a couple of atoms
    succ.out_use = 0;
    succ.out_use |= (1u << 1);
    succ.out_use |= (1u << 4);

    // Set equal values for most atoms, but deliberately differ for one used atom
    for (int i = 0; i < N_ATOMS; ++i) {
        b.val[i] = i;
        succ.val[i] = i;
    }
    // Introduce a conflict on atom 1 (which is in use)
    b.val[1] = 9999;       // different from succ.val[1] which is 1
    // Ensure succ.val[1] remains 1

    // Exercise
    int res = use_conflict(&b, &succ);

    // Verify: conflict detected due to differing values on a used atom
    EXPECT_EQ(res, 1);
}

// Test 4: Multiple used atoms with all equal values should still yield no conflict
static void test_use_conflict_multiple_used_all_equal()
{
    // Setup
    struct block b;
    struct block succ;
    init_blocks_zero(&b, &succ);

    // Use a larger set of atoms
    succ.out_use = 0;
    for (int atom = 0; atom < 4; ++atom) {
        succ.out_use |= (1u << atom);
    }

    // Equal values for all atoms
    for (int i = 0; i < N_ATOMS; ++i) {
        b.val[i] = i;
        succ.val[i] = i;
    }

    // Exercise
    int res = use_conflict(&b, &succ);

    // Verify
    EXPECT_EQ(res, 0);
}

// Entry point for the test suite
int main()
{
    // Run all tests; each test uses the same shared harness and prints its own notes
    printf("Starting use_conflict unit tests...\n");

    test_use_conflict_no_usage();
    test_use_conflict_all_equal_for_used_atoms();
    test_use_conflict_conflict_due_to_diff_in_used_atom();
    test_use_conflict_multiple_used_all_equal();

    if (test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILURES: %d\n", test_failures);
        return 1;
    }
}