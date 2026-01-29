// Lightweight C++11 unit test for the focal method sqlite3PExpr
// This test provides minimal stubs to exercise sqlite3PExpr's allocation path
// and its failure path, without depending on the full SQLite project.
// NOTE: This is a simplified, self-contained harness intended for demonstration.
// It compiles in a C++11 environment and uses a tiny, non-terminating assertion
// approach. The real SQLite environment may differ; adapt as needed for integration.

/*
Test goals:
- Coverage for the true allocation path:
  - Returned Expr is non-null
  - op field is masked with 0xff
  - iAgg is initialized to -1
  - Left/Right subtrees are attached to the new Expr

- Coverage for the false allocation path (allocation failure):
  - Returned Expr is NULL
  - Left/Right subtrees are deleted exactly once

Notes:
- We implement minimal stubs for types and functions used by sqlite3PExpr
  to enable isolated unit testing without the full SQLite codebase.
- We track deletions of the left and right operands to verify proper cleanup.
- We use our own tiny assertion framework to allow multiple tests to run
  even if some assertions fail.
*/

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// Lightweight, test-scoped "public API" to interact with sqlite3PExpr
extern "C" {

// Forward declarations of minimal types to satisfy sqlite3PExpr prototype
struct sqlite3;      // dummy database object
struct Expr;         // expression node
struct Parse;        // parsing context

// Global memory-allocation control for tests
extern bool g_failAlloc;

// Minimal Parse and Expr definitions to align with sqlite3PExpr usage
struct Parse {
    sqlite3* db;
};

struct Expr {
    unsigned int op;
    int iAgg;
    int nHeight;
    Expr* pLeft;
    Expr* pRight;
    int tag; // test-only tag to identify leaves in deletion tracking
};

// Function prototypes (expected from the focal codebase)
Expr* sqlite3PExpr(Parse* pParse, int op, Expr* pLeft, Expr* pRight);

// Dependencies used by sqlite3PExpr (stubs for testing)
void* sqlite3DbMallocRawNN(void* db, size_t size);
void sqlite3ExprAttachSubtrees(void* db, Expr* pRoot, Expr* pLeft, Expr* pRight);
void sqlite3ExprCheckHeight(Parse* pParse, int nHeight);
void sqlite3ExprDelete(void* db, Expr* p);

} // extern "C"

// Global test state
static bool g_failAlloc = false;          // when true, allocation will fail
static int g_leftDeletedCount = 0;        // count of deleted left leaves
static int g_rightDeletedCount = 0;       // count of deleted right leaves

// Utility to reset per-test counters
static void resetDeletionCounters() {
    g_leftDeletedCount = 0;
    g_rightDeletedCount = 0;
}

// Small, non-terminating assertion helpers
static int g_testPasses = 0;
static int g_testFailures = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cout << "TEST FAILED: " << msg << "\n"; \
            ++g_testFailures; \
        } else { \
            ++g_testPasses; \
        } \
    } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if(((a) != (b))) { \
            std::cout << "TEST FAILED: " << msg << " (expected " << (b) << ", got " << (a) << ")\n"; \
            ++g_testFailures; \
        } else { \
            ++g_testPasses; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(p, msg) \
    do { \
        if((p) == nullptr) { \
            std::cout << "TEST FAILED: " << msg << " (pointer is null)\n"; \
            ++g_testFailures; \
        } else { \
            ++g_testPasses; \
        } \
    } while(0)

#define ASSERT_NULL(p, msg) \
    do { \
        if((p) != nullptr) { \
            std::cout << "TEST FAILED: " << msg << " (pointer is not null)\n"; \
            ++g_testFailures; \
        } else { \
            ++g_testPasses; \
        } \
    } while(0)


// Implementation of test-scoped stubs (C linkage to mirror sqlite3PExpr usage)

// Simple, dummy database object (no behavior needed for tests)
struct sqlite3 {};

// Fake allocation function: respects g_failAlloc to simulate OOM
extern "C" void* sqlite3DbMallocRawNN(void* db, size_t size) {
    (void)db; // unused in test
    if(g_failAlloc) {
        return nullptr;
    }
    void* p = std::malloc(size);
    if(p) std::memset(p, 0, size);
    return p;
}

// Attach left/right subtrees to root (simulate SQLite behavior)
extern "C" void sqlite3ExprAttachSubtrees(void* db, Expr* pRoot, Expr* pLeft, Expr* pRight) {
    (void)db;
    if(pRoot) {
        pRoot->pLeft = pLeft;
        pRoot->pRight = pRight;
    }
    (void)pLeft; (void)pRight;
}

// Height check stub (no-op for test)
extern "C" void sqlite3ExprCheckHeight(Parse* pParse, int nHeight) {
    (void)pParse;
    (void)nHeight;
}

// Destructor for expression trees with deletion tracking for test
extern "C" void sqlite3ExprDelete(void* db, Expr* p) {
    if(!p) return;
    // Recursively delete children first
    if(p->pLeft) sqlite3ExprDelete(db, p->pLeft);
    if(p->pRight) sqlite3ExprDelete(db, p->pRight);

    // Track deletions for the test leaves
    if(p->tag == 1) ++g_leftDeletedCount;
    if(p->tag == 2) ++g_rightDeletedCount;

    std::free(p);
    (void)db;
}


// Helper to create a leaf expression that can be tracked via tag
static Expr* makeLeaf(int tag) {
    Expr* e = (Expr*)std::malloc(sizeof(Expr));
    if(!e) return nullptr;
    // Initialize a minimal leaf
    e->op = 0;
    e->iAgg = 0;
    e->nHeight = 0;
    e->pLeft = nullptr;
    e->pRight = nullptr;
    e->tag = tag;
    return e;
}


// Forward declaration: the real function is defined in expr.c; we only declare it here
extern "C" Expr* sqlite3PExpr(Parse* pParse, int op, Expr* pLeft, Expr* pRight);


// Test 1: Allocation succeeds
static void test_allocation_success() {
    std::cout << "Running test: allocation success path\n";

    // Prepare parse context
    Parse pParse;
    sqlite3 dbInstance;
    pParse.db = &dbInstance;

    // Prepare left/right subtrees
    Expr* pLeft = makeLeaf(1);   // tag 1 for left
    Expr* pRight = makeLeaf(2);  // tag 2 for right

    resetDeletionCounters();
    g_failAlloc = false; // force allocation to succeed

    // Use an op with more than 0xff to ensure masking occurs
    int op = 0x1AB; // lower 8 bits = 0xAB
    Expr* pRes = sqlite3PExpr(&pParse, op, pLeft, pRight);

    // Validate allocation path
    ASSERT_NOT_NULL(pRes, "sqlite3PExpr should return a non-null Expr when allocation succeeds");
    if(pRes != nullptr) {
        ASSERT_EQ(pRes->op, (op & 0xff), "Expression root 'op' should be masked to 0xff");
        ASSERT_EQ(pRes->iAgg, -1, "Expression root 'iAgg' should be initialized to -1");
        ASSERT_TRUE(pRes->pLeft == pLeft, "Left subtree should be attached to root");
        ASSERT_TRUE(pRes->pRight == pRight, "Right subtree should be attached to root");
    }

    // Cleanup: delete the root (which should recursively delete subtrees)
    sqlite3ExprDelete(pParse.db, pRes);

    // Verify that the leaves were deleted exactly once
    ASSERT_EQ(g_leftDeletedCount, 1, "Left leaf should be deleted exactly once");
    ASSERT_EQ(g_rightDeletedCount, 1, "Right leaf should be deleted exactly once");

    // Reset pointers
    pLeft = nullptr;
    pRight = nullptr;

    // Summary for this test
    std::cout << "test_allocation_success: passes=" << g_testPasses << " failures=" << g_testFailures << "\n";
}

// Test 2: Allocation fails -> ensure leaves are deleted
static void test_allocation_failure() {
    std::cout << "Running test: allocation failure path\n";

    // Prepare parse context
    Parse pParse;
    sqlite3 dbInstance;
    pParse.db = &dbInstance;

    // Prepare left/right subtrees
    Expr* pLeft = makeLeaf(1);   // tag 1 for left
    Expr* pRight = makeLeaf(2);  // tag 2 for right

    resetDeletionCounters();
    g_failAlloc = true; // force allocation to fail

    int op = 0x55; // arbitrary op
    Expr* pRes = sqlite3PExpr(&pParse, op, pLeft, pRight);

    // Validate failure path
    ASSERT_NULL(pRes, "sqlite3PExpr should return NULL when allocation fails");

    // In failure path, leaves should be deleted by the code
    // (no root expression to delete)
    ASSERT_EQ(g_leftDeletedCount, 1, "Left leaf should be deleted exactly once on allocation failure");
    ASSERT_EQ(g_rightDeletedCount, 1, "Right leaf should be deleted exactly once on allocation failure");

    // No need to free pLeft/pRight separately as they were freed by sqlite3ExprDelete
    pLeft = nullptr;
    pRight = nullptr;

    // Summary for this test
    std::cout << "test_allocation_failure: passes=" << g_testPasses << " failures=" << g_testFailures << "\n";
}


// Main test runner
int main() {
    // Run both tests
    test_allocation_success();
    // Separate runner for second test to isolate state
    test_allocation_failure();

    // Final summary
    int totalTests = g_testPasses + g_testFailures;
    std::cout << "Unit test suite complete. Passed: " << g_testPasses
              << ", Failed: " << g_testFailures
              << ", Total: " << totalTests << "\n";

    // Return non-zero if any test failed
    return (g_testFailures == 0) ? 0 : 1;
}