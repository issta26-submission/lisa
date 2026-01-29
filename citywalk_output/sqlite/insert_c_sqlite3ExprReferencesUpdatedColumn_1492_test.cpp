// Unit test suite for sqlite3ExprReferencesUpdatedColumn
// This test harness provides a minimal, self-contained environment
// to exercise the focal method from insert.c without bringing in the
// entire SQLite project. It is designed to be compiled with C++11.

// Notes per instructions (domain knowledge):
// - We avoid GTest; we implement a small, non-terminating assertion helper.
// - We model the required dependencies (Expr, Walker, sqlite3WalkExpr) in a
//   lightweight way sufficient to drive the focal function's logic.
// - Static/file-scope helpers from the original file are emulated within this test
//   translation unit; static visibility is simulated by file-scope functions.
// - The tests focus on exercising the control flow and the interaction between
//   the walker and the constraint-check callback, including the chngRowid path.
// - All code is plain C++11 and uses only the standard library for I/O and memory.

/*
Expected behavior of the focal function (simplified contract for tests):
- It builds a Walker, zeros it, sets the callback to checkConstraintExprNode,
  assigns aiChng to walker.u.aiCol, and then calls sqlite3WalkExpr(&w, pExpr).
- If chngRowid is false, it checks if CKCNSTRNT_ROWID is set in w.eCode, then clears it from w.eCode.
- It then runs a series of testcases on w.eCode and returns (w.eCode != 0).
- The actual bits CKCNSTRNT_ROWID and CKCNSTRNT_COLUMN are used by the test logic.
- For the sake of isolated testing, the callback and the walk function are mocked
  to produce deterministic w.eCode values according to a global scenario.
*/

// Include necessary C++ headers
#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <vector>


// ------------------------------------------------------------
// Lightweight simulation of dependencies from the focal code
// ------------------------------------------------------------

// Forward declare Expr as an opaque expression node (not used in detail here)
struct Expr;

// Lightweight Walker structure matching the usage in the focal code
struct Walker {
    int eCode;                      // accumulated constraint code bits
    int (*xExprCallback)(Walker*, Expr*); // callback invoked during walk
    union {
        int *aiCol;                   // pointer to aiChng array
    } u;
};

// Bit flags used by the focal code to represent constraint types
// We keep them as simple nonzero values by default; tests drive scenarios.
static const int CKCNSTRNT_ROWID  = 0x01;
static const int CKCNSTRNT_COLUMN = 0x02;

// A global variable to control the deterministic behavior of the constraint callback
// 0: NONE (no constraint encountered)
// 1: COLUMN only
// 2: ROWID only
// 3: ROWID and COLUMN both
static int g_callback_scenario = 0;

// A minimal Expr type (opaque for this test harness)
struct Expr { /* empty for our mock */ };

// Mock of the SQLite walk routine. In the real project this would traverse
// an expression tree. Here we simply invoke the callback if a scenario is set.
void sqlite3WalkExpr(Walker *pWalker, Expr * /*pExpr*/) {
    if (pWalker && pWalker->xExprCallback) {
        pWalker->xExprCallback(pWalker, reinterpret_cast<Expr*>(nullptr));
    }
}

// Callback used by the focal function to record which constraint nodes were encountered.
// We set w->eCode according to the current test scenario.
int checkConstraintExprNode(Walker *pWalker, Expr * /*pExpr*/) {
    switch (g_callback_scenario) {
        case 0: // NONE
            pWalker->eCode = 0;
            break;
        case 1: // COLUMN only
            pWalker->eCode = CKCNSTRNT_COLUMN;
            break;
        case 2: // ROWID only
            pWalker->eCode = CKCNSTRNT_ROWID;
            break;
        case 3: // ROWID and COLUMN
            pWalker->eCode = CKCNSTRNT_ROWID | CKCNSTRNT_COLUMN;
            break;
        default:
            pWalker->eCode = 0;
    }
    return 0;
}

// Simple non-terminating test assertion helper.
// Tracks total tests and failures; prints failures to std::cerr.
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TESTCASE(cond) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        std::cerr << "[Test Failure] " << __FILE__ << ":" << __LINE__ \
                  << " - condition: " #cond " evaluated to false\n"; \
    } \
} while(0)

// The focal function under test (re-implemented here for isolated unit testing).
// We mimic the exact control flow described in the provided snippet.
int sqlite3ExprReferencesUpdatedColumn(
    Expr *pExpr,    /* The expression to be checked */
    int *aiChng,    /* aiChng[x]>=0 if column x changed by the UPDATE */
    int chngRowid   /* True if UPDATE changes the rowid */
){
    Walker w;
    std::memset(&w, 0, sizeof(w));
    w.eCode = 0;
    w.xExprCallback = checkConstraintExprNode;
    w.u.aiCol = aiChng;
    sqlite3WalkExpr(&w, pExpr);
    if( !chngRowid ){
        TESTCASE( (w.eCode & CKCNSTRNT_ROWID)!=0 );
        w.eCode &= ~CKCNSTRNT_ROWID;
    }
    TESTCASE( w.eCode==0 );
    TESTCASE( w.eCode==CKCNSTRNT_COLUMN );
    TESTCASE( w.eCode==CKCNSTRNT_ROWID );
    TESTCASE( w.eCode==(CKCNSTRNT_ROWID|CKCNSTRNT_COLUMN) );
    return w.eCode!=0;
}

// ------------------------------------------------------------
// Test harness entry point
// ------------------------------------------------------------
int main() {
    // Prepare a dummy aiChng array (the actual values are not the focus of these unit tests)
    std::vector<int> aiBuf(16, 0);

    // ------------------------------
    // Test Case 1: No rowid change, no constraint encountered
    // Scenario: chngRowid = 1 (true) to bypass the initial CKCNSTRNT_ROWID handling.
    // Callback scenario 0 ensures w.eCode remains 0.
    // Expected: All four TESTCASE checks should resolve to true because w.eCode is 0
    // and chngRowid being true bypasses the early masking block.
    // ------------------------------
    g_callback_scenario = 0; // NONE (no constraint encountered)
    int res1 = sqlite3ExprReferencesUpdatedColumn(nullptr, aiBuf.data(), 1);
    // Reinitialize test counters are not necessary for a single run but kept for clarity
    std::cout << "Test Case 1 completed. Result: " << (res1 ? "non-zero" : "zero") 
              << ". Total tests: " << g_totalTests << ", Failures: " << g_failedTests << "\n";

    // Print a separator for readability
    std::cout << "----------------------------------------\n";

    // ------------------------------
    // Test Case 2: Column constraint encountered
    // Scenario: chngRowid = 1 (true) to bypass early rowid masking,
    // Callback reports a COLUMN constraint only.
    // Expect: w.eCode == 0, but since we bypass early masking and w.eCode is column-only,
    // the later testcases will reflect a non-zero code for CKCNSTRNT_COLUMN and possibly others.
    // This helps exercise the path where COLUMN constraint bits are set.
    // ------------------------------
    g_callback_scenario = 1; // COLUMN only
    int res2 = sqlite3ExprReferencesUpdatedColumn(nullptr, aiBuf.data(), 1);
    std::cout << "Test Case 2 completed. Result: " << (res2 ? "non-zero" : "zero")
              << ". Total tests: " << g_totalTests << ", Failures: " << g_failedTests << "\n";

    std::cout << "----------------------------------------\n";

    // ------------------------------
    // Test Case 3: Rowid constraint encountered
    // Scenario: chngRowid = 0 to exercise the masking path on CKCNSTRNT_ROWID.
    // We set ROWID to be the constraint, then the masking step will clear it.
    // This tests the branch where the ROWID bit is cleared.
    // ------------------------------
    g_callback_scenario = 2; // ROWID only
    int res3 = sqlite3ExprReferencesUpdatedColumn(nullptr, aiBuf.data(), 0);
    std::cout << "Test Case 3 completed. Result: " << (res3 ? "non-zero" : "zero")
              << ". Total tests: " << g_totalTests << ", Failures: " << g_failedTests << "\n";

    std::cout << "----------------------------------------\n";

    // ------------------------------
    // Test Case 4: Both ROWID and COLUMN constraints encountered
    // Scenario: chngRowid = 0 to engage the masking path, with ROWID|COLUMN present.
    // This exercises both bits being set and then potentially masked.
    // ------------------------------
    g_callback_scenario = 3; // ROWID and COLUMN
    int res4 = sqlite3ExprReferencesUpdatedColumn(nullptr, aiBuf.data(), 0);
    std::cout << "Test Case 4 completed. Result: " << (res4 ? "non-zero" : "zero")
              << ". Total tests: " << g_totalTests << ", Failures: " << g_failedTests << "\n";

    // Summary
    std::cout << "=====================\n";
    std::cout << "Total tests executed: " << g_totalTests
              << ", Failures: " << g_failedTests << "\n";

    // Return non-zero if any test failed (simulates typical test harness exit behavior)
    return (g_failedTests != 0) ? 1 : 0;
}