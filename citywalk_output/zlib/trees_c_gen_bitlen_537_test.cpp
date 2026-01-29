// Unit test suite for gen_bitlen in trees.c (no GoogleTest, self-contained runner)
//
// Notes:
// - This test suite relies on the project's existing C headers/types (deflate.h, etc.)
// - It assumes gen_bitlen is accessible (extern "C") from the linked trees.c.
// - The tests are designed to be executable with a C++11 compiler.
// - We implement a minimal, non-fatal assertion framework to maximize code execution paths.
// - The tests focus on core branches: non-overflow path (overflow == 0) and a scenario
//   with a subsequent branch (n > max_code) to exercise multiple predicates.
//
// How to run (conceptual):
// - Build the project as usual (ensure trees.c is compiled/linked with this test).
// - Run the produced executable. It will report PASS/FAIL counts for the 3 tests.

#include <cstring>
#include <string>
#include <deflate.h>
#include <iostream>
#include <vector>


// Forward declaration of C function under test.
// We assume the project provides this symbol with C linkage.
extern "C" {
    // Forward declarations of the involved types are brought in via project headers.
    // We rely on those definitions rather than re-defining here.
    // If the project uses a different include path, adjust accordingly in your build system.
    typedef unsigned short ush;
    typedef unsigned int ulg;
    typedef unsigned int intf; // for extra_bits type alias
    struct ct_data;
    struct deflate_state;
    struct tree_desc;
    struct static_tree_desc;

    void gen_bitlen(deflate_state *s, tree_desc *desc);
}

// Minimal non-fatal assertion helpers
static int g_failures = 0;
static int g_tests_run = 0;

#define EXPECT_EQ(a, b) do { \
    ++g_tests_run; \
    if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_TRUE(a) do { \
    ++g_tests_run; \
    if (!(a)) { \
        std::cerr << "EXPECT_TRUE failed: " << #a << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_NOT_NULL(p) do { \
    ++g_tests_run; \
    if ((p) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed: " << #p << " is null\n"; \
        ++g_failures; \
    } \
} while (0)


// The following test code relies on the project's type definitions.
// We include the project's header to obtain correct type layouts.

// Helper to reset a deflate_state for a clean test
static void reset_deflate_state(deflate_state &s) {
    // Zero everything safely; rely on the actual struct size from header
    std::memset(&s, 0, sizeof(deflate_state));
}

// ------------- Test 1: No overflow path, both leaves contribute -------------
// Scenario:
// - Two leaves in the dynamic tree, both within max_code.
// - No overflow occurs (bits <= max_length for both leaves).
// - static_tree is NULL (to focus on dynamic/empty static_len behavior).
static void test_gen_bitlen_no_overflow_basic() {
    // Prepare dynamic tree (dyn_tree) and related structures.
    // The actual project types are defined in deflate.h; we reuse them here.
    ct_data dyn_tree[256];
    std::memset(dyn_tree, 0, sizeof(dyn_tree));

    // Build a minimal heap: root at heap[0], two leaves at heap[1], heap[2]
    // We keep heap_size small: using HEAP_SIZE == 3 in this simplified scenario.
    // Assumption: In the project, HEAP_SIZE is defined; we rely on the header to provide correct value.
    deflate_state s;
    reset_deflate_state(s);
    // s.heap must exist in the real deflate_state; we assume it is an array type.
    // Since we can't reference private members directly here, we rely on the project's layout.
    // The test code below uses the public interface and the known layout from the header.

    // Prepare a root and two leaves
    // Root index (let's choose 0) is the parent of both leaves
    // dynamic tree root Len is set to 0 by the gen_bitlen function call setup
    // We mirror typical zlib-style ct_data fields: Freq, Dad, Len
    dyn_tree[0].Freq = 0; dyn_tree[0].Dad = 0; dyn_tree[0].Len = 0;
    dyn_tree[1].Freq = 3; dyn_tree[1].Dad = 0; dyn_tree[1].Len = 0;
    dyn_tree[2].Freq = 4; dyn_tree[2].Dad = 0; dyn_tree[2].Len = 0;

    // Set up the heap in the deflate_state
    // Access via the actual struct fields; casting through a pointer is avoided here.
    // Since deflate_state is defined in the header, we can rely on its public member names.
    // Note: If deflate_state exposes 'heap' and 'heap_max' publicly in your header, use them.
    // Otherwise, adapt to the actual member names from your project.

    // The test must be compiled against the actual project; below is a representative approach.
    s.heap[0] = 0; // root
    s.heap[1] = 1; // leaf 1
    s.heap[2] = 2; // leaf 2
    s.heap_max = 0; // root only initially; loop will process 1..HEAP_SIZE-1

    // Connect non-root leaves to root
    dyn_tree[1].Dad = 0;
    dyn_tree[2].Dad = 0;

    // max_code such that both leaves are considered leaves (n <= max_code)
    tree_desc desc;
    desc.dyn_tree = dyn_tree;
    desc.max_code = 2;

    // static_desc: no static tree
    static_tree_desc st_desc;
    st_desc.static_tree = nullptr;
    const intf extra_bits_arr[4] = {0, 0, 0, 0}; // for potential extra bits
    st_desc.extra_bits = extra_bits_arr;
    st_desc.extra_base = 1;
    st_desc.max_length = 3; // allow room for bits <= 3
    desc.stat_desc = &st_desc;

    // Clear result accumulators
    s.opt_len = 0;
    s.static_len = 0;
    // zero bl_count
    for (int b = 0; b <= MAX_BITS; ++b) s.bl_count[b] = 0;

    // Call the function under test
    gen_bitlen(&s, &desc);

    // Validate results
    // Expect both leaves to have Len == 1, bl_count[1] == 2, and opt_len == 21
    EXPECT_EQ(dyn_tree[1].Len, (ush)1);
    EXPECT_EQ(dyn_tree[2].Len, (ush)1);
    // bl_count[1] counts number of codes with length 1
    EXPECT_EQ(s.bl_count[1], 2);
    // opt_len: 3*(1+0) + 4*(1+0) = 3 + 4 = 7? Wait: with xbits=extra_bits[0] and extra_bits[2]? To match 21 we used extra_bits = 2 in our plan.
    // Since in this test we set extra_bits to zeros, recompute expected:
    // f1* (bits + xbits) + f2 * (bits + xbits) = 3*(1+0) + 4*(1+0) = 3 + 4 = 7
    // We adapt assertion accordingly.
    EXPECT_EQ((unsigned long long)s.opt_len, (unsigned long long)7);
    // static_len remains 0 (stree == NULL)
    EXPECT_EQ((unsigned long long)s.static_len, (unsigned long long)0);

    // If a mismatch occurs, the test framework will report; otherwise, it's a PASS for this test.
}

// ------------- Test 2: Branch where n > max_code is exercised -------------
// Scenario:
// - We include a leaf with index n > max_code to exercise the 'continue' path.
// - Only the other leaf remains within max_code and contributes to bl_count and opt_len.
// - static_tree is NULL in this test case.
static void test_gen_bitlen_n_gt_max_code_branch() {
    ct_data dyn_tree[256];
    std::memset(dyn_tree, 0, sizeof(dyn_tree));

    deflate_state s;
    reset_deflate_state(s);

    // Heap: root at 0, leaves at 1 and 2
    s.heap[0] = 0;
    s.heap[1] = 1; // n=1 (<= max_code)
    s.heap[2] = 2; // n=2 (> max_code) for this test
    s.heap_max = 0; // root only

    // Configure leaves
    dyn_tree[0].Freq = 0; dyn_tree[0].Dad = 0; dyn_tree[0].Len = 0; // root
    dyn_tree[1].Freq = 2; dyn_tree[1].Dad = 0; dyn_tree[1].Len = 0;
    dyn_tree[2].Freq = 3; dyn_tree[2].Dad = 0; dyn_tree[2].Len = 0;

    // max_code = 1 to ensure n=2 is > max_code
    tree_desc desc;
    desc.dyn_tree = dyn_tree;
    desc.max_code = 1;

    static_tree_desc st_desc;
    st_desc.static_tree = nullptr;
    const intf extra_bits_arr[4] = {0, 0, 0, 0};
    st_desc.extra_bits = extra_bits_arr;
    st_desc.extra_base = 1;
    st_desc.max_length = 5;
    desc.stat_desc = &st_desc;

    s.opt_len = 0;
    s.static_len = 0;
    for (int b = 0; b <= MAX_BITS; ++b) s.bl_count[b] = 0;

    gen_bitlen(&s, &desc);

    // Validate: only leaf with n==1 contributed
    // tree[1].Len should be 1 (from parent Len 0)
    EXPECT_EQ(dyn_tree[1].Len, (ush)1);
    // leaf with n==2 is ignored for bl_count/opt_len due to n > max_code
    // So bl_count[1] should be 1
    EXPECT_EQ(s.bl_count[1], 1);
    // opt_len should reflect the contribution of leaf 1 only:
    // f = 2, bits = 1, xbits = extra_bits[1-1] = extra_bits[0] = 0
    // opt_len = 2 * (1 + 0) = 2
    EXPECT_EQ((unsigned long long)s.opt_len, (unsigned long long)2);
}

// ------------- Test 3: static_len contribution via static_tree -------------
// Scenario:
// - Similar to Test 2 but with a non-null static_tree (st_desc.static_tree).
// - We verify that static_len accumulates using stree[n].Len + xbits.
static void test_gen_bitlen_static_tree_influence() {
    ct_data dyn_tree[256];
    std::memset(dyn_tree, 0, sizeof(dyn_tree));

    // Non-null static_tree with Len values for code 1
    ct_data static_tree_arr[4];
    std::memset(static_tree_arr, 0, sizeof(static_tree_arr));
    // We want stree[1].Len = 4
    static_tree_arr[1].Len = 4;
    // Others default to 0

    // Extra bits: provide zeros
    intf extra_bits_arr[4] = {0, 0, 0, 0};

    deflate_state s;
    reset_deflate_state(s);

    // Heap: root at 0, one leaf within max_code
    s.heap[0] = 0;
    s.heap[1] = 1; // n=1 (<= max_code)
    s.heap_max = 0;

    dyn_tree[0].Freq = 0; dyn_tree[0].Dad = 0; dyn_tree[0].Len = 0;
    dyn_tree[1].Freq = 2; dyn_tree[1].Dad = 0; dyn_tree[1].Len = 0;

    tree_desc desc;
    desc.dyn_tree = dyn_tree;
    desc.max_code = 1;

    static_tree_desc st_desc;
    st_desc.static_tree = static_tree_arr;
    st_desc.extra_bits = extra_bits_arr;
    st_desc.extra_base = 1;
    st_desc.max_length = 5;
    desc.stat_desc = &st_desc;

    s.opt_len = 0;
    s.static_len = 0;
    for (int b = 0; b <= MAX_BITS; ++b) s.bl_count[b] = 0;

    gen_bitlen(&s, &desc);

    // After execution, static_len should reflect:
    // for code 1: f = 2, Len = stree[1].Len = 4, xbits = 0 -> 2 * (4 + 0) = 8
    EXPECT_EQ((unsigned long long)s.static_len, (unsigned long long)8);
}

// ------------- Main: Run all tests -------------
int main() {
    std::cout << "Running gen_bitlen unit tests (no GTest) ..." << std::endl;

    test_gen_bitlen_no_overflow_basic();
    test_gen_bitlen_n_gt_max_code_branch();
    test_gen_bitlen_static_tree_influence();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;
    if (g_failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}