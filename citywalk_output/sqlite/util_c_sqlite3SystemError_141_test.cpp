/*
 * SQLite3SystemError - Unit Test Suite (C++11, no GoogleTest)
 *
 * This test suite targets the focal method:
 *   void sqlite3SystemError(sqlite3 *db, int rc)
 *
 * Notes and caveats:
 * - The test relies on internal SQLite structures (sqliteInt.h) to construct a
 *   small in-memory sqlite3 object suitable for exercising sqlite3SystemError.
 * - We provide lightweight, side-effect free stubs for external functions used
 *   by sqlite3SystemError (e.g., sqlite3OsGetLastError, sqlite3BtreePager,
 *   sqlite3PagerWalSystemErrno, sqlite3BtreeEnterAll, sqlite3BtreeLeaveAll).
 * - We simulate both branches of the core predicates and verify that
 *   iSysErrno is set (or not) as expected.
 * - The tests avoid terminating the program on assertion failure; instead they
 *   report failures and continue, producing a final summary.
 * - The tests are designed to be compiled into the same binary as the focal
 *   method (util.c) and linked with the project build.
 *
 * How to build (example):
 *   - Ensure SQLITE_USE_SEH is defined or not as needed to exercise in-page
 *     error handling branch.
 *   - Compile and link with util.c (containing sqlite3SystemError) and this file.
 *   - Run the resulting executable.
 *
 * This file contains explanatory comments for each test case.
 */

#include <math.h>
#include <new>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Include internal SQLite headers to access the necessary structures.
// This assumes the project provides the internal headers (sqliteInt.h, etc.).
// If your build differs, adjust includes accordingly.

// Forward declaration of the focal function from util.c (C linkage)
extern "C" void sqlite3SystemError(sqlite3 *db, int rc);

// STUBS: Lightweight, deterministic simulation of external dependencies.
// These stubs allow exercising the focal method without bringing up the full
// SQLite environment. They mirror the minimal surface that sqlite3SystemError uses.

static int g_lastOsError = 0;    // Simulated OsGetLastError return
static int g_pagerErrno  = 0;    // Simulated Pager/GetErrno return

// Stub: sqlite3OsGetLastError
extern "C" int sqlite3OsGetLastError(sqlite3_vfs *pVfs) {
    (void)pVfs;
    return g_lastOsError;
}

// Stub: sqlite3BtreeEnterAll
extern "C" void sqlite3BtreeEnterAll(sqlite3 *db) {
    (void)db;
    // No-op for testing purposes
}

// Stub: sqlite3BtreeLeaveAll
extern "C" void sqlite3BtreeLeaveAll(sqlite3 *db) {
    (void)db;
    // No-op for testing purposes
}

// Stub: sqlite3BtreePager
// Return a non-null handle to simulate a valid pager for a given Btree.
// We use the input pointer as the "pager" handle to keep the test simple.
extern "C" void* sqlite3BtreePager(void *pBt) {
    return pBt; // Pass-through; tests can control pBt to influence behavior
}

// Stub: sqlite3PagerWalSystemErrno
extern "C" int sqlite3PagerWalSystemErrno(void *pager) {
    (void)pager;
    // Return a deterministic value configured by tests
    return g_pagerErrno;
}

// Helper: create a test sqlite3 database instance
// We rely on internal layout (sqliteInt.h) so that we can poke internal fields.
// We use sqlite3_open to obtain a valid sqlite3* and then tweak internal fields.
static sqlite3* createTestDb(int nDb, bool withBt) {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if( rc != SQLITE_OK || db == nullptr ) {
        std::cerr << "Failed to open in-memory DB for test. rc=" << rc << "\n";
        return nullptr;
    }

    // Set up internal fields (best effort; relies on internal layout)
    // nDb
    db->nDb = nDb;

    // Allocate aDb array if necessary
    // aDb is typically an array or pointer; attempt to allocate if needed
    if( db->aDb == nullptr ) {
        // Use the same approach as SQLite might internally use
        // Here we attempt to allocate an array of 'Db' structures of size nDb
        // Note: If the project uses a different internal layout, this may need adjustment.
        typedef decltype(db->aDb) ADbType; // deduce type of aDb
        ADbType anew = new typename std::remove_pointer<ADbType>::type[nDb];
        db->aDb = anew;
    }

    // Initialize Db entries
    for(int i=0; i<nDb; ++i){
        // pBt non-null if requested
        if(withBt && i % 2 == 0){
            // Acquire the member type via decltype to cast a fake non-null pointer
            db->aDb[i].pBt = reinterpret_cast<decltype(db->aDb[i].pBt)>(0x1);
        }else{
            db->aDb[i].pBt = nullptr;
        }
        // Other fields of Db are not used by our test
    }

    // iSysErrno initial value
    db->iSysErrno = 0;
    // pVfs can be NULL or a dummy pointer
    db->pVfs = nullptr;

    return db;
}

// Helpers for assertions
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_EQ(expected, actual, testname)                          \
    do {                                                               \
        g_tests_run++;                                                 \
        if( (expected) != (actual) ) {                               \
            ++g_tests_failed;                                        \
            std::fprintf(stderr, "TEST FAILED: %s: expected %lld, got %lld\n", \
                testname, (long long)(expected), (long long)(actual)); \
        } else {                                                       \
            std::printf("TEST PASSED: %s\n", testname);              \
        }                                                              \
    } while(0)

#define ASSERT_TRUE(cond, testname)                                      \
    do {                                                                   \
        g_tests_run++;                                                     \
        if(!(cond)) {                                                      \
            ++g_tests_failed;                                            \
            std::fprintf(stderr, "TEST FAILED: %s: condition false\n", testname); \
        } else {                                                           \
            std::printf("TEST PASSED: %s\n", testname);                  \
        }                                                                  \
    } while(0)

// Test 1: SQLITE_IOERR_NOMEM should short-circuit and not modify db->iSysErrno
static void test_ioerr_nmem_noop() {
    sqlite3* db = createTestDb(1, false);
    if(!db){
        std::cerr << "Cannot create test db for test_ioerr_nmem_noop\n";
        g_tests_failed++;
        return;
    }

    // Pre-condition
    db->iSysErrno = 999; // non-zero initial value

    // Act
    sqlite3SystemError(db, SQLITE_IOERR_NOMEM);

    // Post-condition: iSysErrno should remain unchanged
    ASSERT_EQ((int)999, db->iSysErrno, "test_ioerr_nmem_noop_iSysErrno_unchanged");

    // Cleanup
    delete[] db->aDb;
    sqlite3_close(db);
}

// Test 2: CANTOPEN should set iSysErrno from OsGetLastError
static void test_ioerr_cantopen_sets_last_error() {
    sqlite3* db = createTestDb(1, false);
    if(!db){
        std::cerr << "Cannot create test db for test_ioerr_cantopen_sets_last_error\n";
        g_tests_failed++;
        return;
    }

    // Configure OsGetLastError
    g_lastOsError = 1234;

    // Act
    sqlite3SystemError(db, SQLITE_CANTOPEN);

    // Post-condition
    ASSERT_EQ(1234, db->iSysErrno, "test_ioerr_cantopen_iSysErrno_from_last_error");

    // Cleanup
    delete[] db->aDb;
    sqlite3_close(db);
}

// Test 3: IOERR should set iSysErrno from OsGetLastError
static void test_ioerr_sets_last_error() {
    sqlite3* db = createTestDb(1, false);
    if(!db){
        std::cerr << "Cannot create test db for test_ioerr_sets_last_error\n";
        g_tests_failed++;
        return;
    }

    // Configure OsGetLastError
    g_lastOsError = 5678;

    // Act
    sqlite3SystemError(db, SQLITE_IOERR);

    // Post-condition
    ASSERT_EQ(5678, db->iSysErrno, "test_ioerr_sets_last_error_iSysErrno");

    // Cleanup
    delete[] db->aDb;
    sqlite3_close(db);
}

// Test 4: IN_PAGE path under SQLITE_USE_SEH
// This test should only compile/run when the macro SQLITE_USE_SEH is defined.
// It validates that the loop over nDb entries executes and updates iSysErrno
// according to sqlite3PagerWalSystemErrno results.
#ifdef SQLITE_USE_SEH
static void test_in_page_path_updates_sys_errno_with_btree() {
    sqlite3* db = createTestDb(2, true);
    if(!db){
        std::cerr << "Cannot create test db for test_in_page_path_updates_sys_errno_with_btree\n";
        g_tests_failed++;
        return;
    }

    // Prepare: at least one non-null pBt to trigger the loop body
    // The first aDb entry is non-null due to withBt = true in createTestDb

    // Configure stubs
    g_pagerErrno = 7;      // iErr for first iteration
    // When there are multiple entries, the last non-zero will survive;
    // we will rely on first non-zero to set iSysErrno to 7.

    // Act
    sqlite3SystemError(db, SQLITE_IOERR_IN_PAGE);

    // Post-condition: iSysErrno should be set to 7 (pager errno)
    ASSERT_EQ(7, db->iSysErrno, "test_in_page_path_updates_sys_errno_with_btree");

    // Cleanup
    delete[] db->aDb;
    sqlite3_close(db);
}
#endif

// Test 5: IN_PAGE path with no pBt should not change iSysErrno
#ifdef SQLITE_USE_SEH
static void test_in_page_path_no_btree_no_change() {
    sqlite3* db = createTestDb(1, false); // no BTrees
    if(!db){
        std::cerr << "Cannot create test db for test_in_page_path_no_btree_no_change\n";
        g_tests_failed++;
        return;
    }

    g_pagerErrno = 0;
    // Call with IN_PAGE but no BTrees present
    sqlite3SystemError(db, SQLITE_IOERR_IN_PAGE);

    // Since there are no pBt entries, iSysErrno should remain at its initial value (0)
    ASSERT_EQ(0, db->iSysErrno, "test_in_page_path_no_btree_no_change");

    // Cleanup
    delete[] db->aDb;
    sqlite3_close(db);
}
#endif

static void run_all_tests() {
    test_ioerr_nmem_noop();
    test_ioerr_cantopen_sets_last_error();
    test_ioerr_sets_last_error();
#ifdef SQLITE_USE_SEH
    test_in_page_path_updates_sys_errno_with_btree();
    test_in_page_path_no_btree_no_change();
#endif
}

// Entry point
int main() {
    // Run the test suite
    run_all_tests();

    // Summary
    std::cout << "\nTest Summary: "
              << g_tests_run << " tests run, "
              << g_tests_failed << " failed.\n";

    return g_tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}