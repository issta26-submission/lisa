/*
Unit test suite for sqlite3Fts5ExprPoslist
- Target: Fts5ExprPoslist function defined in fts5_expr.c
- Environment: C++11, no Google Test (plain test runner)
- Dependencies: Real project headers should provide Fts5Expr, Fts5ExprPhrase, Fts5ExprNode, etc.
- Approach: Build minimal test fixtures that exercise true/false branches of the conditional
  inside sqlite3Fts5ExprPoslist by constructing Fts5Expr and related structures compatible
  with the production definitions.
- Note: This test relies on the actual project headers for type definitions. It uses an
  extern "C" declaration for the focal function to ensure correct linking with the C code.
*/

#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <iostream>


// Forward declare the focal function with C linkage to ensure correct linkage
extern "C" int sqlite3Fts5ExprPoslist(void *pExpr, int iPhrase, const unsigned char **pa);

// Include the project headers that declare the involved structs.
// We rely on the real definitions provided by the codebase.

// Helper macro for lightweight non-terminating checks: logs failure but continues
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) << " (in " << __func__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

static int g_failures = 0;

/*
Test 1: True branch of the conditional
- Set pNode (phrase node) such that:
  pNode->bEof == 0 and pNode->iRowid == pExpr->pRoot->iRowid
- The function should set *pa to the phrase's poslist.p and return poslist.n
*/
static bool test_poslist_true_case() {
    // Prepare root and node with matching rowid and not EOF
    Fts5ExprNode rootNode;
    rootNode.iRowid = 0; // will be overridden by pNode's own iRowid for equality
    // We need pExpr->pRoot to have iRowid equal to pPhrase->pNode->iRowid
    rootNode.bEof = 0;   // not EOF (though root bEof is not directly used, keep consistent)

    Fts5ExprNode node;
    node.bEof = 0;
    node.iRowid = 0; // will set to match root's iRowid

    // Phrase with a poslist
    Fts5Poslist pos = { nullptr, 0 }; // p and n
    const unsigned char payload[] = { 'h', 'e', 'l', 'l', 'o' }; // 5 bytes
    pos.p = payload;
    pos.n = 5;

    Fts5ExprPhrase phrase;
    phrase.pNode = &node;
    phrase.poslist = pos;

    // Build pExpr with root pointing to rootNode and apExprPhrase[0] referencing phrase
    Fts5Expr *pExpr = new Fts5Expr();
    pExpr->pRoot = &rootNode;
    // Ensure root iRowid equals node.iRowid
    rootNode.iRowid = 123;
    node.iRowid = 123;

    pExpr->apExprPhrase = new Fts5ExprPhrase*[1];
    pExpr->apExprPhrase[0] = &phrase;

    // Call focal function
    const unsigned char *pa = nullptr;
    int nRet = sqlite3Fts5ExprPoslist(pExpr, 0, &pa);

    // Validate
    bool ok = true;
    ok &= (nRet == 5);
    ok &= (pa == payload);
    ok &= (std::memcmp(pa, payload, 5) == 0);

    // Cleanup
    delete[] pExpr->apExprPhrase;
    delete pExpr;

    return ok;
}

/*
Test 2: False branch due to EOF flag
- Set pNode->bEof != 0 (EOF), even if rowid matches.
- Expect *pa == nullptr and return value 0
*/
static bool test_poslist_false_case_eof() {
    Fts5ExprNode rootNode;
    Fts5ExprNode node;
    rootNode.iRowid = 42;
    rootNode.bEof = 0;

    node.bEof = 1; // EOF causes false branch
    node.iRowid = 42; // same rowid as root, but EOF should negate

    Fts5Poslist pos;
    const unsigned char payload[] = { 'w', 'o', 'r', 'l', 'd' };
    pos.p = payload;
    pos.n = 5;

    Fts5ExprPhrase phrase;
    phrase.pNode = &node;
    phrase.poslist = pos;

    Fts5Expr *pExpr = new Fts5Expr();
    pExpr->pRoot = &rootNode;
    pExpr->apExprPhrase = new Fts5ExprPhrase*[1];
    pExpr->apExprPhrase[0] = &phrase;

    const unsigned char *pa = nullptr;
    int nRet = sqlite3Fts5ExprPoslist(pExpr, 0, &pa);

    bool ok = true;
    ok &= (nRet == 0);
    ok &= (pa == nullptr);

    delete[] pExpr->apExprPhrase;
    delete pExpr;

    return ok;
}

/*
Test 3: False branch due to rowid mismatch
- Set pNode->iRowid != pExpr->pRoot->iRowid
- Expect *pa == nullptr and return value 0
*/
static bool test_poslist_false_case_mismatch() {
    Fts5ExprNode rootNode;
    Fts5ExprNode node;
    rootNode.iRowid = 7;
    rootNode.bEof = 0;

    node.bEof = 0;
    node.iRowid = 999; // mismatch

    Fts5Poslist pos;
    const unsigned char payload[] = { 't', 'e', 's', 't' };
    pos.p = payload;
    pos.n = 4;

    Fts5ExprPhrase phrase;
    phrase.pNode = &node;
    phrase.poslist = pos;

    Fts5Expr *pExpr = new Fts5Expr();
    pExpr->pRoot = &rootNode;
    pExpr->apExprPhrase = new Fts5ExprPhrase*[1];
    pExpr->apExprPhrase[0] = &phrase;

    const unsigned char *pa = nullptr;
    int nRet = sqlite3Fts5ExprPoslist(pExpr, 0, &pa);

    bool ok = true;
    ok &= (nRet == 0);
    ok &= (pa == nullptr);

    delete[] pExpr->apExprPhrase;
    delete pExpr;

    return ok;
}

int main() {
    // Run tests and accumulate results
    std::cout << "Running sqlite3Fts5ExprPoslist unit tests (C++11)...\n";

    bool t1 = test_poslist_true_case();
    CHECK(t1, "True-branch test failed");

    bool t2 = test_poslist_false_case_eof();
    CHECK(t2, "False-branch (EOF) test failed");

    bool t3 = test_poslist_false_case_mismatch();
    CHECK(t3, "False-branch (rowid mismatch) test failed");

    int total = 3;
    if(g_failures == 0){
        std::cout << "[PASS] All tests passed (" << total << " tests).\n";
        return 0;
    } else {
        std::cout << "[FAIL] " << g_failures << " test(s) failed out of " << total << ".\n";
        // Return non-zero to indicate failure
        return g_failures;
    }
}