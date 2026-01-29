// Unit test suite for sqlite3IndexedByLookup (inlined within this test harness)
// This test harness provides minimal, self-contained implementations of the
// dependencies required by sqlite3IndexedByLookup to exercise its core logic.
// It is designed to compile under C++11 and run without GTest, using simple
// non-terminating assertions to maximize code execution coverage.

#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <iostream>
#include <cassert>


// Domain knowledge: minimal reproductions of the core data structures used by
// sqlite3IndexedByLookup. We implement only the fields accessed by the function.

static const int SQLITE_OK = 0;
static const int SQLITE_ERROR = 1;

// Forward declarations
struct Parse;
struct SrcItem;
struct Table;
struct Index;

// Minimal string-based error reporting shared by test harness
static std::string lastErrorMsg;

// Case-insensitive string compare (SQLite's sqlite3StrICmp simplification)
static int sqlite3StrICmp(const char *zA, const char *zB){
    if (zA == zB) return 0;
    while (*zA && *zB) {
        unsigned char ca = (unsigned char)std::toupper((unsigned char)*zA);
        unsigned char cb = (unsigned char)std::toupper((unsigned char)*zB);
        if (ca != cb) return (int)ca - (int)cb;
        ++zA; ++zB;
    }
    unsigned char ca = (unsigned char)std::toupper((unsigned char)*zA);
    unsigned char cb = (unsigned char)std::toupper((unsigned char)*zB);
    return (int)ca - (int)cb;
}

// Minimal error-reporting function mimicking sqlite3ErrorMsg signature used by focal method
struct Parse {
    int checkSchema;
};

// Index node in a linked list for a Table
struct Index {
    const char *zName;
    Index *pNext;
};

// Table with a linked list of Indexes
struct Table {
    Index *pIndex;
};

// SrcItem's auxiliary structures (subset)
struct SrcItemFg {
    int isIndexedBy;
    int isCte;
};

struct SrcItem {
    Table *pTab;
    SrcItemFg fg;
    union {
        char *zIndexedBy;       // pFrom->u1.zIndexedBy
    } u1;
    union {
        Index *pIBIndex;          // pFrom->u2.pIBIndex
    } u2;
};

// Externally provided function (in real code, it is in select.c). Here we re-create it
// to enable self-contained unit tests without requiring the full SQLite source tree.
static int sqlite3IndexedByLookup(Parse *pParse, SrcItem *pFrom);

// Helper: reset last error message
static void resetLastErrorMsg() { lastErrorMsg.clear(); }

// Helper: simulate sqlite3ErrorMsg
static void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, const char *zIndexedBy, int /*unused*/) {
    // Lightweight formatting: only handle "%s" in the format used by focal method
    char buf[256];
    std::snprintf(buf, sizeof(buf), zFormat, zIndexedBy ? zIndexedBy : "");
    lastErrorMsg = buf;
}

// Implementation of sqlite3IndexedByLookup (self-contained)
static int sqlite3IndexedByLookup(Parse *pParse, SrcItem *pFrom){
    Table *pTab = pFrom->pTab;
    char *zIndexedBy = pFrom->u1.zIndexedBy;
    Index *pIdx;
    assert( pTab!=0 );
    assert( pFrom->fg.isIndexedBy!=0 );
    for(pIdx=pTab->pIndex;
        pIdx && sqlite3StrICmp(pIdx->zName, zIndexedBy);
        pIdx=pIdx->pNext
    );
    if( !pIdx ){
        sqlite3ErrorMsg(pParse, "no such index: %s", zIndexedBy, 0);
        pParse->checkSchema = 1;
        return SQLITE_ERROR;
    }
    assert( pFrom->fg.isCte==0 );
    pFrom->u2.pIBIndex = pIdx;
    return SQLITE_OK;
}

// Simple test harness utilities
static int tests_run = 0;
static int tests_failed = 0;

// Lightweight assertion replacement for tests (non-terminating)
#define TEST_CHECK(cond, msg) \
    do { \
        ++tests_run; \
        if(!(cond)) { \
            ++tests_failed; \
            std::cerr << "Test failed: " << __FUNCTION__ << " - " << msg << std::endl; \
        } \
    } while(0)

// Test 1: Found index by name (case-insensitive match)
static void test_lookup_finds_index_case_insensitive(){
    resetLastErrorMsg();

    // Create an index named "IdxMain"
    Index idxMain; idxMain.zName = "IdxMain"; idxMain.pNext = nullptr;

    // Table with a single index
    Table tab; tab.pIndex = &idxMain;

    // Parse and SrcItem setup
    Parse parse; parse.checkSchema = 0;
    SrcItem src; src.pTab = &tab;
    src.fg.isIndexedBy = 1;
    src.fg.isCte = 0;
    src.u1.zIndexedBy = (char*)"idxmain"; // lowercased; should match "IdxMain"
    src.u2.pIBIndex = nullptr;

    int rc = sqlite3IndexedByLookup(&parse, &src);

    TEST_CHECK(rc == SQLITE_OK, "Expected SQLITE_OK when index exists (case-insensitive match)");
    TEST_CHECK(src.u2.pIBIndex == &idxMain, "Expected pIBIndex to point to the matched index");
    TEST_CHECK(parse.checkSchema == 0, "checkSchema should remain 0 on success");
}

// Test 2: No such index found -> SQLITE_ERROR and error message set
static void test_lookup_not_found_sets_error(){
    resetLastErrorMsg();

    // Create an index named "IdxMain"
    Index idxMain; idxMain.zName = "IdxMain"; idxMain.pNext = nullptr;

    // Table with a single index
    Table tab; tab.pIndex = &idxMain;

    // Parse and SrcItem setup
    Parse parse; parse.checkSchema = 0;
    SrcItem src; src.pTab = &tab;
    src.fg.isIndexedBy = 1;
    src.fg.isCte = 0;
    src.u1.zIndexedBy = (char*)"notexist"; // does not exist in table
    src.u2.pIBIndex = nullptr;

    int rc = sqlite3IndexedByLookup(&parse, &src);

    TEST_CHECK(rc == SQLITE_ERROR, "Expected SQLITE_ERROR when index does not exist");
    TEST_CHECK(parse.checkSchema == 1, "checkSchema should be set to 1 on error");
    TEST_CHECK(lastErrorMsg == "no such index: notexist", "Expected correct error message");
}

// Test 3: Another case-insensitive match with different index name
static void test_lookup_finds_index_case_insensitive_another(){
    resetLastErrorMsg();

    // Create an index named "CaseIndex"
    Index idxCase; idxCase.zName = "CaseIndex"; idxCase.pNext = nullptr;

    // Table with the single index
    Table tab; tab.pIndex = &idxCase;

    // Parse and SrcItem setup
    Parse parse; parse.checkSchema = 0;
    SrcItem src; src.pTab = &tab;
    src.fg.isIndexedBy = 1;
    src.fg.isCte = 0;
    src.u1.zIndexedBy = (char*)"caseindex"; // exact case-insensitive match
    src.u2.pIBIndex = nullptr;

    int rc = sqlite3IndexedByLookup(&parse, &src);

    TEST_CHECK(rc == SQLITE_OK, "Expected SQLITE_OK for case-insensitive match (second test)");
    TEST_CHECK(src.u2.pIBIndex == &idxCase, "Expected pIBIndex to point to idxCase");
    TEST_CHECK(parse.checkSchema == 0, "checkSchema should remain 0 on success");
}

// Entry point
int main(){
    std::cout << "Running sqlite3IndexedByLookup unit tests (self-contained harness aims for full coverage)" << std::endl;

    test_lookup_finds_index_case_insensitive();
    test_lookup_not_found_sets_error();
    test_lookup_finds_index_case_insensitive_another();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
    if(tests_failed == 0){
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    }else{
        std::cout << "Some tests FAILED" << std::endl;
        return 1;
    }
}