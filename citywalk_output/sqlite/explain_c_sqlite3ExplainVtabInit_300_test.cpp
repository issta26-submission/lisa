// Minimal C++11 test harness for sqlite3ExplainVtabInit
// This test mocks the minimal surrounding sqlite3 API needed by sqlite3ExplainVtabInit
// and validates behavior for both branches of the SQLITE_OMIT_VIRTUALTABLE macro.
//
// Build instructions (two separate runs required to test both branches):
// 1) Test with virtual table support (default): g++ -DTEST_WITH_VIRTUALTABLE test_explain_vtab_init.cpp -o test_virtual
// 2) Test without virtual table support: g++ -DTEST_WITHOUT_VIRTUALTABLE -DSQLITE_OMIT_VIRTUALTABLE test_explain_vtab_init.cpp -o test_without_virtual
//
// Each build uses the same source, but compiles with different macro definitions to exercise both paths.

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>


// -------------------------------------------------------------------------------------
// Mocked SQLite-like types and API used by sqlite3ExplainVtabInit
// -------------------------------------------------------------------------------------

// Forward-declared opaque types to resemble sqlite3 API.
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_module sqlite3_module;

// Minimal sqlite3_module placeholder (behavior not needed for tests)
struct sqlite3_module {
    // Empty placeholder; the real module would contain function pointers
    // not used by this test harness
    int dummy;
};

// Global module instance used by sqlite3ExplainVtabInit when not OMIT
static sqlite3_module explainModule;

// Global capture variables for sqlite3_create_module interception
static sqlite3* g_lastDb = nullptr;
static const char* g_lastName = nullptr;
static std::string g_lastNameStr;
static sqlite3_module* g_lastModule = nullptr;
static void* g_nextClientData = nullptr;
static int g_lastCreatedRc = 0; // the rc that sqlite3_create_module should return

// Pointer to pass-through to the module code; not used in these tests beyond address capture
static void* g_lastClientData = nullptr;

// Our fake sqlite3_create_module that records inputs and returns a configurable RC
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pClientData)
{
    // Capture arguments
    g_lastDb = db;
    // Store a stable copy of the name
    g_lastNameStr = (zName != nullptr) ? std::string(zName) : "";
    g_lastName = g_lastNameStr.c_str();
    g_lastModule = const_cast<sqlite3_module*>(pModule);
    g_lastClientData = pClientData;
    // Return the configured value
    return g_lastCreatedRc;
}

// Simple sqlite3_OK constant replacement
#define SQLITE_OK 0

// -------------------------------------------------------------------------------------
// Re-implementation of the focal function under test
// Note: We copy the exact logic as given, including the preprocessor branch.
// This allows a self-contained test harness without needing the full SQLite build.
// -------------------------------------------------------------------------------------

// Lightweight sqlite3 type definition to allow the function signature to compile
struct sqlite3 {};

// The focal method under test (copied verbatim from the provided snippet)
int sqlite3ExplainVtabInit(sqlite3 *db){
  int rc = SQLITE_OK;
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3_create_module(db, "explain", &explainModule, 0);
#endif
  return rc;
}

// -------------------------------------------------------------------------------------
// Test harness: two separate builds (two main functions) driven by compile-time macros
// -------------------------------------------------------------------------------------

// Helper test runner utilities
static void test_pass(bool ok, const std::string& msg) {
    if (ok) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cerr << "[FAIL] " << msg << "\n";
    }
}

// Test 1: When SQLITE_OMIT_VIRTUALTABLE is NOT defined (virtual table support enabled)
// Expect sqlite3ExplainVtabInit to call sqlite3_create_module with correct parameters
#if defined(TEST_WITH_VIRTUALTABLE)

static void run_test_with_virtual_table() {
    // Reset captured state
    g_lastDb = nullptr;
    g_lastName = nullptr;
    g_lastNameStr.clear();
    g_lastModule = nullptr;
    g_lastClientData = nullptr;
    g_lastCreatedRc = 12345; // distinct value to ensure we return what the mock provides

    // Use a non-null db pointer to simulate a real call
    sqlite3 dummyDb;
    int rc = sqlite3ExplainVtabInit(&dummyDb);

    // Assertions (non-terminating): verify all expected interactions occurred
    test_pass(rc == 12345, "sqlite3ExplainVtabInit returns the rc provided by sqlite3_create_module (12345)");

    test_pass(g_lastDb == &dummyDb, "sqlite3ExplainVtabInit passes the correct db pointer to sqlite3_create_module");
    test_pass(g_lastName != nullptr && std::strcmp(g_lastName, "explain") == 0,
              "sqlite3ExplainVtabInit passes the correct module name 'explain' to sqlite3_create_module");
    test_pass(g_lastModule == &explainModule,
              "sqlite3ExplainVtabInit passes the address of explainModule to sqlite3_create_module");
    test_pass(g_lastClientData == nullptr,
              "sqlite3ExplainVtabInit passes a null clientData (0) to sqlite3_create_module");
}

// Entry point for this branch
int main() {
    std::cout << "Running tests with virtual table support (SQLITE_OMIT_VIRTUALTABLE not defined)...\n";
    run_test_with_virtual_table();
    return 0;
}

#endif // TEST_WITH_VIRTUALTABLE

// Test 2: When SQLITE_OMIT_VIRTUALTABLE is defined
// Expect sqlite3ExplainVtabInit to skip the module creation and return SQLITE_OK
#if defined(TEST_WITHOUT_VIRTUALTABLE) && defined(SQLITE_OMIT_VIRTUALTABLE)

static void run_test_without_virtual_table() {
    // Reset captured state
    g_lastDb = nullptr;
    g_lastName = nullptr;
    g_lastNameStr.clear();
    g_lastModule = nullptr;
    g_lastClientData = nullptr;
    g_lastCreatedRc = 0; // not used in this path

    // Even if a db pointer is provided, it should be ignored for the module creation
    sqlite3 dummyDb;
    int rc = sqlite3ExplainVtabInit(&dummyDb);

    // Assertions (non-terminating)
    test_pass(rc == SQLITE_OK, "sqlite3ExplainVtabInit returns SQLITE_OK when SQLITE_OMIT_VIRTUALTABLE is defined");
    test_pass(g_lastDb == nullptr && g_lastName == nullptr && g_lastModule == nullptr,
              "sqlite3ExplainVtabInit does not invoke sqlite3_create_module when OMIT flag is defined");
}

// Entry point for this branch
int main() {
    std::cout << "Running tests without virtual table support (SQLITE_OMIT_VIRTUALTABLE defined)...\n";
    run_test_without_virtual_table();
    return 0;
}

#endif // TEST_WITHOUT_VIRTUALTABLE && SQLITE_OMIT_VIRTUALTABLE

// -------------------------------------------------------------------------------------
// If neither branch is defined, provide a helpful compile-time message.
// This keeps the file self-contained and user-friendly.
// -------------------------------------------------------------------------------------
#if !defined(TEST_WITH_VIRTUALTABLE) && !defined(TEST_WITHOUT_VIRTUALTABLE)
#error "Define TEST_WITH_VIRTUALTABLE to test with virtual tables, or TEST_WITHOUT_VIRTUALTABLE with SQLITE_OMIT_VIRTUALTABLE defined to test without virtual tables."
#endif