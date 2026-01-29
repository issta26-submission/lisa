/*
  Test Suite for sqlite3Fts5ParseNode (fts5_expr.c)

  - This test suite targets the focal method sqlite3Fts5ParseNode, exercising
    its early-return branches for non-STRING expressions and STRING with near
    support disabled.
  - The tests are written in plain C++11 (no Google Test). They rely on the
    project's existing FTS5 internal types and constants exposed via the
    provided headers (e.g., fts5Int.h, fts5parse.h) and the sqlite3.h API.
  - Each test includes explanatory comments describing intent and what is being verified.
  - A lightweight test harness is used (no terminating asserts). Failures are
    collected and reported at program end.
  - Note: We implement tests that avoid triggering internal assertions by
    the RC-check branch (pParse->rc != SQLITE_OK) since that path contains an
    assertion in the focal code. Instead, we focus on safe, deterministic branches.
*/

#include <fts5Int.h>
#include <cstdio>
#include <fts5parse.h>
#include <sqlite3.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


extern "C" {

// Include the real FTS5 internal headers to get the correct type definitions
// and constants used by sqlite3Fts5ParseNode. These headers are part of the
// project sources and provide the necessary API surface for testing.

// Declaration of the focal function under test (C linkage)
Fts5ExprNode *sqlite3Fts5ParseNode(
  Fts5Parse *pParse,
  int eType,
  Fts5ExprNode *pLeft,
  Fts5ExprNode *pRight,
  Fts5ExprNearset *pNear
);

} // extern "C"

// Simple test assertion helper (non-terminating)
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "TEST FAIL: " << (msg) << " (in " << __FUNCTION__ << ")\n"; \
    ++g_failures; \
  } \
} while(0)

/*
  Helper: create a minimal dummy Fts5ExprNode instance.
  We do not rely on internal layout beyond the fields we touch in the tests.
*/
static Fts5ExprNode* makeDummyNode(int eType) {
  Fts5ExprNode *p = (Fts5ExprNode*)malloc(sizeof(Fts5ExprNode));
  if(!p) return nullptr;
  // Initialize only a subset of fields; real code uses more, but tests below
  // exercise early branches that do not dereference uninitialized fields.
  p->eType = eType;
  p->nChild = 0;
  p->iHeight = 0;
  p->pNear = nullptr;
  p->xNext = nullptr;
  return p;
}

/*
  Test 1: STRING expression with near set null should return 0 immediately.

  Rationale:
  - The focal method contains a guard:
      if( eType==FTS5_STRING && pNear==0 ) return 0;
  - This test ensures that true to the contract, the function returns 0
    without attempting further processing when near data is missing.
*/
static bool test_string_with_null_near_returns_null() {
  Fts5Parse parse;
  parse.rc = SQLITE_OK;       // OK parse state
  parse.bPhraseToAnd = 0;
  parse.pConfig = nullptr;

  Fts5ExprNode *pLeft = nullptr;
  Fts5ExprNode *pRight = nullptr;
  Fts5ExprNearset *pNear = nullptr;

  Fts5ExprNode *res = sqlite3Fts5ParseNode(&parse, FTS5_STRING, pLeft, pRight, pNear);

  TEST_ASSERT(res == nullptr, "STRING with null pNear should return null");
  return g_failures == 0;
}

/*
  Test 2: Non-STRING expression with pLeft == nullptr should return pRight.

  Rationale:
  - The function contains:
      if( eType!=FTS5_STRING && pLeft==0 ) return pRight;
  - This test ensures that when the left child is absent, the right child
    is propagated up without modification.
*/
static bool test_nonstring_left_null_returns_right() {
  Fts5Parse parse;
  parse.rc = SQLITE_OK;
  parse.bPhraseToAnd = 0;
  parse.pConfig = nullptr;

  // Right child is a valid dummy node; left is null to trigger the fast-path
  Fts5ExprNode *pLeft = nullptr;
  Fts5ExprNode *pRight = makeDummyNode(FTS5_STRING); // type can be arbitrary here
  Fts5ExprNearset *pNear = nullptr;

  Fts5ExprNode *res = sqlite3Fts5ParseNode(&parse, FTS5_NOT, pLeft, pRight, pNear);

  TEST_ASSERT(res == pRight, "Non-string with left==nullptr should return right child pointer");
  return g_failures == 0;
}

/*
  Test 3: Non-STRING expression with pRight == nullptr should return pLeft.

  Rationale:
  - The function contains:
      if( eType!=FTS5_STRING && pRight==0 ) return pLeft;
  - This test ensures that when the right child is absent, the left child
    is propagated up without modification.
*/
static bool test_nonstring_right_null_returns_left() {
  Fts5Parse parse;
  parse.rc = SQLITE_OK;
  parse.bPhraseToAnd = 0;
  parse.pConfig = nullptr;

  Fts5ExprNode *pLeft = makeDummyNode(FTS5_STRING);
  Fts5ExprNode *pRight = nullptr;
  Fts5ExprNearset *pNear = nullptr;

  Fts5ExprNode *res = sqlite3Fts5ParseNode(&parse, FTS5_AND, pLeft, pRight, pNear);

  TEST_ASSERT(res == pLeft, "Non-string with right==nullptr should return left pointer");
  return g_failures == 0;
}

int main(void) {
  // Run tests
  bool ok1 = test_string_with_null_near_returns_null();
  bool ok2 = test_nonstring_left_null_returns_right();
  bool ok3 = test_nonstring_right_null_returns_left();

  // Summary
  std::cout << "FTS5ParseNode test summary: "
            << (g_failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
            << "\n";

  // Return non-zero if any test failed
  return g_failures;
}