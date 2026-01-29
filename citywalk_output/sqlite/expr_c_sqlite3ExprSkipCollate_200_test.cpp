// Unit tests for sqlite3ExprSkipCollate
// Target: validate behavior of Expr *sqlite3ExprSkipCollate(Expr *pExpr)
// Scope: use internal SQLite structures (Expr) via sqliteInt.h
// Test framework: simple C++11 test harness (no GTest), non-terminating style with clear logs

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Include internal SQLite declarations to construct Expr trees.
// This relies on the project being buildable with sqliteInt.h available.
extern "C" {
}

// Declaration of the function under test (C linkage)
extern "C" {
Expr *sqlite3ExprSkipCollate(Expr *pExpr);
}

// Lightweight test harness (non-terminating assertions style)
static bool test_passed = true;

// Test 1: Null input should return NULL
// This exercises the "while (pExpr && ...)" guard at the start.
bool test_null_input_returns_null() {
  // NULL input
  Expr *p = nullptr;
  Expr *res = sqlite3ExprSkipCollate(p);
  bool ok = (res == nullptr);

  std::cout << "[Test 1] null input returns null: "
            << (ok ? "PASS" : "FAIL") << "\n";
  if (!ok) {
    std::cout << "\tExpected: nullptr, Got: non-null\n";
  }
  return ok;
}

// Test 2: Input without EP_Skip should be returned as-is
// This covers the false branch of the while condition.
bool test_no_skip_returns_same_pointer() {
  Expr leaf;
  std::memset(&leaf, 0, sizeof(Expr));
  leaf.op = TK_COLLATE; // value doesn't matter since no EP_Skip is set
  leaf.pLeft = nullptr;
  leaf.flags = 0;        // EP_Skip not set

  Expr *res = sqlite3ExprSkipCollate(&leaf);
  bool ok = (res == &leaf);

  std::cout << "[Test 2] no EP_Skip -> returns same pointer: "
            << (ok ? "PASS" : "FAIL") << "\n";
  if (!ok) {
    std::cout << "\tExpected: pointer to input leaf, Got: different pointer\n";
  }
  return ok;
}

// Test 3: Two-level EP_Skip chain should skip to the left-most non-skip leaf
// Structure: top (TK_COLLATE, EP_Skip) -> left (non-skip leaf)
// Expectation: function returns pointer to the left leaf (non-skip)
bool test_two_level_skip_chain_returns_leaf() {
  Expr top, leftLeaf;
  std::memset(&top, 0, sizeof(Expr));
  std::memset(&leftLeaf, 0, sizeof(Expr));

  top.op = TK_COLLATE;
  top.pLeft = &leftLeaf;
  top.flags = EP_Skip;      // top has EP_Skip

  leftLeaf.op = 123;          // some arbitrary non-collation operator
  leftLeaf.pLeft = nullptr;
  leftLeaf.flags = 0;         // not EP_Skip

  Expr *res = sqlite3ExprSkipCollate(&top);
  bool ok = (res == &leftLeaf);

  std::cout << "[Test 3] two-level EP_Skip chain returns leaf: "
            << (ok ? "PASS" : "FAIL") << "\n";
  if (!ok) {
    std::cout << "\tExpected: pointer to leftLeaf, Got: different pointer\n";
  }
  return ok;
}

// Test 4: Three-level EP_Skip chain should skip to the left-most non-skip leaf
// Structure: top (TK_COLLATE, EP_Skip) -> mid (TK_COLLATE, EP_Skip) -> leaf (non-skip)
// Expectation: function returns pointer to the leaf
bool test_three_level_skip_chain_returns_leaf() {
  Expr top, mid, leaf;
  std::memset(&top, 0, sizeof(Expr));
  std::memset(&mid, 0, sizeof(Expr));
  std::memset(&leaf, 0, sizeof(Expr));

  top.op = TK_COLLATE;
  top.pLeft = &mid;
  top.flags = EP_Skip;

  mid.op = TK_COLLATE;
  mid.pLeft = &leaf;
  mid.flags = EP_Skip;

  leaf.op = 999;        // arbitrary non-collation operator
  leaf.pLeft = nullptr;
  leaf.flags = 0;         // not EP_Skip

  Expr *res = sqlite3ExprSkipCollate(&top);
  bool ok = (res == &leaf);

  std::cout << "[Test 4] three-level EP_Skip chain returns leaf: "
            << (ok ? "PASS" : "FAIL") << "\n";
  if (!ok) {
    std::cout << "\tExpected: pointer to leaf, Got: different pointer\n";
  }
  return ok;
}

// Helper to run all tests and summarize
int main() {
  bool all_passed = true;
  all_passed &= test_null_input_returns_null();
  all_passed &= test_no_skip_returns_same_pointer();
  all_passed &= test_two_level_skip_chain_returns_leaf();
  all_passed &= test_three_level_skip_chain_returns_leaf();

  std::cout << "\nTest suite " << (all_passed ? "PASSED" : "FAILED") << ".\n";
  return all_passed ? 0 : 1;
}