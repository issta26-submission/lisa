// Test suite for the focal method: sqlite3_wal_autocheckpoint
// This test suite is designed to work with a C++11 build without GTest.
// It relies on linking against the project's implementation of sqlite3_wal_autocheckpoint
// (as provided in main.c) and mocks out some external SQLite dependencies to observe behavior.
//
// Notes about test design (mapped to the provided instructions):
// - Step 1 (Program Understanding): The focal function toggles a WAL autohook depending on nFrame
//   and compile-time macros. The critical dependent components observed are: sqlite3_wal_hook,
//   sqlite3WalDefaultHook, and sqlite3SafetyCheckOk (plus the return codes SQLITE_OK and
//   SQLITE_MISUSE_BKPT). We create Candidate Keywords accordingly: wal_hook, walDefaultHook,
//   SafetyCheckOk, nFrame, SQLITE_OK, SQLITE_MISUSE_BKPT.
// - Step 2 (Unit Test Generation): We exercise true branches (nFrame > 0 calls the hook with
//   sqlite3WalDefaultHook and pArg = (void*)nFrame) and (nFrame == 0 calls wal_hook with NULL
//   callback and NULL arg). We also exercise the safety check branch that returns a non-zero code
//   when the safety check fails (to cover SQLITE_MISUSE_BKPT scenario).
// - Step 3 (Test Case Refinement): Tests are implemented with a lightweight non-terminating
//   assertion mechanism so execution can proceed after a failure, maximizing coverage.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Forward declare the sqlite3 type and the function under test.
// These declarations mimic the minimal external interface used by sqlite3_wal_autocheckpoint.
// We do not rely on the full SQLite headers here to keep the test focused and self-contained.
struct sqlite3 {};

// Declaration of the focal function under test.
// It is expected to be defined in the project (main.c) and linked in the test binary.
extern "C" int sqlite3_wal_autocheckpoint(sqlite3 *db, int nFrame);

// ------------------------------------------------------------------------------------
// Mocking external dependencies to observe internal calls without requiring a full SQLite build.
// The test suite overrides the following symbols that the focal method interacts with:
// - int sqlite3_wal_hook(sqlite3 *db, int(*xCallback)(void *, sqlite3*, const char*, int), void *pArg)
// - int sqlite3WalDefaultHook(void*, sqlite3*, const char*, int)
// - int sqlite3SafetyCheckOk(sqlite3 *db)
// These mocks are provided with C linkage to ensure linkage against the project's C code.
// ------------------------------------------------------------------------------------

// Global test state to observe the behavior of the focal method
static int g_safetyOk = 1;                          // controls sqlite3SafetyCheckOk return
static int g_walHookCalled = 0;                      // set when sqlite3_wal_hook is invoked
static int g_walHookUsedDefault = 0;                 // 1 if xCallback == sqlite3WalDefaultHook
static void* g_walHookPArg = nullptr;                // pArg passed to sqlite3_wal_hook
static int g_lastRet = 0;                            // last return value from sqlite3_wal_autocheckpoint

// Forward declare the mock "real" hook function to compare pointer identity
extern "C" int sqlite3WalDefaultHook(void*, sqlite3*, const char*, int);

// Mock implementation of sqlite3_wal_hook
extern "C" int sqlite3_wal_hook(sqlite3 *db,
                              int (*xCallback)(void *, sqlite3*, const char*, int),
                              void *pArg)
{
    g_walHookCalled = 1;
    g_walHookUsedDefault = (xCallback == sqlite3WalDefaultHook);
    g_walHookPArg = pArg;
    // In the real SQLite, this would register a hook; here we just record state.
    return 0;
}

// Mock implementation of sqlite3WalDefaultHook (the routine passed by sqlite3_wal_autocheckpoint)
extern "C" int sqlite3WalDefaultHook(void* pClientData, sqlite3 *db, const char *zDb, int nFrame)
{
    // No-op for testing; the identity of the function pointer is what we observe.
    (void)pClientData; (void)db; (void)zDb; (void)nFrame;
    return 0;
}

// Mock implementation of sqlite3SafetyCheckOk
extern "C" int sqlite3SafetyCheckOk(sqlite3 *db)
{
    (void)db;
    return g_safetyOk;
}

// ------------------------------------------------------------------------------------
// Lightweight non-terminating assertion helpers (do not exit on failure)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_total_tests; \
    if ((expected) != (actual)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << (msg) << " Expected: " << (expected) \
                  << " Actual: " << (actual) << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define RUN_TEST(test_func) do { \
    std::cout << "Running " << #test_func << "..." << std::endl; \
    test_func(); \
} while(0)

// ------------------------------------------------------------------------------------
// Test Cases

// Test 1: When nFrame > 0, the function should set the WAL hook to the default hook
// and pass pArg equal to (void*)nFrame.
static void test_wal_autocheckpoint_positive_frame_sets_hook()
{
    // Reset state
    g_safetyOk = 1;
    g_walHookCalled = 0;
    g_walHookUsedDefault = 0;
    g_walHookPArg = nullptr;
    g_lastRet = 0;

    sqlite3 sqliteDb; // dummy db object
    int ret = sqlite3_wal_autocheckpoint(&sqliteDb, 5);

    // The function should return SQLITE_OK (0) on success
    EXPECT_EQ(0, ret, "sqlite3_wal_autocheckpoint should return 0 (SQLITE_OK) on success");

    // Verify that the hook was registered and that the default hook was used
    EXPECT_TRUE(g_walHookCalled, "wal_hook should be called when nFrame > 0");
    EXPECT_TRUE(g_walHookUsedDefault, "wal_hook should be registered with sqlite3WalDefaultHook when nFrame > 0");
    EXPECT_EQ(reinterpret_cast<void*>(5), g_walHookPArg, "wal_hook should be passed SQLITE_INT_TO_PTR(nFrame)");
}

// Test 2: When nFrame == 0, the function should clear the WAL hook (hook callback to NULL)
static void test_wal_autocheckpoint_zero_frame_clears_hook()
{
    // Reset state
    g_safetyOk = 1;
    g_walHookCalled = 0;
    g_walHookUsedDefault = 0;
    g_walHookPArg = reinterpret_cast<void*>(0xDEADBEEF);

    sqlite3 sqliteDb;
    int ret = sqlite3_wal_autocheckpoint(&sqliteDb, 0);

    // We expect success as before
    EXPECT_EQ(0, ret, "sqlite3_wal_autocheckpoint should return 0 (SQLITE_OK) on success (nFrame == 0)");

    // The hook should have been invoked, but with NULL callback and NULL arg
    EXPECT_TRUE(g_walHookCalled, "wal_hook should be called when nFrame == 0");
    EXPECT_FALSE(g_walHookUsedDefault, "wal_hook should be registered with NULL callback when nFrame == 0");
    EXPECT_EQ(nullptr, g_walHookPArg, "wal_hook should be called with NULL pArg when nFrame == 0");
}

// Test 3: When API armor safety check fails, function returns non-zero (SQLITE_MISUSE_BKPT)
static void test_wal_autocheckpoint_safety_check_failure_returns_error()
{
    // Simulate safety check failing
    g_safetyOk = 0;
    g_walHookCalled = 0;
    g_walHookPArg = nullptr;

    sqlite3 sqliteDb;
    int ret = sqlite3_wal_autocheckpoint(&sqliteDb, 3);

    // Expect a failure return code (non-zero indicates misuse)
    EXPECT_TRUE(ret != 0, "sqlite3_wal_autocheckpoint should return a non-zero code when safety check fails");
}

// ------------------------------------------------------------------------------------
// Main: run tests
int main()
{
    std::cout << "Starting sqlite3_wal_autocheckpoint unit tests (C++11, non-GTest)..." << std::endl;

    RUN_TEST(test_wal_autocheckpoint_positive_frame_sets_hook);
    RUN_TEST(test_wal_autocheckpoint_zero_frame_clears_hook);
    RUN_TEST(test_wal_autocheckpoint_safety_check_failure_returns_error);

    std::cout << "Tests completed. "
              << g_failed_tests << " failed, out of "
              << g_total_tests << " tests." << std::endl;

    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED." << std::endl;
        return 1;
    }
}