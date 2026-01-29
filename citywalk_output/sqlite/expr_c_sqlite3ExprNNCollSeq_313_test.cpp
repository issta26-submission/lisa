/*
  Unit Test Suite for sqlite3ExprNNCollSeq (C interface) using C++11

  Step 1 - Program Understanding and Candidate Keywords
  - Focus: CollSeq *sqlite3ExprNNCollSeq(Parse *pParse, const Expr *pExpr)
  - Core dependencies (from the focal class/file context):
    - sqlite3ExprCollSeq(Parse*, const Expr*)
    - Parse structure with member: Db *db
    - Db structure with member: CollSeq *pDfltColl
    - CollSeq structure (opaque in the test; used for identity checks)
    - Expr structure (opaque to the test)
  - Candidate Keywords for test design:
    - sqlite3ExprNNCollSeq, sqlite3ExprCollSeq
    - Parse, Db, CollSeq, Expr
    - pParse, pExpr, pDfltColl
    - Non-null path (when sqlite3ExprCollSeq returns non-null)
    - Null path (when sqlite3ExprCollSeq returns null and default collage is used)
    - Assertions (ensuring non-null return)

  Step 2 - Unit Test Generation
  - Test target: ensure sqlite3ExprNNCollSeq returns the CollSeq from sqlite3ExprCollSeq when non-null,
    and falls back to pParse->db->pDfltColl when sqlite3ExprCollSeq returns null.
  - Test doubles: provide a mock sqlite3ExprCollSeq(Parse*, const Expr*) with controllable return value via a static/global variable.
  - Test cases:
    1) Non-null sqlite3ExprCollSeq return: verify that sqlite3ExprNNCollSeq returns exactly that pointer.
    2) Null sqlite3ExprCollSeq return: verify that sqlite3ExprNNCollSeq returns pDfltColl from the Parse/Db chain.
  - Important: The tests must not terminate the process via assertion; we will handle failures via boolean results and descriptive messages.

  Step 3 - Test Case Refinement
  - Coverage: both branches of the conditional inside sqlite3ExprNNCollSeq are exercised.
  - Static/global mocks are isolated per test to avoid cross-test interference.
  - Use only C++ standard library; avoid Google Test or other frameworks as requested.
  - The test suite will be executable with a simple main() that runs the tests and prints results.

  Implementation notes:
  - We do not implement sqlite3ExprNNCollSeq itself; we declare it externally to test its behavior with a controllable sqlite3ExprCollSeq mock.
  - The mock sqlite3ExprCollSeq uses a global pointer to decide the return value (non-null vs null).

  How to use:
  - Compile this test with the real sqlite3ExprNNCollSeq implementation linked in (e.g., build against the project that provides expr.c).
  - The test overrides sqlite3ExprCollSeq symbol with a C-compatible mock to enable controlled behavior.
*/

#include <iostream>
#include <sqliteInt.h>


// Minimal stand-ins for the real SQLite types to enable compilation of the test harness.
// The actual implementation in the project under test provides real structures.
// We only require enough structure to pass pointers around and verify identity.

struct CollSeq { int id; };    // Minimal placeholder for CollSeq
struct Db { CollSeq *pDfltColl; }; // Database object holding default CollSeq
struct Parse { Db *db; };          // Parser context carrying Db
struct Expr { int dummy; };        // Expression node (opaque for test)

// Forward declaration of the function under test.
// We declare it extern to link against the real implementation from expr.c.
extern "C" CollSeq *sqlite3ExprNNCollSeq(Parse *pParse, const Expr *pExpr);

// Mockable global used by the mock for sqlite3ExprCollSeq.
static CollSeq *g_mock_exprCollSeq_return = nullptr;

// Mock implementation for sqlite3ExprCollSeq to control test behavior.
// This overrides the real library function during the test linking.
// Signature must match the real function for proper linking (C linkage).
extern "C" CollSeq *sqlite3ExprCollSeq(Parse *pParse, const Expr *pExpr) {
    // Return the pointer controlled by tests. If NULL, simulates the "no specific collation" case.
    (void)pParse; // unused in the mock, but kept to match signature
    (void)pExpr;  // unused in the mock
    return g_mock_exprCollSeq_return;
}

// Test helpers and test cases

// Test 1: sqlite3ExprCollSeq returns non-null; sqlite3ExprNNCollSeq should return that same pointer.
bool test_nncollseq_prefers_non_null_collseq() {
    // Arrange
    CollSeq a { 1 };          // The non-null CollSeq to be returned by the mock
    Db db { nullptr };         // Db with defaultColl unused in this scenario
    Parse p { &db };            // Parse referencing our Db

    // Ensure the default is not used in this scenario
    CollSeq dummyDefault { 99 };
    db.pDfltColl = &dummyDefault;

    // Configure mock to return non-null pointer
    g_mock_exprCollSeq_return = &a;

    Expr e { 0 };

    // Act
    CollSeq *result = sqlite3ExprNNCollSeq(&p, &e);

    // Assert
    if (result != &a) {
        std::cerr << "Test 1 Failed: Expected non-null CollSeq from sqlite3ExprCollSeq to be returned by sqlite3ExprNNCollSeq.\n";
        return false;
    }

    // Cleanup
    g_mock_exprCollSeq_return = nullptr;
    return true;
}

// Test 2: sqlite3ExprCollSeq returns null; sqlite3ExprNNCollSeq should fall back to pParse->db->pDfltColl.
bool test_nncollseq_uses_default_when_null() {
    // Arrange
    CollSeq dflt { 2 };          // The default CollSeq to be used if mock returns NULL
    Db db { &dflt };               // Db holds pointer to default CollSeq
    Parse p { &db };                // Parse referencing our Db

    // Configure mock to return NULL to trigger fallback
    g_mock_exprCollSeq_return = nullptr;

    Expr e { 0 };

    // Act
    CollSeq *result = sqlite3ExprNNCollSeq(&p, &e);

    // Assert
    if (result != &dflt) {
        std::cerr << "Test 2 Failed: Expected fallback to default CollSeq when sqlite3ExprCollSeq returns NULL.\n";
        return false;
    }

    // Cleanup
    g_mock_exprCollSeq_return = nullptr;
    return true;
}

// Main runner
int main() {
    bool all_passed = true;

    std::cout << "Running sqlite3ExprNNCollSeq unit tests (mocked sqlite3ExprCollSeq)...\n";

    if (!test_nncollseq_prefers_non_null_collseq()) {
        all_passed = false;
        std::cout << "1) FAILED: test_nncollseq_prefers_non_null_collseq\n";
    } else {
        std::cout << "1) PASSED: test_nncollseq_prefers_non_null_collseq\n";
    }

    if (!test_nncollseq_uses_default_when_null()) {
        all_passed = false;
        std::cout << "2) FAILED: test_nncollseq_uses_default_when_null\n";
    } else {
        std::cout << "2) PASSED: test_nncollseq_uses_default_when_null\n";
    }

    if (all_passed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}