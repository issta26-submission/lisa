// Self-contained C++11 test suite for the focal function lsm_get_user_version
// This test reproduces the essential environment needed by lsm_get_user_version
// and validates true/false branches, preserving non-terminating assertions via
// a lightweight test harness (no GTest required).

#include <vector>
#include <lsmInt.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>


// Domain modeling (simplified to exercise the focal method in a controlled TU)
typedef struct lsm_db lsm_db; // forward declaration for compatibility

// Minimal fake environment to drive lsm_get_user_version
struct LsmTreeHdr {
    unsigned int iUsrVersion;
};

struct lsm_db {
    void* pShmhdr;            // NULL indicates no SHM header
    bool bReadonly;             // read-only flag
    int iReader;                // reader count (<0 means reading transaction active)
    LsmTreeHdr treehdr;         // user version information
};

// Public return code commonly used by the library
static const int LSM_OK = 0;

// Global state used by the stubs to simulate transactional behavior
static int s_beginRoTrans_rc = 0;
static int s_beginReadTrans_rc = 0;
static bool g_release_called = false;

// Stubbed transactional helpers (mimic library behavior for tests)
int lsmBeginRoTrans(lsm_db* /*pDb*/){
    // Return pre-configured rc to simulate success/failure
    return s_beginRoTrans_rc;
}
int lsmBeginReadTrans(lsm_db* /*pDb*/){
    return s_beginReadTrans_rc;
}
void dbReleaseClientSnapshot(lsm_db* /*pDb*/){
    // Track that the cleanup snapshot release was invoked
    g_release_called = true;
}

// Static helper in the original code (mocked for this test TU)
static void assert_db_state(lsm_db* /*pDb*/){
    // No-op for test harness; in real code this would validate invariants
}

// Reproduced focal method under test (as in the provided code)
int lsm_get_user_version(lsm_db *pDb, unsigned int *piUsr){
  int rc = LSM_OK;                /* Return code */
  /* Open a read transaction if one is not already open. */
  assert_db_state(pDb);
  if( pDb->pShmhdr==0 ){
    assert( pDb->bReadonly );
    rc = lsmBeginRoTrans(pDb);
  }else if( pDb->iReader<0 ){
    rc = lsmBeginReadTrans(pDb);
  }
  /* Allocate the multi-cursor. */
  if( rc==LSM_OK ){
    *piUsr = pDb->treehdr.iUsrVersion;
  }
  dbReleaseClientSnapshot(pDb);
  assert_db_state(pDb);
  return rc;
}

// Lightweight test harness (non-terminating assertions)
static std::vector<std::string> g_failures;
static int g_tests_run = 0;

// Helper to log a failure without aborting the test suite
static void log_failure(const std::string& test_name, const std::string& details){
    std::ostringstream oss;
    oss << test_name << ": " << details;
    g_failures.push_back(oss.str());
}

// Simple assertion helpers (non-terminating)
#define TEST_ASSERT(cond, test_name, details) do { \
    if(!(cond)) { log_failure(test_name, details); } \
} while(0)

#define TEST_EQUAL_U(a, b, test_name, details) do { \
    if((unsigned long long)(a) != (unsigned long long)(b)) { \
        std::ostringstream oss; \
        oss << details << " (expected " << (b) << ", got " << (a) << ")"; \
        log_failure(test_name, oss.str()); \
    } \
} while(0)

// Reset global/test state between tests
static void reset_test_state(){
    s_beginRoTrans_rc = 0;
    s_beginReadTrans_rc = 0;
    g_release_called = false;
}

// Test 1: Branch pDb.pShmhdr == 0 should attempt RoTrans and set *piUsr
// Expects: rc == LSM_OK, *piUsr == pDb.treehdr.iUsrVersion, and dbReleaseClientSnapshot was called.
static void test_branch_pShmhdr_zero_sets_usr_and_release(){
    reset_test_state();
    g_tests_run++;

    lsm_db db;
    db.pShmhdr = nullptr;            // first branch: pShmhdr == 0
    db.bReadonly = true;               // required by assert
    db.iReader = 7;                    // not used in this branch
    db.treehdr.iUsrVersion = 0xDEADBEEF;

    unsigned int usr = 0;
    int rc = lsm_get_user_version(&db, &usr);

    TEST_EQUAL_U((unsigned)rc, (unsigned)LSM_OK, "test_branch_pShmhdr_zero_sets_usr_and_release: rc");
    TEST_EQUAL_U(usr, db.treehdr.iUsrVersion, "test_branch_pShmhdr_zero_sets_usr_and_release: usr version");
    TEST_ASSERT(g_release_called, "test_branch_pShmhdr_zero_sets_usr_and_release: release called", "dbReleaseClientSnapshot should be invoked");
}

// Test 2: Branch pDb.pShmhdr != 0 and pDb.iReader < 0 should perform ReadTrans and set *piUsr
// Expects: rc == LSM_OK, *piUsr == pDb.treehdr.iUsrVersion, and dbReleaseClientSnapshot was called.
static void test_branch_pShmhdr_nonzero_iReader_negative_sets_usr_and_release(){
    reset_test_state();
    g_tests_run++;

    lsm_db db;
    db.pShmhdr = (void*)0x1;          // non-null to trigger the else-if branch
    db.bReadonly = false;              // value irrelevant for this branch
    db.iReader = -1;                   // trigger ReadTrans
    db.treehdr.iUsrVersion = 0x12345678;

    unsigned int usr = 0;
    int rc = lsm_get_user_version(&db, &usr);

    TEST_EQUAL_U((unsigned)rc, (unsigned)LSM_OK, "test_branch_pShmhdr_nonzero_iReader_negative_sets_usr_and_release: rc");
    TEST_EQUAL_U(usr, db.treehdr.iUsrVersion, "test_branch_pShmhdr_nonzero_iReader_negative_sets_usr_and_release: usr version");
    TEST_ASSERT(g_release_called, "test_branch_pShmhdr_nonzero_iReader_negative_sets_usr_and_release: release called", "dbReleaseClientSnapshot should be invoked");
}

// Test 3: rc failure path when RoTrans cannot be started should not modify *piUsr
// Expects: rc != LSM_OK, *piUsr unchanged, and dbReleaseClientSnapshot was still called.
static void test_rc_failure_does_not_modify_usr_and_calls_release(){
    reset_test_state();
    g_tests_run++;

    lsm_db db;
    db.pShmhdr = nullptr;            // will attempt RoTrans
    db.bReadonly = true;
    db.iReader = 0;
    db.treehdr.iUsrVersion = 0xCAFEBABE;

    unsigned int usr = 0;              // initial value
    // Configure RoTrans to fail
    s_beginRoTrans_rc = 5;              

    int rc = lsm_get_user_version(&db, &usr);

    TEST_EQUAL_U((unsigned)rc, (unsigned)5, "test_rc_failure_does_not_modify_usr_and_calls_release: rc");
    TEST_EQUAL_U(usr, (unsigned)0, "test_rc_failure_does_not_modify_usr_and_calls_release: usr should remain unchanged");
    TEST_ASSERT(g_release_called, "test_rc_failure_does_not_modify_usr_and_calls_release: release called", "dbReleaseClientSnapshot should be invoked even on failure");
}

// Helper to summarize test results
static void summarize_results(){
    std::cout << "lsm_get_user_version test suite results:\n";
    if(g_failures.empty()){
        std::cout << "All tests passed. Total tests: " << g_tests_run << "\n";
    } else {
        std::cout << g_failures.size() << " failure(s) out of " << g_tests_run << " tests:\n";
        for(const auto& s : g_failures){
            std::cout << " - " << s << "\n";
        }
        std::cout << "Total tests run: " << g_tests_run << "\n";
    }
}

// Entry point to run all tests
int main(){
    test_branch_pShmhdr_zero_sets_usr_and_release();
    test_branch_pShmhdr_nonzero_iReader_negative_sets_usr_and_release();
    test_rc_failure_does_not_modify_usr_and_calls_release();

    summarize_results();
    return g_failures.empty() ? 0 : 1;
}

// Notes on test design and coverage
// - Step 1: The test harness creates a minimal, controlled lsm_db structure with only the
//   members used by lsm_get_user_version. This isolates dependencies and focuses test
//   coverage on the focal method's logic.
// - Step 2: Three tests exercise true/false branches described in the focal method:
//     a) pShmhdr == 0 -> RoTrans path taken, rc OK, piUsr updated.
//     b) pShmhdr != 0 and iReader < 0 -> ReadTrans path taken, rc OK, piUsr updated.
//     c) RoTrans fails (rc != OK) -> rc returned, piUsr remains unchanged, release still called.
// - Step 3: The tests use a lightweight non-terminating assertion mechanism (log_failure) to
//   allow all tests to run and report all failures, aligning with the domain knowledge request.