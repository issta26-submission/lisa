// Unit test suite for the focal method: count_stmts(struct icode *ic, struct block *p)
// This test harness is self-contained and does not rely on GTest. It provides minimal
// C-structure emulation for the production types and implements the focused function
// exactly as in the provided snippet. Each test case includes comments explaining intent
// and expectations.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <os-proto.h>
#include <unordered_set>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Domain-specific typedefs to align with the original code
typedef unsigned int u_int;

// Forward declarations of production-like structures
struct slist;
struct block;
struct icode;

// Macro helpers mirroring the original code
#define JT(b) ((b)->jt)
#define JF(b) ((b)->jf)

// Minimal singly linked list used to represent a list of statements
struct slist {
    slist *next;
};

// Simple helper to create a linked list of length 'len' (contents are irrelevant for counting)
static slist* make_slist(int len) {
    if (len <= 0) return nullptr;
    slist* head = new slist();
    slist* cur = head;
    for (int i = 1; i < len; ++i) {
        cur->next = new slist();
        cur = cur->next;
    }
    cur->next = nullptr;
    return head;
}

// Helper to compute length of slist
static int slength(struct slist *s) {
    int n = 0;
    while (s) {
        ++n;
        s = s->next;
    }
    return n;
}

// Core data structures mirroring the subset used by count_stmts
struct block {
    struct slist *stmts;
    struct block *jt;
    struct block *jf;
    unsigned int longjt;
    unsigned int longjf;
    int id;
    int offset;
};

// icode structure keeping track of marked blocks to prevent re-traversal
struct icode {
    std::unordered_set<block*> marked;
};

// Forward declarations for helper functions used by count_stmts
static int isMarked(struct icode *ic, struct block *p);
static void Mark(struct icode *ic, struct block *p);

// The focal function under test (copied/implemented here to be self-contained)
typedef unsigned int u_int;
u_int count_stmts(struct icode *ic, struct block *p)
{
{
    u_int n;
    if (p == 0 || isMarked(ic, p))
        return 0;
    Mark(ic, p);
    n = count_stmts(ic, JT(p)) + count_stmts(ic, JF(p));
    return slength(p->stmts) + n + 1 + p->longjt + p->longjf;
}
}

// Implementations of isMarked/Mark matching the expected behavior
static int isMarked(struct icode *ic, struct block *p) {
    if (ic == nullptr || p == nullptr) return 0;
    return ic->marked.find(p) != ic->marked.end() ? 1 : 0;
}
static void Mark(struct icode *ic, struct block *p) {
    if (ic && p) ic->marked.insert(p);
}

// Simple test harness
static int g_failures = 0;

static void expect_eq_int(int actual, int expected, const char* desc) {
    if (actual == expected) {
        std::cout << "PASS: " << desc << "\n";
    } else {
        std::cerr << "FAIL: " << desc << " | Expected " << expected << ", got " << actual << "\n";
        ++g_failures;
    }
}

// Helper to allocate blocks for tests
static block* new_block(int stmts_len, unsigned int longjt, unsigned int longjf) {
    block* b = new block();
    b->stmts = make_slist(stmts_len);
    b->jt = nullptr;
    b->jf = nullptr;
    b->longjt = longjt;
    b->longjf = longjf;
    b->id = 0;
    b->offset = 0;
    return b;
}

// Helper to clean up a simple tree (no cycles) - optional but nice
static void free_block_tree(block* b) {
    if (!b) return;
    // Free statements list
    slist* s = b->stmts;
    while (s) {
        slist* nx = s->next;
        delete s;
        s = nx;
    }
    b->stmts = nullptr;
    free_block_tree(b->jt);
    free_block_tree(b->jf);
    delete b;
}

// Test 1: Null block pointer should return 0
static void test_null_pointer() {
    icode ic;
    block* p = nullptr;
    u_int res = count_stmts(&ic, p);
    expect_eq_int((int)res, 0, "count_stmts(null) returns 0");
}

// Test 2: Leaf node with N statements and no children
static void test_leaf_no_children() {
    icode ic;
    block* p = new_block(2, 1, 0); // 2 statements, longjt=1, longjf=0
    // Ensure no children
    p->jt = nullptr;
    p->jf = nullptr;

    u_int res = count_stmts(&ic, p);
    // Expected: slen(2) + 0 + 1 + 1 + 0 = 4
    expect_eq_int((int)res, 4, "leaf node with 2 stmts and (1,0) extras -> 4");
    free_block_tree(p);
}

// Test 3: Root with two children (one leaf, one with its own child)
static void test_root_with_two_children() {
    icode ic;
    // Root: 1 stmt, longjt=2, longjf=3
    block* root = new_block(1, 2, 3);
    // Child 1: leaf with 0 stmts
    block* c1 = new_block(0, 0, 0);
    // Child 2: has 1 stmt and no further children
    block* c2 = new_block(1, 0, 0);

    // Link children
    root->jt = c1;
    root->jf = c2;

    // No grandchildren for c1 or c2 in this setup
    // Compute expected:
    // count(c1) = slen(0) + 0 + 1 + 0 + 0 = 1
    // count(c2) = slen(1) + 0 + 1 + 0 + 0 = 2
    // count(root) = slen(1) + (1+2) + 1 + 2 + 3 = 1 + 3 + 1 + 2 + 3 = 10
    u_int res = count_stmts(&ic, root);
    expect_eq_int((int)res, 10, "root with two children (one empty, one with 1 stmt) -> 10");
    free_block_tree(root);
}

// Test 4: Cycle detection (ensures no infinite recursion due to marking)
static void test_cycle_detection() {
    icode ic;
    // Build two blocks that reference each other
    block* b0 = new_block(0, 0, 0);
    block* b1 = new_block(0, 0, 0);
    b0->jt = b1;
    b0->jf = nullptr;
    b1->jt = b0; // cycle
    b1->jf = nullptr;

    u_int res = count_stmts(&ic, b0);
    // With no statements and a mutual cycle, the traversal should visit
    // b0 -> b1 -> (b0 already marked) and stop.
    // count(b0) = slen(0) + [count(b1) + count(NULL)] + 1 + 0 + 0
    // count(b1) = slen(0) + [count(b0) (but marked) + 0] + 1 + 0 + 0
    // -> count(b1) = 1
    // -> count(b0) = 0 + 1 + 1 = 2
    expect_eq_int((int)res, 2, "cycle detection should yield 2 for two 0-stmt blocks in cycle");
    free_block_tree(b0);
    free_block_tree(b1);
}

// Test 5: Another cycle case with deeper nesting
static void test_cycle_deeper() {
    icode ic;
    block* r = new_block(0, 0, 0);
    block* a = new_block(1, 0, 0); // a has 1 stmt
    block* b = new_block(0, 0, 0);

    r->jt = a;
    r->jf = nullptr;
    a->jt = b;
    a->jf = nullptr;
    b->jt = r; // cycle back to root
    b->jf = nullptr;

    // expected:
    // count(b) = 0 + 0 + 1 + 0 + 0 = 1
    // count(a) = 1 + count(b) + 1 + 0 + 0 = 1 + 1 + 1 = 3
    // count(r) = 0 + (count(a)) + 1 + 0 + 0 = 0 + 3 + 1 = 4
    u_int res = count_stmts(&ic, r);
    expect_eq_int((int)res, 4, "cycle with nested traversal should yield 4");
    free_block_tree(r);
    free_block_tree(a);
    free_block_tree(b);
}

int main() {
    std::cout << "Running count_stmts unit tests...\n";

    test_null_pointer();
    test_leaf_no_children();
    test_root_with_two_children();
    test_cycle_detection();
    test_cycle_deeper();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}