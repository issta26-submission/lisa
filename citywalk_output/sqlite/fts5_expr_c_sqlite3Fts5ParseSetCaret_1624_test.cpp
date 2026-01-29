// Test suite for sqlite3Fts5ParseSetCaret
// This test suite is designed to be compiled with C++11 (no GTest).
// It relies on the public FTS5 headers (fts5Int.h, fts5parse.h) to obtain
// the correct structure definitions for Fts5ExprPhrase and Fts5ExprTerm.

#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// Prototype of the function under test (C linkage)
extern "C" void sqlite3Fts5ParseSetCaret(Fts5ExprPhrase *pPhrase);

// Simple, lightweight test harness (non-terminating assertions)
static int g_failures = 0;

// Helper macro: non-terminating assertion for integers
#define ASSERT_EQ_INT(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "Assertion failed: " msg "\n" \
                  << "  Actual: " << (actual) << ", Expected: " << (expected) \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// Helper macro: non-terminating assertion for pointers (truthiness)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " msg "\n" \
                  << "  Condition is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// Step 3/4: Unit Tests

// Test 1: Passing nullptr should be a no-op and not crash.
static bool test_null_pointer_noop() {
    // This test ensures the function handles a null pointer gracefully.
    // The function should simply return without accessing memory.
    sqlite3Fts5ParseSetCaret(nullptr);
    // If no crash occurred, we consider this test passed.
    return true;
}

// Test 2: Passing a phrase with nTerm == 0 should be a no-op.
static bool test_zero_terms_noop() {
    // Create a phrase object with zero terms; aTerm should not be accessed.
    Fts5ExprPhrase phrase;
    phrase.nTerm = 0;

    // Initialize aTerm memory conservatively (values won't be used, but keep it sane).
    // Some implementations allocate aTerm as a fixed-size array; ensure access is safe.
    for (int i = 0; i < 1; ++i) {
        phrase.aTerm[i].bFirst = 0;
    }

    sqlite3Fts5ParseSetCaret(&phrase);

    // Post-condition: nothing should have been modified (since nTerm == 0).
    ASSERT_EQ_INT(phrase.nTerm, 0, "test_zero_terms_noop: nTerm should remain 0");
    // If the code didn't crash, and nTerm is unchanged, test passes.
    return g_failures == 0;
}

// Test 3: When nTerm > 0, the first term's bFirst should be set to 1.
static bool test_set_caret_first_term() {
    // Build a phrase with at least one term. We rely on the public layout:
    // pPhrase->nTerm indicates number of terms; pPhrase->aTerm[0].bFirst should become 1.
    Fts5ExprPhrase phrase;
    phrase.nTerm = 1;
    phrase.aTerm[0].bFirst = 0; // ensure we observe a change

    sqlite3Fts5ParseSetCaret(&phrase);

    // Validate the expected change on the first term
    ASSERT_EQ_INT(phrase.aTerm[0].bFirst, 1, "test_set_caret_first_term: aTerm[0].bFirst should be set to 1");
    return g_failures == 0;
}

// Main: run all tests and report summary
int main() {
    std::cout << "Running sqlite3Fts5ParseSetCaret unit tests (C++11, no GTest)..." << std::endl;

    // Reset failure counter
    g_failures = 0;

    // Execute tests
    test_null_pointer_noop();
    test_zero_terms_noop();
    test_set_caret_first_term();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}