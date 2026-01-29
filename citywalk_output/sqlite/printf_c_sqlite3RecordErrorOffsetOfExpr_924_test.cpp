#include <iostream>
#include <sqliteInt.h>


// Step 1: Candidate Keywords (for understanding and test planning)
// - sqlite3 (db context with errByteOffset field)
// - Expr (expression node with left child and offset info)
// - ExprHasProperty (predicate using EP_OuterON, EP_InnerON)
// - EP_OuterON, EP_InnerON (property bitmasks)
// - pExpr->pLeft (left traversal in expression tree)
// - pExpr->w.iOfst (integer offset field)
// - sqlite3RecordErrorOffsetOfExpr (function under test)

// The following test suite provides a self-contained minimal environment to exercise
// the focal method sqlite3RecordErrorOffsetOfExpr without requiring the full sqlite3 project.
//
// Note: We re-create the minimal structures and function exactly as the focal method would use,
// so tests reflect the actual control flow and edge cases.

struct sqlite3 {
    int errByteOffset; // observable side-effect of sqlite3RecordErrorOffsetOfExpr
};

// Forward-declare the function under test to ensure linkage in this test harness.
// In the real project, this would come from printf.c as provided.
struct Expr {
    Expr* pLeft;
    struct { int iOfst; } w;
    int iProp; // auxiliary property bits used by our test mock for ExprHasProperty
};

// Property bitmasks (matching the usage in sqlite3RecordErrorOffsetOfExpr)
enum {
    EP_OuterON = 1 << 0,
    EP_InnerON = 1 << 1
};

// Mock implementation of ExprHasProperty used by the focal algorithm.
// This is a simplified stand-in for the real function, tailored for our tests.
static bool ExprHasProperty(const Expr* p, int mask) {
    if (p == nullptr) return false;
    return (p->iProp & mask) != 0;
}

// The focal method under test (reproduced here for a self-contained test harness).
void sqlite3RecordErrorOffsetOfExpr(sqlite3 *db, const Expr *pExpr){
  while( pExpr
     && (ExprHasProperty(pExpr,EP_OuterON|EP_InnerON) || pExpr->w.iOfst<=0)
  ){
    pExpr = pExpr->pLeft;
  }
  if( pExpr==0 ) return;
  db->errByteOffset = pExpr->w.iOfst;
}

// Utility to create an expression node
static Expr* mkExpr(int iOfst, int props, Expr* left){
    Expr* e = new Expr();
    e->pLeft = left;
    e->w.iOfst = iOfst;
    e->iProp = props;
    return e;
}

// Simple test harness (non-terminating assertions; prints failures but continues)
static int g_total = 0;
static int g_failed = 0;
#define EXPECT_EQ(actual, expected, message) do { \
    ++g_total; \
    if ((actual) != (expected)) { \
        ++g_failed; \
        std::cerr << "TEST FAILURE: " << (message) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } \
} while(0)

static void test_null_expr() {
    // Test that a null pExpr leaves db->errByteOffset unchanged
    sqlite3 db; db.errByteOffset = -9999;
    sqlite3RecordErrorOffsetOfExpr(&db, nullptr);
    EXPECT_EQ(db.errByteOffset, -9999, "null Expr should not modify errByteOffset");
}

static void test_chain_stop_at_E1() {
    // Build chain: E3(iOfst=7) -> E2(iOfst=0) -> E1(iOfst=0) -> E0(iOfst=0)
    // Wait: The intended chain for this test is:
    // E3(iOfst=7, no props) -> E2(iOfst=0) -> E1(iOfst=0)
    Expr* E0 = mkExpr(0, 0, nullptr);      // leaf with iOfst <= 0
    Expr* E1 = mkExpr(0, 0, E0);           // iOfst <= 0
    Expr* E2 = mkExpr(0, 0, E1);           // iOfst <= 0
    Expr* E3 = mkExpr(7, 0, E2);           // iOfst > 0, no props
    sqlite3 db; db.errByteOffset = -1;
    sqlite3RecordErrorOffsetOfExpr(&db, E3);
    // Expect to exit on E1 (the first non-zero iOfst with no props encountered after skipping <=0),
    // but the structure above ensures the loop will move down until E1 is last non-zero? 
    // Correct behavior: loop traverses while iOfst <= 0 or has properties; stops at E3 since after traversal
    // E3 has iOfst > 0 and no EP flag, so offset should be 7.
    EXPECT_EQ(db.errByteOffset, 7, "Chain should stop at the first node with iOfst>0 and no properties");
    // cleanup
    delete E3; delete E2; delete E1; delete E0;
}

static void test_skip_due_to_property() {
    // Build chain: E4(iOfst=5, prop OuterON) -> E3(iOfst=12, no props)
    // The loop should skip E4 due to property, and stop at E3, setting offset to 12.
    Expr* E3 = mkExpr(12, 0, nullptr);
    Expr* E4 = mkExpr(5, EP_OuterON, E3);
    sqlite3 db; db.errByteOffset = -2;
    sqlite3RecordErrorOffsetOfExpr(&db, E4);
    EXPECT_EQ(db.errByteOffset, 12, "Node with EP_OuterON should be skipped; should set offset from next node");
    delete E4; delete E3;
}

static void test_end_with_null_chain() {
    // Start with a node whose iOfst <= 0 and left is null.
    // The loop should move to null and return without setting db->errByteOffset.
    Expr* E0 = mkExpr(0, 0, nullptr);
    sqlite3 db; db.errByteOffset = -123;
    sqlite3RecordErrorOffsetOfExpr(&db, E0);
    // Since E0.iOfst <= 0, the loop advances to left (null) and returns without assignment
    EXPECT_EQ(db.errByteOffset, -123, "End of chain with iOfst<=0 should not modify errByteOffset");
    delete E0;
}

int main() {
    test_null_expr();
    test_chain_stop_at_E1();
    test_skip_due_to_property();
    test_end_with_null_chain();

    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl;
    return g_failed ? 1 : 0;
}