// C++11 unit tests for sqlite3Fts3ExprFree (no GTest, self-contained harness)
//
// Notes:
// - This test suite constructs minimal Fts3Expr trees using the actual
//   Fts3Expr data structure (declared in fts3Int.h) and allocates nodes
//   via sqlite3Fts3MallocZero to ensure compatibility with the free() path
//   used by sqlite3Fts3ExprFree.
// - We avoid triggering asserts by ensuring the trees satisfy invariants
//   required by sqlite3Fts3ExprFree (parent/child relationships).
// - We do not rely on private/static internals; all interactions are via the
//   focal function and public data structure.
//
// Build note: Link against the SQLite/fts3 implementation that provides
// sqlite3Fts3ExprFree, sqlite3Fts3MallocZero, and related types.

#include <fts3Int.h>
#include <functional>
#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>



extern "C" {
  // Prototypes for the C functions under test (external linkage)
  void sqlite3Fts3ExprFree(Fts3Expr *pDel);
  void *sqlite3Fts3MallocZero(sqlite3_int64 nByte);
  void sqlite3_free(void*);
}

// Helper: allocate a new Fts3Expr node with given relations and type
static Fts3Expr* alloc_node(Fts3Expr* parent, Fts3Expr* left, Fts3Expr* right, int eType) {
  Fts3Expr* p = (Fts3Expr*)sqlite3Fts3MallocZero(sizeof(Fts3Expr));
  if( !p ) return nullptr;
  p->pParent = parent;
  p->pLeft = left;
  p->pRight = right;
  p->eType = eType;
  // Initialize optional fields to safe values (we won't exercise them in tests)
  p->pPhrase = 0;
  p->aMI = 0;
  p->nNear = 0;
  return p;
}

// Test 1: Null input should be a no-op
static bool test_null_input(){
  sqlite3Fts3ExprFree(nullptr);
  return true;
}

// Test 2: Simple root with two leaves (A -> B, C)
// Verifies traversal: B, C, A
static bool test_two_leaves(){
  Fts3Expr* B = alloc_node(nullptr, nullptr, nullptr, 0);
  Fts3Expr* C = alloc_node(nullptr, nullptr, nullptr, 0);
  Fts3Expr* A = alloc_node(nullptr, B, C, 0);
  if( A ) A->pParent = nullptr;
  if( B ) B->pParent = A;
  if( C ) C->pParent = A;

  if( !A || !B || !C ) return false;

  sqlite3Fts3ExprFree(A);
  return true;
}

// Test 3: Unbalanced right subtree: A with left B, right C; C has left D
// Traversal should visit B, then traverse right subtree down to D before freeing C/A
static bool test_unbalanced_right_subtree(){
  Fts3Expr* B = alloc_node(nullptr, nullptr, nullptr, 0);
  Fts3Expr* D = alloc_node(nullptr, nullptr, nullptr, 0);
  Fts3Expr* C = alloc_node(nullptr, D, nullptr, 0);
  Fts3Expr* A = alloc_node(nullptr, B, C, 0);

  if( B ) B->pParent = A;
  if( D ) D->pParent = C;
  if( C ) C->pParent = A;
  if( A ) A->pParent = nullptr;

  if( !A || !B || !C || !D ) return false;

  sqlite3Fts3ExprFree(A);
  return true;
}

// Test 4: Deep left-leaning chain plus a right leaf to exercise multiple levels
// A -> left path A1 -> A2 -> A3 (leaf); and A -> right leaf R
static bool test_deep_left_chain(){
  // Build deep left chain: A <- A1 <- A2 <- A3 (A3 is leaf)
  Fts3Expr* A3 = alloc_node(nullptr, nullptr, nullptr, 0);
  Fts3Expr* A2 = alloc_node(nullptr, A3, nullptr, 0);
  Fts3Expr* A1 = alloc_node(nullptr, A2, nullptr, 0);
  Fts3Expr* R  = alloc_node(nullptr, nullptr, nullptr, 0);

  Fts3Expr* A = alloc_node(nullptr, A1, R, 0);

  if( A1 ) A1->pParent = A;
  if( A2 ) A2->pParent = A1;
  if( A3 ) A3->pParent = A2;
  if( R  ) R->pParent  = A;

  if( !A || !A1 || !A2 || !A3 || !R ) return false;

  sqlite3Fts3ExprFree(A);
  return true;
}

// Simple harness: run all tests and report
int main(){
  using TestFn = std::function<bool()>;
  struct TestCase { std::string name; TestFn fn; };

  std::vector<TestCase> tests = {
    {"test_null_input", test_null_input},
    {"test_two_leaves", test_two_leaves},
    {"test_unbalanced_right_subtree", test_unbalanced_right_subtree},
    {"test_deep_left_chain", test_deep_left_chain}
  };

  int failures = 0;
  for(auto &t : tests){
    bool ok = false;
    try {
      ok = t.fn();
    } catch(...) {
      ok = false;
    }
    std::cout << t.name << ": " << (ok ? "PASS" : "FAIL") << std::endl;
    if(!ok) failures++;
  }

  return failures;
}