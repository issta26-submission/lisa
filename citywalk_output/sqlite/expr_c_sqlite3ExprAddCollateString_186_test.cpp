// Unit test suite for sqlite3ExprAddCollateString
// This test is implemented in C++11 without using GTest.
// It provides minimal stand-ins for the necessary SQLite-like types
// and stubs for dependencies to exercise the focal method.
// The test harness uses simple, non-terminating expectations and a
// small main() to run tests and report results.
//
// Test aims:
// - Verify that sqlite3ExprAddCollateString constructs a Token from zC,
//   then delegates to sqlite3ExprAddCollateToken with that token and dequote=0.
// - Verify that the delegated function receives correct pParse, pExpr, and z (token string).
// - Use two representative inputs: a non-empty collate name and an empty string.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Candidate Keywords (core dependencies)
// - Parse, Expr: minimal stubs for parsing context and expression nodes
// - Token: token representation used by sqlite3TokenInit
// - sqlite3TokenInit: initializes a Token from a C-string
// - sqlite3ExprAddCollateToken: adds COLLATE clause using a Token
// - sqlite3ExprAddCollateString: focal method under test
// - zC: input collating sequence name
// These elements are sufficient to model the behavior for testing.

// Minimal type definitions to model the focal API surface
typedef struct Token Token;
struct Token {
    const char *z; // pointer to string data
    int n;         // length of the string
};

struct Parse {
    int dummyParse; // placeholder
};

struct Expr {
    int dummyExpr; // placeholder
};

// Forward declarations of the functions under test and its dependencies
extern "C" {
    void sqlite3TokenInit(Token *pTok, char *z);
    Expr *sqlite3ExprAddCollateToken(const Parse *pParse,
                                    Expr *pExpr,
                                    const Token *pCollName,
                                    int dequote);
    Expr *sqlite3ExprAddCollateString(const Parse *pParse,
                                     Expr *pExpr,
                                     const char *zC);
}

// Global state for test assertions to observe internal calls
static const char *g_last_collName = nullptr;
static int g_last_collName_len = 0;
static const Parse *g_last_pParse = nullptr;
static const Expr *g_last_pExpr = nullptr;
static int g_last_dequote = -1;

// Sentinel return value for sqlite3ExprAddCollateToken to verify the wrapper return
static Expr *sqlite3ExprAddCollateToken_returnValue = (Expr *)0xDEADBEEF;

// Implementation of the dependency: initialize a Token from a C-string.
// To ensure that the collate name remains valid after the local scope of
// sqlite3ExprAddCollateString(), allocate a copy of the string.
void sqlite3TokenInit(Token *pTok, char *z) {
    // Duplicate the string to ensure a stable lifetime for test verification
    if (z) {
        pTok->z = strdup(z);
        pTok->n = (int)std::strlen(z);
    } else {
        pTok->z = nullptr;
        pTok->n = 0;
    }
}

// Implementation of the dependency: delegate call storing the received token data.
// We do not rely on the expression object itself; we only verify that the
// token data (string and length) matches the input zC.
Expr *sqlite3ExprAddCollateToken(const Parse *pParse,
                                Expr *pExpr,
                                const Token *pCollName,
                                int dequote) {
    g_last_pParse = pParse;
    g_last_pExpr = pExpr;
    g_last_collName = pCollName ? pCollName->z : nullptr;
    g_last_collName_len = pCollName ? pCollName->n : 0;
    g_last_dequote = dequote;
    return sqlite3ExprAddCollateToken_returnValue;
}

// Implementation of the focal method under test
Expr *sqlite3ExprAddCollateString(const Parse *pParse,
                                 Expr *pExpr,
                                 const char *zC) {
    Token s;
    assert(zC != nullptr);
    sqlite3TokenInit(&s, (char*)zC);
    return sqlite3ExprAddCollateToken(pParse, pExpr, &s, 0);
}

// Small non-terminating expectation helpers
static int g_tests_failed = 0;

#define EXPECT_PTR_EQ(a, b) do { \
    if ((void*)(a) != (void*)(b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " << #a << " != " << #b \
                  << " (" << (void*)(a) << " vs " << (void*)(b) << ")\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_STR_EQ(a, b) do { \
    if ((a) == nullptr && (b) == nullptr) { \
        /* both null, ok */ \
    } else if ((a) == nullptr || (b) == nullptr) { \
        std::cerr << "EXPECT_STR_EQ failed: one of strings is null (" << (void*)(a) \
                  << ", " << (void*)(b) << ")\n"; \
        ++g_tests_failed; \
    } else if (std::strcmp((a), (b)) != 0) { \
        std::cerr << "EXPECT_STR_EQ failed: \"" << (a) << "\" != \"" << (b) << "\"\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_EQ_INT(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ_INT failed: " << (a) << " != " << (b) << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

int main() {
    // Candidate Keywords and dependencies verified via compilation:
    // - Token, Parse, Expr
    // - sqlite3TokenInit
    // - sqlite3ExprAddCollateToken
    // - sqlite3ExprAddCollateString (focal method)

    // Test 1: Basic behavior with non-empty collate name
    // The wrapper should construct a Token from zC and pass it to sqlite3ExprAddCollateToken
    // with dequote = 0, returning the sentinel value.
    {
        Parse p; Expr e;
        const char *z = "NOCASE";

        // Reset captured state
        g_last_pParse = nullptr;
        g_last_pExpr = nullptr;
        g_last_collName = nullptr;
        g_last_collName_len = 0;
        g_last_dequote = -1;

        sqlite3ExprAddCollateToken_returnValue = (Expr *)0xDEAD1;
        Expr *ret = sqlite3ExprAddCollateString(&p, &e, z);

        // Assertions
        EXPECT_PTR_EQ(ret, sqlite3ExprAddCollateToken_returnValue);
        EXPECT_STR_EQ(g_last_collName, z);
        EXPECT_EQ_INT(g_last_collName_len, (int)std::strlen(z));
        EXPECT_PTR_EQ(g_last_pParse, &p);
        EXPECT_PTR_EQ(g_last_pExpr, &e);
        // Dequote should be passed as 0
        EXPECT_EQ_INT(g_last_dequote, 0);
    }

    // Test 2: Empty collate name string
    // zC = "" should still be accepted; token length should be 0, and string should be "".
    {
        // Reset captured state
        g_last_pParse = nullptr;
        g_last_pExpr = nullptr;
        g_last_collName = nullptr;
        g_last_collName_len = 0;
        g_last_dequote = -1;

        const char *z2 = "";
        sqlite3ExprAddCollateToken_returnValue = (Expr *)0xDEAD2;
        Expr *ret = sqlite3ExprAddCollateString(nullptr, nullptr, z2);

        // Assertions
        EXPECT_PTR_EQ(ret, sqlite3ExprAddCollateToken_returnValue);
        EXPECT_STR_EQ(g_last_collName, z2);
        EXPECT_EQ_INT(g_last_collName_len, 0);
        EXPECT_PTR_EQ(g_last_pParse, nullptr);
        EXPECT_PTR_EQ(g_last_pExpr, nullptr);
        EXPECT_EQ_INT(g_last_dequote, 0);
    }

    // Test 3: Another non-empty collate name with distinct inputs
    {
        Parse p2; Expr e2;
        const char *z3 = "BIN";
        sqlite3ExprAddCollateToken_returnValue = (Expr *)0xDEAD3;
        Expr *ret = sqlite3ExprAddCollateString(&p2, &e2, z3);

        // Assertions
        EXPECT_PTR_EQ(ret, sqlite3ExprAddCollateToken_returnValue);
        EXPECT_STR_EQ(g_last_collName, z3);
        EXPECT_EQ_INT(g_last_collName_len, (int)std::strlen(z3));
        EXPECT_PTR_EQ(g_last_pParse, &p2);
        EXPECT_PTR_EQ(g_last_pExpr, &e2);
        EXPECT_EQ_INT(g_last_dequote, 0);
    }

    if (g_tests_failed == 0) {
        std::cout << "All tests passed\n";
        return 0;
    } else {
        std::cerr << g_tests_failed << " test(s) failed\n";
        return 1;
    }
}