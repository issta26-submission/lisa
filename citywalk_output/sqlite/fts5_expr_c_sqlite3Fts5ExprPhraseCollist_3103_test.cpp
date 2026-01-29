// C++11 unit test suite for sqlite3Fts5ExprPhraseCollist
// This test suite is designed to be compiled alongside the sqlite3 FTS5 sources.
// It creates minimal in-memory FTS5 expression structures to exercise
// sqlite3Fts5ExprPhraseCollist in both the "happy path" (no synonym) and
// "no-match" branches. The tests are written without GTest and use a small
// handcrafted test harness that reports failures while continuing execution.

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdint>


// DOMAIN_KNOWLEDGE: Include the project's C headers that define the FTS5 types
// and the function under test. Use extern "C" wrappers to avoid name mangling
// when linking with C source files.
extern "C" {
                       // Fts5Expr, Fts5ExprPhrase, Fts5ExprNode, Fts5ExprTerm,
                       // Fts5IndexIter, Fts5Config, etc., plus constants.
}

// We rely on the SQLITE_OK constant; if not available, provide a fallback.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif

// Simple non-terminating test harness
static int gFailures = 0;

#define TEST_PRINT(...) fprintf(stderr, __VA_ARGS__)
#define TEST_ASSERT(cond, msg) \
    do { if(!(cond)) { ++gFailures; TEST_PRINT("FAIL: %s\n", msg); } } while(0)
#define TEST_ASSERT_EQ(a, b, msg) \
    do { if((a) != (b)) { ++gFailures; TEST_PRINT("FAIL: %s. Expected %ld, got %ld\n", (msg), (long)(b), (long)(a)); } } while(0)
#define TEST_ASSERT_PTR_EQ(a, b, msg) \
    do { if((const void*)(a) != (const void*)(b)) { ++gFailures; TEST_PRINT("FAIL: %s. Pointers differ: %p vs %p\n", (msg), (void*)(a), (void*)(b)); } } while(0)

// Helper to print a test header
static void printHeader(const char* name) {
    TEST_PRINT("=== Test: %s ===\n", name);
}

// Test 1: Basic path without synonym: pNode matches root row, poslist non-empty,
// and the first term has a concrete iterator data (no synonym). Expect to obtain
// collist data from the term's iterator.
static void test_sqlite3Fts5ExprPhraseCollist_basic_no_synonym() {
    printHeader("sqlite3Fts5ExprPhraseCollist_basic_no_synonym");

    // We rely on real types from the FTS5 headers; construct a minimal but valid object graph.

    // Static test data for collist (simulated "column list" data)
    static const unsigned char collData[] = {0xAA, 0xBB, 0xCC, 0xDD};
    const unsigned char *pCollist = nullptr;
    int nCollist = 0;

    // Root node with a specific rowid
    Fts5ExprNode rootNode;
    memset(&rootNode, 0, sizeof(Fts5ExprNode));
    rootNode.iRowid = 42;
    rootNode.bEof = 0;

    // Phrase node that should match the root
    Fts5ExprNode phraseNode;
    memset(&phraseNode, 0, sizeof(Fts5ExprNode));
    phraseNode.iRowid = 42;
    phraseNode.bEof = 0;

    // Iterator for the term: pData and nData must reflect collData
    Fts5IndexIter iter;
    memset(&iter, 0, sizeof(Fts5IndexIter));
    iter.pData = collData; // pointer to data
    iter.nData = (int)(sizeof(collData));

    // Term for the first (and only) term in the phrase
    Fts5ExprTerm term;
    memset(&term, 0, sizeof(Fts5ExprTerm));
    term.pSynonym = NULL; // No synonym to avoid static function path
    term.pIter = &iter;

    // Phrase, with a single term (aTerm[0])
    Fts5ExprPhrase phrase;
    memset(&phrase, 0, sizeof(Fts5ExprPhrase));
    phrase.pNode = &phraseNode;
    phrase.poslist.n = 1; // non-empty poslist

    // The phrase's first term
    // Note: Fts5ExprPhrase uses aTerm[1] as a small flexible array-member;
    // we place our term into that slot.
    phrase.aTerm[0] = term;

    // Expression root/pConfig
    Fts5Config cfg;
    // If the real header defines a specific detail enum, use the macro name
    // as in the focal code: FTS5_DETAIL_COLUMNS
    cfg.eDetail = FTS5_DETAIL_COLUMNS;

    // Fts5Expr structure
    Fts5Expr expr;
    memset(&expr, 0, sizeof(Fts5Expr));
    expr.nPhrase = 1;
    expr.pRoot = &rootNode;
    expr.pConfig = &cfg;

    // apExprPhrase array
    Fts5ExprPhrase *pPhrasePtr = &phrase;
    // For test simplicity allocate the array with one pointer
    Fts5ExprPhrase **ppExprPhrase = (Fts5ExprPhrase **)malloc(sizeof(Fts5ExprPhrase*));
    ppExprPhrase[0] = pPhrasePtr;
    expr.apExprPhrase = ppExprPhrase;

    // Now call the function under test
    const u8 *ppCollist_out = nullptr; // type matches const u8 *
    int pnCollist_out = 0;

    int rc = sqlite3Fts5ExprPhraseCollist(&expr, 0, &ppCollist_out, &pnCollist_out);

    // Assertions
    TEST_ASSERT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK");
    TEST_ASSERT_PTR_EQ(ppCollist_out, collData, "ppCollist_out should point to collData");
    TEST_ASSERT_EQ(pnCollist_out, (int)sizeof(collData), "pnCollist_out should equal data length");

    // Cleanup
    free(ppExprPhrase);
}

// Test 2: No-match path: either bEof != 0, iRowid mismatch, or poslist.n == 0.
// All should yield ppCollist = 0 and pnCollist = 0, rc = SQLITE_OK.
static void test_sqlite3Fts5ExprPhraseCollist_no_match() {
    printHeader("sqlite3Fts5ExprPhraseCollist_no_match");

    // Data for collist (not used in this path)
    static const unsigned char collData[] = {0x11, 0x22, 0x33};
    const unsigned char *pCollist = nullptr;
    int nCollist = 0;

    // Root node with a rowid
    Fts5ExprNode rootNode;
    memset(&rootNode, 0, sizeof(Fts5ExprNode));
    rootNode.iRowid = 999; // mismatch on purpose
    rootNode.bEof = 0;

    // Phrase node that will be non-matching
    Fts5ExprNode phraseNode;
    memset(&phraseNode, 0, sizeof(Fts5ExprNode));
    phraseNode.iRowid = 1;
    phraseNode.bEof = 0;

    // Iterator (data exists but not used due to mismatch)
    Fts5IndexIter iter;
    memset(&iter, 0, sizeof(Fts5IndexIter));
    iter.pData = collData;
    iter.nData = (int)sizeof(collData);

    // Term
    Fts5ExprTerm term;
    memset(&term, 0, sizeof(Fts5ExprTerm));
    term.pSynonym = NULL;
    term.pIter = &iter;

    // Phrase
    Fts5ExprPhrase phrase;
    memset(&phrase, 0, sizeof(Fts5ExprPhrase));
    phrase.pNode = &phraseNode;
    phrase.poslist.n = 0; // Empty poslist to trigger no-match

    phrase.aTerm[0] = term;

    // Config
    Fts5Config cfg;
    cfg.eDetail = FTS5_DETAIL_COLUMNS;

    // Expression
    Fts5Expr expr;
    memset(&expr, 0, sizeof(Fts5Expr));
    expr.nPhrase = 1;
    expr.pRoot = &rootNode;
    expr.pConfig = &cfg;
    Fts5ExprPhrase *pPhrasePtr = &phrase;
    Fts5ExprPhrase **ppExprPhrase = (Fts5ExprPhrase **)malloc(sizeof(Fts5ExprPhrase*));
    ppExprPhrase[0] = pPhrasePtr;
    expr.apExprPhrase = ppExprPhrase;

    const u8 *ppCollist_out = nullptr;
    int pnCollist_out = 0;

    int rc = sqlite3Fts5ExprPhraseCollist(&expr, 0, &ppCollist_out, &pnCollist_out);

    // Assertions
    TEST_ASSERT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK in no-match path");
    TEST_ASSERT_PTR_EQ(ppCollist_out, nullptr, "ppCollist_out should be null in no-match");
    TEST_ASSERT_EQ(pnCollist_out, 0, "pnCollist_out should be 0 in no-match");

    // Cleanup
    free(ppExprPhrase);
}

// Entry point for the test program
int main() {
    // Run the test cases
    test_sqlite3Fts5ExprPhraseCollist_basic_no_synonym();
    test_sqlite3Fts5ExprPhraseCollist_no_match();

    // Summary
    if(gFailures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::printf("Tests completed with %d failure(s).\n", gFailures);
        return 1;
    }
}