// Unit test suite for sqlite3Fts5ExprPhraseSize
// This test suite is designed to be compiled with a C++11 compiler without any
// external testing framework (GTest, etc.). It uses a lightweight, non-terminating
// assertion mechanism to collect and report failures while continuing execution.

#include <vector>
#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <string>
#include <iostream>


// Bring in the C headers with C linkage
extern "C" {
}

// Declare the focal function to ensure linkage when the header does not provide it
extern "C" int sqlite3Fts5ExprPhraseSize(Fts5Expr *pExpr, int iPhrase);

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro: logs failure but does not abort the test suite.
// This aligns with the requirement to use non-terminating assertions to maximize coverage.
#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << msg << " | Expected: " << (expected) \
                  << ", Got: " << (actual) << std::endl; \
    } else { \
        std::cout << "[OK] " << msg << std::endl; \
    } \
} while(0)

// Helper function to create a minimal Fts5Expr and its nested phrases.
// We rely on the actual internal structure defined by the included headers.
// Specifically, we expect:
// - Fts5Expr has a member int nPhrase
// - Fts5Expr has a member Fts5ExprPhrase **apExprPhrase
// - Each Fts5ExprPhrase has an int nTerm
static Fts5Expr* create_test_expr(int nPhrase, const std::vector<int>& terms) {
    Fts5Expr* pExpr = new Fts5Expr;
    pExpr->nPhrase = nPhrase;

    // Allocate array of pointers to Fts5ExprPhrase
    pExpr->apExprPhrase = new Fts5ExprPhrase*[nPhrase];
    for(int i = 0; i < nPhrase; ++i) {
        pExpr->apExprPhrase[i] = new Fts5ExprPhrase;
        // Initialize nTerm for each phrase
        int termVal = (i < (int)terms.size()) ? terms[i] : 0;
        pExpr->apExprPhrase[i]->nTerm = termVal;
    }
    return pExpr;
}

// Clean up resources created by create_test_expr
static void destroy_test_expr(Fts5Expr* pExpr) {
    if(!pExpr) return;
    if(pExpr->apExprPhrase) {
        for(int i = 0; i < pExpr->nPhrase; ++i) {
            delete pExpr->apExprPhrase[i];
        }
        delete[] pExpr->apExprPhrase;
    }
    delete pExpr;
}

// Test 1: iPhrase is negative -> should return 0
static void test_phrase_size_negative_index() {
    // Arrange
    // Create an expression with 3 phrases
    Fts5Expr* p = create_test_expr(3, {2, 4, 6});

    // Act
    int res = sqlite3Fts5ExprPhraseSize(p, -1);

    // Assert
    EXPECT_EQ(res, 0, "Negative iPhrase should return 0");

    // Cleanup
    destroy_test_expr(p);
}

// Test 2: iPhrase is >= nPhrase -> should return 0
static void test_phrase_size_index_out_of_bounds() {
    // Arrange
    Fts5Expr* p = create_test_expr(2, {7, 8});

    // Act
    int res1 = sqlite3Fts5ExprPhraseSize(p, 2); // iPhrase == nPhrase
    int res2 = sqlite3Fts5ExprPhraseSize(p, 3); // iPhrase > nPhrase

    // Assert
    EXPECT_EQ(res1, 0, "iPhrase equal to nPhrase should return 0");
    EXPECT_EQ(res2, 0, "iPhrase greater than nPhrase should return 0");

    // Cleanup
    destroy_test_expr(p);
}

// Test 3: Valid iPhrase returns corresponding nTerm
static void test_phrase_size_valid_index() {
    // Arrange
    // Create an expression with 3 phrases and specified term counts
    Fts5Expr* p = create_test_expr(3, {5, 9, 12});

    // Act
    int res0 = sqlite3Fts5ExprPhraseSize(p, 0);
    int res1 = sqlite3Fts5ExprPhraseSize(p, 1);
    int res2 = sqlite3Fts5ExprPhraseSize(p, 2);

    // Assert
    EXPECT_EQ(res0, 5, "Phrase 0 nTerm should be 5");
    EXPECT_EQ(res1, 9, "Phrase 1 nTerm should be 9");
    EXPECT_EQ(res2, 12, "Phrase 2 nTerm should be 12");

    // Cleanup
    destroy_test_expr(p);
}

// Test 4: Mixed scenario with different nTerm values
static void test_phrase_size_mixed_values() {
    // Arrange
    Fts5Expr* p = create_test_expr(4, {0, 3, 10, 999});

    // Act
    int r0 = sqlite3Fts5ExprPhraseSize(p, 0);
    int r1 = sqlite3Fts5ExprPhraseSize(p, 1);
    int r2 = sqlite3Fts5ExprPhraseSize(p, 2);
    int r3 = sqlite3Fts5ExprPhraseSize(p, 3);

    // Assert
    EXPECT_EQ(r0, 0, "Phrase 0 nTerm should be 0");
    EXPECT_EQ(r1, 3, "Phrase 1 nTerm should be 3");
    EXPECT_EQ(r2, 10, "Phrase 2 nTerm should be 10");
    EXPECT_EQ(r3, 999, "Phrase 3 nTerm should be 999");

    // Cleanup
    destroy_test_expr(p);
}

// Entry point for the test suite
int main() {
    std::cout << "Starting sqlite3Fts5ExprPhraseSize unit tests (C++11, no GTest)" << std::endl;

    // Run tests
    test_phrase_size_negative_index();
    test_phrase_size_index_out_of_bounds();
    test_phrase_size_valid_index();
    test_phrase_size_mixed_values();

    // Summary
    std::cout << "Tests completed. Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests
              << ", Total: " << g_total_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests != 0 ? 1 : 0;
}