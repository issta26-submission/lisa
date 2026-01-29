// This test suite targets the focal method:
// sqlite3_vfs *sqlite3PagerVfs(Pager *pPager) which simply returns the pVfs member
// of the given Pager object.
//
// Plan:
// - Provide a minimal, self-contained Pager type that mirrors the layout of the
//   real Pager's pVfs member (assumed to be the first field).
// - Declare the C function sqlite3PagerVfs with C linkage.
// - Create several scenarios to exercise true/false branches of the dependent
//   component (presence or absence of pVfs).
// - Use non-terminating, lightweight assertions to maximize code coverage.
// - Implement a tiny test harness without any external testing framework (no GTest).

#include <iostream>
#include <wal.h>
#include <sqliteInt.h>


// Forward declare the C type used by the production code to avoid pulling in
// possibly heavy internal SQLite headers.
struct sqlite3_vfs;

// Forward declare the Pager type as used by the production function.
// We define a minimal layout matching the likely real layout: pVfs as the first member.
struct Pager;

// Provide the exact function prototype as seen in the production code.
// We use extern "C" to prevent C++ name mangling, enabling correct linkage.
extern "C" sqlite3_vfs *sqlite3PagerVfs(Pager *pPager);

// Minimal Pager layout for testing: first field is sqlite3_vfs* pVfs
struct Pager {
    sqlite3_vfs *pVfs;
};

// Simple, non-terminating assertion helpers
static int g_testFailures = 0;

static void expect_ptr_eq(const void *a, const void *b, const char *testName) {
    if (a == b) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cerr << "[FAIL] " << testName
                  << " - expected pointer " << b
                  << " but got " << a << "\n";
        ++g_testFailures;
    }
}

// Test 1: Verify that sqlite3PagerVfs returns the exact VFS pointer assigned to pVfs.
static void test_PagerVfs_ReturnsAssignedVfs() {
    // Explanation:
    // - Create a dummy VFS object and assign its address to Pager.pVfs.
    // - Call sqlite3PagerVfs and ensure we receive the same pointer back.
    // This exercises the "true" path where pPager->pVfs is non-null.
    sqlite3_vfs vfsInstance;
    Pager p;
    p.pVfs = &vfsInstance;

    sqlite3_vfs *ret = sqlite3PagerVfs(&p);

    expect_ptr_eq((void*)ret, (void*)&vfsInstance,
                  "test_PagerVfs_ReturnsAssignedVfs: should return the assigned VFS pointer");
}

// Test 2: Verify that sqlite3PagerVfs returns null when the Pager's pVfs is null.
static void test_PagerVfs_ReturnsNullWhenVfsNull() {
    // Explanation:
    // - Set pVfs to null and verify the function returns null.
    // This exercises the "false" path where pPager->pVfs is NULL.
    Pager p;
    p.pVfs = nullptr;

    sqlite3_vfs *ret = sqlite3PagerVfs(&p);

    expect_ptr_eq((void*)ret, nullptr,
                  "test_PagerVfs_ReturnsNullWhenVfsNull: should return NULL when pVfs is NULL");
}

// Test 3: Verify that multiple Pager instances return their respective VFS pointers.
static void test_PagerVfs_AcrossMultiplePagers() {
    // Explanation:
    // - Create two distinct VFS instances and assign them to two different Pagers.
    // - Each call should return the corresponding VFS pointer, ensuring no cross-talk.
    sqlite3_vfs vfsA, vfsB;
    Pager p1, p2;
    p1.pVfs = &vfsA;
    p2.pVfs = &vfsB;

    sqlite3_vfs *ret1 = sqlite3PagerVfs(&p1);
    sqlite3_vfs *ret2 = sqlite3PagerVfs(&p2);

    expect_ptr_eq((void*)ret1, (void*)&vfsA,
                  "test_PagerVfs_AcrossMultiplePagers: pager1 should return its own VFS");
    expect_ptr_eq((void*)ret2, (void*)&vfsB,
                  "test_PagerVfs_AcrossMultiplePagers: pager2 should return its own VFS");
}

// Tiny helper to run all tests and report final results
static void run_all_tests() {
    std::cout << "Starting sqlite3PagerVfs unit tests...\n";
    test_PagerVfs_ReturnsAssignedVfs();
    test_PagerVfs_ReturnsNullWhenVfsNull();
    test_PagerVfs_AcrossMultiplePagers();

    if (g_testFailures == 0) {
        std::cout << "[ALL TESTS PASSED] sqlite3PagerVfs\n";
    } else {
        std::cerr << "[TESTS FAILED] sqlite3PagerVfs: " << g_testFailures
                  << " failure(s) detected\n";
    }
}

// Entry point for the standalone test binary
int main() {
    run_all_tests();
    return g_testFailures;
}