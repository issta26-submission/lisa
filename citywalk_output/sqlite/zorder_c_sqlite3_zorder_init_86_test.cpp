// Test suite for sqlite3_zorder_init in zorder.c
// NOTE: This test harness provides C-compatible mocks for sqlite3_create_function
// and related SQLite types to exercise the focal function without requiring
// the real SQLite runtime. It is designed to be compiled with a C++11-capable
// compiler, and linked against zorder.c (the focal implementation).

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>


// Forward declare minimal SQLite-like types to allow compilation/linking with zorder.c
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Minimal macro constants used by the focal function.
// These mirror common SQLite definitions, kept small for the test harness.
static const int SQLITE_OK = 0;
static const int SQLITE_UTF8 = 1;

// Forward declaration of the focal function under test.
// It will be linked with the real implementation from zorder.c.
extern "C" int sqlite3_zorder_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Global state used by the mock to simulate different return paths.
static std::vector<std::string> g_calls;        // Records names of registered functions
static int g_first_return = SQLITE_OK;          // Return value for the first sqlite3_create_function call
static int g_second_return = SQLITE_OK;         // Return value for the second sqlite3_create_function call

// Mock implementation of sqlite3_create_function with C linkage.
// This intercepts calls from sqlite3_zorder_init and simulates behavior.
extern "C" int sqlite3_create_function(
    sqlite3 *db,
    const char *zFunctionName,
    int nArg,
    int eTextRep,
    void *pApp,
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void (*xStep)(sqlite3_context*, int, sqlite3_value**),
    void (*xFinal)(sqlite3_context*, int, sqlite3_value**)
) {
    // Record which function name was attempted to be created for later verification.
    if (zFunctionName) {
        g_calls.push_back(std::string(zFunctionName));
    } else {
        g_calls.push_back(std::string("<null>"));
    }

    // Simulate behavior based on function name and configured return codes.
    if (zFunctionName && std::strcmp(zFunctionName, "zorder") == 0) {
        return g_first_return;
    }
    if (zFunctionName && std::strcmp(zFunctionName, "unzorder") == 0) {
        return g_second_return;
    }

    // Default: success
    return SQLITE_OK;
}

// Helper to reset test state before each test.
static void reset_state() {
    g_calls.clear();
    g_first_return = SQLITE_OK;
    g_second_return = SQLITE_OK;
}

// -----------------------------------------------------------------------------
// Test 1: Both registrations succeed
// Expectation: sqlite3_zorder_init returns SQLITE_OK and both functions are registered
// -----------------------------------------------------------------------------
static bool test_init_both_functions_ok() {
    reset_state();
    g_first_return = SQLITE_OK;
    g_second_return = SQLITE_OK;

    struct sqlite3 dummy_db;
    char* errMsg = nullptr;
    sqlite3_api_routines api;  // Dummy API struct; not used by the mock.

    int rc = sqlite3_zorder_init(&dummy_db, &errMsg, &api);

    bool ok = (rc == SQLITE_OK) && (g_calls.size() == 2);
    if (!ok) {
        std::cerr << "[Test1] FAILED: rc=" << rc
                  << ", calls=" << g_calls.size()
                  << ", expected 2 calls and SQLITE_OK" << std::endl;
        if (rc != SQLITE_OK) {
            std::cerr << "  Reason: rc != SQLITE_OK" << std::endl;
        }
        if (g_calls.size() != 2) {
            std::cerr << "  Reason: expected 2 registrations (zorder, unzorder)" << std::endl;
        }
    } else {
        // Additional verification: ensure the order and names are as expected
        if (g_calls[0] != "zorder" || g_calls[1] != "unzorder") {
            std::cerr << "[Test1] FAILED: registration order mismatch. "
                      << "got: [" << g_calls[0] << ", " << g_calls[1] << "]" << std::endl;
            ok = false;
        } else {
            std::cout << "[Test1] PASSED: both functions registered in correct order." << std::endl;
        }
    }
    return ok;
}

// -----------------------------------------------------------------------------
// Test 2: First registration fails -> second should not be attempted
// Expectation: sqlite3_zorder_init returns non-SQLITE_OK and only one call was made
// -----------------------------------------------------------------------------
static bool test_init_first_registration_fails() {
    reset_state();
    g_first_return = 1;        // Non-zero simulate error
    g_second_return = SQLITE_OK; // Should not be invoked

    struct sqlite3 dummy_db;
    char* errMsg = nullptr;
    sqlite3_api_routines api;

    int rc = sqlite3_zorder_init(&dummy_db, &errMsg, &api);

    bool ok = (rc != SQLITE_OK) && (g_calls.size() == 1);
    if (!ok) {
        std::cerr << "[Test2] FAILED: rc=" << rc
                  << ", calls=" << g_calls.size()
                  << ", expected 1 call and error rc" << std::endl;
        if (rc == SQLITE_OK) {
            std::cerr << "  Reason: expected non-zero rc" << std::endl;
        }
        if (g_calls.size() != 1) {
            std::cerr << "  Reason: second function should not be registered" << std::endl;
        }
    } else {
        std::cout << "[Test2] PASSED: first registration failed and second not invoked." << std::endl;
    }
    return ok;
}

// -----------------------------------------------------------------------------
// Test 3: First registration OK, second registration fails
// Expectation: sqlite3_zorder_init returns non-SQLITE_OK and two calls were made
// -----------------------------------------------------------------------------
static bool test_init_second_registration_fails() {
    reset_state();
    g_first_return = SQLITE_OK;
    g_second_return = 1;  // Non-zero simulate error

    struct sqlite3 dummy_db;
    char* errMsg = nullptr;
    sqlite3_api_routines api;

    int rc = sqlite3_zorder_init(&dummy_db, &errMsg, &api);

    bool ok = (rc != SQLITE_OK) && (g_calls.size() == 2);
    if (!ok) {
        std::cerr << "[Test3] FAILED: rc=" << rc
                  << ", calls=" << g_calls.size()
                  << ", expected 2 calls and non-zero rc" << std::endl;
        if (rc == SQLITE_OK) {
            std::cerr << "  Reason: expected non-zero rc" << std::endl;
        }
        if (g_calls.size() != 2) {
            std::cerr << "  Reason: both registrations should have been attempted" << std::endl;
        }
    } else {
        std::cout << "[Test3] PASSED: second registration failed after first succeeded." << std::endl;
    }
    return ok;
}

// -----------------------------------------------------------------------------
// Main: run all tests from a single entry point
// This follows the guideline to "call test methods from the main function" and
// uses simple non-terminating assertions (console output) to maximize coverage.
// -----------------------------------------------------------------------------
int main() {
    int total = 0;
    int failed = 0;

    std::cout << "Running tests for sqlite3_zorder_init..." << std::endl;

    if (test_init_both_functions_ok()) {
        ++total;
    } else {
        ++total; ++failed;
    }

    if (test_init_first_registration_fails()) {
        ++total;
    } else {
        ++total; ++failed;
    }

    if (test_init_second_registration_fails()) {
        ++total;
    } else {
        ++total; ++failed;
    }

    int passed = total - failed;
    std::cout << "Test summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Non-terminating: return 0 if all passed, non-zero otherwise
    return failed ? 1 : 0;
}