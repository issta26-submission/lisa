// Unit test suite for lsmShmTestLock in lsm_shared.c
// Target: C++11, no GoogleTest. Tests exercise behavior of lsmShmTestLock
// by constructing minimal in-test data structures that mirror the runtime
// expectations of the focal function.
// Notes:
// - This test suite relies on the surrounding project definitions (lsmInt.h etc.)
//   being available in the include path when compiling.
// - We test true/false branches of key predicates:
//     a) Overlap with existing connections (LSM_BUSY path)
//     b) No overlap and single/multi proc mode handling (LSM_OK path)
//     c) No overlap and multi-proc enabled (calls to lsmEnvTestLock; accepts either
//        LSM_OK or LSM_BUSY as the exact return value since lsmEnvTestLock's
//        behavior depends on the environment).
// - We do not mock non-virtual private methods; we rely on the project-provided
//   lsmMutexEnter/lsmMutexLeave implementations. If those are non-noop in your
//   environment, tests should still execute deterministically for the scenarios
//   constructed here.

#include <lsmInt.h>
#include <iostream>
#include <functional>
#include <string>


// Include the project's core type and function declarations.
// The focal method and its dependencies are defined in the project's sources.
// Ensure the include path points to the correct headers in your build environment.
extern "C" {
}

// Forward declare to avoid any potential name mangling issues
extern "C" int lsmShmTestLock(lsm_db *db, int iLock, int nLock, int eOp);

// Simple test harness utilities
static bool test_passed = true;

static void report_result(const std::string &name, bool ok) {
    if (ok) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
        test_passed = false;
    }
}

// Helper to clean up resources in case of test failures
static void safe_delete_db_chain(lsm_db* db) {
    // Best-effort cleanup; actual project may have more fields and deeper cleanup.
    // We allocate via new, so delete in reverse steps if non-null.
    if (!db) return;
    // Attempt to delete the connected chain's next elements.
    lsm_db* iter = db;
    while (iter) {
        lsm_db* next = iter->pConn;
        // Delete Database if we allocated it locally
        if (iter->pDatabase) {
            delete iter->pDatabase;
            iter->pDatabase = nullptr;
        }
        delete iter;
        iter = next;
    }
}

// Test 1: No overlap, single connection list empty, bMultiProc = false
// Expect: rc == LSM_OK
static bool test_no_overlap_no_mproc() {
    // Set up environment and core structures
    lsm_env* env = new lsm_env();
    Database* p = new Database();
    // Initialize minimal Database fields used by lsmShmTestLock
    // These field names are inferred from the focal method's usage:
    p->pClientMutex = reinterpret_cast<void*>(1); // dummy non-null mutex
    p->bMultiProc = false;
    p->pFile = nullptr;

    lsm_db* db = new lsm_db();
    db->pDatabase = p;
    db->pEnv = env;
    db->pConn = nullptr;  // No other connections
    db->mLock = 0;        // Ensure no pre-existing locks in test

    int rc = lsmShmTestLock(db, 1, 1, /*eOp*/0); // eOp non-EXCL

    // Cleanup
    safe_delete_db_chain(db);
    delete p;
    delete env;

    bool ok = (rc == LSM_OK);
    report_result("test_no_overlap_no_mproc", ok);
    return ok;
}

// Test 2: Overlap with an existing connection (non-EXCL case)
// Expect: rc == LSM_BUSY
static bool test_overlap_non_excl() {
    // Base environment and primary db object
    lsm_env* env = new lsm_env();
    Database* p = new Database();
    p->pClientMutex = reinterpret_cast<void*>(2);
    p->bMultiProc = false;
    p->pFile = nullptr;

    lsm_db* db = new lsm_db();
    db->pDatabase = p;
    db->pEnv = env;
    db->pConn = nullptr;
    db->mLock = 0;

    // Create an overlapping connection in the list
    lsm_db* other = new lsm_db();
    other->pDatabase = new Database();
    other->pEnv = env;
    other->mLock = 1;   // bit 0 set -> would overlap with mask's bit 0 when iLock=1, nLock>=1, eOp non-EXCL
    other->pNext = nullptr;
    other->pConn = nullptr;

    // Link into primary's connection list
    db->pConn = other;

    int rc = lsmShmTestLock(db, 1, 1, /*eOp*/0); // iLock=1, nLock=1, eOp non-EXCL

    // Cleanup
    safe_delete_db_chain(db);
    delete other->pDatabase;
    delete other;
    delete p;
    delete env;

    bool ok = (rc == LSM_BUSY);
    report_result("test_overlap_non_excl", ok);
    return ok;
}

// Test 3: No overlap, multi-proc enabled, EXCLUSIVE path
// Expect: rc is either LSM_OK or LSM_BUSY depending on lsmEnvTestLock() implementation.
// We only require that the branch selection occurs and the function does not crash.
static bool test_no_overlap_with_multi_proc_excl() {
    lsm_env* env = new lsm_env();
    Database* p = new Database();
    p->pClientMutex = reinterpret_cast<void*>(3);
    p->bMultiProc = true; // enable multi-process path
    p->pFile = nullptr;

    lsm_db* db = new lsm_db();
    db->pDatabase = p;
    db->pEnv = env;
    db->pConn = nullptr;
    db->mLock = 0;

    // No overlapping connections in the list
    int rc = lsmShmTestLock(db, 1, 1, LSM_LOCK_EXCL); // iLock=1, nLock=1, eOp EXCL

    // Cleanup
    safe_delete_db_chain(db);
    delete p;
    delete env;

    // Accept either result depending on underlying environment test
    bool ok = (rc == LSM_OK) || (rc == LSM_BUSY);
    report_result("test_no_overlap_with_multi_proc_excl", ok);
    return ok;
}

int main() {
    // Run all tests; in case an environment differs, we still report outcomes.
    bool t1 = test_no_overlap_no_mproc();
    bool t2 = test_overlap_non_excl();
    bool t3 = test_no_overlap_with_multi_proc_excl();

    if (test_passed) {
        std::cout << "All tests completed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
}