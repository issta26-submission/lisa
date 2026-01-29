// A self-contained C++11 test harness for the focal C function sqlite3CreateColumnExpr.
// The goal is to exercise true/false branches of the function's predicates without
// depending on the real SQLite project build. This file defines minimal
// stand-ins for the necessary SQLite structures and functions, reproduces
// the focal function's logic, and provides a small test framework with
// non-terminating assertions to maximize coverage.

#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Lightweight stand-ins to mimic the necessary SQLite-like types and constants.
// This is a self-contained reproduction strictly for unit-testing the focal path.
using uint64 = uint64_t;

// Forward declaration for sqlite3 placeholder type
struct sqlite3 {};

// Typedef used by the focal method
typedef int ynVar;

// Forward declarations of minimal helper functions/macros
static int ExprUseYTab(struct Expr* p);
static void testcase(int);
static struct Expr* sqlite3ExprAlloc(struct sqlite3* db, int op, int a, int b);

// Lightweight struct definitions matching what the focal code uses (simplified)
struct Table {
  int iPKey;
  int tabFlags;
  int nCol;
  struct ColFlag { int colFlags; } *aCol;
};

struct SrcItem {
  Table* pTab;
  int iCursor;
  uint64 colUsed;
};

struct SrcList {
  int nSrc;
  SrcItem* a; // dynamic array of SrcItem
};

// Minimal Expr type with the fields used by sqlite3CreateColumnExpr
struct Expr {
  int op;
  struct {
    Table* pTab;
  } y;
  int iTable;
  int iColumn;
  uint64 colUsed;
};

// Constants and helpers to mirror the original logic
static const int TF_HasGenerated = 0x01;
static const int COLFLAG_GENERATED = 0x01;
static const int BMS = 64; // per the typical SQLite usage in code paths
static const uint64 ALLBITS = 0xFFFFFFFFFFFFFFFFULL;
#define MASKBIT(n) (1ULL << ((n) - 1))
#define TK_COLUMN 1

// Domain knowledge helpers
static int ExprUseYTab(struct Expr* p) {
  // In the real code, this ensures the expression uses Y tab fields.
  // For test purposes, always return true.
  (void)p;
  return 1;
}
static void testcase(int x) {
  // No-op; placeholder to mimic SQLite's test hooks. Kept non-terminating.
  (void)x;
}

// Fake allocator that can be toggled to simulate allocation failure
static bool g_failExprAlloc = false;
static Expr* sqlite3ExprAlloc(struct sqlite3* db, int op, int a, int b) {
  (void)db; (void)a; (void)b;
  if (g_failExprAlloc) return nullptr;
  Expr* p = new Expr();
  p->op = op;
  p->iTable = 0;
  p->iColumn = 0;
  p->colUsed = 0;
  p->y.pTab = nullptr;
  return p;
}

// Focal method reproduced in the test harness
Expr* sqlite3CreateColumnExpr(sqlite3 *db, SrcList *pSrc, int iSrc, int iCol){
  Expr *p = sqlite3ExprAlloc(db, TK_COLUMN, 0, 0);
  if( p ){
    SrcItem *pItem = &pSrc->a[iSrc];
    Table *pTab;
    assert( ExprUseYTab(p) );
    pTab = p->y.pTab = pItem->pTab;
    p->iTable = pItem->iCursor;
    if( p->y.pTab->iPKey==iCol ){
      p->iColumn = -1;
    }else{
      p->iColumn = (ynVar)iCol;
      if( (pTab->tabFlags & TF_HasGenerated)!=0
       && (pTab->aCol[iCol].colFlags & COLFLAG_GENERATED)!=0
      ){
        testcase( pTab->nCol==63 );
        testcase( pTab->nCol==64 );
        pItem->colUsed = pTab->nCol>=64 ? ALLBITS : MASKBIT(pTab->nCol)-1;
      }else{
        testcase( iCol==BMS );
        testcase( iCol==BMS-1 );
        pItem->colUsed |= ((uint64)1)<<(iCol>=BMS ? BMS-1 : iCol);
      }
    }
  }
  return p;
}

// Simple non-terminating assertion helpers for test verification
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed at " << __func__ << " (" << #cond << ")\n"; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  auto _a = (a); auto _b = (b); \
  if(!(_a == _b)) { \
    std::cerr << "EXPECT_EQ failed at " << __func__ << " (" << #a << " == " << #b \
              << ") -> " << _a << " != " << _b << "\n"; \
  } \
} while(0)

// Helper to create a single SrcItem array
static SrcList* makeSrcList(Table* pTab, int iCursor) {
  SrcList* pSrc = (SrcList*)std::malloc(sizeof(SrcList));
  pSrc->nSrc = 1;
  pSrc->a = new SrcItem[1];
  pSrc->a[0].pTab = pTab;
  pSrc->a[0].iCursor = iCursor;
  pSrc->a[0].colUsed = 0;
  return pSrc;
}

// Test 1: Key column -> iColumn should be -1 and other fields set
static void test_key_branch() {
  // Prepare table
  Table t;
  t.iPKey = 5;
  t.tabFlags = 0;
  t.nCol = 10;
  t.aCol = (Table::ColFlag*)std::malloc(6 * sizeof(Table::ColFlag));
  for(int i=0; i<6; ++i) t.aCol[i].colFlags = 0; // default

  // Prepare SrcList with one SrcItem pointing to t
  SrcList* pSrc = makeSrcList(&t, /*iCursor=*/7);
  // Call
  Expr* e = sqlite3CreateColumnExpr(nullptr, pSrc, 0, 5);

  // Verify
  EXPECT_TRUE(e != nullptr);
  EXPECT_EQ(e->y.pTab, &t);
  EXPECT_EQ(e->iTable, 7);
  EXPECT_EQ(e->iColumn, -1);

  // Cleanup
  std::free(t.aCol);
  delete[] pSrc->a;
  std::free(pSrc);
  delete e;
}

// Test 2: HasGenerated path with generated column flag -> pItem->colUsed set accordingly
static void test_generated_path_variants() {
  // Shared table setup
  Table t;
  t.iPKey = -1;              // not a primary key
  t.tabFlags = TF_HasGenerated; // has generated flag to trigger first sub-branch
  // We'll test two nCol scenarios: 63 and 64
  // We allocate aCol up to index 2 (iCol = 2) for safety
  t.nCol = 63;
  t.aCol = (Table::ColFlag*)std::malloc(3 * sizeof(Table::ColFlag));
  for(int i=0; i<3; ++i) t.aCol[i].colFlags = 0;
  t.aCol[2].colFlags = COLFLAG_GENERATED;

  SrcList* pSrc = makeSrcList(&t, /*iCursor=*/3);
  // iCol = 2 (not a primary key)
  Expr* e = sqlite3CreateColumnExpr(nullptr, pSrc, 0, 2);

  // Expected: since nCol == 63 (<64), colUsed = MASKBIT(63) - 1
  uint64 expected = MASKBIT(63) - 1;
  // But pItem references are via pSrc->a[0], allow direct access
  EXPECT_TRUE(e != nullptr);
  // We can't directly access pItem->colUsed from e; instead we check through pSrc
  // The original code sets pItem->colUsed inside the SrcList item:
  EXPECT_EQ(pSrc->a[0].colUsed, expected);

  // Cleanup
  delete e;
  std::free(t.aCol);
  delete[] pSrc->a;
  std::free(pSrc);

  // Second sub-test: nCol = 64 -> ALLBITS
  Table t2;
  t2.iPKey = -1;
  t2.tabFlags = TF_HasGenerated;
  t2.nCol = 64;
  t2.aCol = (Table::ColFlag*)std::malloc(65 * sizeof(Table::ColFlag));
  for(int i=0; i<65; ++i) t2.aCol[i].colFlags = 0;
  t2.aCol[2].colFlags = COLFLAG_GENERATED;

  SrcList* pSrc2 = makeSrcList(&t2, 0);
  Expr* e2 = sqlite3CreateColumnExpr(nullptr, pSrc2, 0, 2);
  EXPECT_TRUE(e2 != nullptr);
  EXPECT_EQ(pSrc2->a[0].colUsed, ALLBITS);

  // Cleanup
  delete e2;
  std::free(t2.aCol);
  delete[] pSrc2->a;
  std::free(pSrc2);
}

// Test 3: Not generated path (or missing flags) -> simple bit shift expectation
static void test_non_generated_path_bits() {
  // Table with no HasGenerated flag
  Table t;
  t.iPKey = -1;
  t.tabFlags = 0;
  t.nCol = 10;
  t.aCol = (Table::ColFlag*)std::malloc(11 * sizeof(Table::ColFlag));
  for(int i=0; i<11; ++i) t.aCol[i].colFlags = 0;

  SrcList* pSrc = makeSrcList(&t, /*iCursor=*/1);
  Expr* e = sqlite3CreateColumnExpr(nullptr, pSrc, 0, 0); // iCol = 0

  // We expect pSrc->a[0].colUsed to have bit 0 set: 1
  EXPECT_TRUE(e != nullptr);
  EXPECT_EQ(pSrc->a[0].colUsed, 1ULL);

  // Cleanup
  delete e;
  std::free(t.aCol);
  delete[] pSrc->a;
  std::free(pSrc);
}

// Test 4: iCol in BMS boundary (iCol >= BMS path)
static void test_bms_boundary() {
  // Table without special generated flag
  Table t;
  t.iPKey = -1;
  t.tabFlags = 0;
  t.nCol = 20;
  t.aCol = (Table::ColFlag*)std::malloc(21 * sizeof(Table::ColFlag));
  for(int i=0; i<21; ++i) t.aCol[i].colFlags = 0;

  SrcList* pSrc = makeSrcList(&t, /*iCursor=*/2);
  // iCol = BMS (64) to exercise branch where iCol>=BMS
  Expr* e = sqlite3CreateColumnExpr(nullptr, pSrc, 0, BMS);

  // After call, pSrc->a[0].colUsed should reflect 1 << (BMS-1)
  uint64 expected = 1ULL << (BMS - 1);
  EXPECT_TRUE(e != nullptr);
  EXPECT_EQ(pSrc->a[0].colUsed, expected);

  // Cleanup
  delete e;
  std::free(t.aCol);
  delete[] pSrc->a;
  std::free(pSrc);
}

// Test 5: Allocation failure path returns null
static void test_allocation_failure() {
  g_failExprAlloc = true;
  Table t;
  t.iPKey = -1;
  t.tabFlags = 0;
  t.nCol = 5;
  t.aCol = (Table::ColFlag*)std::malloc(6 * sizeof(Table::ColFlag));
  for(int i=0; i<6; ++i) t.aCol[i].colFlags = 0;
  SrcList* pSrc = makeSrcList(&t, 0);

  Expr* e = sqlite3CreateColumnExpr(nullptr, pSrc, 0, 0);
  EXPECT_TRUE(e == nullptr);

  // Cleanup
  std::free(t.aCol);
  delete[] pSrc->a;
  std::free(pSrc);
  g_failExprAlloc = false;
}

int main() {
  // Run tests; they are designed to be non-terminating and print only failures.
  test_key_branch();
  test_generated_path_variants();
  test_non_generated_path_bits();
  test_bms_boundary();
  test_allocation_failure();

  std::cout << "Unit tests finished.\n";
  return 0;
}