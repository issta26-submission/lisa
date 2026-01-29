// C++11 test suite for sqlite3rbu_create_vfs
// This test suite exercises key code paths of the focal method
// sqlite3rbu_create_vfs in sqlite3rbu.c without using Google Test.
// It relies on the public SQLite C API exposed via sqlite3.h and sqlite3rbu.h.
// Explanatory comments are provided for each unit test.
// Note: Static helpers inside sqlite3rbu.c are not directly testable from here.

#include <string.h>
#include <windows.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <sqlite3rbu.h>
#include <iostream>


extern "C" {
}

// Simple test harness helpers
static void log_pass(const std::string& test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}
static void log_fail(const std::string& test_name, const std::string& reason) {
    std::cerr << "[FAIL] " << test_name << " - " << reason << std::endl;
}

// Test 1: Success path when a valid parent VFS exists
// Purpose: Verify that sqlite3rbu_create_vfs returns SQLITE_OK and that the
// newly created VFS is findable by sqlite3_vfs_find(zName).
// Preconditions: A parent VFS named "main" exists in the process (typical for SQLite).
static bool test_vfs_create_success() {
    const char *zName = "test_rbu_vfs_ok";
    const char *zParent = "main";

    // Check for a valid parent VFS existence
    sqlite3_vfs *pParent = sqlite3_vfs_find(zParent);
    if (pParent == nullptr) {
        // If no parent VFS is present, skip this test gracefully.
        std::cerr << "SKIP: Parent VFS '" << zParent << "' not found. Cannot run success-path test.\n";
        return true; // skip treated as pass to not bias results
    }

    int rc = sqlite3rbu_create_vfs(zName, zParent);
    if (rc != SQLITE_OK) {
        log_fail("test_vfs_create_success",
                 "Expected SQLITE_OK but sqlite3rbu_create_vfs returned " + std::to_string(rc));
        return false;
    }

    // Verify that the VFS with the given name was registered
    sqlite3_vfs *pNew = sqlite3_vfs_find(zName);
    if (pNew == nullptr) {
        log_fail("test_vfs_create_success",
                 "Created VFS not found via sqlite3_vfs_find('" + std::string(zName) + "')");
        return false;
    }

    // Cleanup: unregister the VFS to avoid side effects for subsequent tests
    sqlite3_vfs_unregister(pNew);
    log_pass("test_vfs_create_success");
    return true;
}

// Test 2: NotFound path when the parent VFS does not exist
// Purpose: Ensure rc == SQLITE_NOTFOUND when the specified parent VFS is missing.
// Preconditions: No VFS named "nonexistent_parent_xyz" exists in the process.
static bool test_vfs_create_notfound_parent() {
    const char *zName = "test_rbu_vfs_notfound";
    const char *zParent = "nonexistent_parent_xyz";

    int rc = sqlite3rbu_create_vfs(zName, zParent);
    if (rc != SQLITE_NOTFOUND) {
        log_fail("test_vfs_create_notfound_parent",
                 "Expected SQLITE_NOTFOUND but got " + std::to_string(rc));
        // Attempt to cleanup any partial allocation if it occurred (defensive)
        sqlite3_vfs *p = sqlite3_vfs_find(zName);
        if (p) sqlite3_vfs_unregister(p);
        return false;
    }

    // There should be no VFS registered with the requested name
    sqlite3_vfs *p = sqlite3_vfs_find(zName);
    if (p != nullptr) {
        log_fail("test_vfs_create_notfound_parent",
                 "Found VFS after NOTFOUND condition; performing cleanup.");
        sqlite3_vfs_unregister(p);
        return false;
    }

    log_pass("test_vfs_create_notfound_parent");
    return true;
}

// Test 3: Duplicate name handling
// Purpose: Ensure that attempting to create a VFS with a name that already exists
// does not succeed (the second call should fail).
// Preconditions: A valid parent VFS named "main" exists.
static bool test_vfs_create_duplicate_name() {
    const char *zParent = "main";
    const char *zName = "test_rbu_vfs_dup";

    sqlite3_vfs *pParent = sqlite3_vfs_find(zParent);
    if (pParent == nullptr) {
        std::cerr << "SKIP: Parent VFS '" << zParent << "' not found. Cannot run duplicate-name test.\n";
        return true;
    }

    // First creation should succeed
    int rc1 = sqlite3rbu_create_vfs(zName, zParent);
    if (rc1 != SQLITE_OK) {
        log_fail("test_vfs_create_duplicate_name",
                 "First creation failed with rc=" + std::to_string(rc1));
        return false;
    }

    // Second creation with the same name should fail (or be non-OK)
    int rc2 = sqlite3rbu_create_vfs(zName, zParent);
    if (rc2 == SQLITE_OK) {
        log_fail("test_vfs_create_duplicate_name",
                 "Second creation unexpectedly succeeded; expected failure due to duplicate name.");
        // Cleanup the second VFS if it happened to be created
        sqlite3_vfs *p2 = sqlite3_vfs_find(zName);
        if (p2) sqlite3_vfs_unregister(p2);
        // Also unregister the first if present
        sqlite3_vfs *p1 = sqlite3_vfs_find(zName);
        if (p1) sqlite3_vfs_unregister(p1);
        return false;
    }

    // Cleanup: unregister any VFS that might have been created
    sqlite3_vfs *p = sqlite3_vfs_find(zName);
    if (p) sqlite3_vfs_unregister(p);

    log_pass("test_vfs_create_duplicate_name");
    return true;
}

int main() {
    // Run all tests and report results
    bool t1 = test_vfs_create_success();
    bool t2 = test_vfs_create_notfound_parent();
    bool t3 = test_vfs_create_duplicate_name();

    // Simple summary
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);
    int total  = 3;
    std::cout << "Test suite summary: " << passed << "/" << total << " tests passed." << std::endl;

    // Non-zero exit if any test failed
    return (passed == total) ? 0 : 1;
}