// Unit test suite for lsm_rollback(lsm_db*, int) from lsm_main.c
// This test suite is written in C++11 (no GTest) and uses a lightweight
// in-process harness with non-terminating assertions. It relies on the
// project's existing C interfaces/types (e.g., lsm_db, TransMark) exposed
// via lsmInt.h and the lsm_rollback symbol provided by lsm_main.c.
//

#include <lsmInt.h>
#include <iostream>
#include <new>
#include <cstdint>


// Include the library header to obtain correct type definitions (lsm_db, TransMark, etc.)

// Declare the function under test with C linkage
extern "C" int lsm_rollback(lsm_db *pDb, int iLevel);

// Declare the internal hooks used by lsm_rollback in the library.
// We provide C-bridged stubs here so that we can observe side effects.
extern "C" void lsmTreeRollback(lsm_db *pDb, void *pTree);
extern "C" void lsmLogSeek(lsm_db *pDb, void *pLog);
extern "C" void lsmFinishWriteTrans(lsm_db *pDb, int);
extern "C" void dbReleaseClientSnapshot(lsm_db *pDb);

// Spy state to verify that internal hooks were invoked correctly
static void* g_lastTreeRolled = nullptr;
static void* g_lastLogSeek = nullptr;
static bool g_finishWriteTransCalled = false;
static bool g_snapshotReleased = false;

// Reset spy state before each test
static void reset_spy_state() {
    g_lastTreeRolled = nullptr;
    g_lastLogSeek = nullptr;
    g_finishWriteTransCalled = false;
    g_snapshotReleased = false;
}

// Spy implementations to attach to the library hooks
extern "C" void lsmTreeRollback(lsm_db *pDb, void *pTree) {
    // Capture the pointer passed for the innermost tree rollback
    g_lastTreeRolled = pTree;
}

extern "C" void lsmLogSeek(lsm_db *pDb, void *pLog) {
    // Capture the log position pointer if a seek happens
    g_lastLogSeek = pLog;
}

extern "C" void lsmFinishWriteTrans(lsm_db *pDb, int) {
    // Indicate that the transaction finished writing
    g_finishWriteTransCalled = true;
}

extern "C" void dbReleaseClientSnapshot(lsm_db *pDb) {
    // Indicate that the client snapshot was released
    g_snapshotReleased = true;
}

// Utility macro for non-terminating assertions (log on failure, continue)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (in " << __FUNCTION__ << ")" << std::endl; \
        test_failures++; \
    } else { \
        test_passes++; \
    } \
} while(0)

static int test_passes = 0;
static int test_failures = 0;

// Test 1: No transaction open should result in no actions performed
// - pDb.nTransOpen == 0 -> lsm_rollback should do nothing.
// - No internal hooks should be called (tree rollback, log seek, etc.).
static void test_no_trans_open() {
    reset_spy_state();

    lsm_db db = {};
    db.nTransOpen = 0;
    db.aTrans = nullptr; // not used when no transaction is open

    int rc = lsm_rollback(&db, -1);

    EXPECT_TRUE(rc == LSM_OK, "rc should be LSM_OK for no-op rollback");
    EXPECT_TRUE(g_lastTreeRolled == nullptr, "no tree should be rolled back when no trans open");
    EXPECT_TRUE(g_lastLogSeek == nullptr, "no log should be seeked when no trans open");
    EXPECT_TRUE(g_finishWriteTransCalled == false, "finishWriteTrans should not be called");
    EXPECT_TRUE(g_snapshotReleased == false, "snapshot should not be released");
}

// Test 2: Roll back inner-most transaction when iLevel < 0 with multiple transactions
// - nTransOpen = 3; iLevel resolves to 2; should rollback aTrans[1].tree and log aTrans[1].log
// - nTransOpen should become 2
// - lsmTreeRollback called with &aTrans[1].tree; lsmLogSeek called with &aTrans[1].log
// - finishWriteTrans should NOT be called; snapshot should be released
static void test_rollback_inner_in_nesting_negative_level() {
    reset_spy_state();

    lsm_db db = {};
    db.nTransOpen = 3;
    db.aTrans = new TransMark[3];

    // Initialize trees/logs for each level
    // Note: We store distinct addresses to verify exact pointer identities
    db.aTrans[0].tree = reinterpret_cast<void*>(0x1010);
    db.aTrans[0].log  = reinterpret_cast<void*>(0x2010);

    db.aTrans[1].tree = reinterpret_cast<void*>(0x1011);
    db.aTrans[1].log  = reinterpret_cast<void*>(0x2011);

    db.aTrans[2].tree = reinterpret_cast<void*>(0x1012);
    db.aTrans[2].log  = reinterpret_cast<void*>(0x2012);

    int rc = lsm_rollback(&db, -1);

    // Expected: rollback of aTrans[1]
    void* expectedTreePtr = &db.aTrans[1].tree;
    void* expectedLogPtr  = &db.aTrans[1].log;

    EXPECT_TRUE(rc == LSM_OK, "rc should be LSM_OK for rollback");
    EXPECT_TRUE(g_lastTreeRolled == expectedTreePtr, "Tree pointer rolled back should be aTrans[1].tree");
    EXPECT_TRUE(g_lastLogSeek == expectedLogPtr, "Log pointer should be lsmLogSeek'd with aTrans[1].log");
    EXPECT_TRUE(db.nTransOpen == 2, "nTransOpen should be updated to 2");
    EXPECT_TRUE(g_finishWriteTransCalled == false, "finishWriteTrans should not be called in mid-nest rollback");
    EXPECT_TRUE(g_snapshotReleased == true, "dbReleaseClientSnapshot should be called");
    
    // Cleanup
    delete[] db.aTrans;
}

// Test 3: Roll back to level 0 (iLevel == 0) when there is 1 transaction open
// - iLevel == 0 should rollback aTrans[0].tree and finish write trans (nTransOpen becomes 0)
// - lsmLogSeek should NOT be called (since iLevel == 0)
// - Snapshot should be released
static void test_rollback_to_top_level_from_one() {
    reset_spy_state();

    lsm_db db = {};
    db.nTransOpen = 1;
    db.aTrans = new TransMark[1];

    db.aTrans[0].tree = reinterpret_cast<void*>(0x1100);
    db.aTrans[0].log  = reinterpret_cast<void*>(0x2100);

    // Pre-set lastLogSeek to a sentinel; ensure it remains unchanged if iLevel == 0
    g_lastLogSeek = reinterpret_cast<void*>(0xDEADBEEF);

    int rc = lsm_rollback(&db, 0);

    void* expectedTreePtr = &db.aTrans[0].tree;
    EXPECT_TRUE(rc == LSM_OK, "rc should be LSM_OK for rollback to top level");
    EXPECT_TRUE(g_lastTreeRolled == expectedTreePtr, "Tree rolled back should be aTrans[0].tree");
    EXPECT_TRUE(g_lastLogSeek == reinterpret_cast<void*>(0xDEADBEEF),
                "Log should not be seeked when iLevel == 0");
    EXPECT_TRUE(db.nTransOpen == 0, "nTransOpen should be updated to 0");
    EXPECT_TRUE(g_finishWriteTransCalled == true, "finishWriteTrans should be called when rolling back to 0");
    EXPECT_TRUE(g_snapshotReleased == true, "dbReleaseClientSnapshot should be called");

    delete[] db.aTrans;
}

// Test 4: iLevel greater than nTransOpen should result in no action
// - nTransOpen stays the same; no lsmTreeRollback/lsmLogSeek should be invoked
static void test_iLevel_exceeds_nTransOpen_no_action() {
    reset_spy_state();

    lsm_db db = {};
    db.nTransOpen = 2;
    db.aTrans = new TransMark[2];

    db.aTrans[0].tree = reinterpret_cast<void*>(0x1200);
    db.aTrans[0].log  = reinterpret_cast<void*>(0x2200);

    db.aTrans[1].tree = reinterpret_cast<void*>(0x1201);
    db.aTrans[1].log  = reinterpret_cast<void*>(0x2201);

    int rc = lsm_rollback(&db, 5); // iLevel > nTransOpen

    EXPECT_TRUE(rc == LSM_OK, "rc should be LSM_OK even if no action is taken");
    EXPECT_TRUE(g_lastTreeRolled == nullptr, "no tree should be rolled back when iLevel > nTransOpen");
    EXPECT_TRUE(g_lastLogSeek == nullptr, "no log should be seeked when iLevel > nTransOpen");
    EXPECT_TRUE(db.nTransOpen == 2, "nTransOpen should remain unchanged");
    EXPECT_TRUE(g_finishWriteTransCalled == false, "finishWriteTrans should not be called");
    EXPECT_TRUE(g_snapshotReleased == false, "snapshot should not be released");

    delete[] db.aTrans;
}

// Runner to execute all tests and report results
int main() {
    std::cout << "Starting lsm_rollback unit tests (non-GTest, standalone)..." << std::endl;

    test_passes = 0;
    test_failures = 0;

    test_no_trans_open();
    test_rollback_inner_in_nesting_negative_level();
    test_rollback_to_top_level_from_one();
    test_iLevel_exceeds_nTransOpen_no_action();

    // Summary
    std::cout << "Tests run: " << (test_passes + test_failures)
              << ", Passed: " << test_passes
              << ", Failed: " << test_failures << std::endl;

    if(test_failures > 0) {
        std::cerr << "Some tests failed. Please review the failure messages above." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}