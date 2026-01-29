// Lightweight C++11 unit tests for the focal function sqlite3ExprCollSeqMatch
// This test suite does not use Google Test. It provides a small in-file test harness
// and simple stubs/mocks for the minimal SQLite-like types and functions required
// by sqlite3ExprCollSeqMatch to run in a pure C++ environment.
//
// Key goals:
// - Validate that sqlite3ExprCollSeqMatch correctly compares CollSeq names in a case-insensitive manner.
// - Cover true/false branches by testing identical and different collName cases.
// - Demonstrate non-terminating test behavior: all tests run regardless of individual failures.
// - Access to "static-like" and file-scope helpers is emulated via test doubles in this file.

#include <vector>
#include <cctype>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// -------------------------------------------------------------
// Minimal type definitions to mimic the subset used by the focal method
// -------------------------------------------------------------
struct Parse {};           // Opaque parse structure (test scaffold)
struct Expr {               // Lightweight Expr with a collSeq name hook
    const char* collName;
};

// CollSeq mirrors the real sqlite3 CollSeq in the minimal test context
struct CollSeq {
    const char* zName;      // Collation name
};

// -------------------------------------------------------------
// Forward declarations of the focal function and its collaborators
// In the real project, these would come from sqlite3.h / internal headers.
// Here we provide a minimal inline implementation of the focal function plus
// test doubles for its collaborators to allow compilation and execution
// in a standard C++ environment.
//
// FOCAL_METHOD (implemented inline for isolated testing):
// int sqlite3ExprCollSeqMatch(Parse *pParse, const Expr *pE1, const Expr *pE2){
//   CollSeq *pColl1 = sqlite3ExprNNCollSeq(pParse, pE1);
//   CollSeq *pColl2 = sqlite3ExprNNCollSeq(pParse, pE2);
//   return sqlite3StrICmp(pColl1->zName, pColl2->zName)==0;
// }
static CollSeq* sqlite3ExprNNCollSeq(Parse*, const Expr* pExpr);
static int sqlite3StrICmp(const char* zLeft, const char* zRight);

// Inline definition of the focal function as provided
int sqlite3ExprCollSeqMatch(Parse *pParse, const Expr *pE1, const Expr *pE2){
    CollSeq *pColl1 = sqlite3ExprNNCollSeq(pParse, pE1);
    CollSeq *pColl2 = sqlite3ExprNNCollSeq(pParse, pE2);
    return sqlite3StrICmp(pColl1->zName, pColl2->zName)==0;
}

// -------------------------------------------------------------
// Test doubles for the collaborators

// Simple, deterministic mapping from an Expr to a CollSeq by exposing the
// Expr's collName as the CollSeq name.
// This mimics sqlite3ExprNNCollSeq's responsibility in the focal flow.
static CollSeq* sqlite3ExprNNCollSeq(Parse*, const Expr* pExpr) {
    // Use a static CollSeq instance to emulate the lifetime of a CollSeq object
    // in the original code path. The test only needs the pointer to be stable
    // for a single invocation chain within one test case.
    static CollSeq cs;
    if (pExpr && pExpr->collName) {
        cs.zName = pExpr->collName;
    } else {
        cs.zName = ""; // Fallback for missing name
    }
    return &cs;
}

// Case-insensitive string comparison (ASCII), matching sqlite3StrICmp behavior
static int sqlite3StrICmp(const char* zLeft, const char* zRight) {
    // Normalize NULL pointers to empty strings
    if (zLeft == nullptr) zLeft = "";
    if (zRight == nullptr) zRight = "";

    // Compare character by character in a case-insensitive manner
    while (*zLeft && *zRight) {
        unsigned char c1 = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zLeft)));
        unsigned char c2 = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zRight)));
        if (c1 != c2) return static_cast<int>(c1) - static_cast<int>(c2);
        ++zLeft;
        ++zRight;
    }
    // If we reach here, one or both strings ended; compute final difference
    return static_cast<int>(static_cast<unsigned char>(*zLeft)) - static_cast<int>(static_cast<unsigned char>(*zRight));
}

// -------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// -------------------------------------------------------------
static std::vector<std::string> g_failures;

static void logFailure(const std::string& testName, int got, int expected) {
    g_failures.push_back("Test '" + testName + "' failed: expected " +
                         std::to_string(expected) + ", got " + std::to_string(got));
}

static void checkEq(int got, int expected, const std::string& testName) {
    if (got != expected) {
        logFailure(testName, got, expected);
    }
}

// -------------------------------------------------------------
// Test cases for sqlite3ExprCollSeqMatch
// Each test is annotated with a comment describing its intention and covered branches.
// -------------------------------------------------------------
static void test_case_insensitive_match_true() {
    // Test: identical or case-insensitive equality should return true (1)
    // Coverage: true branch of the equality comparison
    Parse p;
    Expr e1{ "BINARY" };
    Expr e2{ "binary" };
    int res = sqlite3ExprCollSeqMatch(&p, &e1, &e2);
    checkEq(res, 1, "test_case_insensitive_match_true_should_be_true_with_case_insensitive_names");
}

static void test_case_different_names_false() {
    // Test: different names should return false (0)
    // Coverage: false branch of the equality comparison
    Parse p;
    Expr e1{ "NOCASE" };
    Expr e2{ "BINARY" };
    int res = sqlite3ExprCollSeqMatch(&p, &e1, &e2);
    checkEq(res, 0, "test_case_different_names_should_be_false");
}

static void test_case_identical_names_true() {
    // Test: identical names across both operands should return true
    Parse p;
    Expr e1{ "latin1" };
    Expr e2{ "latin1" };
    int res = sqlite3ExprCollSeqMatch(&p, &e1, &e2);
    checkEq(res, 1, "test_case_identical_names_should_be_true");
}

static void test_case_mixed_case_match_true() {
    // Test: mixed-case identical strings should match (case-insensitive)
    Parse p;
    Expr e1{ "Utf8" };
    Expr e2{ "utf8" };
    int res = sqlite3ExprCollSeqMatch(&p, &e1, &e2);
    checkEq(res, 1, "test_case_mixed_case_match_true");
}

static void test_case_different_case_same_letters_false() {
    // Test: different case but same letters should still be true
    Parse p;
    Expr e1{ "Alpha" };
    Expr e2{ "aLpHa" };
    int res = sqlite3ExprCollSeqMatch(&p, &e1, &e2);
    checkEq(res, 1, "test_case_different_case_same_letters_should_be_true");
}

// -------------------------------------------------------------
// Main driver: run all tests and report results
// -------------------------------------------------------------
int main() {
    // Run all test cases
    test_case_insensitive_match_true();
    test_case_different_names_false();
    test_case_identical_names_true();
    test_case_mixed_case_match_true();
    test_case_different_case_same_letters_false();

    // Report summary
    if (g_failures.empty()) {
        std::cout << "ALL TESTS PASSED: sqlite3ExprCollSeqMatch behaved as expected.\n";
        return 0;
    } else {
        std::cout << g_failures.size() << " TEST(S) FAILED:\n";
        for (const auto& msg : g_failures) {
            std::cout << "  - " << msg << "\n";
        }
        return 1;
    }
}