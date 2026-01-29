// C++11 test harness for the focal function sqlite3_amatch_init in amatch.c
// This test suite is designed to be compiled in two separate configurations:
// 1) Without defining SQLITE_OMIT_VIRTUALTABLE (path where the extension tries to register the module).
// 2) With SQLITE_OMIT_VIRTUALTABLE defined (path where the module registration is skipped).
// The tests use a minimal interposition of sqlite3_create_module to verify the call flow
// and capture the behavior of sqlite3_amatch_init without requiring a full SQLite extension load.
//
// Notes:
// - The test includes amatch.c directly to ensure the focal function and its dependencies are in this translation unit.
// - A C linkage override of sqlite3_create_module is provided to observe whether the function is invoked.
// - The test uses lightweight, non-terminating assertions to maximize code coverage without terminating on first failure.
// - The code is written to be compilable under C++11 with no Google Test or external mocking framework.

#include <stdlib.h>
#include <amatch.c>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <iostream>


// Bring in the focal C source directly to ensure the test runs in a single translation unit.
// The amatch.c file defines sqlite3_amatch_init and references many SQLite-related entities.
// Wrapping the include with extern "C" ensures correct linkage for C symbols when compiled as C++.
extern "C" {
}

// Global state to verify interposed sqlite3_create_module behavior
static int g_create_module_calls = 0;
static const char* g_last_module_name = nullptr;

// Minimal, C-linkage override to intercept calls to sqlite3_create_module.
// This allows us to verify that sqlite3_amatch_init attempts to register the module
// when SQLITE_OMIT_VIRTUALTABLE is not defined.
extern "C" int sqlite3_create_module(sqlite3 *db, const char *zName, const sqlite3_module *p, void *pAux) {
    (void)db;   // unused in test
    (void)pAux; // unused in test
    ++g_create_module_calls;
    g_last_module_name = zName;
    // Simulate successful module registration, as many SQLite environments expect SQLITE_OK
    return SQLITE_OK;
}

// Lightweight test assertion mechanism: non-terminating, prints failures, tracks count.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "Test failure: " << (msg) << " [in " << __FUNCTION__ << "]" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

// Helper to set up a minimal sqlite3_api_routines structure (often opaque in extensions).
// Some SQLite builds provide a large struct; here we rely on zero-initialization to be safe.
static void init_minimal_api_routines(sqlite3_api_routines *pApi) {
    if(pApi) {
        // Zero initialize to avoid using any uninitialized function pointers
        std::memset(pApi, 0, sizeof(*pApi));
    }
}

/*
Domain knowledge notes implemented in tests:
- The test uses in-memory SQLite DBs to avoid filesystem dependencies.
- We verify that, in the non-OMIT path, the module registration attempt is made by checking the interposed create_module call count
  and the module name equals "approximate_match".
- In the OMIT_VIRTUALTABLE path, sqlite3_create_module should not be invoked at all.
- Non-terminating assertions ensure the test continues after a failure to improve coverage.
*/

// Forward declarations for test blocks (compiled conditionally based on macro)
#ifdef SQLITE_OMIT_VIRTUALTABLE

// Test: Path when SQLITE_OMIT_VIRTUALTABLE is defined (module creation is omitted)
static void test_amatch_init_omit_path() {
    std::cout << "Running test_amatch_init_omit_path: SQLITE_OMIT_VIRTUALTABLE defined (no module creation)." << std::endl;

    // Reset interposition state
    g_create_module_calls = 0;
    g_last_module_name = nullptr;

    // Open an in-memory database
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    TEST_ASSERT(rc_open == SQLITE_OK, "sqlite3_open should succeed for in-memory DB");

    // Provide a minimal, zero-initialized api struct to avoid dereferencing real API pointers
    sqlite3_api_routines api;
    init_minimal_api_routines(&api);

    char* pErr = nullptr;
    int rc = sqlite3_amatch_init(db, &pErr, &api);

    // The path should not call sqlite3_create_module; rc should be SQLITE_OK
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3_amatch_init should return SQLITE_OK when OMIT_VIRTUALTABLE is defined");
    TEST_ASSERT(g_create_module_calls == 0, "sqlite3_create_module should not be called in OMIT_VIRTUALTABLE path");
    TEST_ASSERT(g_last_module_name == nullptr, "Module name should remain uninitialized when not called");

    // Cleanup
    sqlite3_close(db);
}

#else

// Test: Path when SQLITE_OMIT_VIRTUALTABLE is not defined (module creation should be attempted)
static void test_amatch_init_with_module() {
    std::cout << "Running test_amatch_init_with_module: module creation path (approximate_match) should be invoked." << std::endl;

    // Reset interposition state
    g_create_module_calls = 0;
    g_last_module_name = nullptr;

    // Open an in-memory database
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    TEST_ASSERT(rc_open == SQLITE_OK, "sqlite3_open should succeed for in-memory DB");

    // Minimal API struct
    sqlite3_api_routines api;
    init_minimal_api_routines(&api);

    char* pErr = nullptr;
    int rc = sqlite3_amatch_init(db, &pErr, &api);

    // Our override returns SQLITE_OK (0) to mimic successful module registration.
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3_amatch_init should return SQLITE_OK when module registration succeeds");
    TEST_ASSERT(g_create_module_calls > 0, "sqlite3_create_module should be called in non-OMIT path");
    TEST_ASSERT(g_last_module_name != nullptr, "Module name should be captured as non-null");
    TEST_ASSERT(std::strcmp(g_last_module_name, "approximate_match") == 0,
                "Module name should be 'approximate_match'");

    // Cleanup
    sqlite3_close(db);
}
#endif

int main() {
    // Run selected tests based on compile-time macro
#ifdef SQLITE_OMIT_VIRTUALTABLE
    test_amatch_init_omit_path();
#else
    test_amatch_init_with_module();
#endif

    if(g_failures > 0) {
        std::cerr << "Total test failures: " << g_failures << std::endl;
        return 1;
    }

    std::cout << "All tests completed." << std::endl;
    return 0;
}