// Test suite for sqlite3_sqlar_init using a lightweight, self-contained harness.
// This avoids GTest and uses only the C++11 standard library.
// The goal is to verify the behavior of sqlite3_sqlar_init when sqlite3_create_function
// succeeds or fails for the first/second function registrations.
//
// Note: This test intentionally mocks the minimal sqlite3 API surface required by the
// focal function. It is designed to compile in a plain C++11 environment.

#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <iostream>
#include <zlib.h>


// Minimal type placeholders to mimic SQLite's API surface used by sqlite3_sqlar_init.
struct sqlite3 {};
struct sqlite3_value {};
struct sqlite3_context {};
struct sqlite3_api_routines {};

// Constants copied from SQLite for test purposes.
#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_UTF8 0x01
#define SQLITE_INNOCUOUS 0x02

// Macro shim: in the real code, SQLITE_EXTENSION_INIT2(pApi) initializes extension hooks.
// For the test harness, this is a no-op to keep compilation simple.
#define SQLITE_EXTENSION_INIT2(pApi) do { (void)(pApi); } while(0)

// Forward declarations of the functions required by sqlite3_sqlar_init.
static void sqlarCompressFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
static void sqlarUncompressFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

// A lightweight, mockable interface for sqlite3_create_function used by sqlite3_sqlar_init.
static int g_callCount = 0; // Tracks how many times sqlite3_create_function has been invoked in a test run.
struct CreateFunctionCall {
    const char* name;
    int argc;
    int flags;
};
static std::vector<CreateFunctionCall> g_calls; // Records the calls for verification.

static bool g_failFirst = false;
static bool g_failSecond = false;

// Mock implementation of sqlite3_create_function.
// It records the call details and can be configured to fail on the first or second call.
static int sqlite3_create_function(
    sqlite3*,                    // db
    const char* zFunctionName,   // function name
    int nArg,                    // number of arguments
    int eTextRep,                // text encoding and flags
    void* pApp,                   // application data
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void (*xStep)(sqlite3_context*, int, sqlite3_value**),
    void (*xFinal)(sqlite3_context*) )
{
    ++g_callCount;
    g_calls.push_back({zFunctionName, nArg, eTextRep});

    // Simulate failure on first or second call based on test configuration.
    if (g_failFirst && g_callCount == 1) {
        return SQLITE_ERROR;
    }
    if (g_failSecond && g_callCount == 2) {
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}

// The focal function under test, reproduced as in the provided snippet.
int sqlite3_sqlar_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
{
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  rc = sqlite3_create_function(db, "sqlar_compress", 1, 
                               SQLITE_UTF8|SQLITE_INNOCUOUS, 0,
                               sqlarCompressFunc, 0, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sqlar_uncompress", 2,
                                 SQLITE_UTF8|SQLITE_INNOCUOUS, 0,
                                 sqlarUncompressFunc, 0, 0);
  }
  return rc;
}
}

// Implementations of the static functions with empty bodies as placeholders.
static void sqlarCompressFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context; (void)argc; (void)argv;
}
static void sqlarUncompressFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context; (void)argc; (void)argv;
}

// Simple test harness helpers
static int gTotalTests = 0;
static int gFailures = 0;

#define CHECK(cond, msg) do { \
    ++gTotalTests; \
    if(!(cond)) { \
        ++gFailures; \
        std::cerr << "Test " << gTotalTests << " FAILED: " << (msg) << std::endl; \
    } else { \
        std::cout << "Test " << gTotalTests << " PASSED: " << (msg) << std::endl; \
    } \
} while(0)

// Helper to reset test state between cases
static void resetTestState() {
    g_callCount = 0;
    g_calls.clear();
    g_failFirst = false;
    g_failSecond = false;
}

// Test Case 1: Both registrations succeed; expect SQLITE_OK and two calls with correct arguments.
static void test_sqlar_init_both_functions_succeed() {
    resetTestState();

    // Arrange
    sqlite3* db = nullptr;
    char* errMsg = nullptr;
    sqlite3_api_routines* pApi = nullptr;

    // Act
    int rc = sqlite3_sqlar_init(db, &errMsg, pApi);

    // Assert
    CHECK(rc == SQLITE_OK, "sqlite3_sqlar_init returns SQLITE_OK when both registrations succeed");
    CHECK(static_cast<int>(g_calls.size()) == 2, "Two sqlite3_create_function calls were made");

    if (g_calls.size() >= 2) {
        CHECK(std::string(g_calls[0].name) == "sqlar_compress",
              "First function registered is sqlar_compress");
        CHECK(g_calls[0].argc == 1, "sqlar_compress has argc = 1");
        int expectedFlags = SQLITE_UTF8 | SQLITE_INNOCUOUS;
        CHECK(g_calls[0].flags == expectedFlags,
              "sqlar_compress has correct flags");
        
        CHECK(std::string(g_calls[1].name) == "sqlar_uncompress",
              "Second function registered is sqlar_uncompress");
        CHECK(g_calls[1].argc == 2, "sqlar_uncompress has argc = 2");
        CHECK(g_calls[1].flags == expectedFlags,
              "sqlar_uncompress has correct flags");
    }
}

// Test Case 2: First registration fails; expect SQLITE_ERROR and only one call recorded.
static void test_sqlar_init_first_registration_fails() {
    resetTestState();

    // Arrange
    g_failFirst = true;
    sqlite3* db = nullptr;
    char* errMsg = nullptr;
    sqlite3_api_routines* pApi = nullptr;

    // Act
    int rc = sqlite3_sqlar_init(db, &errMsg, pApi);

    // Assert
    CHECK(rc == SQLITE_ERROR, "sqlite3_sqlar_init returns SQLITE_ERROR when first registration fails");
    CHECK(static_cast<int>(g_calls.size()) == 1, "Only the first registration attempt is recorded");
    if (!g_calls.empty()) {
        CHECK(std::string(g_calls[0].name) == "sqlar_compress",
              "First call recorded should be sqlar_compress");
    }
}

// Test Case 3: First registration succeeds, second fails; expect SQLITE_ERROR and two calls.
static void test_sqlar_init_second_registration_fails() {
    resetTestState();

    // Arrange
    g_failSecond = true;
    sqlite3* db = nullptr;
    char* errMsg = nullptr;
    sqlite3_api_routines* pApi = nullptr;

    // Act
    int rc = sqlite3_sqlar_init(db, &errMsg, pApi);

    // Assert
    CHECK(rc == SQLITE_ERROR, "sqlite3_sqlar_init returns SQLITE_ERROR when second registration fails");
    CHECK(static_cast<int>(g_calls.size()) == 2, "Two registrations attempted when first succeeds");
    if (g_calls.size() >= 2) {
        CHECK(std::string(g_calls[0].name) == "sqlar_compress",
              "First call should be sqlar_compress");
        CHECK(std::string(g_calls[1].name) == "sqlar_uncompress",
              "Second call should be sqlar_uncompress");
    }
}

// Main driver
int main() {
    std::cout << "Starting sqlite3_sqlar_init unit tests (no GTest)..." << std::endl;

    test_sqlar_init_both_functions_succeed();
    test_sqlar_init_first_registration_fails();
    test_sqlar_init_second_registration_fails();

    // Summary
    std::cout << "Total tests run: " << gTotalTests << std::endl;
    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " test(s) FAILED" << std::endl;
        return 1;
    }
}