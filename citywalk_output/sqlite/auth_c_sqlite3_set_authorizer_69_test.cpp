// Unit test suite for sqlite3_set_authorizer (auth.c)
// This test suite is written in C++11 without Google Test.
// It focuses on behavior exposed by sqlite3_set_authorizer and interacts with a live in-memory SQLite database.
// Explanatory comments are added before each test to describe intent and coverage.

#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Simple lightweight test harness (non-terminating assertions to maximize code coverage)
static int g_test_failures = 0;
static void test_assert(bool condition, const char* message) {
    if (condition) {
        std::cout << "[PASS] " << message << std::endl;
    } else {
        std::cerr << "[FAIL] " << message << std::endl;
        ++g_test_failures;
    }
}

// Domain-specific state used by the authorizer callback to verify propagation of pArg and invocation
struct AuthState {
    int calls;
    const char* last_tab;
    bool argOk; // whether the pArg passed into sqlite3_set_authorizer is propagated to the callback
};

// Global test state instance for use within the C-style callback
static AuthState g_authState;

// C linkage for the authorizer callback so it can be passed to sqlite3_set_authorizer
extern "C" int auth_callback(void* arg, int code, const char* zTab, const char* zCol, const char* zDb, const char* zAuthArg) {
    AuthState* s = static_cast<AuthState*>(arg);
    if (s) {
        s->calls += 1;
        s->last_tab = zTab;
        // The callback receives the same pArg pointer provided to sqlite3_set_authorizer
        s->argOk = (arg == &g_authState);
    }
    // Always allow; this test focuses on invocation and argument passing, not authorization results
    return SQLITE_OK;
}

// Domain knowledge notes:
// - We test: (a) API_ARMOR safety path when db is NULL; (b) that non-null xAuth triggers the authorizer callback during statement execution,
//   and (c) that pArg is correctly passed to the callback.
// - We use an in-memory database to avoid filesystem dependencies.
// - We avoid private/internal SQLite APIs in tests and rely on public API surface (sqlite3_open, sqlite3_exec, sqlite3_set_authorizer, etc.).

// Test 1: API Armor safety path (when a NULL database is supplied). This exercises the early return path.
// This test is guarded by SQLITE_ENABLE_API_ARMOR macro to ensure it is compiled with API armor support.
// If the macro is not defined in the SQLite build, this test is skipped to avoid undefined behavior.
#ifdef SQLITE_ENABLE_API_ARMOR
void test_sqlite3_set_authorizer_api_armor_null_db() {
    // When API armor is enabled, a NULL db should lead to a non-SQLITE_OK result
    int rc = sqlite3_set_authorizer(nullptr, nullptr, nullptr);
    test_assert(rc != SQLITE_OK, "sqlite3_set_authorizer should not return SQLITE_OK for NULL db under API armor");
}
#endif

// Test 2: Authorizer callback invocation and pArg propagation.
// Steps:
//  - Open an in-memory DB and create a simple table.
//  - Install a non-null authorizer callback with a pointer to a test state.
//  - Execute a SELECT statement to trigger the authorizer.
//  - Verify that the callback was invoked, the table name seen by the callback is as expected, and that the pArg was propagated.
void test_sqlite3_set_authorizer_callback_invocation_and_parg_propagation() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    test_assert(rc == SQLITE_OK && db != nullptr, "Open in-memory SQLite DB for authorizer test");
    if (!db) return;

    // Prepare test state
    g_authState.calls = 0;
    g_authState.last_tab = nullptr;
    g_authState.argOk = false;

    // Create a simple table to generate an authorizer callback on SELECT
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, "CREATE TABLE t(x INTEGER);", nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQLite error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    test_assert(rc == SQLITE_OK, "Create table t for authorizer callback test");

    // Install authorizer with non-null xAuth and a pointer to g_authState
    rc = sqlite3_set_authorizer(db, auth_callback, &g_authState);
    test_assert(rc == SQLITE_OK, "sqlite3_set_authorizer should succeed with non-null callback");

    // Execute a statement that will trigger the authorizer for table t
    // We fetch a value to ensure statement execution actually happens
    rc = sqlite3_exec(db, "SELECT x FROM t;", nullptr, nullptr, &errMsg);
    // If there is an error, report it (though authorizer callback should still be invoked before failure)
    if (errMsg) {
        std::cerr << "SQLite error during SELECT: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // Validate that the authorizer callback was invoked and saw the expected table
    test_assert(g_authState.calls > 0, "Authorizer callback should be invoked on SELECT");
    test_assert(g_authState.last_tab != nullptr, "Authorizer callback should receive a non-null tab name");
    if (g_authState.last_tab) {
        test_assert(std::strcmp(g_authState.last_tab, "t") == 0, "Authorizer callback should be invoked for table 't'");
    }

    // And verify that the pArg passed to the callback is the same pointer we provided
    test_assert(g_authState.argOk, "Authorizer callback should receive the correct pArg (passed AuthState address)");

    // Now remove the authorizer by setting NULL callback and NULL arg, and ensure subsequent calls do not trigger the callback
    rc = sqlite3_set_authorizer(db, nullptr, nullptr);
    test_assert(rc == SQLITE_OK, "sqlite3_set_authorizer should succeed when removing callback");

    // Reset count and run another query; the callback should not be invoked now
    int calls_before = g_authState.calls;
    rc = sqlite3_exec(db, "SELECT x FROM t;", nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQLite error during SELECT (after removing authorizer): " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    test_assert(g_authState.calls == calls_before, "Authorizer callback should not be invoked after removing xAuth");
    
    sqlite3_close(db);
}

// Entry point to run all tests
int main() {
    std::cout << "Starting sqlite3_set_authorizer test suite (C++11, no GTest)" << std::endl;

#ifdef SQLITE_ENABLE_API_ARMOR
    test_sqlite3_set_authorizer_api_armor_null_db();
#else
    std::cout << "[SKIP] API Armor test is skipped because SQLITE_ENABLE_API_ARMOR is not defined in this build." << std::endl;
#endif

    test_sqlite3_set_authorizer_callback_invocation_and_parg_propagation();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}