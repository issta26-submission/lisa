// Test suite for lsmInfoCompressionId in lsm_ckpt.c
// - No GTest/GMock, plain C++11 with manual test harness
// - Focus on correctness of the focal method and its dependencies
// - Includes explanatory comments for each test
// - Assumes the project headers provide lsm_db, CKPT_HDR_CMPID, LSM_OK, and related APIs

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cassert>


// Domain knowledge notes:
// - The focal function is declared in C and has C linkage. We access it via extern "C".
// - It relies on the global state set by lsmCheckpointLoad and then reads
//   db->aSnapshot[CKPT_HDR_CMPID] when rc == LSM_OK.
// - We assume the project provides the necessary headers in the include path.

extern "C" {

// Forward declare the focal function to ensure linkage from C++ test code.
int lsmInfoCompressionId(struct lsm_db *db, uint32_t *piCmpId);

// The real project headers typically define LSM_OK, CKPT_HDR_CMPID, and lsm_db.
// We assume they are available via the include path. If not, the project compile
// will fail; this test is written under the assumption that the project headers
// are correctly provided by the test environment.

} // extern "C"


// Note: The test environment is expected to link this test with the library/source
// that implements lsm_ckpt.c, so that lsmInfoCompressionId and lsmCheckpointLoad
// have their real implementations. We do not redefine static helpers here.


// Utility to build a minimal lsm_db instance for tests.
// This function allocates a db-like object with pClient and pWorker set to 0
// and an aSnapshot buffer large enough to hold CKPT_HDR_CMPID index.
static lsm_db* make_fake_db_with_snapshot(uint32_t snapshotValue) {
    // Allocate a db instance. This struct is usually a C-struct defined in the library.
    // We rely on the project header to define its layout.
    lsm_db *db = new lsm_db;
    std::memset(db, 0, sizeof(lsm_db));

    // Ensure the focal function's assert is not triggered by test design.
    // The test environment should configure asserts accordingly. We explicitly
    // set pClient and pWorker to 0 to satisfy the assertion in lsmInfoCompressionId.
    db->pClient = 0;
    db->pWorker = 0;

    // Allocate aSnapshot array. The size must be at least CKPT_HDR_CMPID + 1.
    // We defensively allocate a modest buffer; tests should not rely on more.
    // If aSnapshot is already allocated in the real struct, this is overridden for test isolation.
    size_t snapshot_size = CKPT_HDR_CMPID + 1;
    db->aSnapshot = new uint32_t[snapshot_size];
    // Initialize with a known value to detect writes.
    for (size_t i = 0; i < snapshot_size; ++i) db->aSnapshot[i] = 0;
    db->aSnapshot[CKPT_HDR_CMPID] = snapshotValue;

    return db;
}

// Clean up the fake db created by make_fake_db_with_snapshot
static void free_fake_db(lsm_db *db) {
    if (!db) return;
    delete[] db->aSnapshot;
    delete db;
}


// Test 1: Verify that when lsmCheckpointLoad returns LSM_OK, the compression id is written
// from the current snapshot (CKPT_HDR_CMPID).
static bool test_lsmInfoCompressionId_OK_branch() {
    std::cout << "Running test_lsmInfoCompressionId_OK_branch...\n";

    // Prepare a known compression id value to be picked up from the snapshot.
    const uint32_t expectedCmpId = 0xDEADBEEF;
    lsm_db *db = make_fake_db_with_snapshot(expectedCmpId);

    // This test relies on the real lsmCheckpointLoad implementation to return LSM_OK.
    // We request a compression id and verify the output matches the snapshot value.
    uint32_t outCmpId = 0;
    int rc = lsmInfoCompressionId(db, &outCmpId);

    // Cleanup
    free_fake_db(db);

    // Assertions (non-terminating): useful for coverage in a larger test harness.
    bool ok = (rc == LSM_OK) && (outCmpId == expectedCmpId);
    if (!ok) {
        std::cerr << "FAILED: test_lsmInfoCompressionId_OK_branch\n";
        std::cerr << "Expected rc=" << LSM_OK << " and outCmpId=" << expectedCmpId
                  << ", got rc=" << rc << " and outCmpId=" << outCmpId << "\n";
    }
    return ok;
}


// Test 2: Verify the non-OK path behavior.
// If lsmCheckpointLoad does not return LSM_OK, lsmInfoCompressionId should not modify piCmpId
// and should propagate the non-OK rc.
// Note: This test assumes the test environment can ensure lsmCheckpointLoad returns a non-OK value.
// If the actual implementation returns OK in this environment, this test may need adaptation
// (e.g., by injecting a non-OK path through a dedicated test harness or by configuring the
// environment to simulate checkpoint load failure).
static bool test_lsmInfoCompressionId_notOK_branch() {
    std::cout << "Running test_lsmInfoCompressionId_notOK_branch...\n";

    // Prepare a db with a known compression id value.
    const uint32_t initialCmpId = 0xCAFEBABE;
    lsm_db *db = make_fake_db_with_snapshot(initialCmpId);

    // Initialize output with a distinct value to detect unintended writes.
    uint32_t outCmpIdBefore = 0x12345678;
    uint32_t outCmpId = outCmpIdBefore;

    int rc = lsmInfoCompressionId(db, &outCmpId);

    // Cleanup
    free_fake_db(db);

    // We cannot reliably force a non-OK rc without a test harness that mocks lsmCheckpointLoad.
    // Instead, we perform a conservative check: if rc is not LSM_OK, outCmpId should remain unchanged.
    // If rc unexpectedly becomes LSM_OK, the test will fail (reflecting that the non-OK path isn't exercised).
    bool notOkPathTaken = (rc != LSM_OK);
    bool cmpUnchanged = (outCmpId == outCmpIdBefore);
    bool ok = notOkPathTaken ? cmpUnchanged : false; // Only pass if non-OK path occurred and no write happened

    if (!notOkPathTaken && !cmpUnchanged) {
        // If we reach here, rc == LSM_OK and outCmpId changed, which means the test didn't exercise non-OK path.
        std::cerr << "WARNING: test_lsmInfoCompressionId_notOK_branch could not force non-OK rc in this environment.\n";
    }

    if (!ok) {
        std::cerr << "FAILED: test_lsmInfoCompressionId_notOK_branch (non-OK path not exercised or incorrect behavior)\n";
        std::cerr << "rc=" << rc << ", outCmpId=" << outCmpId << "\n";
    }

    return ok;
}


// Entry point for running all tests
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting test suite for lsmInfoCompressionId...\n";

    // Run Test 1
    ++total;
    if (test_lsmInfoCompressionId_OK_branch()) {
        ++passed;
        std::cout << "Test 1 passed.\n";
    } else {
        std::cout << "Test 1 FAILED.\n";
    }

    // Run Test 2
    ++total;
    if (test_lsmInfoCompressionId_notOK_branch()) {
        ++passed;
        std::cout << "Test 2 passed.\n";
    } else {
        std::cout << "Test 2 may rely on environment to simulate non-OK; treated as warning/failed in strict environments.\n";
    }

    std::cout << "Tests completed: " << passed << " / " << total << "\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}