// Test suite for the focal method: build_bl_tree(deflate_state *s)
// This test is crafted to be self-contained and does not rely on GTest.
// It provides minimal scaffolding to exercise the build_bl_tree logic
// as described in the provided focal method, using C++11.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Domain constants and lightweight types (reproducing necessary parts)
#define BL_CODES 19
typedef unsigned long ulg;

// Minimal data structures to reflect the needed parts
struct ct_data {
    unsigned Len;
};

struct tree_desc {
    int dummy;
};

struct LDesc {
    int max_code;
};

struct DDesc {
    int max_code;
};

struct deflate_state {
    ct_data *dyn_ltree;
    ct_data *dyn_dtree;
    LDesc l_desc;
    DDesc d_desc;
    tree_desc bl_desc;
    ct_data bl_tree[BL_CODES];
    int bl_order[BL_CODES];
    unsigned long opt_len;
    unsigned long static_len;
};

// Forward declarations of stubs used by build_bl_tree
static void scan_tree(deflate_state *s, ct_data *tree, int max_code) {
    // Stub: in the real code this would compress/frequency analyze.
    (void)s; (void)tree; (void)max_code;
}
static void build_tree(deflate_state *s, tree_desc *desc) {
    // Stub: in the real code this would build Huffman trees.
    (void)s; (void)desc;
}

// Trace macro replacement to avoid file-scope I/O in tests
#define Tracev(x) do { (void)0; } while(0)

// The focal method under test (copied logic adapted for test harness)
int build_bl_tree(deflate_state *s) {
    int max_blindex;  /* index of last bit length code of non zero freq */
    /* Determine the bit length frequencies for literal and distance trees */
    scan_tree(s, (ct_data *)s->dyn_ltree, s->l_desc.max_code);
    scan_tree(s, (ct_data *)s->dyn_dtree, s->d_desc.max_code);
    /* Build the bit length tree: */
    build_tree(s, (tree_desc *)(&(s->bl_desc)));
    /* opt_len now includes the length of the tree representations, except the
     * lengths of the bit lengths codes and the 5 + 5 + 4 bits for the counts.
     */
    /* Determine the number of bit length codes to send. The pkzip format
     * requires that at least 4 bit length codes be sent. (appnote.txt says
     * 3 but the actual value used is 4.)
     */
    for (max_blindex = BL_CODES-1; max_blindex >= 3; max_blindex--) {
        if (s->bl_tree[s->bl_order[max_blindex]].Len != 0) break;
    }
    /* Update opt_len to include the bit length tree and counts */
    s->opt_len += 3*((ulg)max_blindex + 1) + 5 + 5 + 4;
    Tracev((stderr, "\ndyn trees: dyn %ld, stat %ld",
            s->opt_len, s->static_len));
    return max_blindex;
}

// Utility: reset state for each test case
static void reset_state(deflate_state &s) {
    s.dyn_ltree = nullptr;
    s.dyn_dtree = nullptr;
    s.l_desc.max_code = 0;
    s.d_desc.max_code = 0;
    s.opt_len = 0;
    s.static_len = 0;
    s.bl_desc.dummy = 0;
    for (int i = 0; i < BL_CODES; ++i) {
        s.bl_tree[i].Len = 0;
        s.bl_order[i] = i; // identity mapping as default
    }
}

// Simple non-terminating assertions with test name reporting
static int g_fail_count = 0;
static void expect_eq_int(int a, int b, const char* name) {
    if (a != b) {
        std::cerr << "[FAIL] " << name << ": expected " << b << ", got " << a << "\n";
        ++g_fail_count;
    } else {
        std::cout << "[PASS] " << name << "\n";
    }
}
static void expect_eq_ull(unsigned long a, unsigned long b, const char* name) {
    if (a != b) {
        std::cerr << "[FAIL] " << name << ": expected " << b << ", got " << a << "\n";
        ++g_fail_count;
    } else {
        std::cout << "[PASS] " << name << "\n";
    }
}

// Test 1: All bit-length codes nonzero are zero -> max_blindex should end at 2
static void test_all_zero_bl_trees() {
    // Setup
    deflate_state s;
    reset_state(s);
    // No non-zero Len in bl_tree
    // Ensure initial opt_len is zero
    s.opt_len = 0;
    // Provide non-null pointers to avoid accidental dereference in stubs
    static ct_data dummy_ltree[32];
    static ct_data dummy_dtree[32];
    s.dyn_ltree = dummy_ltree;
    s.dyn_dtree = dummy_dtree;
    s.l_desc.max_code = 15;
    s.d_desc.max_code = 15;
    // Execute
    int ret = build_bl_tree(&s);
    // Verify: expect 2 and opt_len = 23 (3*((2)+1) + 14)
    expect_eq_int(ret, 2, "build_bl_tree returns 2 when all Len are zero (true case)");
    expect_eq_ull(s.opt_len, 23ul, "build_bl_tree updates opt_len to 23 (all zero case)");
}

// Test 2: Non-zero bit-length at a mid index (e.g., 10)
static void test_nonzero_at_mid_index() {
    deflate_state s;
    reset_state(s);
    static ct_data dummy_ltree[32];
    static ct_data dummy_dtree[32];
    s.dyn_ltree = dummy_ltree;
    s.dyn_dtree = dummy_dtree;
    s.l_desc.max_code = 15;
    s.d_desc.max_code = 15;
    // Set non-zero length at index 10; with identity bl_order, this corresponds to max_blindex == 10
    s.bl_tree[10].Len = 1;
    // Ensure others are zero
    for (int i = 0; i < BL_CODES; ++i) s.bl_tree[i].Len = (i == 10) ? 1 : 0;
    // Execute
    int ret = build_bl_tree(&s);
    // Verify: expect 10 and opt_len = 3*(11) + 14 = 47
    expect_eq_int(ret, 10, "build_bl_tree returns 10 when Len non-zero at index 10");
    expect_eq_ull(s.opt_len, 47ul, "build_bl_tree updates opt_len correctly for mid index (47)");
}

// Test 3: Non-zero bit-length at the highest index (18)
static void test_nonzero_at_highest_index() {
    deflate_state s;
    reset_state(s);
    static ct_data dummy_ltree[32];
    static ct_data dummy_dtree[32];
    s.dyn_ltree = dummy_ltree;
    s.dyn_dtree = dummy_dtree;
    s.l_desc.max_code = 15;
    s.d_desc.max_code = 15;
    // Set non-zero length at index 18; with identity bl_order, this corresponds to max_blindex == 18
    s.bl_tree[18].Len = 1;
    for (int i = 0; i < BL_CODES; ++i) s.bl_tree[i].Len = (i == 18) ? 1 : 0;
    int ret = build_bl_tree(&s);
    // Verify: expect 18 and opt_len = 3*(19) + 14 = 71
    expect_eq_int(ret, 18, "build_bl_tree returns 18 when Len non-zero at index 18");
    expect_eq_ull(s.opt_len, 71ul, "build_bl_tree updates opt_len correctly for highest index (71)");
}

// Entry point for the test suite
int main() {
    std::cout << "Running build_bl_tree unit tests...\n";
    test_all_zero_bl_trees();
    test_nonzero_at_mid_index();
    test_nonzero_at_highest_index();

    if (g_fail_count == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << g_fail_count << " test(s) FAILED.\n";
        return 1;
    }
}