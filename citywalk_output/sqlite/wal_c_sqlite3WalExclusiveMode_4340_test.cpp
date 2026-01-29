// Minimal C++11 test harness for sqlite3WalExclusiveMode
// This test suite provides a self-contained, compilable environment
// that mimics the essential Wal structure and behavior required by
// the focal method, along with lightweight mocks for the dependent
// WAL locking functions. It exercises true/false branches of the
// sqlite3WalExclusiveMode logic without relying on external test frameworks.

#include <Windows.h>
#include <iostream>
#include <wal.h>
#include <cassert>


// Domain-specific simplifications and redefinitions
// The aim is to faithfully exercise the focal method's control flow.

extern "C" {

// Basic sqlite-like return code
const int SQLITE_OK = 0;

// Wal mode constants (as used by the focal method)
const int WAL_NORMAL_MODE    = 0;
const int WAL_EXCLUSIVE_MODE = 1;
const int WAL_HEAPMEMORY_MODE= 2;

// Helper macro simulating WAL read-lock identification (as in the real code)
#define WAL_READ_LOCK(I) (3 + (I))

// Forward declaration of the WAL structure as used by sqlite3WalExclusiveMode
// The real project uses a larger struct; for unit testing, we only model
// the fields that are read or written by sqlite3WalExclusiveMode.
struct Wal {
    int writeLock;
    int exclusiveMode;
    int readLock;
    int lockError;
};

// Mockable behavior control for dependent locking functions
// These are deliberately simple to keep tests deterministic.
static int g_walLockSharedReturn = SQLITE_OK; // 0 => success, non-zero => failure

// Mock: walLockShared - controls whether acquiring a shared lock succeeds
static int walLockShared(Wal *pWal, int lockIdx) {
    (void)pWal;
    (void)lockIdx;
    return g_walLockSharedReturn;
}

// Mock: walUnlockShared - no-op for unit tests
static void walUnlockShared(Wal *pWal, int lockIdx) {
    (void)pWal;
    (void)lockIdx;
}

// The focal method under test, replicated here for a self-contained test.
// It is kept faithful to the provided code path and behavior.
int sqlite3WalExclusiveMode(Wal *pWal, int op){
  int rc;
  assert( pWal->writeLock==0 );
  assert( pWal->exclusiveMode!=WAL_HEAPMEMORY_MODE || op==-1 );
#ifndef SQLITE_USE_SEH
  assert( pWal->readLock>=0 || pWal->lockError );
#endif
  assert( pWal->readLock>=0 || (op<=0 && pWal->exclusiveMode==0) );
  if( op==0 ){
    if( pWal->exclusiveMode!=WAL_NORMAL_MODE ){
      pWal->exclusiveMode = WAL_NORMAL_MODE;
      if( walLockShared(pWal, WAL_READ_LOCK(pWal->readLock))!=SQLITE_OK ){
        pWal->exclusiveMode = WAL_EXCLUSIVE_MODE;
      }
      rc = pWal->exclusiveMode==WAL_NORMAL_MODE;
    }else{
      /* Already in locking_mode=NORMAL */
      rc = 0;
    }
  }else if( op>0 ){
    assert( pWal->exclusiveMode==WAL_NORMAL_MODE );
    assert( pWal->readLock>=0 );
    walUnlockShared(pWal, WAL_READ_LOCK(pWal->readLock));
    pWal->exclusiveMode = WAL_EXCLUSIVE_MODE;
    rc = 1;
  }else{
    rc = pWal->exclusiveMode==WAL_NORMAL_MODE;
  }
  return rc;
}

// Additional minimal placeholders to satisfy potential linker expectations.
// Not used directly in tests, but included for completeness.
} // extern "C"

///////////////////////////// Unit Test Harness ///////////////////////////////

// Lightweight test framework (non-terminating assertions)
static int g_testsRun = 0;
static int g_testsPassed = 0;

#define TEST_OK(cond, name) do { \
    ++g_testsRun; \
    if (cond) { \
        ++g_testsPassed; \
        std::cout << "[PASS] " << name << "\n"; \
    } else { \
        std::cerr << "[FAIL] " << name << "\n"; \
    } \
} while(0)

// Helper to run a named test with a lambda (C++11 support)
template <typename F>
void runTest(const char* name, F testFunc){
    try {
        testFunc();
        TEST_OK(true, name);
    } catch(...) {
        TEST_OK(false, name);
    }
}

// Accessors/macros to facilitate readable test code
#define EXPECT_EQ(actual, expected, name) runTest(name, [&](){ TEST_OK((actual)==(expected), name); })

// Test 1: op == 0, current mode is EXCLUSIVE and walLockShared succeeds
// Expectation: exclusiveMode becomes NORMAL, rc == 1
void test_op0_exclusiveMode_succeeds() {
    Wal w;
    w.writeLock = 0;
    w.exclusiveMode = WAL_EXCLUSIVE_MODE;
    w.readLock = 2;
    w.lockError = 0;

    g_walLockSharedReturn = SQLITE_OK; // simulate acquire shared lock success

    int rc = sqlite3WalExclusiveMode(&w, 0);

    bool ok_rc = (rc == 1);
    bool ok_mode = (w.exclusiveMode == WAL_NORMAL_MODE);
    if(!(ok_rc && ok_mode)) {
        // produce specific failure details
        if(!ok_rc) std::cerr << "  Expected rc==1 but got " << rc << "\n";
        if(!ok_mode) std::cerr << "  Expected exclusiveMode==WAL_NORMAL_MODE but got " << w.exclusiveMode << "\n";
    }
    TEST_OK(ok_rc && ok_mode, "test_op0_exclusiveMode_succeeds");
}

// Test 2: op == 0, current mode is EXCLUSIVE and walLockShared fails
// Expectation: exclusiveMode remains EXCLUSIVE, rc == 0
void test_op0_exclusiveMode_fails() {
    Wal w;
    w.writeLock = 0;
    w.exclusiveMode = WAL_EXCLUSIVE_MODE;
    w.readLock = 0;
    w.lockError = 0;

    g_walLockSharedReturn = 1; // simulate acquire shared lock failure

    int rc = sqlite3WalExclusiveMode(&w, 0);

    bool ok_rc = (rc == 0);
    bool ok_mode = (w.exclusiveMode == WAL_EXCLUSIVE_MODE);
    if(!(ok_rc && ok_mode)) {
        if(!ok_rc) std::cerr << "  Expected rc==0 but got " << rc << "\n";
        if(!ok_mode) std::cerr << "  Expected exclusiveMode==WAL_EXCLUSIVE_MODE but got " << w.exclusiveMode << "\n";
    }
    TEST_OK(ok_rc && ok_mode, "test_op0_exclusiveMode_fails");
}

// Test 3: op == 0, current mode is NORMAL
// Expectation: rc == 0 and exclusiveMode remains NORMAL
void test_op0_already_normal() {
    Wal w;
    w.writeLock = 0;
    w.exclusiveMode = WAL_NORMAL_MODE;
    w.readLock = 1;
    w.lockError = 0;

    int rc = sqlite3WalExclusiveMode(&w, 0);

    bool ok_rc = (rc == 0);
    bool ok_mode = (w.exclusiveMode == WAL_NORMAL_MODE);
    if(!(ok_rc && ok_mode)) {
        if(!ok_rc) std::cerr << "  Expected rc==0 but got " << rc << "\n";
        if(!ok_mode) std::cerr << "  Expected exclusiveMode==WAL_NORMAL_MODE but got " << w.exclusiveMode << "\n";
    }
    TEST_OK(ok_rc && ok_mode, "test_op0_already_normal");
}

// Test 4: op > 0, promote to EXCLUSIVE from NORMAL
// Preconditions: exclusiveMode == NORMAL and readLock >= 0
// Expectation: walUnlockShared called (simulated), exclusiveMode becomes EXCLUSIVE, rc == 1
void test_op_positive_promote_exclusive() {
    Wal w;
    w.writeLock = 0;
    w.exclusiveMode = WAL_NORMAL_MODE;
    w.readLock = 5;
    w.lockError = 0;

    g_walLockSharedReturn = SQLITE_OK; // Not used in op>0 path except for op==0
    int rc = sqlite3WalExclusiveMode(&w, 1);

    bool ok_rc = (rc == 1);
    bool ok_mode = (w.exclusiveMode == WAL_EXCLUSIVE_MODE);
    if(!(ok_rc && ok_mode)) {
        if(!ok_rc) std::cerr << "  Expected rc==1 but got " << rc << "\n";
        if(!ok_mode) std::cerr << "  Expected exclusiveMode==WAL_EXCLUSIVE_MODE but got " << w.exclusiveMode << "\n";
    }
    TEST_OK(ok_rc && ok_mode, "test_op_positive_promote_exclusive");
}

// Test 5: op < 0, behavior depends on current mode
// Subtest A: starting NORMAL -> rc should be 1
// Subtest B: starting EXCLUSIVE -> rc should be 0
void test_op_negative_branch() {
    // Subtest A
    {
        Wal w;
        w.writeLock = 0;
        w.exclusiveMode = WAL_NORMAL_MODE;
        w.readLock = 2;
        w.lockError = 0;

        int rc = sqlite3WalExclusiveMode(&w, -1);
        bool ok_rc = (rc == 1);
        bool ok_mode = (w.exclusiveMode == WAL_NORMAL_MODE);
        if(!(ok_rc && ok_mode)) {
            if(!ok_rc) std::cerr << "  [A] Expected rc==1 but got " << rc << "\n";
            if(!ok_mode) std::cerr << "  [A] Expected exclusiveMode==WAL_NORMAL_MODE but got " << w.exclusiveMode << "\n";
        }
        TEST_OK(ok_rc && ok_mode, "test_op_negative_branch_subtest_A");
    }

    // Subtest B
    {
        Wal w;
        w.writeLock = 0;
        w.exclusiveMode = WAL_EXCLUSIVE_MODE;
        w.readLock = 3;
        w.lockError = 0;

        int rc = sqlite3WalExclusiveMode(&w, -1);
        bool ok_rc = (rc == 0);
        bool ok_mode = (w.exclusiveMode == WAL_EXCLUSIVE_MODE);
        if(!(ok_rc && ok_mode)) {
            if(!ok_rc) std::cerr << "  [B] Expected rc==0 but got " << rc << "\n";
            if(!ok_mode) std::cerr << "  [B] Expected exclusiveMode==WAL_EXCLUSIVE_MODE but got " << w.exclusiveMode << "\n";
        }
        TEST_OK(ok_rc && ok_mode, "test_op_negative_branch_subtest_B");
    }
}

// Entry point: run all tests
int main() {
    // Run tests
    runTest("test_op0_exclusiveMode_succeeds", test_op0_exclusiveMode_succeeds);
    runTest("test_op0_exclusiveMode_fails", test_op0_exclusiveMode_fails);
    runTest("test_op0_already_normal", test_op0_already_normal);
    runTest("test_op_positive_promote_exclusive", test_op_positive_promote_exclusive);
    runTest("test_op_negative_branch", test_op_negative_branch);

    // Summary
    std::cout << "\nTest Summary: " << g_testsPassed << " / " << g_testsRun << " tests passed.\n";
    return (g_testsRun == g_testsPassed) ? 0 : 1;
}