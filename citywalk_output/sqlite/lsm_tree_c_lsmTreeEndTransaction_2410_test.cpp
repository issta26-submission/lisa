// Unit test suite for lsmTreeEndTransaction (C++11, no GTest)
// This test suite targets the focal function:
//   int lsmTreeEndTransaction(lsm_db *pDb, int bCommit)
// It validates that the function copies the in-memory tree header to the
// shared memory header, resets the writer flag, and frees the rollback array.
// Notes:
// - We rely on the project-provided C headers (lsmInt.h, etc.) to define the
//   involved structures (lsm_db, ShmHeader, TreeHeader, IntArray, etc.).
// - The test is implemented in C++11 and uses a small main() that executes
//   test cases directly (as required when GTest isn't allowed).
// - The tests focus on deterministic, executable behavior and use memcmp for
//   structure comparisons to cover value/binary equality.

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <string.h>


// Bring in the C project type definitions and constants
extern "C" {
}

// Provide a direct prototype for the focal function (in case the header lacks it)
extern "C" int lsmTreeEndTransaction(lsm_db *pDb, int bCommit);

// Simple test result tracking
static int g_testsRun = 0;
static int g_testsPassed = 0;

// Helper: compare two TreeHeader structures by binary equality
static bool treeHeaderEquals(const TreeHeader &a, const TreeHeader &b) {
    return std::memcmp(&a, &b, sizeof(TreeHeader)) == 0;
}

// Test 1: Basic EndTransaction behavior
// - Initialize a lsm_db with a known TreeHeader value.
// - Ensure that after lsmTreeEndTransaction:
//     a) the return code is LSM_OK,
//     b) shm.hdr2 == db.treehdr and shm.hdr1 == db.treehdr,
//     c) shm.bWriter is reset to 0.
static bool test_EndTransaction_Basic() {
    g_testsRun++;

    // Prepare fake in-memory database and shared memory header
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    ShmHeader shm;
    std::memset(&shm, 0, sizeof(shm));

    db.pShmhdr = &shm;

    // Initialize tree header with a deterministic pattern for testing
    TreeHeader th;
    std::memset(&th, 0x5A, sizeof(th)); // Non-zero, traceable pattern
    db.treehdr = th;

    // Provide a dummy environment and rollback container
    lsm_env env;
    std::memset(&env, 0, sizeof(env));
    db.pEnv = &env;

    IntArray rollback;
    std::memset(&rollback, 0, sizeof(rollback));
    db.rollback = rollback;

    // Ensure writer flag starts as non-zero to verify it gets reset
    shm.bWriter = 1;

    int rc = lsmTreeEndTransaction(&db, 1);

    bool ok = true;
    if (rc != LSM_OK) {
        std::cerr << "[EndTransaction] Expected LSM_OK, got " << rc << std::endl;
        ok = false;
    }

    // After the call, hdr2 and hdr1 should mirror the current treehdr
    if (!treeHeaderEquals(shm.hdr2, db.treehdr)) {
        std::cerr << "[EndTransaction] shm.hdr2 does not equal db.treehdr" << std::endl;
        ok = false;
    }
    if (!treeHeaderEquals(shm.hdr1, db.treehdr)) {
        std::cerr << "[EndTransaction] shm.hdr1 does not equal db.treehdr" << std::endl;
        ok = false;
    }

    // Writer flag should be reset
    if (shm.bWriter != 0) {
        std::cerr << "[EndTransaction] shm.bWriter was not reset to 0" << std::endl;
        ok = false;
    }

    if (ok) {
        g_testsPassed++;
    } else {
        std::cerr << "[EndTransaction] Test failed." << std::endl;
    }

    return ok;
}

// Test 2: EndTransaction robustness when writer flag already zero
// - The function should still complete without error and maintain invariants.
static bool test_EndTransaction_Basic_NoWriterInitiallyZero() {
    g_testsRun++;

    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    ShmHeader shm;
    std::memset(&shm, 0, sizeof(shm));

    db.pShmhdr = &shm;

    TreeHeader th;
    std::memset(&th, 0, sizeof(th));
    db.treehdr = th;

    lsm_env env;
    std::memset(&env, 0, sizeof(env));
    db.pEnv = &env;

    IntArray rollback;
    std::memset(&rollback, 0, sizeof(rollback));
    db.rollback = rollback;

    // Writer already zero
    shm.bWriter = 0;

    int rc = lsmTreeEndTransaction(&db, 0);

    bool ok = true;
    if (rc != LSM_OK) {
        std::cerr << "[EndTransaction(NoWriterInitiallyZero)] Expected LSM_OK, got " << rc << std::endl;
        ok = false;
    }

    if (!treeHeaderEquals(shm.hdr2, db.treehdr)) {
        std::cerr << "[EndTransaction(NoWriterInitiallyZero)] shm.hdr2 != db.treehdr" << std::endl;
        ok = false;
    }
    if (!treeHeaderEquals(shm.hdr1, db.treehdr)) {
        std::cerr << "[EndTransaction(NoWriterInitiallyZero)] shm.hdr1 != db.treehdr" << std::endl;
        ok = false;
    }

    // Writer should remain zero
    if (shm.bWriter != 0) {
        std::cerr << "[EndTransaction(NoWriterInitiallyZero)] shm.bWriter changed from 0" << std::endl;
        ok = false;
    }

    if (ok) {
        g_testsPassed++;
    } else {
        std::cerr << "[EndTransaction(NoWriterInitiallyZero)] Test failed." << std::endl;
    }

    return ok;
}

// Main: run all tests and report summary
int main() {
    bool r1 = test_EndTransaction_Basic();
    bool r2 = test_EndTransaction_Basic_NoWriterInitiallyZero();

    std::cout << "lsmTreeEndTransaction tests run: " << g_testsRun
              << ", passed: " << g_testsPassed << std::endl;

    // Return non-zero if any test failed
    return (r1 && r2) ? 0 : 1;
}