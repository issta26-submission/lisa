// Unit tests for sqlite3Fts5ExprAnd
// Target: test the focal method sqlite3Fts5ExprAnd from fts5_expr.c
// Environment: C++11, no Google Test, simple in-file test harness
// Notes:
// - This test relies on the real project headers for Fts5Expr, Fts5Parse, Fts5ExprPhrase, etc.
// - We allocate memory using malloc/free to keep compatibility with the C code under test.
// - We exercise true/false branches of the dependent condition in sqlite3Fts5ExprAnd.
// - We cover a scenario where multiple phrases from two expressions are merged to ensure the realloc/memmove logic is exercised.
// - We provide explanatory comments for each unit test as requested.

#include <fts5Int.h>
#include <fts5parse.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Import C headers with C linkage for the SQLite FTS5 types and functions.
extern "C" {
}

// Helper: create a minimal Fts5Expr with a given number of phrases.
// We allocate the Fts5Expr and its apExprPhrase array (and inner phrase objects) manually.
// This avoids pulling in full construction logic from the library, but provides a valid
// structure suitable for sqlite3Fts5ExprAnd() testing.
static Fts5Expr* create_expr_with_phrases(int n) {
    Fts5Expr* e = (Fts5Expr*)malloc(sizeof(Fts5Expr));
    if (!e) return NULL;
    e->nPhrase = n;
    e->pRoot = NULL;

    if (n > 0) {
        // Allocate an array for phrase pointers using sqlite3_realloc semantics.
        e->apExprPhrase = (Fts5ExprPhrase**)sqlite3_realloc(NULL, n * sizeof(Fts5ExprPhrase*));
        if (!e->apExprPhrase) {
            free(e);
            return NULL;
        }
        // Allocate each Fts5ExprPhrase object
        for (int i = 0; i < n; ++i) {
            e->apExprPhrase[i] = (Fts5ExprPhrase*)malloc(sizeof(Fts5ExprPhrase));
            // Its content is not used by sqlite3Fts5ExprAnd beyond pointer copying.
        }
    } else {
        e->apExprPhrase = NULL;
    }

    return e;
}

// Simple test harness for reporting
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_OK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILED: " << (msg) << std::endl; \
        ++g_failed_tests; \
    } else { \
        std::cout << "TEST PASSED: " << (msg) << std::endl; \
    } \
    ++g_total_tests; \
} while(0)

// Test 1: True branch: both pp1 and p2 non-null; verify merging behavior and internal pointer rearrangement.
static void test_sqlite3Fts5ExprAnd_merge_two_terms() {
    // Arrange: create p1 with 2 phrases, p2 with 1 phrase
    Fts5Expr *p1 = create_expr_with_phrases(2);
    Fts5Expr *p2 = create_expr_with_phrases(1);
    if (!p1 || !p2) {
        TEST_OK(false, "malloc failure in test_sqlite3Fts5ExprAnd_merge_two_terms");
        return;
    }

    // Keep original pointers for comparison after the call
    Fts5ExprPhrase *p1p0_old = p1->apExprPhrase[0];
    Fts5ExprPhrase *p1p1_old = p1->apExprPhrase[1];
    Fts5ExprPhrase *p2p0_old = p2->apExprPhrase[0];

    // pp1 points to p1
    Fts5Expr *pp1 = p1;

    // Act: call the function under test
    int rc = sqlite3Fts5ExprAnd(&pp1, p2);

    // Assert: rc should be SQLITE_OK (normal parse)
    TEST_OK(rc == SQLITE_OK, "rc == SQLITE_OK after sqlite3Fts5ExprAnd merge");

    // The result pointer should still point to p1
    TEST_OK(pp1 == p1, "pp1 should remain pointing to the original p1 after merge");

    // The number of phrases should be the sum: 2 + 1 = 3
    TEST_OK(p1->nPhrase == 3, "p1->nPhrase == 3 after merge");

    // After the realloc/work, the apExprPhrase layout should be:
    // ap[0] = p2->apExprPhrase[0], ap[1] = old ap[0], ap[2] = old ap[1]
    TEST_OK(p1->apExprPhrase[0] == p2p0_old, "apExprPhrase[0] is the original p2 phrase");
    TEST_OK(p1->apExprPhrase[1] == p1p0_old, "apExprPhrase[1] is the original p1 first phrase");
    TEST_OK(p1->apExprPhrase[2] == p1p1_old, "apExprPhrase[2] is the original p1 second phrase");

    // Cleanup: free allocated resources. Be cautious to avoid double-free.
    // We own all Fts5ExprPhrase objects now via p1->apExprPhrase.
    for (int i = 0; i < p1->nPhrase; ++i) {
        free(p1->apExprPhrase[i]);
    }
    free(p1->apExprPhrase);
    free(p1);
    // Note: p2's phrase array memory was freed by the function; p2 itself has been leaked in this test harness scope.
    // In a full test environment, ensure proper allocator/deallocator symmetry. Here we focus on functional correctness.
}

// Test 2: False branch: *pp1 is NULL but p2 is non-null; ensure *pp1 becomes p2.
static void test_sqlite3Fts5ExprAnd_assign_when_pp1_null() {
    // Arrange: create p2 with 2 phrases
    Fts5Expr *p2 = create_expr_with_phrases(2);
    if (!p2) {
        TEST_OK(false, "malloc failure in test_sqlite3Fts5ExprAnd_assign_when_pp1_null");
        return;
    }

    Fts5Expr *pp1 = NULL;

    // Act
    int rc = sqlite3Fts5ExprAnd(&pp1, p2);

    // Assert
    TEST_OK(rc == SQLITE_OK, "rc == SQLITE_OK when pp1 is NULL and p2 is non-null");
    TEST_OK(pp1 == p2, "pp1 should become p2 when pp1 was NULL");

    // Cleanup: free p2's resources (including inner phrases)
    for (int i = 0; i < p2->nPhrase; ++i) {
        free(p2->apExprPhrase[i]);
    }
    free(p2->apExprPhrase);
    free(p2);
}

// Test 3: False branch: p2 is NULL; ensure no changes to *pp1 and rc is SQLITE_OK.
static void test_sqlite3Fts5ExprAnd_no_op_when_p2_null() {
    // Arrange: p1 with 1 phrase
    Fts5Expr *p1 = create_expr_with_phrases(1);
    if (!p1) {
        TEST_OK(false, "malloc failure in test_sqlite3Fts5ExprAnd_no_op_when_p2_null");
        return;
    }

    Fts5Expr *pp1 = p1;
    Fts5Expr *p2 = NULL;

    // Keep original reference for later comparison
    Fts5ExprPhrase *p1p0_old = p1->apExprPhrase[0];

    // Act
    int rc = sqlite3Fts5ExprAnd(&pp1, p2);

    // Assert
    TEST_OK(rc == SQLITE_OK, "rc == SQLITE_OK when p2 is NULL");
    TEST_OK(pp1 == p1, "pp1 should remain as p1 when p2 is NULL");
    TEST_OK(p1->nPhrase == 1, "p1->nPhrase remains unchanged when p2 is NULL");
    TEST_OK(p1->apExprPhrase[0] == p1p0_old, "p1's phrase remains unchanged after no-op call");

    // Cleanup: free p1 resources
    for (int i = 0; i < p1->nPhrase; ++i) {
        free(p1->apExprPhrase[i]);
    }
    free(p1->apExprPhrase);
    free(p1);
}

// Main launcher
int main() {
    std::cout << "SQLite3 FTS5 expression AND() unit tests (C++11 harness) start\n";

    test_sqlite3Fts5ExprAnd_merge_two_terms();
    test_sqlite3Fts5ExprAnd_assign_when_pp1_null();
    test_sqlite3Fts5ExprAnd_no_op_when_p2_null();

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}