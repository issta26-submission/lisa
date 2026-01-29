/*
  Test suite for the focal method: lsmTreeCursorSeek

  Context and approach:
  - The focal method belongs to a complex C module (lsm_tree.c) that models a
    disk-backed B-tree like structure. The test harness below is designed to
    exercise the behavior of lsmTreeCursorSeek via the public API surface that
    (usually) the project exposes: lsmTreeCursorNew, lsmTreeCursorSeek, and
    lsmTreeCursorDestroy, plus a minimal set of operations to build a small tree.

  - This test suite is C++11 compatible and avoids any GTest usage as required.
    It relies on the project’s own header declarations so that the actual
    implementation and memory layout are exercised as intended by the original
    project. If your environment places the headers in a non-standard path, adjust
    the include paths accordingly.

  - Test harness uses a lightweight, non-terminating assertion mechanism (EXPECT_*)
    to maximize code execution and coverage. It prints failures but does not abort
    the test run, aligning with the non-terminating assertion guidance.

  - Important: This suite assumes the availability of the project’s core API:
      - lsmTreeCursorNew(lsm_db*, int, TreeCursor**)
      - lsmTreeCursorDestroy(TreeCursor*)
      - lsmTreeCursorSeek(TreeCursor*, void* pKey, int nKey, int* pRes)
      - lsmTreeInsert(lsm_db*, void* pKey, int nKey, void* pVal, int nVal)
    as well as the core types lsm_db, TreeCursor, etc. Ensure the include
    path is set up to expose these declarations in your environment.
*/

/* Candidate Keywords distilled from the focal method (for test design guidance):
   - TreeCursor, lsm_db, TreeRoot, iRoot, iNodePtr, TreeNode, TreeKey
   - LSM_CONTIGUOUS, TKV_LOADKEY, treeKeycmp, treeShmptrUnsafe
   - getChildPtr, iTest, iNode, aiKeyPtr, aiCell, pTreeKey, pBlob
   - treeCursorRestore, tblobFree, pRes, pRoot->nHeight, pRoot->iTransId
   - lsmTreeCursorNew / lsmTreeCursorDestroy
   - lsmTreeInsert, lsmTreeCursorSeek
   - Assertions: rc, pRes, iNode, Match paths (res==0), non-match paths
   - Memory layout: contiguous vs non-contiguous keys, left/right traversal
   - Safety: NULL pKey handling, empty tree (iRoot == 0)
*/

#include <functional>
#include <string.h>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>


// C linkage declarations for the project API
extern "C" {
    // Forward declarations assuming typical project API
    typedef struct lsm_db lsm_db;
    typedef struct TreeCursor TreeCursor;

    // API surface used by tests
    int lsmTreeCursorNew(lsm_db *pDb, int bOld, TreeCursor **ppCsr);
    void lsmTreeCursorDestroy(TreeCursor *pCsr);
    int lsmTreeCursorSeek(TreeCursor *pCsr, void *pKey, int nKey, int *pRes);
    int lsmTreeInsert(lsm_db *pDb, void *pKey, int nKey, void *pVal, int nVal);

    // Optional helper in public API (if available in your build)
    // int lsmTreeCursorReset(TreeCursor *pCsr);
    // ... other APIs not used by these tests
}

// Lightweight test harness: non-terminating assertions
static int g_test_total = 0;
static int g_test_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_test_total; \
        if(!(cond)) { \
            std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")" << std::endl; \
            ++g_test_failed; \
        } \
    } while(0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++g_test_total; \
        if(!((a) == (b))) { \
            std::cerr << "[FAILED] " << msg << " | Expected: " << (b) << ", Got: " << (a) \
                      << " (line " << __LINE__ << ")" << std::endl; \
            ++g_test_failed; \
        } \
    } while(0)

#define EXPECT_NEQ(a, b, msg) \
    do { \
        ++g_test_total; \
        if((a) == (b)) { \
            std::cerr << "[FAILED] " << msg << " | Both are: " << (a) \
                      << " (line " << __LINE__ << ")" << std::endl; \
            ++g_test_failed; \
        } \
    } while(0)

// Helper to print summary
static void print_summary() {
    std::cout << "\nTest Summary: " << g_test_total << " tests, "
              << g_test_failed << " failures." << std::endl;
}

// Test 1: Seek on an empty tree should report not found (pRes == -1)
static void test_seek_on_empty_tree() {
    // Create a dummy DB object (handles are opaque to the test)
    lsm_db *pDb = new lsm_db{};
    TreeCursor *pCsr = nullptr;

    // Create a cursor for the empty tree
    int rc = lsmTreeCursorNew(pDb, 0, &pCsr);
    EXPECT_EQ(rc, 0, "lsmTreeCursorNew should succeed for a fresh DB");

    // Prepare a key to seek. The actual value isn't critical since the tree is empty.
    const char dummyKey[] = "empty";
    int res = 0;

    // Perform the seek
    rc = lsmTreeCursorSeek(pCsr, (void*)dummyKey, (int)sizeof(dummyKey) - 1, &res);
    EXPECT_EQ(rc, 0, "lsmTreeCursorSeek should return OK for empty tree");
    EXPECT_EQ(res, -1, "Seek result for empty tree should be -1 (not found)");

    // Cleanup
    lsmTreeCursorDestroy(pCsr);
    delete pDb;
}

// Test 2: Seek to an existing key after an insert.
// This exercise requires that the DB supports insertion prior to seek.
static void test_seek_existing_key_after_insert() {
    lsm_db *pDb = new lsm_db{};
    TreeCursor *pCsr = nullptr;

    int rc = lsmTreeCursorNew(pDb, 0, &pCsr);
    EXPECT_EQ(rc, 0, "lsmTreeCursorNew should succeed");

    // Insert a key-value pair into the tree
    const char key[] = "test-key";
    rc = lsmTreeInsert(pDb, (void*)key, (int)strlen(key), NULL, 0);
    EXPECT_EQ(rc, 0, "lsmTreeInsert should succeed for a simple key");

    // Seek for the same key
    int res = 0;
    rc = lsmTreeCursorSeek(pCsr, (void*)key, (int)strlen(key), &res);
    EXPECT_EQ(rc, 0, "lsmTreeCursorSeek should succeed after insertion");
    // The exact semantics of res can depend on the internal ordering; we expect a match
    EXPECT_EQ(res, 0, "Seek result for existing key should be 0 (key found exactly)");

    lsmTreeCursorDestroy(pCsr);
    delete pDb;
}

// Test 3: Seek for a key that does not exist but would be located in the tree.
// This ensures the function traverses and returns a non-zero comparison result.
static void test_seek_missing_key() {
    lsm_db *pDb = new lsm_db{};
    TreeCursor *pCsr = nullptr;

    int rc = lsmTreeCursorNew(pDb, 0, &pCsr);
    EXPECT_EQ(rc, 0, "lsmTreeCursorNew should succeed");

    // Insert a definite key
    const char presentKey[] = "present";
    rc = lsmTreeInsert(pDb, (void*)presentKey, (int)strlen(presentKey), NULL, 0);
    EXPECT_EQ(rc, 0, "Insertion of present key should succeed");

    // Seek for a different key that is not present
    const char missingKey[] = "absent";
    int res = 0;
    rc = lsmTreeCursorSeek(pCsr, (void*)missingKey, (int)strlen(missingKey), &res);
    EXPECT_EQ(rc, 0, "lsmTreeCursorSeek should succeed even if key is missing");
    // res is expected to be non-zero (positive/negative) depending on ordering
    EXPECT_TRUE(res != 0, "Seek result for missing key should indicate position (non-zero)");

    lsmTreeCursorDestroy(pCsr);
    delete pDb;
}

// Entry point for test runner
int main() {
    // Explain test goals
    std::cout << "Running lsmTreeCursorSeek unit tests (C API) without GTest.\n";

    test_seek_on_empty_tree();
    test_seek_existing_key_after_insert();
    test_seek_missing_key();

    print_summary();
    return (g_test_failed == 0) ? 0 : 1;
}