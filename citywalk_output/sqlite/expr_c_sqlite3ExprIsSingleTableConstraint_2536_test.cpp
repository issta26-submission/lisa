// Minimal C++11 test harness for sqlite3ExprIsSingleTableConstraint
// This test suite provides a self-contained reproduction of the
// sqlite3ExprIsSingleTableConstraint logic with lightweight stubs for
// dependencies. It verifies true/false branches and ensures coverage
// of the function under varied scenarios.
// Author: Generated test harness tailored to the provided focal method.

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <sqliteInt.h>


// Domain-specific constants (subset sufficient for testing)
#define JT_LTORJ 0x01  // Left-To-Right Outer Join flag (mock)
#define JT_LEFT  0x02   // LEFT join flag (mock)

// Expression property flags (mock)
#define EP_OuterON 0x01
#define EP_InnerON 0x02

// Forward declarations of dependencies (mocked for tests)
struct Expr;
struct SrcItem;
struct SrcList;

// Forward declare the function under test and its dependency
int sqlite3ExprIsTableConstant(const Expr *pExpr, int iCur);
int sqlite3ExprIsSingleTableConstraint(
  Expr *pExpr,                 /* The constraint */
  const SrcList *pSrcList,     /* Complete FROM clause */
  int iSrc                     /* Which element of pSrcList to use */
);

// Mocked minimal Expr structure (subset sufficient for tests)
struct Expr {
  struct { int iJoin; } w; // join context info
  int flags;              // property flags (EP_OuterON, EP_InnerON, etc.)
};

// SrcItem with nested jointype flags (subset)
struct SrcItem {
  struct { int jointype; } fg;
  int iCursor;
};

// SrcList containing an array of SrcItem
struct SrcList {
  SrcItem *a;
  int nSrc;
};

// Global control for sqlite3ExprIsTableConstant behavior in tests
static int g_tableConstantFlag = 0;

// Lightweight helper to check properties on Expr (mock of ExprHasProperty)
static int ExprHasProperty(const Expr *p, int mask){
  if(!p) return 0;
  return (p->flags & mask) != 0;
}

// Mock implementation of sqlite3ExprIsTableConstant (test control)
int sqlite3ExprIsTableConstant(const Expr *pExpr, int iCur){
  (void)pExpr; (void)iCur; // unused in this mock besides being testable via global flag
  return g_tableConstantFlag;
}

// The focal method copied (adapted to compile in this harness)
int sqlite3ExprIsSingleTableConstraint(
  Expr *pExpr,                 /* The constraint */
  const SrcList *pSrcList,     /* Complete FROM clause */
  int iSrc                     /* Which element of pSrcList to use */
){
  const SrcItem *pSrc = &pSrcList->a[iSrc];
  if( pSrc->fg.jointype & JT_LTORJ ){
    return 0;  /* rule (3) */
  }
  if( pSrc->fg.jointype & JT_LEFT ){
    if( !ExprHasProperty(pExpr, EP_OuterON) ) return 0;   /* rule (4a) */
    if( pExpr->w.iJoin!=pSrc->iCursor ) return 0;         /* rule (4b) */
  }else{
    if( ExprHasProperty(pExpr, EP_OuterON) ) return 0;    /* rule (5) */
  }
  if( ExprHasProperty(pExpr, EP_OuterON|EP_InnerON)  /* (6a) */
   && (pSrcList->a[0].fg.jointype & JT_LTORJ)!=0     /* Fast pre-test of (6b) */
  ){
    int jj;
    for(jj=0; jj<iSrc; jj++){
      if( pExpr->w.iJoin==pSrcList->a[jj].iCursor ){
        if( (pSrcList->a[jj].fg.jointype & JT_LTORJ)!=0 ){
          return 0;  /* restriction (6) */
        }
        break;
      }
    }
  }
  return sqlite3ExprIsTableConstant(pExpr, pSrc->iCursor); /* rules (1), (2) */
}

// Simple test harness and helpers
static int g_test_count = 0;
static int g_fail_count = 0;

// Non-terminating assertion macro (logs but continues)
#define TEST_ASSERT(cond, msg) do { \
  ++g_test_count; \
  if(!(cond)) { \
    ++g_fail_count; \
    std::cerr << "[FAIL] " << __FUNCTION__ << ": " << msg << std::endl; \
  } \
} while(0)

// Helpers to build SrcList and Expr instances
SrcList* makeSrcList(int n){
  SrcList *sl = new SrcList;
  sl->nSrc = n;
  sl->a = new SrcItem[n];
  // Initialize with safe defaults
  for(int i=0;i<n;i++){
    sl->a[i].iCursor = i + 1;
    sl->a[i].fg.jointype = 0;
  }
  return sl;
}

void freeSrcList(SrcList *sl){
  if(!sl) return;
  delete[] sl->a;
  delete sl;
}

Expr makeExpr(int flags, int iJoin){
  Expr e;
  e.flags = flags;
  e.w.iJoin = iJoin;
  return e;
}

// Test cases

// Test 1: If the single source is JT_LTORJ, expect 0 (rule (3))
void test_case1(){
  SrcList *sl = makeSrcList(1);
  sl->a[0].iCursor = 1;
  sl->a[0].fg.jointype = JT_LTORJ; // triggers early return

  Expr e = makeExpr(0, -1);

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 0);
  TEST_ASSERT(res == 0, "case1: LTORJ should return 0 (rule 3)");
  freeSrcList(sl);
}

// Test 2: JT_LEFT without Outer ON property should return 0 (rule 4a)
void test_case2(){
  SrcList *sl = makeSrcList(1);
  sl->a[0].iCursor = 2;
  sl->a[0].fg.jointype = JT_LEFT;

  Expr e = makeExpr(0, -1); // EP_OuterON not set

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 0);
  TEST_ASSERT(res == 0, "case2: LEFT without OuterON should return 0 (rule 4a)");
  freeSrcList(sl);
}

// Test 3: JT_LEFT with Outer ON true but iJoin mismatch -> 0 (rule 4b)
void test_case3(){
  SrcList *sl = makeSrcList(1);
  sl->a[0].iCursor = 5;
  sl->a[0].fg.jointype = JT_LEFT;

  Expr e = makeExpr(EP_OuterON, 999); // OuterON set, iJoin != iCursor

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 0);
  TEST_ASSERT(res == 0, "case3: LEFT with OuterON and iJoin mismatch should return 0 (rule 4b)");
  freeSrcList(sl);
}

// Test 4: Not JT_LEFT (inner join) but OuterON is set -> 0 (rule 5)
void test_case4(){
  SrcList *sl = makeSrcList(1);
  sl->a[0].iCursor = 7;
  sl->a[0].fg.jointype = 0; // not LEFT

  Expr e = makeExpr(EP_OuterON, -1);

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 0);
  TEST_ASSERT(res == 0, "case4: non-LEFT with OuterON should return 0 (rule 5)");
  freeSrcList(sl);
}

// Test 5: Complex path through (6a)/(6b) leading to final check
// Setup: EP_OuterON triggers, a[0].jointype has JT_LTORJ, iSrc=1, and
// pExpr.w.iJoin does not match a[0].iCursor to avoid fast path; final call
// uses sqlite3ExprIsTableConstant which we control via global flag.
void test_case5(){
  SrcList *sl = makeSrcList(2);
  // pSrcList->a[0]
  sl->a[0].iCursor = 10;
  sl->a[0].fg.jointype = JT_LTORJ;
  // pSrcList->a[1]
  sl->a[1].iCursor = 20;
  sl->a[1].fg.jointype = 0;

  // pExpr must have EP_OuterON or EP_InnerON, and iJoin should not equal a[0].iCursor
  Expr e = makeExpr(EP_OuterON, 999);

  // Set to 1 so final call to sqlite3ExprIsTableConstant returns 1
  g_tableConstantFlag = 1;

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 1);
  // Since iJoin != a[0].iCursor, loop does not short-circuit; final constant check returns 1
  TEST_ASSERT(res == 1, "case5: complex path should reach final constant check and return 1");
  freeSrcList(sl);
}

// Test 6: Path through 6a early-return when pExpr->w.iJoin matches a[0].iCursor
void test_case6(){
  SrcList *sl = makeSrcList(2);
  sl->a[0].iCursor = 3;
  sl->a[0].fg.jointype = JT_LTORJ;
  sl->a[1].iCursor = 4;
  sl->a[1].fg.jointype = 0;

  // pExpr matches iJoin to a[0].iCursor and has LTORJ, enabling early return
  Expr e = makeExpr(EP_OuterON, 3);

  g_tableConstantFlag = 0; // Not relevant for early exit

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 1);
  TEST_ASSERT(res == 0, "case6: early return when pExpr->w.iJoin equals a[0].iCursor and LTORJ");
  freeSrcList(sl);
}

// Test 7: Sanity check that non-triggering paths still evaluate final table-constant
void test_case7(){
  SrcList *sl = makeSrcList(1);
  sl->a[0].iCursor = 11;
  sl->a[0].fg.jointype = 0;

  // pExpr has OuterON but iJoin does not matter for this test
  Expr e = makeExpr(EP_OuterON, -1);

  // Force table-constant to be true to ensure final return value is 1
  g_tableConstantFlag = 1;

  int res = sqlite3ExprIsSingleTableConstraint(&e, sl, 0);
  TEST_ASSERT(res == 1, "case7: final table-constant check should return 1 when applicable");
  freeSrcList(sl);
}

// Main orchestration
int main(){
  std::cout << "Running sqlite3ExprIsSingleTableConstraint test suite (mocked environment)" << std::endl;

  test_case1();
  test_case2();
  test_case3();
  test_case4();
  test_case5();
  test_case6();
  test_case7();

  std::cout << "Tests run: " << g_test_count << ", Failures: " << g_fail_count << std::endl;
  // Cleanup: ensure no leaks (in this short harness, memory freed within tests)

  return g_fail_count ? EXIT_FAILURE : EXIT_SUCCESS;
}