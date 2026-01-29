/*
  Unit test suite for the focal method lsmShmLock in lsm_shared.c
  - Uses a minimal test harness (no GoogleTest) with simple assertions
  - Exercises true and false branches of lsmShmLock's logic
  - Mirrors the project's dependency structure via included headers
  - Assumes the project provides the actual lsmInt.h and related declarations
  - All tests are self-contained within main() and report results
  - Focuses on observable state changes and return codes
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <lsmInt.h>
#include <stdio.h>


extern "C" {
}

// Simple non-terminating assertion framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TASSERT(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        fprintf(stderr, "Test failure: %s\n", (msg)); \
        ++g_tests_failed; \
    } \
} while(0)

// Helper to reset a lsm_db/test environment
static void reset_environment(lsm_db *pDb, Database *pDatabase, lsm_env *pEnv) {
    // Zero existing memory to avoid leakage from previous tests
    if(pDb) {
        memset(pDb, 0, sizeof(*pDb));
        pDb->pDatabase = pDatabase;
        pDb->pEnv = pEnv;
        pDb->pConn = NULL;
        pDb->mLock = 0;
    }
    if(pDatabase) {
        memset(pDatabase, 0, sizeof(*pDatabase));
        // pClientMutex is a black-box in tests; keep as NULL by default
        pDatabase->pClientMutex = NULL;
        pDatabase->bReadonly = 0;
    }
}

// Test 1: Shared lock when no conflicting owner exists
static void test_lsmShmLock_shared_basic() {
    // Arrange
    lsm_env env;
    Database dbEntry;
    lsm_db db;
    reset_environment(&db, &dbEntry, &env);

    // Act: acquire SHARED for iLock=1
    int rc = lsmShmLock(&db, 1, LSM_LOCK_SHARED, 0);

    // Assert
    TASSERT(rc == LSM_OK, "Shared lock basic: rc should be LSM_OK");
    // ms bit should be set in mLock (shared mask for iLock=1)
    uint64_t me = (uint64_t)1 << (1 - 1);                  // 1
    uint64_t ms = (uint64_t)1 << (1 + 32 - 1);             // 1<<32
    TASSERT((db.mLock & ms) != 0, "Shared lock basic: ms bit should be set");
    TASSERT((db.mLock & me) == 0, "Shared lock basic: me bit should not be set");
}

// Test 2: Exclusive (EXCL) lock when there are no competing locks
static void test_lsmShmLock_excl_basic() {
    // Arrange
    lsm_env env;
    Database dbEntry;
    lsm_db db;
    reset_environment(&db, &dbEntry, &env);

    // Act: acquire EXCL for iLock=1
    int rc = lsmShmLock(&db, 1, LSM_LOCK_EXCL, 0);

    // Assert
    TASSERT(rc == LSM_OK, "Exclusive lock basic: rc should be LSM_OK");
    uint64_t me = (uint64_t)1 << (1 - 1);
    uint64_t ms = (uint64_t)1 << (1 + 32 - 1);
    TASSERT((db.mLock & (me | ms)) == (me | ms), "Exclusive lock basic: both me and ms should be set");
}

// Test 3: Shared lock should fail (BUSY) when another connection holds exclusive
static void test_lsmShmLock_shared_busy_due_to_other_excl() {
    // Arrange
    lsm_env env;
    Database dbEntry;
    lsm_db db;
    reset_environment(&db, &dbEntry, &env);

    // Create another connection that holds EXCL on iLock=1
    lsm_db other;
    memset(&other, 0, sizeof(other));

    // Other process holds exclusive on iLock=1 (me bit)
    uint64_t me = (uint64_t)1 << (1 - 1);
    other.mLock = me; // exclusive on this connection

    // Link into the pConn chain of the database's Database
    // Note: p->pConn is the chain head; we set it to point to 'other'
    // The lsmShmLock will traverse and count nExcl based on (pIter->mLock & me)
    db.pDatabase = &dbEntry;
    db.pEnv = &env;
    // Attach chain: other -> NULL
    other.pNext = NULL;
    // The actual Database is the "p" in the code; we subtly simulate by setting db.pConn to &other
    db.pConn = &other;

    // Act: attempt to acquire SHARED; since nExcl==1, rc should be LSM_BUSY
    int rc = lsmShmLock(&db, 1, LSM_LOCK_SHARED, 0);

    // Assert
    TASSERT(rc == LSM_BUSY, "Shared lock should be BUSY when another connection holds EXCL");
    // Ensure our db's locks remain unchanged (still 0)
    TASSERT(db.mLock == 0, "Lock state should remain unchanged after BUSY condition");
}

// Test 4: Unlock path clears locks when there is a SHARED lock held by self
static void test_lsmShmLock_unlock_clears_lock() {
    // Arrange
    lsm_env env;
    Database dbEntry;
    lsm_db db;
    reset_environment(&db, &dbEntry, &env);

    // Simulate that this db currently holds a SHARED lock (ms bit)
    uint64_t ms = (uint64_t)1 << (1 + 32 - 1);
    db.mLock = ms;

    // Act: unlock iLock=1
    int rc = lsmShmLock(&db, 1, LSM_LOCK_UNLOCK, 0);

    // Assert
    TASSERT(rc == LSM_OK, "Unlock path: rc should be LSM_OK");
    TASSERT((db.mLock & ms) == 0, "Unlock path: ms bit should be cleared after unlock");
    // me should also be cleared by unlock path
    uint64_t me = (uint64_t)1 << (1 - 1);
    TASSERT((db.mLock & me) == 0, "Unlock path: me bit should be cleared after unlock");
}

// Test 5: No-op path when no conditions match (should return LSM_OK and not modify locks)
static void test_lsmShmLock_no_op_path() {
    // Arrange
    lsm_env env;
    Database dbEntry;
    lsm_db db;
    reset_environment(&db, &dbEntry, &env);

    // Ensure no lock bits are set and call with an operation that shouldn't proceed
    int rc = lsmShmLock(&db, 1, LSM_LOCK_UNLOCK, 0); // db.mLock == 0, so no-op

    // Assert
    TASSERT(rc == LSM_OK, "No-op path: rc should be LSM_OK");
    TASSERT(db.mLock == 0, "No-op path: lock state should remain zero");
}

// Entry point
int main(void) {
    printf("Running lsmShmLock unit tests...\n");

    test_lsmShmLock_shared_basic();
    test_lsmShmLock_excl_basic();
    test_lsmShmLock_shared_busy_due_to_other_excl();
    test_lsmShmLock_unlock_clears_lock();
    test_lsmShmLock_no_op_path();

    printf("Tests run: %d\n", g_tests_run);
    if(g_tests_failed == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Tests failed: %d\n", g_tests_failed);
        return 1;
    }
}