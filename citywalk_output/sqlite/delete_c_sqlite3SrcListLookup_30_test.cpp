/*
  Unit test suite for the focal method sqlite3SrcListLookup (C-style code adapted for C++11 tests)
  - No GTest; custom lightweight assertions used
  - Focuses on the behavior of sqlite3SrcListLookup with dependency stubs
  - Covers true/false branches and both non-null and null tab scenarios
  - Provides explanatory comments for each test case
*/

#include <iostream>
#include <sqliteInt.h>
#include <vector>


// Minimal type definitions to mirror the SQLite-like structures used by sqlite3SrcListLookup
struct Table {
    int nTabRef;
    int id;
    Table(int i = 0) : nTabRef(0), id(i) {}
};

struct Db {};

struct SrcItemFG {
    int notCte;
    int isIndexedBy;
    SrcItemFG() : notCte(0), isIndexedBy(0) {}
};

struct SrcItem {
    Table* pTab;
    SrcItemFG fg;
    SrcItem() : pTab(nullptr) {}
};

struct SrcList {
    SrcItem* a;
    int nSrc;
};

// Parse object used by sqlite3SrcListLookup
struct Parse {
    Db* db;
};

// Global/testing context to control behavior of dependencies
static Table* g_locateReturn = nullptr;          // what sqlite3LocateTableItem will return
static int g_indexedByLookupReturn = 0;          // return value for sqlite3IndexedByLookup
static Db g_dbStorage;                             // dummy Db instance
static Table* g_oldDeletedCapture = nullptr;      // capture a specific deleted table (optional)
static std::vector<Table*> g_deleted;              // records of deleted tables for verification

// Dependency stubs
Table* sqlite3LocateTableItem(Parse* /*pParse*/, int /*iFrom*/, SrcItem* /*pItem*/) {
    // Controlled by g_locateReturn for tests
    return g_locateReturn;
}

void sqlite3DeleteTable(Db* /*db*/, Table* t) {
    // Record any deletion for verification
    g_deleted.push_back(t);
}

int sqlite3IndexedByLookup(Parse* /*pParse*/, SrcItem* /*pItem*/) {
    // Controlled by g_indexedByLookupReturn
    return g_indexedByLookupReturn;
}

// The focal method under test (copied/adapted from the provided source)
Table* sqlite3SrcListLookup(Parse* pParse, SrcList* pSrc){
    SrcItem *pItem = pSrc->a;
    Table *pTab;
    // Note: In real code there is an assert: assert( pItem && pSrc->nSrc>=1 );
    pTab = sqlite3LocateTableItem(pParse, 0, pItem);
    if( pItem->pTab ) sqlite3DeleteTable(pParse->db, pItem->pTab);
    pItem->pTab = pTab;
    pItem->fg.notCte = 1;
    if( pTab ){
        pTab->nTabRef++;
        if( pItem->fg.isIndexedBy && sqlite3IndexedByLookup(pParse, pItem) ){
            pTab = 0;
        }
    }
    return pTab;
}

// Lightweight test harness
static int g_failures = 0;
static void ExpectTrue(bool cond, const char* msg) {
    if (!cond) {
        ++g_failures;
        std::cerr << "Failure: " << msg << std::endl;
    }
}
static void ExpectEqPtr(const void* a, const void* b, const char* msg) {
    if (a != b) {
        ++g_failures;
        std::cerr << "Failure: " << msg << " (addresses differ: " 
                  << a << " vs " << b << ")" << std::endl;
    }
}

// Test 1: Non-null pTab (oldTab) replaced by locateReturn; no isIndexedBy; oldTab deleted; new tab returned
// - Verifies: old tab deletion, pItem->pTab updated, notCte set, nTabRef increment, correct return
void test_case1_basic_lookup_no_index_by_false() {
    g_deleted.clear();
    g_locateReturn = new Table(101);          // new/located tab
    Table* oldTab = new Table(1);              // tab currently referenced by SrcItem
    SrcItem item;
    item.pTab = oldTab;
    item.fg.notCte = 0;
    item.fg.isIndexedBy = 0;                   // not indexed-by path

    SrcList src;
    src.a = &item;
    src.nSrc = 1;

    Parse pParse;
    pParse.db = &g_dbStorage;

    Table* ret = sqlite3SrcListLookup(&pParse, &src);

    ExpectTrue(ret == g_locateReturn, "Test1: return should be locateReturn");
    ExpectTrue(item.pTab == g_locateReturn, "Test1: item.pTab should be updated to locateReturn");
    ExpectTrue(item.fg.notCte == 1, "Test1: notCte should be set to 1");
    ExpectTrue(g_locateReturn->nTabRef == 1, "Test1: located tab nTabRef should be incremented to 1");
    ExpectTrue(g_deleted.size() == 1 && g_deleted[0] == oldTab, "Test1: old tab should be deleted exactly once");
}

// Test 2: pTab non-null; isIndexedBy true and sqlite3IndexedByLookup returns true -> return NULL
// - Verifies: old tab deletion, pItem->pTab updated to locateReturn, nTabRef incremented, return NULL
void test_case2_index_by_lookup_true_returns_null() {
    g_deleted.clear();
    g_locateReturn = new Table(202);          // new/located tab
    Table* oldTab2 = new Table(2);             // previous tab
    SrcItem item2;
    item2.pTab = oldTab2;
    item2.fg.notCte = 0;
    item2.fg.isIndexedBy = 1;                  // indexed-by path

    SrcList src2;
    src2.a = &item2;
    src2.nSrc = 1;

    g_indexedByLookupReturn = 1;                 // sqlite3IndexedByLookup returns true

    Parse pParse;
    pParse.db = &g_dbStorage;

    Table* ret2 = sqlite3SrcListLookup(&pParse, &src2);

    ExpectTrue(ret2 == nullptr, "Test2: return should be NULL due to index-based lookup");
    ExpectTrue(item2.pTab == g_locateReturn, "Test2: item2.pTab should be updated to locateReturn");
    ExpectTrue(item2.fg.notCte == 1, "Test2: notCte should be set to 1");
    ExpectTrue(g_locateReturn->nTabRef == 1, "Test2: located tab nTabRef should be incremented to 1");
    ExpectTrue(g_deleted.size() == 1 && g_deleted[0] == oldTab2, "Test2: old tab2 should be deleted exactly once");
}

// Test 3: locate returns NULL; old tab deleted if present; final return NULL; pItem->pTab NULL
// - Verifies: deletion of old tab, pItem->pTab becomes NULL, notCte set
void test_case3_null_locate_returns_null_and_deletes_old() {
    g_deleted.clear();
    g_locateReturn = nullptr;                  // locate returns NULL
    Table* oldTab3 = new Table(3);

    SrcItem item3;
    item3.pTab = oldTab3;
    item3.fg.notCte = 0;
    item3.fg.isIndexedBy = 0;

    SrcList src3;
    src3.a = &item3;
    src3.nSrc = 1;

    Parse pParse;
    pParse.db = &g_dbStorage;

    Table* ret3 = sqlite3SrcListLookup(&pParse, &src3);

    ExpectTrue(ret3 == nullptr, "Test3: return should be NULL when locate returns NULL");
    ExpectTrue(item3.pTab == nullptr, "Test3: item3.pTab should be NULL after update");
    ExpectTrue(g_deleted.size() == 1 && g_deleted[0] == oldTab3, "Test3: oldTab3 should be deleted exactly once");
}

// Test 4: Old tab is null; locate returns non-null new tab; ensure no deletion occurs
// - Verifies: pItem->pTab updated to locateReturn, nTabRef increment, return equals locateReturn
void test_case4_no_old_tab_no_deletion() {
    g_deleted.clear();
    g_locateReturn = new Table(4);

    SrcItem item4;
    item4.pTab = nullptr;                       // no old tab
    item4.fg.notCte = 0;
    item4.fg.isIndexedBy = 0;

    SrcList src4;
    src4.a = &item4;
    src4.nSrc = 1;

    Parse pParse;
    pParse.db = &g_dbStorage;

    Table* ret4 = sqlite3SrcListLookup(&pParse, &src4);

    ExpectTrue(ret4 == g_locateReturn, "Test4: return should be locateReturn");
    ExpectTrue(item4.pTab == g_locateReturn, "Test4: item4.pTab should be updated to locateReturn");
    ExpectTrue(g_locateReturn->nTabRef == 1, "Test4: located tab nTabRef should be incremented to 1");
    ExpectTrue(g_deleted.empty(), "Test4: no deletion should occur when there is no old tab");
}

// Run all tests and report summary
int main() {
    // Run tests
    test_case1_basic_lookup_no_index_by_false();
    test_case2_index_by_lookup_true_returns_null();
    test_case3_null_locate_returns_null_and_deletes_old();
    test_case4_no_old_tab_no_deletion();

    // Report summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}