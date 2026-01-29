// File: mocks/sqlite_mock.h
// Minimal mock definitions to enable compiling tests for sqlite3_fuzzer_init
// without requiring the real SQLite library. This header is intended to be
// included by the test translation units that link against fuzzer.c.

#ifndef SQLITE_MOCK_H
#define SQLITE_MOCK_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <mocks/sqlite_mock.h>
#include <cstddef>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>


extern "C" {

// Forward declarations of SQLite core types as opaque structs to keep the
// test environment lightweight. The real layout is not needed for our tests.
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_api_routines sqlite3_api_routines;
typedef struct sqlite3_module sqlite3_module;

// SQLite result code used by the function under test.
enum {
    SQLITE_OK = 0
};

// Prototype of the function that the focal code calls to register a module.
// We provide this so tests can intercept module registration.
int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pClientData);

} // extern "C"

#endif // SQLITE_MOCK_H

// File: tests/test_fuzzer_init_without_omission.cpp
// Test the focal function sqlite3_fuzzer_init when SQLITE_OMIT_VIRTUALTABLE is NOT defined.
// Expectation: sqlite3_create_module should be invoked exactly once with the module name "fuzzer".
// This test uses the mock sqlite3_create_module to count invocations.

#define _GNU_SOURCE

static int g_sqlite3_create_module_calls = 0;
static sqlite3* g_last_db = nullptr;
static const char* g_last_name = nullptr;

// Provide a C linkage function to intercept calls from sqlite3_fuzzer_init.
// The actual fuzzer.c will call sqlite3_create_module(db, "fuzzer", &fuzzerModule, 0);
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pClientData) {
    ++g_sqlite3_create_module_calls;
    g_last_db = db;
    g_last_name = zName;
    // Return SQLITE_OK to simulate successful module registration
    return SQLITE_OK;
}

// Empty placeholder for the fuzzerModule symbol expected by fuzzer.c.
// This is not used by the test logic, but ensures linker resolves the symbol
// if fuzzer.c references it.
static sqlite3_module fuzzerModule;

int main() {
    // Prepare dummy arguments for sqlite3_fuzzer_init
    sqlite3 db;
    char* errMsg = nullptr;
    sqlite3_api_routines* pApi = nullptr;

    // Call the focal method under test
    int rc = sqlite3_fuzzer_init(&db, &errMsg, pApi);

    // Verify: function reports SQLITE_OK
    if (rc != SQLITE_OK) {
        std::cerr << "sqlite3_fuzzer_init returned non-OK: " << rc << "\n";
        return 1;
    }

    // Verify: sqlite3_create_module was invoked exactly once
    if (g_sqlite3_create_module_calls != 1) {
        std::cerr << "Expected sqlite3_create_module to be called exactly once, but was called "
                  << g_sqlite3_create_module_calls << " times.\n";
        return 2;
    }

    // Verify: the module name used in the registration is "fuzzer"
    if (g_last_name == nullptr || std::string(g_last_name) != "fuzzer") {
        std::cerr << "Expected module name 'fuzzer', got '"
                  << (g_last_name ? g_last_name : "null") << "'.\n";
        return 3;
    }

    std::cout << "Test without SQLITE_OMIT_VIRTUALTABLE passed.\n";
    return 0;
}

// File: tests/test_fuzzer_init_with_omission.cpp
// Test the focal function sqlite3_fuzzer_init when SQLITE_OMIT_VIRTUALTABLE is defined.
// Expectation: sqlite3_create_module should NOT be invoked.

#define _GNU_SOURCE
// Define the omission macro for this test build
#define SQLITE_OMIT_VIRTUALTABLE 1


static int g_sqlite3_create_module_calls = 0;
static sqlite3* g_last_db = nullptr;
static const char* g_last_name = nullptr;

// Intercept sqlite3_create_module; should not be called in this scenario
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pClientData) {
    ++g_sqlite3_create_module_calls;
    g_last_db = db;
    g_last_name = zName;
    return SQLITE_OK;
}

static sqlite3_module fuzzerModule;

int main() {
    sqlite3 db;
    char* errMsg = nullptr;
    sqlite3_api_routines* pApi = nullptr;

    int rc = sqlite3_fuzzer_init(&db, &errMsg, pApi);

    if (rc != SQLITE_OK) {
        std::cerr << "sqlite3_fuzzer_init returned non-OK: " << rc << "\n";
        return 1;
    }

    // When SQLITE_OMIT_VIRTUALTABLE is defined, the module should not be registered.
    if (g_sqlite3_create_module_calls != 0) {
        std::cerr << "sqlite3_create_module should not have been called, but was called "
                  << g_sqlite3_create_module_calls << " times.\n";
        return 2;
    }

    std::cout << "Test with SQLITE_OMIT_VIRTUALTABLE passed.\n";
    return 0;
}