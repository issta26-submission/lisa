// Unit Test Suite for lsmCheckpointSaveWorker (lsm_ckpt.c)
// This test is designed to be compiled with the project sources (including lsm_ckpt.c and lsmInt.h).
// It uses a lightweight, non-terminating assertion style to maximize code coverage without
// terminating on failure (as requested by the domain guidelines).

#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Include internal headers to obtain type definitions and constants.
// This assumes the project provides lsmInt.h with lsm_db, Snapshot, ShmHeader, LSM_OK,
// and LSM_META_RW_PAGE_SIZE definitions.
extern "C" {
}

// Declaration of the focal function using C linkage for the C++ test binary.
extern "C" int lsmCheckpointSaveWorker(lsm_db *pDb, int bFlush);

// Lightweight, non-terminating assertion helpers (E.g., EXPECT_* style).
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (condition: " << #cond << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual, msg) do { \
    if(!((expected) == (actual))) { \
        std::cerr << "[FAIL] " << msg << " (expected: " \
                  << (expected) << ", actual: " << (actual) << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

// Test 1: Basic functional path of lsmCheckpointSaveWorker
// - Verify that the worker increments the snapshot id on each call.
// - Verify that aSnap1 and aSnap2 buffers receive identical data (the code writes the same snapshot
//   data to both buffers with a barrier in between).
// - Verify that the function returns LSM_OK on success.
// Notes:
// - This test relies on the real ckptExportSnapshot producing deterministic output that satisfies
//   the internal asserts in lsmCheckpointSaveWorker (ckptChecksumOk and size bound).
// - We keep the environment minimal and rely on the library's internal handling for memory free
//   (pDb->pEnv is set to nullptr to minimize side effects if the environment is optional).
static void test_lsmCheckpointSaveWorker_basic_path() {
    // Setup: create a minimal in-memory lsm_db-like structure with required fields.
    Snapshot snap;
    // Initialize snapshot id to 0 so that the first call increments to 1.
    snap.iId = 0;

    // The ShmHeader is expected to contain two buffers aSnap1 and aSnap2 of size LSM_META_RW_PAGE_SIZE.
    // We allocate it on the stack to keep the test isolated.
    ShmHeader shm;
    // Zero the header to ensure clean memory, then the test will observe deterministic writes.
    std::memset(&shm, 0, sizeof(shm));

    // Prepare a fake lsm_db structure instance with required pointers.
    // Note: The actual lsm_db type is defined in lsmInt.h; we rely on its layout there.
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    // Wire up the necessary fields for the focal function.
    // If the real structure uses different field names/types, this test relies on the same
    // definitions provided by lsmInt.h included above.
    db.pWorker = &snap;
    db.pShmhdr = &shm;
    db.pEnv = nullptr; // Avoid environment side-effects in tests.

    // Execute: first checkpoint save
    int rc = lsmCheckpointSaveWorker(&db, 0);

    // Validate: rc must be LSM_OK for the success path (as expected for the basic run).
    EXPECT_TRUE(rc == LSM_OK, "First call should return LSM_OK");

    // Validate: snapshot id should have been incremented from 0 to 1.
    EXPECT_EQ(1, snap.iId, "Snapshot iId should increment to 1 after first call");

    // Validate: the two buffers in the shared memory should be identical after the copy.
    // The code copies the snapshot to aSnap2, barriers, then to aSnap1.
    // Hence aSnap1 should be identical to aSnap2 for all bytes written (up to LSM_META_RW_PAGE_SIZE).
    int cmpRes = std::memcmp(shm.aSnap1, shm.aSnap2, LSM_META_RW_PAGE_SIZE);
    EXPECT_TRUE(cmpRes == 0, "aSnap1 and aSnap2 buffers should be identical after first write");

    // Execute: second checkpoint save (bFlush value does not affect the basic path here).
    rc = lsmCheckpointSaveWorker(&db, 1);

    // Validate: rc must be LSM_OK for the second run as well.
    EXPECT_TRUE(rc == LSM_OK, "Second call should return LSM_OK");

    // Validate: snapshot id should increment again to 2.
    EXPECT_EQ(2, snap.iId, "Snapshot iId should increment to 2 after second call");

    // Validate: aSnap1 and aSnap2 buffers still identical after the second call.
    cmpRes = std::memcmp(shm.aSnap1, shm.aSnap2, LSM_META_RW_PAGE_SIZE);
    EXPECT_TRUE(cmpRes == 0, "aSnap1 and aSnap2 buffers should be identical after second write");
}

// Entry point for the test binary
int main() {
    std::cout << "Running tests for lsmCheckpointSaveWorker (lsm_ckpt.c) with C linkage\n";

    // Run test cases
    test_lsmCheckpointSaveWorker_basic_path();

    // Report overall results
    if(g_test_failures == 0) {
        std::cout << "[OK] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[TOTAL FAILURES] " << g_test_failures << std::endl;
        return 1;
    }
}