#include <iostream>
#include <sqliteInt.h>
#include <cassert>
#include <string>


// Lightweight, self-contained stubs to exercise sqlite3SrcListAssignCursors
// without requiring the full SQLite codebase. This is a minimal, compilable
// C++11 test harness that mirrors the relevant data structures used by the
// focal method.
//
// Domain guidance followed:
// - Provide true/false coverage by constructing scenarios that exercise both
//   branches of conditional logic and recursion.
// - Avoid private members; use public-like struct members in test harness.
// - Use non-terminating assertions implemented by the test harness (no GTest).
// - Add explanatory comments for each unit test.

// Candidate keywords extracted from the focal method context:
// Parse, SrcList, SrcItem, pList, iCursor, pItem, pSelect, SrcListAssignCursors
// nTab, mallocFailed, ALWAYS, recursion

// Domain: Always evaluate non-null pointers as true, null as false.
#ifndef ALWAYS
#define ALWAYS(x) ((x) ? 1 : 0)
#endif

// Forward declarations to emulate the minimal SQLite-like types used by the function.
struct SrcList;
struct SrcItem;
struct Select;
struct Parse;

// Minimal database status holder
struct Db {
    int mallocFailed;
};

// Minimal Parse structure used by the focal method
struct Parse {
    Db* db;
    int nTab;
};

// Minimal Select wrapper containing a source list
struct Select {
    SrcList* pSrc;
};

// A single source item in a SrcList; it may hold a nested Select
struct SrcItem {
    int iCursor;
    Select* pSelect;
};

// A list of source items; mirrors the usage in the focal method
struct SrcList {
    int nSrc;
    SrcItem* a;
};

// The focal method under test (copied/adapted to this test harness)
void sqlite3SrcListAssignCursors(Parse *pParse, SrcList *pList){
  int i;
  SrcItem *pItem;
  assert( pList || pParse->db->mallocFailed );
  if( ALWAYS(pList) ){
    for(i=0, pItem=pList->a; i<pList->nSrc; i++, pItem++){
      if( pItem->iCursor>=0 ) continue;
      pItem->iCursor = pParse->nTab++;
      if( pItem->pSelect ){
        sqlite3SrcListAssignCursors(pParse, pItem->pSelect->pSrc);
      }
    }
  }
}

// Simple test harness helpers

static int g_failCount = 0;

#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failCount; \
    } \
} while(0)

// Helpers to allocate/free test data

static SrcList* makeSrcList(int n) {
    SrcList* lst = new SrcList();
    lst->nSrc = n;
    lst->a = new SrcItem[n];
    for(int i=0;i<n;i++) {
        lst->a[i].iCursor = -1;
        lst->a[i].pSelect = nullptr;
    }
    return lst;
}

static void freeSrcList(SrcList* p) {
    if(!p) return;
    for(int i = 0; i < p->nSrc; ++i) {
        SrcItem &it = p->a[i];
        if(it.pSelect) {
            if(it.pSelect->pSrc) {
                freeSrcList(it.pSelect->pSrc);
            }
            delete it.pSelect;
        }
    }
    delete[] p->a;
    delete p;
}

// Test 1: Basic non-nested assignment
// - nTab starts at 5
// - pList has 3 items; items 0 and 2 are -1 (to be assigned), item 1 already assigned (>=0)
// - No nested selects present
// Expected: item0.iCursor = 5, item1 remains 3, item2 = 6; nTab ends at 7
static void test_basic_assignment() {
    // Setup
    Parse p;
    Db d;
    d.mallocFailed = 0;
    p.db = &d;
    p.nTab = 5;

    SrcList* pList = makeSrcList(3);
    // item0: -1, no select
    pList->a[0].iCursor = -1;
    pList->a[0].pSelect = nullptr;

    // item1: already assigned
    pList->a[1].iCursor = 3;
    pList->a[1].pSelect = nullptr;

    // item2: -1, no select
    pList->a[2].iCursor = -1;
    pList->a[2].pSelect = nullptr;

    // Act
    sqlite3SrcListAssignCursors(&p, pList);

    // Assert
    TEST_ASSERT(pList->a[0].iCursor == 5, "test1: first item should be assigned cursor 5");
    TEST_ASSERT(pList->a[1].iCursor == 3, "test1: second item should remain 3");
    TEST_ASSERT(pList->a[2].iCursor == 6, "test1: third item should be assigned cursor 6");
    TEST_ASSERT(p.nTab == 7, "test1: nTab should be incremented to 7");

    // Cleanup
    freeSrcList(pList);
}

// Test 2: Nested SrcList recursion
// - Outer list has one item that references a nested SrcList via pSelect->pSrc
// - Outer iCursor starts at -1 and becomes 10
// - Inner SrcList has two items both -1; they become 11 and 12 respectively
// - nTab ends at 13
static void test_nested_assignment() {
    // Setup
    Parse p;
    Db d;
    d.mallocFailed = 0;
    p.db = &d;
    p.nTab = 10;

    // Outer list with 1 item pointing to a nested list
    SrcList* outer = makeSrcList(1);
    outer->a[0].iCursor = -1;
    outer->a[0].pSelect = new Select();
    outer->a[0].pSelect->pSrc = makeSrcList(2);

    // Initialize inner list (two items -1)
    outer->a[0].pSelect->pSrc->a[0].iCursor = -1;
    outer->a[0].pSelect->pSrc->a[1].iCursor = -1;

    // Act
    sqlite3SrcListAssignCursors(&p, outer);

    // Assert
    TEST_ASSERT(outer->a[0].iCursor == 10, "test2: outer item cursor should be 10");
    TEST_ASSERT(outer->a[0].pSelect->pSrc->a[0].iCursor == 11, "test2: inner item0 cursor should be 11");
    TEST_ASSERT(outer->a[0].pSelect->pSrc->a[1].iCursor == 12, "test2: inner item1 cursor should be 12");
    TEST_ASSERT(p.nTab == 13, "test2: nTab should be incremented to 13");

    // Cleanup
    // Free inner first
    freeSrcList(outer->a[0].pSelect->pSrc);
    delete outer->a[0].pSelect;
    freeSrcList(outer);
}

// Test 3: Skip recursion when iCursor is already non-negative
// - Outer list has one item with iCursor >= 0 and a non-null pSelect
// - Recursion should not occur, inner list remains untouched
static void test_skip_when_cursor_set() {
    // Setup
    Parse p;
    Db d;
    d.mallocFailed = 0;
    p.db = &d;
    p.nTab = 7;

    SrcList* outer = makeSrcList(1);
    outer->a[0].iCursor = 0; // already set, should skip
    outer->a[0].pSelect = new Select();
    outer->a[0].pSelect->pSrc = makeSrcList(2);

    // Initialize inner list to ensure it would be changed if recursion happened
    outer->a[0].pSelect->pSrc->a[0].iCursor = -1;
    outer->a[0].pSelect->pSrc->a[1].iCursor = -1;

    // Act
    sqlite3SrcListAssignCursors(&p, outer);

    // Assert
    TEST_ASSERT(outer->a[0].iCursor == 0, "test3: outer item cursor should remain 0");
    // Inner list should remain untouched due to early skip
    TEST_ASSERT(outer->a[0].pSelect->pSrc->a[0].iCursor == -1, "test3: inner item0 should remain -1");
    TEST_ASSERT(outer->a[0].pSelect->pSrc->a[1].iCursor == -1, "test3: inner item1 should remain -1");

    // Cleanup
    freeSrcList(outer->a[0].pSelect->pSrc);
    delete outer->a[0].pSelect;
    freeSrcList(outer);
}

// Test 4: Null SrcList with mallocFailed flag set should not crash
// - pList is null but pParse.db mallocFailed is true
// - The function should simply return without dereferencing a null pointer
static void test_null_list_with_malloc_failed() {
    // Setup
    Parse p;
    Db d;
    d.mallocFailed = 1; // triggers the assert path to be satisfied
    p.db = &d;
    p.nTab = 9;

    SrcList* pList = nullptr;

    // Act
    sqlite3SrcListAssignCursors(&p, pList);

    // Assert: nothing to check beyond ensuring no crash; use a benign assertion
    TEST_ASSERT(true, "test4: null pList with mallocFailed should not crash");
}

// Runner
static void runAllTests() {
    test_basic_assignment();
    test_nested_assignment();
    test_skip_when_cursor_set();
    test_null_list_with_malloc_failed();

    if(g_failCount == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << g_failCount << " test(s) failed.\n";
    }
}

int main() {
    runAllTests();
    return (g_failCount == 0) ? 0 : 1;
}