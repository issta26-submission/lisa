// C++11 test suite for sqlite3ParseObjectReset in prepare.c
// This test harness mocks necessary SQLite internals to exercise the focal method
// without relying on GoogleTest. It uses lightweight EXPECT-like macros that
// continue execution on failure to maximize coverage.

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Include SQLite internal declarations used by sqlite3ParseObjectReset

// Forward declarations for test-time mocks (C linkage to match SQLite internals)
extern "C" {

// Mocks to intercept internal free/delete primitives called by sqlite3ParseObjectReset
void sqlite3DbNNFreeNN(sqlite3 *db, void *p);
void sqlite3ExprListDelete(sqlite3 *db, void *p);

// Test-specific cleanup function
void test_xCleanup(sqlite3 *db, void *pPtr);

} // extern "C"

//////////////////////////////////////
// Global test state (mocks tracking)
static std::vector<void*> g_freedPointers;     // Pointers passed to sqlite3DbNNFreeNN
static std::vector<void*> g_cleanupPtrCalls;   // Pointers passed to test_xCleanup
static std::vector<void*> g_expectedCleanup;   // Expected cleanup pointers order for verification

static void reset_mock_state() {
    g_freedPointers.clear();
    g_cleanupPtrCalls.clear();
    g_expectedCleanup.clear();
}

// Mock implementations (C linkage) to satisfy the focal function's calls
extern "C" void sqlite3DbNNFreeNN(sqlite3 *db, void *p) {
    // Track freed pointers
    g_freedPointers.push_back(p);
}

// Mock implementation for ExprList deletion
extern "C" void sqlite3ExprListDelete(sqlite3 *db, void *p) {
    // For testing, simply record that the deletion was invoked
    // We reuse g_freedPointers to indicate deletion as well (distinct tracking is optional)
    g_freedPointers.push_back(p);
}

// Our test-provided cleanup callback
extern "C" void test_xCleanup(sqlite3 *db, void *pPtr) {
    // Record pointer passed to cleanup to verify order and invocation
    g_cleanupPtrCalls.push_back(pPtr);
}

//////////////////////////////////////
// Lightweight EXPECT-like macros (non-terminating)
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "[EXPECT_TRUE] FAILED: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } } while(0)

#define EXPECT_EQ_INT(a, b, msg) \
    do { if((int)(a) != (int)(b)) { \
        std::cerr << "[EXPECT_EQ_INT] FAILED: " << (msg) << " expected " << (b) \
                  << " got " << (a) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } } while(0)

#define EXPECT_EQ_PTR(a, b, msg) \
    do { if((void*)(a) != (void*)(b)) { \
        std::cerr << "[EXPECT_EQ_PTR] FAILED: " << (msg) << " expected " << b \
                  << " got " << a << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } } while(0)

#define EXPECT_TRUE_MSG(cond, msg) EXPECT_TRUE(cond, msg)

// Helper to set up a minimal Parse and ParseCleanup chain
static Parse* create_test_parse(sqlite3 *db,
                              void *aTableLockPtr,
                              void *aLabelPtr,
                              void *pConstExprPtr,
                              Parse *outerParse,
                              int disableLookaside,
                              int nested = 0)
{
    // Allocate a Parse object
    Parse *p = new Parse();
    // Initialize minimal fields used by sqlite3ParseObjectReset
    // Pointers
    p->db = db;
    p->pOuterParse = outerParse;
    p->pCleanup = nullptr;
    p->aTableLock = aTableLockPtr;
    p->aLabel = (void*)aLabelPtr;     // typically a Label*; cast to void*
    p->pConstExpr = (ExprList*)pConstExprPtr;
    p->nested = nested;
    p->disableLookaside = disableLookaside;

    // db linkage
    db->pParse = p;
    return p;
}

// Create a simple ParseCleanup node
static ParseCleanup* create_cleanup_node(void (*xCleanupFn)(sqlite3*,void*), void *pPtr, ParseCleanup *pNext) {
    ParseCleanup *pc = new ParseCleanup();
    pc->xCleanup = xCleanupFn;
    pc->pPtr = pPtr;
    pc->pNext = pNext;
    return pc;
}

// Convenience to zero a sqlite3 object lightly (per SQLite internals)
static sqlite3 create_test_db() {
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));
    // initialize lookaside with sensible defaults
    // Accessed fields are looked up by sqlite3ParseObjectReset
    db.lookaside.bDisable = 0;
    db.lookaside.sz = 0;
    db.lookaside.szTrue = 1024;
    return db;
}

//////////////////////////////////////
// Test 1: Basic reset with no extras
// - No aTableLock, no pCleanup, no aLabel, no pConstExpr
// - lookaside disables unchanged (disableLookaside = 0)
// - pOuterParse = nullptr; after reset, db->pParse should become nullptr
void test_basic_reset_no_extras() {
    reset_mock_state();
    std::cout << "Running test_basic_reset_no_extras..." << std::endl;

    // Setup db and parse
    sqlite3 db = create_test_db();
    Parse *pParse = new Parse();
    std::memset(pParse, 0, sizeof(Parse));

    pParse->db = &db;
    pParse->pOuterParse = nullptr;
    pParse->pCleanup = nullptr;
    pParse->aTableLock = nullptr;
    pParse->aLabel = nullptr;
    pParse->pConstExpr = nullptr;
    pParse->nested = 0;
    pParse->disableLookaside = 0;

    db.pParse = pParse;

    // Prepare outer parse pointer (could be nullptr)
    Parse *pOuter = nullptr;
    pParse->pOuterParse = pOuter;

    // Call the function under test
    sqlite3ParseObjectReset(pParse);

    // Assertions
    EXPECT_TRUE(db.pParse == pOuter, "db.pParse should be set to pOuterParse (nullptr)");  // after call
    // No allocations should have been freed
    EXPECT_EQ_INT(g_freedPointers.size(), 0, "no free should have been invoked");
    // No exprs freed
    EXPECT_EQ_INT(g_freedPointers.size(), 0, "no exprs freed");
    // Lookaside should remain unchanged
    EXPECT_EQ_INT(db.lookaside.bDisable, 0, "lookaside.bDisable unchanged");
    EXPECT_EQ_INT(db.lookaside.sz, db.lookaside.szTrue, "lookaside.sz should equal szTrue when disabled count is zero");

    delete pParse;
}

// Test 2: Reset with table lock, cleanup chain, a label, and a const Expr
void test_reset_with_all_extras() {
    reset_mock_state();
    std::cout << "Running test_reset_with_all_extras..." << std::endl;

    // Setup db
    sqlite3 db = create_test_db();
    // Outer parse (after reset, db->pParse should point here)
    Parse *outer = new Parse();
    std::memset(outer, 0, sizeof(Parse));

    // Primary parse with extras
    Parse *pParse = new Parse();
    std::memset(pParse, 0, sizeof(Parse));
    pParse->db = &db;
    pParse->pOuterParse = outer;
    pParse->nested = 0;
    pParse->disableLookaside = 1; // ensures sz may be set to 0

    // aTableLock path (non-null) should trigger sqlite3DbNNFreeNN
    void* tableLock = (void*)0xABCD;
    pParse->aTableLock = tableLock;

    // aLabel path
    void* labelObj = (void*)0x1111;
    pParse->aLabel = labelObj;

    // pConstExpr path
    void* exprListObj = (void*)0x2222;
    pParse->pConstExpr = (ExprList*)exprListObj;

    // Cleanup chain: two items
    ParseCleanup *pc2 = create_cleanup_node(test_xCleanup, (void*)0x2, nullptr);
    ParseCleanup *pc1 = create_cleanup_node(test_xCleanup, (void*)0x1, pc2);
    pParse->pCleanup = pc1;  // head

    // Wire db
    db.pParse = pParse;
    // Set up lookaside to test subtraction and sz behavior
    db.lookaside.bDisable = 2;
    db.lookaside.sz = 100;
    db.lookaside.szTrue = 1000;

    // Now call
    sqlite3ParseObjectReset(pParse);

    // Post-conditions
    // All cleanup items should have been processed in order: 0x1, 0x2
    EXPECT_TRUE(g_cleanupPtrCalls.size() >= 2, "two cleanup callbacks should have been invoked");
    EXPECT_EQ_PTR(g_cleanupPtrCalls[0], (void*)0x1, "first cleanup pPtr should be 0x1");
    EXPECT_EQ_PTR(g_cleanupPtrCalls[1], (void*)0x2, "second cleanup pPtr should be 0x2");

    // All cleanup nodes and related objects should be freed
    // Freed pointers should include aTableLock, pCleanup objects, aLabel, and expr list
    bool hasLock = false, hasLabel = false, hasExprList = false;
    for (auto ptr : g_freedPointers) {
        if (ptr == tableLock) hasLock = true;
        if (ptr == labelObj) hasLabel = true;
        if (ptr == (ParseCleanup*)pc1) {} // pc1 is a ParseCleanup object; freed after
        if (ptr == (ParseCleanup*)pc2) {} // same reasoning
        if (ptr == exprListObj) hasExprList = true;
    }
    EXPECT_TRUE(hasLock, "table lock should be freed");
    EXPECT_TRUE(hasLabel, "label object should be freed");
    EXPECT_TRUE(hasExprList, "expression list should be freed");

    // Lookaside adjustment
    // initially bDisable = 2, disableLookaside = 1 => new bDisable = 1
    EXPECT_EQ_INT(db.lookaside.bDisable, 1, "lookaside.bDisable after reset should be decremented by disableLookaside");
    // Since bDisable is non-zero, sz should be 0
    EXPECT_EQ_INT(db.lookaside.sz, 0, "lookaside.sz should be 0 when bDisable is non-zero");

    // pParse should be set to pOuterParse (outer)
    EXPECT_TRUE(db.pParse == outer, "db.pParse should be set to pOuterParse (outer parse)");

    // Cleanup resources
    delete pc2;
    delete pc1;
    delete pParse;
    delete outer;
}

// Test 3: Reset where lookaside becomes zero and sz should reflect szTrue
void test_reset_lookaside_becomes_zero() {
    reset_mock_state();
    std::cout << "Running test_reset_lookaside_becomes_zero..." << std::endl;

    sqlite3 db = create_test_db();
    Parse *pParse = new Parse();
    std::memset(pParse, 0, sizeof(Parse));

    // Outer parse
    Parse *outer = new Parse();

    pParse->db = &db;
    pParse->pOuterParse = outer;
    pParse->nested = 0;
    pParse->disableLookaside = 2; // will subtract to 0
    pParse->pCleanup = nullptr;
    pParse->aTableLock = nullptr;
    pParse->aLabel = nullptr;
    pParse->pConstExpr = nullptr;

    db.pParse = pParse;
    db.lookaside.bDisable = 2; // equal to disableLookaside
    db.lookaside.sz = 55;
    db.lookaside.szTrue = 1234;

    // Call function
    sqlite3ParseObjectReset(pParse);

    // Expect bDisable reduced to 0 and sz restored to szTrue
    EXPECT_EQ_INT(db.lookaside.bDisable, 0, "lookaside.bDisable should be zero after decrement");
    EXPECT_EQ_INT(db.lookaside.sz, db.lookaside.szTrue, "lookaside.sz should equal szTrue when bDisable is zero");
    // And db.pParse should be outer
    EXPECT_TRUE(db.pParse == outer, "db.pParse should be set to pOuterParse (outer parse)");

    // Cleanup
    delete pParse;
    delete outer;
}

// Main entry: run tests
int main() {
    std::cout << "Starting sqlite3ParseObjectReset unit tests (C++11 harness)" << std::endl;

    test_basic_reset_no_extras();
    test_reset_with_all_extras();
    test_reset_lookaside_becomes_zero();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}