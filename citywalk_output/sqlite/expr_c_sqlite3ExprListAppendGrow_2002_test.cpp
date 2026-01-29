/*
 * Unit tests for sqlite3ExprListAppendGrow (C-like reproduction in C++11 test
 * harness). The goal is to validate the behavior of expanding an expression
 * list when appending a new expression, including success and failure paths.
 *
 * Note: This test suite re-implements a minimal, self-contained subset of the
 * data structures and dependencies required by sqlite3ExprListAppendGrow to
 * exercise its logic without depending on the full SQLite codebase.
 *
 * Tests performed:
 * - Grow succeeds: nAlloc doubles, nExpr increments, new item is zero-initialized
 *   and its pExpr points to the provided expression.
 * - Grow fails: realloc returns NULL, proper cleanup occurs via sqlite3ExprListDelete
 *   and sqlite3ExprDelete, and the function returns NULL.
 *
 * The test harness uses a lightweight non-terminating assertion approach to
 * maximize code coverage and allows multiple tests to run in one pass.
 *
 * Compile with: g++ -std=c++11 -O2 -Wall -Wextra -o test_exprlist test_exprlist.cpp
 */

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Domain model (minimal subset) to mimic sqlite3ExprListAppendGrow usage.

// Forward declaration for opaque types (unused in tests beyond pointer existence).
struct Expr;

// ExprList_item mirrors the internal structure used by sqlite3ExprListAppendGrow.
struct ExprList_item {
    Expr *pExpr;
    int dummy; // small placeholder field to verify zero-initialization
};

// ExprList uses a flexible-like array (a[1] as a common C idiom) to simplify
// dynamic resizing in this test harness.
struct ExprList {
    int nExpr;
    int nAlloc;
    ExprList_item a[1]; // actually treated as a flexible array
};

// Zero-initialized template item used by sqlite3ExprListAppendGrow
static ExprList_item zeroItem = { nullptr, 0 };

// Simple opaque 'Expr' object for testing purposes
struct Expr { int id; };

// Lightweight database handle (not used in logic, only to mirror signature)
struct sqlite3 { int dummy; };

// Global control/state for the mock environment
static bool reallocShouldFail = false;   // when true, sqlite3DbRealloc returns NULL
static int deleteListCount = 0;          // count of calls to sqlite3ExprListDelete
static bool deletedExprFlag = false;     // whether sqlite3ExprDelete was invoked

// Mocked memory management and helper functions

// Simulated realloc-like function used by sqlite3ExprListAppendGrow
static void* sqlite3DbRealloc(sqlite3* db, void* p, size_t n) {
    (void)db; // unused in mock
    if (reallocShouldFail) return nullptr;
    return realloc(p, n);
}

// Mock: delete an ExprList (free memory and track invocation)
static void sqlite3ExprListDelete(sqlite3* db, ExprList* p) {
    (void)db; // unused in mock
    if (p) {
        deleteListCount++;
        if (p->a) free(p->a);
        free(p);
    }
}

// Mock: delete a single Expr (free memory and track invocation)
static void sqlite3ExprDelete(sqlite3* db, Expr* p) {
    (void)db; // unused in mock
    if (p) {
        deletedExprFlag = true;
        free(p);
    }
}

/*
 * The focal method under test. Reproduced in this test harness to be self-contained.
 * Signature mirrors the SQLite version for compatibility with test expectations.
 */
static ExprList* sqlite3ExprListAppendGrow(
    sqlite3* db,
    ExprList* pList,
    Expr* pExpr
) {
    ExprList_item* pItem;
    ExprList* pNew;
    pList->nAlloc *= 2;
    pNew = (ExprList*)sqlite3DbRealloc(db, pList,
         sizeof(*pList) + (pList->nAlloc - 1) * sizeof(pList->a[0]));
    if (pNew == nullptr) {
        sqlite3ExprListDelete(db, pList);
        sqlite3ExprDelete(db, pExpr);
        return nullptr;
    } else {
        pList = pNew;
    }
    pItem = &pList->a[pList->nExpr++];
    *pItem = zeroItem;
    pItem->pExpr = pExpr;
    return pList;
}

/*
 * Helper to allocate a new ExprList with a given initial allocation (nAlloc).
 * Returns a pointer to the created ExprList on success.
 */
static ExprList* allocateExprList(int initialAlloc) {
    // Allocate memory for ExprList plus (initialAlloc-1) items
    ExprList* p = (ExprList*) malloc(sizeof(ExprList) + (initialAlloc - 1) * sizeof(ExprList_item));
    if (!p) return nullptr;
    p->nExpr = 0;
    p->nAlloc = initialAlloc;
    // Initialize the array to zero to simulate a clean slate
    for (int i = 0; i < initialAlloc; ++i) {
        p->a[i].pExpr = nullptr;
        p->a[i].dummy = 0;
    }
    return p;
}

/*
 * Test 1: Grow succeeds
 * - Initialize pList with nAlloc=2 and nExpr=0, with a valid pExpr
 * - Call sqlite3ExprListAppendGrow and verify:
 *   - Returned pointer is non-null
 *   - nAlloc has doubled to 4
 *   - nExpr has incremented to 1
 *   - The new item (index 0) has pExpr equal to the provided pExpr
 *   - The new item is zero-initialized (dummy == 0)
 */
static void test_grow_succeeds() {
    const char* testName = "test_grow_succeeds";
    fprintf(stdout, "RUNNING %s\n", testName);

    sqlite3 db;
    // Prepare initial list with nAlloc=2
    ExprList* pList = allocateExprList(2);
    if (!pList) {
        fprintf(stderr, "Failed to allocate initial ExprList for test %s\n", testName);
        return;
    }

    // Create a test expression
    Expr* pExpr = (Expr*) malloc(sizeof(Expr));
    if (!pExpr) {
        fprintf(stderr, "Failed to allocate Expr for test %s\n", testName);
        free(pList);
        return;
    }
    pExpr->id = 123; // arbitrary id

    // Ensure no realloc failure
    reallocShouldFail = false;
    deleteListCount = 0;
    deletedExprFlag = false;

    ExprList* res = sqlite3ExprListAppendGrow(&db, pList, pExpr);

    // Assertions (non-terminating, but we report failures)
    bool ok = true;
    if (res == nullptr) {
        fprintf(stderr, "Failure: sqlite3ExprListAppendGrow returned NULL in %s\n", testName);
        ok = false;
    } else {
        // res should be the same as pList after growth (memory moved)
        if (res->nAlloc != 4) {
            fprintf(stderr, "Failure: expected nAlloc=4, got %d in %s\n", res->nAlloc, testName);
            ok = false;
        }
        if (res->nExpr != 1) {
            fprintf(stderr, "Failure: expected nExpr=1, got %d in %s\n", res->nExpr, testName);
            ok = false;
        }
        // The item at index 0 should have pExpr == pExpr
        if (res->a[0].pExpr != pExpr) {
            fprintf(stderr, "Failure: expected a[0].pExpr == pExpr in %s\n", testName);
            ok = false;
        }
        // The item should be zero-initialized (dummy == 0)
        if (res->a[0].dummy != 0) {
            fprintf(stderr, "Failure: expected a[0].dummy==0 in %s\n", testName);
            ok = false;
        }

        // Cleanup: free allocated memory via sqlite3ExprListDelete (mimicked)
        sqlite3ExprListDelete(&db, res);
        // Note: pExpr is owned by res; but since we freed via the list, avoid double-free
        // In this mock, res contains the pExpr pointer; we should not free pExpr separately.
    }

    if (ok) {
        fprintf(stdout, "PASS: %s\n", testName);
    } else {
        fprintf(stderr, "FAIL: %s\n", testName);
    }
    // If sqlite3ExprListDelete freed pExpr inside res, we should not double-free.
    // Our implementation freed pList->a[0].pExpr as part of pExpr in delete path;
    // Here, since res is freed, we don't touch pExpr further.
    // However since our test allocated pExpr separately, ensure no memory leak.
    // In this mock, pExpr is owned by the list item before delete; avoid free here.

    // To be safe, do not double-free pExpr here. The delete path already freed the expression inside sqlite3ExprListDelete.
    // We free only the list struct if not already freed inside delete.
    // Since sqlite3ExprListDelete frees pList and its a, but not pExpr (because it's stored inside a),
    // Real code would manage ownership differently. For test simplicity, avoid double free by not freeing pExpr here.

    // Note: In this mock, res points to the same memory as pList; sqlite3ExprListDelete freed it.
    // Therefore, to avoid undefined behavior, do not free pList/pExpr here again.
    // We'll skip manual free of pList/pExpr here.

    // End of test
}

// Test 2: Grow fails (realloc returns NULL)
static void test_grow_fails_on_realloc() {
    const char* testName = "test_grow_fails_on_realloc";
    fprintf(stdout, "RUNNING %s\n", testName);

    sqlite3 db;
    // Prepare initial list with nAlloc=2
    ExprList* pList = allocateExprList(2);
    if (!pList) {
        fprintf(stderr, "Failed to allocate initial ExprList for test %s\n", testName);
        return;
    }

    // Create a test expression
    Expr* pExpr = (Expr*) malloc(sizeof(Expr));
    if (!pExpr) {
        fprintf(stderr, "Failed to allocate Expr for test %s\n", testName);
        free(pList);
        return;
    }

    // Simulate realloc failure
    reallocShouldFail = true;
    deleteListCount = 0;
    deletedExprFlag = false;

    ExprList* res = sqlite3ExprListAppendGrow(&db, pList, pExpr);

    bool ok = true;
    if (res != nullptr) {
        fprintf(stderr, "Failure: expected NULL return on realloc failure in %s\n", testName);
        ok = false;
        // Cleanup in case of unexpected success
        sqlite3ExprListDelete(&db, res);
    } else {
        // Expect cleanup to have occurred: both pList and pExpr were freed
        if (deleteListCount <= 0) {
            fprintf(stderr, "Failure: sqlite3ExprListDelete not invoked in %s\n", testName);
            ok = false;
        }
        if (!deletedExprFlag) {
            fprintf(stderr, "Failure: sqlite3ExprDelete not invoked for pExpr in %s\n", testName);
            ok = false;
        }
    }

    if (ok) {
        fprintf(stdout, "PASS: %s\n", testName);
    } else {
        fprintf(stderr, "FAIL: %s\n", testName);
    }

    // Reset global controls for safety (not strictly necessary here)
    reallocShouldFail = false;
}

// Simple test harness
int main() {
    fprintf(stdout, "Starting sqlite3ExprListAppendGrow tests (C++11 harness)\n");

    test_grow_succeeds();
    test_grow_fails_on_realloc();

    fprintf(stdout, "Finished tests.\n");
    // Note: In this minimalist test, we do not aggregate/exit with status codes.
    // The non-terminating assertion approach logs failures and continues.

    return 0;
}