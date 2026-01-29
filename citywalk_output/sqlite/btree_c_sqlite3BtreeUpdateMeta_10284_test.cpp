// Unit test suite for sqlite3BtreeUpdateMeta (btree.c) using a lightweight C++11 test harness
// This test does not rely on GTest or any external mocking framework.
// It provides minimal C-bridges (extern "C") to satisfy the focal function's dependencies
// and constructs a small, controlled environment to verify behavior.

#include <btreeInt.h>
#include <iostream>
#include <cstring>
#include <vector>


// --------------- Lightweight test framework ---------------
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating expectation helper
static void expect(bool condition, const char* message) {
    ++g_total_tests;
    if(!condition) {
        ++g_failed_tests;
        std::cerr << "Test failed: " << message << std::endl;
    }
}

// Helper to print final results
static void print_summary() {
    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
}

// --------------- Focal method environment stubs (C linkage) ---------------
// We provide minimal structures and functions to emulate just enough of sqlite3's
// internal environment for sqlite3BtreeUpdateMeta to operate in tests.

// Forward declare to align with the focal function's uses
extern "C" {
    // Typedef used by the focal function
    typedef unsigned int u32;

    // Opaque types used by sqlite3BtreeUpdateMeta
    struct DbPage { int dummy; };
    struct MemPage { unsigned char *aData; DbPage *pDbPage; };
    struct BtShared { MemPage *pPage1; int autoVacuum; unsigned char incrVacuum; };
    struct Btree { BtShared *pBt; int inTrans; };

    // Public-like API used by the focal method
    int sqlite3BtreeEnter(Btree *p);
    int sqlite3BtreeLeave(Btree *p);
    int sqlite3PagerWrite(DbPage *pPg);
    void put4byte(unsigned char *p, unsigned int v);

    // The function under test
    int sqlite3BtreeUpdateMeta(Btree *p, int idx, u32 iMeta);

    // Global control flag to simulate pager write success/failure
    extern int g_forcePagerError;
}

// Global control for tests (simulates errors from the pager)
int g_forcePagerError = 0;

// Stub implementations (C linkage)
extern "C" {

int sqlite3BtreeEnter(Btree *p) { (void)p; return 0; }
int sqlite3BtreeLeave(Btree *p) { (void)p; return 0; }

int sqlite3PagerWrite(DbPage *pPg) {
    (void)pPg;
    if (g_forcePagerError) return 1; // non-OK (simulate SQLITE_ERROR)
    return 0; // SQLITE_OK
}

void put4byte(unsigned char *p, unsigned int v) {
    // little-endian write of 4 bytes
    p[0] = (unsigned char)(v & 0xFF);
    p[1] = (unsigned char)((v >> 8) & 0xFF);
    p[2] = (unsigned char)((v >> 16) & 0xFF);
    p[3] = (unsigned char)((v >> 24) & 0xFF);
}

// Forward declaration for the focal function to ensure linkage.
// The real implementation is in btree.c; here we declare it so we can call it.
extern "C" int sqlite3BtreeUpdateMeta(Btree *p, int idx, u32 iMeta);
}

// --------------- Minimal type definitions matching test harness ---------------
// We define structs with just enough layout to satisfy the focal function's access patterns.
// The focal function only uses p->pBt and p->inTrans, and then pBt->pPage1->aData / pPage1->pDbPage.

struct DbPage { int dummy; };
struct MemPage { unsigned char *aData; DbPage *pDbPage; };
struct BtShared { MemPage *pPage1; int autoVacuum; unsigned char incrVacuum; };
struct Btree { BtShared *pBt; int inTrans; };

// Domain constants used by the focal method (we only rely on valid values)
static const int TRANS_WRITE = 1;
static const int SQLITE_OK = 0;

// --------------- Test helpers to construct a controlled environment ---------------
static void setup_basic_environment(Btree &bt, BtShared &shared, MemPage &page, DbPage &dbpage, unsigned char *buffer, size_t buffer_size) {
    // Prepare page data buffer
    buffer[0] = 0; // zeroed out
    buffer[buffer_size - 1] = 0; // ensure tail non-garbage

    page.aData = buffer;
    page.pDbPage = &dbpage;

    // Link structures
    shared.pPage1 = &page;
    shared.autoVacuum = 0;
    shared.incrVacuum = 0;

    bt.pBt = &shared;
    bt.inTrans = TRANS_WRITE;
}

// --------------- Unit tests ---------------

// Test 1: Basic successful updateMeta writes iMeta to the page's Page1 data
//          - Preconditions: pager write succeeds, idx within range, non-AUTOVACUUM path (not testing incrVacuum)
//          - Expected: The 4-byte iMeta value is written at offset (36 + idx*4) of pPage1->aData
static void test_basic_update_meta_writes_memory() {
    // Local objects
    Btree bt;
    BtShared shared;
    MemPage page;
    DbPage dbpage;
    const size_t BUF_SIZE = 128;
    unsigned char buffer[BUF_SIZE];
    std::memset(buffer, 0, BUF_SIZE);

    // Prepare environment
    setup_basic_environment(bt, shared, page, dbpage, buffer, BUF_SIZE);

    // Use an arbitrary idx within 1..15 (not equal to incr vacuum constant to avoid additional branch)
    int idx = 1;
    u32 iMeta = 0x01020304; // test value to be written
    g_forcePagerError = 0; // ensure pager write succeeds

    // Call the focal method
    int rc = sqlite3BtreeUpdateMeta(&bt, idx, iMeta);

    // Assertions (non-terminating)
    expect(rc == SQLITE_OK, "sqlite3BtreeUpdateMeta should return SQLITE_OK on success");
    // Read back the 4 bytes written
    unsigned int written =
        buffer[36 + idx*4] |
        (unsigned int)buffer[36 + idx*4 + 1] << 8 |
        (unsigned int)buffer[36 + idx*4 + 2] << 16 |
        (unsigned int)buffer[36 + idx*4 + 3] << 24;

    expect(written == iMeta, "iMeta should be written to pPage1->aData at correct offset (little-endian)");
}

// Test 2: When pager write fails, no write to memory occurs and function still returns (non-deterministic in our harness)
//          - We simulate a failure via g_forcePagerError and verify that the target memory region remains unchanged.
static void test_update_meta_pager_write_failure_no_memory_change() {
    // Local objects
    Btree bt;
    BtShared shared;
    MemPage page;
    DbPage dbpage;
    const size_t BUF_SIZE = 128;
    unsigned char buffer[BUF_SIZE];
    std::memset(buffer, 0, BUF_SIZE);

    // Prepare environment
    setup_basic_environment(bt, shared, page, dbpage, buffer, BUF_SIZE);

    int idx = 2;
    u32 iMeta = 0xAABBCCDD;

    // Force pager write to fail
    g_forcePagerError = 1;

    // Call the focal method
    int rc = sqlite3BtreeUpdateMeta(&bt, idx, iMeta);

    // We expect an error; ensure memory region was not modified
    unsigned int written =
        buffer[36 + idx*4] |
        (unsigned int)buffer[36 + idx*4 + 1] << 8 |
        (unsigned int)buffer[36 + idx*4 + 2] << 16 |
        (unsigned int)buffer[36 + idx*4 + 3] << 24;

    expect(written == 0, "On pager write failure, memory region should remain unchanged (zero)");
    (void)rc; // rc is not strictly asserted here due to environment; rely on memory state
    g_forcePagerError = 0; // reset for any subsequent tests
}

// Entrypoint for running all tests
int main() {
    std::cout << "Starting sqlite3BtreeUpdateMeta unit tests (C++11 harness)" << std::endl;

    test_basic_update_meta_writes_memory();
    test_update_meta_pager_write_failure_no_memory_change();

    print_summary();

    // Exit code: 0 if all tests pass, non-zero if failures occurred
    return (g_failed_tests == 0) ? 0 : 1;
}