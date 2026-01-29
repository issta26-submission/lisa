// test_find_ud.cpp
// A lightweight, non-GTest C++11 test suite for the focal method:
//      find_ud(opt_state_t *opt_state, struct block *root)
// Notes:
// - This test relies on the project's actual optimize.h definitions for
//   struct block, opt_state_t, and the JT/JF macros.
// - We can't rely on private/internal functions or static file-scope state.
// - The tests are written to be minimally invasive and use a simple
//   assertion framework (no termination on first failure) to maximize
//   code coverage during execution.
// - The test focuses on exercising the two loops and their branches by
//   constructing small graphs of blocks with controllable in_use/out_use/kill
//   values and the true/false edges (JT/JF).
// - If the underlying structs or macros differ in your environment, adjust the
//   field names accordingly (fields used here follow common patterns from
//   the original code: level, link, in_use, out_use, kill, JT, JF).

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
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <pcap-types.h>


// Include the focal API header to obtain the real types and macros.
// The test harness is designed to work with the actual project headers.
// If your project uses a different include path, adjust accordingly.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
#define TEST_ASSERT(cond, msg)                                        \
    do {                                                              \
        if (!(cond)) {                                                \
            std::cerr << "[TEST FAILED] " << (msg) << "\n";          \
            return false;                                           \
        }                                                             \
    } while (0)

#define TEST_ASSERT_EQ(a, b, msg)                                       \
    do {                                                                  \
        if (!((a) == (b))) {                                            \
            std::cerr << "[TEST FAILED] " << (msg)                              \
                      << " | Expected: " << (b) << " Got: " << (a) << "\n"; \
            return false;                                               \
        }                                                             \
    } while (0)

// Helper function to create a new block (wrapper around malloc with zero-init)
static struct block* new_block() {
    struct block* b = (struct block*)std::malloc(sizeof(struct block));
    if (!b) {
        std::cerr << "[ERROR] malloc failed for block\n";
        exit(EXIT_FAILURE);
    }
    std::memset(b, 0, sizeof(struct block));
    // We rely on the actual optimize.h initialization semantics; zeroing is safe.
    return b;
}

// Helper function to create a new opt_state_t wrapper
static opt_state_t* new_opt_state(int levels_count) {
    opt_state_t* os = (opt_state_t*)std::malloc(sizeof(opt_state_t));
    if (!os) {
        std::cerr << "[ERROR] malloc failed for opt_state_t\n";
        exit(EXIT_FAILURE);
    }
    // Assume the real structure has a member named levels which is an array of pointers
    // to struct block (struct block **levels). We allocate accordingly.
    os->levels = (struct block**)std::malloc((levels_count) * sizeof(struct block*));
    // Initialize to NULL
    for (int i = 0; i < levels_count; ++i) {
        os->levels[i] = nullptr;
    }
    return os;
}

// Test 1: Empty graph across all levels - ensure no crashes and proper no-op behavior
static bool test_find_ud_empty_graph() {
    // Root at some max level (e.g., 1) but with no blocks in any level.
    struct block* root = new_block();
    root->level = 1;
    // Build opt_state with 2 levels (0..1) but both empty
    opt_state_t* opt_state = new_opt_state(2);

    // Call the function under test
    find_ud(opt_state, root);

    // Post-conditions: Nothing should crash and no blocks should be modified.
    // Since there are no blocks, we can't assert much; we verify the pointers remain NULL.
    TEST_ASSERT(opt_state->levels[0] == nullptr, "Level 0 should be empty");
    TEST_ASSERT(opt_state->levels[1] == nullptr, "Level 1 should be empty");
    // Clean up
    std::free(opt_state->levels);
    std::free(opt_state);
    std::free(root);

    return true;
}

// Test 2: Single block only at level 0; verify first loop clears out_use to 0
// and that no crash occurs when maxlevel == 0
static bool test_find_ud_single_level0_block() {
    // Root at level 0
    struct block* root = new_block();
    root->level = 0;

    // Prepare opt_state with one level (level 0) containing a single block
    opt_state_t* opt_state = new_opt_state(1);
    struct block* p = new_block();
    p->link = nullptr;
    // Ensure some pre-state that should be overwritten by the function
    p->out_use = 12345;
    p->in_use = 2;
    p->kill = 0;
    // Level 0 head points to p
    opt_state->levels[0] = p;

    // Call the function under test
    find_ud(opt_state, root);

    // Post-conditions:
    // - p->out_use should be reset to 0 by the first loop
    TEST_ASSERT_EQ(p->out_use, 0, "Block at level 0: out_use should be reset to 0 by first loop");
    // - p->in_use should remain unchanged or be consistent with out_use and kill
    // Since there are no level 1 blocks, second loop doesn't run; in_use should be unchanged
    TEST_ASSERT_EQ(p->in_use, 2, "Block at level 0: in_use should remain unchanged without next level");

    // Clean up
    std::free(opt_state->levels);
    std::free(opt_state);
    std::free(p);
    std::free(root);

    return true;
}

// Test 3: Two levels with a block at level 1 referencing true/false successors
// Ensure second loop updates out_use and in_use appropriately
static bool test_find_ud_two_levels_boolean_edges() {
    // Root with maxlevel = 1
    struct block* root = new_block();
    root->level = 1;

    opt_state_t* opt_state = new_opt_state(2);

    // Level 0: a single block (not directly used in the assertion, but to ensure first loop runs)
    struct block* a = new_block();
    a->link = nullptr;
    a->level = 0;
    a->in_use = 0;
    a->out_use = 999; // will be reset to 0 by first loop
    a->kill = 0;
    opt_state->levels[0] = a;

    // Level 1: p (the one we will update)
    struct block* p = new_block();
    p->link = nullptr;
    p->level = 1;
    p->in_use = 0;
    p->out_use = 0;
    p->kill = 0;

    // Create JT(p) and JF(p) as two blocks with in_use values
    struct block* t = new_block();
    t->in_use = 1;      // true branch uses in_use = 1
    t->out_use = 0;
    t->kill = 0;
    struct block* f = new_block();
    f->in_use = 0;      // false branch uses in_use = 0
    f->out_use = 0;
    f->kill = 0;

    // Link edges
    p->JT = t;
    p->JF = f;

    opt_state->levels[1] = p;

    // Call function
    find_ud(opt_state, root);

    // Post-conditions:
    // After second loop, p->out_use should include (t->in_use | f->in_use) = 1 | 0 = 1
    // and p->in_use should incorporate out_use if kill == 0
    TEST_ASSERT_EQ(p->out_use, 1, "Block at level 1: out_use should become 1 based on successors' in_use");
    TEST_ASSERT_EQ(p->in_use, 1, "Block at level 1: in_use should be updated to include out_use when kill==0");

    // Also ensure level 0 block's state remains valid (first loop clears only its out_use)
    TEST_ASSERT_EQ(a->out_use, 0, "Level 0 block: its out_use should be reset to 0 by first loop");

    // Clean up
    std::free(opt_state->levels); // frees array
    std::free(opt_state);
    std::free(a);
    std::free(p);
    std::free(t);
    std::free(f);
    std::free(root);

    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running test_find_ud: (C++11 minimal test harness)\n";

    total++; if (test_find_ud_empty_graph()) { std::cout << "[OK] test_find_ud_empty_graph\n"; ++passed; } else { std::cout << "[FAILED] test_find_ud_empty_graph\n"; }
    total++; if (test_find_ud_single_level0_block()) { std::cout << "[OK] test_find_ud_single_level0_block\n"; ++passed; } else { std::cout << "[FAILED] test_find_ud_single_level0_block\n"; }
    total++; if (test_find_ud_two_levels_boolean_edges()) { std::cout << "[OK] test_find_ud_two_levels_boolean_edges\n"; ++passed; } else { std::cout << "[FAILED] test_find_ud_two_levels_boolean_edges\n"; }

    std::cout << "Passed " << passed << " / " << total << " tests.\n";
    return (passed == total) ? EXIT_SUCCESS : EXIT_FAILURE;
}