// Lightweight C++11 unit tests for sqlite3IndexHasDuplicateRootPage
// This test suite creates minimal in-memory structures that mimic the
// sqlite3 internal Index/Table layout required by sqlite3IndexHasDuplicateRootPage.
// It uses a small, non-terminating test harness (no Google Test) and prints
// a summary at the end.
//
// Important: This harness declares the necessary types (Index, Table) loosely
// to be compatible with the function under test. It does not depend on the
// full SQLite sources and keeps memory management explicit.
//
// The tests cover true/false branches for the predicate inside sqlite3IndexHasDuplicateRootPage.

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// Step 1: Provide a minimal C-style declaration of the function under test.
// We declare the function with the same signature as it appears in the focal file.
// The real implementation uses the internal sqlite3 Index/Table structures; we
// provide compatible forward declarations here for the test harness.

extern "C" {
    // Forward declaration to match the function's parameter type
    struct Index;
    int sqlite3IndexHasDuplicateRootPage(struct Index *pIndex);
}

// Step 2: Local minimal type definitions to build test scenarios.
// We intentionally mirror only the fields used by sqlite3IndexHasDuplicateRootPage:
// - tnum (root page number for the index)
// - pNext (linked list of indices on the table)
// - pTable (pointer to the containing Table which holds the head pIndex)
struct Table {
    struct Index* pIndex;
};

// Forward declaration of Index is provided above; define its fields here.
struct Index {
    int tnum;
    struct Index* pNext;
    struct Table* pTable;
};

// Lightweight test harness state
static int g_total = 0;
static int g_failed = 0;

// Simple non-terminating assertion macro
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAIL] " << msg << std::endl; \
        ++g_failed; \
    } \
    ++g_total; \
} while (0)

// Helper constructors for building test graphs
static Index* makeIndex(int tnum, Table* tbl) {
    Index* node = new Index;
    node->tnum = tnum;
    node->pNext = nullptr;
    node->pTable = tbl;
    return node;
}

static Table* makeTable(Index* root) {
    Table* tbl = new Table;
    tbl->pIndex = root;
    return tbl;
}

// Helper to clean up a single linked list of Index nodes (memory cleanup)
static void freeIndexChain(Index* root) {
    while (root) {
        Index* nxt = root->pNext;
        delete root;
        root = nxt;
    }
}

// Test Case 1: No duplicates in the index list
// - Graph: Table with root A(tnum=1) -> B(tnum=2)
// - Expect: sqlite3IndexHasDuplicateRootPage returns 0
static void test_no_duplicates() {
    // Explain: Build a small index chain without duplicates.
    Table* tbl = makeTable(nullptr);
    Index* A = makeIndex(1, tbl);
    Index* B = makeIndex(2, tbl);
    A->pNext = B;
    tbl->pIndex = A;

    // Call the focal method
    int res = sqlite3IndexHasDuplicateRootPage(A);

    // Expect no duplicates
    TEST_ASSERT(res == 0, "test_no_duplicates: expected 0 (no duplicates) but got non-zero");
    // Cleanup
    freeIndexChain(A); // frees A and B
    delete tbl;
}

// Test Case 2: A duplicate exists deeper in the list
// - Graph: Table with root A(tnum=1) -> B(tnum=1)
// - Expect: sqlite3IndexHasDuplicateRootPage returns 1
static void test_duplicate_exists_root_first() {
    // Build graph: A (root) and B with same tnum
    Table* tbl = makeTable(nullptr);
    Index* A = makeIndex(1, tbl);
    Index* B = makeIndex(1, tbl);
    A->pNext = B;
    tbl->pIndex = A;

    int res = sqlite3IndexHasDuplicateRootPage(A);

    TEST_ASSERT(res == 1, "test_duplicate_exists_root_first: expected 1 (duplicate found) but got 0");
    freeIndexChain(A);
    delete tbl;
}

// Test Case 3: Duplicate exists at the end of a longer list
// - Graph: Table with root A(tnum=2) -> B(tnum=3) -> C(tnum=2)
// - Expect: sqlite3IndexHasDuplicateRootPage returns 1
static void test_duplicate_exists_at_end() {
    Table* tbl = makeTable(nullptr);
    Index* A = makeIndex(2, tbl);
    Index* B = makeIndex(3, tbl);
    Index* C = makeIndex(2, tbl);
    A->pNext = B;
    B->pNext = C;
    tbl->pIndex = A;

    int res = sqlite3IndexHasDuplicateRootPage(A);

    TEST_ASSERT(res == 1, "test_duplicate_exists_at_end: expected 1 (duplicate at end) but got 0");
    freeIndexChain(A);
    delete tbl;
}

// Test Case 4: All unique values across a longer chain
// - Graph: Table with root A(tnum=10) -> B(tnum=20) -> C(tnum=30) -> D(tnum=40)
// - Expect: sqlite3IndexHasDuplicateRootPage returns 0
static void test_all_unique_long_chain() {
    Table* tbl = makeTable(nullptr);
    Index* A = makeIndex(10, tbl);
    Index* B = makeIndex(20, tbl);
    Index* C = makeIndex(30, tbl);
    Index* D = makeIndex(40, tbl);
    A->pNext = B;
    B->pNext = C;
    C->pNext = D;
    tbl->pIndex = A;

    int res = sqlite3IndexHasDuplicateRootPage(A);

    TEST_ASSERT(res == 0, "test_all_unique_long_chain: expected 0 (all unique) but got non-zero");
    freeIndexChain(A);
    delete tbl;
}

// Entry point: run all tests and report summary
int main() {
    // Run tests
    test_no_duplicates();
    test_duplicate_exists_root_first();
    test_duplicate_exists_at_end();
    test_all_unique_long_chain();

    // Report summary
    std::cout << "Total tests run: " << g_total << std::endl;
    if (g_failed > 0) {
        std::cout << "Tests failed: " << g_failed << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}