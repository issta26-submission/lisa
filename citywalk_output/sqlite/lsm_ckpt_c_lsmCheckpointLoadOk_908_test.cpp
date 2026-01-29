// C++11 test harness for lsmCheckpointLoadOk (no GTest, no GMock)
// Step-by-step: create a minimal test suite that exercises both branches of
// lsmCheckpointLoadOk by manipulating the underlying shared memory snapshots.
// Explanatory comments are provided for each unit test.

#include <cstdlib>
#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Import the project's C header to get the real types.
// This header defines lsm_db, u32, and the function under test.
extern "C" {

  // Declaration of the focal function under test (from a C source file).
  int lsmCheckpointLoadOk(lsm_db *pDb, int iSnap);
}

// Helper to perform a simple, non-terminating assertion with messaging.
#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        std::cout << "[PASS] " << msg << std::endl; \
        ++g_passed; \
    } else { \
        std::cout << "[FAIL] " << msg << std::endl; \
        ++g_failed; \
    } \
} while(0)

// Global counters for test results
static int g_passed = 0;
static int g_failed = 0;

// Utility: create a minimal lsm_db instance with controllable snapshot values.
// Assumes the project header defines the structures similarly to what's shown
// in the focal code: lsm_db containing aSnapshot and pShmhdr with aSnap1/aSnap2.
static lsm_db* makeTestDb(uint32_t snapshotVal, uint32_t snap1Val, uint32_t snap2Val)
{
    // Allocate the main DB structure
    lsm_db *pDb = (lsm_db*)std::malloc(sizeof(lsm_db));
    if (!pDb) return nullptr;
    std::memset(pDb, 0, sizeof(lsm_db));

    // Allocate and wire the snapshot pointers
    pDb->aSnapshot = (u32*)std::malloc(sizeof(u32));
    if (!pDb->aSnapshot) { std::free(pDb); return nullptr; }
    pDb->aSnapshot[0] = snapshotVal;

    // Allocate the shared memory header and its snap arrays
    pDb->pShmhdr = (ShmHdr*)std::malloc(sizeof(ShmHdr));
    if (!pDb->pShmhdr) {
        std::free(pDb->aSnapshot);
        std::free(pDb);
        return nullptr;
    }
    ShmHdr *hdr = pDb->pShmhdr;
    hdr->aSnap1 = (u32*)std::malloc(sizeof(u32));
    hdr->aSnap2 = (u32*)std::malloc(sizeof(u32));
    if (!hdr->aSnap1 || !hdr->aSnap2) {
        if (hdr->aSnap1) std::free(hdr->aSnap1);
        if (hdr->aSnap2) std::free(hdr->aSnap2);
        std::free(pDb->pShmhdr);
        std::free(pDb->aSnapshot);
        std::free(pDb);
        return nullptr;
    }

    hdr->aSnap1[0] = snap1Val;
    hdr->aSnap2[0] = snap2Val;

    return pDb;
}

// Utility: clean up a test DB instance
static void freeTestDb(lsm_db *pDb)
{
    if (!pDb) return;
    if (pDb->aSnapshot) std::free(pDb->aSnapshot);
    if (pDb->pShmhdr) {
        if (pDb->pShmhdr->aSnap1) std::free(pDb->pShmhdr->aSnap1);
        if (pDb->pShmhdr->aSnap2) std::free(pDb->pShmhdr->aSnap2);
        std::free(pDb->pShmhdr);
    }
    std::free(pDb);
}

// Main test runner
int main() {
    std::cout << "Unit tests for lsmCheckpointLoadOk (C++11 test harness)" << std::endl;

    // Test 1: iSnap == 1, equality case -> expect true
    // Setup: aSnapshot[0] equals aSnap1[0]
    {
        lsm_db *db = makeTestDb(/*snapshotVal=*/1234, /*snap1Val=*/1234, /*snap2Val=*/0);
        TEST_ASSERT(db != nullptr, "Test1: allocate test DB");

        int ret = lsmCheckpointLoadOk(db, 1);
        TEST_ASSERT(ret != 0, "Test1: lsmCheckpointLoadOk(db,1) should return true when aSnapshot[0] == aSnap1[0]");

        freeTestDb(db);
    }

    // Test 2: iSnapshot == 2 with inequality -> expect false
    // Setup: aSnapshot[0] != aSnap2[0]
    {
        lsm_db *db = makeTestDb(/*snapshotVal=*/1234, /*snap1Val=*/1234, /*snap2Val=*/9999);
        TEST_ASSERT(db != nullptr, "Test2: allocate test DB");

        int ret = lsmCheckpointLoadOk(db, 2);
        TEST_ASSERT(ret == 0, "Test2: lsmCheckpointLoadOk(db,2) should return false when aSnapshot[0] != aSnap2[0]");

        freeTestDb(db);
    }

    // Test 3: iSnap == 2 with equality -> expect true
    // Setup: aSnapshot[0] equals aSnap2[0]
    {
        lsm_db *db = makeTestDb(/*snapshotVal=*/1234, /*snap1Val=*/0, /*snap2Val=*/1234);
        TEST_ASSERT(db != nullptr, "Test3: allocate test DB");

        int ret = lsmCheckpointLoadOk(db, 2);
        TEST_ASSERT(ret != 0, "Test3: lsmCheckpointLoadOk(db,2) should return true when aSnapshot[0] == aSnap2[0]");

        freeTestDb(db);
    }

    // Summary
    std::cout << "Tests passed: " << g_passed << ", failed: " << g_failed << std::endl;
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}