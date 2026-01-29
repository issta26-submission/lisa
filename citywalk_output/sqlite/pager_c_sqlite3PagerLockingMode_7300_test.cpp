// C++11 unit test for the focal method sqlite3PagerLockingMode
// Note: This test provides a self-contained, minimal mock environment
// that replicates just enough of the real SQLite types and behavior
// to exercise the core logic of the locking mode switching.
// It does not rely on GTest or any external framework.

#include <iostream>
#include <wal.h>
#include <sqliteInt.h>
#include <cstdint>


// Domain-specific placeholders and minimal definitions
typedef uint8_t u8;

// Candidate Keywords (core components):
// - Pager: holds exclusiveMode, tempFile, pWal
// - Wal: opaque WAL structure
// - sqlite3WalHeapMemory(pWal): reports whether the WAL heap is resident in memory
// - PAGER_LOCKINGMODE_QUERY, NORMAL, EXCLUSIVE constants
// - sqlite3PagerLockingMode(Pager*, int): the function under test

// Minimal constants matching the intent of the original code
const int PAGER_LOCKINGMODE_QUERY     = -1;
const int PAGER_LOCKINGMODE_NORMAL    = 0;
const int PAGER_LOCKINGMODE_EXCLUSIVE = 1;

// Forward declaration of WAL type
struct Wal {};

// Global hook to control WAL memory behavior for tests
static bool g_wal_heap_memory = false;

// Mocked WAL memory query used by sqlite3PagerLockingMode
int sqlite3WalHeapMemory(Wal* /*pWal*/) {
    // Return non-zero if the WAL heap is memory-resident
    return g_wal_heap_memory ? 1 : 0;
}

// Definition of the Pager struct (only fields used by the focal method)
struct Pager {
    u8 exclusiveMode;    // file-scope type used by sqlite3PagerLockingMode
    int tempFile;          // non-zero means a temporary file is used
    Wal* pWal;             // associated WAL object
};

// The focal method under test (embedded here for self-contained testing)
int sqlite3PagerLockingMode(Pager *pPager, int eMode){
  // Keep behavior identical to the provided code snippet
  // Assertions are kept to mimic the original contract; test cases avoid triggering them.
  assert( eMode==PAGER_LOCKINGMODE_QUERY
            || eMode==PAGER_LOCKINGMODE_NORMAL
            || eMode==PAGER_LOCKINGMODE_EXCLUSIVE );
  assert( PAGER_LOCKINGMODE_QUERY<0 );
  assert( PAGER_LOCKINGMODE_NORMAL>=0 && PAGER_LOCKINGMODE_EXCLUSIVE>=0 );
  assert( pPager->exclusiveMode || 0==sqlite3WalHeapMemory(pPager->pWal) );
  if( eMode>=0 && !pPager->tempFile && !sqlite3WalHeapMemory(pPager->pWal) ){
    pPager->exclusiveMode = (u8)eMode;
  }
  return (int)pPager->exclusiveMode;
}

// Simple non-terminating test harness (no GTest)
// Provides four targeted test scenarios to cover true/false branches
// and both branches of the assignment inside the if condition.
static bool test_case1() {
    // Case 1: eMode = NORMAL, wal heap memory NOT used, no temp file
    // Expect exclusiveMode to be set to eMode and return value equals eMode.
    Pager p;
    p.exclusiveMode = 7; // non-zero to satisfy the assertion with wal mem false
    p.tempFile = 0;
    Wal w;
    p.pWal = &w;

    g_wal_heap_memory = false; // simulate no WAL heap memory
    int ret = sqlite3PagerLockingMode(&p, PAGER_LOCKINGMODE_NORMAL);

    bool cond = (ret == 0) && (p.exclusiveMode == 0);
    if (!cond) {
        std::cerr << "Test Case 1 Failed: expected return 0 and exclusiveMode 0, got ret="<<ret<<" exclusiveMode="<<int(p.exclusiveMode)<<"\n";
    }
    return cond;
}

static bool test_case2() {
    // Case 2: wal heap memory is in use; should NOT change exclusiveMode
    // eMode = EXCLUSIVE, but since wal heap memory is present, the if condition
    // should not fire and exclusiveMode should remain unchanged.
    Pager p;
    p.exclusiveMode = 123;
    p.tempFile = 0;
    Wal w;
    p.pWal = &w;

    g_wal_heap_memory = true; // simulate WAL heap memory is in use
    int ret = sqlite3PagerLockingMode(&p, PAGER_LOCKINGMODE_EXCLUSIVE);

    bool cond = (ret == 123) && (p.exclusiveMode == 123);
    if (!cond) {
        std::cerr << "Test Case 2 Failed: expected return 123 and exclusiveMode 123, got ret="<<ret<<" exclusiveMode="<<int(p.exclusiveMode)<<"\n";
    }
    return cond;
}

static bool test_case3() {
    // Case 3: tempFile is set; should NOT change exclusiveMode even if wal heap false
    // eMode = NORMAL, but since tempFile is true, the assignment should not happen.
    Pager p;
    p.exclusiveMode = 42;
    p.tempFile = 1; // temp file active
    Wal w;
    p.pWal = &w;

    g_wal_heap_memory = false;
    int ret = sqlite3PagerLockingMode(&p, PAGER_LOCKINGMODE_NORMAL);

    bool cond = (ret == 42) && (p.exclusiveMode == 42);
    if (!cond) {
        std::cerr << "Test Case 3 Failed: expected return 42 and exclusiveMode 42, got ret="<<ret<<" exclusiveMode="<<int(p.exclusiveMode)<<"\n";
    }
    return cond;
}

static bool test_case4() {
    // Case 4: eMode = EXCLUSIVE, normal path (no wal memory, no temp file)
    // initial exclusiveMode is non-zero; expect it to change to eMode and return eMode.
    Pager p;
    p.exclusiveMode = 2;
    p.tempFile = 0;
    Wal w;
    p.pWal = &w;

    g_wal_heap_memory = false;
    int ret = sqlite3PagerLockingMode(&p, PAGER_LOCKINGMODE_EXCLUSIVE);

    bool cond = (ret == 1) && (p.exclusiveMode == 1);
    if (!cond) {
        std::cerr << "Test Case 4 Failed: expected return 1 and exclusiveMode 1, got ret="<<ret<<" exclusiveMode="<<int(p.exclusiveMode)<<"\n";
    }
    return cond;
}

int main() {
    // Execute all test cases and report summary
    int total = 0;
    int passed = 0;

    std::cout << "Running sqlite3PagerLockingMode unit tests (self-contained, no GTest)..." << std::endl;

    total++;
    if (test_case1()) {
        passed++;
        std::cout << "[PASS] Test Case 1: Normal path sets exclusiveMode when no WAL memory." << std::endl;
    } else {
        std::cout << "[FAIL] Test Case 1" << std::endl;
    }

    total++;
    if (test_case2()) {
        passed++;
        std::cout << "[PASS] Test Case 2: WAL heap memory present prevents change." << std::endl;
    } else {
        std::cout << "[FAIL] Test Case 2" << std::endl;
    }

    total++;
    if (test_case3()) {
        passed++;
        std::cout << "[PASS] Test Case 3: tempFile prevents change even with no WAL memory." << std::endl;
    } else {
        std::cout << "[FAIL] Test Case 3" << std::endl;
    }

    total++;
    if (test_case4()) {
        passed++;
        std::cout << "[PASS] Test Case 4: EXCLUSIVE path updates from non-zero to eMode." << std::endl;
    } else {
        std::cout << "[FAIL] Test Case 4" << std::endl;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}