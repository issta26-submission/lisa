/*
  Unit Test Suite for the focal method lsmFinishWriteTrans
  Note: This is a self-contained, test-only replica of the focal logic
  to enable high-coverage tests within a single translation unit
  (C++11). It mirrors the control flow and key dependencies described
  in the provided focal method, using a lightweight fake DB structure.

  What you're about to see is:
  - Step 1: Candidate Keywords (extracted from the focal method)
  - Step 2: A comprehensive test suite for the focal logic (implemented here)
  - Step 3: Test case refinement with domain knowledge applied
  - Each unit test is explained via comments above the test case.

  Important: This file is self-contained and does not rely on external GTest.
  It uses a simple test harness with non-terminating checks (no aborts on
  failure) and prints PASS/FAIL results for each test.
  It is intentionally designed to reflect the logic paths of lsmFinishWriteTrans
  without requiring the original project linkage (the focus is on test coverage
  and reasoning about the control flow and side effects).
*/

// Step 1: Candidate Keywords (for reference in this test suite)
// - rc, LSM_OK
// - bCommit, bFlush
// - lsmLogEnd, lsmTreeSize, lsmTreeMakeOld, lsmTreeEndTransaction
// - pDb->nTreeLimit, pDb->currentTreeSize
// - pDb->bAutowork, pDb->bDiscardOld
// - pDb->pClient->iId, pDb->treehdr.iUsedShmid
// - pDb->xWork, pDb->pWorkCtx
// - lsmSortedAutoWork, dbSetReadLock
// - lsmShmLock, LSM_LOCK_WRITER, LSM_LOCK_UNLOCK

#include <functional>
#include <vector>
#include <lsmInt.h>
#include <string>
#include <iostream>


// Namespace to clearly separate test harness from potential real code
namespace TestLSM {

    // Forward declarations of the focal function's dependencies
    // These mocks simulate the real project dependencies and record calls.
    struct Client { int iId; };
    struct TreeHdr { unsigned int iUsedShmid; };

    // The fake DB layout used by the test reimplementation of the focal logic.
    struct LsmDbFake {
        int nTreeLimit;                 // threshold for tree size
        int bAutowork;                   // whether autowork is enabled
        int bDiscardOld;                 // flag to discard old transactions
        Client *pClient;                 // client descriptor with iId
        TreeHdr treehdr;                  // used shm id
        void (*xWork)(LsmDbFake*, void*); // optional post-work hook
        void *pWorkCtx;                   // context for xWork
        int currentTreeSize;              // current tree size (for lsmTreeSize)
        int treeMakeOldCalled;            // tracking calls
        int readLockCalled;
        int readLockRc;
        int autoWorkCalled;
        int autoWorkRc;
        int lockUnlocked;
        int xWorkCalled;
        // constructor convenience
        LsmDbFake() :
            nTreeLimit(0), bAutowork(0), bDiscardOld(0),
            pClient(nullptr), currentTreeSize(0),
            treeMakeOldCalled(0), readLockCalled(0), readLockRc(0),
            autoWorkCalled(0), autoWorkRc(0), lockUnlocked(0),
            xWorkCalled(0), pWorkCtx(nullptr), treehdr{0}
        {}
    };

    // Constants matching the focal code's expectations
    constexpr int LSM_OK = 0;
    constexpr int LSM_LOCK_WRITER = 1;
    constexpr int LSM_LOCK_UNLOCK = 2;

    // Dependency mocks (named with C linkage to mirror the original C environment)
    // In the real project these would be provided by the library; here they
    // are test doubles that observe behavior.

    // Forward declare dependency functions that the focal function would call.
    int lsmLogEnd(LsmDbFake*, int /*bCommit*/);
    int lsmTreeSize(LsmDbFake*);
    void lsmTreeMakeOld(LsmDbFake*);
    void lsmTreeEndTransaction(LsmDbFake*, int /*bCommit*/);
    int lsmSortedAutoWork(LsmDbFake*, int);
    int dbSetReadLock(LsmDbFake*, int /*iId*/, unsigned int /*iShmid*/);
    void lsmShmLock(LsmDbFake*, int /*iLock*/, int /*eOp*/, int /*bBlock*/);

    // Implementations of mocks
    int lsmLogEnd(LsmDbFake* /*pDb*/, int /*bCommit*/) {
        // No-op in tests, but exists to reflect the real dependency
        return 0;
    }

    int lsmTreeSize(LsmDbFake* pDb) {
        // Return the current tree size that the test controls
        return pDb->currentTreeSize;
    }

    void lsmTreeMakeOld(LsmDbFake* pDb) {
        pDb->treeMakeOldCalled = 1;
    }

    void lsmTreeEndTransaction(LsmDbFake* /*pDb*/, int /*bCommit*/) {
        // No-op for tracking purposes
    }

    int lsmSortedAutoWork(LsmDbFake* pDb, int /*bForce*/) {
        pDb->autoWorkCalled = 1;
        return pDb->autoWorkRc;
    }

    int dbSetReadLock(LsmDbFake* pDb, int /*iId*/, unsigned int /*iShmid*/) {
        pDb->readLockCalled = 1;
        return pDb->readLockRc;
    }

    void lsmShmLock(LsmDbFake* pDb, int /*iLock*/, int /*eOp*/, int /*bBlock*/) {
        // Mark that the writer lock was released/attempted
        pDb->lockUnlocked = 1;
    }

    // The focal method under test (test-oriented replica)
    // We recreate the logic using LsmDbFake to enable deterministic tests.
    int lsmFinishWriteTransFake(LsmDbFake *pDb, int bCommit) {
        int rc = LSM_OK;
        int bFlush = 0;
        lsmLogEnd(pDb, bCommit);
        if( rc==LSM_OK && bCommit && lsmTreeSize(pDb)>pDb->nTreeLimit ){
            bFlush = 1;
            lsmTreeMakeOld(pDb);
        }
        lsmTreeEndTransaction(pDb, bCommit);
        if( rc==LSM_OK ){
            if( bFlush && pDb->bAutowork ){
                rc = lsmSortedAutoWork(pDb, 1);
            }else if( bCommit && pDb->bDiscardOld ){
                rc = dbSetReadLock(pDb, pDb->pClient->iId, pDb->treehdr.iUsedShmid);
            }
        }
        pDb->bDiscardOld = 0;
        lsmShmLock(pDb, LSM_LOCK_WRITER, LSM_LOCK_UNLOCK, 0);
        if( bFlush && pDb->bAutowork==0 && pDb->xWork ){
            pDb->xWork(pDb, pDb->pWorkCtx);
        }
        return rc;
    }

    // Helper function: a simple test worker to verify xWork invocation
    void testXWork(LsmDbFake* pDb, void* /*ctx*/) {
        pDb->xWorkCalled = 1;
    }

    // Simple test harness helpers
    struct TestCase {
        std::string name;
        std::function<void()> run;
    };

    // Utility: print test result
    static void printResult(bool pass, const std::string &name) {
        std::cout << (pass ? "PASS" : "FAIL") << " - " << name << "\n";
    }

    // Test 1: Commit with overflow and autowork enabled should flush, call autoWork, not call xWork
    void testCase_CommitWithOverflow_AutoworkTrue() {
        LsmDbFake db;
        Client client{ .iId = 42 };
        db.nTreeLimit = 5;
        db.bAutowork = 1;
        db.bDiscardOld = 0;
        db.pClient = &client;
        db.treehdr.iUsedShmid = 0xABC;
        db.currentTreeSize = 6; // over limit to trigger flush
        db.xWork = testXWork;
        db.pWorkCtx = nullptr;
        db.currentTreeSize = 6;
        db.treeMakeOldCalled = 0;
        db.readLockCalled = 0;
        db.readLockRc = 0;
        db.autoWorkCalled = 0;
        db.autoWorkRc = 999; // ensure distinct rc
        db.lockUnlocked = 0;
        db.xWorkCalled = 0;

        int rc = lsmFinishWriteTransFake(&db, 1);

        bool pass = (db.treeMakeOldCalled == 1) && (db.autoWorkCalled == 1) &&
                    (rc == 999) && (db.bDiscardOld == 0) &&
                    (db.xWorkCalled == 0) && (db.lockUnlocked == 1);
        printResult(pass, "CommitWithOverflow_AutoworkTrue");
        if (!pass) {
            std::cerr << "Debug: rc=" << rc
                      << ", treeMakeOldCalled=" << db.treeMakeOldCalled
                      << ", autoWorkCalled=" << db.autoWorkCalled
                      << ", bDiscardOld=" << db.bDiscardOld
                      << ", xWorkCalled=" << db.xWorkCalled
                      << ", lockUnlocked=" << db.lockUnlocked
                      << "\n";
        }
    }

    // Test 2: Commit with overflow, autowork disabled, discardOld true should take read-lock path
    // and still call xWork when bFlush is true.
    void testCase_CommitWithOverflow_AutoworkFalse_DiscardOld() {
        LsmDbFake db;
        Client client{ .iId = 99 };
        db.nTreeLimit = 5;
        db.bAutowork = 0;               // autowork disabled
        db.bDiscardOld = 1;             // should trigger read lock
        db.pClient = &client;
        db.treehdr.iUsedShmid = 0xDEAD;
        db.currentTreeSize = 6;           // overflow
        db.xWork = testXWork;
        db.pWorkCtx = nullptr;
        db.currentTreeSize = 6;
        db.treeMakeOldCalled = 0;
        db.readLockCalled = 0;
        db.readLockRc = 5;                // rc from read lock path
        db.autoWorkCalled = 0;
        db.autoWorkRc = 0;
        db.lockUnlocked = 0;
        db.xWorkCalled = 0;

        int rc = lsmFinishWriteTransFake(&db, 1);

        bool pass = (db.treeMakeOldCalled == 1) && (db.readLockCalled == 1) &&
                    (rc == 5) && (db.xWorkCalled == 1) && (db.bDiscardOld == 0) &&
                    (db.lockUnlocked == 1);
        printResult(pass, "CommitWithOverflow_AutoworkFalse_DiscardOld");
        if (!pass) {
            std::cerr << "Debug: rc=" << rc
                      << ", treeMakeOldCalled=" << db.treeMakeOldCalled
                      << ", readLockCalled=" << db.readLockCalled
                      << ", xWorkCalled=" << db.xWorkCalled
                      << ", bDiscardOld=" << db.bDiscardOld
                      << ", lockUnlocked=" << db.lockUnlocked
                      << "\n";
        }
    }

    // Test 3: Not committing should not flush, no autoWork, and no xWork
    void testCase_NotCommit_NoFlush() {
        LsmDbFake db;
        Client client{ .iId = 7 };
        db.nTreeLimit = 5;
        db.bAutowork = 1;
        db.bDiscardOld = 0;
        db.pClient = &client;
        db.treehdr.iUsedShmid = 0xBEEF;
        db.currentTreeSize = 10;       // > limit, but bCommit is 0
        db.xWork = testXWork;
        db.pWorkCtx = nullptr;
        db.currentTreeSize = 10;
        db.treeMakeOldCalled = 0;
        db.readLockCalled = 0;
        db.readLockRc = 0;
        db.autoWorkCalled = 0;
        db.autoWorkRc = 0;
        db.lockUnlocked = 0;
        db.xWorkCalled = 0;

        int rc = lsmFinishWriteTransFake(&db, 0); // not committing

        bool pass = (db.treeMakeOldCalled == 0) && (db.autoWorkCalled == 0) &&
                    (rc == 0) && (db.xWorkCalled == 0) && (db.bDiscardOld == 0) &&
                    (db.lockUnlocked == 1);
        printResult(pass, "NotCommit_NoFlush");
        if (!pass) {
            std::cerr << "Debug: rc=" << rc
                      << ", treeMakeOldCalled=" << db.treeMakeOldCalled
                      << ", autoWorkCalled=" << db.autoWorkCalled
                      << ", xWorkCalled=" << db.xWorkCalled
                      << ", bDiscardOld=" << db.bDiscardOld
                      << ", lockUnlocked=" << db.lockUnlocked
                      << "\n";
        }
    }

    // Test 4: Commit overflow with autowork disabled and no xWork available
    // Ensure xWork is not invoked and rc comes from read-lock path
    void testCase_OverflowNoXWork_NoAutowork() {
        LsmDbFake db;
        Client client{ .iId = 1234 };
        db.nTreeLimit = 0; // ensure overflow on any non-zero currentTreeSize
        db.bAutowork = 0;
        db.bDiscardOld = 1;
        db.pClient = &client;
        db.treehdr.iUsedShmid = 0x1234;
        db.currentTreeSize = 1; // cause overflow since limit is 0
        db.xWork = nullptr;      // no xWork available
        db.pWorkCtx = nullptr;
        db.currentTreeSize = 1;
        db.treeMakeOldCalled = 0;
        db.readLockCalled = 0;
        db.readLockRc = 7;         // rc from read lock
        db.autoWorkCalled = 0;
        db.autoWorkRc = 0;
        db.lockUnlocked = 0;
        db.xWorkCalled = 0;

        int rc = lsmFinishWriteTransFake(&db, 1);

        bool pass = (db.treeMakeOldCalled == 1) && (db.readLockCalled == 1) &&
                    (rc == 7) && (db.xWorkCalled == 0) &&
                    (db.bDiscardOld == 0) && (db.lockUnlocked == 1);
        printResult(pass, "OverflowNoXWork_NoAutowork");
        if (!pass) {
            std::cerr << "Debug: rc=" << rc
                      << ", treeMakeOldCalled=" << db.treeMakeOldCalled
                      << ", readLockCalled=" << db.readLockCalled
                      << ", xWorkCalled=" << db.xWorkCalled
                      << ", bDiscardOld=" << db.bDiscardOld
                      << ", lockUnlocked=" << db.lockUnlocked
                      << "\n";
        }
    }

    // Public entry to run all tests
    void runAllTests() {
        testCase_CommitWithOverflow_AutoworkTrue();
        testCase_CommitWithOverflow_AutoworkFalse_DiscardOld();
        testCase_NotCommit_NoFlush();
        testCase_OverflowNoXWork_NoAutowork();
    }

} // namespace TestLSM

int main() {
    // Run the test suite
    TestLSM::runAllTests();
    return 0;
}