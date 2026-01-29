// Minimal C++11 test suite for sqlite3Fts3EvalPhraseStats
// This test avoids GTest and uses a lightweight test harness.
// It mocks dependent functionality to achieve controlled coverage.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


extern "C" {
  // Declaration of the focal function under test.
  // The real implementation resides in fts3.c; here we only declare it
  // so the linker can bind during tests.
  int sqlite3Fts3EvalPhraseStats(
      struct Fts3Cursor *pCsr,
      struct Fts3Expr *pExpr,
      uint32_t *aiOut
  );

  // Mockable helper used by the focal function in the non-first branch.
  // We provide a non-static definition here so tests can control rc values.
  int fts3EvalGatherStats(struct Fts3Cursor *pCsr, struct Fts3Expr *pExpr);
}

// Lightweight C-compatible structures to mirror the focal API's dependencies.
// These definitions are purposely minimal to exercise the logic without
// pulling in the entire sqlite3/FTS3 codebase.

typedef uint32_t u32;
#define SQLITE_OK 0

// Forward declarations for test-scoped types
struct Fts3Table;
struct Fts3ExprParent;
struct Fts3Expr;
struct Fts3Cursor;

// Simple representation of a virtual-table base in the cursor
struct Fts3Base {
  void *pVtab;
};

// Cursor type used by sqlite3Fts3EvalPhraseStats
struct Fts3Cursor {
  Fts3Base base;
  int nDoc; // number of documents in the cursor context
};

// Table metadata used by the focal function to determine counts
struct Fts3Table {
  int nColumn;
};

// Expression dependency structure (minimal)
struct Fts3ExprParent {
  int eType; // expression type (e.g., FTSQUERY_NEAR)
};

struct Fts3Expr {
  int bDeferred;              // whether the expression is deferred
  Fts3ExprParent *pParent;     // parent expression
  unsigned int *aMI;           // micro-index statistics (mocked)
};

// Global control for mock of fts3EvalGatherStats
static int g_mockReturnCode = 0;

// Mock implementation of fts3EvalGatherStats used by tests.
// It fills pExpr->aMI with deterministic values based on the table columns
// when rc==SQLITE_OK, otherwise it returns an error code.
extern "C" int fts3EvalGatherStats(struct Fts3Cursor *pCsr, struct Fts3Expr *pExpr) {
  if (g_mockReturnCode != SQLITE_OK) {
    return g_mockReturnCode;
  }
  if (!pCsr || !pExpr) return SQLITE_OK;

  Fts3Table *pTab = (Fts3Table *)pCsr->base.pVtab;
  if (!pTab) return SQLITE_OK;

  if (!pExpr->aMI) return SQLITE_OK; // nothing to fill if not allocated

  for (int iCol = 0; iCol < pTab->nColumn; ++iCol) {
    // Fill aMI[iCol*3 + 1] and aMI[iCol*3 + 2] with deterministic test data
    pExpr->aMI[iCol * 3 + 1] = (unsigned int)((iCol + 1) * 10 + 1);
    pExpr->aMI[iCol * 3 + 2] = (unsigned int)((iCol + 1) * 10 + 2);
  }
  return SQLITE_OK;
}

// Simple test assertion helper: non-terminating (does not exit on failure)
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { \
  std::cerr << "ASSERT_TRUE FAILED: " << (msg) << " (" << #cond << ")" << std::endl; \
  ++g_failCount; } } while(0)

#define ASSERT_EQ(expected, actual, msg) do { \
  if(!((expected) == (actual))) { \
    std::cerr << "ASSERT_EQ FAILED: " << (msg) << " | expected: " << (expected) \
              << ", actual: " << (actual) << std::endl; \
    ++g_failCount; \
  } \
} while(0)

static int g_failCount = 0;

// Helper to create a printable view for a uint32_t value
static const char* to_str_uint32(uint32_t v) {
  static char buf[32];
  std::snprintf(buf, sizeof(buf), "%u", v);
  return buf;
}

/*
  Test 1: Branch when pExpr->bDeferred is true and pExpr->pParent->eType != FTSQUERY_NEAR
  Expected behavior:
  - aiOut is populated for each column with the value pCsr->nDoc in positions
    iCol*3 + 1 and iCol*3 + 2.
  - rc is SQLITE_OK.
*/
bool test_evalPhraseStats_branchDeferred() {
  // Setup
  Fts3Table tab;
  tab.nColumn = 3;

  Fts3Cursor csr;
  csr.nDoc = 10;
  csr.base.pVtab = &tab;

  Fts3ExprParent parent;
  parent.eType = 0; // not FTSQUERY_NEAR
  Fts3Expr expr;
  expr.bDeferred = 1;
  expr.pParent = &parent;
  expr.aMI = nullptr; // not used in this branch

  uint32_t aiOut[3 * 3];
  // Initialize to non-zero to ensure only specific indices are updated
  for (int i = 0; i < 9; ++i) aiOut[i] = 0xDEADBEEF;

  // Exercise
  extern "C" int sqlite3Fts3EvalPhraseStats(struct Fts3Cursor*, struct Fts3Expr*, uint32_t*);
  int rc = sqlite3Fts3EvalPhraseStats(&csr, &expr, aiOut);

  // Verify
  ASSERT_EQ(SQLITE_OK, rc, "rc should be SQLITE_OK for branchDeferred test");
  // The three columns should have nDoc in indices 1,2; 4,5; 7,8
  for (int iCol = 0; iCol < tab.nColumn; ++iCol) {
    uint32_t v1 = aiOut[iCol * 3 + 1];
    uint32_t v2 = aiOut[iCol * 3 + 2];
    ASSERT_EQ((uint32_t)csr.nDoc, v1, "aiOut[iCol*3+1] should equal nDoc");
    ASSERT_EQ((uint32_t)csr.nDoc, v2, "aiOut[iCol*3+2] should equal nDoc");
  }

  // Also ensure other entries were not overwritten by the branch
  ASSERT_TRUE(aiOut[0] == 0xDEADBEEF, "aiOut[0] should remain unchanged");
  ASSERT_TRUE(aiOut[3] == 0xDEADBEEF, "aiOut[3] should remain unchanged");
  ASSERT_TRUE(aiOut[6] == 0xDEADBEEF, "aiOut[6] should remain unchanged");

  return g_failCount == 0;
}

/*
  Test 2: Branch when pExpr->bDeferred is false (or parent is near) and
  fts3EvalGatherStats returns SQLITE_OK.
  Expected behavior:
  - aiOut is populated from pExpr->aMI after the gather stats fill.
  - rc is SQLITE_OK.
*/
bool test_evalPhraseStats_branchGatherStatsOk() {
  // Setup
  Fts3Table tab;
  tab.nColumn = 2;

  Fts3Cursor csr;
  csr.nDoc = 0; // not used in this path
  csr.base.pVtab = &tab;

  Fts3ExprParent parent;
  parent.eType = 0; // not FTSQUERY_NEAR
  Fts3Expr expr;
  expr.bDeferred = 0;
  expr.pParent = &parent;

  // Allocate aMI array expected to be sized for nColumn*3
  int miSize = tab.nColumn * 3;
  expr.aMI = new unsigned int[miSize];
  // Initialize to 0 to ensure we properly verify the fill
  std::memset(expr.aMI, 0, miSize * sizeof(unsigned int));

  uint32_t aiOut[tab.nColumn * 3];
  // Ensure aiOut is separate from aMI so we can verify the data transfer
  std::memset(aiOut, 0, sizeof(aiOut));

  // Ensure mock returns OK and fills aMI via the mock
  g_mockReturnCode = SQLITE_OK;

  // Exercise
  extern "C" int sqlite3Fts3EvalPhraseStats(struct Fts3Cursor*, struct Fts3Expr*, uint32_t*);
  int rc = sqlite3Fts3EvalPhraseStats(&csr, &expr, aiOut);

  // Verify
  ASSERT_EQ(SQLITE_OK, rc, "rc should be SQLITE_OK for branchGatherStatsOk test");
  // aiOut should reflect aMI entries: indices 1,2 and 4,5
  // Our mock fills aMI as:
  // iCol=0 -> aMI[1] = 11, aMI[2] = 12
  // iCol=1 -> aMI[4] = 21, aMI[5] = 22
  ASSERT_EQ((uint32_t)11, aiOut[1], "aiOut[1] should copy aMI[1] (col0)");
  ASSERT_EQ((uint32_t)12, aiOut[2], "aiOut[2] should copy aMI[2] (col0)");
  ASSERT_EQ((uint32_t)21, aiOut[4], "aiOut[4] should copy aMI[4] (col1)");
  ASSERT_EQ((uint32_t)22, aiOut[5], "aiOut[5] should copy aMI[5] (col1)");

  // Cleanup
  delete[] expr.aMI;

  return g_failCount == 0;
}

/*
  Test 3: Branch where fts3EvalGatherStats returns a non-SQLITE_OK code.
  Expected behavior:
  - rc is non-zero and aiOut remains unchanged from its initial state.
*/
bool test_evalPhraseStats_branchGatherStatsError() {
  // Setup similar to Test 2
  Fts3Table tab;
  tab.nColumn = 2;

  Fts3Cursor csr;
  csr.nDoc = 0;
  csr.base.pVtab = &tab;

  Fts3ExprParent parent;
  parent.eType = 0;
  Fts3Expr expr;
  expr.bDeferred = 0;
  expr.pParent = &parent;

  int miSize = tab.nColumn * 3;
  expr.aMI = new unsigned int[miSize];
  std::memset(expr.aMI, 0, miSize * sizeof(unsigned int));

  uint32_t aiOut[tab.nColumn * 3];
  // Initialize to a sentinel value so we can detect changes
  for (int i = 0; i < tab.nColumn * 3; ++i) aiOut[i] = 0xA5A5A5A5;

  // Force mock to return error
  g_mockReturnCode = 1; // non-zero error

  // Exercise
  extern "C" int sqlite3Fts3EvalPhraseStats(struct Fts3Cursor*, struct Fts3Expr*, uint32_t*);
  int rc = sqlite3Fts3EvalPhraseStats(&csr, &expr, aiOut);

  // Verify
  ASSERT_EQ(1, rc, "rc should propagate non-OK from gatherStats");
  // aiOut should remain unchanged because rc != SQLITE_OK
  for (int i = 0; i < tab.nColumn * 3; ++i) {
    ASSERT_EQ((uint32_t)0xA5A5A5A5, aiOut[i], "aiOut should remain unchanged on error path");
  }

  // Cleanup
  delete[] expr.aMI;

  return g_failCount == 0;
}

// Entry point for running tests.
int main() {
  int total = 0;
  int passed = 0;

  // Run Test 1
  {
    bool ok = test_evalPhraseStats_branchDeferred();
    ++total;
    if (ok) ++passed;
  }

  // Run Test 2
  {
    bool ok = test_evalPhraseStats_branchGatherStatsOk();
    ++total;
    if (ok) ++passed;
  }

  // Run Test 3
  {
    bool ok = test_evalPhraseStats_branchGatherStatsError();
    ++total;
    if (ok) ++passed;
  }

  std::cout << "Tests run: " << total << ", Passed: " << passed
            << ", Failed: " << (total - passed) << std::endl;

  // Return non-zero if any test failed
  if (g_failCount != 0) {
    return 1;
  }
  return 0;
}