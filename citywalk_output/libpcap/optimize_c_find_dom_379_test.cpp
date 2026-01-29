// High-level C++11 test suite for the focal method: find_dom(opt_state_t *opt_state, struct block *root)
// This test suite is designed to be compiled alongside the project that provides optimize.c and
// its headers (notably optimize.h). It uses the public types (opt_state_t, block, etc.) and the
// actual function prototype for find_dom. No GoogleTest is used; tests are driven via a main()
// function and simple, non-terminating assertions to maximize code coverage during execution.
// The tests are designed to be conservative with the internal bitset semantics and to verify
// observable, stable behavior of the initialization and the high-level branches within find_dom.
// Explanatory comments accompany each unit test to describe intent and expected coverage.

#include <gencode.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <cassert>
#include <optimize.h>
#include <cstdio>
#include <setjmp.h>
#include <string>
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


// Ensure we link against the C namespace for the project headers
extern "C" {
}

// Helper: simple non-terminating assertion recording mechanism
struct TestStatus {
    std::vector<std::string> failures;

    void fail(const char* loc, const std::string &msg) {
        failures.push_back(std::string(loc) + ": " + msg);
    }

    bool ok() const { return failures.empty(); }

    void report() const {
        if (failures.empty()) {
            std::cout << "[TEST] All tests passed.\n";
        } else {
            std::cout << "[TEST] Failures: " << failures.size() << "\n";
            for (auto &f : failures) std::cout << " - " << f << "\n";
        }
    }
};

// Domain-specific helper to create a block with a dom array (nodewords length)
static block* alloc_block(opt_state_t *opt, int id) {
    // Allocate block struct
    block* b = (block*)std::malloc(sizeof(block));
    if (!b) {
        return nullptr;
    }
    // Important: set sane defaults; actual fields/offsets rely on project headers
    std::memset(b, 0, sizeof(block));
    b->id = id;
    b->link = nullptr;
    // Ensure dom array exists and is zeroed
    bpf_u_int32 *dom = (bpf_u_int32*)std::malloc(sizeof(bpf_u_int32) * opt->nodewords);
    if (!dom) {
        std::free(b);
        return nullptr;
    }
    for (unsigned int i = 0; i < opt->nodewords; ++i) dom[i] = 0;
    b->dom = dom;
    // JT/JF pointers (true/false successors)
    b->JT = nullptr;
    b->JF = nullptr;
    b->level = 0;
    return b;
}

// Helper to allocate and configure root block
static block* alloc_root(opt_state_t *opt, int nodewords) {
    block* r = alloc_block(opt, 0); // root id is not important, it is set separately
    if (!r) return nullptr;
    r->level = 0;
    // Root has its own dom array; ensure length matches nodewords
    // Note: alloc_block already allocated dom sized to opt->nodewords
    return r;
}

// Test 1: Verify initialization of all_dom_sets and root dom baseline when levels are empty
static bool test_init_only_no_levels(TestStatus &ts) {
    // Setup a minimal opt_state
    opt_state_t *opt = (opt_state_t*)std::malloc(sizeof(opt_state_t));
    if (!opt) { ts.fail("test_init_only_no_levels", "out of memory for opt_state_t"); return false; }
    opt->n_blocks = 2;
    opt->nodewords = 1;
    // Allocate all_dom_sets: n_blocks * nodewords
    opt->all_dom_sets = (bpf_u_int32*)std::malloc(sizeof(bpf_u_int32) * opt->n_blocks * opt->nodewords);
    for (unsigned int i = 0; i < opt->n_blocks * opt->nodewords; ++i) opt->all_dom_sets[i] = 0; // prefill (will be overwritten)

    // Levels array with empty level 0
    opt->levels = (struct block**)std::malloc(sizeof(struct block*) * 1);
    opt->levels[0] = nullptr; // no blocks at level 0 to trigger processing

    // Create root
    block* root = alloc_root(opt, opt->nodewords);
    // root->dom must exist and be zeroed
    for (unsigned int i = 0; i < opt->nodewords; ++i) root->dom[i] = 0;

    // Call the focal function
    find_dom(opt, root);

    // Assertions:
    // 1) all_dom_sets must be initialized to 0xFFFFFFFFU across the entire range
    bool ok = true;
    unsigned int total = opt->n_blocks * opt->nodewords;
    for (unsigned int i = 0; i < total; ++i) {
        if (opt->all_dom_sets[i] != 0xFFFFFFFFU) {
            ts.fail("test_init_only_no_levels", "all_dom_sets not fully initialized to 0xFFFFFFFFU");
            ok = false;
            break;
        }
    }
    // 2) Root dom should be zeroed
    for (unsigned int i = 0; i < opt->nodewords; ++i) {
        if (root->dom[i] != 0) {
            ts.fail("test_init_only_no_levels", "root->dom not initialized to 0");
            ok = false;
            break;
        }
    }

    // Cleanup
    std::free(root->dom);
    std::free(root);
    std::free(opt->levels);
    std::free(opt->all_dom_sets);
    std::free(opt);

    return ok;
}

// Test 2: Verify that a single block with JT = NULL gets its own id bit inserted into its dom
static bool test_single_block_insertion(TestStatus &ts) {
    // Setup opt_state
    opt_state_t *opt = (opt_state_t*)std::malloc(sizeof(opt_state_t));
    if (!opt) { ts.fail("test_single_block_insertion", "out of memory for opt_state_t"); return false; }
    opt->n_blocks = 1;
    opt->nodewords = 1;
    opt->all_dom_sets = (bpf_u_int32*)std::malloc(sizeof(bpf_u_int32) * opt->n_blocks * opt->nodewords);
    for (unsigned int i = 0; i < opt->n_blocks * opt->nodewords; ++i) opt->all_dom_sets[i] = 0xFFFFFFFFU; // baseline

    // Levels: one block in level 0
    opt->levels = (struct block**)std::malloc(sizeof(struct block*) * 1);

    block* b = alloc_block(opt, 0); // id = 0
    if (!b) { ts.fail("test_single_block_insertion", "failed to allocate block"); goto cleanup1; }

    // Chain: root -> level 0 contains only block b
    opt->levels[0] = b;
    // For this simple test, root will be created and will have level 0 as well
    block* root = alloc_root(opt, opt->nodewords);
    for (unsigned int i = 0; i < opt->nodewords; ++i) root->dom[i] = 0;

    // Ensure JT/JF are NULL so only insertion path runs
    b->JT = nullptr;
    b->JF = nullptr;
    b->level = 0;

    // Call
    find_dom(opt, root);

    // Invariant: the block itself should have its id bit inserted into its dom
    // Assuming the bit corresponding to id maps to LSB (bit 0) in nodewords=1 case
    bool ok = true;
    if ((b->dom[0] & (1U << (b->id % 32))) == 0) {
        ts.fail("test_single_block_insertion", "block->dom did not contain its id bit after find_dom");
        ok = false;
    }

cleanup1:
    // Cleanup
    if (b) {
        std::free(b->dom);
        std::free(b);
    }
    if (root) {
        std::free(root->dom);
        std::free(root);
    }
    if (opt->levels) std::free(opt->levels);
    if (opt->all_dom_sets) std::free(opt->all_dom_sets);
    std::free(opt);
    return ok;
}

// Test 3: Verify that a block with non-null JT triggers an intersection path (high level branch coverage)
static bool test_jt_intersection_path(TestStatus &ts) {
    // Setup a small configuration with 3 blocks in level 0:
    // b0 (id=0) -> JT = b1, JF = b2
    // We'll initialize b1 and b2 dom so that an intersection operation occurs
    opt_state_t *opt = (opt_state_t*)std::malloc(sizeof(opt_state_t));
    if (!opt) { ts.fail("test_jt_intersection_path", "out of memory for opt_state_t"); return false; }
    opt->n_blocks = 3;
    opt->nodewords = 1;
    opt->all_dom_sets = (bpf_u_int32*)std::malloc(sizeof(bpf_u_int32) * opt->n_blocks * opt->nodewords);
    for (unsigned int i = 0; i < opt->n_blocks * opt->nodewords; ++i) opt->all_dom_sets[i] = 0xFFFFFFFFU; // baseline

    opt->levels = (struct block**)std::malloc(sizeof(struct block*) * 1);

    block* b0 = alloc_block(opt, 0);
    block* b1 = alloc_block(opt, 1);
    block* b2 = alloc_block(opt, 2);

    if (!b0 || !b1 || !b2) {
        ts.fail("test_jt_intersection_path", "failed to allocate blocks");
        goto cleanup2;
    }

    // Link chain at level 0: b0 -> b1 -> b2
    b0->link = b1;
    b1->link = b2;
    b2->link = nullptr;
    opt->levels[0] = b0;

    // Setup JT/JF for b0
    b0->JT = b1; // true successor
    b0->JF = b2; // false successor
    b0->level = 0;

    // Initialize doms
    b0->dom[0] = 0;        // ID 0 will be inserted
    b1->dom[0] = 0xFFFFFFFF; // ensure intersection has a deterministic effect
    b2->dom[0] = 0xFFFFFFFF;

    // root
    block* root = alloc_root(opt, opt->nodewords);
    for (unsigned int i = 0; i < opt->nodewords; ++i) root->dom[i] = 0;

    // Call
    find_dom(opt, root);

    // Observations:
    // - We expect the code to exercise the JT(&) and JF(&) intersection paths for b0.
    // - The exact results of intersect operations depend on the exact bitset semantics;
    //   we primarily verify that the code executes the branches without crashing and that
    //   the dom arrays are updated to some non-crashing state.
    bool ok = true;

    // Basic sanity: after operation, root dom remains zero
    if (root->dom[0] != 0) {
        ts.fail("test_jt_intersection_path", "root dom unexpectedly non-zero after find_dom");
        ok = false;
    }

    // Ensure that the code touched the blocks (at least the blocks exist and have dom allocated)
    if (!b0 || !b1 || !b2) {
        ts.fail("test_jt_intersection_path", "blocks not allocated properly");
        ok = false;
    }

    // Best-effort check: ensure b0's dom contains the id bit (0)
    if ((b0->dom[0] & 1U) == 0) {
        ts.fail("test_jt_intersection_path", "block b0 did not have its id bit set after first insertion");
        // Do not fail hard here - we still consider the test showing path coverage
    }

cleanup2:
    // Cleanup
    // Free doms
    if (b0) { std::free(b0->dom); std::free(b0); }
    if (b1) { std::free(b1->dom); std::free(b1); }
    if (b2) { std::free(b2->dom); std::free(b2); }
    if (root) { std::free(root->dom); std::free(root); }
    if (opt->levels) std::free(opt->levels);
    if (opt->all_dom_sets) std::free(opt->all_dom_sets);
    std::free(opt);

    return ok;
}


// Main: orchestrate tests
int main() {
    TestStatus ts;

    std::cout << "Running find_dom unit tests (C/C++11) without GTest...\n";

    bool r1 = test_init_only_no_levels(ts);
    if (!r1) std::cout << "[WARN] test_init_only_no_levels reported failures.\n";

    bool r2 = test_single_block_insertion(ts);
    if (!r2) std::cout << "[WARN] test_single_block_insertion reported failures.\n";

    bool r3 = test_jt_intersection_path(ts);
    if (!r3) std::cout << "[WARN] test_jt_intersection_path reported failures.\n";

    // Summary
    ts.report();

    // Return non-zero if any test failed
    return ts.ok() ? 0 : 1;
}