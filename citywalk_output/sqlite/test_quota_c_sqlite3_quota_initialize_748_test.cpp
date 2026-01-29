// Test suite for sqlite3_quota_initialize using a lightweight, non-GTest approach.
// Language: C++11 (link against the project's C code and SQLite headers).
// This file provides three targeted tests to exercise critical control-flow paths.
// Each test uses a lightweight CHECK macro that records failures but continues
// execution to maximize code coverage.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cassert>


// Include SQLite and quota headers from the project test environment.
extern "C" {
}

// Lightweight test harness
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg) do { \
    if (cond) { \
        ++g_pass; \
    } else { \
        ++g_fail; \
        std::cerr << "Test failure: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Forward declarations for dummy VFS used in the tests
static int dummy_xOpen(sqlite3_vfs*, const char*, sqlite3_file*, int, int*);
static int dummy_xDelete(sqlite3_vfs*, const char*, int);
static int dummy_xAccess(sqlite3_vfs*, const char*, int, int*);
static int dummy_xFullPathname(sqlite3_vfs*, const char*, int, char*);

// Simple dummy VFS to use with sqlite3_vfs_register for controlled testing
static sqlite3_vfs dummy_vfs;

// Implementations for the dummy VFS minimum required callbacks.
// These functions are intentionally no-ops; the quota code only copies and overrides
// a subset of the VFS data, and the tests do not perform real I/O through this VFS.
static int dummy_xOpen(sqlite3_vfs* vfs, const char* zName, sqlite3_file* pFile, int flags, int* pOutFlags) {
    (void)vfs; (void)zName; (void)pFile; (void)flags; if (pOutFlags) *pOutFlags = 0; return SQLITE_OK;
}
static int dummy_xDelete(sqlite3_vfs* vfs, const char* zName, int syncDir) {
    (void)vfs; (void)zName; (void)syncDir; return SQLITE_OK;
}
static int dummy_xAccess(sqlite3_vfs* vfs, const char* zName, int flags, int* pResOut) {
    (void)vfs; (void)zName; (void)flags; if (pResOut) *pResOut = 0; return SQLITE_OK;
}
static int dummy_xFullPathname(sqlite3_vfs* vfs, const char* zName, int nOut, char* zOut) {
    (void)vfs; (void)zName; if (nOut > 0 && zOut) zOut[0] = '\0'; return SQLITE_OK;
}

// Test 1: If quota is already initialized, sqlite3_quota_initialize should return SQLITE_MISUSE
// This validates the early-exit guard on gQuota.isInitialized.
void test_initialize_already_initialized_should_return_misuse() {
    // Access to gQuota to simulate the "already initialized" state.
    // The test framework's test_quota.h is expected to expose gQuota.
    gQuota.isInitialized = 1; // Simulate an existing initialization.

    int rc = sqlite3_quota_initialize(nullptr, 0);

    // SQLITE_MISUSE indicates misuse of the quota initialization API when already initialized.
    CHECK(rc == SQLITE_MISUSE, "Expected SQLITE_MISUSE when quota already initialized");
}

// Test 2: If the target VFS name is not registered, sqlite3_vfs_find should return NULL and
// sqlite3_quota_initialize should yield SQLITE_ERROR.
void test_initialize_with_missing_vfs_should_return_error() {
    // Reset state to ensure a clean environment for this test.
    gQuota.isInitialized = 0;
    gQuota.pMutex = nullptr;
    gQuota.pOrigVfs = nullptr;
    // Use a deliberately unlikely VFS name to ensure it is not registered.
    const char* missingVfsName = "vfs_that_does_not_exist_abcdef";

    int rc = sqlite3_quota_initialize(missingVfsName, 0);

    CHECK(rc == SQLITE_ERROR, "Expected SQLITE_ERROR when original VFS is not found");
}

// Test 3: Successful initialization when a fake VFS is registered.
// This validates that the function copies the VFS, overrides methods properly,
// and marks the quota system as initialized.
void test_initialize_with_registered_mock_vfs_should_succeed() {
    // Prepare a minimal dummy VFS and register it so sqlite3_vfs_find can return it.
    // Reset quota state first.
    gQuota.isInitialized = 0;
    gQuota.pMutex = nullptr;
    gQuota.pOrigVfs = nullptr;

    // Initialize the dummy VFS structure (compatible with sqlite3_vfs expectations)
    // Note: Only a subset of fields are explicitly set; remaining fields default to zero.
    dummy_vfs = sqlite3_vfs(); // Zero-initialize (requires C++11; fallback if not available)
    // If sqlite3_vfs() is not available, fallback to zero-init
    // The following two lines ensure a well-defined object in either case.
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-length-array"
#endif
    // Explicit field setup
    dummy_vfs.iVersion = 2;
    dummy_vfs.zName = "dummy_quota_vfs2";
    dummy_vfs.xOpen = dummy_xOpen;
    dummy_vfs.xDelete = dummy_xDelete;
    dummy_vfs.xAccess = dummy_xAccess;
    dummy_vfs.xFullPathname = dummy_xFullPathname;

#ifdef __clang__
#pragma clang diagnostic pop
#endif

    // Register the dummy VFS with SQLite. We don't make it the default VFS here (makeDefault = 0).
    int rc_vfs = sqlite3_vfs_register(&dummy_vfs, 0);
    // If registration fails for some reason, the test can still report a failure.
    CHECK(rc_vfs == SQLITE_OK, "Failed to register dummy VFS for quota initialization test");

    // Now perform the quota initialization using the dummy VFS's name.
    int rc = sqlite3_quota_initialize("dummy_quota_vfs2", 0);

    // Expect success
    CHECK(rc == SQLITE_OK, "Expected SQLITE_OK from quota initialization with mock VFS");

    // Additional internal state checks (as exposed by gQuota) to validate side effects.
    CHECK(gQuota.pOrigVfs == &dummy_vfs, "pOrigVfs should point to the registered dummy VFS");
    CHECK(gQuota.isInitialized == 1, "Quota should be marked as initialized after successful init");
    // sThisVfs should be a copy of pOrigVfs with overrides applied
    if (gQuota.sThisVfs.zName) {
        CHECK(std::strcmp(gQuota.sThisVfs.zName, "quota") == 0,
              "sThisVfs.zName should be set to 'quota'");
    } else {
        CHECK(false, "sThisVfs.zName is null after initialization");
    }
    // Versioning for IoMethods (V1 -> V2)
    CHECK(gQuota.sIoMethodsV2.iVersion == 2, "sIoMethodsV2.iVersion should be 2 after initialization");
}

// Entry point for the test suite
int main() {
    // Clear counters
    g_pass = g_fail = 0;

    // Run tests
    test_initialize_already_initialized_should_return_misuse();
    test_initialize_with_missing_vfs_should_return_error();
    test_initialize_with_registered_mock_vfs_should_succeed();

    // Summary
    std::cout << "Test Summary: "
              << g_pass << " passed, "
              << g_fail << " failed." << std::endl;

    // Return non-zero if any test failed
    return (g_fail == 0) ? 0 : 1;
}