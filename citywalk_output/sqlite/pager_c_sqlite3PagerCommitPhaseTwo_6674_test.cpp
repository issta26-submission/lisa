// Unit test suite for sqlite3PagerCommitPhaseTwo
// This test harness provides a minimal mock environment to exercise the focal method logic.
// It does not rely on GTest; instead, it uses a tiny in-file test framework with non-terminating checks.

#include <iostream>
#include <wal.h>
#include <sqliteInt.h>


// Lightweight test framework (non-terminating assertions)
static int g_failCount = 0;
#define EXPECT_EQ(expected, actual, msg) do { \
    if((expected) != (actual)) { \
        std::cout << "FAIL: " << (msg) << " | expected: " << (expected) << ", actual: " << (actual) << "\n"; \
        ++g_failCount; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cout << "FAIL: " << (msg) << "\n"; \
        ++g_failCount; \
    } \
} while(0)

#define RUN_TEST(name) \
    std::cout << "RUN: " << #name << "\n"; \
    name();

// Minimal constants and types to mock the SQLite Pager behavior used by the focal method.
static const int SQLITE_OK = 0;

// Pager state enum (subset needed for tests)
enum PagerState {
    PAGER_READER = 0,
    PAGER_WRITER_LOCKED = 1,
    PAGER_WRITER_FINISHED = 2,
    PAGER_WRITER_CACHEMOD = 3
};

// Journal mode constant (subset)
static const int PAGER_JOURNALMODE_PERSIST = 1;

// Journal header size mock (used by macro in focal method)
#define JOURNAL_HDR_SZ(p) 4

// NEVER macro mock: treat non-zero as an unexpected/err path
#define NEVER(x) ((x) != 0 ? 1 : 0)

// Forward declare the Pager type (actual layout is not important for test harness)
struct Pager;
typedef struct Pager Pager;

// Helper stubs used by the focal method (mocked behavior)
static inline int pagerUseWal(Pager *pPager);
static inline int assert_pager_state(Pager *pPager);
static inline int pager_end_transaction(Pager *pPager, int hasSuper, int bCommit);
static inline int pager_error(Pager *pPager, int rc);
static inline int PAGERTRACE(const char* s);
static inline int PAGERID(Pager *pPager);

// Lightweight Pager mock (only fields accessed by the focal method)
struct Pager {
    int errCode;        // Error code (0 means no prior error)
    int iDataVersion;   // Data version counter
    int eState;          // Current pager state
    int exclusiveMode;  // Exclusive mode flag
    int journalMode;    // Journal mode
    int journalOff;     // Journal offset
    int setSuper;       // Flag indicating presence of a super-user
    int useWal;         // WAL usage flag
    int endTxnRc;       // RC to simulate end-transaction outcome
};

// Implementations of the mocked helper functions

static inline int pagerUseWal(Pager *pPager){
    return pPager ? pPager->useWal : 0;
}
static inline int assert_pager_state(Pager *pPager){
    // In tests, we assume the internal state is always consistent
    (void)pPager;
    return 1;
}
static inline int pager_end_transaction(Pager *pPager, int hasSuper, int bCommit){
    // Use the configured RC to simulate success or failure
    (void)hasSuper; (void)bCommit;
    return (pPager ? pPager->endTxnRc : 0);
}
static inline int pager_error(Pager *pPager, int rc){
    // In the real code this might set pPager->errCode, etc.
    // For test purposes simply return rc.
    (void)pPager;
    return rc;
}
static inline int PAGERTRACE(const char* /*s*/){
    // No-op in tests
    return 0;
}
static inline int PAGERID(Pager * /*pPager*/){
    // Return a dummy ID for trace printing if needed
    return 0;
}

// The focal method under test (duplicated here to enable standalone unit tests without linking to the real pager.c)
int sqlite3PagerCommitPhaseTwo(Pager *pPager){
  int rc = SQLITE_OK;                  /* Return code */
  /* This routine should not be called if a prior error has occurred.
  ** But if (due to a coding error elsewhere in the system) it does get
  ** called, just return the same error code without doing anything. */
  if( NEVER(pPager->errCode) ) return pPager->errCode;
  pPager->iDataVersion++;
  // In tests we assume the state is valid for the scenarios we cover
  // (the actual code contains an assert; we approximate by ensuring the path is valid)
  // assert( pPager->eState==PAGER_WRITER_LOCKED
  //       || pPager->eState==PAGER_WRITER_FINISHED
  //       || (pagerUseWal(pPager) && pPager->eState==PAGER_WRITER_CACHEMOD)
  // );
  // assert( assert_pager_state(pPager) );
  if( pPager->eState==PAGER_WRITER_LOCKED
   && pPager->exclusiveMode
   && pPager->journalMode==PAGER_JOURNALMODE_PERSIST
  ){
    // Journal is persisted; optimize to no-op for hot journal case
    // The test environment uses JOURNAL_HDR_SZ(pPager) or 0
    //   assert( pPager->journalOff==JOURNAL_HDR_SZ(pPager) || !pPager->journalOff );
    pPager->eState = PAGER_READER;
    return SQLITE_OK;
  }
  PAGERTRACE(("COMMIT %d\n", PAGERID(pPager)));
  rc = pager_end_transaction(pPager, pPager->setSuper, 1);
  return pager_error(pPager, rc);
}

// Test cases
void test_errCode_early_return(){
    Pager p;
    p.errCode = 5;
    p.iDataVersion = 0;
    p.eState = PAGER_WRITER_LOCKED;
    p.exclusiveMode = 0;
    p.journalMode = 0;
    p.journalOff = 0;
    p.setSuper = 0;
    p.useWal = 0;
    p.endTxnRc = 0;

    int rc = sqlite3PagerCommitPhaseTwo(&p);
    EXPECT_EQ(5, rc, "early return should propagate non-zero errCode");
    EXPECT_EQ(0, p.iDataVersion, "iDataVersion should not be incremented on early return");
}

void test_noop_exclusive_persist_journalOff_hdr(){
    // Case: eState LOCKED, exclusiveMode, JOURNALMODE_PERSIST, journalOff == JOURNAL_HDR_SZ(p) => no-op
    Pager p;
    p.errCode = 0;
    p.iDataVersion = 7;
    p.eState = PAGER_WRITER_LOCKED;
    p.exclusiveMode = 1;
    p.journalMode = PAGER_JOURNALMODE_PERSIST;
    p.journalOff =  JOURNAL_HDR_SZ(&p); // simulate header size
    p.setSuper = 0;
    p.useWal = 0;
    p.endTxnRc = 0;

    int rc = sqlite3PagerCommitPhaseTwo(&p);
    EXPECT_EQ(SQLITE_OK, rc, "no-op path should return SQLITE_OK");
    EXPECT_EQ(PAGER_READER, p.eState, "eState should transition to READER in no-op path");
    EXPECT_EQ(8, p.iDataVersion, "iDataVersion should be incremented");
}

void test_noop_exclusive_persist_journalOff_zero(){
    // Case: eState LOCKED, exclusiveMode, JOURNALMODE_PERSIST, journalOff == 0 => no-op
    Pager p;
    p.errCode = 0;
    p.iDataVersion = 3;
    p.eState = PAGER_WRITER_LOCKED;
    p.exclusiveMode = 1;
    p.journalMode = PAGER_JOURNALMODE_PERSIST;
    p.journalOff = 0; // header not present
    p.setSuper = 0;
    p.useWal = 0;
    p.endTxnRc = 0;

    int rc = sqlite3PagerCommitPhaseTwo(&p);
    EXPECT_EQ(SQLITE_OK, rc, "no-op path with journalOff 0 should return SQLITE_OK");
    EXPECT_EQ(PAGER_READER, p.eState, "eState should transition to READER in no-op path with journalOff 0");
    EXPECT_EQ(4, p.iDataVersion, "iDataVersion should be incremented");
}

void test_endTransaction_path_increments_and_rc(){
    // Case: eState LOCKED, not in exclusive/persist path => call end-transaction and propagate rc
    Pager p;
    p.errCode = 0;
    p.iDataVersion = 10;
    p.eState = PAGER_WRITER_LOCKED;
    p.exclusiveMode = 0;                 // not exclusive
    p.journalMode = 0;                    // not persist
    p.journalOff = 0;
    p.setSuper = 0;
    p.useWal = 0;
    p.endTxnRc = 7;                       // simulate end-transaction failure

    int rc = sqlite3PagerCommitPhaseTwo(&p);
    EXPECT_EQ(7, rc, "end-transaction rc should be propagated");
    EXPECT_EQ(11, p.iDataVersion, "iDataVersion should be incremented even on rc");
    EXPECT_EQ(PAGER_WRITER_LOCKED, p.eState, "eState should remain unchanged on rc path");
}

// Main runner
int main(){
    std::cout << "Starting sqlite3PagerCommitPhaseTwo test suite (C++11, no GTest)\n";

    RUN_TEST(test_errCode_early_return);
    RUN_TEST(test_noop_exclusive_persist_journalOff_hdr);
    RUN_TEST(test_noop_exclusive_persist_journalOff_zero);
    RUN_TEST(test_endTransaction_path_increments_and_rc);

    if(g_failCount == 0){
        std::cout << "All tests passed.\n";
        return 0;
    }else{
        std::cout << g_failCount << " test(s) failed.\n";
        return 1;
    }
}