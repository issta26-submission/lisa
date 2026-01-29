// C++11 test suite for sqlite3Fts3EvalPhrasePoslist
// Focuses on exercising key branches of the focal method with minimal, safe setups.
// This test suite does not use Google Test; it uses simple main() execution with lightweight expectations.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <cstdint>


extern "C" {
}

// Lightweight test assertions (non-terminating) to maximize code execution.
// If a condition fails, we print a message but continue running other tests.
#define EXPECT_TRUE(x, msg) do { if(!(x)) fprintf(stderr, "EXPECT_TRUE FAILED: %s (line %d): %s\n", __FILE__, __LINE__, (msg)); else printf("PASS: %s\n", (msg)); } while(0)
#define EXPECT_EQ(a,b, msg) do { if((a)!=(b)) fprintf(stderr, "EXPECT_EQ FAILED: %s (line %d): %s\n", __FILE__, __LINE__, (msg)); else printf("PASS: %s\n", (msg)); } while(0)

////////////////////////////////////////////////////////////////////////////////
// Helper: zero-initialization for safety in tests
template <typename T>
static void zero(T &x) { std::memset(&x, 0, sizeof(T)); }

////////////////////////////////////////////////////////////////////////////////
// Test 1: Early return path when the phrase targets a different column than iCol
// Scenario:
// - pPhrase->iColumn < pTab->nColumn
// - pPhrase->iColumn != iCol
// Expected: sqlite3Fts3EvalPhrasePoslist returns SQLITE_OK and *ppOut remains 0
////////////////////////////////////////////////////////////////////////////////
static void test_case_early_return_diff_column() {
    // Prepare minimal Fts3Table
    Fts3Table tab;
    zero(tab);
    tab.nColumn = 3;      // enough columns
    tab.bDescIdx = 0;     // not used in this path

    // Prepare Fts3Cursor with base.pVtab pointing to our Fts3Table
    Fts3Cursor csr;
    zero(csr);
    csr.base.pVtab = (sqlite3_vtab*)&tab; // cast to satisfy pointer type in code
    csr.iPrevId = 0;       // doesn't matter for this path

    // Prepare expression and phrase such that iColumn != iCol triggers early return
    Fts3Expr expr;
    zero(expr);
    Fts3Phrase phrase;
    zero(phrase);
    // Phrase targets column 1; iCol we will set to 2 (different)
    phrase.iColumn = 1;

    // Link into expression
    expr.pPhrase = &phrase;
    expr.iDocid = 42;
    expr.bEof = 0;

    // pPhrase.doclist is irrelevant for this early-return path (the condition triggers before doclist usage)
    phrase.doclist.pList = nullptr;

    // Call the focal method with a different iCol
    char *pOut = (char*)0;
    int rc = sqlite3Fts3EvalPhrasePoslist(&csr, &expr, /*iCol=*/2, &pOut);

    // Validate: RC is SQLITE_OK and no output pointer was produced
    EXPECT_TRUE(rc == SQLITE_OK, "test_case_early_return_diff_column: rc == SQLITE_OK");
    EXPECT_TRUE(pOut == nullptr, "test_case_early_return_diff_column: ppOut == nullptr");
}

////////////////////////////////////////////////////////////////////////////////
// Test 2: Path where we skip the large OR/NEAR handling and obtain a direct poslist pointer.
// Scenario:
// - iCol matches pPhrase->iColumn (so the early return condition does not trigger)
// - iDocid == pCsr->iPrevId and pExpr->bEof == false (so we skip the heavy path)
// - pPhrase.doclist.pList is non-null and does not start with 0x01
// Expected: Function returns SQLITE_OK and *ppOut points to pPhrase.doclist.pList
////////////////////////////////////////////////////////////////////////////////
static void test_case_direct_poslist_pointer() {
    // Prepare minimal Fts3Table
    Fts3Table tab;
    zero(tab);
    tab.nColumn = 3;
    tab.bDescIdx = 0;

    // Prepare Fts3Cursor
    Fts3Cursor csr;
    zero(csr);
    csr.base.pVtab = (sqlite3_vtab*)&tab;
    csr.iPrevId = 123; // will be overwritten below by b/c iDocid equality
    csr.bDesc = 0;

    // Prepare expression and phrase
    Fts3Expr expr;
    zero(expr);
    Fts3Phrase phrase;
    zero(phrase);
    phrase.iColumn = 0; // iCol will be 0; matches iColumn to avoid early return

    // Create a dummy doclist buffer with a non-terminating first byte (not 0x01)
    unsigned char dummyBuf[1] = { 0x02 }; // first byte != 0x01; also not 0x00
    phrase.doclist.pList = (char*)dummyBuf; // non-null

    // iDocid matches iPrevId to avoid heavy path
    expr.pPhrase = &phrase;
    expr.iDocid = 123;
    expr.bEof = 0;

    // Ensure pPhrase.doclist has content
    Fts3Table *pTabPtr = &tab;
    (void)pTabPtr; // silence unused if needed

    // Call focal method with iCol = 0 (equal to phrase.iColumn)
    char *pOut = nullptr;
    int rc = sqlite3Fts3EvalPhrasePoslist(&csr, &expr, /*iCol=*/0, &pOut);

    // Expect rc SQLITE_OK and pOut pointing to our dummy buffer
    EXPECT_TRUE(rc == SQLITE_OK, "test_case_direct_poslist_pointer: rc == SQLITE_OK");
    EXPECT_TRUE(pOut == (char*)dummyBuf, "test_case_direct_poslist_pointer: ppOut points to doclist buffer");
}

////////////////////////////////////////////////////////////////////////////////
// Test 3: Path where pIter is null (no doclist to process) after skipping heavy path.
// Scenario:
// - iCol matches, iDocid == iPrevId, bEof == false
// - pPhrase.doclist.pList == nullptr
// Expected: Function returns SQLITE_OK with ppOut == nullptr
////////////////////////////////////////////////////////////////////////////////
static void test_case_null_doclist() {
    // Prepare minimal Fts3Table
    Fts3Table tab;
    zero(tab);
    tab.nColumn = 3;
    tab.bDescIdx = 0;

    // Prepare Fts3Cursor
    Fts3Cursor csr;
    zero(csr);
    csr.base.pVtab = (sqlite3_vtab*)&tab;
    csr.iPrevId = 555;

    // Prepare expression and phrase
    Fts3Expr expr;
    zero(expr);
    Fts3Phrase phrase;
    zero(phrase);
    phrase.iColumn = 0;

    // pList is null
    phrase.doclist.pList = nullptr;

    expr.pPhrase = &phrase;
    expr.iDocid = 555;
    expr.bEof = 0;

    char *pOut = (char*)0;
    int rc = sqlite3Fts3EvalPhrasePoslist(&csr, &expr, /*iCol=*/0, &pOut);

    // Expect early return with no output
    EXPECT_TRUE(rc == SQLITE_OK, "test_case_null_doclist: rc == SQLITE_OK");
    EXPECT_TRUE(pOut == nullptr, "test_case_null_doclist: ppOut == nullptr");
}

////////////////////////////////////////////////////////////////////////////////
// Main: run all tests
int main(void) {
    printf("Starting sqlite3Fts3EvalPhrasePoslist unit tests (C++11 harness)\n");

    test_case_early_return_diff_column();
    test_case_direct_poslist_pointer();
    test_case_null_doclist();

    printf("All tests executed.\n");
    return 0;
}