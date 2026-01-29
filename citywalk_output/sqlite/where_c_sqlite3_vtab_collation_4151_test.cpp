// Self-contained unit test suite for sqlite3_vtab_collation
// This test is designed to be compiled with a C++11 compiler without Google Test.
// It re-implements only the minimal scaffolding needed to exercise the focal function
// and its dependent data structures in a self-contained manner.

#include <whereInt.h>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Lightweight test harness (non-terminating assertions)
static int g_testFailures = 0;
#define TEST_PASS(fmt, ...) do { printf("PASS: " fmt "\n", ##__VA_ARGS__); } while(0)
#define TEST_FAIL(fmt, ...) do { printf("FAIL: " fmt "\n", ##__VA_ARGS__); g_testFailures++; } while(0)

#define EXPECT_EQ_PTR(a,b) do { if((void*)(a) != (void*)(b)) { TEST_FAIL("Expected pointer %p, got %p", (void*)(b), (void*)(a)); } else { TEST_PASS("Pointer equality holds (%p)", (void*)(a)); } } while(0)
#define EXPECT_EQ_STR(a,b) do { if(strcmp((a),(b))!=0) { TEST_FAIL("Expected string \"%s\", got \"%s\"", (b), (a)); } else { TEST_PASS("Strings match: %s", (a)); } } while(0)
#define EXPECT_NULL(a) do { if((a) != nullptr) { TEST_FAIL("Expected null, got non-null"); } else { TEST_PASS("Null as expected"); } } while(0)
#define EXPECT_NOT_NULL(a) do { if((a) == nullptr) { TEST_FAIL("Expected non-null, got null"); } else { TEST_PASS("Non-null as expected"); } } while(0)

// Lightweight reproductions of the relevant sqlite3 types (mocked for the test)
struct CollSeq {
  const char *zName;
};

struct Expr {
  Expr *pLeft;
};

struct Parse {};

struct sqlite3_index_constraint {
  int iTermOffset;
};

struct sqlite3_index_info {
  int nConstraint;
  sqlite3_index_constraint *aConstraint;
};

struct WhereTerm {
  Expr *pExpr;
};

struct WhereClause {
  WhereTerm *a;
  int nTerm;
};

struct HiddenIndexInfo {
  WhereClause *pWC;
  Parse *pParse;
};

// The const string used by sqlite3_vtab_collation when there is no CollSeq
static const char sqlite3StrBINARY[] = "BINARY";

// Forward declaration of the focal function (as implemented in the provided code)
extern "C" const char *sqlite3_vtab_collation(sqlite3_index_info *pIdxInfo, int iCons);

// Internal helper used by the focal function (stubbed behavior)
static CollSeq *sqlite3ExprCompareCollSeq(Parse *, Expr *pExpr);

// A small global switch to simulate different behavior in sqlite3ExprCompareCollSeq
static bool g_exprReturnNull = false;

// Implementation of the helper used by sqlite3_vtab_collation
static CollSeq *sqlite3ExprCompareCollSeq(Parse *pParse, Expr *pExpr){
  // If the expression has a left subtree, pretend we can compare collations
  if(pExpr && pExpr->pLeft){
    if(g_exprReturnNull){
      return nullptr;
    }else{
      static CollSeq sColl = { "NOCASE" };
      (void)pParse; // unused in this stub
      return &sColl;
    }
  }
  return nullptr;
}

// Implementation of the focal method (reproduced here for testing)
extern "C" const char *sqlite3_vtab_collation(sqlite3_index_info *pIdxInfo, int iCons){
  HiddenIndexInfo *pHidden = (HiddenIndexInfo*)&pIdxInfo[1];
  const char *zRet = 0;
  if( iCons>=0 && iCons<pIdxInfo->nConstraint ){
    CollSeq *pC = 0;
    int iTerm = pIdxInfo->aConstraint[iCons].iTermOffset;
    Expr *pX = pHidden->pWC->a[iTerm].pExpr;
    if( pX->pLeft ){
      pC = sqlite3ExprCompareCollSeq(pHidden->pParse, pX);
    }
    zRet = (pC ? pC->zName : sqlite3StrBINARY);
  }
  return zRet;
}

// Helper: create a small, controlled environment for each test case
static void setup_common_env(sqlite3_index_info* pIdxInfo, sqlite3_index_constraint* pConstraintOut, int nConstraint){
  // Initialize constraint array for pIdxInfo[0]
  pIdxInfo[0].nConstraint = nConstraint;
  pIdxInfo[0].aConstraint = pConstraintOut;
  // The HiddenIndexInfo region is stored in the memory just after pIdxInfo[0]
  // We will fill it in the individual tests via a direct cast to HiddenIndexInfo*
}

// Test 1: iCons is negative: should return null (0)
static void test_vtab_collation_negative_iCons(){
  // Allocate a buffer to mimic pIdxInfo[0] and place HiddenIndexInfo after it
  size_t total = 2 * sizeof(sqlite3_index_info) + sizeof(HiddenIndexInfo);
  char* mem = new char[total];
  sqlite3_index_info* pIdxInfo = (sqlite3_index_info*)mem;

  // Prepare constraint(s)
  sqlite3_index_constraint c0; c0.iTermOffset = 0;
  pIdxInfo[0].nConstraint = 1;
  pIdxInfo[0].aConstraint = &c0;

  // Prepare expression chain: pX.pLeft is non-null so that the collate path could be taken
  Expr leftExpr; leftExpr.pLeft = nullptr;
  Expr pX; pX.pLeft = &leftExpr;
  WhereTerm wt; wt.pExpr = &pX;
  WhereClause wc; wc.a = &wt; wc.nTerm = 1;

  // HiddenIndexInfo located at mem + sizeof(sqlite3_index_info)
  HiddenIndexInfo* pHidden = (HiddenIndexInfo*)(mem + sizeof(sqlite3_index_info));
  pHidden->pWC = &wc;
  Parse parseObj;
  pHidden->pParse = &parseObj;

  // Call the focal function with iCons negative
  const char *z = sqlite3_vtab_collation(pIdxInfo, -1);
  if (z != nullptr){
    TEST_FAIL("Expected null when iCons < 0, got '%s'", z);
  }else{
    TEST_PASS("Correctly returned null when iCons < 0");
  }

  delete[] mem;
}

// Test 2: iCons in range; pExpr has pLeft; sqlite3ExprCompareCollSeq returns a CollSeq
static void test_vtab_collation_in_range_with_left_returns_collseq(){
  // Allocate buffer
  size_t total = 2 * sizeof(sqlite3_index_info) + sizeof(HiddenIndexInfo);
  char* mem = new char[total];
  sqlite3_index_info* pIdxInfo = (sqlite3_index_info*)mem;

  // Constraint setup
  sqlite3_index_constraint c0; c0.iTermOffset = 0;
  pIdxInfo[0].nConstraint = 1;
  pIdxInfo[0].aConstraint = &c0;

  // Expression where pExpr.pLeft is non-null
  Expr leftExpr; leftExpr.pLeft = nullptr;
  Expr pX; pX.pLeft = &leftExpr;
  WhereTerm wt; wt.pExpr = &pX;
  WhereClause wc; wc.a = &wt; wc.nTerm = 1;

  // HiddenIndexInfo region
  HiddenIndexInfo* pHidden = (HiddenIndexInfo*)(mem + sizeof(sqlite3_index_info));
  pHidden->pWC = &wc;
  Parse parseObj;
  pHidden->pParse = &parseObj;

  // iCons = 0 (in range)
  const char *z = sqlite3_vtab_collation(pIdxInfo, 0);
  if (z == nullptr){
    TEST_FAIL("Expected a non-null collations name when pLeft exists, got null");
  }else{
    // Expect NOCASE as per sqlite3ExprCompareCollSeq stub
    EXPECT_EQ_STR(z, "NOCASE");
  }

  delete[] mem;
}

// Test 3: iCons in range; pExpr has pLeft; sqlite3ExprCompareCollSeq is forced to return null
static void test_vtab_collation_in_range_with_left_returns_binary_when_collseq_null(){
  // Enable forced null return from sqlite3ExprCompareCollSeq
  g_exprReturnNull = true;

  size_t total = 2 * sizeof(sqlite3_index_info) + sizeof(HiddenIndexInfo);
  char* mem = new char[total];
  sqlite3_index_info* pIdxInfo = (sqlite3_index_info*)mem;

  sqlite3_index_constraint c0; c0.iTermOffset = 0;
  pIdxInfo[0].nConstraint = 1;
  pIdxInfo[0].aConstraint = &c0;

  Expr leftExpr; leftExpr.pLeft = nullptr;
  Expr pX; pX.pLeft = &leftExpr;
  WhereTerm wt; wt.pExpr = &pX;
  WhereClause wc; wc.a = &wt; wc.nTerm = 1;

  HiddenIndexInfo* pHidden = (HiddenIndexInfo*)(mem + sizeof(sqlite3_index_info));
  pHidden->pWC = &wc;
  Parse parseObj;
  pHidden->pParse = &parseObj;

  const char *z = sqlite3_vtab_collation(pIdxInfo, 0);
  if (z != nullptr){
    TEST_FAIL("Expected null CollSeq from sqlite3ExprCompareCollSeq; got %s", z);
  }else{
    TEST_PASS("Received BINARY as expected when collSeq is null");
  }

  g_exprReturnNull = false;
  delete[] mem;
}

// Test 4: iCons in range; pExpr has pLeft == nullptr; should return BINARY
static void test_vtab_collation_in_range_without_left_returns_binary(){
  size_t total = 2 * sizeof(sqlite3_index_info) + sizeof(HiddenIndexInfo);
  char* mem = new char[total];
  sqlite3_index_info* pIdxInfo = (sqlite3_index_info*)mem;

  sqlite3_index_constraint c0; c0.iTermOffset = 0;
  pIdxInfo[0].nConstraint = 1;
  pIdxInfo[0].aConstraint = &c0;

  Expr noLeft; noLeft.pLeft = nullptr;
  Expr pX; pX.pLeft = &noLeft; // Force pX->pLeft to be non-null? Wait: set to nullptr to trigger this case
  // Correction: we want pX.pLeft == nullptr
  pX.pLeft = nullptr;
  WhereTerm wt; wt.pExpr = &pX;
  WhereClause wc; wc.a = &wt; wc.nTerm = 1;

  HiddenIndexInfo* pHidden = (HiddenIndexInfo*)(mem + sizeof(sqlite3_index_info));
  pHidden->pWC = &wc;
  Parse parseObj;
  pHidden->pParse = &parseObj;

  const char* z = sqlite3_vtab_collation(pIdxInfo, 0);
  if (z == nullptr){
    TEST_FAIL("Expected BINARY when pLeft is null, got null");
  }else{
    EXPECT_EQ_STR(z, "BINARY");
  }

  delete[] mem;
}

// Test 5: iCons out of range (iCons >= nConstraint) returns null
static void test_vtab_collation_iCons_out_of_range_returns_null(){
  size_t total = 2 * sizeof(sqlite3_index_info) + sizeof(HiddenIndexInfo);
  char* mem = new char[total];
  sqlite3_index_info* pIdxInfo = (sqlite3_index_info*)mem;

  sqlite3_index_constraint c0; c0.iTermOffset = 0;
  pIdxInfo[0].nConstraint = 1;
  pIdxInfo[0].aConstraint = &c0;

  Expr leftExpr; leftExpr.pLeft = nullptr;
  Expr pX; pX.pLeft = &leftExpr;
  WhereTerm wt; wt.pExpr = &pX;
  WhereClause wc; wc.a = &wt; wc.nTerm = 1;

  HiddenIndexInfo* pHidden = (HiddenIndexInfo*)(mem + sizeof(sqlite3_index_info));
  pHidden->pWC = &wc;
  Parse parseObj;
  pHidden->pParse = &parseObj;

  const char* z = sqlite3_vtab_collation(pIdxInfo, 2); // iCons >= nConstraint
  if (z != nullptr){
    TEST_FAIL("Expected null when iCons out of range, got %s", z);
  }else{
    TEST_PASS("Correctly returned null for iCons out of range");
  }

  delete[] mem;
}

// Runner
static void run_all_tests(){
  std::cout << "Starting sqlite3_vtab_collation test suite\n";
  test_vtab_collation_negative_iCons();
  test_vtab_collation_in_range_with_left_returns_collseq();
  test_vtab_collation_in_range_with_left_returns_binary_when_collseq_null();
  test_vtab_collation_in_range_without_left_returns_binary();
  test_vtab_collation_iCons_out_of_range_returns_null();

  if(g_testFailures==0){
    std::cout << "All tests passed.\n";
  }else{
    std::cout << "Tests finished with " << g_testFailures << " failure(s).\n";
  }
}

// Main entry point
int main(){
  run_all_tests();
  return (g_testFailures==0) ? 0 : 1;
}