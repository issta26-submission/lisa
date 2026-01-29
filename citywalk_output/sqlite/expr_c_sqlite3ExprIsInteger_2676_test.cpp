// Self-contained C++11 test harness for sqlite3ExprIsInteger
// This test recreates a minimal environment sufficient to exercise the focal
// function's behavior without requiring the full SQLite sources.
// It follows the instructions to avoid external testing frameworks (no GTest).

#include <cerrno>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Minimal definitions to emulate the environment used by sqlite3ExprIsInteger
// These mimic the essential parts of SQLite's internal structures needed for
// this function's logic.

#define NEVER(X) (0 && (X))            // Ensures the branch is never taken in tests

// Op codes (subset required by sqlite3ExprIsInteger)
enum {
  TK_INTEGER = 1,
  TK_UPLUS   = 2,
  TK_UMINUS  = 3
};

// Integer expression flag
const int EP_IntValue = 1;

// Minimal Expr structure
struct Expr {
  int op;
  int flags;
  struct {
    int iValue;
    const char *zToken;
  } u;
  Expr *pLeft;
  Expr *pRight;
};

// Forward declaration for the helper that sqlite3ExprIsInteger relies upon
// In SQLite this is a static function in the same translation unit, but for our
// test harness we implement it here with C linkage-like behavior.
extern "C" {

// Lightweight implementation of sqlite3GetInt32 used by sqlite3ExprIsInteger.
// Returns 0 on success (value fits in 32-bit), non-zero otherwise.
int sqlite3GetInt32(const char *z, int *pOut) {
  if (z == nullptr) return 1;
  // Trim leading whitespace
  while (*z == ' ' || *z == '\t' || *z == '\n' || *z == '\r') z++;
  if (*z == '\0') return 1;

  char *end = nullptr;
  errno = 0;
  long v = std::strtol(z, &end, 10);
  if (end == z) return 1;           // no digits were parsed
  // If there are non-space characters after the number, treat as failure
  while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') end++;
  if (*end != '\0') return 1;
  // Check range for 32-bit signed int
  if (v < INT32_MIN || v > INT32_MAX) return 1;
  *pOut = static_cast<int>(v);
  return 0;
}

// The focal function under test: sqlite3ExprIsInteger
int sqlite3ExprIsInteger(const Expr *p, int *pValue){
  int rc = 0;
  if( NEVER(p==0) ) return 0;  /* Used to only happen following on OOM */
  /* If an expression is an integer literal that fits in a signed 32-bit
  ** integer, then the EP_IntValue flag will have already been set */
  assert( p->op!=TK_INTEGER || (p->flags & EP_IntValue)!=0
           || sqlite3GetInt32(p->u.zToken, &rc)==0 );
  if( p->flags & EP_IntValue ){
    *pValue = p->u.iValue;
    return 1;
  }
  switch( p->op ){
    case TK_UPLUS: {
      rc = sqlite3ExprIsInteger(p->pLeft, pValue);
      break;
    }
    case TK_UMINUS: {
      int v = 0;
      if( sqlite3ExprIsInteger(p->pLeft, &v) ){
        assert( ((unsigned int)v)!=0x80000000 );
        *pValue = -v;
        rc = 1;
      }
      break;
    }
    default: break;
  }
  return rc;
}
} // extern "C"

// Simple test framework (non-terminating assertions)
static int g_failures = 0;

static void reportFailure(const std::string& testName, const std::string& detail) {
  std::cerr << "FAIL: " << testName << " - " << detail << "\n";
  ++g_failures;
}

// Helpers to run individual test cases

// Test 1: EP_IntValue directly on the expression should return its value
bool test_exprIsInteger_EP_IntValue() {
  // Build: p.flags has EP_IntValue, p.u.iValue = 123
  Expr p;
  p.op = 0;
  p.flags = EP_IntValue;
  p.u.iValue = 123;
  p.pLeft = nullptr;
  int value = 0;
  int rc = sqlite3ExprIsInteger(&p, &value);
  if (rc != 1 || value != 123) {
    reportFailure("test_exprIsInteger_EP_IntValue",
                  "expected rc=1 and value=123");
    return false;
  }
  return true;
}

// Test 2: TK_UPLUS should propagate integer-ness from its left child
bool test_exprIsInteger_UNARY_PLUS() {
  // Left child: EP_IntValue with iValue = 77
  Expr left;
  left.op = 0;
  left.flags = EP_IntValue;
  left.u.iValue = 77;
  left.pLeft = nullptr;

  // Parent: op = TK_UPLUS, left = &left
  Expr p;
  p.op = TK_UPLUS;
  p.flags = 0;
  p.u.iValue = 0;
  p.pLeft = &left;
  int value = -1;
  int rc = sqlite3ExprIsInteger(&p, &value);
  if (rc != 1 || value != 77) {
    reportFailure("test_exprIsInteger_UNARY_PLUS",
                  "expected rc=1 and value=77 from left IntValue");
    return false;
  }
  return true;
}

// Test 3: TK_UMINUS should negate the integer value from its left child
bool test_exprIsInteger_UNARY_MINUS() {
  // Left child: EP_IntValue with iValue = 5
  Expr left;
  left.op = 0;
  left.flags = EP_IntValue;
  left.u.iValue = 5;
  left.pLeft = nullptr;

  // Parent: op = TK_UMINUS, left = &left
  Expr p;
  p.op = TK_UMINUS;
  p.flags = 0;
  p.u.iValue = 0;
  p.pLeft = &left;

  int value = 0;
  int rc = sqlite3ExprIsInteger(&p, &value);
  if (rc != 1 || value != -5) {
    reportFailure("test_exprIsInteger_UNARY_MINUS",
                  "expected rc=1 and value=-5 from left IntValue");
    return false;
  }
  return true;
}

// Test 4: Non-integer expression should yield rc = 0 (no integer)
bool test_exprIsInteger_NonIntegerNoValue() {
  // Parent op not special (e.g., 999)
  Expr p;
  p.op = 999; // not TK_UPLUS or TK_UMINUS
  p.flags = 0;
  p.u.iValue = 0;
  p.pLeft = nullptr;

  int value = 42;
  int rc = sqlite3ExprIsInteger(&p, &value);
  if (rc != 0 || value != 42) {
    reportFailure("test_exprIsInteger_NonIntegerNoValue",
                  "expected rc=0 and value unchanged");
    return false;
  }
  return true;
}

// Test 5: TK_INTEGER with numeric zToken should not set pValue unless EP_IntValue is set
// and should return rc=0 (since the switch has no matching case)
bool test_exprIsInteger_TK_INTEGER_withToken() {
  // op = TK_INTEGER, token "456"
  Expr p;
  p.op = TK_INTEGER;
  p.flags = 0; // EP_IntValue not set
  p.u.zToken = "456";
  p.pLeft = nullptr;

  int value = 7; // should be left unchanged
  int rc = sqlite3ExprIsInteger(&p, &value);
  if (rc != 0 || value != 7) {
    reportFailure("test_exprIsInteger_TK_INTEGER_withToken",
                  "expected rc=0 and value unchanged; token parsed but not treated as int");
    return false;
  }
  return true;
}

// Main driver
int main() {
  std::cout << "Running sqlite3ExprIsInteger unit tests (self-contained)\n";

  bool all_passed = true;
  if (!test_exprIsInteger_EP_IntValue()) all_passed = false;
  if (!test_exprIsInteger_UNARY_PLUS()) all_passed = false;
  if (!test_exprIsInteger_UNARY_MINUS()) all_passed = false;
  if (!test_exprIsInteger_NonIntegerNoValue()) all_passed = false;
  if (!test_exprIsInteger_TK_INTEGER_withToken()) all_passed = false;

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << "TOTAL FAILURES: " << g_failures << "\n";
  }

  // Return non-zero if any test failed
  return g_failures ? 1 : 0;
}