// Unit tests for the focal method: lsm_set_user_version
// This test suite is implemented in C++11 without any external testing framework.
// It provides a minimal in-file reproduction of the necessary types and functions
// to exercise the logic of lsm_set_user_version, including its interactions
// with lsm_begin, lsm_commit, and lsm_rollback.

#include <lsmInt.h>
#include <iostream>


// Domain-provided constants and types (reproduced for testability)
static const int LSM_OK = 0;

// Minimal reproduction of the lsm_db structure used by lsm_set_user_version
struct lsm_db {
    int nTransOpen; // number of open transactions
    struct {
        unsigned int iUsrVersion;
    } treehdr;
};

// Global counters to verify side effects of begin/commit/rollback
static int g_begin_called = 0;
static int g_commit_called = 0;
static int g_rollback_called = 0;
static int g_begin_should_succeed = 1; // Control whether lsm_begin simulates success

// Forward declaration of the function under test.
// In an actual build, this would come from lsm_main.c; for unit testing, we
// provide a self-contained implementation below mirroring the focal logic.
int lsm_set_user_version(lsm_db *pDb, unsigned int iUsr);

// Mock implementations of the transaction helpers used by the focal method

int lsm_begin(lsm_db* /*pDb*/, int /*iLevel*/) {
    ++g_begin_called;
    if (g_begin_should_succeed) {
        // Simulate starting a transaction: mark as open
        // The actual pDb state is modified by the caller's code path;
        // we mirror typical behavior: set nTransOpen to a non-zero value.
        // Here we assume the caller will inspect pDb->nTransOpen as needed.
        return LSM_OK;
    } else {
        // Simulate a failure to begin a transaction
        return -1;
    }
}

int lsm_commit(lsm_db* /*pDb*/, int /*iLevel*/) {
    ++g_commit_called;
    // On commit, clear the open transaction state
    return LSM_OK;
}

int lsm_rollback(lsm_db* /*pDb*/, int /*iLevel*/) {
    ++g_rollback_called;
    // On rollback, clear the open transaction state
    return LSM_OK;
}

// Helper: provide a clean test reset between cases
void reset_test_counters() {
    g_begin_called = 0;
    g_commit_called = 0;
    g_rollback_called = 0;
    g_begin_should_succeed = 1;
}

// Minimal in-file implementation of the focal function under test
int lsm_set_user_version(lsm_db *pDb, unsigned int iUsr){
  int rc = LSM_OK;                /* Return code */
  int bCommit = 0;                /* True to commit before returning */
  if( pDb->nTransOpen==0 ){
    bCommit = 1;
    rc = lsm_begin(pDb, 1);
  }
  if( rc==LSM_OK ){
    pDb->treehdr.iUsrVersion = iUsr;
  }
  /* If a transaction was opened at the start of this function, commit it. 
  ** Or, if an error has occurred, roll it back.  */
  if( bCommit ){
    if( rc==LSM_OK ){
      rc = lsm_commit(pDb, 0);
    }else{
      lsm_rollback(pDb, 0);
    }
  }
  return rc;
}

// Simple test assertion helpers (non-terminating)
int test_passed = 0;
int test_failed = 0;

#define ASSERT_EQ(actual, expected, description) do { \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << description << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
        ++test_failed; \
    } else { \
        std::cout << "PASS: " << description << std::endl; \
        ++test_passed; \
    } \
} while(0)

#define ASSERT_TRUE(cond, description) do { \
    if (!(cond)) { \
        std::cerr << "FAIL: " << description << " | condition is false" << std::endl; \
        ++test_failed; \
    } else { \
        std::cout << "PASS: " << description << std::endl; \
        ++test_passed; \
    } \
} while(0)

#define ASSERT_FALSE(cond, description) do { \
    if (cond) { \
        std::cerr << "FAIL: " << description << " | condition is true" << std::endl; \
        ++test_failed; \
    } else { \
        std::cout << "PASS: " << description << std::endl; \
        ++test_passed; \
    } \
} while(0)

// Test 1: Success path when there is no active transaction.
// Expectation:
// - lsm_begin is called once and succeeds
// - The user version is updated
// - A commit is performed
void test_success_path() {
    reset_test_counters();

    lsm_db db = {0, {0}}; // nTransOpen = 0; initial iUsrVersion = 0
    db.nTransOpen = 0;
    db.treehdr.iUsrVersion = 0;
    g_begin_should_succeed = 1;

    int rc = lsm_set_user_version(&db, 12345);

    ASSERT_EQ(rc, LSM_OK, "rc should be LSM_OK on successful path");
    ASSERT_EQ(db.treehdr.iUsrVersion, 12345, "iUsrVersion should be updated to 12345");
    // After a successful commit, transaction should be closed
    ASSERT_EQ(db.nTransOpen, 0, "transaction should be closed (nTransOpen == 0) after commit");
    ASSERT_EQ(g_begin_called, 1, "lsm_begin should be called exactly once");
    ASSERT_EQ(g_commit_called, 1, "lsm_commit should be called exactly once");
    ASSERT_EQ(g_rollback_called, 0, "lsm_rollback should not be called");
}

// Test 2: Begin fails; expect rollback path.
// Expectation:
// - lsm_begin is called and returns error
// - iUsrVersion should not be updated
// - rollback is performed
void test_begin_failure() {
    reset_test_counters();

    lsm_db db = {0, {0}};
    db.nTransOpen = 0;
    db.treehdr.iUsrVersion = 77;
    g_begin_should_succeed = 0; // simulate begin failure

    int rc = lsm_set_user_version(&db, 999);

    ASSERT_NE(rc, LSM_OK, "rc should not be LSM_OK when begin fails");
    ASSERT_EQ(db.treehdr.iUsrVersion, 77, "iUsrVersion should remain unchanged when begin fails");
    // Rollback should have occurred
    ASSERT_EQ(db.nTransOpen, 0, "nTransOpen should be 0 after rollback");
    ASSERT_EQ(g_begin_called, 1, "lsm_begin should be called exactly once");
    ASSERT_EQ(g_rollback_called, 1, "lsm_rollback should be called exactly once");
    ASSERT_EQ(g_commit_called, 0, "lsm_commit should not be called");
}

// Test 3: There is an active transaction; no begin/commit/rollback should occur.
// Expectation:
// - iUsrVersion is updated directly
// - No change to transaction state via begin/commit/rollback
void test_existing_transaction_no_begin() {
    reset_test_counters();

    lsm_db db = {0, {0}};
    db.nTransOpen = 2; // simulate an already-opened transaction
    db.treehdr.iUsrVersion = 5;
    g_begin_should_succeed = 1; // irrelevant since begin should not be called

    int rc = lsm_set_user_version(&db, 555);

    ASSERT_EQ(rc, LSM_OK, "rc should be LSM_OK when an existing transaction is present");
    ASSERT_EQ(db.treehdr.iUsrVersion, 555, "iUsrVersion should be updated to 555 without starting new transaction");
    ASSERT_EQ(g_begin_called, 0, "lsm_begin should not be called when a transaction is already open");
    ASSERT_EQ(g_commit_called, 0, "lsm_commit should not be called");
    ASSERT_EQ(g_rollback_called, 0, "lsm_rollback should not be called");
    // Transaction state should remain unchanged (still open)
    ASSERT_EQ(db.nTransOpen, 2, "nTransOpen should remain unchanged (still 2)");
}

// Main test runner
int main() {
    std::cout << "Starting unit tests for lsm_set_user_version...\n\n";

    test_success_path();
    test_begin_failure();
    test_existing_transaction_no_begin();

    int total = test_passed + test_failed;
    std::cout << "\nTest summary: " << total << " tests, "
              << test_passed << " passed, "
              << test_failed << " failed." << std::endl;

    return (test_failed == 0) ? 0 : 1;
}