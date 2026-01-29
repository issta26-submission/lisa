// Unit test suite for sqlite3ErrorToParser using a lightweight, self-contained runner
// This test targets the focal method:
// int sqlite3ErrorToParser(sqlite3 *db, int errCode)
//
// Behavior to verify:
// - If db is NULL or db->pParse is NULL, returns errCode without modifying any Parse state.
// - If db and db->pParse are non-NULL, sets pParse->rc = errCode, increments pParse->nErr, and returns errCode.
//
// The tests assume the project provides sqliteInt.h (as per <FOCAL_CLASS_DEP>), which defines
// the sqlite3 and Parse structures used by sqlite3ErrorToParser.

#include <math.h>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>


// Include internal sqlite types used by sqlite3ErrorToParser.
// This header is project-specific as per the provided dependencies.


// Simple non-terminating assertion framework for C++11 without GTest.
// Tracks number of failures and prints diagnostic messages.
// All tests will be executed; program exits with non-zero status if any test fails.
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "ASSERTION FAILED: " << (msg) << std::endl; \
            ++g_failures; \
        } \
    } while(0)


// Test 1: db is NULL -> function should return the provided errCode unchanged.
static void test_sqlite3ErrorToParser_NullDb_ReturnsErrCode() {
    // Domain knowledge: true branch when db == NULL
    int errCode = 123;
    int rc = sqlite3ErrorToParser(nullptr, errCode);
    ASSERT_TRUE(rc == errCode, "When db is NULL, return value must equal errCode");
}


// Test 2: db is non-NULL but db->pParse is NULL -> function should return errCode unchanged.
static void test_sqlite3ErrorToParser_NullPParse_ReturnsErrCode() {
    // Prepare a dummy sqlite3 object with pParse = NULL
    sqlite3 db;
    // Ensure pParse is NULL; depending on compiler, uninitialized memory might be unsafe.
    // Explicitly set to NULL to model the scenario.
    db.pParse = nullptr;

    int errCode = 456;
    int rc = sqlite3ErrorToParser(&db, errCode);
    ASSERT_TRUE(rc == errCode, "When db->pParse is NULL, return value must equal errCode");
}


// Test 3: db and db->pParse are non-NULL -> pParse->rc should be set, nErr incremented, and return value matches errCode.
// Additionally verifies that repeated calls continue to update state correctly.
static void test_sqlite3ErrorToParser_UpdatesParseState() {
    // Prepare a Parse object and a sqlite3 wrapper around it
    Parse p;
    p.rc = -1;   // initial state
    p.nErr = 0;  // initial state

    sqlite3 db;
    db.pParse = &p;

    // First invocation
    int errCode1 = 7;
    int rc1 = sqlite3ErrorToParser(&db, errCode1);
    ASSERT_TRUE(rc1 == errCode1, "First call should return the provided errCode");
    ASSERT_TRUE(p.rc == errCode1, "First call should set pParse->rc to errCode");
    ASSERT_TRUE(p.nErr == 1, "First call should increment pParse->nErr to 1");

    // Second invocation with a different error code
    int errCode2 = 42;
    int rc2 = sqlite3ErrorToParser(&db, errCode2);
    ASSERT_TRUE(rc2 == errCode2, "Second call should return the provided errCode");
    ASSERT_TRUE(p.rc == errCode2, "Second call should update pParse->rc to new errCode");
    ASSERT_TRUE(p.nErr == 2, "Second call should increment pParse->nErr to 2");
}


// Entry point for the test suite
int main() {
    // Run tests
    test_sqlite3ErrorToParser_NullDb_ReturnsErrCode();
    test_sqlite3ErrorToParser_NullPParse_ReturnsErrCode();
    test_sqlite3ErrorToParser_UpdatesParseState();

    // Report summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}