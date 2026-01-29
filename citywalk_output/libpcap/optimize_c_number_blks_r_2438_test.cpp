// Unit test suite for the focal method: number_blks_r
// Target environment: C++11, no GTest, self-contained minimal test harness.
// The tests exercise number_blks_r defined in optimize.c, using a lightweight
// mock/bridge for required callbacks (isMarked, Mark, opt_error) and
// a minimal representation of the related types via optimize.h.

#include <gencode.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <optimize.h>
#include <setjmp.h>
#include <set>
#include <diag-control.h>
#include <vector>
#include <limits.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <pcap-types.h>
#include <string.h>
#include <memory.h>
#include <os-proto.h>
#include <cstdlib>
#include <cstring>


// Include the project's optimize header to pick up the real type definitions.
// This is important to align with the actual struct block, struct icode, and
// opt_state_t used by number_blks_r.

extern "C" {
    // Declaration of the focal function (as it would appear in a C file).
    void number_blks_r(opt_state_t *opt_state, struct icode *ic, struct block *p);

    // Prototypes for the test bridge hooks that optimize.c expects to call.
    // We provide C linkage so that the C/C++ linker can bind them correctly.
    int isMarked(struct icode *ic, struct block *p);
    void Mark(struct icode *ic, struct block *p);
    void opt_error(opt_state_t *opt_state, const char *fmt, ...);

    // Helper to reset all marks between tests (exposed for test control).
    void reset_marks();
}

/*
  Bridge/Mock state for marking blocks during tests.
  We don't rely on any internal fields of struct block (to remain stable with
  the real optimize.h layout). Instead, we maintain an external registry of
  marked block pointers. isMarked/Mark consult this registry.
*/
static std::set<void*> g_marked_ptrs;

extern "C" void reset_marks() {
    g_marked_ptrs.clear();
}

// isMarked(ic, p) - consults the external registry to see if p has been marked.
extern "C" int isMarked(struct icode *ic, struct block *p) {
    (void)ic; // ic is not used in this mock
    if (p == nullptr) return 0;
    return g_marked_ptrs.find((void*)p) != g_marked_ptrs.end() ? 1 : 0;
}

// Mark(ic, p) - add p to the external registry of marked blocks.
extern "C" void Mark(struct icode *ic, struct block *p) {
    (void)ic; // ic is not used in this mock
    if (p) g_marked_ptrs.insert((void*)p);
}

// opt_error(...) - a no-op stub suitable for tests.
// The real optimizer would log an error; for tests we simply expose that the
// function was invoked (by not crashing) and keep test flow going.
extern "C" void opt_error(opt_state_t *opt_state, const char *fmt, ...) {
    (void)opt_state;
    (void)fmt;
    // Intentionally empty to avoid side effects during unit tests.
    // In a more elaborate test, we could capture the message for verification.
}

// Helper macro for test results
#define TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { std::cerr << "Test failed: " msg "\n"; return false; } } while (0)


// Helper: create a block instance using the project's actual struct layout.
// We assume sizeof(struct block) is available via optimize.h and that the
// structure is zero-initializable in tests.
static struct block* make_block() {
    struct block* b = (struct block*) std::malloc(sizeof(struct block));
    if (!b) {
        std::cerr << "Out of memory creating block\n";
        std::exit(1);
    }
    // Initialize commonly accessed fields to safe defaults.
    // We rely on number_blks_r to set p->id; other fields are not touched in our tests.
    std::memset(b, 0, sizeof(struct block));
    // Do not rely on any internal fields beyond those used by number_blks_r
    // and the JT/JF macros (which rely on b->jt and b->jf).
    return b;
}

// Helper: initialize opt_state for tests
static void init_opt_state(opt_state_t *opt, size_t blocks_capacity, struct block **blocks_arr) {
    opt->n_blocks = 0;
    opt->blocks = blocks_arr;
    // We won't rely on other members of opt_state_t for these tests.
}

int main() {
    // Basic, self-contained unit tests for number_blks_r
    // Each test prints a short description and returns non-zero on failure.

    // Test 1: When p is NULL, function should return immediately and not mutate state.
    {
        std::cout << "Test 1: NULL pointer p yields no changes\n";
        // Prepare an optimizer state with a capacity for 4 blocks
        const size_t CAP = 4;
        struct block **blocks = (struct block **) std::malloc(CAP * sizeof(struct block*));
        for (size_t i = 0; i < CAP; ++i) blocks[i] = nullptr;
        opt_state_t opt;
        init_opt_state(&opt, CAP, blocks);

        struct icode ic;
        // Ensure ic is zeroed; content is ignored by our mocks
        std::memset(&ic, 0, sizeof(ic));

        // Case A: p is NULL
        number_blks_r(&opt, &ic, NULL);
        TEST_ASSERT(opt.n_blocks == 0, "n_blocks should remain 0 when p is NULL");
        // Case B: p non-NULL but marked (simulate with a block that will be ignored)
        struct block *p = make_block();
        // Do not mark in Case B; we want to test the "false" branch (p != 0) but isMarked==true
        // Mark using our bridge
        Mark(&ic, p);
        // p should stay unchanged (no new blocks, and p's id should be untouched)
        unsigned int old_id = p->id;
        number_blks_r(&opt, &ic, p);
        TEST_ASSERT(p->id == old_id, "Block id should remain unchanged when block is marked (early return)");
        // Cleanup
        reset_marks();
        std::free(p);
        std::free(blocks);
        std::cout << "Test 1 passed\n";
    }

    // Test 2: Simple DAG with 3 blocks visited in JT/JF order
    {
        std::cout << "Test 2: Simple 3-block graph (0 -> 1, 0 -> 2)\n";
        const size_t CAP = 3;
        struct block **blocks = (struct block **) std::malloc(CAP * sizeof(struct block*));
        for (size_t i = 0; i < CAP; ++i) blocks[i] = make_block();
        // Connect blocks: p0 -> p1 (JT), p0 -> p2 (JF); p1 and p2 are leaves
        blocks[0]->jt = blocks[1];
        blocks[0]->jf = blocks[2];
        blocks[1]->jt = nullptr;
        blocks[1]->jf = nullptr;
        blocks[2]->jt = nullptr;
        blocks[2]->jf = nullptr;

        opt_state_t opt;
        init_opt_state(&opt, CAP, blocks);
        struct icode ic;
        std::memset(&ic, 0, sizeof(ic));

        reset_marks();
        number_blks_r(&opt, &ic, blocks[0]);

        // Expectations:
        // blocks[0] id = 0, blocks[1] id = 1, blocks[2] id = 2
        TEST_ASSERT(blocks[0]->id == 0, "Block 0 should get id 0");
        TEST_ASSERT(blocks[1]->id == 1, "Block 1 should get id 1");
        TEST_ASSERT(blocks[2]->id == 2, "Block 2 should get id 2");
        TEST_ASSERT(opt.n_blocks == 3, "Total blocks counted should be 3");
        TEST_ASSERT(opt.blocks[0] == blocks[0], "opt_state.blocks[0] should point to block 0");
        TEST_ASSERT(opt.blocks[1] == blocks[1], "opt_state.blocks[1] should point to block 1");
        TEST_ASSERT(opt.blocks[2] == blocks[2], "opt_state.blocks[2] should point to block 2");

        // Cleanup
        reset_marks();
        for (size_t i = 0; i < CAP; ++i) std::free(blocks[i]);
        std::free(blocks);
        std::cout << "Test 2 passed\n";
    }

    // Test 3: Graph with a cycle to validate Mark/isMarked prevents infinite recursion
    {
        std::cout << "Test 3: Cycle detection (p0 -> p1, p1 -> p0)\n";
        const size_t CAP = 2;
        struct block **blocks = (struct block **) std::malloc(CAP * sizeof(struct block*));
        for (size_t i = 0; i < CAP; ++i) blocks[i] = make_block();
        // Create cycle: p0.jt = p1; p1.jt = p0; both no false-edges
        blocks[0]->jt = blocks[1];
        blocks[0]->jf = nullptr;
        blocks[1]->jt = blocks[0];
        blocks[1]->jf = nullptr;

        opt_state_t opt;
        init_opt_state(&opt, CAP, blocks);
        struct icode ic;
        std::memset(&ic, 0, sizeof(ic));

        reset_marks();
        number_blks_r(&opt, &ic, blocks[0]);

        // Expectations: both blocks visited once
        TEST_ASSERT(blocks[0]->id == 0, "Block 0 should get id 0 in cycle case");
        TEST_ASSERT(blocks[1]->id == 1, "Block 1 should get id 1 in cycle case");
        TEST_ASSERT(opt.n_blocks == 2, "Total blocks counted should be 2 in cycle case");

        // Cleanup
        reset_marks();
        for (size_t i = 0; i < CAP; ++i) std::free(blocks[i]);
        std::free(blocks);
        std::cout << "Test 3 passed\n";
    }

    // Test 4: Ensure non-null p with no children yields a single block entry
    {
        std::cout << "Test 4: Single block (no children)\n";
        const size_t CAP = 1;
        struct block **blocks = (struct block **) std::malloc(CAP * sizeof(struct block*));
        blocks[0] = make_block();

        opt_state_t opt;
        init_opt_state(&opt, CAP, blocks);
        struct icode ic;
        std::memset(&ic, 0, sizeof(ic));

        reset_marks();
        number_blks_r(&opt, &ic, blocks[0]);

        TEST_ASSERT(blocks[0]->id == 0, "Single block should receive id 0");
        TEST_ASSERT(opt.n_blocks == 1, "Total blocks counted should be 1 for a single node");

        // Cleanup
        reset_marks();
        std::free(blocks[0]);
        std::free(blocks);
        std::cout << "Test 4 passed\n";
    }

    std::cout << "All tests executed.\n";
    return 0;
}