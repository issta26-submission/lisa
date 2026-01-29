// Unit test suite for the focal method: find_levels_r in optimize.c
// This test harness uses plain C++11, without Google Test, and calls the C
// function directly. It relies on the project's existing type definitions
// (opt_state_t, struct icode, struct block) exposed through the project's headers.
// The tests cover: leaf processing, recursive level computation, and
// early-return when a node is already marked.
// Notes:
// - All tests are non-terminating; they log failures and continue.
// - Static/global members in the focal code are accessed through the provided API.
// - We avoid mocking; we exercise real behavior via real data structures.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
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


// Bring in the C declarations for the focal function and related types.
// Use extern "C" to ensure proper linkage when compiling as C++.
extern "C" {
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_EXPECT_TRUE(cond, desc) \
    do { \
        if (!(cond)) { \
            std::cerr << "Test failure: " << __func__ << ": " << (desc) << "\n"; \
            ++g_tests_failed; \
        } \
        ++g_tests_run; \
    } while (0)

#define TEST_EXPECT_EQ(a, b, desc) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "Test failure: " << __func__ << ": " << (desc) \
                      << " | Expected " << (a) << " == " << (b) << "\n"; \
            ++g_tests_failed; \
        } \
        ++g_tests_run; \
    } while (0)

#define TEST_EXPECT_PTR_EQ(a, b, desc) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "Test failure: " << __func__ << ": " << (desc) \
                      << " | Pointers differ: " << (void*)(a) << " != " << (void*)(b) << "\n"; \
            ++g_tests_failed; \
        } \
        ++g_tests_run; \
    } while (0)

#define TEST_EXPECT_NULL(p, desc) \
    do { \
        if ((p) != nullptr) { \
            std::cerr << "Test failure: " << __func__ << ": " << (desc) \
                      << " | Pointer is not NULL: " << (void*)(p) << "\n"; \
            ++g_tests_failed; \
        } \
        ++g_tests_run; \
    } while (0)


// Helper to safely zero-initialize C-style structs
template <typename T>
static void zero(T &t) {
    std::memset(&t, 0, sizeof(T));
}

// We assume the project provides the following public types and members used by find_levels_r:
// - opt_state_t with a member: struct block *levels[] (array of level-head pointers)
// - struct icode with bookkeeping for isMarked/Mark macros
// - struct block with fields/jt/jf/level/link as used by the focal method
//
// The tests below construct simple graphs and verify the behavior of find_levels_r.

//
// Test 1: Leaf node processing
// - Create a single block b with no true/false successors (JT(b) == NULL, JF(b) == NULL).
// - After find_levels_r, expect:
//   * b.level == 0
//   * b.link == NULL (since the previous head at level 0 is NULL)
//   * opt_state.levels[0] points to b
//
static void test_find_levels_r_leaf_sets_level_and_pushes_to_levels() {
    // Prepare fresh state
    opt_state_t opt;
    zero(opt); // Clear levels array and any other fields to a known state

    struct icode ic;
    zero(ic);

    struct block b;
    zero(b);
    // Ensure no outgoing edges
    // The macros JT(b) / JF(b) will examine b.jt / b.jf (assumed fields)
    b.jt = nullptr;
    b.jf = nullptr;

    // Call focal method
    find_levels_r(&opt, &ic, &b);

    // Validate
    TEST_EXPECT_EQ(b.level, 0, "Leaf block should have level 0");
    TEST_EXPECT_NULL(b.link, "Leaf's link should be NULL (no prior head)");
    TEST_EXPECT_PTR_EQ(opt.levels[0], &b, "Block should be at the head of levels[0]");
}

// Test 2: Recursive level computation for a binary-like branching node
// - Create a parent block p with two children t and f (JT(p) -> t, JF(p) -> f), both leaves.
// - Leaves have level 0; parent should get level 1 and be pushed to levels[1].
// - Ensure leaves have level 0 and are not re-ordered in a conflicting way.
static void test_find_levels_r_recursive_computation() {
    opt_state_t opt;
    zero(opt);

    struct icode ic;
    zero(ic);

    // Create left and right leaf blocks
    struct block t;
    zero(t);
    t.jt = nullptr;
    t.jf = nullptr;

    struct block f;
    zero(f);
    f.jt = nullptr;
    f.jf = nullptr;

    // Create parent block p with JT and JF pointing to t and f
    struct block p;
    zero(p);
    p.jt = &t;
    p.jf = &f;

    // Call focal method on parent
    find_levels_r(&opt, &ic, &p);

    // Validate leaves are level 0
    TEST_EXPECT_EQ(t.level, 0, "Left leaf should have level 0");
    TEST_EXPECT_EQ(f.level, 0, "Right leaf should have level 0");

    // Validate parent level computation
    TEST_EXPECT_EQ(p.level, 1, "Parent should have level 1 (max(child levels) + 1)");
    TEST_EXPECT_PTR_EQ(opt.levels[1], &p, "Parent should be at the head of levels[1]");
}

// Test 3: Early return on already-marked node (static/marked state per ic)
// - Re-run find_levels_r on the same graph and verify no changes occur on second call.
// - This checks the isMarked/Mark path preventing reprocessing.
static void test_find_levels_r_already_marked_is_noop_on_second_call() {
    opt_state_t opt;
    zero(opt);

    struct icode ic;
    zero(ic);

    // Create a simple binary-like graph
    struct block t;
    zero(t);
    t.jt = nullptr;
    t.jf = nullptr;

    struct block f;
    zero(f);
    f.jt = nullptr;
    f.jf = nullptr;

    struct block p;
    zero(p);
    p.jt = &t;
    p.jf = &f;

    // First call - should process and set levels
    find_levels_r(&opt, &ic, &p);

    int level_after_first = p.level;
    struct block *head1 = opt.levels[1];

    // Second call on the same ic/p should be a no-op
    find_levels_r(&opt, &ic, &p);

    // Validate no changes on second call
    TEST_EXPECT_EQ(p.level, level_after_first, "Second call should not modify parent's level");
    TEST_EXPECT_PTR_EQ(opt.levels[1], head1, "Second call should not modify levels[1] head");
}

// Entry point that runs all tests
int main() {
    std::cout << "Running tests for find_levels_r (C -> C++)\n";

    test_find_levels_r_leaf_sets_level_and_pushes_to_levels();
    test_find_levels_r_recursive_computation();
    test_find_levels_r_already_marked_is_noop_on_second_call();

    std::cout << "Tests completed. Ran: " << g_tests_run << ", Failed: " << g_tests_failed << "\n";

    // If any test failed, return non-zero to indicate failure to build system
    return g_tests_failed ? 1 : 0;
}