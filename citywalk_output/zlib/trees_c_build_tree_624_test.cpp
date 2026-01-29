// Lightweight unit test for the focal method "build_tree" in trees.c
// This test uses a minimal, self-contained harness to exercise
// the core branching behavior of build_tree by constructing two scenarios:
// 1) Several non-zero frequencies (multiple leaves in the initial heap)
// 2) Only a single non-zero frequency (forcing additional leaf nodes to be created)
//
// Assumptions:
// - The project provides deflate.h with the real type definitions (deflate_state, tree_desc, ct_data, etc.).
// - build_tree is linked from the provided trees.c (non-private in the test environment).
// - This test is designed for C++11 compilation and does not rely on GTest; it uses a small
//   internal test harness with non-terminating EXPECT-like assertions.
// - We avoid modifying the library code; we only exercise it via extern/C linkage.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Domain knowledge hints: only public, non-terminating checks should be used.
// Lightweight assertion macros to mirror "EXPECT_*" style without terminating tests.
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[TEST_FAIL] " #cond " is false at " __FILE__ ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if((a) != (b)) { \
        std::cerr << "[TEST_FAIL] " #a " != " #b " (" << (a) << " != " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

extern "C" {
    // Declares the focal function. The actual definition is in trees.c and linked with the test binary.
    // The "deflate.h" header is expected to define the related types (deflate_state, tree_desc, ct_data, etc.).
    // If the signature differs (e.g., due to macro 'local'), this extern should still resolve in the test environment.
    void build_tree(struct deflate_state *s, struct tree_desc *desc);
}

// To maximize compatibility, rely on minimal forward declarations.
// The actual project should provide the full definitions via deflate.h.

// Helper: perform a small, isolated test of build_tree with a given configuration.
// This function does not rely on any private library state outside of the structures we pass in.
static void run_build_tree_test_three_nonzero() {
    // Scenario 1: 3 non-zero frequencies -> expect max_code to reflect the highest index with non-zero freq
    const int elems = 3;

    // Prepare dynamic tree array
    ct_data *dyn_tree = new ct_data[elems];
    // Capture original frequencies to check post-conditions
    int orig_freq[elems];
    for (int i = 0; i < elems; ++i) {
        // Non-zero frequencies for all three elements
        dyn_tree[i].Freq = 1 + i; // 1, 2, 3
        orig_freq[i] = dyn_tree[i].Freq;
        dyn_tree[i].Len = 0;       // reset length; will be set by build_tree
        dyn_tree[i].Dad = 0;
    }

    // Prepare static tree descriptor (desc->stat_desc)
    // We assume the library provides a type static_tree_desc with fields: static_tree and elems
    static_tree_desc st_desc;
    st_desc.static_tree = nullptr; // not used in this test
    st_desc.elems = elems;

    // Prepare tree_desc
    tree_desc tdesc;
    tdesc.dyn_tree = dyn_tree;
    tdesc.stat_desc = &st_desc;

    // Prepare deflate_state
    deflate_state s;
    std::memset(&s, 0, sizeof(s));

    // The actual unit under test
    build_tree(&s, &tdesc);

    // Post-conditions:
    // - For each index where orig_freq[i] != 0, the corresponding Len should be > 0 (codes generated)
    for (int i = 0; i < elems; ++i) {
        if (orig_freq[i] != 0) {
            EXPECT_TRUE(dyn_tree[i].Len > 0);
        } else {
            // Should remain zero since it was never a leaf
            EXPECT_EQ(dyn_tree[i].Len, 0);
        }
    }

    // - max_code should reflect the highest index with non-zero frequency (here, 2)
    // Since the initial frequencies cover indices 0..2, max_code is expected to be 2.
    EXPECT_EQ(st_desc.elems > 0 ? s.max_code : -1, 2);

    delete[] dyn_tree;
}

static void run_build_tree_test_one_nonzero() {
    // Scenario 2: Only 1 non-zero frequency -> force creation of extra heap entries
    // Expect max_code to reach 2 after two additional leaves are created
    const int elems = 3;

    ct_data *dyn_tree = new ct_data[elems];
    int orig_freq[elems];
    for (int i = 0; i < elems; ++i) {
        dyn_tree[i].Freq = (i == 0) ? 5 : 0; // only index 0 is non-zero
        orig_freq[i] = dyn_tree[i].Freq;
        dyn_tree[i].Len = 0;
        dyn_tree[i].Dad = 0;
    }

    static_tree_desc st_desc;
    st_desc.static_tree = nullptr;
    st_desc.elems = elems;

    tree_desc tdesc;
    tdesc.dyn_tree = dyn_tree;
    tdesc.stat_desc = &st_desc;

    deflate_state s;
    std::memset(&s, 0, sizeof(s));
    build_tree(&s, &tdesc);

    // Post-conditions:
    // - Non-zero frequency element (index 0) should get a code length > 0
    EXPECT_TRUE(dyn_tree[0].Len > 0);
    // - Other elements with zero frequency should remain Len == 0
    EXPECT_EQ(dyn_tree[1].Len, 0);
    EXPECT_EQ(dyn_tree[2].Len, 0);

    // - max_code should reflect the creation of two extra leaves (indices 1 and 2), so max_code == 2
    EXPECT_EQ(s.max_code, 2);

    delete[] dyn_tree;
}

int main() {
    std::cout << "Running unit tests for build_tree (C++11 harness)" << std::endl;

    // Run tests
    run_build_tree_test_three_nonzero();
    run_build_tree_test_one_nonzero();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}