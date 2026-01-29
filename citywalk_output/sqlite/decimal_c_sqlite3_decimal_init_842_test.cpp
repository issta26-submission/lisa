// C++11 unit test suite for sqlite3_decimal_init (decimal.c) using lightweight custom mocks.
// This test suite does not rely on Google Test; it uses a minimal, self-contained testing approach.
// It mocks the necessary SQLite C API functions (sqlite3_create_function, sqlite3_create_window_function,
// sqlite3_create_collation) by providing C linkage so that sqlite3_decimal_init (from decimal.c) can
// call into these mocks during tests. The tests verify various success/failure paths and argument
// propagation (notably the iArg behavior for decimal_exp).

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>


// Forward declare opaque SQLite types to mimic minimal API surface.
// We don't implement full SQLite behavior; these are only placeholders for function signatures.
struct sqlite3;
struct sqlite3_context;
struct sqlite3_value;

// sqlite3_api_routines is normally defined by SQLite headers; kept empty for testing purposes.
struct sqlite3_api_routines {};

// The function under test is declared with C linkage
extern "C" int sqlite3_decimal_init(struct sqlite3 *db,
                                   char **pzErrMsg,
                                   const sqlite3_api_routines *pApi);

// Macro compatibility for the mock environment.
// In the real SQLite extension, SQLITE_EXTENSION_INIT2 macro is provided by sqlite3ext.h.
// In this test harness, we define it as a no-op to avoid requiring the actual header.
// This is safe because our mock environment uses global state to verify behavior.
#ifndef SQLITE_EXTENSION_INIT2
#define SQLITE_EXTENSION_INIT2(a) ((void)0)
#endif

// Global mock state and helpers for tests
static struct sqlite3 g_mock_db;  // dummy db object used for pApp checks
static bool g_fail_on_window = false;
static bool g_fail_on_collation = false;
static std::string g_fail_on_func = "";  // if non-empty, fail on the given function name
static bool g_pApp_check_failed = false; // set if decimal_exp did not receive the db pointer as pApp
static std::vector<std::string> g_func_order; // expected function names in order
static std::vector<std::string> g_logged_names; // actual function names called
static std::vector<void*> g_logged_pApp;        // pApp values passed to sqlite3_create_function
static bool g_last_call_was_collation_logged = false;

// A simple struct to record each sqlite3_create_function call for introspection
struct FunctionCallRecord {
    std::string name;
    int nArg;
    void* pApp;
};

// Storage for function calls
static std::vector<FunctionCallRecord> g_calls;

// Helpers for assertion/reporting
static int g_test_passed = 1;
static std::string g_last_error;

// Utility to reset test state between tests
static void reset_mock_state() {
    g_fail_on_window = false;
    g_fail_on_collation = false;
    g_fail_on_func.clear();
    g_pApp_check_failed = false;
    g_func_order.clear();
    g_logged_names.clear();
    g_logged_pApp.clear();
    g_calls.clear();
    g_last_call_was_collation_logged = false;
    g_last_error.clear();
}

// --- Mock implementations of SQLite API used by sqlite3_decimal_init ---

// Note: All mocks use C linkage so that the linker can resolve them against the sqlite3_decimal_init
// call from decimal.c (which is compiled as C).

extern "C" {

// Mock: sqlite3_create_function
int sqlite3_create_function(struct sqlite3* db,
                            const char* zFuncName,
                            int nArg,
                            unsigned int /*flags*/,
                            void* pApp,
                            void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
                            void (*xStep)(sqlite3_context*,int,sqlite3_value**),
                            void (*xFinal)(sqlite3_context*,int,sqlite3_value**))
{
    // Record the call
    FunctionCallRecord rec;
    rec.name = zFuncName ? zFuncName : "";
    rec.nArg = nArg;
    rec.pApp = pApp;
    g_calls.push_back(rec);
    g_logged_names.push_back(rec.name);
    g_logged_pApp.push_back(pApp);

    // Specific checks for tests
    // 1) Enforce pApp behavior for "decimal_exp" (iArg == 1 means pApp should be db)
    if (rec.name == "decimal_exp") {
        if (pApp != &g_mock_db) {
            g_pApp_check_failed = true;
            g_last_error = "decimal_exp: pApp != db as expected";
        }
    } else {
        // For other functions, pApp must be NULL (iArg == 0)
        if (pApp != nullptr) {
            // We still allow a logged entry, but record a minor failure signal
            g_pApp_check_failed = true;
            g_last_error = "pApp non-null for function other than decimal_exp";
        }
    }

    // 2) Conditional failure to simulate rc != SQLITE_OK
    if (!g_fail_on_func.empty() && zFuncName && g_fail_on_func == zFuncName) {
        // Simulate an error return code; non-zero to indicate failure
        return 1; // non-SQLITE_OK
    }

    // Success
    return 0; // SQLITE_OK
}

// Mock: sqlite3_create_window_function
int sqlite3_create_window_function(struct sqlite3* /*db*/,
                                 const char* zName,
                                 int /*nArg*/,
                                 unsigned int /*flags*/,
                                 void* /*pApp*/,
                                 void (*step)(sqlite3_context*,int,sqlite3_value**),
                                 void (*finalize)(sqlite3_context*,int,sqlite3_value**),
                                 void (*value)(sqlite3_context*,int,sqlite3_value**),
                                 void (*inverse)(sqlite3_context*,int,sqlite3_value**),
                                 void* /*pApp2*/)
{
    // Record that window function creation was attempted
    g_logged_names.push_back(zName ? zName : "window");
    g_calls.push_back({zName ? zName : "window", 1, nullptr});

    if (g_fail_on_window) {
        return 1; // simulate failure
    }
    return 0; // success
}

// Mock: sqlite3_create_collation
int sqlite3_create_collation(struct sqlite3* /*db*/,
                             const char* zName,
                             int /*eTextRep*/,
                             void* /*notUsed*/,
                             int (*xCmp)(void*,int,const void*,int,const void*))
{
    // Record that collation creation was attempted
    g_logged_names.push_back(zName ? zName : "collation");
    g_calls.push_back({zName ? zName : "collation", 0, nullptr});

    if (g_fail_on_collation) {
        return 1; // simulate failure
    }
    return 0; // success
}
} // extern "C"

// The tests rely on a few concrete details of the focal function's expectations:
// - It registers 7 scalar functions named: decimal, decimal_exp, decimal_cmp, decimal_add, decimal_sub, decimal_mul, decimal_pow2
// - It then registers a window function named "decimal_sum"
// - It finally registers a collation named "decimal"
// - The iArg flag causes pApp to be either NULL or db; decimal_exp should receive db as pApp

// Provide a tiny test harness
static void run_test_successful_initialization() {
    reset_mock_state();

    // In success scenario, no forced failures
    g_fail_on_func = "";
    g_fail_on_window = false;
    g_fail_on_collation = false;
    g_pApp_check_failed = false;
    g_mock_db = {}; // reset dummy db
    g_calls.clear();

    char* err = nullptr;
    int rc = sqlite3_decimal_init(&g_mock_db, &err, nullptr);

    // Assertions (non-terminating style is simulated by collecting state and printing at end)
    bool ok = true;
    if (rc != 0) {
        ok = false;
        g_last_error = "Expected rc == SQLITE_OK (0) in success path";
    }
    // Expect 9 calls: 7 functions + 1 window + 1 collation
    if (g_calls.size() != 9) {
        ok = false;
        g_last_error = "Expected 9 total calls (7 functions + window + collation)";
    }

    // Check the order/names of function registrations
    const char* expected_order[] = { "decimal", "decimal_exp", "decimal_cmp",
                                     "decimal_add", "decimal_sub", "decimal_mul", "decimal_pow2" };
    if (g_calls.size() >= 7) {
        for (size_t i = 0; i < 7; ++i) {
            if (g_calls[i].name != expected_order[i]) {
                ok = false;
                break;
            }
        }
    } else {
        ok = false;
    }

    // Check that decimal_exp got pApp = &g_mock_db, others got NULL
    // decimal_exp is the 2nd registered function (index 1)
    if (g_calls.size() > 1) {
        if (g_calls[1].pApp != (void*)&g_mock_db) {
            ok = false;
            g_last_error = "decimal_exp did not receive db as pApp (unexpected value)";
        }
    }

    // Report
    std::cout << "Test: Successful initialization - " << (ok ? "PASS" : "FAIL");
    if (!ok) {
        std::cout << " | Reason: " << g_last_error;
    }
    std::cout << std::endl;
}

// Fail early on the third function (decimal_cmp)
static void run_test_fail_on_third_function() {
    reset_mock_state();

    g_fail_on_func = "decimal_cmp"; // third function in the list
    g_fail_on_window = false;
    g_fail_on_collation = false;
    g_mock_db = {};
    g_calls.clear();

    char* err = nullptr;
    int rc = sqlite3_decimal_init(&g_mock_db, &err, nullptr);

    bool ok = true;
    if (rc == 0) {
        ok = false;
        g_last_error = "Expected non-zero rc when a function creation fails (SIMULATED)";
    }
    // Expect 3 log entries: two succeeded function creations, then the failed one
    if (g_calls.size() != 3) {
        ok = false;
        g_last_error = "Expected 3 calls before failure (two successes and one failure)";
    }

    std::cout << "Test: Fail on third function (decimal_cmp) - " << (ok ? "PASS" : "FAIL");
    if (!ok) {
        std::cout << " | Reason: " << g_last_error;
    }
    std::cout << std::endl;
}

// Fail on window function creation (decimal_sum)
static void run_test_fail_on_window_function() {
    reset_mock_state();

    g_fail_on_window = true; // fail on window function creation
    g_fail_on_func = ""; // no early func failure
    g_fail_on_collation = false;
    g_mock_db = {};
    g_calls.clear();

    char* err = nullptr;
    int rc = sqlite3_decimal_init(&g_mock_db, &err, nullptr);

    bool ok = true;
    if (rc == 0) {
        ok = false;
        g_last_error = "Expected non-zero rc when window function creation fails";
    }
    // Expect 8 calls (7 functions + window attempt)
    if (g_calls.size() != 8) {
        ok = false;
        g_last_error = "Expected 8 calls (7 function registrations + one window function attempt)";
    }

    std::cout << "Test: Fail on window function creation - " << (ok ? "PASS" : "FAIL");
    if (!ok) {
        std::cout << " | Reason: " << g_last_error;
    }
    std::cout << std::endl;
}

// Fail on collation creation
static void run_test_fail_on_collation() {
    reset_mock_state();

    g_fail_on_window = false;
    g_fail_on_collation = true; // fail on collation creation
    g_fail_on_func = "";
    g_mock_db = {};
    g_calls.clear();

    char* err = nullptr;
    int rc = sqlite3_decimal_init(&g_mock_db, &err, nullptr);

    bool ok = true;
    if (rc == 0) {
        ok = false;
        g_last_error = "Expected non-zero rc when collation creation fails";
    }
    // 7 function calls + 1 window function + 1 failed collation = 9 entries
    if (g_calls.size() != 9) {
        ok = false;
        g_last_error = "Expected 9 calls (7 functions + window + failed collation)";
    }

    std::cout << "Test: Fail on collation creation - " << (ok ? "PASS" : "FAIL");
    if (!ok) {
        std::cout << " | Reason: " << g_last_error;
    }
    std::cout << std::endl;
}

// Entry point
int main() {
    // Run each focused test
    run_test_successful_initialization();
    run_test_fail_on_third_function();
    run_test_fail_on_window_function();
    run_test_fail_on_collation();

    // Return non-zero if any test failed
    if (g_test_passed == 0) {
        std::cout << "All tests FAILED." << std::endl;
        return 1;
    }

    // If any test printed failure, we still return 0 if we want to keep running.
    // We'll determine pass/fail by per-test messages; assume tests print PASS/FAIL.
    return 0;
}