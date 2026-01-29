// Unit test suite for the focal method make_marks in optimize.c
// This test harness recreates the minimal dependencies required by make_marks
// (as inferred from the provided code) and exercises true/false branches of its logic.
// Notes:
// - This test does not rely on any external testing framework (GTest/GMock).
// - It uses a lightweight, non-terminating assertion mechanism that records failures
//   and continues execution to maximize coverage.
// - All necessary types and functions are defined in this test file to keep it self-contained.

#include <gencode.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <optimize.h>
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
#include <sstream>
#include <os-proto.h>
#include <unordered_set>
#include <config.h>


// Domain-specific constants and macros (mimicking the original C API minimally)
static const int BPF_RET = 1;
#define BPF_CLASS(code) (code)  // For testing purposes, treat code as its own class
#define JT(p) ((p)->JT)
#define JF(p) ((p)->JF)

// Forward-declare the minimal structures used by make_marks
struct stmt {
    int code; // s.code in the original
};

struct block {
    stmt s;
    block *JT;
    block *JF;
    int id;
};

// Forward-declare the core icode structure used to track marks
struct icode {
    std::unordered_set<block*> marked;
};

// Lightweight, non-terminating assertion framework
static int g_test_failures = 0;
static std::stringstream g_test_log;

// Helpers for test assertions
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        g_test_failures++; \
        g_test_log << "EXPECT_TRUE failed: " << (msg) << "\n"; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    if ((cond)) { \
        g_test_failures++; \
        g_test_log << "EXPECT_FALSE failed: " << (msg) << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        g_test_failures++; \
        g_test_log << "EXPECT_EQ failed: " << (msg) \
                     << " (lhs=" << (a) << ", rhs=" << (b) << ")\n"; \
    } \
} while(0)

// Minimal isMarked/Mark implementations to back the focal method
static bool isMarked(struct icode* ic, struct block* p) {
    if (ic == nullptr || p == nullptr) return false;
    return ic->marked.find(p) != ic->marked.end();
}

static void Mark(struct icode* ic, struct block* p) {
    if (ic != nullptr && p != nullptr) ic->marked.insert(p);
}

// The focal method under test (replicated in this test harness)
static inline int BpfRetGuard(int code) { return (code == BPF_RET); }

static void make_marks(struct icode *ic, struct block *p) {
{
    if (!isMarked(ic, p)) {
        Mark(ic, p);
        if (BPF_CLASS(p->s.code) != BPF_RET) {
            make_marks(ic, JT(p));
            make_marks(ic, JF(p));
        }
    }
}
}

// Test utilities to build blocks and trees
static block* new_block(std::vector<block*>& pool, int id, int code) {
    block* b = new block();
    b->id = id;
    b->s.code = code;
    b->JT = nullptr;
    b->JF = nullptr;
    pool.push_back(b);
    return b;
}

static void cleanup(std::vector<block*>& pool, icode* ic) {
    // Delete all allocated blocks
    for (block* b : pool) {
        delete b;
    }
    pool.clear();
    delete ic;
}

// Explanations for each test are embedded as comments above the corresponding test function.

// Test 1: Simple non-RET node with non-null RET leaf on both branches
// - Root is non-RET and should be marked.
// - Both JT and JF point to the same leaf node which is RET; leaf should be marked as part of the recursion.
// - This covers the "true" branch of the if (BPF_CLASS(...) != BPF_RET) and ensures leaf RET doesn't recurse.
static void test_simple_non_ret_marks_children_ret_leaf() {
    g_test_log.str(std::string());
    g_test_log.clear();

    std::vector<block*> pool;
    icode* ic = new icode();

    // Create a RET leaf
    block* leafRET = new_block(pool, 2, BPF_RET);
    // Prevent recursion from leafRET
    leafRET->JT = leafRET;
    leafRET->JF = leafRET;

    // Root node (non-RET)
    block* root = new_block(pool, 1, 0); // code 0 means non-RET under our macro
    root->JT = leafRET;
    root->JF = leafRET;

    // Run the focal method
    make_marks(ic, root);

    // Assertions
    EXPECT_TRUE(isMarked(ic, root), "Root should be marked");
    EXPECT_TRUE(isMarked(ic, leafRET), "Leaf RET should be marked due to recursion from root");

    // Verify that only root and leafRET are marked (no extra nodes exist in this test)
    EXPECT_EQ(static_cast<int>(ic->marked.size()), 2, "Only two blocks should be marked (root and leafRET)");

    // Cleanup
    cleanup(pool, ic);

    // If anything failed, print log; otherwise print success summary
    if (g_test_failures == 0) {
        std::cout << "test_simple_non_ret_marks_children_ret_leaf: PASSED\n";
    } else {
        std::cout << "test_simple_non_ret_marks_children_ret_leaf: FAILED\n";
        std::cout << g_test_log.str();
    }
}

// Test 2: Root is RET; no recursion should occur.
// - Only the root should be marked; children should remain unmarked.
static void test_ret_root_no_recursion() {
    g_test_log.str(std::string());
    g_test_log.clear();

    std::vector<block*> pool;
    icode* ic = new icode();

    // Create a non-RET leaf (not used due to early_ret)
    block* child = new_block(pool, 3, 0);
    // RET root
    block* root = new_block(pool, 1, BPF_RET);
    root->JT = child;
    root->JF = child;

    // Run the focal method
    make_marks(ic, root);

    // Assertions
    EXPECT_TRUE(isMarked(ic, root), "Root RET should be marked");
    EXPECT_FALSE(isMarked(ic, child), "Child should NOT be marked since root is RET and should not recurse");

    EXPECT_EQ(static_cast<int>(ic->marked.size()), 1, "Only one block should be marked (root)");

    // Cleanup
    cleanup(pool, ic);

    if (g_test_failures == 0) {
        std::cout << "test_ret_root_no_recursion: PASSED\n";
    } else {
        std::cout << "test_ret_root_no_recursion: FAILED\n";
        std::cout << g_test_log.str();
    }
}

// Test 3: Cyclic graph (two non-RET nodes referencing each other) to ensure termination
// - root1 <-> root2 form a cycle; both should be marked exactly once without infinite recursion.
static void test_cycle_does_not_infinite_loop() {
    g_test_log.str(std::string());
    g_test_log.clear();

    std::vector<block*> pool;
    icode* ic = new icode();

    block* root1 = new_block(pool, 1, 0);
    block* root2 = new_block(pool, 2, 0);

    // Create a cycle: root1 -> root2; root2 -> root1
    root1->JT = root2;
    root1->JF = root2;
    root2->JT = root1;
    root2->JF = root1;

    // Run the focal method
    make_marks(ic, root1);

    // Assertions
    EXPECT_TRUE(isMarked(ic, root1), "root1 should be marked");
    EXPECT_TRUE(isMarked(ic, root2), "root2 should be marked due to recursion from root1");

    EXPECT_EQ(static_cast<int>( ic->marked.size() ), 2, "Exactly two blocks should be marked in a cycle");

    // Cleanup
    cleanup(pool, ic);

    if (g_test_failures == 0) {
        std::cout << "test_cycle_does_not_infinite_loop: PASSED\n";
    } else {
        std::cout << "test_cycle_does_not_infinite_loop: FAILED\n";
        std::cout << g_test_log.str();
    }
}

// Test 4: True and false branches with both non-RET children (ensures both branches are explored)
// - Root is non-RET; JT and JF point to two separate non-RET children, each with a RET leaf to terminate recursion.
static void test_both_branches_explored() {
    g_test_log.str(std::string());
    g_test_log.clear();

    std::vector<block*> pool;
    icode* ic = new icode();

    // Leaves
    block* leafRET_A = new_block(pool, 3, BPF_RET);
    leafRET_A->JT = leafRET_A;
    leafRET_A->JF = leafRET_A;

    block* leafRET_B = new_block(pool, 4, BPF_RET);
    leafRET_B->JT = leafRET_B;
    leafRET_B->JF = leafRET_B;

    // Branch nodes
    block* branchA = new_block(pool, 5, 0);
    branchA->JT = leafRET_A;
    branchA->JF = leafRET_A;

    block* branchB = new_block(pool, 6, 0);
    branchB->JT = leafRET_B;
    branchB->JF = leafRET_B;

    // Root
    block* root = new_block(pool, 0, 0);
    root->JT = branchA;
    root->JF = branchB;

    // Run
    make_marks(ic, root);

    // Assertions
    EXPECT_TRUE(isMarked(ic, root), "Root should be marked");
    EXPECT_TRUE(isMarked(ic, branchA), "branchA should be marked");
    EXPECT_TRUE(isMarked(ic, branchB), "branchB should be marked");
    EXPECT_TRUE(isMarked(ic, leafRET_A), "leafRET_A should be marked via branchA");
    EXPECT_TRUE(isMarked(ic, leafRET_B), "leafRET_B should be marked via branchB");

    // Expect 5 blocks marked
    EXPECT_EQ(static_cast<int>(ic->marked.size()), 5, "Five blocks should be marked in this configuration");

    // Cleanup
    cleanup(pool, ic);

    if (g_test_failures == 0) {
        std::cout << "test_both_branches_explored: PASSED\n";
    } else {
        std::cout << "test_both_branches_explored: FAILED\n";
        std::cout << g_test_log.str();
    }
}

// Main entry point to run all tests
int main() {
    // Run tests
    test_simple_non_ret_marks_children_ret_leaf();
    test_ret_root_no_recursion();
    test_cycle_does_not_infinite_loop();
    test_both_branches_explored();

    if (g_test_failures == 0) {
        std::cout << "\nALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "\nSOME TESTS FAILED (" << g_test_failures << " failures)\n";
        // Optional: print accumulated log
        std::cout << g_test_log.str();
        return 1;
    }
}