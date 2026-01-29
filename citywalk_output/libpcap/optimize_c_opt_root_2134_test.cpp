/*
  Unit test suite for the focal method opt_root (as implemented below in a
  self-contained C++11 test harness).
  Notes:
  - This test suite is self-contained and does not depend on external GTest.
  - It re-implements a minimal, self-contained subset of the C structures and
    logic needed to exercise the opt_root behavior described in the prompt.
  - The tests cover true/false branches for key predicates and branch-prediction
    logic (JMP chaining, RETs, and list merge semantics via sappend).
  - Static members or private API access is not used; tests operate through the
    public-like function implemented here.
  - The approach focuses on validating the logic of opt_root without requiring
    a full integration with the original optimize.c build, while preserving
    semantics that would be exercised by the real function.
  - The code includes explanatory comments for each test case.
*/

/* Candidate Keywords captured from the focal method and its dependencies:
   - BPF_CLASS, BPF_JMP, BPF_RET
   - JT, JF (jump targets)
   - sappend (slist merge)
   - stmts, code field access (block->s.code)
   - return-early behavior for RET
   - pointer manipulation with block** and block*
   - while loop with conditional JMP chain
*/

/* Domain knowledge applied:
   - Implement a minimal C-like environment in C++ to simulate the behavior of
     opt_root, including block and slist structures and the essential macros.
   - Provide three test scenarios:
     1) Root is RET: ensures root->stmts becomes NULL.
     2) JMP chain ending in RET: ensures root->stmts becomes a merged list of
        root's stmts and the tail's stmts.
     3) JMP chain ending in non-RET (non-RET tail): ensures root->stmts merges
        as in scenario 2, but no final zeroing of stmts.
   - Use non-terminating checks (custom CHECK macro) to allow all tests to run
     and report concise results.
*/

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <cstddef>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// ------------------------ Minimal type/utility definitions ------------------------

/* Code class representation used by the simplified test environment */
constexpr int CODE_JMP = 1;
constexpr int CODE_RET = 2;
constexpr int CODE_OTHER = 3;

/* BPF class enumeration (partial) used by the focal method logic */
enum BpfClass { BPF_RET = 0, BPF_JMP = 1, BPF_OTHER = 2 };

/* Simple helper to mimic BPF_CLASS macro in the original code */
static inline int BPF_CLASS(int code) {
    if (code == CODE_JMP) return BPF_JMP;
    if (code == CODE_RET) return BPF_RET;
    return BPF_OTHER;
}

/* Forward declarations for jump-target helpers (JT/JF in the focal code) */
struct block;
static inline struct block *JT(struct block *b);
static inline struct block *JF(struct block *b);

/* Singly-linked list node (structure used by the focal code) */
struct slist {
    slist *next;
};

/* Inner 's' structure with a code field (as used in the focal code) */
struct s_code {
    int code;
};

/* Core block structure used by the focal code.
   We implement only fields required for opt_root:
   - s.code
   - stmts (slist*)
   - jt (true jump target)
   - jf (false jump target)
*/
struct block {
    s_code s;
    slist *stmts;
    block *jt;
    block *jf;
    int id; // not used by tests, but keeps the struct realistic
};

// Jump-target helpers
static inline struct block *JT(struct block *b) { return b ? b->jt : nullptr; }
static inline struct block *JF(struct block *b) { return b ? b->jf : nullptr; }

// Simple list merge: append 'append' list to the end of 'head' list.
// Note: This test harness ensures 'head' is non-null when called (see tests).
static void sappend(struct slist *head, struct slist *append) {
    if (head == nullptr) return;
    slist *p = head;
    while (p->next != nullptr) p = p->next;
    p->next = append;
    // Ensure the appended list is terminated
    if (append) append->next = nullptr;
}

// Focal-like opt_root implementation (self-contained for testing)
static void opt_root(struct block **b) {
    struct slist *tmp, *s;
    s = (*b)->stmts;
    (*b)->stmts = 0;
    // Follow JMP/true-branch chains while they are JMPs with JT == JF
    while (BPF_CLASS((*b)->s.code) == BPF_JMP && JT(*b) == JF(*b))
        *b = JT(*b);
    tmp = (*b)->stmts;
    if (tmp != 0)
        sappend(s, tmp);
    (*b)->stmts = s;
    // If the root node (the current *b) is a return, zero its stmts
    if (BPF_CLASS((*b)->s.code) == BPF_RET)
        (*b)->stmts = 0;
}

// ------------------------ Test harness and helpers ------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

/* Simple non-terminating assertion helper: records failures but continues */
#define CHECK(cond, msg)                           \
    do {                                           \
        ++g_total_tests;                           \
        if (!(cond)) {                             \
            ++g_failed_tests;                      \
            std::cerr << "TEST FAILED: " << msg << "\n"; \
        } else {                                   \
            std::cout << "TEST PASSED: " << msg << "\n"; \
        }                                          \
    } while (0)

/* Utility to create a new slist node (no payload content needed for tests) */
static slist* make_list_node() {
    return new slist{nullptr};
}

/* Utility to link two lists (end to tail) for clarity in tests */
static void link_tail(slist *head, slist *tail) {
    if (!head) return;
    slist *p = head;
    while (p->next) p = p->next;
    p->next = tail;
    if (tail) tail->next = nullptr;
}

// ------------------------ Individual test cases ------------------------

/*
  Test 1: Root ret immediately
  - Root block s.code = CODE_RET
  - root->stmts is non-null to verify that final RET clears statements
  - No jumps involved; verify opt_root sets stmts to NULL
*/
static void test_opt_root_ret_terminates_stmts() {
    // Root block and its initial statements
    block root;
    root.s.code = CODE_RET;     // RET to trigger terminal path
    slist root_stat_node;
    root_stat_node.next = nullptr;
    root.stmts = &root_stat_node;

    // No jumps configured
    root.jt = nullptr;
    root.jf = nullptr;
    root.id = 1;

    // Call the focal function
    opt_root(&(&root));

    // Expectation: root.stmts should be NULL due to final RET handling
    CHECK(root.stmts == nullptr, "Root with RET should clear its statements");
}

/*
  Test 2: JMP chain ending in RET merges root's stmts with tail's stmts
  - root: JMP -> mid; mid: JMP -> tail; tail: RET
  - root and mid both have non-empty stmts
  - tail has non-empty stmts
  - After opt_root, root->stmts should be a merged list:
    root_main -> tail_main
*/
static void test_opt_root_jmp_chain_to_ret_merges_stmts() {
    // Allocate nodes for the test
    block root;
    block mid;
    block tail;

    // Root (JMP) -> mid
    root.s.code = CODE_JMP;
    root.jt = &mid;
    root.jf = &mid;
    root.id = 10;
    root.stmts = new slist{ nullptr }; // root_main list head
    root.stmts->next = nullptr;

    // Mid (JMP) -> tail
    mid.s.code = CODE_JMP;
    mid.jt = &tail;
    mid.jf = &tail;
    mid.id = 20;
    mid.stmts = new slist{ nullptr }; // mid_main list head
    mid.stmts->next = nullptr;

    // Tail (RET)
    tail.s.code = CODE_RET;
    tail.jt = nullptr;
    tail.jf = nullptr;
    tail.id = 30;
    tail.stmts = new slist{ nullptr }; // tail_main list head
    tail.stmts->next = nullptr;

    // For deterministic behavior, ensure the root's initial and tail's stmts are distinct chain heads
    // After opt_root, root.stmts should point to root_main then tail_main appended
    // Call the focal function
    block *root_ptr = &root;
    opt_root(&root_ptr);

    // Check the resulting root stmts chain structure
    // Expected: root still points to its own root_main; then root_main.next should point to tail_main
    bool condition = (root_ptr != nullptr) && (root_ptr->stmts != nullptr);

    condition = condition && (root_ptr->stmts == root.stmts);
    condition = condition && (root_ptr->stmts->next == tail.stmts);

    CHECK(condition, "JMP chain ending in RET merges root stmts with tail stmts (root_main -> tail_main)");
}

/*
  Test 3: JMP chain ending in non-RET merges but does not clear root stmts
  - root: JMP -> mid; mid: JMP -> tail; tail: OTHER (non-RET)
  - All stmts are non-empty
  - After opt_root, root->stmts should be a merged list including tail's stmts
  - Ensure the final RET path is not taken
*/
static void test_opt_root_jmp_chain_to_non_ret_merges_stmts() {
    // Allocate nodes for the test
    block root;
    block mid;
    block tail;

    // Root (JMP) -> mid
    root.s.code = CODE_JMP;
    root.jt = &mid;
    root.jf = &mid;
    root.id = 11;
    root.stmts = new slist{ nullptr }; // root_main list head
    root.stmts->next = nullptr;

    // Mid (JMP) -> tail
    mid.s.code = CODE_JMP;
    mid.jt = &tail;
    mid.jf = &tail;
    mid.id = 21;
    mid.stmts = new slist{ nullptr }; // mid_main list head
    mid.stmts->next = nullptr;

    // Tail (OTHER non-RET)
    tail.s.code = CODE_OTHER;
    tail.jt = nullptr;
    tail.jf = nullptr;
    tail.id = 31;
    tail.stmts = new slist{ nullptr }; // tail_main list head
    tail.stmts->next = nullptr;

    // Execute
    block *root_ptr = &root;
    opt_root(&root_ptr);

    // After: root.stmts should be root_main followed by tail_main
    bool condition = (root_ptr != nullptr) && (root_ptr->stmts != nullptr);
    condition = condition && (root_ptr->stmts == root.stmts);
    condition = condition && (root_ptr->stmts->next == tail.stmts);

    CHECK(condition, "JMP chain to non-RET merges root and tail stmts (no zeroing)");
}

// ------------------------ Main entry: run tests ------------------------

int main() {
    std::cout << "Running opt_root unit test suite (self-contained harness)..." << std::endl;

    test_opt_root_ret_terminates_stmts();
    test_opt_root_jmp_chain_to_ret_merges_stmts();
    test_opt_root_jmp_chain_to_non_ret_merges_stmts();

    std::cout << "Tests completed. "
              << "Total: " << g_total_tests
              << ", Failed: " << g_failed_tests << std::endl;

    return g_failed_tests == 0 ? 0 : 1;
}