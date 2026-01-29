// cpp_unit_tests_sqlite3OsDelete.cpp
// A small, self-contained C++11 test suite for the focal method sqlite3OsDelete.
// No GoogleTest/GMock; uses a minimal custom test harness and direct function calls.

#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Include SQLite headers to obtain sqlite3_vfs type, SQLITE_OK, etc.
// We wrap the include in extern "C" to ensure C linkage for the C API.
extern "C" {
}

// Extern declaration of the focal function under test (from os.c)
extern "C" int sqlite3OsDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync);

// --- Test scaffolding (mock VFS) ---

// A test VFS instance used to inject a mock xDelete implementation.
static sqlite3_vfs testVfs = {0};

// State captured by the mock xDelete for assertions
static int lastDirSync = -1;
static const char* lastPath = nullptr;
static int mockCalled = 0;

// Return value that the mock xDelete should produce
static int mockReturnValue = 0;

// Mock implementation of xDelete to verify behavior of sqlite3OsDelete
static int mock_xDelete(sqlite3_vfs* /*pVfs*/, const char* zPath, int dirSync) {
    lastDirSync = dirSync;
    lastPath = zPath;
    mockCalled = 1;
    return mockReturnValue;
}

// Prepare the test VFS with the mock and reset state
static void prepareTestEnvironment() {
    testVfs.xDelete = mock_xDelete;
    lastDirSync = -1;
    lastPath = nullptr;
    mockCalled = 0;
    mockReturnValue = 0;
}

// --- Unit tests ---

// Test 1: When xDelete is provided, dirSync=0 should be passed through to xDelete
// and the return value should match the xDelete's return value.
static bool test_sqlite3OsDelete_uses_xDelete_with_dirSync0() {
    prepareTestEnvironment();
    mockReturnValue = 12345; // arbitrary non-zero return value to verify propagation
    int rc = sqlite3OsDelete(&testVfs, "/tmp/test_case_1", 0);

    bool ok = (rc == 12345)
              && (mockCalled == 1)
              && (lastDirSync == 0)
              && (std::strcmp(lastPath, "/tmp/test_case_1") == 0);

    if (!ok) {
        std::cerr << "[FAIL] test_sqlite3OsDelete_uses_xDelete_with_dirSync0: "
                  << "rc=" << rc
                  << ", mockCalled=" << mockCalled
                  << ", lastDirSync=" << lastDirSync
                  << ", lastPath=" << (lastPath ? lastPath : "nullptr")
                  << std::endl;
    }
    return ok;
}

// Test 2: When xDelete is provided, dirSync=1 should be passed through to xDelete
// and the return value should match the xDelete's return value.
static bool test_sqlite3OsDelete_uses_xDelete_with_dirSync1() {
    prepareTestEnvironment();
    mockReturnValue = -7; // another arbitrary return value
    int rc = sqlite3OsDelete(&testVfs, "/tmp/test_case_2", 1);

    bool ok = (rc == -7)
              && (mockCalled == 1)
              && (lastDirSync == 1)
              && (std::strcmp(lastPath, "/tmp/test_case_2") == 0);

    if (!ok) {
        std::cerr << "[FAIL] test_sqlite3OsDelete_uses_xDelete_with_dirSync1: "
                  << "rc=" << rc
                  << ", mockCalled=" << mockCalled
                  << ", lastDirSync=" << lastDirSync
                  << ", lastPath=" << (lastPath ? lastPath : "nullptr")
                  << std::endl;
    }
    return ok;
}

// Test 3: When xDelete is NULL, sqlite3OsDelete should return SQLITE_OK
// and should not attempt to call xDelete.
static bool test_sqlite3OsDelete_when_xDelete_is_null() {
    // Reset environment and intentionally disable xDelete
    prepareTestEnvironment();
    testVfs.xDelete = nullptr;

    int rc = sqlite3OsDelete(&testVfs, "/tmp/test_case_3", 0);

    bool ok = (rc == SQLITE_OK)
              && (mockCalled == 0) // xDelete should not have been invoked
              && (lastDirSync == -1)
              && (lastPath == nullptr);

    if (!ok) {
        std::cerr << "[FAIL] test_sqlite3OsDelete_when_xDelete_is_null: "
                  << "rc=" << rc
                  << ", mockCalled=" << mockCalled
                  << ", lastDirSync=" << lastDirSync
                  << ", lastPath=" << (lastPath ? lastPath : "nullptr")
                  << std::endl;
    }
    return ok;
}

// --- Minimal test runner (no external framework) ---

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool condition){
        ++total;
        if (condition) {
            ++passed;
            std::cout << "[PASS] " << name << std::endl;
            return;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
            return;
        }
    };

    run("sqlite3OsDelete uses xDelete with dirSync=0", test_sqlite3OsDelete_uses_xDelete_with_dirSync0());
    run("sqlite3OsDelete uses xDelete with dirSync=1", test_sqlite3OsDelete_uses_xDelete_with_dirSync1());
    run("sqlite3OsDelete returns SQLITE_OK when xDelete is NULL", test_sqlite3OsDelete_when_xDelete_is_null());

    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}