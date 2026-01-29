// A self-contained C++11 test harness for the SQLite-like function
// sqlite3ExprCodeRunJustOnce(Parse*, Expr*, int) implemented below.
// This test suite provides minimal stubs for required SQLite types and
// functions to exercise the focal method's branches without a full SQLite
// dependency. It uses plain C++11 and prints diagnostic output to stdout.
//
// Note: This harness is designed to compile as a single translation unit.

#include <new>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-specific constants (subset for testing)
static const int OP_Once = 1;

// Property flags for Expr
static const int EP_HasFunc = 1 << 0;

// Forward declarations to mimic SQLite's small API surface used by the focal method
struct sqlite3;
struct Vdbe;
struct Expr;
struct ExprList;
struct ExprList_item;
struct Parse;

// Lightweight stub implementations to support the focal method's logic

// Minimal sqlite3 structure
struct sqlite3 {
  bool mallocFailed;
};

// Vdbe stub (tracking basic actions)
struct Vdbe {
  bool addedOpOnce;
  bool jumpedHere;
  Vdbe() : addedOpOnce(false), jumpedHere(false) {}
};

// Expr structure with simple flags
struct Expr {
  int flags; // bitfield to hold EP_HasFunc-like flags
  // Additional fields are unnecessary for the tests
};

// Item inside an expression list
struct ExprList_item {
  Expr *pExpr;
  struct { int reusable; } fg;
  int u_iConstExprReg; // renamed to avoid macro conflicts in C++
};

// Expression list container
struct ExprList {
  int nExpr;
  ExprList_item *a;
  ExprList() : nExpr(0), a(nullptr) {}
};

// Parsing context (subset)
struct Parse {
  ExprList *pConstExpr; // cached constant expressions
  Vdbe *pVdbe;          // virtual machine instance
  int okConstFactor;
  sqlite3 *db;
  int nMem;
  // For tests, record where an expression was coded
  int lastCodeDest;
  Parse() : pConstExpr(nullptr), pVdbe(nullptr), okConstFactor(1),
            db(nullptr), nMem(0), lastCodeDest(-1) {}
};

// Utility function to simulate the "ConstFactorOk" check
static int ConstFactorOk(Parse *pParse) {
  // Always true for tests
  (void)pParse;
  return 1;
}

// Expression comparison stub: returns 0 when expressions are "equal".
static int sqlite3ExprCompare(const void *pParse, const Expr *pA, const Expr *pB, int iTab) {
  (void)pParse;
  (void)iTab;
  // Equality based on pointer identity for test determinism
  return (pA == pB) ? 0 : ((pA < pB) ? -1 : 1);
}

// Check if an expression has a given property (flag-based)
static int ExprHasProperty(const Expr *p, int pr) {
  return (p != nullptr) && ((p->flags & pr) != 0);
}

// Duplication stub: in tests we return the same pointer
static Expr* sqlite3ExprDup(sqlite3 *db, Expr *pExpr, int flags) {
  (void)db;
  (void)flags;
  return pExpr;
}

// Delete an expression (no-op for tests)
static void sqlite3ExprDelete(sqlite3 *db, Expr *pExpr) {
  (void)db;
  (void)pExpr;
  // No-op in test environment
}

// Encode an expression into a register (record for test inspection)
static void sqlite3ExprCode(Parse *pParse, Expr *pExpr, int regDest) {
  (void)pExpr;
  pParse->lastCodeDest = regDest;
}

// Append an expression to an ExprList (creates a new list if needed)
static ExprList* sqlite3ExprListAppend(Parse *pParse, ExprList *pList, Expr *pExpr) {
  // If there is no existing list, create one
  if (pList == nullptr) {
    pList = new ExprList();
    pList->nExpr = 1;
    pList->a = new ExprList_item[1];
  } else {
    // Append to existing list
    int oldN = pList->nExpr;
    ExprList_item *oldA = pList->a;
    pList->nExpr = oldN + 1;
    pList->a = new ExprList_item[pList->nExpr];
    // Copy old entries
    for (int i = 0; i < oldN; ++i) pList->a[i] = oldA[i];
    delete[] oldA;
  }
  // Initialize the new last item
  pList->a[pList->nExpr - 1].pExpr = pExpr;
  pList->a[pList->nExpr - 1].fg.reusable = 0;
  pList->a[pList->nExpr - 1].u_iConstExprReg = 0;
  return pList;
}

// Vdbe.addOp0 stub
static int sqlite3VdbeAddOp0(Vdbe *v, int op) {
  (void)op;
  v->addedOpOnce = true;
  return 0;
}

// Macro placeholder for coverage instrumentation
#define VdbeCoverage(v) do { (void)(v); } while(0)

// Simulated "Jump Here" — indicate the Vdbe jumped to an address
static void sqlite3VdbeJumpHere(Vdbe *v, int addr) {
  (void)addr;
  v->jumpedHere = true;
}

// Deleted in test; real implementation would free memory
static void sqlite3ExprDelete(sqlite3 *db, Expr *pExpr) {
  (void)db;
  (void)pExpr;
  // No-op; memory management is manual in test harness
}

// The focal method under test (re-implemented in test harness)
int sqlite3ExprCodeRunJustOnce(
  Parse *pParse,    /* Parsing context */
  Expr *pExpr,      /* The expression to code when the VDBE initializes */
  int regDest       /* Store the value in this register */
){
  // Local helper types to mimic the real code
  ExprList *p;
  // Use signals from test harness
  // Ensure basic invariants
  // Note: In the provided code there is a syntax quirk; adapt to a clean equivalent
  // Begin logic
  // Step A: fast path using cached constants
  if (!pParse) return 0;
  if (!ConstFactorOk(pParse)) return 0;
  // Emulate assert(regDest != 0)
  assert(regDest != 0);
  p = pParse->pConstExpr;
  if( regDest<0 && p ){
    struct ExprList_item *pItem;
    int i;
    for(pItem=p->a, i=p->nExpr; i>0; pItem++, i--){
      if( pItem->fg.reusable
       && sqlite3ExprCompare(0,pItem->pExpr,pExpr,-1)==0
      ){
        return pItem->u_iConstExprReg;
      }
    }
  }
  // Step B: duplicate expression and decide how to code it
  pExpr = sqlite3ExprDup(pParse->db, pExpr, 0);
  if( pExpr!=0 && ExprHasProperty(pExpr, EP_HasFunc) ){
    Vdbe *v = pParse->pVdbe;
    int addr;
    // Real code asserts the Vdbe exists
    assert( v );
    addr = sqlite3VdbeAddOp0(v, OP_Once); VdbeCoverage(v);
    pParse->okConstFactor = 0;
    if( !pParse->db->mallocFailed ){
      if( regDest<0 ) regDest = ++pParse->nMem;
      sqlite3ExprCode(pParse, pExpr, regDest);
    }
    pParse->okConstFactor = 1;
    sqlite3ExprDelete(pParse->db, pExpr);
    sqlite3VdbeJumpHere(v, addr);
  }else{
    p = sqlite3ExprListAppend(pParse, p, pExpr);
    if( p ){
       struct ExprList_item *pItem = &p->a[p->nExpr-1];
       pItem->fg.reusable = regDest<0;
       if( regDest<0 ) regDest = ++pParse->nMem;
       pItem->u_iConstExprReg = regDest;
    }
    pParse->pConstExpr = p;
  }
  return regDest;
}

// End of focal method replica

// ----------------------
// Test harness begins here
// ----------------------

static void printHeader(const char* s) {
  std::cout << "=== " << s << " ===" << std::endl;
}

// Simple non-terminating assertion helper (logs but continues)
static void expect_true(bool cond, const char* msg) {
  if (!cond) {
    std::cerr << "ASSERTION FAILED: " << msg << std::endl;
  }
}

// Test 1: When regDest < 0 and a matching const Expr exists in pConstExpr, expect return of its reg
static bool test_EarlyReturn_ConstExprMatch() {
  printHeader("Test 1: Early return via matching const Expr in pConstExpr");

  // Setup sqlite3 and Parse
  sqlite3 db;
  db.mallocFailed = false;
  Parse p;
  p.db = &db;
  p.pConstExpr = nullptr;
  p.pVdbe = nullptr;
  p.okConstFactor = 1;
  p.nMem = 0;

  // Create target expression (the one we are "evaluating")
  Expr target;
  target.flags = 0;

  // Create ExprList with one item that is reusable and equal to target
  ExprList *plist = new ExprList();
  plist->nExpr = 1;
  plist->a = new ExprList_item[1];
  plist->a[0].pExpr = &target;
  plist->a[0].fg.reusable = 1;
  plist->a[0].u_iConstExprReg = 42;

  p.pConstExpr = plist;

  // Call focal method with regDest = -1 (negative)
  int regDest = -1;
  int result = sqlite3ExprCodeRunJustOnce(&p, &target, regDest);

  // Expectations
  bool ok = (result == 42);
  expect_true(ok, "Should return the previously cached const expression register (42).");

  // Cleanup allocated memory
  delete[] plist->a;
  delete plist;

  return ok;
}

// Test 2: When pExpr has EP_HasFunc, ensure the "Once" path is taken and regDest allocated
static bool test_HasFunc_Path() {
  printHeader("Test 2: HasFunc path triggers OP_Once and code path");

  sqlite3 db;
  db.mallocFailed = false;
  Parse p;
  p.db = &db;
  p.pConstExpr = nullptr;
  p.pVdbe = new Vdbe();
  p.okConstFactor = 1;
  p.nMem = 0;

  // Create a function-like expression (HasFunc)
  Expr funcExpr;
  funcExpr.flags = EP_HasFunc; // indicates EP_HasFunc property present

  // Call focal method with regDest = -1 to force allocation
  int regDest = -1;
  int result = sqlite3ExprCodeRunJustOnce(&p, &funcExpr, regDest);

  // Validate outcome
  bool jumped = p.pVdbe->jumpedHere;
  bool allocated = (result == 1); // regDest should become 1 (since nMem starts at 0)
  // Also, a_once op should have been added
  bool addedOnce = p.pVdbe->addedOpOnce;

  expect_true(jumped, "Vdbe should indicate a jump happened after OP_Once.");
  expect_true(allocated, "Register should be allocated as regDest becomes 1 when negative input is used.");
  expect_true(addedOnce, "OP_Once should be issued to Vdbe.");

  // Cleanup
  delete p.pVdbe;
  return jumped && allocated && addedOnce;
}

// Test 3: When pExpr has no EP_HasFunc, ensure it is appended to ExprList and regDest is assigned
static bool test_ExprListAppend_Path() {
  printHeader("Test 3: No HasFunc => append to ExprList and assign reg");

  sqlite3 db;
  db.mallocFailed = false;
  Parse p;
  p.db = &db;
  p.pConstExpr = nullptr;
  p.pVdbe = nullptr;
  p.okConstFactor = 1;
  p.nMem = 0;

  // Create a non-function expression
  Expr nonFuncExpr;
  nonFuncExpr.flags = 0; // no EP_HasFunc

  int regDest = -1;
  int result = sqlite3ExprCodeRunJustOnce(&p, &nonFuncExpr, regDest);

  bool regAssigned = (result == 1); // should allocate reg 1
  bool listCreated = p.pConstExpr != nullptr;
  bool listHasOneExpr = listCreated && p.pConstExpr->nExpr == 1;
  bool itemRegSet = listHasOneExpr && (p.pConstExpr->a[0].u_iConstExprReg == 1);
  bool itemReusable = listHasOneExpr && (p.pConstExpr->a[0].fg.reusable != 0);

  expect_true(regAssigned, "regDest should be assigned to 1 when starting from -1 and no func.");
  expect_true(listCreated, "ExprList should be created and attached to pConstExpr.");
  expect_true(listHasOneExpr, "ExprList should contain exactly one expression after append.");
  expect_true(itemRegSet, "The appended item's register should be set to 1.");
  expect_true(itemReusable, "The appended item should be reusable when regDest < 0.");

  // Cleanup
  delete[] p.pConstExpr->a;
  delete p.pConstExpr;

  return regAssigned && listCreated && listHasOneExpr && itemRegSet && itemReusable;
}

// Entry point
int main() {
  bool pass1 = test_EarlyReturn_ConstExprMatch();
  std::cout << "Test 1 " << (pass1 ? "PASSED" : "FAILED") << std::endl;

  bool pass2 = test_HasFunc_Path();
  std::cout << "Test 2 " << (pass2 ? "PASSED" : "FAILED") << std::endl;

  bool pass3 = test_ExprListAppend_Path();
  std::cout << "Test 3 " << (pass3 ? "PASSED" : "FAILED") << std::endl;

  if (pass1 && pass2 && pass3) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}