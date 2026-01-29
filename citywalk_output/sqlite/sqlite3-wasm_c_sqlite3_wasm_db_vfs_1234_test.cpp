// Lightweight C++11 test suite for sqlite3_wasm_db_vfs
// This test uses a compile-time mock of sqlite3_file_control by redefining
// the symbol to a test-specific implementation before including sqlite3-wasm.c.
// It verifies branching on zDbName (non-null vs null) and ensures the returned VFS
// pointer comes from the mock despite the internal call.
// Note: This approach assumes sqlite3-wasm.c is compilable in this translation unit
// with the macro override in place.

#include <emscripten/wasmfs.h>
#include <sqlite3-wasm.c>
#include <assert.h>
#include <cstdio>
#include <cstring>


// Forward declare minimal SQLite-like types to satisfy the test harness.
// We do not rely on a real SQLite linkage here; the actual function under test
// is bound by the included sqlite3-wasm.c, which we mock at call sites.
struct sqlite3;
struct sqlite3_vfs;

// Prototype of the function under test (from sqlite3-wasm.c)
extern "C" sqlite3_vfs * sqlite3_wasm_db_vfs(sqlite3 *pDb, const char *zDbName);

// Testing harness globals to capture mock interactions
static const char* g_last_dbname_passed = nullptr;
static void* g_mock_vfs_ptr = (void*)0x12345678; // sentinel VFS pointer returned by mock

// Mock of sqlite3_file_control to intercept the call made by sqlite3_wasm_db_vfs.
// This macro must be defined before including the target C file so that all
// sqlite3_file_control calls are redirected to this function.
extern "C" {

// The mock must be flexible to accept the pointer type used by the real code.
int fake_sqlite3_file_control(void* pDb, const char* zDbName, int op, void** ppVfs) {
    // Capture which database name was requested (as passed to the function).
    g_last_dbname_passed = zDbName;
    // Provide a sentinel VFS pointer back to the callee, matching expectations.
    if (ppVfs) {
        *ppVfs = g_mock_vfs_ptr;
    }
    // Indicate success to mimic the real SQLite API behavior in this test harness.
    return 0;
}
}

// Redirect all calls to sqlite3_file_control in sqlite3-wasm.c to our fake
// The macro definition must precede the inclusion of sqlite3-wasm.c
#define sqlite3_file_control(pDb, zDbName, op, ppVfs) fake_sqlite3_file_control((pDb), (zDbName), (op), (ppVfs))

// Now include the focal implementation. This will compile sqlite3_wasm_db_vfs
// and any necessary code, but all sqlite3_file_control calls will go through the mock.

// Candidate keywords extracted from Step 1 (for reference in test design)
/*
  - sqlite3
  - sqlite3_vfs
  - sqlite3_file_control
  - SQLITE_FCNTL_VFS_POINTER
  - pDb, zDbName, pVfs
  - zDbName ? zDbName : "main"
  - NULL handling
  - pointer return value (VFS pointer)
  - test stubs/mocks
*/

// Simple test harness helpers
static int g_total = 0;
static int g_failed = 0;

static void report(const char* test_desc, bool passed) {
    ++g_total;
    if (passed) {
        printf("[PASS] %s\n", test_desc);
    } else {
        ++g_failed;
        printf("[FAIL] %s\n", test_desc);
    }
}

// Test 1: zDbName is non-null, ensure the function passes the provided name to sqlite3_file_control
// and returns the mocked VFS pointer.
static void test_db_vfs_with_non_null_dbname() {
    // Reset capture data
    g_last_dbname_passed = nullptr;
    const char* testDbName = "mydb";
    void* pDb = (void*)0xDEADBEEF; // arbitrary non-null database handle

    sqlite3_vfs* pVfs = sqlite3_wasm_db_vfs((sqlite3*)pDb, testDbName);

    bool passedPointer = (pVfs == (sqlite3_vfs*)g_mock_vfs_ptr);
    bool passedName    = (g_last_dbname_passed != nullptr) && (strcmp(g_last_dbname_passed, testDbName) == 0);

    report("test_db_vfs_with_non_null_dbname: returned VFS matches mock; correct zDbName passed", 
           passedPointer && passedName);
}

// Test 2: zDbName is NULL, ensure the function uses "main" as default and returns the mocked VFS pointer.
// Also verify that "main" is passed to sqlite3_file_control.
static void test_db_vfs_with_null_dbname_defaults_to_main() {
    g_last_dbname_passed = nullptr;
    const char* expected = "main";
    void* pDb = (void*)0xCAFEBABE;

    sqlite3_vfs* pVfs = sqlite3_wasm_db_vfs((sqlite3*)pDb, nullptr);

    bool passedPointer = (pVfs == (sqlite3_vfs*)g_mock_vfs_ptr);
    bool passedName    = (g_last_dbname_passed != nullptr) && (strcmp(g_last_dbname_passed, expected) == 0);

    report("test_db_vfs_with_null_dbname_defaults_to_main: default name 'main' passed; VFS pointer returned", 
           passedPointer && passedName);
}

// Test 3: pDb is NULL (edge case), ensure the function still attempts to fetch VFS pointer
// and returns the mocked VFS pointer without crashing.
static void test_db_vfs_with_null_pDb() {
    g_last_dbname_passed = nullptr;
    const char* expectedName = "main"; // same as Test 2 when zDbName is NULL
    void* pDb = nullptr;

    sqlite3_vfs* pVfs = sqlite3_wasm_db_vfs((sqlite3*)pDb, nullptr);

    bool passedPointer = (pVfs == (sqlite3_vfs*)g_mock_vfs_ptr);
    bool passedName    = (g_last_dbname_passed != nullptr) && (strcmp(g_last_dbname_passed, expectedName) == 0);

    report("test_db_vfs_with_null_pDb: NULL pDb handled; default name used and VFS returned", 
           passedPointer && passedName);
}

int main() {
    // Run tests in a simple sequence with explanatory comments.
    test_db_vfs_with_non_null_dbname();
    test_db_vfs_with_null_dbname();
    test_db_vfs_with_null_pDb();

    // Summary
    printf("Test summary: total=%d, failed=%d\n", g_total, g_failed);
    return g_failed;
}