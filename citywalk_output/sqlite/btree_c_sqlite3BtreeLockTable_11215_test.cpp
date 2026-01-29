// Unit test suite for sqlite3BtreeLockTable
// This test suite provides a minimal, self-contained environment to
// exercise the focal method logic without requiring the full SQLite source.
// It uses simple mock implementations for dependencies and a lightweight
// test harness that performs non-terminating assertions.

#include <btreeInt.h>
#include <iostream>
#include <cassert>
#include <string>


// Domain-specific types and constants (simplified for tests)
typedef unsigned char u8;
typedef unsigned int Pgno;

#define SQLITE_OK 0
#define READ_LOCK 0
#define WRITE_LOCK 1
#define TRANS_NONE 0

// Forward declaration of the focal class dependency (mocked)
struct Btree;

// Global test harness state (mocks)
static int g_enterCount = 0;
static int g_leaveCount = 0;
static int g_queryCalled = 0;
static int g_setLockCalled = 0;
static int g_nextQueryResult = SQLITE_OK;
static int g_lastLockType = -1;
static int g_lastTab = -1;

// Mocked dependency implementations (global linkage)
int sqlite3BtreeEnter(Btree *p){
    ++g_enterCount;
    return SQLITE_OK;
}
int sqlite3BtreeLeave(Btree *p){
    ++g_leaveCount;
    return SQLITE_OK;
}

// The focal method's dependencies (mocks) and the test targets
static int querySharedCacheTableLock(Btree *p, Pgno iTab, u8 eLock);
static int setSharedCacheTableLock(Btree *p, Pgno iTable, u8 eLock);

// The focal method under test (reproduced here for a self-contained test)
struct Btree {
    int inTrans;
    int sharable;
};

int sqlite3BtreeLockTable(Btree *p, int iTab, u8 isWriteLock){
  int rc = SQLITE_OK;
  assert( p->inTrans!=TRANS_NONE );
  if( p->sharable ){
    u8 lockType = READ_LOCK + isWriteLock;
    assert( READ_LOCK+1==WRITE_LOCK );
    assert( isWriteLock==0 || isWriteLock==1 );
    sqlite3BtreeEnter(p);
    rc = querySharedCacheTableLock(p, iTab, lockType);
    if( rc==SQLITE_OK ){
      rc = setSharedCacheTableLock(p, iTab, lockType);
    }
    sqlite3BtreeLeave(p);
  }
  return rc;
}

// Mock implementations for dependencies used by sqlite3BtreeLockTable
static int querySharedCacheTableLock(Btree *p, Pgno iTab, u8 eLock){
    g_queryCalled = 1;
    g_lastLockType = (int)eLock;
    g_lastTab = (int)iTab;
    return g_nextQueryResult;
}
static int setSharedCacheTableLock(Btree *p, Pgno iTable, u8 eLock){
    g_setLockCalled = 1;
    g_lastLockType = (int)eLock;
    g_lastTab = (int)iTable;
    return SQLITE_OK;
}

// Tiny test framework (non-terminating, prints failures but continues)
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { std::cerr << "TEST FAILURE: " << msg << std::endl; return false; } \
} while(0)

static void resetMocks(){
    g_enterCount = 0;
    g_leaveCount = 0;
    g_queryCalled = 0;
    g_setLockCalled = 0;
    g_lastLockType = -1;
    g_lastTab = -1;
    // keep g_nextQueryResult as set by test to simulate ok/error
}

// Test 1: When p->sharable is false, no locking operations occur.
// Expect rc == SQLITE_OK and no calls to enter/leave or query/setLock.
bool test_NotSharableNoLocks(){
    resetMocks();
    Btree p;
    p.inTrans = 1; // non TRANS_NONE to satisfy assertion
    p.sharable = 0; // false branch
    int rc = sqlite3BtreeLockTable(&p, 5, 0);
    TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when not sharable");
    TEST_ASSERT(g_enterCount == 0, "enter should not be called when not sharable");
    TEST_ASSERT(g_leaveCount == 0, "leave should not be called when not sharable");
    TEST_ASSERT(g_queryCalled == 0, "query should not be called when not sharable");
    TEST_ASSERT(g_setLockCalled == 0, "setLock should not be called when not sharable");
    return true;
}

// Test 2: When sharing is enabled and query succeeds, both query and setLock should be called.
// Also verify that lockType is computed as READ_LOCK + isWriteLock.
bool test_SharableQueryOk_SetLock(int isWriteLock){
    resetMocks();
    Btree p;
    p.inTrans = 1;
    p.sharable = 1;
    int iTab = 7;
    g_nextQueryResult = SQLITE_OK;
    int rc = sqlite3BtreeLockTable(&p, iTab, (u8)isWriteLock);
    TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when query and setLock succeed");
    TEST_ASSERT(g_enterCount == 1, "enter should be called once");
    TEST_ASSERT(g_leaveCount == 1, "leave should be called once");
    TEST_ASSERT(g_queryCalled == 1, "query should be called once");
    TEST_ASSERT(g_setLockCalled == 1, "setLock should be called once");
    TEST_ASSERT(g_lastTab == iTab, "queried tab should be recorded");
    TEST_ASSERT(g_lastLockType == (READ_LOCK + isWriteLock), "lockType should be READ_LOCK + isWriteLock");
    return true;
}

// Test 3: When query fails (non-SQLITE_OK), setLock should not be called, rc should reflect error.
bool test_SharableQueryNotOk(){
    resetMocks();
    Btree p;
    p.inTrans = 1;
    p.sharable = 1;
    int iTab = 9;
    g_nextQueryResult = 1; // simulate error
    int rc = sqlite3BtreeLockTable(&p, iTab, 0);
    TEST_ASSERT(rc == 1, "rc should propagate error from query (non-SQLITE_OK)");
    TEST_ASSERT(g_enterCount == 1, "enter should be called once when sharable");
    TEST_ASSERT(g_leaveCount == 1, "leave should be called once when sharable");
    TEST_ASSERT(g_queryCalled == 1, "query should be called once");
    TEST_ASSERT(g_setLockCalled == 0, "setLock should not be called when query fails");
    return true;
}

// Runner
int main(){
    int total = 0;
    int failed = 0;

    std::cout << "Running sqlite3BtreeLockTable unit tests (self-contained mocks)" << std::endl;

    // Test 1
    total++;
    if(!test_NotSharableNoLocks()){
        ++failed;
    } else {
        std::cout << "Test 1 passed: NotSharableNoLocks" << std::endl;
    }

    // Test 2a: isWriteLock = 0
    total++;
    if(!test_SharableQueryOk_SetLock(0)){
        ++failed;
    } else {
        std::cout << "Test 2 passed: SharableQueryOk_SetLock with isWriteLock=0" << std::endl;
    }

    // Test 2b: isWriteLock = 1
    total++;
    if(!test_SharableQueryOk_SetLock(1)){
        ++failed;
    } else {
        std::cout << "Test 2 passed: SharableQueryOk_SetLock with isWriteLock=1" << std::endl;
    }

    // Test 3
    total++;
    if(!test_SharableQueryNotOk()){
        ++failed;
    } else {
        std::cout << "Test 3 passed: SharableQueryNotOk" << std::endl;
    }

    // Summary
    std::cout << "Tests run: " << total << ", Failures: " << failed << std::endl;
    return failed;
}