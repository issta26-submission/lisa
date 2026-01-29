// C++11 test harness for sqlite3BtreeNext (btree.c) without using GTest
// This test focuses on the true-branch path of sqlite3BtreeNext
// where the cursor state is CURSOR_VALID and the page is a leaf page.
// It verifies side effects: info.nSize reset to 0, certain flags cleared,
// ix increment behavior, and that SQLITE_OK is returned.
// The test uses only the C standard library and provided project headers.

#include <btreeInt.h>
#include <iostream>
#include <cstring>


// Include the SQLite B-Tree related definitions.
// We rely on the project's existing btreeInt.h to provide BtCursor, MemPage, and constants.
extern "C" {
}

// Declare the focal function from the project (C linkage)
extern "C" int sqlite3BtreeNext(BtCursor *pCur, int flags);

// Simple non-terminating test harness (no GTest). We implement lightweight checks
// and accumulate results. Each test prints its own explanatory comments.
static bool test_leaf_path_true_branch() {
    // Purpose:
    // Exercise sqlite3BtreeNext when:
    // - pCur->eState == CURSOR_VALID
    // - pPage->leaf == true
    // - (++pCur->ix) < pPage->nCell
    // Expect:
    // - sqlite3BtreeNext returns SQLITE_OK (0)
    // - pCur->info.nSize is reset to 0
    // - BTCF_ValidNKey and BTCF_ValidOvfl flags are cleared from curFlags
    // - pCur->ix is incremented by 1
    // - No crash or undefined behavior

    MemPage page;
    memset(&page, 0, sizeof(MemPage));
    page.nCell = 3;   // Ensure (++ix) < nCell for initial ix = 0
    page.leaf = 1;    // Leaf page to take the early return path

    BtCursor cur;
    memset(&cur, 0, sizeof(BtCursor));

    // Link the structures
    cur.pPage = &page;
    cur.eState = CURSOR_VALID;          // Must be a valid state to enter the main if-block
    cur.ix = 0;                          // Will be incremented to 1 by sqlite3BtreeNext
    cur.info.nSize = 123;                // Non-zero initially to confirm it's reset
    cur.curFlags = BTCF_ValidNKey | BTCF_ValidOvfl; // Set flags that should be cleared

    // Call the function under test
    int rc = sqlite3BtreeNext(&cur, 0);

    // Verify outcomes (non-terminating assertions: print and continue)
    bool ok = true;

    // Expected return code: SQLITE_OK -> 0
    if (rc != 0) {
        std::cerr << "test_leaf_path_true_branch: Failure - expected rc == 0 (SQLITE_OK), got " << rc << "\n";
        ok = false;
    }

    // info.nSize should be reset to 0
    if (cur.info.nSize != 0) {
        std::cerr << "test_leaf_path_true_branch: Failure - expected cur.info.nSize == 0, got " << cur.info.nSize << "\n";
        ok = false;
    }

    // curFlags: BTCF_ValidNKey and BTCF_ValidOvfl should be cleared
    const unsigned int clearedMask = BTCF_ValidNKey | BTCF_ValidOvfl;
    if ((cur.curFlags & clearedMask) != 0) {
        std::cerr << "test_leaf_path_true_branch: Failure - expected BTCF_ValidNKey and BTCF_ValidOvfl to be cleared from curFlags, got 0x" 
                  << std::hex << cur.curFlags << std::dec << "\n";
        ok = false;
    }

    // eState should remain CURSOR_VALID
    if (cur.eState != CURSOR_VALID) {
        std::cerr << "test_leaf_path_true_branch: Failure - expected eState to remain CURSOR_VALID\n";
        ok = false;
    }

    // ix should have been incremented to 1 (from 0)
    if (cur.ix != 1) {
        std::cerr << "test_leaf_path_true_branch: Failure - expected ix == 1, got " << cur.ix << "\n";
        ok = false;
    }

    // The page should still be leaf and have the same nCell
    if (page.leaf != 1 || page.nCell != 3) {
        std::cerr << "test_leaf_path_true_branch: Failure - page state altered unexpectedly (leaf=" 
                  << (int)page.leaf << ", nCell=" << page.nCell << ")\n";
        ok = false;
    }

    return ok;
}

// Entry point for running all tests
int main() {
    int total_pass = 0;
    int total_tests = 0;

    // Test 1: Leaf path true-branch coverage
    ++total_tests;
    if (test_leaf_path_true_branch()) {
        std::cout << "[PASS] test_leaf_path_true_branch\n";
        ++total_pass;
    } else {
        std::cout << "[FAIL] test_leaf_path_true_branch\n";
    }

    // Summary
    std::cout << "Tests passed: " << total_pass << " / " << total_tests << "\n";

    // Return non-zero if any test failed
    return (total_tests == total_pass) ? 0 : 1;
}