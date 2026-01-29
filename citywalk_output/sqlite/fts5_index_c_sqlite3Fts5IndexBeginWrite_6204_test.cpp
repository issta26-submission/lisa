// sqlite3Fts5IndexBeginWrite unit tests (C++11, no GoogleTest)
// This test suite targets the focal method:
//   int sqlite3Fts5IndexBeginWrite(Fts5Index *p, int bDelete, i64 iRowid)
// The tests rely on the real Fts5Index/Fts5Config types and the actual
// implementation in fts5_index.c (linked together in the build).
// The static/internal helpers in fts5_index.c are not mocked; we exercise
// the public behavior exposed by sqlite3Fts5IndexBeginWrite and observe the
// state changes on Fts5Index to infer correct branching.
//
// Notes:
// - We assume sqlite3Int64 (sqlite3_int64) is the i64 alias used by the project.
// - We make minimal, non-terminating assertions (log-only) to maximize
//   execution coverage without aborting tests on first failure.
// - We rely on the real project headers to define Fts5Index and Fts5Config.
//   Include paths should be configured in the build system.
//
// Compile example (conceptual):
//   g++ -std=c++11 -I<path-to-headers> test_fts5_begin_write.cpp fts5_index.c -lsqlite3 -o test_fts5_begin_write
//
// The test output will enumerate passed/failed tests and provide a summary.

#include <vector>
#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Include SQLite3 header for SQLITE_OK and sqlite3_int64 types

// Include project internal types
// Forward declare the function under test with C linkage
extern "C" int sqlite3Fts5IndexBeginWrite(Fts5Index *p, int bDelete, sqlite3_int64 iRowid);

// Simple test runtime framework (non-GTest)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TEST_PASSED() do { \
    ++g_totalTests; \
    std::cout << "[PASS] " << __func__ << " - " << __LINE__ << "\n"; \
} while(0)

#define TEST_FAILED(msg) do { \
    ++g_totalTests; \
    ++g_failedTests; \
    std::cerr << "[ERR ] " << __func__ << " - " << __LINE__ << ": " << msg << "\n"; \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)){ TEST_FAILED(msg); } else { /* no-op */ } \
} while(0)


// Helper: reset an Fts5Index instance for a clean test
static void resetIndex(Fts5Index *p, sqlite3_int64 iWrite, int bDel, int nPendingData, int nPendingRow) {
    // Clear and reinitialize fields used by sqlite3Fts5IndexBeginWrite
    // We rely on real structure layout from the project's headers.
    p->rc = SQLITE_OK;
    p->pHash = 0;
    // p->pConfig should be already set by test, but ensure iif not NULL
    // when using the function, p->pConfig must be non-null; we won't modify here.
    p->iWriteRowid = iWrite;
    p->bDelete = bDel;
    p->nPendingData = nPendingData;
    p->nPendingRow = nPendingRow;
    // Note: p->pHash==0 triggers allocation inside sqlite3Fts5IndexBeginWrite
    // so keep it as 0 for tests that need hashing allocation.
}


// Test 1: When p->pHash is NULL and iRowid < p->iWriteRowid, a hash is allocated
// and the write is prepared for the given iRowid. nPendingRow increments if not deleting.
static void test_BeginWrite_hashAllocWhenNull_and_iRowidLessThanWrite() {
    // Arrange
    Fts5Index idx;
    // Initialize a minimal valid configuration
    // We rely on the project's types: Fts5Config and its fields
    Fts5Config cfg;
    std::memset(&cfg, 0, sizeof(Fts5Config));
    cfg.nHashSize = 4; // small for test
    // Link config into index
    idx.rc = SQLITE_OK;
    idx.pHash = 0;              // forces allocation path
    idx.pConfig = &cfg;
    idx.iWriteRowid = 100;        // current write row boundary
    idx.bDelete = 0;
    idx.nPendingData = 0;
    idx.nPendingRow = 0;

    sqlite3_int64 iRowid = 50;    // iRowid < iWriteRowid triggers flush

    g_totalTests++;

    // Act
    int rc = sqlite3Fts5IndexBeginWrite(&idx, 0, iRowid);

    // Assert: state transitions expected by code path
    bool hashAllocated = (idx.pHash != 0);
    bool iWriteUpdated = (idx.iWriteRowid == iRowid);
    bool bDeleteStored = (idx.bDelete == 0);
    bool pendingIncremented = (idx.nPendingRow == 1);

    if (rc == 0 && hashAllocated && iWriteUpdated && bDeleteStored && pendingIncremented) {
        TEST_PASSED();
    } else {
        std::string msg = "Test1 failed: ";
        if (!(rc == 0)) msg += "return code not zero; ";
        if (!hashAllocated) msg += "hash not allocated; ";
        if (!iWriteUpdated) msg += "iWriteRowid not updated; ";
        if (!bDeleteStored) msg += "bDelete not stored correctly; ";
        if (!pendingIncremented) msg += "nPendingRow not incremented; ";
        TEST_FAILED(msg);
    }
}

// Test 2: When iRowid equals iWriteRowid and bDelete == 0, triggers flush path.
// The test ensures iWriteRowid remains the same after setting to iRowid and PendingRow increments.
static void test_BeginWrite_flush_on_sameRowid_and_notDeleting() {
    // Arrange
    Fts5Index idx;
    Fts5Config cfg;
    std::memset(&cfg, 0, sizeof(Fts5Config));
    cfg.nHashSize = 8;
    idx.rc = SQLITE_OK;
    idx.pHash = 0;
    idx.pConfig = &cfg;
    idx.iWriteRowid = 60;
    idx.bDelete = 0;     // not deleting -> pending row should increment
    idx.nPendingData = 0;
    idx.nPendingRow = 2;

    sqlite3_int64 iRowid = 60; // equal to current write rowid

    g_totalTests++;

    // Act
    int rc = sqlite3Fts5IndexBeginWrite(&idx, 0, iRowid);

    // Assert
    bool iWriteEquals = (idx.iWriteRowid == iRowid);
    bool pendingIncremented = (idx.nPendingRow == 3); // incremented by 1 from 2
    bool bDeleteStored = (idx.bDelete == 0);

    if (rc == 0 && iWriteEquals && pendingIncremented && bDeleteStored) {
        TEST_PASSED();
    } else {
        std::string msg = "Test2 failed: ";
        if (!(rc == 0)) msg += "return code not zero; ";
        if (!iWriteEquals) msg += "iWriteRowid not preserved; ";
        if (!pendingIncremented) msg += "nPendingRow not incremented; ";
        if (!bDeleteStored) msg += "bDelete not stored correctly; ";
        TEST_FAILED(msg);
    }
}

// Test 3: When nPendingData > nHashSize, triggers a flush.
// We ensure that the scenario is reached even if iRowid is greater than iWriteRowid.
static void test_BeginWrite_flush_when_pendingData_exceeds_hashSize() {
    // Arrange
    Fts5Index idx;
    Fts5Config cfg;
    std::memset(&cfg, 0, sizeof(Fts5Config));
    cfg.nHashSize = 2;
    idx.rc = SQLITE_OK;
    idx.pHash = 0;
    idx.pConfig = &cfg;
    idx.iWriteRowid = 10;
    idx.bDelete = 1;       // deleting; nPendingRow should NOT increment
    idx.nPendingData = 3;    // > nHashSize => flush
    idx.nPendingRow = 0;

    sqlite3_int64 iRowid = 20;

    g_totalTests++;

    // Act
    int rc = sqlite3Fts5IndexBeginWrite(&idx, 1, iRowid);

    // Assert
    bool iWriteUpdated = (idx.iWriteRowid == iRowid);
    bool pendingUnchanged = (idx.nPendingRow == 0);
    bool bDeleteStored = (idx.bDelete == 1);

    if (rc == 0 && iWriteUpdated && pendingUnchanged && bDeleteStored) {
        TEST_PASSED();
    } else {
        std::string msg = "Test3 failed: ";
        if (!(rc == 0)) msg += "return code not zero; ";
        if (!iWriteUpdated) msg += "iWriteRowid not updated; ";
        if (!pendingUnchanged) msg += "nPendingRow changed unexpectedly; ";
        if (!bDeleteStored) msg += "bDelete not stored correctly; ";
        TEST_FAILED(msg);
    }
}

// Test 4: None of the flush conditions apply; iWriteRowid should update to iRowid,
// bDelete should be stored, and nPendingRow should increment only if not deleting.
static void test_BeginWrite_no_flush_when_conditions_not_met() {
    // Arrange: iRowid > iWriteRowid, nPendingData <= nHashSize, and bDelete == 0 would still
    // trigger nPendingRow increment; we want to ensure flush does not trigger by other conditions.
    Fts5Index idx;
    Fts5Config cfg;
    std::memset(&cfg, 0, sizeof(Fts5Config));
    cfg.nHashSize = 16;
    idx.rc = SQLITE_OK;
    idx.pHash = 0;
    idx.pConfig = &cfg;
    idx.iWriteRowid = 30;
    idx.bDelete = 1;       // deleting; nPendingRow should not increment
    idx.nPendingData = 5;    // <= nHashSize
    idx.nPendingRow = 7;

    sqlite3_int64 iRowid = 40; // greater than iWriteRowid and not deleting

    g_totalTests++;

    // Act
    int rc = sqlite3Fts5IndexBeginWrite(&idx, 1, iRowid);

    // Assert
    bool iWriteUpdated = (idx.iWriteRowid == iRowid);
    bool pendingUnchanged = (idx.nPendingRow == 7);
    bool bDeleteStored = (idx.bDelete == 1);

    if (rc == 0 && iWriteUpdated && pendingUnchanged && bDeleteStored) {
        TEST_PASSED();
    } else {
        std::string msg = "Test4 failed: ";
        if (!(rc == 0)) msg += "return code not zero; ";
        if (!iWriteUpdated) msg += "iWriteRowid not updated; ";
        if (!pendingUnchanged) msg += "nPendingRow changed unexpectedly; ";
        if (!bDeleteStored) msg += "bDelete not stored correctly; ";
        TEST_FAILED(msg);
    }
}


// Entry point
int main() {
    std::cout << "Running sqlite3Fts5IndexBeginWrite unit tests (C++11, no GTest)\n";
    test_BeginWrite_hashAllocWhenNull_and_iRowidLessThanWrite();
    test_BeginWrite_flush_on_sameRowid_and_notDeleting();
    test_BeginWrite_flush_when_pendingData_exceeds_hashSize();
    test_BeginWrite_no_flush_when_conditions_not_met();

    std::cout << "Tests completed. Total=" << g_totalTests
              << ", Failed=" << g_failedTests << "\n";

    // Return non-zero if any test failed
    return (g_failedTests == 0) ? 0 : 1;
}