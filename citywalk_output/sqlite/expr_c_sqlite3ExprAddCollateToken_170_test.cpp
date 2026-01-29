// Unit tests for sqlite3ExprAddCollateToken (adapted for C++11 without GTest)
// This test suite uses a minimal mock environment to exercise the focal function's logic.
// It covers: (a) adding a COLLATE token when pCollName.n > 0, (b) skipping when n == 0,
// (c) behavior when sqlite3ExprAlloc fails (returns NULL).
//
// Note: This test provides a self-contained mock of the dependent data structures
// and the sqlite3ExprAlloc helper to validate the control flow and state changes
// performed by sqlite3ExprAddCollateToken.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// ----------------------
// Minimal Mock Types
// ----------------------

// Token structure used by sqlite3ExprAddCollateToken
struct Token {
    int n;          // length (in tokens) of the name; >0 means non-empty
    const char* z;  // token text (not used in tests beyond passing through)
};

// Forward declarations of the dependencies
struct sqlite3; // opaque db handle
struct Parse {
    sqlite3* db;
};

// Expression node (minimal subset)
struct Expr {
    int op;                 // operation code (e.g., TK_COLLATE for collate token)
    const Token* pToken;    // associated token (pointer to the collation name)
    Expr* pLeft;              // left sub-expression
    unsigned int flags;       // bit flags (e.g., EP_Collate, EP_Skip)
    int dequote;              // whether the token was dequoted (pass-through)
};

// Public-ish constants (mocked)
const int TK_COLLATE = 1234;       // made-up opcode for COLLATE
const unsigned int EP_Collate = 0x01;
const unsigned int EP_Skip    = 0x02;

// ----------------------
// Mocked Helpers
// ----------------------

// Global switch to simulate allocation failure in tests
static bool g_failExprAlloc = false;

// A lightweight dummy sqlite3 object (not used in tests beyond passing a pointer)
struct sqlite3 {
    int dummy;
};

// Mock of sqlite3ExprAlloc used by the focal function.
// In real SQLite, this allocates an Expr and ties it to the provided token.
// Here, we emulate minimal behavior needed for tests.
Expr* sqlite3ExprAlloc(sqlite3* db, int op, const Token* pToken, int dequote) {
    (void)db;      // unused in mock
    (void)dequote; // in tests we verify that the dequote value is passed through

    if (g_failExprAlloc) {
        return nullptr; // simulate allocation failure
    }

    Expr* e = new Expr();
    e->op = op;
    e->pToken = pToken;
    e->pLeft = nullptr;
    e->flags = 0;
    e->dequote = dequote;
    return e;
}

// The focal function under test (adapted to be self-contained in this file).
// In the real project, this function is defined in expr.c. We reproduce the logic here
// so the unit tests can exercise its behavior in isolation.
Expr* sqlite3ExprAddCollateToken(
    const Parse* pParse,     /* Parsing context */
    Expr* pExpr,             /* Add the "COLLATE" clause to this expression */
    const Token* pCollName,  /* Name of collating sequence */
    int dequote              /* True to dequote pCollName */
) {
    if (pCollName->n > 0) {
        Expr* pNew = sqlite3ExprAlloc(pParse->db, TK_COLLATE, pCollName, dequote);
        if (pNew) {
            pNew->pLeft = pExpr;
            pNew->flags |= EP_Collate | EP_Skip;
            pExpr = pNew;
        }
    }
    return pExpr;
}

// ----------------------
// Simple Test Harness
// ----------------------

static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_TRUE(cond, msg) do {                                     \
    ++g_tests_run;                                                        \
    if (!(cond)) {                                                        \
        std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")\n";\
    } else {                                                              \
        ++g_tests_passed;                                                \
    }                                                                     \
} while (0)

#define EXPECT_EQ(a, b, msg) do {                                        \
    ++g_tests_run;                                                        \
    if ((a) != (b)) {                                                     \
        std::cerr << "[FAILED] " << msg << " (expected " << (b)            \
                  << ", got " << (a) << ") (line " << __LINE__ << ")\n"; \
    } else {                                                              \
        ++g_tests_passed;                                                \
    }                                                                     \
} while (0)

#define EXPECT_NOT_NULL(p, msg) do {                                      \
    ++g_tests_run;                                                        \
    if ((p) == nullptr) {                                               \
        std::cerr << "[FAILED] " << msg << " (null pointer) (line "        \
                  << __LINE__ << ")\n";                                  \
    } else {                                                              \
        ++g_tests_passed;                                                \
    }                                                                     \
} while (0)

static void test_AddCollateToken_AddsNode() {
    // Test that when collate token has non-empty name, a new COLLATE node is created
    // and linked to the original expression as its left child. Flags must be EP_Collate | EP_Skip.

    // Setup
    Parse pParse;
    sqlite3 db;
    pParse.db = &db;

    // Original expression (could be any expression; we keep it simple)
    Expr* pOriginal = new Expr();
    pOriginal->op = 999;
    pOriginal->pToken = nullptr;
    pOriginal->pLeft = nullptr;
    pOriginal->flags = 0;
    pOriginal->dequote = 0;

    // Collation name token with non-empty length
    Token collName;
    collName.n = 5; // non-empty triggers creation
    collName.z = "BINARY";

    // Call the function under test
    Expr* pResult = sqlite3ExprAddCollateToken(&pParse, pOriginal, &collName, 1);

    // Assertions
    EXPECT_TRUE(pResult != nullptr, "Result should not be NULL when collate token is non-empty");
    EXPECT_TRUE(pResult != pOriginal, "Result should be a new node when collate token is non-empty");
    // pResult should be a TK_COLLATE node with left pointing to pOriginal
    EXPECT_EQ(pResult->op, TK_COLLATE, "Result op should be TK_COLLATE");
    EXPECT_TRUE(pResult->pLeft == pOriginal, "Result's left child should be the original expression");
    EXPECT_TRUE((pResult->flags & (EP_Collate | EP_Skip)) == (EP_Collate | EP_Skip),
                "Result flags should include EP_Collate and EP_Skip");
    EXPECT_TRUE(pResult->pToken == &collName, "Result's token should be the provided collName");
    EXPECT_EQ(pResult->dequote, 1, "Result's dequote should be equal to the input dequote");
    
    // Cleanup
    delete pResult; // Note: pResult->pLeft points to pOriginal, delete cascades manually
    delete pOriginal;
    // db is stack-allocated; no delete needed
}

static void test_AddCollateToken_SkipWhenEmptyName() {
    // Test that when collate token name is empty (n == 0), the original expression is returned
    // and no new node is allocated.

    // Setup
    Parse pParse;
    sqlite3 db;
    pParse.db = &db;

    // Original expression
    Expr* pOriginal = new Expr();
    pOriginal->op = 888;
    pOriginal->pToken = nullptr;
    pOriginal->pLeft = nullptr;
    pOriginal->flags = 0;
    pOriginal->dequote = 0;

    // Collation name token with empty name
    Token collName;
    collName.n = 0;  // empty means no COLLATE node should be created
    collName.z = "";

    // Call the function under test
    Expr* pResult = sqlite3ExprAddCollateToken(&pParse, pOriginal, &collName, 0);

    // Assertions
    EXPECT_TRUE(pResult != nullptr, "Result should not be NULL when input expr is non-NULL");
    EXPECT_TRUE(pResult == pOriginal, "Result should be the same as input when collate name is empty");

    // Cleanup
    delete pOriginal;
}

static void test_AddCollateToken_AllocationFailure() {
    // Test that when sqlite3ExprAlloc fails (returns NULL), the original expression is returned
    // and no allocation is leaked.

    // Setup
    Parse pParse;
    sqlite3 db;
    pParse.db = &db;

    // Original expression
    Expr* pOriginal = new Expr();
    pOriginal->op = 777;
    pOriginal->pToken = nullptr;
    pOriginal->pLeft = nullptr;
    pOriginal->flags = 0;
    pOriginal->dequote = 0;

    // Collation name token with non-empty name
    Token collName;
    collName.n = 4;
    collName.z = "NOCASE";

    // Simulate allocation failure
    g_failExprAlloc = true;

    // Call the function under test
    Expr* pResult = sqlite3ExprAddCollateToken(&pParse, pOriginal, &collName, 0);

    // Assertions
    EXPECT_TRUE(pResult == pOriginal, "Result should be the same as input when allocation fails");

    // Cleanup
    g_failExprAlloc = false;
    delete pOriginal;
}

// ----------------------
// Main (Runner)
// ----------------------

int main() {
    std::cout << "Running unit tests for sqlite3ExprAddCollateToken (mocked) ..." << std::endl;

    test_AddCollateToken_AddsNode();
    test_AddCollateToken_SkipWhenEmptyName();
    test_AddCollateToken_AllocationFailure();

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed << std::endl;
    if (g_tests_run == g_tests_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}