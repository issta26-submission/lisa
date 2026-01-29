/*
  C++11 test suite for sqlite3Fts5ExprClearPoslists (fts5_expr.c)

  Notes:
  - Relies on the real FTS5 data structures defined in the project's headers
    (e.g., fts5Int.h, fts5parse.h). The test creates in-memory instances of
    Fts5Expr, Fts5ExprPhrase, Fts5Buffer, Fts5ExprNode, and Fts5PoslistPopulator
    matching the fields used by sqlite3Fts5ExprClearPoslists.
  - Provides a C-linkage shim for sqlite3_malloc64 to ensure deterministic
    memory allocation in tests.
  - Uses non-terminating assertions (print-only) to maximize code coverage.
  - Tests focus on two code paths: bLive true and false, and specific branches
    inside the loop (Miss path vs. clear poslists path).
  - No GTest; tests are invoked from main().
*/

#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Bring in the required FTS5 type definitions with C linkage.
// The actual project defines these; we mirror the necessary structures via the
// project headers to ensure compatibility with sqlite3Fts5ExprClearPoslists.
extern "C" {
}

// Prototype for the function under test.
extern "C" Fts5PoslistPopulator* sqlite3Fts5ExprClearPoslists(Fts5Expr *pExpr, int bLive);

// Provide a C-compatible memory allocator used by the code under test.
extern "C" void* sqlite3_malloc64(size_t n) {
    return malloc(n);
}

// Simple test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_test_pass(const char* name) {
    std::cout << "[PASS] " << name << std::endl;
}

static void log_test_fail(const char* name, const char* reason) {
    std::cerr << "[FAIL] " << name << ": " << reason << std::endl;
    ++g_failed_tests;
}

static void run_all_tests(void);

// Test 1: Basic behavior with bLive = 1
// - Phrase 0: poslist.n == 0 => Miss should be set (pRet[0].bMiss == 1), poslists unchanged.
// - Phrase 1: poslist.n > 0 and iRowid matches root and not EOF => No Miss, poslist reset to 0.
static void test_basic_miss_and_poslists_reset_live_true() {
    const char* test_name = "test_basic_miss_and_poslists_reset_live_true";
    // Prepare root
    Fts5ExprRoot root;
    root.iRowid = 1;

    // Prepare phrases
    Fts5ExprPhrase* ph0 = (Fts5ExprPhrase*)(std::malloc(sizeof(Fts5ExprPhrase)));
    ph0->nTerm = 0;
    ph0->poslist.n = 0; // triggers Miss due to pBuf->n == 0
    ph0->pNode = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    ph0->pNode->iRowid = 2; // different, but Miss already triggered by poslist.n == 0
    ph0->pNode->bEof = 0;

    Fts5ExprPhrase* ph1 = (Fts5ExprPhrase*)(std::malloc(sizeof(Fts5ExprPhrase)));
    ph1->nTerm = 0;
    ph1->poslist.n = 5; // non-zero
    ph1->pNode = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    ph1->pNode->iRowid = 1; // same as root
    ph1->pNode->bEof = 0;

    // Assemble pExpr
    Fts5Expr* pExpr = (Fts5Expr*)std::malloc(sizeof(Fts5Expr));
    pExpr->nPhrase = 2;
    pExpr->apExprPhrase = (Fts5ExprPhrase**)std::malloc(sizeof(Fts5ExprPhrase*) * pExpr->nPhrase);
    pExpr->pRoot = &root;

    pExpr->apExprPhrase[0] = ph0;
    pExpr->apExprPhrase[1] = ph1;

    // Call function under test
    Fts5PoslistPopulator* pRet = sqlite3Fts5ExprClearPoslists(pExpr, 1);

    bool ok = true;
    if (pRet == nullptr) {
        ok = false;
        log_test_fail(test_name, "Returned null pointer for pRet");
    } else {
        if (pRet[0].bMiss != 1) ok = false;
        if (pRet[1].bMiss != 0) ok = false;
        if (ph1->poslist.n != 0) ok = false;
        // ph0 remained unchanged (poslist.n == 0) in Miss path
        if (ph0->poslist.n != 0) ok = false;
    }

    // Cleanup
    std::free(pRet); // allocated via sqlite3_malloc64 in the test context
    std::free(ph0->pNode);
    std::free(ph0);
    std::free(ph1->pNode);
    std::free(ph1);
    std::free(pExpr->apExprPhrase);
    std::free(pExpr);
    log_test_pass(test_name);
}

// Test 2: bLive = 0 should zero poslists and produce no Miss flags
// - Regardless of initial poslist values, all poslists should be set to 0 and
//   no Miss flags should be set.
static void test_live_false_always_resets_poslists_no_miss() {
    const char* test_name = "test_live_false_always_resets_poslists_no_miss";
    // Prepare root
    Fts5ExprRoot root;
    root.iRowid = 1;

    // Prepare phrases
    Fts5ExprPhrase* ph0 = (Fts5ExprPhrase*)std::malloc(sizeof(Fts5ExprPhrase));
    ph0->nTerm = 0;
    ph0->poslist.n = 7; // non-zero
    ph0->pNode = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    ph0->pNode->iRowid = 99;
    ph0->pNode->bEof = 0;

    Fts5ExprPhrase* ph1 = (Fts5ExprPhrase*)std::malloc(sizeof(Fts5ExprPhrase));
    ph1->nTerm = 0;
    ph1->poslist.n = 3; // non-zero
    ph1->pNode = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    ph1->pNode->iRowid = 1;
    ph1->pNode->bEof = 0;

    // Assemble pExpr
    Fts5Expr* pExpr = (Fts5Expr*)std::malloc(sizeof(Fts5Expr));
    pExpr->nPhrase = 2;
    pExpr->apExprPhrase = (Fts5ExprPhrase**)std::malloc(sizeof(Fts5ExprPhrase*) * pExpr->nPhrase);
    pExpr->pRoot = &root;

    pExpr->apExprPhrase[0] = ph0;
    pExpr->apExprPhrase[1] = ph1;

    Fts5PoslistPopulator* pRet = sqlite3Fts5ExprClearPoslists(pExpr, 0);

    bool ok = true;
    if (pRet == nullptr) {
        ok = false;
        log_test_fail(test_name, "Returned null pointer for pRet");
    } else {
        if (pRet[0].bMiss != 0) ok = false;
        if (pRet[1].bMiss != 0) ok = false;
        if (ph0->poslist.n != 0) ok = false;
        if (ph1->poslist.n != 0) ok = false;
    }

    // Cleanup
    std::free(pRet);
    std::free(ph0->pNode);
    std::free(ph0);
    std::free(ph1->pNode);
    std::free(ph1);
    std::free(pExpr->apExprPhrase);
    std::free(pExpr);
    log_test_pass(test_name);
}

// Test 3: bLive = 1 with an iRowid mismatch to trigger Miss due to node rowid
// - Phrase 0: iRowid != root.iRowid => Miss
// - Phrase 1: iRowid == root.iRowid && poslist.n > 0 => not Miss; poslist reset
static void test_miss_due_to_node_rowid_mismatch() {
    const char* test_name = "test_miss_due_to_node_rowid_mismatch";
    // Prepare root
    Fts5ExprRoot root;
    root.iRowid = 5;

    // Phrase 0: mismatch in iRowid (trigger Miss)
    Fts5ExprPhrase* ph0 = (Fts5ExprPhrase*)std::malloc(sizeof(Fts5ExprPhrase));
    ph0->nTerm = 0;
    ph0->poslist.n = 4;
    ph0->pNode = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    ph0->pNode->iRowid = 1; // different from root
    ph0->pNode->bEof = 0;

    // Phrase 1: matches root, should clear poslist
    Fts5ExprPhrase* ph1 = (Fts5ExprPhrase*)std::malloc(sizeof(Fts5ExprPhrase));
    ph1->nTerm = 0;
    ph1->poslist.n = 9;
    ph1->pNode = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    ph1->pNode->iRowid = 5; // matches root
    ph1->pNode->bEof = 0;

    // Assemble pExpr
    Fts5Expr* pExpr = (Fts5Expr*)std::malloc(sizeof(Fts5Expr));
    pExpr->nPhrase = 2;
    pExpr->apExprPhrase = (Fts5ExprPhrase**)std::malloc(sizeof(Fts5ExprPhrase*) * pExpr->nPhrase);
    pExpr->pRoot = &root;

    pExpr->apExprPhrase[0] = ph0;
    pExpr->apExprPhrase[1] = ph1;

    Fts5PoslistPopulator* pRet = sqlite3Fts5ExprClearPoslists(pExpr, 1);

    bool ok = true;
    if (pRet == nullptr) {
        ok = false;
        log_test_fail(test_name, "Returned null pointer for pRet");
    } else {
        if (pRet[0].bMiss != 1) ok = false;
        if (pRet[1].bMiss != 0) ok = false;
        if (ph0->poslist.n != 4) ok = false; // unchanged in Miss path
        if (ph1->poslist.n != 0) ok = false; // cleared
    }

    // Cleanup
    std::free(pRet);
    std::free(ph0->pNode);
    std::free(ph0);
    std::free(ph1->pNode);
    std::free(ph1);
    std::free(pExpr->apExprPhrase);
    std::free(pExpr);
    log_test_pass(test_name);
}

// Entry points
static void run_all_tests(void) {
    test_basic_miss_and_poslists_reset_live_true();
    test_live_false_always_resets_poslists_no_miss();
    test_miss_due_to_node_rowid_mismatch();

    std::cout << "Test summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures." << std::endl;
}

// Main
int main(void) {
    // Initialize test count
    g_total_tests = 0;

    // Run all tests
    run_all_tests();

    // Final verdict
    if (g_failed_tests == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failed_tests << " test(s) failed." << std::endl;
        return 1;
    }
}