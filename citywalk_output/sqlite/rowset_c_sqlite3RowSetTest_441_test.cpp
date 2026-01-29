// C++11 test suite for sqlite3RowSetTest in rowset.c
// This test harness exercises the focal method by constructing
// minimal RowSet and RowSetEntry structures and verifying both
// true and false branches of the logic. It avoids terminating
// on assertion failures by using non-terminating EXPECT-like checks.

#include <vector>
#include <cstdio>
#include <sqliteInt.h>
#include <rowset.c>
#include <cstdlib>
#include <cstdint>


// Bring the focal method and necessary types into scope.
// We include the C source to ensure the RowSet/RowSetEntry layouts
// and constants (e.g., ROWSET_NEXT, ROWSET_SORTED) match the production code.
// If your build system uses separate compilation units, replace this
// with an appropriate header-based inclusion and linkage.
extern "C" {
}

// Lightweight non-terminating test assertion macros.
// They print failures but do not abort the test execution.
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { fprintf(stderr, "[EXPECT_FAIL] %s\n", (msg)); ++g_failures; } } while(0)

#define EXPECT_EQ_INT(actual, expected, msg) \
  do { if((int)(actual) != (int)(expected)) { fprintf(stderr, "[EXPECT_FAIL] %s: expected %d, got %d\n", (msg), (int)(expected), (int)(actual)); ++g_failures; } } while(0)

#define EXPECT_EQ_LL(actual, expected, msg) \
  do { if((long long)(actual) != (long long)(expected)) { fprintf(stderr, "[EXPECT_FAIL] %s: expected %lld, got %lld\n", (msg), (long long)(expected), (long long)(actual)); ++g_failures; } } while(0)

// Helper: build a singly-linked list of RowSetEntry nodes for pEntry.
// The list uses the pRight pointer as the next in the list and stores
// values in v. The order in the list is preserved by pushing values in order.
static RowSetEntry* buildEntryList(const sqlite3_int64* values, int n) {
    RowSetEntry* head = nullptr;
    for(int i = 0; i < n; ++i){
        RowSetEntry* node = new RowSetEntry;
        node->v = values[i];
        node->pLeft = nullptr;
        node->pRight = head; // next element in list
        head = node;
    }
    return head;
}

// Helper: free a list of RowSetEntry nodes (pEntry list)
static void freeEntryList(RowSetEntry* head) {
    RowSetEntry* cur = head;
    while(cur){
        RowSetEntry* nxt = cur->pRight;
        delete cur;
        cur = nxt;
    }
}

// Helper: free a forest/tree of RowSetEntry nodes
static void freeForest(RowSetEntry* root) {
    if(!root) return;
    freeForest(root->pLeft);
    freeForest(root->pRight);
    delete root;
}

// Build a RowSet instance with a given entry list.
// All fields are initialized to sane defaults for testing.
static RowSet* makeRowSet(sqlite3_int64* values, int n) {
    RowSet* rs = new RowSet;
    rs->rsFlags = 0;
    rs->iBatch = 0;
    rs->pEntry = buildEntryList(values, n);
    rs->pForest = nullptr;
    rs->pLast = nullptr;
    return rs;
}

// Free a RowSet instance and its internal data structures.
static void freeRowSet(RowSet* rs) {
    if(!rs) return;
    if(rs->pEntry) {
        // pEntry is a list using pRight as the next pointer
        freeEntryList(rs->pEntry);
        rs->pEntry = nullptr;
    }
    if(rs->pForest) {
        freeForest(rs->pForest);
        rs->pForest = nullptr;
    }
    delete rs;
}

// Main: run several focused tests for sqlite3RowSetTest
int main() {
    // Test 1: iBatch changes from 0 -> 1 with a non-empty entry list.
    // Expect a true result when iRowid is present in the forest after sorting/merging.
    {
        sqlite3_int64 vals1[] = {1, 2, 3};
        RowSet* rs = makeRowSet(vals1, 3);

        int res = sqlite3RowSetTest(rs, 1, 2); // iBatch changes to 1; search for 2
        EXPECT_EQ_INT(res, 1, "Test1: value 2 should be found in forest after first batch");

        // Now test a false case in the same forest (value not present)
        int res2 = sqlite3RowSetTest(rs, 1, 5); // same batch, no rebuild
        EXPECT_EQ_INT(res2, 0, "Test1: value 5 should not be found in existing forest");

        freeRowSet(rs);
    }

    // Test 2: iBatch changes again with a fresh entry list including new values.
    // This tests the rebuild path when iBatch toggles and pEntry is non-null again.
    {
        sqlite3_int64 vals2a[] = {4, 5};
        RowSet* rs2 = makeRowSet(vals2a, 2);

        int res = sqlite3RowSetTest(rs2, 1, 5); // first batch with 4,5 -> should find 5
        EXPECT_EQ_INT(res, 1, "Test2: value 5 should be found after first batch with 4,5");

        // Change to a new batch with new values (simulate iBatch change)
        // Provide new values for pEntry so forest must be rebuilt
        sqlite3_int64 vals2b[] = {6, 7};
        // Overwrite pEntry with a new list
        freeEntryList(rs2->pEntry);
        rs2->pEntry = buildEntryList(vals2b, 2);

        int res2b = sqlite3RowSetTest(rs2, 2, 7); // rebuild forest for batch 2; search for 7
        EXPECT_EQ_INT(res2b, 1, "Test2: value 7 should be found after batch 2 rebuild");

        // Also test a value not present after rebuild
        int res2c = sqlite3RowSetTest(rs2, 2, 99);
        EXPECT_EQ_INT(res2c, 0, "Test2: value 99 should not be found after batch 2 rebuild");

        freeRowSet(rs2);
    }

    // Test 3: Edge case where pEntry is NULL (no new entries) but forest may exist.
    // We expect false when forest is initially empty.
    {
        RowSet* rs3 = new RowSet;
        rs3->rsFlags = 0;
        rs3->iBatch = 0;
        rs3->pEntry = nullptr; // no entries
        rs3->pForest = nullptr;
        rs3->pLast = nullptr;

        int res = sqlite3RowSetTest(rs3, 1, 1); // batch 1, no entries, forest empty
        EXPECT_EQ_INT(res, 0, "Test3: no entries and empty forest should yield 0");
        freeRowSet(rs3); // safe to free (rs3 allocated, internal lists NULL)
    }

    // Summary
    if(g_failures == 0) {
        printf("All tests passed (non-terminating assertions).\n");
    } else {
        printf("Total failures: %d\n", g_failures);
    }

    return g_failures;
}