// Unit tests for the fold_edge function from optimize.c
// Target: C++11, no Google Test, lightweight harness with non-terminating EXPECT_* macros.
// This test suite relies on the project headers to define the real structures
// (struct block, struct edge) and macros (A_ATOM, JT, JF, BPF_JMP, BPF_JEQ, BPF_K, etc.).
// The tests exercise true/false branches and various edge-cases in fold_edge.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdint>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <cstdio>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <pcap-types.h>


// Include the project's headers to obtain the real type definitions and macros.
// This file should provide: struct block, struct edge, A_ATOM, JT(), JF(), and fold_edge().

extern "C" int fold_edge(struct block *child, struct edge *ep); // ensure C linkage in C++

static int total_tests = 0;
static int failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    total_tests++; \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAILED: %s (line %d): %s\n", __FILE__, __LINE__, (msg)); \
        failed_tests++; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", (msg)); \
    } \
} while (0)

#define EXPECT_EQ(expected, actual, msg) do { \
    total_tests++; \
    if ((expected) != (actual)) { \
        fprintf(stderr, "TEST FAILED: %s (line %d): %s. Expected %p, got %p\n", __FILE__, __LINE__, (msg), (void*)(expected), (void*)(actual)); \
        failed_tests++; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", (msg)); \
    } \
} while (0)

#define EXPECT_NULL(ptr, msg) do { \
    total_tests++; \
    if ((ptr) != nullptr) { \
        fprintf(stderr, "TEST FAILED: %s (line %d): %s. Expected nullptr, got %p\n", __FILE__, __LINE__, (msg), (void*)(ptr)); \
        failed_tests++; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", (msg)); \
    } \
} while (0)

// Helper to initialize a simple block with minimal required fields.
// We rely on the real struct block layout provided by project headers.
// We zero-initialize and then set only fields used by fold_edge.
static void init_block(struct block *b)
{
    if (!b) return;
    // Zero the whole structure to a known baseline
    memset(b, 0, sizeof(struct block));
    // Note: real code accesses b->s.code, b->val[A_ATOM], b->oval, and
    // the successor pointers via JT()/JF() macros which map to fields in block.
}

// Helper to run a single test case with a descriptive name.
static void run_test_case(const char *name, void (*test_func)())
{
    fprintf(stdout, "Running test: %s\n", name);
    test_func();
}

// Test 1: True branch with equal A-atom and equal operands and equal ovals
// Expectation: fold_edge returns JT(child).
static void test_fold_edge_true_equal_oval_equal_aval()
{
    // Build the graph:
    // child.s.code = code; ep.code = code (positive); sense = 1
    // aval0 == aval1; oval0 == oval1
    // JT(child) should be returned.
    struct block child;
    struct block pred; // not used directly in outcome except for aval/oval
    struct block trueSucc; // sentinel to be returned by JT(child)
    struct edge ep;

    init_block(&child);
    init_block(&pred);
    init_block(&trueSucc);
    init_block(&ep);

    int code = 42; // positive code
    child.s.code = code;
    child.val[A_ATOM] = 7;    // aval0
    child.oval = 9;           // oval0

    pred.val[A_ATOM] = 7;     // aval1 equals aval0
    pred.oval = 9;            // oval1 equals oval0

    // Link successors for JT(child)
    child.JT = &trueSucc;       // macros JT(child) -> &trueSucc
    child.JF = nullptr;           // not used in this path

    ep.code = code;
    ep.pred = &pred;              // pred is the predecessor block

    // Run
    struct block *result = fold_edge(&child, &ep);

    // Expect result to be trueSucc (JT(child))
    EXPECT_EQ(&trueSucc, result, "fold_edge should return JT(child) when ovals equal and code positive");
}

// Test 2: True branch with equal A-atom but unequal ovals and eq-code path
// Expectation: when oval0 != oval1 and code == (BPF_JMP|BPF_JEQ|BPF_K) and sense==1,
// it should return JF(child).
static void test_fold_edge_true_oval_diff_eqcode()
{
    struct block child;
    struct block pred;
    struct block falseSucc;
    struct edge ep;

    init_block(&child);
    init_block(&pred);
    init_block(&falseSucc);
    init_block(&ep);

    int code = (BPF_JMP|BPF_JEQ|BPF_K); // eq-path code
    child.s.code = code;
    child.val[A_ATOM] = 5;
    child.oval = 3;

    pred.val[A_ATOM] = 5;     // equal
    pred.oval = 9;            // different

    // JT is irrelevant here; we test JF(child)
    child.JT = nullptr;
    child.JF = &falseSucc;

    ep.code = code;
    ep.pred = &pred;

    struct block *result = fold_edge(&child, &ep);

    EXPECT_EQ(&falseSucc, result, "fold_edge should return JF(child) on eq-code path with oval mismatch and sense=true");
}

// Test 3: A-atom mismatch -> should return 0 (no replacement candidate)
static void test_fold_edge_aval_mismatch()
{
    struct block child;
    struct block pred;
    struct edge ep;
    struct block trueSucc;

    init_block(&child);
    init_block(&pred);
    init_block(&ep);
    init_block(&trueSucc);

    int code = 7;
    child.s.code = code;
    child.val[A_ATOM] = 1;     // aval0
    child.oval = 5;

    pred.val[A_ATOM] = 999;    // aval1 different
    pred.oval = 5;

    child.JT = &trueSucc;
    child.JF = nullptr;
    ep.code = code;
    ep.pred = &pred;

    struct block *result = fold_edge(&child, &ep);
    EXPECT_NULL(result, "fold_edge should return NULL (0) when aval on exit differs");
}

// Test 4: Code mismatch after handling negative edge code -> should return 0
static void test_fold_edge_code_mismatch()
{
    struct block child;
    struct block pred;
    struct edge ep;
    struct block trueSucc;

    init_block(&child);
    init_block(&pred);
    init_block(&ep);
    init_block(&trueSucc);

    int original_code = 7;
    child.s.code = 7;          // matches non-negative computed code
    child.val[A_ATOM] = 2;
    child.oval = 4;

    pred.val[A_ATOM] = 2;
    pred.oval = 4;

    child.JT = &trueSucc;
    child.JF = nullptr;

    // ep.code is negative; computed code becomes -code
    ep.code = -original_code;  // negative
    ep.pred = &pred;

    // We want child.s.code != computed code to trigger code-mismatch branch
    // computed code will be 7, as -(-7) = 7; to cause mismatch, set child.s.code != 7
    child.s.code = 999;

    struct block *result = fold_edge(&child, &ep);
    EXPECT_NULL(result, "fold_edge should return NULL when block codes do not match after negation");
}

// Test 5: Negative edge path with equal ovals -> returns JF(child)
static void test_fold_edge_negative_edge_false_path()
{
    struct block child;
    struct block pred;
    struct edge ep;
    struct block falseSucc;

    init_block(&child);
    init_block(&pred);
    init_block(&ep);
    init_block(&falseSucc);

    int code = 42; // positive code used for comparison
    child.s.code = code;
    child.val[A_ATOM] = 11;
    child.oval = 8;

    pred.val[A_ATOM] = 11;
    pred.oval = 8;

    // sense = 0 since ep.code will be negative
    child.JT = nullptr;
    child.JF = &falseSucc;

    ep.code = -code;     // negative -> sense = 0
    ep.pred = &pred;

    struct block *result = fold_edge(&child, &ep);
    EXPECT_EQ(&falseSucc, result, "fold_edge should return JF(child) when edge is negative (sense 0) and ovals equal");
}

int main(void)
{
    // Run test suite
    run_test_case("fold_edge_true_equal_oval_equal_aval", test_fold_edge_true_equal_oval_equal_aval);
    run_test_case("fold_edge_true_oval_diff_eq_code", test_fold_edge_true_oval_diff_eq_code);
    run_test_case("fold_edge_aval_mismatch", test_fold_edge_aval_mismatch);
    run_test_case("fold_edge_code_mismatch", test_fold_edge_code_mismatch);
    run_test_case("fold_edge_negative_edge_false_path", test_fold_edge_negative_edge_false_path);

    // Summary
    printf("\nTest summary: %d executed, %d failed\n", total_tests, failed_tests);
    return (failed_tests == 0) ? 0 : 1;
}