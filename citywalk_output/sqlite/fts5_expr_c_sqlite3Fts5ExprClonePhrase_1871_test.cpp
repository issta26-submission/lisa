/*
 * Unit tests for sqlite3Fts5ExprClonePhrase
 * Target: Test sqlite3Fts5ExprClonePhrase(Fts5Expr*, int, Fts5Expr **)
 * Language: C++11 (no GoogleTest; simple assertions)
 * Notes:
 *  - This test relies on the FTS5 internal data structures exposed via fts5Int.h
 *  - We exercise multiple code paths by creating Fts5Expr inputs via sqlite3Fts5ExprNew
 *  - We validate that cloning succeeds and that resulting structure reflects expected branches
 *  - All tests are designed to be non-terminating on assertion failures to maximize coverage
 *  - The tests require linking with the project that implements sqlite3Fts5ExprClonePhrase and friends
 *  - Static/internal helpers from the production code (like fts5ParseTokenize) are exercised through the real function
 *  - This file should be compiled and linked with the C sources of the FTS5 module
 *
 * How to run (example):
 *   g++ -std=c++11 -o test_fts5expr_clonephrase test_fts5expr_clonephrase.cpp -lsqlite3 -l... (link as appropriate)
 *   ./test_fts5expr_clonephrase
 *
 * The code contains explanatory comments for every unit test block.
 */

#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Include the FTS5 internal structures and APIs.
// The real project should expose these through its headers.
// We rely on their presence to construct actual expressions and call into sqlite3Fts5ExprClonePhrase.
extern "C" {
}

// Ensure SQLITE_OK is defined for comparison with rc return codes.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif

// Helper macro for simple test logging
#define LOG_TEST_PASS(name)  std::cout << "[PASSED] " << name << std::endl
#define LOG_TEST_FAIL(name)  std::cerr << "[FAILED] " << name << std::endl

// Utility to safely free expressions if created
static void safe_free_expr(Fts5Expr *p) {
    if( p ) {
        // The production library provides sqlite3Fts5ExprFree to reclaim
        // all memory associated with an Fts5Expr instance.
        sqlite3Fts5ExprFree(p);
    }
}

// Test 1: Clone a simple single-term phrase with no synonyms and bFirst == 0
// Expected: Cloning succeeds and produces a new Fts5Expr with a single phrase, Type TERM
// Purpose: Exercise the branch pOrig->nTerm == 1 && pOrig->aTerm[0].pSynonym == 0 && pOrig->aTerm[0].bFirst == 0
static bool test_clone_single_term_no_synonym() {
    int rc = SQLITE_OK;
    Fts5Expr *pExpr = NULL;
    Fts5Expr *pClone = NULL;
    char *zErr = NULL;

    // Create a simple expression: one term "hello"
    // Using the production API to create a valid Fts5Expr; this gives us a realistic input
    rc = SQLITE_OK;
    pExpr = NULL;
    rc = sqlite3Fts5ExprNew(NULL, 0, 0, "hello", &pExpr, &zErr);
    if( rc != SQLITE_OK || pExpr == NULL ) {
        LOG_TEST_FAIL("Clone-SingleTerm: Failed to create source expression (rc=" << rc << ")");
        return false;
    }

    // Attempt to clone the first (and only) phrase
    rc = sqlite3Fts5ExprClonePhrase(pExpr, 0, &pClone);
    if( rc != SQLITE_OK || pClone == NULL ) {
        LOG_TEST_FAIL("Clone-SingleTerm: Cloning failed (rc=" << rc << ")");
        safe_free_expr(pExpr);
        return false;
    }

    // Validation: The clone should contain exactly one phrase and be of TERM type
    // Accessing internals is production-API dependent; we rely on public fields known to the library
    bool ok = true;
    if( pClone->nPhrase != 1 ) {
        ok = false;
        std::cerr << "Clone-SingleTerm: Unexpected nPhrase=" << pClone->nPhrase << std::endl;
    }
    // The root node type should be TERM as per the code path
    if( pClone->pRoot == NULL ) {
        ok = false;
        std::cerr << "Clone-SingleTerm: pRoot is NULL" << std::endl;
    } else {
        // eType is expected to be FTS5_TERM when single term with no synonyms and no first-s flag
        if( pClone->pRoot->eType != FTS5_TERM ) {
            ok = false;
            std::cerr << "Clone-SingleTerm: Unexpected eType=" << pClone->pRoot->eType << " (expected FTS5_TERM)" << std::endl;
        }
    }

    // Cleanup
    safe_free_expr(pClone);
    safe_free_expr(pExpr);

    if( ok ) {
        LOG_TEST_PASS("Clone-SingleTerm");
        return true;
    } else {
        LOG_TEST_FAIL("Clone-SingleTerm");
        return false;
    }
}

// Test 2: Clone a phrase with multiple terms (to exercise STRING path)
// Expected: Cloning succeeds and produces a new Fts5Expr with STRING type path
static bool test_clone_multiple_terms_string_path() {
    int rc = SQLITE_OK;
    Fts5Expr *pExpr = NULL;
    Fts5Expr *pClone = NULL;
    char *zErr = NULL;

    // Create a phrase with two terms to force STRING type path
    // Use the parser to build a more complex expression
    rc = SQLITE_OK;
    pExpr = NULL;
    rc = sqlite3Fts5ExprNew(NULL, 0, 0, "termA termB", &pExpr, &zErr);
    if( rc != SQLITE_OK || pExpr == NULL ) {
        LOG_TEST_FAIL("Clone-MultiTerms: Failed to create source expression (rc=" << rc << ")");
        return false;
    }

    rc = sqlite3Fts5ExprClonePhrase(pExpr, 0, &pClone);
    if( rc != SQLITE_OK || pClone == NULL ) {
        LOG_TEST_FAIL("Clone-MultiTerms: Cloning failed (rc=" << rc << ")");
        safe_free_expr(pExpr);
        return false;
    }

    bool ok = true;
    if( pClone->nPhrase != 1 ) {
        ok = false;
        std::cerr << "Clone-MultiTerms: Unexpected nPhrase=" << pClone->nPhrase << std::endl;
    }
    if( pClone->pRoot == NULL ) {
        ok = false;
        std::cerr << "Clone-MultiTerms: pRoot is NULL" << std::endl;
    } else {
        if( pClone->pRoot->eType != FTS5_STRING ) {
            ok = false;
            std::cerr << "Clone-MultiTerms: Unexpected eType=" << pClone->pRoot->eType << " (expected FTS5_STRING)" << std::endl;
        }
    }

    safe_free_expr(pClone);
    safe_free_expr(pExpr);

    if( ok ) {
        LOG_TEST_PASS("Clone-MultiTerms");
        return true;
    } else {
        LOG_TEST_FAIL("Clone-MultiTerms");
        return false;
    }
}

// Test 3: Clone a phrase whose original colset is NULL (no pColset copy path)
// Expected: Cloning succeeds and should not crash when pColset is absent
static bool test_clone_with_null_colset() {
    int rc = SQLITE_OK;
    Fts5Expr *pExpr = NULL;
    Fts5Expr *pClone = NULL;
    char *zErr = NULL;

    rc = SQLITE_OK;
    pExpr = NULL;
    rc = sqlite3Fts5ExprNew(NULL, 0, 0, "colsetless", &pExpr, &zErr);
    if( rc != SQLITE_OK || pExpr == NULL ) {
        LOG_TEST_FAIL("Clone-NullColset: Failed to create source expression (rc=" << rc << ")");
        return false;
    }

    // Manually tweak the source phrase so its pNear->pColset is NULL to hit the null path.
    // This relies on internal layout; we attempt to locate and nullify pColset if available.
    if( pExpr->apExprPhrase && pExpr->apExprPhrase[0] && pExpr->apExprPhrase[0]->pNode
        && pExpr->apExprPhrase[0]->pNode->pNear ) {
        pExpr->apExprPhrase[0]->pNode->pNear->pColset = NULL;
    }

    rc = sqlite3Fts5ExprClonePhrase(pExpr, 0, &pClone);
    if( rc != SQLITE_OK || pClone == NULL ) {
        LOG_TEST_FAIL("Clone-NullColset: Cloning failed (rc=" << rc << ")");
        safe_free_expr(pExpr);
        return false;
    }

    // Validate that a clone was produced and is structurally valid (even if colset was absent)
    bool ok = true;
    if( pClone->pRoot == NULL ) {
        ok = false;
        std::cerr << "Clone-NullColset: pRoot is NULL" << std::endl;
    } else {
        // No explicit expectation on eType here since path depends on parsed terms; ensure not NULL
        // If available, ensure pNear colset copy did not crash (we cannot assert on null-ness here)
    }

    safe_free_expr(pClone);
    safe_free_expr(pExpr);

    if( ok ) {
        LOG_TEST_PASS("Clone-NullColset");
        return true;
    } else {
        LOG_TEST_FAIL("Clone-NullColset");
        return false;
    }
}

// Test 4: Clone a token-less quoted phrase (e.g., MATCH "")
// Expected: Cloning should succeed by creating an empty sCtx.pPhrase and not crash
static bool test_clone_empty_token_phrase() {
    int rc = SQLITE_OK;
    Fts5Expr *pExpr = NULL;
    Fts5Expr *pClone = NULL;
    char *zErr = NULL;

    rc = SQLITE_OK;
    pExpr = NULL;
    // The quoted phrase with no token characters
    rc = sqlite3Fts5ExprNew(NULL, 0, 0, "\"\"", &pExpr, &zErr);
    if( rc != SQLITE_OK || pExpr == NULL ) {
        LOG_TEST_FAIL("Clone-EmptyToken: Failed to create source expression (rc=" << rc << ")");
        return false;
    }

    rc = sqlite3Fts5ExprClonePhrase(pExpr, 0, &pClone);
    if( rc != SQLITE_OK || pClone == NULL ) {
        LOG_TEST_FAIL("Clone-EmptyToken: Cloning failed (rc=" << rc << ")");
        safe_free_expr(pExpr);
        return false;
    }

    bool ok = true;
    if( pClone->nPhrase != 1 ) {
        ok = false;
        std::cerr << "Clone-EmptyToken: Unexpected nPhrase=" << pClone->nPhrase << std::endl;
    }

    safe_free_expr(pClone);
    safe_free_expr(pExpr);

    if( ok ) {
        LOG_TEST_PASS("Clone-EmptyToken");
        return true;
    } else {
        LOG_TEST_FAIL("Clone-EmptyToken");
        return false;
    }
}

int main() {
    // Run all unit tests for sqlite3Fts5ExprClonePhrase
    int passed = 0;
    int total = 0;

    std::cout << "Starting sqlite3Fts5ExprClonePhrase unit tests (C++11 harness)" << std::endl;

    // Test 1
    total++;
    if( test_clone_single_term_no_synonym() ) passed++;
    // Test 2
    total++;
    if( test_clone_multiple_terms_string_path() ) passed++;
    // Test 3
    total++;
    if( test_clone_with_null_colset() ) passed++;
    // Test 4
    total++;
    if( test_clone_empty_token_phrase() ) passed++;

    std::cout << "Tests passed: " << passed << "/" << total << std::endl;
    return (passed == total) ? 0 : 1;
}