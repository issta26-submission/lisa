// Unit tests for sqlite3BtreeCloseCursor(BtCursor *pCur)
// This test suite is written in C++11 without GoogleTest.
// It relies on the repository's SQLite btreeInt.h and the actual sqlite3BtreeCloseCursor
// implementation compiled in the same project (as in the focal method).
// The tests exercise key branches of sqlite3BtreeCloseCursor by constructing minimal
// BtCursor/BtShared/Btree graphs and verifying structural changes after the call.
//
// Notes:
// - We do not rely on exact internal behavior of sqlite3BtreeEnter/Leave/Close;
//   we verify observable state changes (cursor list manipulation and pointer cleanup).
// - We avoid terminating assertions; we log PASS/FAIL for each test.
// - Static helpers inside the actual SQLite code are not mocked; we assume the real
//   implementation is linked in the test executable.
// - All memory management mirrors the usage in the focal method (malloc/free as in the code).

#include <cstdio>
#include <btreeInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// Forward declaration of the focal function (C linkage)
extern "C" int sqlite3BtreeCloseCursor(BtCursor *pCur);

// Simple test harness utility
static void logTest(const char* testName, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << std::endl;
}

// Helper to allocate and zero a BtShared with a given cursor list head
static BtShared* allocBtSharedWithHead(BtCursor* head) {
    BtShared* pBt = (BtShared*)calloc(1, sizeof(BtShared));
    pBt->pCursor = head;
    return pBt;
}

// Helper to allocate a BtCursor with specified fields
static BtCursor* allocBtCursor(BtCursor* pNext, Btree* pBtree, BtShared* pBt) {
    BtCursor* pCur = (BtCursor*)calloc(1, sizeof(BtCursor));
    pCur->pNext = pNext;
    pCur->pBtree = pBtree;
    pCur->pBt = pBt;
    // allocate dummy payloads to exercise frees in the focal method
    pCur->aOverflow = malloc(16);
    pCur->pKey = malloc(16);
    return pCur;
}

// Test 1: When closing a single cursor in BTREE_SINGLE mode, the cursor becomes NULL
// and the BtShared's pCursor advances to 0 because there is no next cursor.
static bool test_close_cursor_single_node_replaces_cursor_with_null() {
    // Setup
    Btree *pBtree = (Btree*)calloc(1, sizeof(Btree));
    pBtree->sharable = 0; // satisfy assertion in focal method for BTREE_SINGLE path

    BtShared *pBt = (BtShared*)calloc(1, sizeof(BtShared));
    BtCursor *pCur = allocBtCursor(nullptr, pBtree, pBt);

    // Chain: single cursor in list
    pBt->pCursor = pCur;
    pBt->openFlags = BTREE_SINGLE;

    // Execute
    sqlite3BtreeCloseCursor(pCur);

    // Verify
    bool ok = (pCur->pBtree == 0) && (pBt->pCursor == nullptr);

    // Cleanup
    // Note: aOverflow and pKey were freed inside sqlite3BtreeCloseCursor;
    // pCur is not owned by the library after close; safe to free here.
    free(pCur);
    free(pBt);
    free(pBtree);

    logTest("test_close_cursor_single_node_replaces_cursor_with_null", ok);
    return ok;
}

// Test 2: Else-branch: remove pCur from a 2-element list where pBt->pCursor points to pPrev
// and pPrev->pNext == pCur. After closing, pPrev->pNext must point to pCur->pNext (nullptr here).
static bool test_close_cursor_else_branch_relinks_previous_node() {
    // Setup
    Btree *pBtree = (Btree*)calloc(1, sizeof(Btree));
    pBtree->sharable = 0;

    BtCursor *pPrev = allocBtCursor(nullptr, pBtree, nullptr);
    BtCursor *pCur  = allocBtCursor(nullptr, pBtree, nullptr);

    // Create a two-node list: pBt->pCursor -> pPrev -> pCur
    BtShared *pBt = allocBtSharedWithHead(pPrev);
    pPrev->pNext = pCur;
    // pBt->pCursor already points to pPrev

    // Ensure the header fields point to the BtShared
    pCur->pBt = pBt;
    pCur->pBtree = pBtree;
    pPrev->pBt = pBt;
    pPrev->pBtree = pBtree;
    pBt->openFlags = 0; // not BTREE_SINGLE

    // Execute
    sqlite3BtreeCloseCursor(pCur);

    // Verify
    bool ok = (pPrev->pNext == nullptr) && (pCur->pBtree == 0) && (pBt->pCursor == pPrev);

    // Cleanup
    free(pCur); free(pPrev); free(pBt); free(pBtree);

    logTest("test_close_cursor_else_branch_relinks_previous_node", ok);
    return ok;
}

// Test 3: Else-branch with longer chain: pBt->pCursor -> pPrev1 -> pPrev2 -> pCur
// Ensure the code traverses until finding the correct pPrev2 and relinks it.
static bool test_close_cursor_else_branch_traverses_chain() {
    // Setup
    Btree *pBtree = (Btree*)calloc(1, sizeof(Btree));
    pBtree->sharable = 0;

    BtCursor *pPrev2 = allocBtCursor(nullptr, pBtree, nullptr);
    BtCursor *pPrev1 = allocBtCursor(pPrev2, pBtree, nullptr);
    BtCursor *pCur   = allocBtCursor(nullptr, pBtree, nullptr);

    BtShared *pBt = allocBtSharedWithHead(pPrev1);
    // Chain: pBt->pCursor -> pPrev1 -> pPrev2 -> pCur
    pPrev1->pNext = pPrev2;
    pPrev2->pNext = pCur;

    // Link back to BtShared
    pPrev1->pBt = pBt; pPrev2->pBt = pBt; pCur->pBt = pBt;
    pPrev1->pBtree = pBtree; pPrev2->pBtree = pBtree; pCur->pBtree = pBtree;
    pBt->openFlags = 0;

    // Execute
    sqlite3BtreeCloseCursor(pCur);

    // Verify
    bool ok = (pPrev2->pNext == nullptr) && (pPrev1->pNext == pPrev2) && (pCur->pBtree == 0) && (pBt->pCursor == pPrev1);

    // Cleanup
    free(pCur); free(pPrev2); free(pPrev1); free(pBt); free(pBtree);

    logTest("test_close_cursor_else_branch_traverses_chain", ok);
    return ok;
}

int main() {
    bool all = true;
    all &= test_close_cursor_single_node_replaces_cursor_with_null();
    all &= test_close_cursor_else_branch_relinks_previous_node();
    all &= test_close_cursor_else_branch_traverses_chain();

    if (all) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
}