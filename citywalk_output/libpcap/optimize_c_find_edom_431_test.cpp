// Unit test suite for the focal method: find_edom(opt_state_t *opt_state, struct block *root)
// This test suite is written in C++11 (without Google Test) and uses a lightweight, non-terminating
// assertion framework. It exercises the C code from optimize.c via the project headers.

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


// Bring the C symbols into C++ so we can call the focal function directly.
extern "C" {
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "[TEST FAILED] " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

#define TEST_PRINT_PROGRESS(msg) do { std::cout << "[TEST] " << (msg) << std::endl; } while (0)

// Helper functions to construct blocks and their edom storage
static struct block* allocate_block_with_edom(int edgewords) {
    struct block* b = (struct block*) std::malloc(sizeof(struct block));
    if (!b) return nullptr;
    std::memset(b, 0, sizeof(struct block)); // clear to avoid uninitialized peephole usage

    // Initialize block linkage and level (caller will set as needed)
    b->link = nullptr;

    // Allocate edom storage for et and ef (each has a pointer 'edom')
    // We rely on the project definition: et.edom and ef.edom are of type uset*
    b->et.edom = (uset*) std::malloc(edgewords * sizeof(uset));
    b->ef.edom = (uset*) std::malloc(edgewords * sizeof(uset));

    // Initialize with a non-zero value to observe zeroing after find_edom
    for (int i = 0; i < edgewords; ++i) {
        if (b->et.edom) b->et.edom[i] = 1;
        if (b->ef.edom) b->ef.edom[i] = 1;
    }
    return b;
}

// Helper to free a block's edom memory safely
static void free_block(struct block* b) {
    if (!b) return;
    if (b->et.edom) std::free(b->et.edom);
    if (b->ef.edom) std::free(b->ef.edom);
    std::free(b);
}

// Test 1: Basic scenario with at least one block in levels (level 1) and a root with edom buffers.
// Verifies:
// - all_edge_sets is filled with 0xFFFFFFFFU as performed by find_edom
// - root->et.edom and root->ef.edom are zeroed by find_edom
// - The function does not crash and handles multiple levels
static void test_find_edom_basic_with_blocks() {
    TEST_PRINT_PROGRESS("Starting test_find_edom_basic_with_blocks");

    // Configuration
    const int edgewords = 2;
    const int n_edges = 2;
    const int max_level = 1; // levels 0 and 1 will be used

    // Allocate and initialize opt_state
    opt_state_t* opt = (opt_state_t*) std::malloc(sizeof(opt_state_t));
    std::memset(opt, 0, sizeof(opt_state_t));
    opt->n_edges = (u_int) n_edges;
    opt->edgewords = (u_int) edgewords;

    // all_edge_sets memory
    opt->all_edge_sets = (uset*) std::malloc(n_edges * edgewords * sizeof(uset));
    // Initialize with non-zero values to observe that the function overwrites them
    for (int i = 0; i < n_edges * edgewords; ++i) {
        opt->all_edge_sets[i] = 0xABCD1234U;
    }

    // levels array: allocate max_level+1 pointers
    opt->levels = (struct block**) std::malloc((max_level + 1) * sizeof(struct block*));
    for (int l = 0; l <= max_level; ++l) opt->levels[l] = nullptr;

    // Root block with level = max_level
    struct block* root = allocate_block_with_edom(edgewords);
    TEST_ASSERT(root != nullptr, "Root block allocation failed");
    root->level = max_level;

    // Block at level 1
    struct block* b1 = allocate_block_with_edom(edgewords);
    TEST_ASSERT(b1 != nullptr, "Block b1 allocation failed");
    b1->level = 1;
    // Link into levels[1]
    opt->levels[1] = b1;

    // Block at level 0 (optional, to exercise multiple levels)
    struct block* b0 = allocate_block_with_edom(edgewords);
    TEST_ASSERT(b0 != nullptr, "Block b0 allocation failed");
    b0->level = 0;
    b0->link = nullptr;
    opt->levels[0] = b0;

    // Initialize root edom to non-zero so memset(0) can be observed
    root->et.edom[0] = 0x12;
    root->et.edom[1] = 0x34;
    root->ef.edom[0] = 0x56;
    root->ef.edom[1] = 0x78;

    // Call the focal function
    find_edom(opt, root);

    // Verify: all_edge_sets have been set to 0xFFFFFFFF
    bool all_ff = true;
    for (int i = 0; i < n_edges * edgewords; ++i) {
        if (opt->all_edge_sets[i] != 0xFFFFFFFFU) {
            all_ff = false;
            break;
        }
    }
    TEST_ASSERT(all_ff, "all_edge_sets should be set to 0xFFFFFFFFU by find_edom");

    // Verify: root edom buffers are zeroed
    bool root_edom_zero = true;
    for (int i = 0; i < edgewords; ++i) {
        if (root->et.edom[i] != 0 || root->ef.edom[i] != 0) {
            root_edom_zero = false;
            break;
        }
    }
    TEST_ASSERT(root_edom_zero, "root->et.edom and root->ef.edom should be zeroed");

    // Cleanup
    free_block(root);
    free_block(b0);
    free_block(b1);
    std::free(opt->levels);
    std::free(opt->all_edge_sets);
    std::free(opt);

    TEST_PRINT_PROGRESS("Finished test_find_edom_basic_with_blocks");
}

// Test 2: Scenario with no blocks (levels[] entries are NULL). Ensures that
// the function still zeroes root edom slices and fills all_edge_sets.
// This covers the false branch where the inner for-loop over blocks is skipped.
static void test_find_edom_no_blocks() {
    TEST_PRINT_PROGRESS("Starting test_find_edom_no_blocks");

    const int edgewords = 3;
    const int n_edges = 2;
    const int max_level = 1;

    opt_state_t* opt = (opt_state_t*) std::malloc(sizeof(opt_state_t));
    std::memset(opt, 0, sizeof(opt_state_t));
    opt->n_edges = (u_int) n_edges;
    opt->edgewords = (u_int) edgewords;

    // all_edge_sets memory
    opt->all_edge_sets = (uset*) std::malloc(n_edges * edgewords * sizeof(uset));
    for (int i = 0; i < n_edges * edgewords; ++i) {
        opt->all_edge_sets[i] = 0xDEADBEEF;
    }

    // levels array with no blocks
    opt->levels = (struct block**) std::malloc((max_level + 1) * sizeof(struct block*));
    for (int l = 0; l <= max_level; ++l) opt->levels[l] = nullptr;

    // Root
    struct block* root = allocate_block_with_edom(edgewords);
    TEST_ASSERT(root != nullptr, "Root allocation failed for test 2");
    root->level = max_level;

    // Initialize root edom to non-zero values
    for (int i = 0; i < edgewords; ++i) {
        root->et.edom[i] = (uset)(i + 1);
        root->ef.edom[i] = (uset)(i + 2);
    }

    // Call the focal function
    find_edom(opt, root);

    // Verify: all_edge_sets have been set to 0xFFFFFFFF
    bool all_ff = true;
    for (int i = 0; i < n_edges * edgewords; ++i) {
        if (opt->all_edge_sets[i] != 0xFFFFFFFFU) {
            all_ff = false;
            break;
        }
    }
    TEST_ASSERT(all_ff, "all_edge_sets should be set to 0xFFFFFFFFU when no blocks exist");

    // Verify: root edom buffers are zeroed
    bool root_edom_zero = true;
    for (int i = 0; i < edgewords; ++i) {
        if (root->et.edom[i] != 0 || root->ef.edom[i] != 0) {
            root_edom_zero = false;
            break;
        }
    }
    TEST_ASSERT(root_edom_zero, "root->et.edom and root->ef.edom should be zeroed when no blocks exist");

    // Cleanup
    free_block(root);
    std::free(opt->levels);
    std::free(opt->all_edge_sets);
    std::free(opt);

    TEST_PRINT_PROGRESS("Finished test_find_edom_no_blocks");
}

// Runner: executes all tests and prints a summary
int main() {
    TEST_PRINT_PROGRESS("Starting test suite for find_edom");

    test_find_edom_basic_with_blocks();
    test_find_edom_no_blocks();

    std::cout << "[SUMMARY] Ran " << g_tests_run << " tests, "
              << g_tests_failed << " failures." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}