// Unit tests for sqlite3BtreeCheckpoint
// Target: C++11, no GTest; simple in-file test harness with lightweight expectations.
// The tests provide minimal stubs for the dependent SQLite-like structures and functions
// to exercise the focal logic without requiring the full SQLite library.
//
// Focal method under test (reproduced for testability within this file):
// int sqlite3BtreeCheckpoint(Btree *p, int eMode, int *pnLog, int *pnCkpt)

#include <btreeInt.h>
#include <iostream>
#include <memory>


// Domain knowledge notes (summarized):
// - We need a Btree, which holds a pointer to BtShared (pBt) and a database pointer (db).
// - BtShared contains inTransaction and a Pager pointer (pPager).
// - The function logic:
//   If p is non-null, enter Btree, then if inTransaction != TRANS_NONE return SQLITE_LOCKED,
//   else call sqlite3PagerCheckpoint and propagate its result. Always leave the Btree.
// - We must cover both branches of the condition and verify interactions with pager.
// - We will provide lightweight "static-like" counters to verify static/global interaction points.
// - We will not rely on any external test frameworks; use a tiny assertion framework.

// Candidate Keywords (core dependencies)
 // Btree, BtShared, Pager, Db
 // sqlite3BtreeEnter, sqlite3BtreeLeave
 // sqlite3PagerCheckpoint
 // TRANS_NONE, SQLITE_OK, SQLITE_LOCKED
 // eMode, pnLog, pnCkpt
 // NULL p pointer branch, locked branch, and success branch

// -------------------- Mocked Dependencies and State --------------------

struct Pager {};
struct Db {};

struct BtShared {
    int inTransaction;
    Pager *pPager;
};

struct Btree {
    BtShared *pBt;
    Db *db;
};

// Constants to mimic SQLite's return codes and transaction state
const int TRANS_NONE = 0;
const int SQLITE_OK = 0;
const int SQLITE_LOCKED = 6;

// Global counters to verify interactions (simulating internal static/stateful behavior)
static int g_btree_enter_calls = 0;
static int g_btree_leave_calls = 0;
static int g_pager_checkpoint_calls = 0;

// Lightweight "fake" implementations of the dependencies used by sqlite3BtreeCheckpoint
extern "C" void sqlite3BtreeEnter(Btree * /*p*/){
    ++g_btree_enter_calls;
}
extern "C" void sqlite3BtreeLeave(Btree * /*p*/){
    ++g_btree_leave_calls;
}

// Mocked pager checkpoint: mimic behavior and record calls
extern "C" int sqlite3PagerCheckpoint(Pager * /*pPager*/, Db * /*db*/, int /*eMode*/, int *pnLog, int *pnCkpt){
    ++g_pager_checkpoint_calls;
    // Fill outputs if provided to simulate real behavior
    if(pnLog) *pnLog = 7;
    if(pnCkpt) *pnCkpt = 9;
    // Return a non-zero status to distinguish from SQLITE_OK in tests
    return 42;
}

// -------------------- Focal Method Under Test --------------------
// Reproduced here to create a self-contained test harness without depending on external files.
// This mirrors the provided <FOCAL_METHOD> exactly (adjusted for the test harness environment).
int sqlite3BtreeCheckpoint(Btree *p, int eMode, int *pnLog, int *pnCkpt){
  int rc = SQLITE_OK;
  if( p ){
    BtShared *pBt = p->pBt;
    sqlite3BtreeEnter(p);
    if( pBt->inTransaction!=TRANS_NONE ){
      rc = SQLITE_LOCKED;
    }else{
      rc = sqlite3PagerCheckpoint(pBt->pPager, p->db, eMode, pnLog, pnCkpt);
    }
    sqlite3BtreeLeave(p);
  }
  return rc;
}

// -------------------- Test Harness --------------------

static int gFailures = 0;

// Simple expectation macro: prints PASS/FAIL without terminating the test
#define EXPECT_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        ++gFailures; \
        std::cerr << "FAIL: " << (desc) << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } else { \
        std::cout << "PASS: " << (desc) << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond, desc) do { \
    if (!(cond)) { \
        ++gFailures; \
        std::cerr << "FAIL: " << (desc) << " | Condition is false" << std::endl; \
    } else { \
        std::cout << "PASS: " << (desc) << std::endl; \
    } \
} while(0)

// Helpers to construct/destroy a minimal Btree environment
static Btree* createBtree(int inTrans){
    Btree *p = new Btree();
    BtShared *pBt = new BtShared();
    Pager *pPager = new Pager();
    pBt->inTransaction = inTrans;
    pBt->pPager = pPager;
    p->pBt = pBt;
    p->db = new Db();
    return p;
}
static void destroyBtree(Btree *p){
    if(!p) return;
    delete p->pBt->pPager;
    delete p->pBt;
    delete p->db;
    delete p;
}
static void resetCounters(){
    g_btree_enter_calls = 0;
    g_btree_leave_calls = 0;
    g_pager_checkpoint_calls = 0;
}

// Test 1: When p is NULL, function should return SQLITE_OK and not touch outputs
void test_null_pointer_p(){
    resetCounters();
    int log = 0;
    int ckpt = 0;
    int rc = sqlite3BtreeCheckpoint(nullptr, 0, &log, &ckpt);
    EXPECT_EQ(rc, SQLITE_OK, "NULL p should return SQLITE_OK");
    EXPECT_EQ(log, 0, "NULL p should not modify log pointer value");
    EXPECT_EQ(ckpt, 0, "NULL p should not modify ckpt pointer value");
    EXPECT_EQ(g_btree_enter_calls, 0, "ENTER should not be called for NULL p");
    EXPECT_EQ(g_btree_leave_calls, 0, "LEAVE should not be called for NULL p");
    EXPECT_EQ(g_pager_checkpoint_calls, 0, "PagerCheckpoint should not be called for NULL p");
}

// Test 2: When inTransaction != TRANS_NONE, should return SQLITE_LOCKED and not call pager
void test_locked_branch(){
    resetCounters();
    Btree *p = createBtree(1); // inTransaction != TRANS_NONE
    int log = 0;
    int ckpt = 0;
    int rc = sqlite3BtreeCheckpoint(p, 0, &log, &ckpt);
    destroyBtree(p);

    EXPECT_EQ(rc, SQLITE_LOCKED, "Locked branch should return SQLITE_LOCKED");
    // Ensure pager not invoked
    EXPECT_EQ(g_pager_checkpoint_calls, 0, "PagerCheckpoint should not be called when locked");
    // ENTER/LEAVE should be invoked exactly once each
    EXPECT_EQ(g_btree_enter_calls, 1, "ENTER should be called exactly once in locked branch");
    EXPECT_EQ(g_btree_leave_calls, 1, "LEAVE should be called exactly once in locked branch");
}

// Test 3: When inTransaction == TRANS_NONE, should delegate to pager checkpoint and propagate its result
void test_ok_branch_with_valid_pager(){
    resetCounters();
    Btree *p = createBtree(TRANS_NONE); // no active transaction
    int log = 0;
    int ckpt = 0;
    int rc = sqlite3BtreeCheckpoint(p, 5, &log, &ckpt);
    destroyBtree(p);

    // PagerCheckpoint is mocked to return 42 and set log/ckpt to 7/9
    EXPECT_EQ(rc, 42, "OK branch should return pager's return code (42)");
    EXPECT_EQ(log, 7, "PagerCheckpoint should set log to 7 when non-null");
    EXPECT_EQ(ckpt, 9, "PagerCheckpoint should set ckpt to 9 when non-null");
    EXPECT_EQ(g_pager_checkpoint_calls, 1, "PagerCheckpoint should be called exactly once");
    EXPECT_EQ(g_btree_enter_calls, 1, "ENTER should be called exactly once");
    EXPECT_EQ(g_btree_leave_calls, 1, "LEAVE should be called exactly once");
}

// Test 4: When inTransaction == TRANS_NONE and outputs are NULL, should still return pager result and not crash
void test_ok_branch_with_null_outputs(){
    resetCounters();
    Btree *p = createBtree(TRANS_NONE); // no active transaction
    int rc = sqlite3BtreeCheckpoint(p, 0, nullptr, nullptr);
    destroyBtree(p);

    EXPECT_EQ(rc, 42, "OK branch with NULL outputs should still return pager's code (42)");
    EXPECT_EQ(g_pager_checkpoint_calls, 1, "PagerCheckpoint should be called once even if outputs are NULL");
    EXPECT_EQ(g_btree_enter_calls, 1, "ENTER should be called once in this path");
    EXPECT_EQ(g_btree_leave_calls, 1, "LEAVE should be called once in this path");
}

// Run all tests and summarize
int main(){
    std::cout << "Running sqlite3BtreeCheckpoint unit tests (C++11, no GTest)" << std::endl;

    test_null_pointer_p();
    test_locked_branch();
    test_ok_branch_with_valid_pager();
    test_ok_branch_with_null_outputs();

    if (gFailures == 0){
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}