// Unit tests for sqlite3Fts5ExprPopulatePoslists
// Target: test behavior of bOk population logic within the function
// Constraints: no GTest; use a lightweight, self-contained test harness.
// The tests rely on the project headers for Fts5* types.
// This test is intended to be compiled and linked with the fts5_expr.c sources (and related SQLite FTS5 codebase).
// The mock scaffolding below focuses on covering the conditional branches in the for-loop
// that sets aPopulator[i].bOk based on pColset and aPopulator[i].bMiss.

#include <vector>
#include <fts5Int.h>
#include <memory>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <iostream>


// Forward-declare C APIs and include project headers to bring in the Fts5 types.
// We wrap C headers to ensure proper linkage when compiled as C++11.
extern "C" {

// Prototype for the focal function under test.
// This declaration must match the actual function signature from the C source.
int sqlite3Fts5ExprPopulatePoslists(
  Fts5Config *pConfig,
  Fts5Expr *pExpr, 
  Fts5PoslistPopulator *aPopulator,
  int iCol, 
  const char *z, int n
);
}

// Simple non-terminating test assertion helpers.
// They log failures but do not abort the test execution, enabling higher code coverage.
static int g_failures = 0;

static void LogFail(const char* desc, int expected, int actual)
{
    std::cerr << "FAIL: " << desc << " (expected: " << expected << ", actual: " << actual << ")" << std::endl;
    ++g_failures;
}

static void LogPass(const char* desc)
{
    std::cout << "PASS: " << desc << std::endl;
}

// Lightweight test harness helpers for setting up FTS5 data structures
// using the project-provided types. The test focuses on the behavior inside
// sqlite3Fts5ExprPopulatePoslists' for-loop, which inspects:
// - pExpr->apExprPhrase[i]->pNode->pNear->pColset
// - aPopulator[i].bMiss
// and assigns aPopulator[i].bOk accordingly.
static void test_populatePoslists_noColset_noMiss()
{
    // Description for logging
    const char* testDesc = "PopulatePoslists: no colset and no miss -> bOk should be 1";

    // Prepare a minimal FTS5 expression with two phrases, both lacking a Colset.
    Fts5Config cfg;
    std::memset(&cfg, 0, sizeof(cfg));

    Fts5Expr expr;
    std::memset(&expr, 0, sizeof(expr));
    const int nPhrase = 2;
    expr.nPhrase = nPhrase;

    // Allocate apExprPhrase array
    Fts5ExprPhrase **ppExpr = new Fts5ExprPhrase*[nPhrase];
    for (int i = 0; i < nPhrase; ++i) {
        Fts5ExprPhrase *pPhrase = new Fts5ExprPhrase();
        pPhrase->pNode = new Fts5ExprNode();
        pPhrase->pNode->pNear = new Fts5ExprNear();
        // Intentionally leave pColset NULL to simulate "no colset"
        pPhrase->pNode->pNear->pColset = NULL;
        ppExpr[i] = pPhrase;
    }
    expr.apExprPhrase = ppExpr;

    // Prepare aPopulator array with both elements having bMiss = 0 (false)
    Fts5PoslistPopulator pop[2];
    for (int i = 0; i < nPhrase; ++i) {
        // Zero-initialize and set bMiss to false
        pop[i].bMiss = 0;
        pop[i].bOk = 0;
    }

    // Call the focal function with iCol = 0 and a sample string
    const char *zExpr = "a test string";
    int rc = sqlite3Fts5ExprPopulatePoslists(&cfg, &expr, pop, 0, zExpr, (int)std::strlen(zExpr));

    // Expect rc == 0 (mocked tokenization would typically return 0)
    if (rc != 0) {
        LogFail("Return code rc", 0, rc);
    } else {
        LogPass("Return code rc equals 0");
    }

    // Validate that both populator entries were marked as OK (bOk == 1)
    if (pop[0].bOk != 1) {
        LogFail("pop[0].bOk == 1 when no colset and no miss", 1, pop[0].bOk);
    } else {
        LogPass("pop[0].bOk == 1 (no colset, no miss)");
    }

    if (pop[1].bOk != 1) {
        LogFail("pop[1].bOk == 1 when no colset and no miss", 1, pop[1].bOk);
    } else {
        LogPass("pop[1].bOk == 1 (no colset, no miss)");
    }

    // Cleanup
    for (int i = 0; i < nPhrase; ++i) {
        delete expr.apExprPhrase[i]->pNode->pNear;
        delete expr.apExprPhrase[i]->pNode;
        delete expr.apExprPhrase[i];
    }
    delete[] expr.apExprPhrase;
}

static void test_populatePoslists_withMissFlag()
{
    // Description for logging
    const char* testDesc = "PopulatePoslists: aPopulator[i].bMiss == 1 -> corresponding bOk should be 0";

    // Prepare a minimal FTS5 expression with two phrases, both lacking a Colset.
    Fts5Config cfg;
    std::memset(&cfg, 0, sizeof(cfg));

    Fts5Expr expr;
    std::memset(&expr, 0, sizeof(expr));
    const int nPhrase = 2;
    expr.nPhrase = nPhrase;

    // Allocate apExprPhrase array
    Fts5ExprPhrase **ppExpr = new Fts5ExprPhrase*[nPhrase];
    for (int i = 0; i < nPhrase; ++i) {
        Fts5ExprPhrase *pPhrase = new Fts5ExprPhrase();
        pPhrase->pNode = new Fts5ExprNode();
        pPhrase->pNode->pNear = new Fts5ExprNear();
        // Intentionally leave pColset NULL to simulate "no colset"
        pPhrase->pNode->pNear->pColset = NULL;
        ppExpr[i] = pPhrase;
    }
    expr.apExprPhrase = ppExpr;

    // Prepare aPopulator array with bMiss = 0 except for index 0 which we'll flip to 1
    Fts5PoslistPopulator pop[2];
    for (int i = 0; i < nPhrase; ++i) {
        pop[i].bMiss = (i == 0) ? 1 : 0;
        pop[i].bOk = 0;
    }

    // Run the function with iCol = 0
    const char *zExpr = "another test";
    int rc = sqlite3Fts5ExprPopulatePoslists(&cfg, &expr, pop, 0, zExpr, (int)std::strlen(zExpr));

    // Expect rc == 0
    if (rc != 0) {
        LogFail("Return code rc", 0, rc);
    } else {
        LogPass("Return code rc equals 0");
    }

    // The first entry should have bOk == 0 due to bMiss = 1
    if (pop[0].bOk != 0) {
        LogFail("pop[0].bOk == 0 when bMiss == 1", 0, pop[0].bOk);
    } else {
        LogPass("pop[0].bOk == 0 (bMiss true)");
    }

    // The second entry should still be set to 1 (no colset, bMiss == 0)
    if (pop[1].bOk != 1) {
        LogFail("pop[1].bOk == 1 when no colset and bMiss == 0", 1, pop[1].bOk);
    } else {
        LogPass("pop[1].bOk == 1 (no colset, bMiss false)");
    }

    // Cleanup
    for (int i = 0; i < nPhrase; ++i) {
        delete expr.apExprPhrase[i]->pNode->pNear;
        delete expr.apExprPhrase[i]->pNode;
        delete expr.apExprPhrase[i];
    }
    delete[] expr.apExprPhrase;
}

// Entry point for running all tests.
// This function mirrors a lightweight test runner that executes test cases
// in a single process and summarizes results at the end.
static void RunAllTests()
{
    std::cout << "Running sqlite3Fts5ExprPopulatePoslists unit tests..." << std::endl;
    test_populatePoslists_noColset_noMiss();
    test_populatePoslists_withMissFlag();
    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
    }
}

// Provide a standard main function to satisfy environments that expect
// a standalone executable test.
int main()
{
    RunAllTests();
    return g_failures ? 1 : 0;
}