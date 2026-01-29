// test_resolve_exprcolused.cpp
// Minimal, self-contained C++11 test harness for sqlite3ExprColUsed
// Note: This test suite is designed to work within a project that exposes the
// internal SQLite structures (Expr, Table, etc.) via sqliteInt.h (or equivalent).
// It follows the instructions to avoid GoogleTest and instead uses a small
// in-file test harness with non-terminating expectations (via custom EXPECT_*).
// The code includes explanatory comments for each unit test.

// Important: This file assumes the build environment provides the exact internal
// SQLite definitions (Expr, Table, Bitmask, BMS, ALLBITS, MASKBIT, etc.)
// as used by sqlite3ExprColUsed. If your environment uses different internal
// representations, please adapt the test scaffolding accordingly.

#include <vector>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cstdint>


// Forward declarations and macros are pulled from the project's internal headers.
// We rely on sqliteInt.h (or equivalent) to define Expr, Table, Bitmask, and macros.
extern "C" {
  // The focal function under test (as defined in resolve.c)
  // Signature matches: Bitmask sqlite3ExprColUsed(Expr *pExpr);
  typedef unsigned int Bitmask;
  // Forward declare to avoid depending on the actual header layout here.
  struct Expr;
  Bitmask sqlite3ExprColUsed(struct Expr *pExpr);
}

// To maximize compatibility across environments, we try to pull in
// the internal SQLite types/macros if available. If not available in your
// environment, you will need to adjust the included headers accordingly.
#ifdef SQLITE_INT_HEADER_AVAILABLE
  // If the environment provides the real headers, include them.
  #include "sqliteInt.h" // provides Expr, Table, Bitmask, etc., including BMS, ALLBITS, MASKBIT
#endif

// Fallback lightweight stub definitions (only used if the real internal headers
// are not available in the test environment). This is for illustrative purposes
// and should be replaced with the real internal headers in a full test setup.
// Do not rely on these stubs for actual test execution against real SQLite code.
#ifndef SQLITE_INT_HEADER_AVAILABLE
  // Basic, approximate definitions to allow compilation in a simplified environment.
  typedef unsigned int Bitmask;
  #define BMS 8
  #define ALLBITS ((Bitmask)~(Bitmask)0)
  #define MASKBIT(n) ((Bitmask)1 << (n))

  struct ColInfo {
    unsigned int colFlags;
  };

  struct Table {
    unsigned int tabFlags;
    int nCol;
    ColInfo *aCol; // dynamic array of column info
  };

  struct YTab {
    Table *pTab;
  };

  struct Expr {
    int iColumn;
    YTab y;
  };

  // Optional: define TF_HasGenerated and COLFLAG_GENERATED if not present
  #ifndef TF_HasGenerated
  #define TF_HasGenerated 0x01
  #endif
  #ifndef COLFLAG_GENERATED
  #define COLFLAG_GENERATED 0x01
  #endif
  #ifndef ExprUseYTab
  #define ExprUseYTab(p) ((p)->y.pTab != nullptr)
  #endif
#endif

// Simple, non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Lightweight assertion helpers that do not terminate the test program.
// They log failures but continue executing subsequent tests.
#define EXPECT_EQ_INT(a, b) do { \
  ++g_total_tests; \
  if ((a) != (b)) { \
    fprintf(stderr, "EXPECT_EQ_INT failed at %s:%d: %ld != %ld\n", __FILE__, __LINE__, (long)(a), (long)(b)); \
    ++g_failed_tests; \
  } \
} while(0)

#define EXPECT_TRUE(x) do { \
  ++g_total_tests; \
  if (!(x)) { \
    fprintf(stderr, "EXPECT_TRUE failed at %s:%d: expression is false\n", __FILE__, __LINE__); \
    ++g_failed_tests; \
  } \
} while(0)

#define EXPECT_NOT_NULL(p) do { \
  ++g_total_tests; \
  if ((p) == nullptr) { \
    fprintf(stderr, "EXPECT_NOT_NULL failed at %s:%d: pointer is NULL\n", __FILE__, __LINE__); \
    ++g_failed_tests; \
  } \
} while(0)


// Helper to allocate a mock Table with aCol array of required size.
// This mirrors, as closely as possible, the minimal layout used by sqlite3ExprColUsed:
// - pExTab->tabFlags
// - pExTab->aCol[n].colFlags
// - pExTab->nCol
// - pExpr->iColumn and pExpr->y.pTab
template <typename ColFlagsType = unsigned int>
static Table* make_mock_table(int nCol, unsigned int tabFlags, int required_col_index, unsigned int colFlagForIndex) {
  // Allocate aTable with aCol array large enough for required_col_index
  // We assume aCol is a simple array (ColInfo) in Table as per fallback stub.
  const size_t base_size = sizeof(Table);
  const size_t per_col = sizeof(ColInfo);
  size_t total = base_size + (required_col_index + 1) * per_col;
  Table* pTab = (Table*)std::malloc(total);
  if (!pTab) return nullptr;
  std::memset(pTab, 0, total);
  pTab->tabFlags = (unsigned int)tabFlags;
  pTab->nCol = nCol;
  // Initialize aCol array
  pTab->aCol = (ColInfo*)((char*)pTab + offsetof(Table, aCol));
  // Fill the necessary index
  for (int i = 0; i <= required_col_index; ++i) {
    pTab->aCol[i].colFlags = 0;
  }
  pTab->aCol[required_col_index].colFlags = colFlagForIndex;
  return pTab;
}

// Helper to build an Expr referencing a mock Table
static Expr* make_mock_expr(int iColumn, Table* pTab) {
  Expr* pExpr = (Expr*)std::malloc(sizeof(Expr));
  if (!pExpr) return nullptr;
  std::memset(pExpr, 0, sizeof(Expr));
  pExpr->iColumn = iColumn;
  pExpr->y.pTab = pTab;
  return pExpr;
}

// Cleanup helpers
static void free_mock_expr(Expr* pExpr) {
  if (pExpr) {
    // Free the Table if owned (we own only in tests)
    free_mock_table(pExpr->y.pTab);
    std::free(pExpr);
  }
}
static void free_mock_table(Table* pTab) {
  if (pTab) {
    // If aCol was allocated as part of pTab memory, it is inside the same block here.
    // In the simple stub, aCol offset is inside the allocated block, so just free the block.
    std::free(pTab);
  }
}

/*
  Step 1: True branch (generated column) with nCol < BMS
  Conditions:
  - pExpr->iColumn is some valid index (e.g., 0)
  - pExTab->tabFlags has TF_HasGenerated
  - pExTab->aCol[n].colFlags has COLFLAG_GENERATED
  - pExTab->nCol < BMS
  Expected:
  - return MASKBIT(pExTab->nCol) - 1
*/
static bool test_generated_branch_before_BMS() {
  const int nColumnIndex = 0;
  const int nColValue = BMS - 1; // ensure < BMS
  // Build mock table
  Table* pExTab = make_mock_table< unsigned int >(nColValue, 0 /* tabFlags*/, nColumnIndex, COLFLAG_GENERATED);
  if (!pExTab) return false;
  pExTab->tabFlags |= TF_HasGenerated;
  pExTab->nCol = nColValue;
  // Build expression referencing this table
  Expr* pExpr = make_mock_expr(nColumnIndex, pExTab);
  if (!pExpr) {
    free_mock_table(pExTab);
    return false;
  }

  // Call the focal function
  Bitmask result = sqlite3ExprColUsed(pExpr);

  // Expected
  Bitmask expected = MASKBIT(pExTab->nCol) - 1;
  EXPECT_EQ_INT((long long)result, (long long)expected);

  // Cleanup
  free_mock_expr(pExpr);
  return true;
}

/*
  Step 1b: True branch with nCol >= BMS
  - pExTab->nCol >= BMS should yield ALLBITS
*/
static bool test_generated_branch_at_or_above_BMS() {
  const int nColumnIndex = 0;
  // Make nCol >= BMS
  Table* pExTab = make_mock_table< unsigned int >(BMS, 0, nColumnIndex, COLFLAG_GENERATED);
  if (!pExTab) return false;
  pExTab->tabFlags |= TF_HasGenerated;
  pExTab->nCol = BMS; // >= BMS
  Expr* pExpr = make_mock_expr(nColumnIndex, pExTab);
  if (!pExpr) { free_mock_table(pExTab); return false; }

  Bitmask result = sqlite3ExprColUsed(pExpr);
  EXPECT_EQ_INT((long long)result, (long long)ALLBITS);

  free_mock_expr(pExpr);
  return true;
}

/*
  Step 2: Non-generated branch with n < BMS
  - pExTab->tabFlags does not include TF_HasGenerated OR aCol[n].colFlags lacks COLFLAG_GENERATED
  - n = BMS - 1
  Expected:
  - return ((Bitmask)1) << (BMS-1)
*/
static bool test_non_generated_branch_before_BMS() {
  const int nColumnIndex = BMS - 1;
  Table* pExTab = make_mock_table< unsigned int >(BMS - 1, 0, nColumnIndex, 0); // no generation flag
  if (!pExTab) return false;
  // do not set generated flag
  pExTab->tabFlags = 0;
  pExTab->nCol = BMS - 1;
  // Ensure aCol[n] has colFlags 0 (non-generated)
  // Not strictly necessary since default is 0, but set explicitly
  pExTab->aCol[nColumnIndex].colFlags = 0;

  Expr* pExpr = make_mock_expr(nColumnIndex, pExTab);
  if (!pExpr) { free_mock_table(pExTab); return false; }

  Bitmask result = sqlite3ExprColUsed(pExpr);
  Bitmask expected = ((Bitmask)1) << nColumnIndex;
  EXPECT_EQ_INT((long long)result, (long long)expected);

  free_mock_expr(pExpr);
  return true;
}

/*
  Step 3: Non-generated branch with n >= BMS
  - n = BMS
  Expected:
  - return ((Bitmask)1) << (BMS-1)
*/
static bool test_non_generated_branch_at_or_above_BMS() {
  const int nColumnIndex = BMS;
  Table* pExTab = make_mock_table< unsigned int >(BMS - 1, 0, nColumnIndex, 0);
  if (!pExTab) return false;
  pExTab->tabFlags = 0;
  pExTab->nCol = BMS - 1;
  // Ensure aCol[BMS] exists; extend array if necessary in your real environment.
  // In this stub, we allocate until BMS index.
  // For simplicity, we set the index with 0 flag
  if (BMS > 0) pExTab->aCol[BMS].colFlags = 0;

  Expr* pExpr = make_mock_expr(nColumnIndex, pExTab);
  if (!pExpr) { free_mock_table(pExTab); return false; }

  Bitmask result = sqlite3ExprColUsed(pExpr);
  Bitmask expected = ((Bitmask)1) << (BMS - 1);
  EXPECT_EQ_INT((long long)result, (long long)expected);

  free_mock_expr(pExpr);
  return true;
}

// Entry point for test execution
int main() {
  printf("Starting sqlite3ExprColUsed test suite (non-terminating, logging failures).\n");

  // Run tests
  bool ok = true;
  ok &= test_generated_branch_before_BMS();
  ok &= test_generated_branch_at_or_above_BMS();
  ok &= test_non_generated_branch_before_BMS();
  ok &= test_non_generated_branch_at_or_above_BMS();

  // Summary
  printf("Ran %d test(s). Passed: %d. Failed: %d.\n",
         g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);

  // Clean exit: return 0 if all tests passed, non-zero otherwise
  return (g_failed_tests == 0) ? 0 : 1;
}

// Notes for future maintainers:
// - This test suite relies on internal SQLite structures. If your build uses
//   different definitions or if a flexible array member is used for aCol,
//   you may need to adapt memory allocations accordingly.
// - The tests cover the two primary branches of sqlite3ExprColUsed: the
//   "generated" path and the "non-generated" path, and also validate the
//   boundary behavior around the BMS value.
// - The test harness uses non-terminating expectations to allow multiple tests
//   to execute in a single run, enabling higher coverage in a single pass.