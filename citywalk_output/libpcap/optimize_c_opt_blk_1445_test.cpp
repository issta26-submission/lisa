// Test suite for the focal method opt_blk in optimize.c
// Targeted for C++11 compilation, no GTest used, just a lightweight test harness.
// The tests exercise the opt_blk behavior by constructing minimal in-memory
// representations of the relevant data structures defined in the project headers.
// Note: This test harness relies on the real project headers (optimize.h, etc.)
// and links against the C implementation. The code is written to be portable
// across compilers that support C++11 and C linkage for the C code under test.

#include <gencode.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <cassert>
#include <config.h>
#include <optimize.h>
#include <setjmp.h>
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


// Bring in the C interfaces/types from the project.
extern "C" {
}

// Lightweight, non-terminating test assertion macro.
// It records a test failure but does not abort the entire test run.
#defineASSERT_TRUE(cond, msg, failvec) \
    do { if(!(cond)) { failvec.push_back(std::string("ASSERT_TRUE failed: ") + (msg)); return false; } } while(0)

struct TestFailureCollector {
    std::vector<std::string> failures;
    void note(const std::string &s) { failures.push_back(s); }
    bool empty() const { return failures.empty(); }
};

// Utility: allocate and zero a block instance
static block* alloc_block() {
    block* b = (block*)calloc(1, sizeof(block));
    return b;
}

// Utility: allocate and create a simple slist node (one statement placeholder)
static struct slist* alloc_slist_with_dummy() {
    struct slist* head = (struct slist*)calloc(1, sizeof(struct slist));
    if (head) {
        // Minimal dummy statement; the content is not important for tests
        head->next = nullptr;
        head->s.code = 0;
    }
    return head;
}

// Utility: create an edge pointing to a given predecessor block
static struct edge* alloc_edge(block* pred) {
    struct edge* ep = (struct edge*)calloc(1, sizeof(struct edge));
    if (ep) {
        ep->pred = pred;
        ep->next = nullptr;
    }
    return ep;
}

// Test 1: No predecessors (p == 0) -> all b->val initialized to 0
static bool test_opt_blk_no_predecessors(TestFailureCollector &collector) {
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state)); // minimal init
    block* b = alloc_block();
    // Ensure no in_edges
    b->in_edges = NULL;
    // Ensure some values exist in the array (will be zeroed by opt_blk)
    // The exact number of atoms is controlled by the headers; write a safe pattern.
    // We avoid relying on uninitialized memory by setting a cover value (anything is overwritten).
    for (unsigned i = 0; i < N_ATOMS; ++i) {
        b->val[i] = 0xDEADBEEF; // non-zero, will be overwritten
    }

    // Ensure there are some statements to exercise later loop (even if non-functional here)
    b->stmts = alloc_slist_with_dummy();

    // Initialize block's small code field
    b->s.code = 0; // non-RET code; actual value is not important for first test
    b->s.k = 0;

    // Call the focal function
    opt_blk(&opt_state, b, 1);

    // Verify that all atoms were zeroed when there are no predecessors
    for (unsigned i = 0; i < N_ATOMS; ++i) {
        if (b->val[i] != 0) {
            collector.note("test_opt_blk_no_predecessors: val[" + std::to_string(i) + "] != 0 after opt_blk with no predecessors");
            free(b->stmts);
            free(b);
            return false;
        }
    }

    // Clean up
    free(b->stmts);
    free(b);
    return true;
}

// Test 2: Multiple predecessors with identical values -> inherit first pred values
static bool test_opt_blk_multiple_preds_same(TestFailureCollector &collector) {
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    // Predecessor 1
    block* pred1 = alloc_block();
    pred1->in_edges = NULL;
    pred1->stmts = NULL;
    // Set a known val pattern
    bpf_u_int32 val_pattern[ N_ATOMS ];
    for (unsigned i = 0; i < N_ATOMS; ++i)
        val_pattern[i] = (i + 1) * 3; // arbitrary distinct values
    for (unsigned i = 0; i < N_ATOMS; ++i) pred1->val[i] = val_pattern[i];

    // Predecessor 2 (same values)
    block* pred2 = alloc_block();
    pred2->in_edges = NULL;
    pred2->stmts = NULL;
    for (unsigned i = 0; i < N_ATOMS; ++i) pred2->val[i] = val_pattern[i];

    // Block under test
    block* b = alloc_block();
    b->in_edges = alloc_edge(pred1);
    b->in_edges->next = alloc_edge(pred2);
    b->stmts = NULL;
    // Clear b->val to ensure it gets overwritten from predecessors
    for (unsigned i = 0; i < N_ATOMS; ++i) b->val[i] = 0;

    // No statements to affect values
    b->s.code = 0;

    opt_blk(&opt_state, b, 1);

    // Expect b->val to equal pred1->val (since pred2 has identical values)
    for (unsigned i = 0; i < N_ATOMS; ++i) {
        if (b->val[i] != val_pattern[i]) {
            collector.note("test_opt_blk_multiple_preds_same: val[" + std::to_string(i) + "] differs from first predecessor");
            // cleanup
            free(b->stmts); // likely NULL
            // free edges
            free(b->in_edges);
            free(pred1);
            free(pred2);
            free(b);
            return false;
        }
    }

    // cleanup
    free(b->stmts);
    free(b->in_edges);
    free(pred1);
    free(pred2);
    free(b);
    return true;
}

// Test 3: Multiple predecessors with a differing value -> specific atoms become undefined (0)
static bool test_opt_blk_multiple_preds_diff(TestFailureCollector &collector) {
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    // Predecessor 1 values
    block* pred1 = alloc_block();
    pred1->in_edges = NULL;
    pred1->stmts = NULL;
    bpf_u_int32 val1[ N_ATOMS ];
    for (unsigned i = 0; i < N_ATOMS; ++i) val1[i] = (i + 1) * 4;
    for (unsigned i = 0; i < N_ATOMS; ++i) pred1->val[i] = val1[i];

    // Predecessor 2 with a difference on A_ATOM (index 0)
    block* pred2 = alloc_block();
    pred2->in_edges = NULL;
    pred2->stmts = NULL;
    bpf_u_int32 val2[ N_ATOMS ];
    for (unsigned i = 0; i < N_ATOMS; ++i) val2[i] = val1[i];
    val2[0] = val2[0] + 1; // introduce a difference
    for (unsigned i = 0; i < N_ATOMS; ++i) pred2->val[i] = val2[i];

    // Block under test
    block* b = alloc_block();
    b->in_edges = alloc_edge(pred1);
    b->in_edges->next = alloc_edge(pred2);
    b->stmts = NULL;
    b->s.code = 0;
    // Pre-set b->val to something to observe the merge effect
    for (unsigned i = 0; i < N_ATOMS; ++i) b->val[i] = 1234 + i;

    opt_blk(&opt_state, b, 1);

    // After merge, all positions where pred1 and pred2 differ should be 0
    // We expect index 0 to become 0 due to the difference
    if (b->val[0] != 0) {
        collector.note("test_opt_blk_multiple_preds_diff: A_ATOM value not undefined (expected 0) after merge");
        // cleanup
        free(b->stmts);
        free(b->in_edges);
        free(pred1);
        free(pred2);
        free(b);
        return false;
    }
    // Other atoms should remain equal to pred1's values
    for (unsigned i = 1; i < N_ATOMS; ++i) {
        if (b->val[i] != val1[i]) {
            collector.note("test_opt_blk_multiple_preds_diff: non-differing atom changed unexpectedly");
            free(b->stmts);
            free(b->in_edges);
            free(pred1);
            free(pred2);
            free(b);
            return false;
        }
    }

    // cleanup
    free(b->stmts);
    free(b->in_edges);
    free(pred1);
    free(pred2);
    free(b);
    return true;
}

// Test 4: Elimination path triggered by RET (or equivalent). We ensure the branch condition is true
// and verify that statements get eliminated (b->stmts becomes 0) and flags are updated.
static bool test_opt_blk_elimination_ret(TestFailureCollector &collector) {
    opt_state_t opt_state;
    memset(&opt_state, 0, sizeof(opt_state));

    // Predecessor with known values
    block* pred = alloc_block();
    pred->in_edges = NULL;
    pred->stmts = NULL;
    // Choose values to be preserved
    bpf_u_int32 val_pred[ N_ATOMS ];
    for (unsigned i = 0; i < N_ATOMS; ++i) val_pred[i] = (i + 2) * 7;
    for (unsigned i = 0; i < N_ATOMS; ++i) pred->val[i] = val_pred[i];

    // Target block
    block* b = alloc_block();
    b->in_edges = alloc_edge(pred);
    b->in_edges->next = NULL;
    // Ensure there is at least one statement so elimination has something to remove
    b->stmts = alloc_slist_with_dummy();
    b->s.code = BPF_RET; // RET should trigger the special-case elimination
    b->s.k = 0;
    // out_use = 0 ensures we consider "uses nothing" path
    b->out_use = 0;

    // Set b->val to pred's val initially
    for (unsigned i = 0; i < N_ATOMS; ++i) b->val[i] = val_pred[i];

    // Do not rely on opt_stmt to modify these values; ensure some safe dummy statement exists
    // The key is that the condition aval/xval are defined (not VAL_UNKNOWN) and the RET path triggers
    opt_blk(&opt_state, b, 1);

    // Expect the block statements to be eliminated
    if (b->stmts != 0) {
        collector.note("test_opt_blk_elimination_ret: statements were not eliminated as expected");
        free(b->stmts);
        free(b->in_edges);
        free(pred);
        free(b);
        return false;
    }

    // Expect opt_state flags to reflect the "movement" performed
    if (opt_state.done != 0) {
        collector.note("test_opt_blk_elimination_ret: opt_state.done should be 0 after elimination");
        free(b->in_edges);
        free(pred);
        free(b);
        return false;
    }
    if (opt_state.non_branch_movement_performed != 1) {
        collector.note("test_opt_blk_elimination_ret: non_branch_movement_performed not set to 1 after elimination");
        free(b->in_edges);
        free(pred);
        free(b);
        return false;
    }

    // cleanup
    free(b->stmts); // already NULL by elimination, but safe
    free(b->in_edges);
    free(pred);
    free(b);
    return true;
}

// Helper: run all tests and print results
static void run_all_tests() {
    TestFailureCollector collector;

    int passed = 0, total = 0;

    auto run = [&](bool (*test_func)(TestFailureCollector&), const std::string &name) {
        total++;
        bool ok = test_func(collector);
        if (ok) {
            std::cout << "[PASS] " << name << "\n";
            passed++;
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run(test_opt_blk_no_predecessors, "opt_blk_no_predecessors_true_branch");
    run(test_opt_blk_multiple_preds_same, "opt_blk_multiple_preds_same_values");
    run(test_opt_blk_multiple_preds_diff, "opt_blk_multiple_preds_diff_values");
    run(test_opt_blk_elimination_ret, "opt_blk_elimination_ret_path");

    if (!collector.failures.empty()) {
        std::cout << "\nSummary of failures:\n";
        for (const auto &f : collector.failures) {
            std::cout << "  - " << f << "\n";
        }
    }

    std::cout << "\nTest result: " << passed << " / " << total << " tests passed.\n";
}

int main() {
    // Run the suite
    run_all_tests();
    return 0;
}