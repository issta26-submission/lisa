/*
 * Unit test suite for the focal method:
 *   int sqlite3Fts5ExprNext(Fts5Expr *p, i64 iLast)
 *
 * Notes:
 * - This test harness is written for C++11, without Google Test.
 * - It uses lightweight, non-terminating assertions so that all tests
 *   execute and report their results.
 * - The tests rely on the public behavior of sqlite3Fts5ExprNext as observed
 *   through its observable side effects on the Fts5Expr/Fts5ExprNode structures.
 * - Because sqlite3Fts5ExprNext is part of a larger C project with internal
 *   dependencies (e.g., fts5ExprNodeNext and fts5RowidCmp), the tests construct
 *   minimal, compatible shims for the parts required by sqlite3Fts5ExprNext during
 *   runtime (iRoot, iRowid, bNomatch, bEof). We assume a compatible ABI with the
 *   project headers included in the build environment.
 *
 * This test focuses on:
 * - true/false branches of the bNomatch and bEof related logic
 * - the rowid comparison outcome affecting bEof
 * - the loop behavior with bNomatch toggling
 *
 * Because the underlying fts5ExprNodeNext is a static (internal) function in the
 * original file, we do not mock it here; instead we exercise the real function in a
 * controlled state and verify deterministic side effects on the public fields.
 *
 * How to run (example):
 *   g++ -std=c++11 -c fts5_expr.c -o fts5_expr.o
 *   g++ -std=c++11 test_fts5expr_next.cpp fts5_expr.o -o test_fts5expr_next
 *   ./test_fts5expr_next
 *
 * If your build system uses different compilation units, ensure that:
 * - The translation unit containing sqlite3Fts5ExprNext is linked.
 * - The Fts5Expr and Fts5ExprNode types used in this test are ABI-compatible
 *   with those defined in the project (use the project's headers).
 *
 * Explanatory comments accompany each unit test.
 */

#include <fts5Int.h>
#include <cstdio>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declare the C function under test.
// Note: The actual signature in the project is:
extern "C" int sqlite3Fts5ExprNext(/*Fts5Expr*/ void* p, long long iLast);

// Minimal ABI-compatible type aliases to align with the project.
// We keep these minimal and rely on the project's real definitions in the linked library.
typedef long long i64;

// Forward declare minimal shapes. We do not rely on the full
// project headers here; we assume the following fields exist in the real types.
// This is only to craft tests against observable side effects.
struct Fts5ExprNode {
    int bEof;
    int bNomatch;
    i64 iRowid;
    // ... other fields are not accessed directly by this test.
};

struct Fts5Expr {
    Fts5ExprNode *pRoot;
    // ... other fields are not accessed directly by this test.
};

// Lightweight assertion helper that does not terminate the test execution.
static int g_failures = 0;
static void assert_true(bool cond, const char* msg) {
    if (!cond) {
        g_failures++;
        std::cerr << "ASSERT FAIL: " << msg << std::endl;
    }
}

// Helper to allocate and initialize a root node
static Fts5ExprNode* alloc_root_node(int bEof, int bNomatch, i64 iRowid) {
    Fts5ExprNode* p = (Fts5ExprNode*)std::malloc(sizeof(Fts5ExprNode));
    if (!p) {
        std::cerr << "Allocation failure for Fts5ExprNode" << std::endl;
        std::exit(1);
    }
    p->bEof = bEof;
    p->bNomatch = bNomatch;
    p->iRowid = iRowid;
    return p;
}

// Helper to allocate an Fts5Expr with a given root
static Fts5Expr* alloc_expr_with_root(Fts5ExprNode* pRoot) {
    Fts5Expr* p = (Fts5Expr*)std::malloc(sizeof(Fts5Expr));
    if (!p) {
        std::cerr << "Allocation failure for Fts5Expr" << std::endl;
        std::exit(1);
    }
    p->pRoot = pRoot;
    return p;
}

// Cleanup helpers
static void free_expr(Fts5Expr* p) {
    if (p) {
        if (p->pRoot) std::free(p->pRoot);
        std::free(p);
    }
}

/*
 * Test 1: Nomatch == 0 path
 * - Setup: pRoot->bNomatch == 0; iRowid > iLast to trigger bEof = 1 after the call.
 * - Expectation: After sqlite3Fts5ExprNext returns, pRoot->bEof == 1.
 * - The return code rc is taken from fts5ExprNodeNext; we simply ensure the call completes.
 */
static void test_sqlite3Fts5ExprNext_nomatch_zero_eof_true() {
    // Arrange
    i64 iLast = 10;
    i64 rootRowid = 20; // greater than iLast to trigger bEof
    Fts5ExprNode* pRoot = alloc_root_node(0 /* bEof */, 0 /* bNomatch */, rootRowid);
    Fts5Expr* p = alloc_expr_with_root(pRoot);

    // Act
    int rc = sqlite3Fts5ExprNext(p, iLast);

    // Assert
    // bEof should be set to 1 since rootRowid > iLast
    assert_true(pRoot->bEof == 1, "Test1: bEof should be set to 1 when iRowid > iLast");
    // We cannot guarantee a specific rc (depends on inner implementation); ensure it returns with a non-crash value.
    assert_true(rc >= 0, "Test1: rc is non-negative (typical SQLITE_OK on success)");
    free_expr(p);
}

/*
 * Test 2: Nomatch == 0 path with no EOF
 * - Setup: pRoot->bNomatch == 0; iRowid <= iLast so bEof should remain unchanged.
 * - Expectation: pRoot->bEof remains whatever it was before (we set to 0).
 */
static void test_sqlite3Fts5ExprNext_nomatch_zero_no_eof() {
    // Arrange
    i64 iLast = 100;
    i64 rootRowid = 90; // not greater than iLast
    Fts5ExprNode* pRoot = alloc_root_node(0 /* bEof */, 0 /* bNomatch */, rootRowid);
    Fts5Expr* p = alloc_expr_with_root(pRoot);

    // Act
    int rc = sqlite3Fts5ExprNext(p, iLast);

    // Assert
    // bEof should remain 0 because iRowid <= iLast
    assert_true(pRoot->bEof == 0, "Test2: bEof should remain 0 when iRowid <= iLast");
    // rc should be non-negative in a successful run
    assert_true(rc >= 0, "Test2: rc is non-negative when successful");
    free_expr(p);
}

/*
 * Test 3: Nomatch toggling behavior
 * - Setup: pRoot->bNomatch == 1 so the loop executes at least once.
 * - We rely on the internal fts5ExprNodeNext to eventually clear bNomatch to 0.
 * - After the loop, perform a deterministic check on bEof (if iRowid > iLast).
 */
static void test_sqlite3Fts5ExprNext_nomatch_one_then_resolve() {
    // Arrange
    i64 iLast = 15;
    i64 rootRowid = 12; // less than iLast; ensure no EOF set after check
    Fts5ExprNode* pRoot = alloc_root_node(0 /* bEof */, 1 /* bNomatch */, rootRowid);
    Fts5Expr* p = alloc_expr_with_root(pRoot);

    // Act
    int rc = sqlite3Fts5ExprNext(p, iLast);

    // Assert
    // After the call, we expect either:
    // - bNomatch to be cleared by the internal logic (unknown here),
    // - and bEof set only if iRowid > iLast (not in this case).
    // We check for non-crash and a sane rc, and that bEof did not become 1 in this scenario.
    assert_true(pRoot->bEof == 0, "Test3: bEof should remain 0 when iRowid <= iLast after resolution");
    assert_true(rc >= 0, "Test3: rc is non-negative after nomatch-driven advancement");
    free_expr(p);
}

/*
 * Test 4: Boundary when iLast is exactly one less than iRowid
 * - Setup: iLast = rootRowid - 1; expect bEof to be set to 1 after function returns.
 */
static void test_sqlite3Fts5ExprNext_boundary_eof() {
    // Arrange
    i64 rootRowid = 7;
    i64 iLast = rootRowid - 1;
    Fts5ExprNode* pRoot = alloc_root_node(0 /* bEof */, 0 /* bNomatch */, rootRowid);
    Fts5Expr* p = alloc_expr_with_root(pRoot);

    // Act
    int rc = sqlite3Fts5ExprNext(p, iLast);

    // Assert
    assert_true(pRoot->bEof == 1, "Test4: bEof should be 1 when iRowid > iLast by boundary condition");
    assert_true(rc >= 0, "Test4: rc is non-negative after boundary condition");
    free_expr(p);
}

// Entry point
int main(void) {
    std::cout << "Running test suite for sqlite3Fts5ExprNext (Fts5ExprNext) ..." << std::endl;

    test_sqlite3Fts5ExprNext_nomatch_zero_eof_true();
    test_sqlite3Fts5ExprNext_nomatch_zero_no_eof();
    test_sqlite3Fts5ExprNext_nomatch_one_then_resolve();
    test_sqlite3Fts5ExprNext_boundary_eof();

    if (g_failures == 0) {
        std::cout << "All tests completed. Summary: PASS" << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with failures: " << g_failures << std::endl;
        return 2;
    }
}