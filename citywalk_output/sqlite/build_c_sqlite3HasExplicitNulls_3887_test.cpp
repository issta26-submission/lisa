// C++11 test suite for the focal method: sqlite3HasExplicitNulls
// This test suite provides a minimal, self-contained environment to exercise
// the behavior of sqlite3HasExplicitNulls without requiring GTest.
// It mocks the dependent types (Parse, ExprList) and the sqlite3ErrorMsg
// function to verify both return values and error message content.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Domain: Lightweight C-style scaffolding to support testing of sqlite3HasExplicitNulls

typedef unsigned char u8;

// Forward declare the Parse type (opaque to the test).
struct Parse;

// Minimal Expr and ExprList structures to mirror usage in sqlite3HasExplicitNulls
struct Expr {
  struct {
    int bNulls;     // indicates explicit NULLs usage in this expression
    u8 sortFlags;   // sort order flags, determines FIRST/LAST
  } fg;
};

struct ExprList {
  int nExpr; // number of expressions
  Expr* a;   // array of expressions
};

// Global to capture the last error message produced by sqlite3ErrorMsg
static std::string g_lastErrorMessage;

// Mocked sqlite3ErrorMsg to capture error messages for assertions.
// Signature matches common sqlite3ErrorMsg usage (variadic).
extern "C" void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, ...) {
  (void)pParse; // Not used by tests, but kept to mirror original API
  char zBuf[1024];
  va_list ap;
  va_start(ap, zFormat);
  vsnprintf(zBuf, sizeof(zBuf), zFormat, ap);
  va_end(ap);
  g_lastErrorMessage = zBuf;
}

// Forward declaration of the focal method under test (reproduced here for testability).
extern "C" int sqlite3HasExplicitNulls(Parse* pParse, ExprList* pList);

// Implementation of the focal method (copied from the provided snippet)
extern "C" int sqlite3HasExplicitNulls(Parse* pParse, ExprList* pList){
  if( pList ){
    int i;
    for(i=0; i<pList->nExpr; i++){
      if( pList->a[i].fg.bNulls ){
        u8 sf = pList->a[i].fg.sortFlags;
        sqlite3ErrorMsg(pParse, "unsupported use of NULLS %s",
            (sf==0 || sf==3) ? "FIRST" : "LAST"
        );
        return 1;
      }
    }
  }
  return 0;
}

// Simple non-terminating test harness (no GTest, no GMock)
// Keeps a log of failed assertions and continues execution to maximize coverage.
static std::vector<std::string> g_failures;

// Helper to reset error capture between tests
static void resetErrorCapture() {
  g_lastErrorMessage.clear();
}

// Helper to record a failure message
static void recordFailure(const std::string& msg) {
  g_failures.push_back(msg);
}

// Helper to compare integers and record a descriptive failure when mismatched
static void expectIntEq(const std::string& testName, int expected, int actual) {
  if (expected != actual) {
    recordFailure("[" + testName + "] Expected int " + std::to_string(expected) +
                  ", got " + std::to_string(actual));
  }
}

// Helper to assert that the captured error message contains a substring
static void expectErrorContains(const std::string& testName, const std::string& substring) {
  if (g_lastErrorMessage.find(substring) == std::string::npos) {
    recordFailure("[" + testName + "] Error message does not contain expected substring: " + substring +
                  " (actual: \"" + g_lastErrorMessage + "\")");
  }
}

// Test 1: Null pList should yield 0 (no NULLS usage checked)
static void test_null_pList_returns_zero() {
  // Test case description:
  // When pList is null, sqlite3HasExplicitNulls should simply return 0.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList* pList = nullptr;
  int res = sqlite3HasExplicitNulls(pParse, pList);
  expectIntEq("test_null_pList_returns_zero", 0, res);
  if (!g_failures.empty()) return;
}

// Test 2: Empty ExprList (nExpr == 0) should yield 0
static void test_empty_exprList_returns_zero() {
  // Test case description:
  // If there are no expressions, there is no NULLS usage, so return 0.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList list;
  list.nExpr = 0;
  list.a = nullptr;
  int res = sqlite3HasExplicitNulls(pParse, &list);
  expectIntEq("test_empty_exprList_returns_zero", 0, res);
  if (!g_failures.empty()) return;
}

// Test 3: Single expression with bNulls = true and sf = 0 (FIRST)
static void test_single_null_sf0_first() {
  // Test case description:
  // A single expression with explicit NULLS and FIRST sortFlags should trigger error
  // and indicate FIRST in the message.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList list;
  Expr item;
  item.fg.bNulls = 1;
  item.fg.sortFlags = 0; // FIRST according to sf==0 -> FIRST
  list.nExpr = 1;
  list.a = &item;

  int res = sqlite3HasExplicitNulls(pParse, &list);
  expectIntEq("test_single_null_sf0_first", 1, res);
  expectErrorContains("test_single_null_sf0_first", "FIRST");
  if (!g_failures.empty()) return;
}

// Test 4: Single expression with bNulls = true and sf = 3 (FIRST)
static void test_single_null_sf3_first() {
  // Test case description:
  // sf = 3 should also result in FIRST.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList list;
  Expr item;
  item.fg.bNulls = 1;
  item.fg.sortFlags = 3; // 3 -> FIRST
  list.nExpr = 1;
  list.a = &item;

  int res = sqlite3HasExplicitNulls(pParse, &list);
  expectIntEq("test_single_null_sf3_first", 1, res);
  expectErrorContains("test_single_null_sf3_first", "FIRST");
  if (!g_failures.empty()) return;
}

// Test 5: Single expression with bNulls = true and sf = 1 (LAST)
static void test_single_null_sf1_last() {
  // Test case description:
  // sf = 1 should result in LAST.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList list;
  Expr item;
  item.fg.bNulls = 1;
  item.fg.sortFlags = 1; // LAST
  list.nExpr = 1;
  list.a = &item;

  int res = sqlite3HasExplicitNulls(pParse, &list);
  expectIntEq("test_single_null_sf1_last", 1, res);
  expectErrorContains("test_single_null_sf1_last", "LAST");
  if (!g_failures.empty()) return;
}

// Test 6: Multiple expressions; first non-nulls, second nulls with sf = 2 (LAST)
static void test_multiple_expressions_second_null_sf2_last() {
  // Test case description:
  // Ensure the function reports LAST when the first nulls occurrence has sf = 2.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList list;
  Expr e1, e2;
  e1.fg.bNulls = 0;    // not NULLs
  e1.fg.sortFlags = 0;  // irrelevant here
  e2.fg.bNulls = 1;    // NULLs present
  e2.fg.sortFlags = 2;  // LAST
  Expr items[2] = { e1, e2 };
  list.nExpr = 2;
  list.a = items;

  int res = sqlite3HasExplicitNulls(pParse, &list);
  expectIntEq("test_multiple_expressions_second_null_sf2_last", 1, res);
  expectErrorContains("test_multiple_expressions_second_null_sf2_last", "LAST");
  if (!g_failures.empty()) return;
}

// Test 7: Multiple expressions; first nulls with sf = 1 (LAST) -> ensure first hit is used
static void test_first_null_sf1_last_preferred() {
  // Test case description:
  // When the first expression has bNulls true with sf=1, LAST should be reported.
  resetErrorCapture();
  Parse* pParse = nullptr;
  ExprList list;
  Expr e1, e2;
  e1.fg.bNulls = 1;
  e1.fg.sortFlags = 1; // LAST
  e2.fg.bNulls = 0;
  e2.fg.sortFlags = 0;
  Expr items[2] = { e1, e2 };
  list.nExpr = 2;
  list.a = items;

  int res = sqlite3HasExplicitNulls(pParse, &list);
  expectIntEq("test_first_null_sf1_last_preferred", 1, res);
  expectErrorContains("test_first_null_sf1_last_preferred", "LAST");
  if (!g_failures.empty()) return;
}

// Runner: execute all tests and report results
int main() {
  // Execute tests
  test_null_pList_returns_zero();
  test_empty_exprList_returns_zero();
  test_single_null_sf0_first();
  test_single_null_sf3_first();
  test_single_null_sf1_last();
  test_multiple_expressions_second_null_sf2_last();
  test_first_null_sf1_last_preferred();

  // Report
  if (g_failures.empty()) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << g_failures.size() << " TEST(S) FAILED:\n";
    for (const auto& f : g_failures) {
      std::cout << f << "\n";
    }
    return 1;
  }
}