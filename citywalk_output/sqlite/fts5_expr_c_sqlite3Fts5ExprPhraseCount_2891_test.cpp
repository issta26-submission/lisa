#include <vector>
#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <string>
#include <iostream>


#ifdef __cplusplus
extern "C" {
#endif
// Include the public/internal FTS5 definitions required for sqlite3Fts5ExprPhraseCount.
// The test relies on the real Fts5Expr type layout to access nPhrase correctly.

 // Prototype for the function under test (C linkage for C codebase compatibility)
 int sqlite3Fts5ExprPhraseCount(Fts5Expr *pExpr);
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion helper: logs failures but does not exit.
// This allows executing multiple test cases in a single run for higher coverage.
static void expect_eq_int(int expected, int actual, const std::string& testDesc,
                          std::vector<std::string>& logs) {
    if (expected == actual) {
        logs.push_back("PASS: " + testDesc);
    } else {
        logs.push_back("FAIL: " + testDesc +
                       " | expected: " + std::to_string(expected) +
                       ", actual: " + std::to_string(actual));
    }
}

// Test 1: Null pointer should yield 0 (true branch when pExpr is null)
static void test_null_pointer(std::vector<std::string>& logs) {
    // Purpose: ensure the function handles a null input gracefully
    int result = sqlite3Fts5ExprPhraseCount(nullptr);
    expect_eq_int(0, result, "sqlite3Fts5ExprPhraseCount(null) == 0", logs);
}

// Test 2: Non-null expression with nPhrase = 0 should yield 0 (false branch for non-null but zero)
static void test_nonnull_zero(std::vector<std::string>& logs) {
    Fts5Expr expr;
    expr.nPhrase = 0;
    int result = sqlite3Fts5ExprPhraseCount(&expr);
    expect_eq_int(0, result, "sqlite3Fts5ExprPhraseCount(nPhrase=0) == 0", logs);
}

// Test 3: Non-null expression with a positive nPhrase should return that value
static void test_nonnull_positive(std::vector<std::string>& logs) {
    Fts5Expr expr;
    expr.nPhrase = 5;
    int result = sqlite3Fts5ExprPhraseCount(&expr);
    expect_eq_int(5, result, "sqlite3Fts5ExprPhraseCount(nPhrase=5) == 5", logs);
}

// Test 4: Non-null expression with a negative nPhrase should return the negative value
static void test_nonnull_negative(std::vector<std::string>& logs) {
    Fts5Expr expr;
    expr.nPhrase = -7;
    int result = sqlite3Fts5ExprPhraseCount(&expr);
    expect_eq_int(-7, result, "sqlite3Fts5ExprPhraseCount(nPhrase=-7) == -7", logs);
}

int main() {
    std::vector<std::string> logs;

    // Execute test cases
    test_null_pointer(logs);
    test_nonnull_zero(logs);
    test_nonnull_positive(logs);
    test_nonnull_negative(logs);

    // Print test results
    int passes = 0;
    for (const auto& line : logs) {
        std::cout << line << std::endl;
        if (line.rfind("PASS:", 0) == 0) {
            ++passes;
        }
    }

    int total = static_cast<int>(logs.size());
    std::cout << "Summary: " << passes << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed to assist automated pipelines
    return (passes == total) ? 0 : 1;
}