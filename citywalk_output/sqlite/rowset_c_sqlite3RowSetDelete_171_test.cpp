// Unit test suite for sqlite3RowSetDelete in rowset.c
// This test suite is designed to be self-contained and uses a lightweight
// assertion mechanism (non-terminating) to maximize code coverage without
// relying on GoogleTest. It mocks the dependencies of sqlite3RowSetDelete
// (sqlite3RowSetClear and sqlite3DbFree) via C linkage to observe side effects.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>


// Forward declare minimal types to satisfy the test harness for the focal function.
// We assume the actual RowSet used by sqlite3RowSetDelete has a member "db" of type sqlite3*.
typedef struct sqlite3 sqlite3;
typedef long long i64;

// Forward declaration of the focal function from rowset.c
extern "C" void sqlite3RowSetDelete(void *pArg);

// Minimal RowSet layout expected by sqlite3RowSetDelete: a struct whose first member
// is sqlite3* db. This mirrors the usage: ((RowSet*)pArg)->db.
struct RowSet {
    sqlite3* db;
};

// Global state to observe calls from sqlite3RowSetDelete and its dependencies
static bool g_clear_called = false;
static void *g_clear_arg = nullptr;

static sqlite3* g_last_db_free = nullptr;
static void *g_last_ptr_free = nullptr;

// Mock implementations of the dependency functions with C linkage.
// They record invocation details so we can verify the focal method behavior.

extern "C" {

// Mock: sqlite3RowSetClear(void *pArg)
void sqlite3RowSetClear(void *pArg) {
    g_clear_called = true;
    g_clear_arg = pArg;
}

// Mock: sqlite3DbFree(sqlite3 *db, void *ptr)
void sqlite3DbFree(void *db, void *ptr) {
    g_last_db_free = (sqlite3*)db;
    g_last_ptr_free = ptr;
}
} // extern "C"

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void expect_true(bool cond, const char *message) {
    ++g_total_tests;
    if (!cond) {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << message << std::endl;
    } else {
        // Optional: verbose pass indicator (comment out if noisy)
        // std::cerr << "[PASS] " << message << std::endl;
    }
}

static void expect_eq_ptr(void* a, void* b, const char* message) {
    expect_true(a == b, message);
}

// Test case 1: Verify that sqlite3RowSetDelete calls sqlite3RowSetClear with the same argument
// and then forwards the correct parameters to sqlite3DbFree.
static void test_rowset_delete_calls_clear_and_free_once() {
    // Reset observable state
    g_clear_called = false;
    g_clear_arg = nullptr;
    g_last_db_free = nullptr;
    g_last_ptr_free = nullptr;

    // Prepare a RowSet instance and its dummy sqlite3 db
    sqlite3 dummy_db;
    RowSet rs;
    rs.db = &dummy_db;

    void* pArg = &rs;

    // Invoke the focal method
    sqlite3RowSetDelete(pArg);

    // Validate that sqlite3RowSetClear was called with the exact argument
    expect_true(g_clear_called, "sqlite3RowSetClear should be called exactly once by sqlite3RowSetDelete");
    expect_eq_ptr(g_clear_arg, pArg, "sqlite3RowSetClear should receive the original argument (pArg)");

    // Validate that sqlite3DbFree was called with the correct db and pointer
    expect_true(g_last_db_free == &dummy_db, "sqlite3DbFree should be called with the RowSet's db pointer");
    expect_eq_ptr(g_last_ptr_free, pArg, "sqlite3DbFree should be called with the RowSet argument pointer");
}

// Test case 2: Verify behavior with a second, distinct RowSet instance
static void test_rowset_delete_with_second_instance() {
    // Reset observable state
    g_clear_called = false;
    g_clear_arg = nullptr;
    g_last_db_free = nullptr;
    g_last_ptr_free = nullptr;

    // Prepare a second RowSet instance and its dummy sqlite3 db
    sqlite3 dummy_db2;
    RowSet rs2;
    rs2.db = &dummy_db2;

    void* pArg2 = &rs2;

    // Invoke the focal method
    sqlite3RowSetDelete(pArg2);

    // Validate that sqlite3RowSetClear was called with the correct argument
    expect_true(g_clear_called, "sqlite3RowSetClear should be called for second instance");
    expect_eq_ptr(g_clear_arg, pArg2, "sqlite3RowSetClear should receive the second argument (pArg2)");

    // Validate that sqlite3DbFree was called with the correct db and pointer
    expect_true(g_last_db_free == &dummy_db2, "sqlite3DbFree should be called with the second RowSet's db");
    expect_eq_ptr(g_last_ptr_free, pArg2, "sqlite3DbFree should be called with the second RowSet argument");
}

// Entry point for running tests
int main() {
    // Run tests
    test_rowset_delete_calls_clear_and_free_once();
    test_rowset_delete_with_second_instance();

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}