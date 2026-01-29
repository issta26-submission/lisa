/*
Unit test suite for the focal method:
  int sqlite3_vtablog_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)

Overview:
- The real function delegates to sqlite3_create_module(...) with a fixed module name "vtablog" and a module object vtablogModule.
- We cannot rely on the full SQLite runtime in this test harness, so we provide a controlled mock for sqlite3_create_module
  by declaring it in the test TU and capturing its invocation parameters. This allows us to verify:
  - The function returns whatever sqlite3_create_module returns (rc propagation).
  - The module name passed to sqlite3_create_module is exactly "vtablog".
  - The pzErrMsg argument is not modified by the function (basic side-effect check).
- We keep this test implementation independent of Google Test (no external test framework).
- The tests are designed to be compiled in a C++11 environment that also provides vtablog.c (the focal
  implementation). The test links against vtablog.c and uses a mock sqlite3_create_module to intercept calls.

Notes:
- We declare minimal external symbols (sqlite3, sqlite3_module, sqlite3_api_routines) to satisfy linkage
  with the focal file, without pulling in the real SQLite headers (the test harness relies on the real
  environment to supply them when building).
- We assume the real vtablog.c defines the symbol vtablogModule (as used by sqlite3_vtablog_init). We
  reference it as extern sqlite3_module vtablogModule.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <string>


// Forward declare the minimal C types expected by the focal function.
// We do not pull in the real SQLite headers here to keep the test self-contained.
// The actual build should provide the real definitions when linking with vtablog.c.
extern "C" {
    typedef struct sqlite3 sqlite3;
    typedef struct sqlite3_api_routines sqlite3_api_routines;
    typedef struct sqlite3_module sqlite3_module;

    // The focal function to test is declared in the real source file (vtablog.c).
    // We rely on the actual object file during linking.
    int sqlite3_vtablog_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

    // The module used by the focal function in the real code.
    extern sqlite3_module vtablogModule;
    
    // Mock of the SQLite API used by the focal function:
    // We intercept the call to sqlite3_create_module in order to control its return value.
    int sqlite3_create_module(sqlite3 *db, const char *zName, sqlite3_module *pModule, void *pClientData);
}

// Global state for the mock
static int g_fake_rc = 0;
static const char* g_last_name = nullptr;
static sqlite3* g_last_db = nullptr;
static sqlite3_module* g_last_module = nullptr;
static void* g_last_client = nullptr;

// The mock implementation of sqlite3_create_module.
// It records the invocation parameters and returns g_fake_rc.
extern "C" int sqlite3_create_module(sqlite3 *db, const char *zName, sqlite3_module *pModule, void *pClientData) {
    g_last_db = db;
    g_last_name = zName;
    g_last_module = pModule;
    g_last_client = pClientData;
    return g_fake_rc;
}

// Helper to reset captured data before each test
static void reset_mock_state() {
    g_fake_rc = 0;
    g_last_name = nullptr;
    g_last_db = nullptr;
    g_last_module = nullptr;
    g_last_client = nullptr;
}

// Test 1: rc = 0 (module creation succeeds)
static bool test_init_returns_zero_and_passes_correct_args() {
    reset_mock_state();
    g_fake_rc = 0;                 // sqlite3_create_module should return 0
    char* errMsg = nullptr;

    // Call the focal function with null db and null pApi to exercise basic path.
    int rc = sqlite3_vtablog_init(nullptr, &errMsg, nullptr);
    
    // Expect rc to be exactly the value returned by sqlite3_create_module (0)
    if (rc != 0) {
        std::fprintf(stderr, "TEST FAILED: Expected rc == 0, got %d\n", rc);
        return false;
    }

    // Expect the module name to be "vtablog"
    if (g_last_name == nullptr || std::strcmp(g_last_name, "vtablog") != 0) {
        std::fprintf(stderr, "TEST FAILED: Expected module name 'vtablog', got '%s'\n",
                     g_last_name ? g_last_name : "<null>");
        return false;
    }

    // pzErrMsg should not be modified by sqlite3_vtablog_init (caller-supplied pointer remains unchanged)
    if (errMsg != nullptr) {
        std::fprintf(stderr, "TEST FAILED: Expected pzErrMsg to remain unchanged (nullptr), got non-null\n");
        return false;
    }

    // The mock should have captured at least the db/pClientData arguments (we pass null here, so verify not crashing)
    // No explicit check on db here since null is a valid test case, but we ensure no crash happened.

    return true;
}

// Test 2: rc = 1 (module creation fails) and verify propagation
static bool test_init_propagates_error_code() {
    reset_mock_state();
    g_fake_rc = 1;                 // sqlite3_create_module should return 1 (error)
    char* errMsg = nullptr;

    // Call the focal function again with a non-null db to simulate realistic usage.
    sqlite3 dummy_db;
    int rc = sqlite3_vtablog_init(&dummy_db, &errMsg, nullptr);

    // rc should propagate the value from sqlite3_create_module
    if (rc != 1) {
        std::fprintf(stderr, "TEST FAILED: Expected rc == 1, got %d\n", rc);
        return false;
    }

    // Module name should still be "vtablog"
    if (g_last_name == nullptr || std::strcmp(g_last_name, "vtablog") != 0) {
        std::fprintf(stderr, "TEST FAILED: Expected module name 'vtablog', got '%s'\n",
                     g_last_name ? g_last_name : "<null>");
        return false;
    }

    // Error message pointer should remain untouched
    if (errMsg != nullptr) {
        std::fprintf(stderr, "TEST FAILED: Expected pzErrMsg to remain unchanged (nullptr), got non-null\n");
        return false;
    }

    return true;
}

// Test 3: Verify that pApi pointer does not cause a crash or unexpected behavior (non-null pApi path)
static bool test_init_with_non_null_pApi_does_not_crash() {
    reset_mock_state();
    g_fake_rc = 0;
    char* errMsg = nullptr;

    // Create a dummy pApi object (contents are irrelevant for the mock)
    static int dummyApi;
    const sqlite3_api_routines* pApi = reinterpret_cast<const sqlite3_api_routines*>(&dummyApi);

    int rc = sqlite3_vtablog_init(nullptr, &errMsg, pApi);
    if (rc != 0) {
        std::fprintf(stderr, "TEST FAILED: Expected rc == 0 with non-null pApi, got %d\n", rc);
        return false;
    }

    // Ensure the module name still passed as "vtablog"
    if (g_last_name == nullptr || std::strcmp(g_last_name, "vtablog") != 0) {
        std::fprintf(stderr, "TEST FAILED: Expected module name 'vtablog' with non-null pApi, got '%s'\n",
                     g_last_name ? g_last_name : "<null>");
        return false;
    }

    return true;
}

// Entry point
int main() {
    // Run tests and report results
    bool all_ok = true;

    std::printf("Starting sqlite3_vtablog_init unit tests (C++11, no GTest)\n");

    if (!test_init_returns_zero_and_passes_correct_args()) {
        std::fprintf(stderr, "Error in test: test_init_returns_zero_and_passes_correct_args\n");
        all_ok = false;
    } else {
        std::printf("PASS: test_init_returns_zero_and_passes_correct_args\n");
    }

    if (!test_init_propagates_error_code()) {
        std::fprintf(stderr, "Error in test: test_init_propagates_error_code\n");
        all_ok = false;
    } else {
        std::printf("PASS: test_init_propagates_error_code\n");
    }

    if (!test_init_with_non_null_pApi_does_not_crash()) {
        std::fprintf(stderr, "Error in test: test_init_with_non_null_pApi_does_not_crash\n");
        all_ok = false;
    } else {
        std::printf("PASS: test_init_with_non_null_pApi_does_not_crash\n");
    }

    if (all_ok) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("ONE OR MORE TESTS FAILED\n");
        return 1;
    }
}