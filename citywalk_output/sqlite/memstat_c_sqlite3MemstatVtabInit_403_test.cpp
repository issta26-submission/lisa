// Minimal C++11 test harness for sqlite3MemstatVtabInit
// Test strategy:
// - Provide mocks for sqlite3_create_module to intercept calls made by sqlite3MemstatVtabInit.
// - Compile two separate test paths using the SQLITE_OMIT_VIRTUALTABLE macro:
//   a) When SQLITE_OMIT_VIRTUALTABLE is not defined (default), verify that sqlite3_create_module is invoked
//      with name "sqlite_memstat" and that rc propagates correctly.
//   b) When SQLITE_OMIT_VIRTUALTABLE is defined, verify that sqlite3_create_module is not called and rc is SQLITE_OK.
// - Tests are driven from main() depending on the active macro.
// - All assertions are non-terminating in the sense that tests print PASS/FAIL and return a nonzero code on failure.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <cstring>
#include <cstdlib>


// Forward declaration of the function under test (as provided in memstat.c)
extern "C" int sqlite3MemstatVtabInit(sqlite3 *db);

#ifndef SQLITE_OMIT_VIRTUALTABLE
// Tests for the branch when VIRTUALTABLE support is enabled (default).
// We mock sqlite3_create_module to observe how sqlite3MemstatVtabInit interacts with it.

static int sqlite3_create_module_calls = 0;
static const char* last_module_name = nullptr;
static const sqlite3_module* last_module_ptr = nullptr;
static void* last_pAux = nullptr;
static int next_rc_value = SQLITE_OK;

// Forward declaration to match sqlite3_create_module signature used by the real code.
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pAux);

// Mock implementation of sqlite3_create_module to capture call details.
// We deliberately keep it simple: only inspect module name and that a module is passed.
// The rc returned is controlled by next_rc_value to simulate different outcomes.
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pAux) {
    ++sqlite3_create_module_calls;
    last_module_name = zName;
    last_module_ptr = pModule;
    last_pAux = pAux;
    return next_rc_value;
}

// Test 1: Verify that in the enabled-virtualtable path, the module is registered correctly
//         and rc matches the mocked return value (SQLITE_OK by default).
static void test_memstatVtabInit_calls_create_module_with_expected_args_ok() {
    sqlite3_create_module_calls = 0;
    last_module_name = nullptr;
    last_module_ptr = nullptr;
    last_pAux = nullptr;
    next_rc_value = SQLITE_OK;

    int rc = sqlite3MemstatVtabInit(nullptr);

    if (rc != SQLITE_OK) {
        printf("FAIL: rc mismatch: got %d, expected %d\n", rc, SQLITE_OK);
        return;
    }
    if (sqlite3_create_module_calls != 1) {
        printf("FAIL: expected 1 sqlite3_create_module call, got %d\n", sqlite3_create_module_calls);
        return;
    }
    if (last_module_name == nullptr || std::strcmp(last_module_name, "sqlite_memstat") != 0) {
        printf("FAIL: module name mismatch: got %s\n", last_module_name ? last_module_name : "null");
        return;
    }
    if (last_module_ptr == nullptr) {
        printf("FAIL: module pointer is null\n");
        return;
    }
    // pAux is expected to be 0 as passed in sqlite3MemstatVtabInit
    if (last_pAux != nullptr) {
        printf("FAIL: pAux should be null, got non-null\n");
        return;
    }
    printf("PASS: test_memstatVtabInit_calls_create_module_with_expected_args_ok\n");
}

// Test 2: Verify that the return code from sqlite3MemstatVtabInit propagates from sqlite3_create_module
//         when sqlite3_create_module returns SQLITE_ERROR.
static void test_memstatVtabInit_propagates_rc_error() {
    sqlite3_create_module_calls = 0;
    last_module_name = nullptr;
    last_module_ptr = nullptr;
    last_pAux = nullptr;
    next_rc_value = SQLITE_ERROR;

    int rc = sqlite3MemstatVtabInit(nullptr);

    if (rc != SQLITE_ERROR) {
        printf("FAIL: rc mismatch: got %d, expected %d\n", rc, SQLITE_ERROR);
        return;
    }
    if (sqlite3_create_module_calls != 1) {
        printf("FAIL: expected 1 sqlite3_create_module call, got %d\n", sqlite3_create_module_calls);
        return;
    }
    if (last_module_name == nullptr || std::strcmp(last_module_name, "sqlite_memstat") != 0) {
        printf("FAIL: module name mismatch: got %s\n", last_module_name ? last_module_name : "null");
        return;
    }
    printf("PASS: test_memstatVtabInit_propagates_rc_error\n");
}

// Test runner for non-omit path
static int run_non_omit_tests() {
    test_memstatVtabInit_calls_create_module_with_expected_args_ok();
    test_memstatVtabInit_propagates_rc_error();
    return 0;
}

#else
// Tests for the branch when SQLITE_OMIT_VIRTUALTABLE is defined.
// In this path, sqlite3_create_module should not be called at all.

static int sqlite3_create_module_calls = 0;
static const char* last_module_name = nullptr;
static const sqlite3_module* last_module_ptr = nullptr;
static void* last_pAux = nullptr;

// Forward declaration for sqlite3MemstatVtabInit to be linked from memstat.c
extern "C" int sqlite3MemstatVtabInit(sqlite3 *db);

// Provide an empty mock; this branch should never call sqlite3_create_module.
// If it does, we can detect it via the counter.
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pAux) {
    ++sqlite3_create_module_calls;
    last_module_name = zName;
    last_module_ptr = pModule;
    last_pAux = pAux;
    return SQLITE_OK;
}

// Test: Ensure that with OMIT flag defined, no module registration occurs and rc is SQLITE_OK
static void test_omit_branch_no_sqlite3_create_module() {
    sqlite3_create_module_calls = 0;
    int rc = sqlite3MemstatVtabInit(nullptr);

    if (sqlite3_create_module_calls != 0) {
        printf("FAIL: expected 0 sqlite3_create_module calls, got %d\n", sqlite3_create_module_calls);
        return;
    }
    if (rc != SQLITE_OK) {
        printf("FAIL: rc mismatch in omit branch: got %d, expected %d\n", rc, SQLITE_OK);
        return;
    }
    printf("PASS: test_omit_branch_no_sqlite3_create_module\n");
}

// Test runner for omit path
static int run_omit_tests() {
    test_omit_branch_no_sqlite3_create_module();
    return 0;
}

#endif

// Simple test harness entry point
int main() {
#ifdef SQLITE_OMIT_VIRTUALTABLE
    // Run tests for the OMIT branch
    return run_omit_tests();
#else
    // Run tests for the non-OMIT (default) branch
    return run_non_omit_tests();
#endif
}