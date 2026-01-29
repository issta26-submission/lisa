/*
  Lightweight C++11 unit tests for the focal method:
    int sqlite3ExprIdToTrueFalse(Expr *pExpr)
  The tests implement a minimal, self-contained mock of the surrounding
  dependencies (Expr structure and helper functions) to exercise the
  core branching logic of the focal method without requiring the full
  SQLite codebase.

  Notes:
  - This is a self-contained test harness with minimal stubs to model the
    environment needed by sqlite3ExprIdToTrueFalse.
  - Tests cover true-branch and false-branch scenarios, plus a branch
    where an "EP_Quoted" (simulated) flag prevents transformation.
  - We avoid terminating assertions by using a small, non-throwing test
    checker that reports pass/fail and continues executing other tests.
  - The implementation of sqlite3IsTrueOrFalse is tailored for the test
    to deterministically exercise branches (case-insensitive match for
    "TRUE"/"FALSE").
*/

#include <cctype>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>
#include <cstdint>


// Domain-specific constants (minimal subset to mirror focal method)
using u32 = uint32_t;

static const int TK_ID = 1;
static const int TK_STRING = 2;
static const int TK_TRUEFALSE = 3;

// Property flags (subset for test purposes)
static const unsigned int EP_Quoted  = 0x01;
static const unsigned int EP_IntValue = 0x02;

// Minimal Expr structure mirroring needed fields for the focal method
struct Expr {
    int op;               // Operation type (TK_ID, TK_STRING, etc.)
    struct { const char* zToken; } u; // Token string used by sqlite3IsTrueOrFalse
    unsigned int flags;     // Simulated property flags (EP_Quoted, EP_IntValue, etc.)
    unsigned int property;    // Simulated storage for property set by ExprSetProperty
};

// Forward declarations (simplified stubs for the test harness)
static int ExprHasProperty(const Expr* pExpr, unsigned int mask);
static void ExprSetProperty(Expr* pExpr, unsigned int v);
static u32 sqlite3IsTrueOrFalse(const char* zIn);

// Minimal implementation of ExprHasProperty used by focal method
static int ExprHasProperty(const Expr* pExpr, unsigned int mask){
    return (pExpr->flags & mask) != 0;
}

// Minimal implementation of ExprSetProperty used by focal method
static void ExprSetProperty(Expr* pExpr, unsigned int v){
    pExpr->property = v;
}

// Deterministic, case-insensitive check for "TRUE"/"FALSE" strings
static bool iequals(const char* a, const char* b){
    if(a==nullptr || b==nullptr) return false;
    while(*a && *b){
        if(static_cast<unsigned char>(std::tolower(*a)) != static_cast<unsigned char>(std::tolower(*b)))
            return false;
        ++a; ++b;
    }
    return (*a == '\0' && *b == '\0');
}

// Simplified sqlite3IsTrueOrFalse matching: non-zero for "TRUE" or "FALSE"
static u32 sqlite3IsTrueOrFalse(const char* zIn){
    if(zIn==nullptr) return 0;
    if(iequals(zIn, "TRUE") || iequals(zIn, "FALSE")) return 1;
    return 0;
}

// Focal method under test (reproduced here in a self-contained form for the harness)
int sqlite3ExprIdToTrueFalse(Expr *pExpr){
  u32 v;
  // Ensure op is either ID or STRING (as per original code)
  // In test harness, we rely on this being true to proceed
  // We simulate the assertion by making sure tests only call with valid ops
  if( !(pExpr->op==TK_ID || pExpr->op==TK_STRING) ){
      // In real code this would assert; here we guard to avoid undefined behavior
      return 0;
  }
  if( !ExprHasProperty(pExpr, EP_Quoted|EP_IntValue)
   && (v = sqlite3IsTrueOrFalse(pExpr->u.zToken))!=0
  ){
    pExpr->op = TK_TRUEFALSE;
    ExprSetProperty(pExpr, v);
    return 1;
  }
  return 0;
}

/***********************
 * Test Harness Helpers
 ***********************/

static bool test_passed = true;

// Simple assertion helper that reports and continues
static void check(bool cond, const std::string& msg){
    if(cond){
        std::cout << "[PASS] " << msg << "\n";
    }else{
        std::cerr << "[FAIL] " << msg << "\n";
        test_passed = false;
    }
}

/***********************
 * Unit Tests
 **********************/

// Test 1: True-branch path when token is "TRUE" (case-insensitive)
static bool test_trueBranch_WithTrueToken(){
    // Arrange
    Expr p;
    p.op = TK_ID;
    p.u.zToken = "TRUE"; // should trigger transformation
    p.flags = 0;          // no EP_Quoted or EP_IntValue
    p.property = 0;

    // Act
    int ret = sqlite3ExprIdToTrueFalse(&p);

    // Assert
    check(ret == 1, "sqlite3ExprIdToTrueFalse returns 1 on TRUE token with no EP flags");
    check(p.op == TK_TRUEFALSE, "Operator updated to TK_TRUEFALSE on true/false token");
    check(p.property == 1, "Property set to non-zero value (1) on true/false token");
    return test_passed;
}

// Test 2: False-branch path when token is not a true/false token
static bool test_falseBranch_WithRandomToken(){
    // Arrange
    Expr p;
    p.op = TK_STRING;
    p.u.zToken = "XYZ"; // not a true/false token
    p.flags = 0;
    p.property = 0;

    // Act
    int ret = sqlite3ExprIdToTrueFalse(&p);

    // Assert
    check(ret == 0, "sqlite3ExprIdToTrueFalse returns 0 on non-true/false token");
    check(p.op == TK_STRING, "Operator remains unchanged on non-true/false token");
    check(p.property == 0, "Property remains unchanged on non-true/false token");
    return test_passed;
}

// Test 3: Early exit path when EP_Quoted flag is set (no transformation)
static bool test_noTransform_WhenQuotedFlagPresent(){
    // Arrange
    Expr p;
    p.op = TK_STRING;
    p.u.zToken = "TRUE";
    p.flags = EP_Quoted; // simulate quoted token prevents transformation
    p.property = 0;

    // Act
    int ret = sqlite3ExprIdToTrueFalse(&p);

    // Assert
    check(ret == 0, "sqlite3ExprIdToTrueFalse returns 0 when EP_Quoted is set");
    check(p.op == TK_STRING, "Operator remains unchanged when EP_Quoted is set");
    check(p.property == 0, "Property unchanged when EP_Quoted is set");
    return test_passed;
}

// Test 4: True-branch with lowercase token to ensure case-insensitivity
static bool test_trueBranch_LowercaseToken(){
    // Arrange
    Expr p;
    p.op = TK_ID;
    p.u.zToken = "true"; // lowercase should still be recognized
    p.flags = 0;
    p.property = 0;

    // Act
    int ret = sqlite3ExprIdToTrueFalse(&p);

    // Assert
    check(ret == 1, "sqlite3ExprIdToTrueFalse returns 1 on lowercase 'true' token");
    check(p.op == TK_TRUEFALSE, "Operator updated to TK_TRUEFALSE for lowercase token");
    check(p.property == 1, "Property set to non-zero value (1) for lowercase token");
    return test_passed;
}

/***********************
 * Main: run all tests
 ***********************/
int main(){
    std::cout << "Starting unit tests for sqlite3ExprIdToTrueFalse (self-contained harness)\n";

    test_passed = true;
    test_trueBranch_WithTrueToken();
    test_falseBranch_WithRandomToken();
    test_noTransform_WhenQuotedFlagPresent();
    test_trueBranch_LowercaseToken();

    if(test_passed){
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }else{
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}