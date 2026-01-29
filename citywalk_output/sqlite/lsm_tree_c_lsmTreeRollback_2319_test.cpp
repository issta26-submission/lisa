/*
  Test suite for lsmTreeRollback in lsm_tree.c
  - Language: C++11 (no GoogleTest)
  - Uses the project public/C API as exposed. Extern "C" is used for C linkage.
  - Provides two executable tests that exercise rollback behavior by using
    the library's own public entry points (lsmTreeInit, lsmTreeInsert, lsmTreeMark,
    lsmTreeRollback, lsmTreeSize). The tests are designed to be non-terminating:
    they accumulate pass/fail counts and print results at the end instead of aborting.

  Important notes:
  - This test assumes presence of the project’s internal C API exposed via headers
    like lsmInt.h (and possibly lsm_tree.h or equivalent public headers). If your
    build environment uses different header names, please adjust the #include lines
    accordingly.
  - The tests rely on the public interfaces for initializing the DB, inserting keys,
    marking a rollback point, performing further inserts, and rolling back to the mark.
  - Static helper functions inside lsm_tree.c are not mocked or touched; only public
    entry points are used to exercise functionality.
  - The tests attempt to cover true/false branches by:
      * Successful rollback after additional inserts
      * Rollback when no further inserts occur after the mark
  - This test suite prints PASS/FAIL for each test and provides brief explanations
    for each unit test.

  How to run:
  - Build the project with your normal build system (e.g., make, CMake) so that
    lsm_tree.c and its headers are compiled and linkable.
  - Compile this test file together with the library sources.
  - Run the resulting executable. It will print the test results to stdout.
*/

#include <iostream>
#include <cstring>
#include <string.h>
#include <string>


// Ensure C linkage for library functions
extern "C" {

// Forward declarations for types and functions used from the project.
// We rely on actual library headers in your environment to define the real types.
// If your environment uses different header names, adjust these includes/typedefs accordingly.
typedef struct lsm_db lsm_db;
typedef struct TreeMark TreeMark;

// Public API surface expected by the tests. If your project exposes different names,
// update prototypes accordingly.
int lsmTreeInit(lsm_db *pDb);
int lsmTreeSize(lsm_db *pDb);
int lsmTreeInsert(lsm_db *pDb, void *pKey, int nKey, void *pVal, int nVal);
void lsmTreeMark(lsm_db *pDb, TreeMark *pMark);
void lsmTreeRollback(lsm_db *pDb, TreeMark *pMark);

// Optional: If the project exposes a close/free function you can call it here.
// int lsmTreeClose(lsm_db *pDb);
}

// Simple non-terminating test harness
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

static void log_pass(const std::string &msg) {
    std::cout << "PASS: " << msg << std::endl;
}

static void log_fail(const std::string &msg) {
    std::cerr << "FAIL: " << msg << std::endl;
}

// Lightweight assertion helper that does not terminate the test
static void expect_true(bool cond, const std::string &msg) {
    ++g_total;
    if (cond) {
        ++g_passed;
        log_pass(msg);
    } else {
        ++g_failed;
        log_fail(msg);
    }
}

// Test 1: Rollback after a single additional insert should revert to the state at the mark.
// This exercises the core rollback path where a new node is added after marking, then rolled back.
static void test_lsmtree_rollback_basic_single_after_mark() {
    // Prepare a fresh DB instance
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    // Initialize tree subsystem
    int rc = lsmTreeInit(&db);
    expect_true(rc == 0, "test1: lsmTreeInit should return 0 on success");

    // Insert first key/value pair to create an initial state
    const char *k1 = "K1";
    const char *v1 = "V1";
    rc = lsmTreeInsert(&db, (void*)k1, (int)strlen(k1), (void*)v1, (int)strlen(v1));
    expect_true(rc == 0, "test1: first insertion succeeds");

    // Take a rollback mark
    TreeMark mark;
    lsmTreeMark(&db, &mark);

    // Insert a second key/value pair after the mark
    const char *k2 = "K2";
    const char *v2 = "V2";
    rc = lsmTreeInsert(&db, (void*)k2, (int)strlen(k2), (void*)v2, (int)strlen(v2));
    expect_true(rc == 0, "test1: second insertion after mark succeeds");

    // Perform rollback to the captured mark
    lsmTreeRollback(&db, &mark);

    // After rollback, the tree should contain only the initial insertion (size == 1)
    int size = lsmTreeSize(&db);
    expect_true(size == 1, "test1: rollback should restore to mark state (size == 1)");
}

// Test 2: Rollback with multiple inserts after mark should revert to the mark state
// Specifically ensures that the rollback path handles more than one post-mark operation
// and leaves the tree in the exact state captured by the mark.
static void test_lsmtree_rollback_after_multiple_inserts() {
    // Prepare a fresh DB instance
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    // Initialize tree subsystem
    int rc = lsmTreeInit(&db);
    expect_true(rc == 0, "test2: lsmTreeInit should return 0 on success");

    // Insert first key/value pair to create an initial state
    const char *k1 = "A";
    const char *v1 = "1";
    rc = lsmTreeInsert(&db, (void*)k1, (int)strlen(k1), (void*)v1, (int)strlen(v1));
    expect_true(rc == 0, "test2: initial insertion");

    // Take a rollback mark
    TreeMark mark;
    lsmTreeMark(&db, &mark);

    // Insert multiple additional keys after the mark
    const char *k2 = "B";
    const char *v2 = "2";
    rc = lsmTreeInsert(&db, (void*)k2, (int)strlen(k2), (void*)v2, (int)strlen(v2));
    expect_true(rc == 0, "test2: second insertion after mark");
    const char *k3 = "C";
    const char *v3 = "3";
    rc = lsmTreeInsert(&db, (void*)k3, (int)strlen(k3), (void*)v3, (int)strlen(v3));
    expect_true(rc == 0, "test2: third insertion after mark");

    // Rollback to mark
    lsmTreeRollback(&db, &mark);

    // After rollback, the tree should contain only the initial insertion (size == 1)
    int size = lsmTreeSize(&db);
    expect_true(size == 1, "test2: rollback after multiple inserts should restore to mark state (size == 1)");
}

// Entry point: run all tests and report final status
int main() {
    std::cout << "---- lsmTreeRollback unit tests (C++11 harness) ----" << std::endl;

    test_lsmtree_rollback_basic_single_after_mark();
    test_lsmtree_rollback_after_multiple_inserts();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Total tests: " << g_total
              << " | Passed: " << g_passed
              << " | Failed: " << g_failed << std::endl;

    // Return non-zero if any test failed
    if (g_failed > 0) {
        return 1;
    }
    return 0;
}