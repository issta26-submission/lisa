// Tests for the focal method: sqlite3ExprListAppendNew
// This file provides a self-contained test harness (no GTest/GMock) for the C-style
// function sqlite3ExprListAppendNew, reproducing the minimal dependencies needed
// to exercise the function's two main branches:
// - Allocation failure branch (pList == 0) => sqlite3ExprDelete is invoked and NULL is returned
// - Allocation success branch (pList != 0) => pList is initialized with expected fields and first item

// Step 1: Candidate Keywords (core dependencies extracted from the focal method)
// - sqlite3: Database handle type used for memory allocation
// - Expr: Expression node type to be appended
// - ExprList: List structure that stores expressions and its internal array a[4]
// - ExprList_item: element in the ExprList's internal array (contains a pointer to Expr)
// - zeroItem: a static sentinel ExprList_item used to initialize a new item
// - sqlite3DbMallocRawNN: low-level allocator used by sqlite3ExprListAppendNew
// - sqlite3ExprDelete: cleanup function called on pExpr when allocation fails
// - pExpr: input expression to be appended (can be NULL)
// - nAlloc, nExpr: allocator count and current expression count in ExprList
// - a[4]: fixed-size internal array for up to 4 expressions in this simplified test
// - NULL vs non-NULL handling in pExpr for correctness verification

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// Step 1: Minimal type definitions to mirror the focal function dependencies

// Forward declare to mimic C-style opaque types
struct sqlite3;
struct Expr;
struct ExprList_item;
struct ExprList;

// Sentinel item used to initialize a new list item
static ExprList_item zeroItem = { nullptr };

// Minimal sqlite3 type (opaque in this test)
struct sqlite3 {
    int dummy;
};

// Expression node (simple placeholder)
struct Expr {
    int dummy;
};

// An item within ExprList (holds a pointer to an Expr)
struct ExprList_item {
    Expr *pExpr;
};

// The expression list that will be allocated by sqlite3ExprListAppendNew
// It stores up to 4 expressions and tracks allocation/usage
struct ExprList {
    int nAlloc;           // number of allocated items in a[]
    int nExpr;            // number of expressions currently stored
    ExprList_item a[4];   // fixed-size internal array (mimics dynamic array in real SQLite)
};

// Debug/Control flags for test harness
static int g_mallocFail = 0;      // when non-zero, sqlite3DbMallocRawNN should fail (return NULL)
static int g_exprDeleted = 0;     // whether sqlite3ExprDelete has been invoked to clean up pExpr

// Forward declarations of the dependencies used by the focal method
extern "C" {
    // Memory allocator used by the focal function
    void* sqlite3DbMallocRawNN(sqlite3 *db, size_t n);

    // Cleanup for Expr passed to the focal function on allocation failure
    void sqlite3ExprDelete(sqlite3 *db, Expr *p);

    // The focal function under test (re-declared here for the self-contained harness)
    ExprList* sqlite3ExprListAppendNew(sqlite3 *db, Expr *pExpr);
}

// --- Implementation of test harness helpers (to simulate SQLite-like behavior) ---

// Simulated allocator: respects g_mallocFail to simulate allocation failure
extern "C" void* sqlite3DbMallocRawNN(sqlite3 *db, size_t n) {
    (void)db; // unused in this simplified test
    (void)n;  // size used for the allocation, not validated here
    if (g_mallocFail) return nullptr;
    return std::malloc(n);
}

// Simulated expr deletion: records that deletion occurred and frees memory if needed
extern "C" void sqlite3ExprDelete(sqlite3 *db, Expr *p) {
    (void)db;
    if (p) {
        delete p;
    }
    g_exprDeleted = 1;
}

// The focal method copied (simplified) to operate on the test's types
ExprList* sqlite3ExprListAppendNew(sqlite3 *db, Expr *pExpr) {
    struct ExprList_item *pItem;
    ExprList *pList;
    // Important: sizeof(pList->a[0]) is used on a pointer whose type is known; it's a compile-time size
    pList = static_cast<ExprList*>(sqlite3DbMallocRawNN(db, sizeof(ExprList) + sizeof(pList->a[0]) * 4));
    if (pList == 0) {
        sqlite3ExprDelete(db, pExpr);
        return 0;
    }
    pList->nAlloc = 4;
    pList->nExpr = 1;
    pItem = &pList->a[0];
    *pItem = zeroItem;
    pItem->pExpr = pExpr;
    return pList;
}

// --- Simple test harness (no GTest) ---

static int s_tests_run = 0;
static int s_tests_failed = 0;

static void logPass(const std::string &name) {
    std::cout << "[PASS] " << name << "\n";
    ++s_tests_run;
}

static void logFail(const std::string &name, const std::string &reason) {
    std::cout << "[FAIL] " << name << " - " << reason << "\n";
    ++s_tests_run;
    ++s_tests_failed;
}

// Helper to reset harness state between tests
static void resetHarnessState() {
    g_mallocFail = 0;
    g_exprDeleted = 0;
}

// Test 1: Allocation failure path
// - Simulate sqlite3DbMallocRawNN returning NULL
// - Expect sqlite3ExprListAppendNew to return NULL and to call sqlite3ExprDelete(pExpr)
static void test_exprListAppendNew_allocationFailure() {
    const std::string testName = "test_exprListAppendNew_allocationFailure";
    resetHarnessState();
    g_mallocFail = 1; // force malloc to fail

    // Prepare a fake db and a single expression
    sqlite3 db{};
    Expr *pExpr = new Expr{0};

    ExprList *pList = sqlite3ExprListAppendNew(&db, pExpr);

    // Verify behavior: NULL return and expr deletion invoked
    if (pList != nullptr) {
        logFail(testName, "Expected NULL when allocation fails, got non-NULL.");
        // Clean up to avoid leaks
        // pList is non-NULL; but we expect NULL, so this path shouldn't occur
        delete pExpr;
        if (pList) { std::free(pList); }
    } else {
        if (g_exprDeleted != 1) {
            logFail(testName, "Expected sqlite3ExprDelete to be invoked on pExpr.");
        } else {
            logPass(testName);
        }
    }

    // If allocation failed, pExpr should have been deleted by sqlite3ExprDelete
    // No further action required
}

// Test 2: Allocation success path with non-NULL pExpr
// - Expect pList to be non-NULL with proper initialization:
//   nAlloc == 4, nExpr == 1, first item's pExpr == pExpr
static void test_exprListAppendNew_allocationSuccess_nonNullExpr() {
    const std::string testName = "test_exprListAppendNew_allocationSuccess_nonNullExpr";
    resetHarnessState();
    g_mallocFail = 0; // allow allocation

    sqlite3 db{};
    Expr *pExpr = new Expr{42};

    ExprList *pList = sqlite3ExprListAppendNew(&db, pExpr);

    if (pList == nullptr) {
        logFail(testName, "Expected non-NULL ExprList on successful allocation.");
        delete pExpr;
        return;
    }

    // Verify structural initialization
    bool ok = true;
    if (pList->nAlloc != 4) {
        logFail(testName, "nAlloc should be 4.");
        ok = false;
    }
    if (pList->nExpr != 1) {
        logFail(testName, "nExpr should be 1.");
        ok = false;
    }
    if (pList->a[0].pExpr != pExpr) {
        logFail(testName, "First ExprList_item.pExpr should point to the original pExpr.");
        ok = false;
    }

    if (ok) {
        logPass(testName);
    }

    // Cleanup: pExpr is owned by the test and the list stores it; free both
    delete pExpr;
    std::free(pList);
}

// Test 3: Allocation success path with NULL pExpr
// - Ensure pExpr can be NULL and is stored as NULL in the first item
static void test_exprListAppendNew_allocationSuccess_nullExpr() {
    const std::string testName = "test_exprListAppendNew_allocationSuccess_nullExpr";
    resetHarnessState();
    g_mallocFail = 0; // allow allocation

    sqlite3 db{};
    Expr *pExpr = nullptr; // explicit NULL

    ExprList *pList = sqlite3ExprListAppendNew(&db, pExpr);

    if (pList == nullptr) {
        logFail(testName, "Expected non-NULL ExprList even when pExpr is NULL.");
        return;
    }

    bool ok = true;
    if (pList->nAlloc != 4) {
        logFail(testName, "nAlloc should be 4.");
        ok = false;
    }
    if (pList->nExpr != 1) {
        logFail(testName, "nExpr should be 1.");
        ok = false;
    }
    if (pList->a[0].pExpr != nullptr) {
        logFail(testName, "First ExprList_item.pExpr should be NULL when pExpr is NULL.");
        ok = false;
    }

    if (ok) {
        logPass(testName);
    }

    // Cleanup
    std::free(pList);
}

// Step 3: Test harness runner
int main() {
    std::cout << "Running sqlite3ExprListAppendNew test suite (self-contained, no GTest)..." << std::endl;

    test_exprListAppendNew_allocationFailure();
    test_exprListAppendNew_allocationSuccess_nonNullExpr();
    test_exprListAppendNew_allocationSuccess_nullExpr();

    std::cout << "Tests run: " << s_tests_run << ", Passed: " << (s_tests_run - s_tests_failed)
              << ", Failed: " << s_tests_failed << std::endl;

    // Exit status: 0 if all tests pass, 1 otherwise
    return (s_tests_failed == 0) ? 0 : 1;
}