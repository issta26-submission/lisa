// A self-contained C++11 test suite for the sqlite3ExprAlloc function.
// The test environment provides minimal stand-ins for the SQLite types and
// helpers used by sqlite3ExprAlloc. This file does not rely on GTest and uses
// a simple in-file test harness with descriptive comments for each case.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Lightweight stand-ins and helpers to emulate the SQLite environment.

typedef unsigned char u8;

struct sqlite3 { int dummy; }; // dummy database handle for tests

// Token structure used by sqlite3ExprAlloc
struct Token {
  const char *z;  // token text
  int n;          // length of token text
};

// Opcodes (simplified)
#define TK_INTEGER 1

// Expression flags (subset)
#define EP_IntValue 0x01
#define EP_Leaf     0x02
#define EP_IsTrue   0x04
#define EP_IsFalse  0x08

// Expression depth setting for tests
#define SQLITE_MAX_EXPR_DEPTH 1

// The core Expr structure used by sqlite3ExprAlloc (simplified)
struct Expr {
  u8 op;
  int iAgg;
  unsigned int flags;
  union {
    int iValue;
    char *zToken;
  } u;
#if SQLITE_MAX_EXPR_DEPTH>0
  int nHeight;
#endif
};

// --- Minimal SQLite-like helpers ---

// malloc wrapper (test-only)
static void* sqlite3DbMallocRawNN(sqlite3*, size_t n) {
  return std::malloc(n);
}

// Simple int parser for token text (non-negative integers only for test)
static int sqlite3GetInt32(const char *z, int *pVal) {
  if (!z) return 0;
  char *end = nullptr;
  long v = std::strtol(z, &end, 10);
  if (end == z || *end != '\0') return 0;
  *pVal = (int)v;
  return 1;
}

// Detect plain quote characters
static int sqlite3Isquote(char c) {
  return c=='\'' || c=='"';
}

// Dequote: remove surrounding quotes if present (naive implementation)
static void sqlite3DequoteExpr(Expr *p) {
  if(!p) return;
  char *s = p->u.zToken;
  if(!s) return;
  size_t len = std::strlen(s);
  if (len >= 2) {
    char f = s[0];
    char l = s[len-1];
    if ((f=='\'' && l=='\'') || (f=='"' && l=='"')) {
      // remove surrounding quotes
      memmove(s, s+1, len-2);
      s[len-2] = '\0';
    }
  }
}

// --- Focal method under test: sqlite3ExprAlloc (embedded in test) ---

Expr *sqlite3ExprAlloc(
  sqlite3 *db,            /* Handle for sqlite3DbMallocRawNN() */
  int op,                 /* Expression opcode */
  const Token *pToken,    /* Token argument.  Might be NULL */
  int dequote             /* True to dequote */
){
  Expr *pNew;
  int nExtra = 0;
  int iValue = 0;
  assert( db!=0 );
  if( pToken ){
    if( op!=TK_INTEGER || pToken->z==0
          || sqlite3GetInt32(pToken->z, &iValue)==0 ){
      nExtra = pToken->n+1;
      assert( iValue>=0 );
    }
  }
  pNew = (Expr*)sqlite3DbMallocRawNN(db, sizeof(Expr)+nExtra);
  if( pNew ){
    std::memset(pNew, 0, sizeof(Expr));
    pNew->op = (u8)op;
    pNew->iAgg = -1;
    if( pToken ){
      if( nExtra==0 ){
        pNew->flags |= EP_IntValue|EP_Leaf|(iValue?EP_IsTrue:EP_IsFalse);
        pNew->u.iValue = iValue;
      }else{
        pNew->u.zToken = (char*)&pNew[1];
        assert( pToken->z!=0 || pToken->n==0 );
        if( pToken->n ) std::memcpy(pNew->u.zToken, pToken->z, pToken->n);
        pNew->u.zToken[pToken->n] = 0;
        if( dequote && sqlite3Isquote(pNew->u.zToken[0]) ){
          sqlite3DequoteExpr(pNew);
        }
      }
    }
#if SQLITE_MAX_EXPR_DEPTH>0
    pNew->nHeight = 1;
#endif
  }
  return pNew;
}

// --- End of focal method and helpers ---

// --- Lightweight test harness ---

static int g_tests_run = 0;
static int g_tests_passed = 0;

static void report_test(const char* name, bool passed) {
  std::cout << "[TEST] " << name << " - " << (passed ? "PASS" : "FAIL") << std::endl;
  ++g_tests_run;
  if (passed) ++g_tests_passed;
}

// Test 1: NULL token should allocate a simple leaf with no extra data
static bool test_expr_alloc_null_token() {
  sqlite3 db;
  Expr *p = sqlite3ExprAlloc(&db, 42, nullptr, 0);
  bool ok = (p != nullptr)
            && p->op == (u8)42
            && p->iAgg == -1
            && p->flags == 0
            && p->u.iValue == 0
            && p->u.zToken == nullptr;
  if (p) std::free(p);
  return ok;
}

// Test 2: Non-integer op with a token should allocate extra space and copy token text
static bool test_expr_alloc_non_integer_token_copies_text() {
  sqlite3 db;
  Token t;
  t.z = "xyz";
  t.n = 3;
  Expr *p = sqlite3ExprAlloc(&db, 5, &t, 0); // op != TK_INTEGER
  bool ok = (p != nullptr)
            && p->u.zToken != nullptr
            && std::strncmp(p->u.zToken, "xyz", 3) == 0
            && p->u.zToken[3] == '\0'
            && p->flags == 0;
  if (p) std::free(p);
  return ok;
}

// Test 3: TK_INTEGER with numeric token should set iValue and appropriate flags
static bool test_expr_alloc_integer_token_numeric_value() {
  sqlite3 db;
  Token t;
  t.z = "5";
  t.n = 1;
  Expr *p = sqlite3ExprAlloc(&db, TK_INTEGER, &t, 0);
  bool ok = (p != nullptr)
            && p->u.iValue == 5
            && (p->flags & EP_IntValue) != 0
            && (p->flags & EP_Leaf) != 0
            && (p->flags & EP_IsTrue) != 0
            && p->u.zToken == nullptr;
  if (p) std::free(p);
  return ok;
}

// Test 4: TK_INTEGER with numeric token "0" should set IsFalse flag
static bool test_expr_alloc_integer_token_zero_value() {
  sqlite3 db;
  Token t;
  t.z = "0";
  t.n = 1;
  Expr *p = sqlite3ExprAlloc(&db, TK_INTEGER, &t, 0);
  bool ok = (p != nullptr)
            && p->u.iValue == 0
            && (p->flags & EP_IntValue) != 0
            && (p->flags & EP_Leaf) != 0
            && (p->flags & EP_IsFalse) != 0
            && p->u.zToken == nullptr;
  if (p) std::free(p);
  return ok;
}

// Test 5: TK_INTEGER with non-numeric token should allocate extra and copy text
static bool test_expr_alloc_integer_token_non_numeric_text() {
  sqlite3 db;
  Token t;
  t.z = "abc";
  t.n = 3;
  Expr *p = sqlite3ExprAlloc(&db, TK_INTEGER, &t, 0);
  bool ok = (p != nullptr)
            && p->u.zToken != nullptr
            && std::strcmp(p->u.zToken, "abc") == 0
            && p->flags == 0;
  if (p) std::free(p);
  return ok;
}

// Test 6: Dequoting behavior when dequote is true and token starts with a quote
static bool test_expr_alloc_dequote_applied() {
  sqlite3 db;
  Token t;
  t.z = "\"hello\"";
  t.n = 7;
  Expr *p = sqlite3ExprAlloc(&db, 999, &t, 1); // non-integer op to trigger dequote path
  bool ok = (p != nullptr)
            && p->u.zToken != nullptr
            && std::strcmp(p->u.zToken, "hello") == 0;
  if (p) std::free(p);
  return ok;
}

// Test 7: Depth height hint is set when enabled
static bool test_expr_alloc_height_hint_set() {
  sqlite3 db;
  Token t;
  t.z = "7";
  t.n = 1;
  Expr *p = sqlite3ExprAlloc(&db, 2, &t, 0);
  bool ok = (p != nullptr);
#if SQLITE_MAX_EXPR_DEPTH>0
  ok = ok && (p->nHeight == 1);
#else
  (void)ok; // no height field
#endif
  if (p) std::free(p);
  return ok;
}

int main() {
  // Run all tests and print a final summary.
  test_expr_alloc_null_token();
  report_test("ExprAlloc_NullToken", test_expr_alloc_null_token());

  report_test("ExprAlloc_NonIntegerTokenCopiesText", test_expr_alloc_non_integer_token_copies_text());

  report_test("ExprAlloc_IntegerTokenNumericValue", test_expr_alloc_integer_token_numeric_value());

  report_test("ExprAlloc_IntegerTokenZeroValue", test_expr_alloc_integer_token_zero_value());

  report_test("ExprAlloc_IntegerTokenNonNumericText", test_expr_alloc_integer_token_non_numeric_text());

  report_test("ExprAlloc_DequoteApplied", test_expr_alloc_dequote_applied());

  report_test("ExprAlloc_HeightHintSet", test_expr_alloc_height_hint_set());

  std::cout << "Summary: " << g_tests_passed << " / " << g_tests_run << " tests passed." << std::endl;
  return (g_tests_run == g_tests_passed) ? 0 : 1;
}