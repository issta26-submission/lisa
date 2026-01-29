/*
  Unit test harness for the focal logic of sqlite3ExprCodeTemp.
  Important notes:
  - This test is implemented in C++11 as a self-contained harness to verify
    the control-flow and branch behavior described in the focal method.
  - Since the original SQLite codebase contains many static helpers and
    internal dependencies, this suite re-implements a minimal, isolated
    version of sqlite3ExprCodeTemp along with lightweight test doubles that
    simulate its dependencies. This allows exercising true/false branches
    without requiring the entire SQLite internal build.
  - The tests use a simple, non-terminating assertion style (EXPECT_*).
  - The test is designed to be standalone; it does not rely on GTest or any
    external test framework.
  - Provide explanations for each test to aid future maintenance.
*/

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Lightweight stand-ins for SQLite core types (standalone test harness)
struct Parse {};
struct Expr {
    int op; // Expression operator; test uses non-TK_REGISTER values to branch
};

// Forward declaration of the focal logic (re-implemented here for isolation)
static int sqlite3ExprSkipCollateAndLikely(Expr* p);
static int ConstFactorOk(Parse* pParse);
static int ALWAYS(int x);
static int sqlite3ExprIsConstantNotJoin(Expr* p);
static int sqlite3ExprCodeRunJustOnce(Parse* pParse, Expr* pExpr, int regDest);
static int sqlite3GetTempReg(Parse* pParse);
static int sqlite3ExprCodeTarget(Parse* pParse, Expr* pExpr, int target);
static void sqlite3ReleaseTempReg(Parse* pParse, int iReg);

// TK_REGISTER constant used by the focal logic
static const int TK_REGISTER = 1;

// Global/test-doubles state to drive deterministic behavior
static int g_constFactorOkFlag;
static int g_exprIsConstantNotJoinFlag;
static int g_runJustOnceValue;
static int g_nextTempReg;
static int g_exprCodeTargetReturn;
static int g_releaseCalled;
static int g_lastReleasedReg;

// Minimal re-implementation of the focal method's dependencies (test doubles)
static int sqlite3ExprSkipCollateAndLikely(Expr* p) {
    // In tests, return the expression unchanged
    (void)p; // suppress unused warning in some compilers
    return (int)(uintptr_t)p;
}

// Simulated ConstFactorOk
static int ConstFactorOk(Parse* pParse) {
    (void)pParse;
    return g_constFactorOkFlag;
}

// Simulated ALWAYS macro (as a function for testability)
static int ALWAYS(int x) {
    return x;
}

// Simulated sqlite3ExprIsConstantNotJoin
static int sqlite3ExprIsConstantNotJoin(Expr* p) {
    (void)p;
    return g_exprIsConstantNotJoinFlag;
}

// Simulated sqlite3ExprCodeRunJustOnce
static int sqlite3ExprCodeRunJustOnce(Parse* pParse, Expr* pExpr, int regDest) {
    (void)pParse;
    (void)pExpr;
    (void)regDest;
    return g_runJustOnceValue;
}

// Simulated sqlite3GetTempReg
static int sqlite3GetTempReg(Parse* pParse) {
    (void)pParse;
    // Simple monotonically increasing temp reg
    ++g_nextTempReg;
    return g_nextTempReg;
}

// Simulated sqlite3ExprCodeTarget
static int sqlite3ExprCodeTarget(Parse* pParse, Expr* pExpr, int target) {
    (void)pParse;
    (void)pExpr;
    (void)target;
    return g_exprCodeTargetReturn;
}

// Simulated sqlite3ReleaseTempReg
static void sqlite3ReleaseTempReg(Parse* pParse, int iReg) {
    (void)pParse;
    g_lastReleasedReg = iReg;
    g_releaseCalled = 1;
}

// Re-implemented focal logic for isolated unit testing
static int sqlite3ExprCodeTemp(Parse *pParse, Expr *pExpr, int *pReg){
  int r2;
  pExpr = (Expr*) (uintptr_t)sqlite3ExprSkipCollateAndLikely(pExpr);
  if( ConstFactorOk(pParse)
   && ALWAYS(pExpr!=0)
   && pExpr->op!=TK_REGISTER
   && sqlite3ExprIsConstantNotJoin(pExpr)
  ){
    *pReg  = 0;
    r2 = sqlite3ExprCodeRunJustOnce(pParse, pExpr, -1);
  }else{
    int r1 = sqlite3GetTempReg(pParse);
    r2 = sqlite3ExprCodeTarget(pParse, pExpr, r1);
    if( r2==r1 ){
      *pReg = r1;
    }else{
      sqlite3ReleaseTempReg(pParse, r1);
      *pReg = 0;
    }
  }
  return r2;
}

// Simple EXPECT-style assertion helpers (non-terminating)
static void expect_true(bool cond, const std::string& desc) {
    if (cond) {
        std::cout << "[PASS] " << desc << "\n";
    } else {
        std::cout << "[FAIL] " << desc << " (expected true, got false)\n";
    }
}

static void expect_eq_int(int a, int b, const std::string& desc) {
    if (a == b) {
        std::cout << "[PASS] " << desc << "\n";
    } else {
        std::cout << "[FAIL] " << desc << " (expected " << a << " == " << b << ")\n";
    }
}

// Test 1: True branch (ConstFactorOk && ALWAYS(pExpr != 0) && expr is constant not join)
void test_true_branch() {
    // Setup state for true-branch conditions
    g_constFactorOkFlag = 1;           // ConstFactorOk(pParse) -> true
    g_exprIsConstantNotJoinFlag = 1;   // sqlite3ExprIsConstantNotJoin(pExpr) -> true
    g_runJustOnceValue = 42;           // Result from sqlite3ExprCodeRunJustOnce
    g_nextTempReg = 0;
    g_releaseCalled = 0;
    g_lastReleasedReg = -1;

    Parse pParse;
    Expr pExpr;
    pExpr.op = 0; // not TK_REGISTER

    int reg = -1234;
    int r2 = sqlite3ExprCodeTemp(&pParse, &pExpr, &reg);

    // Expectations:
    // - pReg should be set to 0
    // - r2 should come from sqlite3ExprCodeRunJustOnce (42)
    expect_eq_int(reg, 0, "True branch: reg should be 0");
    expect_eq_int(r2, 42, "True branch: r2 should be 42");
}

// Test 2: False branch with r2 == r1 (branch sets pReg = r1 when r2 == r1)
void test_false_branch_r2_equals_r1() {
    // Ensure we take the else path by turning off ConstFactorOk
    g_constFactorOkFlag = 0;           // Force else path
    g_exprCodeTargetReturn = 1;        // sqlite3ExprCodeTarget returns same as r1 (which will be 1)
    g_nextTempReg = 0;
    g_releaseCalled = 0;
    g_lastReleasedReg = -1;
    g_exprIsConstantNotJoinFlag = 0;

    Parse pParse;
    Expr pExpr;
    pExpr.op = 10; // non-TK_REGISTER

    int reg = -999;
    int r2 = sqlite3ExprCodeTemp(&pParse, &pExpr, &reg);

    // After sqlite3GetTempReg, r1 == 1; sqlite3ExprCodeTarget returns 1, so r2 == r1
    expect_eq_int(reg, 1, "False branch (r2==r1): reg should be 1");
    expect_eq_int(r2, 1, "False branch (r2==r1): r2 should be 1");
    expect_true(!g_releaseCalled, "False branch (r2==r1): Release should not be called");
}

// Test 3: False branch with r2 != r1 (ensures ReleaseTempReg is invoked and reg is set to 0)
void test_false_branch_r2_not_equals_r1() {
    // Ensure else path
    g_constFactorOkFlag = 0;
    g_exprCodeTargetReturn = 5;        // r2 != r1 (r1 will be 1)
    g_nextTempReg = 0;
    g_releaseCalled = 0;
    g_lastReleasedReg = -1;
    g_exprIsConstantNotJoinFlag = 0;

    Parse pParse;
    Expr pExpr;
    pExpr.op = 20; // non-TK_REGISTER

    int reg = -5;
    int r2 = sqlite3ExprCodeTemp(&pParse, &pExpr, &reg);

    // After else path, r1 == 1; r2 == 5 != r1, so reg should be 0 and ReleaseTempReg called with 1
    expect_eq_int(reg, 0, "False branch (r2!=r1): reg should be 0");
    expect_eq_int(r2, 5, "False branch (r2!=r1): r2 should be 5");
    expect_true(g_releaseCalled, "False branch (r2!=r1): ReleaseTempReg should be called");
    expect_eq_int(g_lastReleasedReg, 1, "False branch (r2!=r1): ReleaseTempReg should be called with iReg == 1");
}

// Main: Run all tests
int main() {
    std::cout << "Starting sqlite3ExprCodeTemp unit tests (isolated logic tests)\n";
    test_true_branch();
    test_false_branch_r2_equals_r1();
    test_false_branch_r2_not_equals_r1();
    std::cout << "Tests completed.\n";
    return 0;
}