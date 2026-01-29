// Unit test suite (C++11) for the focal method sqlite3RowSetInsert
// This test is self-contained and reproduces the core data structures
// and behavior expected by sqlite3RowSetInsert to enable thorough local testing
// without requiring the full SQLite build environment.
//
// Notes:
// - This test mirrors the logic of the focal method, including the RowSet and
//   RowSetEntry structures, the ROWSET_NEXT and ROWSET_SORTED flags, and the
//   allocation hook rowSetEntryAlloc.
// - The test uses a small, non-terminating assertion framework so tests
//   continue executing after a failure and report all issues at the end.
// - The implementation is designed to be compilable with a C++11 compiler.
// - It is a self-contained reproduction to verify the algorithmic behavior of
//   sqlite3RowSetInsert, focusing on true/false branches and pointer/linkage
//   correctness.

#include <vector>
#include <iomanip>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain knowledge / Candidate Keywords (for traceability)
// - RowSet, RowSetEntry
// - ROWSET_NEXT, ROWSET_SORTED
// - rsFlags, pEntry, pLast, pRight
// - v (row value), rowid
// - rowSetEntryAlloc (allocation hook)
// - sqlite3RowSetInsert (core focal method under test)
// - Linked list semantics: first entry (pEntry), last pointer (pLast), right-link (pRight)

using namespace std;

// Typedefs and constants mirroring the focal code
typedef long long i64;

static const int ROWSET_NEXT  = 1 << 0; // 0x01
static const int ROWSET_SORTED = 1 << 1; // 0x02

// Forward declarations mirroring expected object layout
struct RowSetEntry;
struct RowSet;

// RowSetEntry: element node in the linked list
struct RowSetEntry {
    i64 v;                 // stored rowid
    RowSetEntry *pRight;   // pointer to the next entry to the right (forward link)
};

// RowSet: container holding the list of RowSetEntry nodes
struct RowSet {
    int rsFlags;             // bit flags (e.g., ROWSET_NEXT, ROWSET_SORTED)
    RowSetEntry *pEntry;      // head of the list
    RowSetEntry *pLast;       // tail (last entry)
};

// Allocation hook used by sqlite3RowSetInsert
static struct RowSetEntry *rowSetEntryAlloc(RowSet *p){
    (void)p;
    // Simple allocation; in real code this could fail, but for unit tests we keep it reliable
    return new RowSetEntry{0, nullptr};
}

// Focal method under test (reproduced locally for unit testing)
// void sqlite3RowSetInsert(RowSet *p, i64 rowid){
void sqlite3RowSetInsert(RowSet *p, i64 rowid){
    struct RowSetEntry *pEntry;  /* The new entry */
    struct RowSetEntry *pLast;   /* The last prior entry */
    /* This routine is never called after sqlite3RowSetNext() */
    assert( p!=0 && (p->rsFlags & ROWSET_NEXT)==0 );
    pEntry = rowSetEntryAlloc(p);
    if( pEntry==0 ) return;
    pEntry->v = rowid;
    pEntry->pRight = 0;
    pLast = p->pLast;
    if( pLast ){
        if( rowid<=pLast->v ){  /*OPTIMIZATION-IF-FALSE*/
            /* Avoid unnecessary sorts by preserving the ROWSET_SORTED flags
            ** where possible */
            p->rsFlags &= ~ROWSET_SORTED;
        }
        pLast->pRight = pEntry;
    }else{
        p->pEntry = pEntry;
    }
    p->pLast = pEntry;
}

// Tiny test framework (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TEST_ASSERT(cond, message)                                   \
    do {                                                              \
        ++g_totalTests;                                               \
        if(!(cond)) {                                                 \
            ++g_failedTests;                                          \
            std::cerr << "Test failed: " << (message)                \
                      << " (" << __FILE__ << ":" << __LINE__ << ")"  \
                      << std::endl;                                   \
        }                                                           \
    } while(0)

static void freeRowSet(RowSet *p){
    // Free all allocated RowSetEntry nodes
    RowSetEntry *cur = p ? p->pEntry : nullptr;
    while(cur){
        RowSetEntry *next = cur->pRight;
        delete cur;
        cur = next;
    }
    if(p){
        p->pEntry = nullptr;
        p->pLast = nullptr;
    }
}

// Utility: build a string representation of the current list values for debugging
static std::string listValues(const RowSet *p){
    std::string s = "[";
    const RowSetEntry *cur = p ? p->pEntry : nullptr;
    while(cur){
        s += std::to_string(cur->v);
        cur = cur->pRight;
        if(cur) s += ", ";
    }
    s += "]";
    return s;
}

// Test 1: Inserting into an empty set initializes the list correctly
// - Expect: head and last point to the new entry; value equals rowid; pRight is null
// - Expect: rsFlags unchanged from initial value (we start with ROWSET_SORTED to test preservation)
static void test_insert_into_empty_set(){
    RowSet rs;
    rs.rsFlags = ROWSET_SORTED;
    rs.pEntry = nullptr;
    rs.pLast = nullptr;

    sqlite3RowSetInsert(&rs, 5);

    TEST_ASSERT(rs.pEntry != nullptr, "First insert should create an entry");
    TEST_ASSERT(rs.pLast == rs.pEntry, "First entry should be both head and last");
    TEST_ASSERT(rs.pEntry->v == 5, "First entry value should be 5");
    TEST_ASSERT(rs.pEntry->pRight == nullptr, "First entry should have no right link");
    TEST_ASSERT(rs.rsFlags == ROWSET_SORTED, "rsFlags should remain ROWSET_SORTED after first insert");
    // Clean up
    freeRowSet(&rs);
}

// Test 2: Appending a larger id should link the new entry to the end without clearing ROWSET_SORTED
static void test_insert_append_larger_preserves_sorted(){
    RowSet rs;
    rs.rsFlags = ROWSET_SORTED;
    rs.pEntry = nullptr;
    rs.pLast = nullptr;

    sqlite3RowSetInsert(&rs, 5);  // first
    sqlite3RowSetInsert(&rs, 10); // second (larger)

    // Validate list: 5 -> 10
    TEST_ASSERT(rs.pEntry != nullptr, "List should have a head after two inserts");
    TEST_ASSERT(rs.pEntry->v == 5, "Head value should be 5");
    TEST_ASSERT(rs.pLast != nullptr, "Last should not be null after two inserts");
    TEST_ASSERT(rs.pLast->v == 10, "Last value should be 10");
    TEST_ASSERT(rs.pEntry->pRight != nullptr, "Head should link to second entry");
    TEST_ASSERT(rs.pEntry->pRight == rs.pLast, "Head's right should be the second entry");
    TEST_ASSERT(rs.pLast->pRight == nullptr, "Second entry should have no right link");
    TEST_ASSERT(rs.rsFlags == ROWSET_SORTED, "rsFlags should remain ROWSET_SORTED when new value is larger than last");
    // Clean up
    freeRowSet(&rs);
}

// Test 3: Inserting a smaller/equal id should trigger ROWSET_SORTED clearing and append
static void test_insert_smaller_triggers_unsorted_clear(){
    RowSet rs;
    rs.rsFlags = ROWSET_SORTED;
    rs.pEntry = nullptr;
    rs.pLast = nullptr;

    sqlite3RowSetInsert(&rs, 5);  // first
    sqlite3RowSetInsert(&rs, 10); // second (larger)
    sqlite3RowSetInsert(&rs, 3);  // third (smaller/equal to last)

    // Validate order: 5 -> 10 -> 3
    TEST_ASSERT(rs.pEntry != nullptr, "List should exist after inserts");
    TEST_ASSERT(rs.pEntry->v == 5, "Head should be 5");
    TEST_ASSERT(rs.pEntry->pRight != nullptr, "Head should link to next entry");
    TEST_ASSERT(rs.pEntry->pRight->v == 10, "Second entry should be 10");
    TEST_ASSERT(rs.pLast != nullptr, "There should be a last pointer");
    TEST_ASSERT(rs.pLast->v == 3, "Last value should be 3");
    TEST_ASSERT(rs.pLast->pRight == nullptr, "Last entry should have no right link");
    TEST_ASSERT(rs.pEntry->pRight->pRight != nullptr, "There should be a third entry in middle");
    TEST_ASSERT(rs.rsFlags == 0, "rsFlags should be cleared (ROWSET_SORTED removed) after inserting a smaller/equal value");

    // Also check the actual sequence of values for completeness: 5, 10, 3
    std::string seq = listValues(&rs);
    TEST_ASSERT(seq == "[5, 10, 3]", ("List sequence should be [5, 10, 3], got " + seq).c_str());

    // Clean up
    freeRowSet(&rs);
}

// Test 4: Ensure the false-branch of the optimization (rowid > last->v) preserves ROWSET_SORTED
static void test_insert_false_branch_preserves_sorted(){
    RowSet rs;
    rs.rsFlags = ROWSET_SORTED;
    rs.pEntry = nullptr;
    rs.pLast = nullptr;

    sqlite3RowSetInsert(&rs, 5);  // first
    sqlite3RowSetInsert(&rs, 6);  // second (greater than last)

    // Expect: no clearing of ROWSET_SORTED
    TEST_ASSERT(rs.rsFlags == ROWSET_SORTED, "rsFlags should remain ROWSET_SORTED when new rowid is greater than last->v");
    TEST_ASSERT(rs.pEntry->v == 5, "Head should be 5");
    TEST_ASSERT(rs.pLast->v == 6, "Last should be 6");
    TEST_ASSERT(rs.pEntry->pRight != nullptr, "Head should link to second entry");
    TEST_ASSERT(rs.pLast->pRight == nullptr, "Second entry should have no right link");

    // Clean up
    freeRowSet(&rs);
}

// Entry point
int main(){
    cout << "sqlite3RowSetInsert unit tests (self-contained) - C++11\n";

    test_insert_into_empty_set();
    test_insert_append_larger_preserves_sorted();
    test_insert_smaller_triggers_unsorted_clear();
    test_insert_false_branch_preserves_sorted();

    cout << "=========================================\n";
    cout << "Total tests run: " << g_totalTests << "\n";
    cout << "Total failures: " << g_failedTests << "\n";

    if(g_failedTests > 0){
        cout << "Some tests FAILED. Please review the error messages above." << endl;
        return 1;
    }
    cout << "All tests PASSED." << endl;
    return 0;
}