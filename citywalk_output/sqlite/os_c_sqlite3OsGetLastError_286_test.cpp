// Comprehensive unit test suite for sqlite3OsGetLastError using a lightweight harness (no GTest).
// This test targets the focal method:
//   int sqlite3OsGetLastError(sqlite3_vfs *pVfs) {
//     return pVfs->xGetLastError ? pVfs->xGetLastError(pVfs, 0, 0) : 0;
//   }
//
// We rely on the public sqlite3_vfs structure from sqlite3.h to craft test scenarios.
// The tests cover:
// - true branch: when xGetLastError is non-NULL.
// - false branch: when xGetLastError is NULL.
// - multiple VFS instances exercising identity-sensitive dispatch via xGetLastError.
//
// Note: This test uses a non-terminating assertion style (does not abort on failure) to maximize
// code coverage, per the provided domain knowledge.

#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Import the official sqlite3 public API definitions to get sqlite3_vfs and the target function.
// It is assumed the environment provides sqlite3.h and the library implementing sqlite3OsGetLastError.

// Ensure we can call the C function from C++ code without name mangling issues.
extern "C" int sqlite3OsGetLastError(sqlite3_vfs *pVfs);

// Simple non-terminating assertion helper: prints result and increments global counter on failure.
static int g_testFailures = 0;

static void expect_eq(const char* testName, int expected, int actual) {
    if (expected == actual) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        ++g_testFailures;
        std::cerr << "[FAIL] " << testName
                  << " | expected: " << expected
                  << ", actual: " << actual << "\n";
    }
}

// Test helper functions to simulate different xGetLastError behaviors.

// 1) A deterministic non-NULL xGetLastError that always returns a fixed value.
static int vfsGetLastErrorFixed(sqlite3_vfs *pVfs, int, const char*) {
    (void)pVfs; // unused in this test
    return 123;  // arbitrary fixed value to validate the call path
}

// 2) A selector-based function to differentiate between multiple VFS instances by pointer identity.
static sqlite3_vfs g_vfs1, g_vfs2; // global VFS instances used for identity checks

static int vfsGetLastErrorSelector(sqlite3_vfs *pVfs, int, const char*) {
    if (pVfs == &g_vfs1) return 101;
    if (pVfs == &g_vfs2) return 202;
    return -999; // should not be hit in the tests
}

// Entry point for tests
int main() {
    // Test 1: true branch - xGetLastError is NULL; expect 0
    {
        // Arrange
        sqlite3_vfs vfs;
        std::memset(&vfs, 0, sizeof(vfs));
        vfs.xGetLastError = nullptr; // simulate absence of error reporting

        // Act
        int result = sqlite3OsGetLastError(&vfs);

        // Assert (non-terminating)
        expect_eq("GetLastError_NullCallback_Returns0", 0, result);
    }

    // Test 2: true branch - non-NULL xGetLastError returning a fixed value
    {
        // Arrange
        sqlite3_vfs vfs;
        std::memset(&vfs, 0, sizeof(vfs));
        vfs.xGetLastError = &vfsGetLastErrorFixed; // provide custom behavior

        // Act
        int result = sqlite3OsGetLastError(&vfs);

        // Assert
        expect_eq("GetLastError_NonNullCallback_ReturnsFixedValue", 123, result);
    }

    // Test 3: multi-VM scenario - verify identity-sensitive dispatch via xGetLastError
    {
        // Prepare two distinct VFS objects whose addresses are distinct
        std::memset(&g_vfs1, 0, sizeof(g_vfs1));
        std::memset(&g_vfs2, 0, sizeof(g_vfs2));
        g_vfs1.xGetLastError = vfsGetLastErrorSelector;
        g_vfs2.xGetLastError = vfsGetLastErrorSelector;

        // Act for VFS1
        int res1 = sqlite3OsGetLastError(&g_vfs1);
        // Act for VFS2
        int res2 = sqlite3OsGetLastError(&g_vfs2);

        // Assert for VFS1 identity
        expect_eq("GetLastError_MultiVFS_VFS1", 101, res1);
        // Assert for VFS2 identity
        expect_eq("GetLastError_MultiVFS_VFS2", 202, res2);
    }

    // Summary
    if (g_testFailures == 0) {
        std::cout << "[OVERALL] All tests passed.\n";
    } else {
        std::cerr << "[OVERALL] " << g_testFailures << " test(s) failed.\n";
    }

    return g_testFailures != 0 ? 1 : 0;
}

/*
Notes for maintainers:
- The tests rely on the public sqlite3.h definitions to ensure compatibility with sqlite3_vfs layout.
- We intentionally avoid terminating assertions to maximize code coverage (per domain guidance).
- The tests cover both branches of the conditional in sqlite3OsGetLastError and ensure identity-based dispatch
  across multiple VFS instances, aligning with the "Candidate Keywords" derived from the focal method and its dependencies.
- This test file should be compiled and linked against the same build as the code under test (os.c) and the
  sqlite3 library that provides sqlite3OsGetLastError.
*/