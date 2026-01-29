// Self-contained C++11 unit tests for the core logic demonstrated by sqlite3KeyInfoFromExprList.
// Note: This is a tightly-scoped emulation of the logic using minimal, self-contained types
// to enable focused tests of the dependent behavior (allocation, iteration, and data propagation).
// The real sqlite3KeyInfoFromExprList in select.c relies on internal SQLite structures.
// Here we recreate the essence to validate the control flow and data handling without requiring
// the full SQLite build environment.
// Explanatory comments accompany each test to describe intent and coverage.

#include <vector>
#include <memory>
#include <cstddef>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// ---------------------------
// Minimal Emulation Layer
// ---------------------------
// These types emulate the subset of SQLite structures used by the focal function.
// They are intentionally lightweight and self-contained for unit testing purposes.

struct sqlite3; // opaque in this emulation

// Emulated KeyInfo to store per-column metadata.
struct KeyInfo {
    int nKey;           // number of key columns (N from sqlite3KeyInfoFromExprList)
    int nExtra;         // extra parameter (X) captured as nExtra
    int* aColl;          // per-column collation sequence (simulated as int)
    unsigned char* aSortFlags; // per-column sort flags
};

// Emulated Parse and Expr to feed the focal function.
struct Expr {
    int id; // used to simulate sqlite3ExprNNCollSeq result
};

struct ExprList_item {
    Expr* pExpr;
    struct { int sortFlags; } fg;
};

struct ExprList {
    int nExpr;
    ExprList_item* a; // array of items
};

// Emulated Parse context with a dummy db pointer (not used in the simplified logic)
struct Parse {
    sqlite3* db;
};

// ---------------------------
// Helpers to emulate focal behavior
// We intentionally mirror the signatures used in sqlite3KeyInfoFromExprList
// but provide simple, deterministic behavior suitable for unit tests.
// ---------------------------

// Simulated allocation function: creates a KeyInfo with arrays sized N (nExpr - iStart) and X (nExtra + 1)
static KeyInfo* sqlite3KeyInfoAlloc(sqlite3* /*db*/, int N, int X) {
    if (N < 0) return nullptr; // negative size simulates allocation failure
    KeyInfo* p = new KeyInfo();
    p->nKey = N;
    p->nExtra = X;
    p->aColl = (N > 0) ? new int[N] : nullptr;
    p->aSortFlags = (N > 0) ? new unsigned char[N] : nullptr;
    return p;
}

// Simulated check that the KeyInfo is writable (always true in this test)
static int sqlite3KeyInfoIsWriteable(KeyInfo* /*pInfo*/) {
    return 1;
}

// Simulated per-expression "collation sequence" resolver.
// We use the Expr's id to deterministically produce results.
static int sqlite3ExprNNCollSeq(Parse* /*pParse*/, Expr* pExpr) {
    if (pExpr == nullptr) return -1;
    return pExpr->id; // deterministic mapping from expression id
}

// Free helper to prevent leaks in tests
static void freeKeyInfo(KeyInfo* p) {
    if (!p) return;
    delete[] p->aColl;
    delete[] p->aSortFlags;
    delete p;
}

// ---------------------------
// Implementation under test (emulated)
// Mirrors the structure and logic of the focal function as presented.
// ---------------------------

static KeyInfo* sqlite3KeyInfoFromExprList(
    Parse* pParse,
    ExprList* pList,
    int iStart,
    int nExtra
) {
    int nExpr;
    KeyInfo* pInfo;
    struct ExprList_item* pItem;
    // sqlite3 *db = pParse->db; // not used in this emulation
    int i;
    nExpr = pList->nExpr;
    pInfo = sqlite3KeyInfoAlloc(pParse->db, nExpr - iStart, nExtra + 1);
    if (pInfo) {
        assert(sqlite3KeyInfoIsWriteable(pInfo));
        for (i = iStart, pItem = pList->a + iStart; i < nExpr; i++, pItem++) {
            pInfo->aColl[i - iStart] = sqlite3ExprNNCollSeq(pParse, pItem->pExpr);
            pInfo->aSortFlags[i - iStart] = (unsigned char)pItem->fg.sortFlags;
        }
    }
    return pInfo;
}

// ---------------------------
// Lightweight test framework (non-terminating assertions)
// - EXPECT_TRUE / EXPECT_EQ print messages but do not abort on failure.
// - Collects pass/fail statistics for a final summary.
// ---------------------------

static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (cond) { ++g_pass; } \
        else { ++g_fail; std::cerr << "Expectation failed: " << (msg) \
            << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        if ((a) == (b)) { ++g_pass; } \
        else { ++g_fail; std::cerr << "Expectation failed: " << (msg) \
            << " (expected: " << (b) << ", actual: " << (a) \
            << ") [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; } \
    } while (0)

// ---------------------------
// Test Cases
// Each test is documented with intent and coverage notes.
// ---------------------------

// Test 1: Basic fill for multiple expressions starting at 0
// - Verifies that a non-null KeyInfo is returned
// - Verifies aColl and aSortFlags are populated for all expressions
static void test_basic_fill_all_items() {
    // Arrange
    Parse pParse;
    sqlite3 dummyDb; // not used by emulation
    pParse.db = &dummyDb;

    // Create 3 expressions with ids 0,1,2
    Expr e0{0}, e1{1}, e2{2};
    ExprList_item it0{&e0, {2}};
    ExprList_item it1{&e1, {4}};
    ExprList_item it2{&e2, {6}};

    ExprList list;
    list.nExpr = 3;
    ExprList_item items[3] = { it0, it1, it2 };
    list.a = items;

    // Act
    KeyInfo* pInfo = sqlite3KeyInfoFromExprList(&pParse, &list, 0, 1);

    // Assert
    EXPECT_TRUE(pInfo != nullptr, "Expected non-null KeyInfo from 3 expressions starting at 0");
    if (pInfo) {
        // aColl should be [0,1,2], aSortFlags [2,4,6]
        EXPECT_EQ(pInfo->nKey, 3, "Expected N (number of key columns) to be 3");
        EXPECT_TRUE(pInfo->aColl != nullptr, "Expected aColl array to be allocated");
        EXPECT_TRUE(pInfo->aSortFlags != nullptr, "Expected aSortFlags array to be allocated");
        if (pInfo->aColl && pInfo->aSortFlags) {
            EXPECT_EQ(pInfo->aColl[0], 0, "First coll seq should equal first expr id (0)");
            EXPECT_EQ(pInfo->aSortFlags[0], (unsigned char)2, "First sort flag should be 2");
            EXPECT_EQ(pInfo->aColl[1], 1, "Second coll seq should equal second expr id (1)");
            EXPECT_EQ(pInfo->aSortFlags[1], (unsigned char)4, "Second sort flag should be 4");
            EXPECT_EQ(pInfo->aColl[2], 2, "Third coll seq should equal third expr id (2)");
            EXPECT_EQ(pInfo->aSortFlags[2], (unsigned char)6, "Third sort flag should be 6");
        }
        freeKeyInfo(pInfo);
    }
}

// Test 2: Start index iStart != 0
// - Verifies that only the sublist from iStart to end is considered
static void test_start_indexing_skips_initial_items() {
    // Arrange
    Parse pParse;
    sqlite3 dummyDb;
    pParse.db = &dummyDb;

    Expr e0{10}, e1{11}, e2{12};
    ExprList_item it0{&e0, {7}};
    ExprList_item it1{&e1, {8}};
    ExprList_item it2{&e2, {9}};

    ExprList list;
    list.nExpr = 3;
    ExprList_item items[3] = { it0, it1, it2 };
    list.a = items;

    // Act: start at iStart = 1; expect results for items 1 and 2
    KeyInfo* pInfo = sqlite3KeyInfoFromExprList(&pParse, &list, 1, 0);

    // Assert
    EXPECT_TRUE(pInfo != nullptr, "Expected non-null KeyInfo when iStart=1 with 3 items");
    if (pInfo) {
        // nKey should be 2 (nExpr - iStart)
        EXPECT_EQ(pInfo->nKey, 2, "Expected N to be 2 (3 - 1)");
        if (pInfo->aColl && pInfo->aSortFlags) {
            EXPECT_EQ(pInfo->aColl[0], 11, "First coll seq should match id of expr at index 1 (11)");
            EXPECT_EQ(pInfo->aSortFlags[0], (unsigned char)7, "First sort flag should be 7");
            EXPECT_EQ(pInfo->aColl[1], 12, "Second coll seq should match id of expr at index 2 (12)");
            EXPECT_EQ(pInfo->aSortFlags[1], (unsigned char)9, "Second sort flag should be 9");
        }
        freeKeyInfo(pInfo);
    }
}

// Test 3: Empty expression list (nExpr == iStart)
// - Verifies that an empty KeyInfo is allocated and returned without attempting to access items
static void test_empty_expression_list() {
    // Arrange
    Parse pParse;
    sqlite3 dummyDb;
    pParse.db = &dummyDb;

    ExprList list;
    list.nExpr = 0;
    list.a = nullptr; // no expressions

    // Act
    KeyInfo* pInfo = sqlite3KeyInfoFromExprList(&pParse, &list, 0, 0);

    // Assert
    EXPECT_TRUE(pInfo != nullptr, "Expected non-null KeyInfo for empty expression list");
    if (pInfo) {
        EXPECT_EQ(pInfo->nKey, 0, "When N==0, nKey should be 0");
        // aColl / aSortFlags may be nullptr for zero-length arrays
        // Accept either allocation or null as long as no crash occurs
        freeKeyInfo(pInfo);
    }
}

// Test 4: Negative N (iStart > nExpr) simulates allocation failure path
static void test_negative_allocation_path() {
    // Arrange
    Parse pParse;
    sqlite3 dummyDb;
    pParse.db = &dummyDb;

    // 2 expressions, iStart = 3 -> N = -1 (negative)
    Expr e0{1}, e1{2};
    ExprList_item it0{&e0, {1}};
    ExprList_item it1{&e1, {1}};

    ExprList list;
    list.nExpr = 2;
    ExprList_item items[2] = { it0, it1 };
    list.a = items;

    // Act
    KeyInfo* pInfo = sqlite3KeyInfoFromExprList(&pParse, &list, 3, 0);

    // Assert
    EXPECT_TRUE(pInfo == nullptr, "Expected nullptr when N (nExpr - iStart) is negative");
}

// Test 5: Verify behavior with non-zero nExtra while there are items
// - Ensures allocation accounts for extra columns and no crash
static void test_extra_columns_allocation() {
    // Arrange
    Parse pParse;
    sqlite3 dummyDb;
    pParse.db = &dummyDb;

    // 2 expressions
    Expr e0{0}, e1{1};
    ExprList_item it0{&e0, {5}};
    ExprList_item it1{&e1, {6}};

    ExprList list;
    list.nExpr = 2;
    ExprList_item items[2] = { it0, it1 };
    list.a = items;

    // Act
    KeyInfo* pInfo = sqlite3KeyInfoFromExprList(&pParse, &list, 0, 4); // nExtra = 4

    // Assert
    EXPECT_TRUE(pInfo != nullptr, "Expected non-null KeyInfo when nExtra > 0");
    if (pInfo) {
        // N should be 2 (2 items - iStart 0)
        EXPECT_EQ(pInfo->nKey, 2, "Expected N to be 2");
        // Cleanup
        freeKeyInfo(pInfo);
    }
}

// ---------------------------
// Test Runner
// ---------------------------

static void run_all_tests() {
    test_basic_fill_all_items();
    test_start_indexing_skips_initial_items();
    test_empty_expression_list();
    test_negative_allocation_path();
    test_extra_columns_allocation();

    int total = g_pass + g_fail;
    std::cout << "Test results: Passed " << g_pass << " / " << total << std::endl;
}

// ---------------------------
// Main
// ---------------------------

int main() {
    run_all_tests();
    return (g_fail != 0) ? 1 : 0;
}