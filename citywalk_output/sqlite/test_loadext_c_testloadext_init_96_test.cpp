// test_loadext.cpp
// Lightweight C++11 test suite for the focal function: testloadext_init
// This test suite is designed to compile without Google Test and to run
// as a simple standalone executable. It exercises the focal function's
// behavior with a valid in-memory SQLite database and with an invalid (NULL) db.
// Explanatory comments are provided for each test to describe intent and coverage.

/*
Key concepts covered (mapped to Candidate Keywords from Step 1):
- testloadext_init: the function under test
- sqlite3_create_function: three registrations attempted
- "half" (arity 1) and "sqlite3_status" (arity 1 and 2)
- SQLITE_EXTENSION_INIT2(pApi): macro initialization with pApi
- SQLITE_OK / SQLITE_ERROR: return value mapping
- pApi (sqlite3_api_routines): required by SQLITE_EXTENSION_INIT2
- true/false condition coverage via nErr accumulation
- Domain knowledge: C/C++ interop, non-terminating assertions, minimal framework
*/

// Include standard library for simple test harness
#include <string.h>
#include <vector>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <string>
#include <iostream>


// Include SQLite header for API

// Ensure the focal function is visible for linking with C linkage
extern "C" int testloadext_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
);

// Simple non-terminating assertion helpers
static int g_failures = 0;

static void expect_true(bool cond, const std::string &msg) {
    if (!cond) {
        std::cerr << "EXPECT_TRUE FAILED: " << msg << std::endl;
        ++g_failures;
    }
}

static void expect_eq(int a, int b, const std::string &msg) {
    if (a != b) {
        std::cerr << "EXPECT_EQ FAILED: " << msg
                  << " (expected " << b << ", got " << a << ")" << std::endl;
        ++g_failures;
    }
}

// Test 1: Valid database should lead to SQLITE_OK from testloadext_init
static void test_loadext_init_ok() {
    // Import dependencies and set up a fresh in-memory database
    sqlite3 *db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    expect_true(db != nullptr, "Database should open successfully (in-memory).");
    expect_true(rc_open == SQLITE_OK, "sqlite3_open should return SQLITE_OK for in-memory DB.");

    // Prepare a dummy pApi to satisfy the extension macro
    sqlite3_api_routines api = {0}; // zero-initialized; tests do not call into real API paths here

    // Call the focal function
    char *pzErrMsg = nullptr;
    int rc = testloadext_init(db, &pzErrMsg, &api);

    // Verify behavior: no error across all sqlite3_create_function calls
    expect_eq(rc, SQLITE_OK, "testloadext_init should return SQLITE_OK for a valid DB.");

    // Clean up
    sqlite3_close(db);
}

// Test 2: NULL db should cause a failure (non-zero nErr, leading to SQLITE_ERROR)
static void test_loadext_init_null_db() {
    // Prepare a dummy pApi to satisfy the macro
    sqlite3_api_routines api = {0};

    char *pzErrMsg = nullptr;
    // Pass NULL db to induce an error from sqlite3_create_function
    int rc = testloadext_init(nullptr, &pzErrMsg, &api);

    // Expect SQLITE_ERROR due to at least one failure in the chain
    expect_eq(rc, SQLITE_ERROR, "testloadext_init should return SQLITE_ERROR when db is NULL.");
}

// Optional: quick smoke check that the test harness can run multiple tests
static void run_all_tests() {
    test_loadext_init_ok();
    test_loadext_init_null_db();
    // Future tests could be added here to increase coverage (e.g., simulate partial failures
    // by injecting mocks or by verifying function availability via SQL execution).
}

int main() {
    // Run tests; non-terminating assertions will accumulate failures
    run_all_tests();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}