/*
Test suite for the focal method:
void sqlite3ExprSetHeightAndFlags(Parse *pParse, Expr *p){
  if( pParse->nErr ) return;
  exprSetHeight(p);
  sqlite3ExprCheckHeight(pParse, p->nHeight);
}
This test harness mocks the minimal dependencies and focuses on validating:
- The early return when pParse->nErr != 0 (false branch).
- The normal path when pParse->nErr == 0 (true branch), ensuring:
  - exprSetHeight is invoked.
  - sqlite3ExprCheckHeight is invoked with the height produced by exprSetHeight.
- The height value passed to sqlite3ExprCheckHeight matches p->nHeight after exprSetHeight.

Candidate Keywords (derived from the focal method and its dependencies):
Parse, pParse->nErr, Expr, p->nHeight, exprSetHeight, sqlite3ExprCheckHeight, early return, height propagation.

Note: This test suite uses a lightweight, custom test harness (no GTest) and provides minimal stand-in definitions for the dependent functions to isolate and exercise the focal method logic.
*/

#include <iostream>
#include <sqliteInt.h>


// Domain knowledge: provide minimal dependency stubs and the focal function implementation.

// Minimal representations of the types used by the focal function.
struct Parse {
    int nErr; // error flag; non-zero means error
};

struct Expr {
    int nHeight; // height field that exprSetHeight will set
};

// Global test-state tracking to observe internal calls without modifying the focal code.
// These are used by the test doubles for exprSetHeight and sqlite3ExprCheckHeight.
static bool g_exprSetHeight_called = false;
static bool g_checkHeight_called = false;
static int g_checkHeight_height_passed = 0;

// Control variable to influence what height exprSetHeight writes into Expr
static int gHeightToSet = 0;

// Forward declarations of the dependent functions (stubs) used by the focal method.
// In the real project these would be defined in the code base; here we provide test doubles.
void exprSetHeight(Expr *p) {
    // Record that the function was invoked
    g_exprSetHeight_called = true;
    // Set the height to the test-controlled value
    p->nHeight = gHeightToSet;
}

void sqlite3ExprCheckHeight(Parse *pParse, int nHeight) {
    // Record that the function was invoked and with what height
    g_checkHeight_called = true;
    g_checkHeight_height_passed = nHeight;
    // In real code, this might raise pParse->nErr, etc. For tests we keep it simple.
}

// The focal method under test, implemented in this test file to enable isolated testing.
// Signature must match exactly to exercise the intended logic.
void sqlite3ExprSetHeightAndFlags(Parse *pParse, Expr *p) {
    if( pParse->nErr ) return;
    exprSetHeight(p);
    sqlite3ExprCheckHeight(pParse, p->nHeight);
}

// Lightweight test harness (non-terminating, prints failures but continues execution)
static int gTestFailures = 0;
#define TEST_ASSERT(cond, desc) do { \
    if(!(cond)) { \
        std::cerr << "FAIL: " << desc << std::endl; \
        ++gTestFailures; \
    } \
} while(0)

// Utility to reset test state between tests
void reset_test_state() {
    g_exprSetHeight_called = false;
    g_checkHeight_called = false;
    g_checkHeight_height_passed = 0;
    gHeightToSet = 0;
}

// Test Case 1: When pParse->nErr is non-zero, sqlite3ExprSetHeightAndFlags should return immediately
// and not invoke exprSetHeight or sqlite3ExprCheckHeight.
void test_case_nErr_nonzero_returns_without_calls() {
    reset_test_state();
    Parse pParse;
    Expr e;

    // Configure non-zero error state
    pParse.nErr = 1;
    e.nHeight = 0;
    gHeightToSet = 999; // arbitrary; should be ignored due to early return

    // Execute focal method
    sqlite3ExprSetHeightAndFlags(&pParse, &e);

    // Verify that neither dependent function was called
    TEST_ASSERT(g_exprSetHeight_called == false, "Case1: exprSetHeight should NOT be called when pParse.nErr != 0");
    TEST_ASSERT(g_checkHeight_called == false, "Case1: sqlite3ExprCheckHeight should NOT be called when pParse.nErr != 0");
}

// Test Case 2: When pParse->nErr is zero, both functions should be invoked and height propagated correctly
void test_case_nErr_zero_calls_both_functions_with_correct_height() {
    reset_test_state();
    Parse pParse;
    Expr e;

    // Configure zero error state
    pParse.nErr = 0;
    // Control the height that exprSetHeight will assign
    gHeightToSet = 7;
    e.nHeight = 0;

    // Execute focal method
    sqlite3ExprSetHeightAndFlags(&pParse, &e);

    // Verify that both functions were called
    TEST_ASSERT(g_exprSetHeight_called == true, "Case2: exprSetHeight should be called when pParse.nErr == 0");
    TEST_ASSERT(g_checkHeight_called == true, "Case2: sqlite3ExprCheckHeight should be called when pParse.nErr == 0");

    // Ensure the height passed to sqlite3ExprCheckHeight equals the height set by exprSetHeight
    TEST_ASSERT(g_checkHeight_height_passed == e.nHeight, "Case2: sqlite3ExprCheckHeight should receive the height set by exprSetHeight");
    TEST_ASSERT(e.nHeight == 7, "Case2: pExpr.nHeight should be the value set by exprSetHeight (7 in this test)");
}

// Entry point for running tests
int main() {
    std::cout << "Running test suite for sqlite3ExprSetHeightAndFlags (isolated environment)" << std::endl;

    test_case_nErr_nonzero_returns_without_calls();
    test_case_nErr_zero_calls_both_functions_with_correct_height();

    if(gTestFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "TESTS FAILED: " << gTestFailures << " failure(s)" << std::endl;
        return 1;
    }
}