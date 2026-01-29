// Minimal C++11 unit tests for the focal function sqlite3ExprCodeFactorable
// This test mocks the dependent SQLite-style APIs to exercise the control flow
// of the focal method without requiring the full SQLite build.

// The test is self-contained: it redefines the necessary types and functions
// (Parse, Expr, sqlite3ExprIsConstantNotJoin, sqlite3ExprCodeRunJustOnce,
// sqlite3ExprCodeCopy) so that the exact branches of the focal method can be
// exercised deterministically.

#include <iostream>
#include <sqliteInt.h>


// Global failure counter for non-terminating test assertions
static int g_failures = 0;

// Lightweight mocked domain types mirroring minimal usage in the focal method
struct Expr {
  bool isConstant; // controls sqlite3ExprIsConstantNotJoin(Expr*)
};

struct Parse {
  // Control flag used by the focal function
  bool okConstFactor;

  // Test-scoped bookkeeping for how many times each mocked helper was invoked
  int runJustOnceCalls;
  int copyCalls;

  // Capture last target values passed to the mocked helpers (for assertion)
  int lastRunJustOnceTarget;
  int lastCopyTarget;
};

// Forward declarations for the mocks and dependencies
extern "C" {
  // Mocked predicate used by the focal method
  int sqlite3ExprIsConstantNotJoin(Expr *pExpr);

  // Mocked code emission hooks used by the focal method
  void sqlite3ExprCodeRunJustOnce(Parse *pParse, Expr *pExpr, int target);
  void sqlite3ExprCodeCopy(Parse *pParse, Expr *pExpr, int target);
}

// A direct translation of the focal method under test (as provided)
void sqlite3ExprCodeFactorable(Parse *pParse, Expr *pExpr, int target){
  if( pParse->okConstFactor && sqlite3ExprIsConstantNotJoin(pExpr) ){
    sqlite3ExprCodeRunJustOnce(pParse, pExpr, target);
  }else{
    sqlite3ExprCodeCopy(pParse, pExpr, target);
  }
}

// ---------------- Mocked implementations ----------------

int sqlite3ExprIsConstantNotJoin(Expr *pExpr){
  // True iff the expression is a constant (no join involved)
  return pExpr != nullptr && pExpr->isConstant ? 1 : 0;
}

void sqlite3ExprCodeRunJustOnce(Parse *pParse, Expr *pExpr, int target){
  // Record that RunJustOnce path was taken and the target value used
  (void)pExpr; // not used by the test logic beyond isConstant
  ++pParse->runJustOnceCalls;
  pParse->lastRunJustOnceTarget = target;
}

void sqlite3ExprCodeCopy(Parse *pParse, Expr *pExpr, int target){
  // Record that Copy path was taken and the target value used
  (void)pExpr; // not used by the test logic beyond isConstant
  ++pParse->copyCalls;
  pParse->lastCopyTarget = target;
}

// ---------------- Test utilities ----------------

#define EXPECT_EQ(actual, expected, msg) \
  do { \
    if ((actual) != (expected)) { \
      std::cerr << "TEST FAIL: " << (msg) \
                << " | expected: " << (expected) \
                << ", actual: " << (actual) << "\n"; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_TRUE(cond, msg) \
  do { \
    if (!(cond)) { \
      std::cerr << "TEST FAIL: " << (msg) << "\n"; \
      ++g_failures; \
    } \
  } while(0)

// Convenience test runner functions with explanatory comments

// Test 1: True branch of both predicates (okConstFactor && sqlite3ExprIsConstantNotJoin(pExpr))
// Expect: sqlite3ExprCodeRunJustOnce is invoked exactly once; Copy is not invoked.
void test_case_runJustOnce_path_when_all_true() {
  // Arrange
  Parse p;
  p.okConstFactor = true;
  p.runJustOnceCalls = 0;
  p.copyCalls = 0;
  p.lastRunJustOnceTarget = -1;
  p.lastCopyTarget = -1;

  Expr e;
  e.isConstant = true; // sqlite3ExprIsConstantNotJoin will return 1

  // Act
  sqlite3ExprCodeFactorable(&p, &e, 42);

  // Assert
  EXPECT_EQ(p.runJustOnceCalls, 1, "RunJustOnce should be called exactly once");
  EXPECT_EQ(p.copyCalls, 0, "Copy should not be called when RunJustOnce is taken");
  EXPECT_EQ(p.lastRunJustJustOnceTarget, 42, "RunJustOnce should be invoked with target 42");
}

// Test 2: okConstFactor = true, but sqlite3ExprIsConstantNotJoin(pExpr) returns false
// Expect: sqlite3ExprCodeCopy is invoked exactly once.
void test_case_copy_path_when_isConstantNotJoin_false() {
  // Arrange
  Parse p;
  p.okConstFactor = true;
  p.runJustOnceCalls = 0;
  p.copyCalls = 0;
  p.lastRunJustOnceTarget = -1;
  p.lastCopyTarget = -1;

  Expr e;
  e.isConstant = false; // sqlite3ExprIsConstantNotJoin will return 0

  // Act
  sqlite3ExprCodeFactorable(&p, &e, 7);

  // Assert
  EXPECT_EQ(p.runJustOnceCalls, 0, "RunJustOnce should not be called when isConstantNotJoin is false");
  EXPECT_EQ(p.copyCalls, 1, "Copy should be called exactly once");
  EXPECT_EQ(p.lastCopyTarget, 7, "Copy should be invoked with target 7");
}

// Test 3: okConstFactor = false, even if isConstantNotJoin is true
// Expect: sqlite3ExprCodeCopy is invoked exactly once (branch guard fails on first condition).
void test_case_copy_path_when_okConstFactor_false() {
  // Arrange
  Parse p;
  p.okConstFactor = false;
  p.runJustOnceCalls = 0;
  p.copyCalls = 0;
  p.lastRunJustOnceTarget = -1;
  p.lastCopyTarget = -1;

  Expr e;
  e.isConstant = true;

  // Act
  sqlite3ExprCodeFactorable(&p, &e, 99);

  // Assert
  EXPECT_EQ(p.runJustOnceCalls, 0, "RunJustOnce should not be called when okConstFactor is false");
  EXPECT_EQ(p.copyCalls, 1, "Copy should be called exactly once when okConstFactor is false");
  EXPECT_EQ(p.lastCopyTarget, 99, "Copy should be invoked with target 99");
}

// Test 4: Re-run the first scenario to ensure repeated usage doesn't regress
// Repeat Test 1 to verify consistent behavior across multiple invocations.
void test_case_repeat_runJustOnce_path() {
  // Arrange
  Parse p;
  p.okConstFactor = true;
  p.runJustOnceCalls = 0;
  p.copyCalls = 0;
  p.lastRunJustOnceTarget = -1;
  p.lastCopyTarget = -1;

  Expr e;
  e.isConstant = true;

  // Act
  sqlite3ExprCodeFactorable(&p, &e, 123);

  // Assert
  EXPECT_EQ(p.runJustOnceCalls, 1, "RunJustOnce should be called exactly once on repeat");
  EXPECT_EQ(p.copyCalls, 0, "Copy should not be called on repeat when RunJustOnce taken");
  EXPECT_EQ(p.lastRunJustOnceTarget, 123, "RunJustOnce should be invoked with target 123");
}

// ---------------- Main harness ----------------

int main() {
  // Run all test cases
  test_case_runJustOnce_path_when_all_true();
  test_case_copy_path_when_isConstantNotJoin_false();
  test_case_copy_path_when_okConstFactor_false();
  test_case_repeat_runJustOnce_path();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << g_failures << " TEST(s) FAILED\n";
    return 1;
  }
}