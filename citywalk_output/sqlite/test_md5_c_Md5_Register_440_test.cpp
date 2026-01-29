// Minimal C++11 test harness for Md5_Register using a lightweight mock of the sqlite3 API.
// This test does not rely on GTest. It uses a small in-file test runner with non-terminating
// assertions and prints a summary at the end.
// The test focuses on verifying that Md5_Register correctly registers the md5sum function
// via sqlite3_create_function and exercises sqlite3_overload_function as per the focal code.

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <string>
#include <iostream>


// ------------------------------------------------------------
// Mocked sqlite3 API (inline, within this test file only)
#ifndef INLINE_SQLITE3_MOCK
#define INLINE_SQLITE3_MOCK

// Forward-declare minimal sqlite3-related types to allow Md5_Register to compile.
// We define a dummy layout so objects can be instantiated in tests.
struct sqlite3 {
    int dummy;
};
struct sqlite3_context {
    int dummy;
};
struct sqlite3_value {
    int dummy;
};

// Forward declare sqlite3_api_routines struct as used in Md5_Register's signature.
struct sqlite3_api_routines;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Constants to mirror sqlite3 API usage in the focal code.
#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_UTF8 1

// Global state to observe calls made by Md5_Register via the mock API.
static int g_next_rc = SQLITE_OK;                 // Next return code for sqlite3_create_function
static bool g_overload_called = false;            // Whether sqlite3_overload_function was invoked
static std::string g_last_function_name;          // Name passed to sqlite3_create_function
static int g_last_nArg = 0;                       // nArg passed to sqlite3_create_function
static int g_last_eTextRep = 0;                   // eTextRep passed to sqlite3_create_function
static int g_sqlite3_create_function_calls = 0;   // Count of calls to sqlite3_create_function

extern "C" {

// Mock implementation of sqlite3_create_function
int sqlite3_create_function(
    sqlite3* db,
    const char* zFunctionName,
    int nArg,
    int eTextRep,
    void* pApp,
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void (*xStep)(sqlite3_context*, int, sqlite3_value**),
    void (*xFinal)(sqlite3_context*)
) {
    (void)db; (void)pApp; (void)xFunc; (void)xStep; (void)xFinal; // silence unused in test
    g_last_function_name = zFunctionName ? zFunctionName : "";
    g_last_nArg = nArg;
    g_last_eTextRep = eTextRep;
    g_sqlite3_create_function_calls++;
    return g_next_rc;
}

// Mock implementation of sqlite3_overload_function
void sqlite3_overload_function(sqlite3* db, const char* zName, int nArg) {
    (void)db; (void)zName; (void)nArg;
    g_overload_called = true;
}

// Minimal stubs for md5step/md5finalize usage in Md5_Register (no real MD5 logic needed for this test)
static void md5step(sqlite3_context* context, int argc, sqlite3_value** argv) {
    (void)context; (void)argc; (void)argv;
}
static void md5finalize(sqlite3_context* context) {
    (void)context;
}

// Implement the type used by Md5_Register for pThunk (present in the focal code)
typedef struct sqlite3_api_routines sqlite3_api_routines;
}
#endif // INLINE_SQLITE3_MOCK

// ------------------------------------------------------------
// Production-like focal function and its dependencies (reproduced here for testing)
extern "C" {

// Forward declare the API routine type used by Md5_Register
typedef struct sqlite3_api_routines sqlite3_api_routines;

// The Md5_Register function under test (as extracted and corrected from the provided snippet)
int Md5_Register(
    sqlite3 *db,
    char **pzErrMsg,
    const sqlite3_api_routines *pThunk
){
    int rc = sqlite3_create_function(db, "md5sum", -1, SQLITE_UTF8, 0, 0,
                                     md5step, md5finalize);
    sqlite3_overload_function(db, "md5sum", -1);
    return rc;
}
} // extern "C"

// ------------------------------------------------------------
// Lightweight test harness (non-terminating assertions, simple reporting)
namespace {

// Utility to reset mock state between tests
void reset_mock_state() {
    g_next_rc = SQLITE_OK;
    g_overload_called = false;
    g_last_function_name.clear();
    g_last_nArg = 0;
    g_last_eTextRep = 0;
    g_sqlite3_create_function_calls = 0;
}

// Simple, non-terminating assertion helper
void expect_true(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "EXPECTATION FAILED: " << message << std::endl;
    }
}

} // anonymous namespace

// ------------------------------------------------------------
// Test 1: Normal path - sqlite3_create_function returns SQLITE_OK
// Verifies Md5_Register registers md5sum with correct metadata and calls sqlite3_overload_function.
bool test_Md5_Register_Normal() {
    // Arrange
    reset_mock_state();
    g_next_rc = SQLITE_OK;     // Simulate successful registration
    sqlite3 db;                  // dummy db object (opaque to the mock)
    char* errMsg = nullptr;

    // Act
    int rc = Md5_Register(&db, &errMsg, nullptr);

    // Assert
    bool cond = (rc == SQLITE_OK)
                && (g_sqlite3_create_function_calls == 1)
                && (g_last_function_name == "md5sum")
                && (g_last_nArg == -1)
                && (g_last_eTextRep == SQLITE_UTF8)
                && (g_overload_called);

    expect_true(cond, "Md5_Register Normal path should return SQLITE_OK and register md5sum with proper metadata; overload should be invoked.");
    return cond;
}

// ------------------------------------------------------------
// Test 2: Error path - sqlite3_create_function returns an error code
// Verifies Md5_Register propagates the error code and still invokes sqlite3_overload_function (as per code sequence).
bool test_Md5_Register_Error() {
    // Arrange
    reset_mock_state();
    g_next_rc = SQLITE_ERROR;    // Simulate registration failure
    sqlite3 db;
    char* errMsg = nullptr;

    // Act
    int rc = Md5_Register(&db, &errMsg, nullptr);

    // Assert
    bool cond = (rc == SQLITE_ERROR)
                && (g_sqlite3_create_function_calls == 1)
                && (g_last_function_name == "md5sum")
                && (g_last_nArg == -1)
                && (g_last_eTextRep == SQLITE_UTF8)
                && (g_overload_called);

    expect_true(cond, "Md5_Register Error path should propagate error code and still call overload function.");
    return cond;
}

// ------------------------------------------------------------
// Entry point: run tests and print a summary
int main() {
    std::cout << "Running test suite for Md5_Register...\n";

    int passed = 0;
    int failed = 0;

    if (test_Md5_Register_Normal()) ++passed; else ++failed;
    if (test_Md5_Register_Error()) ++passed; else ++failed;

    std::cout << "Tests run: 2, Passed: " << passed << ", Failed: " << failed << "\n";
    if (failed > 0) {
        std::cout << "Some tests failed. Check error messages above for details.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}