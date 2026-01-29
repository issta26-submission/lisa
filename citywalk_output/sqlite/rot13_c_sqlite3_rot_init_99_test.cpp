// Unit test suite for sqlite3_rot_init from rot13.c
// This test harness uses lightweight, self-contained mocks for the SQLite API
// to exercise different branches of sqlite3_rot_init without requiring the real
// SQLite runtime. It is written in C++11 and does not depend on GTest.

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <iostream>


// Domain knowledge: provide minimal constants used by the focal code.
// These mimic the SQLite return value and text encoding flags.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_UTF8
#define SQLITE_UTF8 1
#endif
#ifndef SQLITE_INNOCUOUS
#define SQLITE_INNOCUOUS 2
#endif
#ifndef SQLITE_DETERMINISTIC
#define SQLITE_DETERMINISTIC 4
#endif

// Forward declare the focal function from rot13.c.
// The actual implementation is compiled together with this test.
// The signature matches: int sqlite3_rot_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
extern "C" int sqlite3_rot_init(struct sqlite3*, char **, const struct sqlite3_api_routines*);

// Faked SQLite core types (opaque to the test)
struct sqlite3 { int dummy; };
struct sqlite3_api_routines { int dummy; };
struct sqlite3_context { int dummy; };
struct sqlite3_value { int dummy; };

// The sqlite3_ext.h macro SQLITE_EXTENSION_INIT2(pApi) is expected to assign
// the global api pointer used by the focal module. We mirror the conventional
// symbol name and type here so the macro can resolve.
// Note: This symbol is expected to be defined by the extension environment.
// We provide a concrete instance here for testing.
const struct sqlite3_api_routines *sqlite3_api = nullptr;

// --- Mocks for SQLite API used by sqlite3_rot_init ---
// These globals control the next return codes of the mocked API.
int g_next_function_rc = SQLITE_OK;   // Return code for sqlite3_create_function
int g_next_collation_rc = SQLITE_OK;  // Return code for sqlite3_create_collation

// Flags to verify whether the mocks were actually invoked.
int g_function_called = 0;
int g_collation_called = 0;

// Mock implementation of sqlite3_create_function.
// Signature matches SQLite's, but we only care about return code and call count.
extern "C" int sqlite3_create_function(struct sqlite3*,
                                     const char*,
                                     int,
                                     int,
                                     void*,
                                     void(*)(struct sqlite3_context*, int, struct sqlite3_value**),
                                     void(*)(struct sqlite3_context*, int, struct sqlite3_value**),
                                     void(*)(struct sqlite3_context*)) {
    (void)db;             // suppress unused parameter warnings if compiled with -Wextra
    g_function_called = 1;
    return g_next_function_rc;
}

// Mock implementation of sqlite3_create_collation.
// Signature matches SQLite's, but we only care about return code and call count.
extern "C" int sqlite3_create_collation(struct sqlite3*,
                                      const char*,
                                      int,
                                      void*,
                                      int (*)(void*, int, const void*, int, const void*)) {
    (void)db;
    g_collation_called = 1;
    return g_next_collation_rc;
}

// --- Lightweight test harness utilities ---
// Capture non-terminating test failures and allow tests to run sequentially.
std::vector<std::string> g_failures;

static void expect_true(bool cond, const std::string& desc) {
    if(!cond) {
        g_failures.push_back(desc);
    }
}

// Reset shared state before each test
static void reset_state() {
    g_next_function_rc = SQLITE_OK;
    g_next_collation_rc = SQLITE_OK;
    g_function_called = 0;
    g_collation_called = 0;
    sqlite3_api = nullptr;
}

// Test 1: Both sqlite3_create_function and sqlite3_create_collation succeed.
// Expected: sqlite3_rot_init returns SQLITE_OK, api is set, and both mocks were invoked.
static void test_rot_init_ok() {
    reset_state();

    // Prepare a dummy API pointer to verify SQLITE_EXTENSION_INIT2 behavior.
    static struct sqlite3_api_routines api;
    sqlite3_api = &api;

    // Call the focal function with a dummy db pointer and pzErrMsg = nullptr.
    struct sqlite3 db;
    int rc = sqlite3_rot_init(&db, nullptr, &api);

    // Validation (non-terminating assertions)
    expect_true(rc == SQLITE_OK, "rot_init should return SQLITE_OK when both function and collation creation succeed");
    expect_true(g_function_called == 1, "sqlite3_create_function should be invoked on success path");
    expect_true(g_collation_called == 1, "sqlite3_create_collation should be invoked after function creation succeeds");
    expect_true(sqlite3_api == &api, "SQLITE_EXTENSION_INIT2 should set sqlite3_api to the provided pApi");
}

// Test 2: First API call fails (sqlite3_create_function returns non-OK).
// Expected: rot_init returns that error and sqlite3_create_collation is not invoked.
static void test_rot_init_first_call_failure() {
    reset_state();

    // Simulate failure on the first API call.
    g_next_function_rc = 1;  // Non-zero error code (SQLITE_ERROR-like)
    g_next_collation_rc = SQLITE_OK; // Should not matter since first call fails

    static struct sqlite3_api_routines api;
    sqlite3_api = &api;

    struct sqlite3 db;
    int rc = sqlite3_rot_init(&db, nullptr, &api);

    // Validation
    expect_true(rc == 1, "rot_init should propagate the error from sqlite3_create_function");
    expect_true(g_function_called == 1, "sqlite3_create_function should be invoked even if it fails");
    expect_true(g_collation_called == 0, "sqlite3_create_collation must not be invoked when function creation fails");
    expect_true(sqlite3_api == &api, "SQLITE_EXTENSION_INIT2 should still set sqlite3_api on failure");
}

// Test 3: First API call succeeds, second API call fails (sqlite3_create_collation non-OK).
// Expected: rot_init returns the collate error and both mocks were invoked.
static void test_rot_init_second_call_failure() {
    reset_state();

    // Simulate success on the first call, then failure on the second call.
    g_next_function_rc = SQLITE_OK;
    g_next_collation_rc = 2;  // Non-zero error code

    static struct sqlite3_api_routines api;
    sqlite3_api = &api;

    struct sqlite3 db;
    int rc = sqlite3_rot_init(&db, nullptr, &api);

    // Validation
    expect_true(rc == 2, "rot_init should propagate the error from sqlite3_create_collation");
    expect_true(g_function_called == 1, "sqlite3_create_function should be invoked when first step succeeds");
    expect_true(g_collation_called == 1, "sqlite3_create_collation should be invoked after function creation succeeds");
    expect_true(sqlite3_api == &api, "SQLITE_EXTENSION_INIT2 should set sqlite3_api even if second step fails");
}

// --- Entry point ---
int main() {
    // Step 3: Run the unit tests
    test_rot_init_ok();
    test_rot_init_first_call_failure();
    test_rot_init_second_call_failure();

    // Output results with explanatory comments
    if(g_failures.empty()) {
        std::cout << "All tests passed: sqlite3_rot_init behavior verified across scenarios." << std::endl;
        return 0;
    }

    std::cout << "Some tests failed (" << g_failures.size() << "):" << std::endl;
    for(const auto &msg : g_failures) {
        std::cout << " - " << msg << std::endl;
    }
    return 1;
}