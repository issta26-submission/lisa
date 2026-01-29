// Self-contained C++11 unit tests for a focused reproduction of sqlite3PagerBegin.
// This test harness re-implements a minimal, self-contained version of the focal method
// and its dependencies to enable thorough, executable unit tests without GoogleTest.
// The goal is to validate the control flow and key state changes in sqlite3PagerBegin
// under different simulated conditions.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cassert>


// Minimal domain-specific types and constants to support the focal method logic.
typedef unsigned char u8;

const int SQLITE_OK = 0;
const int SQLITE_ERROR = 1;
const int SQLITE_BUSY = 5;

// Pager states (simplified)
const int PAGER_READER = 0;
const int PAGER_WRITER_LOCKED = 1;
const int PAGER_CACHEMOD = 2;
const int PAGER_ERROR_STATE = 3; // used only for upper bound in assertion

// Locking constants
const int EXCLUSIVE_LOCK = 2;
const int RESERVED_LOCK  = 1;

// Fallback assertion helper (the real code uses assert to verify invariants)
static int assert_pager_state_dummy(Pager*){
    return 1;
}

// Forward declaration of the focal method and its helpers.
// This is a self-contained reproduction to allow isolated unit testing without the real SQLite sources.
struct Pager {
    int errCode;          // non-zero => error
    int eState;           // current pager state
    void* pWal;            // WAL handle (opaque in this test)
    int exclusiveMode;    // when non-zero, request exclusive lock if WAL is used
    int subjInMemory;       // input parameter (will be cast to u8)
    int useWal;           // treat as true/false to route code path
    void* pInJournal;       // unused in tests (must be 0 for the initial branch)
    int dbSize;           // current database size
    int dbHintSize;       // copied from dbSize on Begin
    int dbFileSize;       // copied from dbSize on Begin
    int dbOrigSize;       // copied from dbSize on Begin
    int journalOff;       // journal offset (set to 0 in Begin)
    int walBeginOK;        // control whether WAL begin write transaction succeeds
    int pagerLockOk;       // control whether pagerLockDb returns SQLITE_OK
    int waitOnLockOk;        // control whether pager_wait_on_lock succeeds
};

// Global pointer to the current pager for stubbed WAL/locking helpers.
static Pager* g_pCurr = nullptr;

// Helper to emulate the rest of the system for tests.
static int pagerUseWal(Pager *pPager){
    return pPager ? pPager->useWal : 0;
}
static int sqlite3WalExclusiveMode(void *pWal, int eMode){
    // In tests, we route behavior through g_pCurr to control outcomes.
    (void)pWal; (void)eMode;
    if(g_pCurr) return (g_pCurr->exclusiveMode != 0) ? 1 : 0;
    return 0;
}
static int pagerLockDb(Pager *pPager, int eLock){
    (void)pPager; (void)eLock;
    if(g_pCurr) return g_pCurr->pagerLockOk ? SQLITE_OK : SQLITE_BUSY;
    return SQLITE_OK;
}
static int sqlite3WalBeginWriteTransaction(void *pWal){
    (void)pWal;
    if(g_pCurr) return g_pCurr->walBeginOK ? SQLITE_OK : SQLITE_ERROR;
    return SQLITE_OK;
}
static int pager_wait_on_lock(Pager *pPager, int locktype){
    (void)pPager; (void)locktype;
    if(g_pCurr) return g_pCurr->waitOnLockOk ? SQLITE_OK : SQLITE_BUSY;
    return SQLITE_OK;
}

// The focal method (self-contained version)
static int sqlite3PagerBegin(Pager *pPager, int exFlag, int subjInMemory){
    int rc = SQLITE_OK;
    if( pPager->errCode ) return pPager->errCode;
    // In real code, this assert validates the state range before proceeding.
    assert( pPager->eState>=PAGER_READER && pPager->eState<PAGER_ERROR_STATE );
    pPager->subjInMemory = (u8)subjInMemory;
    if( pPager->eState==PAGER_READER ){
        // For tests, enforce pInJournal == 0
        assert( pPager->pInJournal==0 );
        if( pagerUseWal(pPager) ){
            if( pPager->exclusiveMode && sqlite3WalExclusiveMode(pPager->pWal, -1) ){
                rc = pagerLockDb(pPager, EXCLUSIVE_LOCK);
                if( rc!=SQLITE_OK ){
                    return rc;
                }
                (void)sqlite3WalExclusiveMode(pPager->pWal, 1);
            }
            rc = sqlite3WalBeginWriteTransaction(pPager->pWal);
        }else{
            rc = pagerLockDb(pPager, RESERVED_LOCK);
            if( rc==SQLITE_OK && exFlag ){
                rc = pager_wait_on_lock(pPager, EXCLUSIVE_LOCK);
            }
        }
        if( rc==SQLITE_OK ){
            /* Change to WRITER_LOCKED state and sync size fields. */
            pPager->eState = PAGER_WRITER_LOCKED;
            pPager->dbHintSize = pPager->dbSize;
            pPager->dbFileSize = pPager->dbSize;
            pPager->dbOrigSize = pPager->dbSize;
            pPager->journalOff = 0;
        }
        // State assertions (mirrored from original)
        assert( rc==SQLITE_OK || pPager->eState==PAGER_READER );
        assert( rc!=SQLITE_OK || pPager->eState==PAGER_WRITER_LOCKED );
        assert( assert_pager_state_dummy(pPager) );
    }
    // Trace hook (no-op in tests)
    // PAGERTRACE(("TRANSACTION %d\n", PAGERID(pPager)));
    return rc;
}

// Test scaffolding helpers
static void resetPager(Pager &p){
    std::memset(&p, 0, sizeof(Pager));
    p.eState = PAGER_READER;
    p.subjInMemory = 0;
    p.dbSize = 0;
    p.dbHintSize = 0;
    p.dbFileSize = 0;
    p.dbOrigSize = 0;
    p.journalOff = 0;
    pInJournal = nullptr;
}

// Simple test harness utilities
static int gTestPassCount = 0;
static int gTestFailCount = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    if((expected) != (actual)){ \
        std::cerr << "FAIL: " << msg << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
        ++gTestFailCount; \
    } else { \
        ++gTestPassCount; \
    } \
} while(0)

#define TEST_CASE(name) void name()
#define RUN_TEST(name) do { name(); } while(0)

// Test 1: Early return when errCode is non-zero
TEST_CASE(test_errCode_precheck){
    Pager p;
    resetPager(p);
    p.errCode = 7;          // non-zero cause early return
    p.eState = PAGER_READER;
    p.useWal = 1;
    p.pWal = (void*)1;
    g_pCurr = &p;

    int rc = sqlite3PagerBegin(&p, 0 /*exFlag*/, 1 /*subjInMemory*/);

    EXPECT_EQ(7, rc, "Early return should propagate errCode");
    // State should be untouched
    EXPECT_EQ(PAGER_READER, p.eState, "State unchanged on early return");
}

// Test 2: WAL path with exclusive lock granted and begin write succeeds
TEST_CASE(test_wal_path_exclusive_lock_success){
    Pager p;
    resetPager(p);
    p.errCode = 0;
    p.eState = PAGER_READER;
    p.useWal = 1;
    p.pWal = (void*)1;
    p.exclusiveMode = 1;
    p.dbSize = 1234;
    p.subjInMemory = 0;
    g_pCurr = &p;

    // Control WAL/locking behavior
    g_pCurr->pagerLockOk = 1;     // pagerLockDb returns SQLITE_OK
    g_pCurr->walBeginOK = 1;      // WAL begin write succeeds
    g_pCurr->exclusiveMode = 1;   // require exclusive path

    int rc = sqlite3PagerBegin(&p, 1 /*exFlag*/, 1 /*subjInMemory*/);

    EXPECT_EQ(SQLITE_OK, rc, "WAL path with exclusive lock and begin should succeed");
    EXPECT_EQ(PAGER_WRITER_LOCKED, p.eState, "State should become WRITER_LOCKED after successful Begin");
    EXPECT_EQ(p.dbSize, p.dbHintSize, "dbHintSize should copy dbSize");
    EXPECT_EQ(p.dbSize, p.dbFileSize, "dbFileSize should copy dbSize");
    EXPECT_EQ(p.dbSize, p.dbOrigSize, "dbOrigSize should copy dbSize");
    EXPECT_EQ(0, p.journalOff, "journalOff should be reset to 0");
    EXPECT_EQ(1, p.subjInMemory, "subjInMemory should be stored (cast to u8)");
}

// Test 3: WAL path with exclusive lock fails on pagerLockDb
TEST_CASE(test_wal_path_exclusive_lock_failure){
    Pager p;
    resetPager(p);
    p.errCode = 0;
    p.eState = PAGER_READER;
    p.useWal = 1;
    p.pWal = (void*)1;
    p.exclusiveMode = 1;
    p.dbSize = 100;
    g_pCurr = &p;

    // Force the lock to fail
    g_pCurr->pagerLockOk = 0;      // pagerLockDb returns SQLITE_BUSY
    g_pCurr->walBeginOK = 1;       // WAL begin write would succeed, but lock prevents

    int rc = sqlite3PagerBegin(&p, 1 /*exFlag*/, 0 /*subjInMemory*/);

    EXPECT_EQ(SQLITE_BUSY, rc, "WAL exclusive path should propagate pagerLockDb failure");
    EXPECT_EQ(PAGER_READER, p.eState, "State should remain READER when lock fails");
}

// Test 4: Non-WAL path with exFlag true and lock succeeds (RESERVED_LOCK then EXCLUSIVE_WAIT)
TEST_CASE(test_nonwal_path_exFlag_true_lock_succeeds){
    Pager p;
    resetPager(p);
    p.errCode = 0;
    p.eState = PAGER_READER;
    p.useWal = 0;
    p.pWal = nullptr;
    p.dbSize = 2048;
    g_pCurr = &p;

    // Non-WAL path: first obtain RESERVED_LOCK, then possibly upgrade
    g_pCurr->pagerLockOk = 1;     // RESERVED_LOCK succeeds
    g_pCurr->waitOnLockOk = 1;    // EXCLUSIVE_LOCK wait succeeds when exFlag true
    int rc = sqlite3PagerBegin(&p, 1 /*exFlag*/, 1 /*subjInMemory*/);

    EXPECT_EQ(SQLITE_OK, rc, "Non-WAL path with lock and upgrade should succeed");
    EXPECT_EQ(PAGER_WRITER_LOCKED, p.eState, "State should be WRITER_LOCKED after Begin");
    EXPECT_EQ(1, p.subjInMemory, "subjInMemory should be stored");
}

// Test 5: Non-WAL path with exFlag false (no upgrade path)
TEST_CASE(test_nonwal_path_exFlag_false){
    Pager p;
    resetPager(p);
    p.errCode = 0;
    p.eState = PAGER_READER;
    p.useWal = 0;
    p.pWal = nullptr;
    p.dbSize = 512;
    g_pCurr = &p;

    g_pCurr->pagerLockOk = 1;
    // exFlag = 0 means we should not call pager_wait_on_lock
    int rc = sqlite3PagerBegin(&p, 0 /*exFlag*/, 0 /*subjInMemory*/);

    EXPECT_EQ(SQLITE_OK, rc, "Non-WAL path with exFlag false should succeed");
    EXPECT_EQ(PAGER_WRITER_LOCKED, p.eState, "State should be WRITER_LOCKED");
    EXPECT_EQ(0, p.subjInMemory, "subjInMemory should reflect input (0)");
}

// Simple main to run tests and report summary
int main(){
    // Run tests
    RUN_TEST(test_errCode_precheck);
    RUN_TEST(test_wal_path_exclusive_lock_success);
    RUN_TEST(test_wal_path_exclusive_lock_failure);
    RUN_TEST(test_nonwal_path_exFlag_true_lock_succeeds);
    RUN_TEST(test_nonwal_path_exFlag_false);

    // Summary
    std::cout << "Tests run: " << (gTestPassCount + gTestFailCount)
              << ", Passed: " << gTestPassCount
              << ", Failed: " << gTestFailCount << std::endl;
    if(gTestFailCount>0){
        return 1;
    }
    return 0;
}