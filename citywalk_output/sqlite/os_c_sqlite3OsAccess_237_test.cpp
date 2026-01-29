// Unit test suite for sqlite3OsAccess (C function) using C++11 (no GoogleTest).
// This test suite is designed to be compiled alongside the provided C sources
// (e.g., os.c) in a typical SQLite-like project. It mocks the sqlite3_vfs.xAccess
// callback and verifies that sqlite3OsAccess delegates correctly and forwards
// parameters and return values as expected.
//
// Notes:
// - This test uses a minimal, compatible definition of sqlite3_vfs to model the C
//   structure expected by sqlite3OsAccess. It relies on the xAccess function being
//   the first member to simplify layout assumptions (as per common SQLite vfs layouts).
// - The test assumes that linking with the actual sqlite3OsAccess symbol is possible
//   and that the DO_OS_MALLOC_TEST macro used in sqlite3OsAccess either has no
//   side effects for the test or is provided by the real build environment.
// - All tests are designed to be non-terminating (they collect failures and report them
//   at the end, rather than exiting on first failure).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <string>


// Domain knowledge: define a minimal compatible sqlite3_vfs type
// that matches the likely layout used by sqlite3OsAccess (xAccess is the first member).
typedef struct sqlite3_vfs sqlite3_vfs;
typedef int (*sqlite3_xAccess)(sqlite3_vfs*, const char*, int, int*);

struct sqlite3_vfs {
    sqlite3_xAccess xAccess; // minimal dependency required by sqlite3OsAccess
    // other members are not required for this unit test
};

// Forward declaration of the function under test.
// We declare it with C linkage to match the actual implementation in os.c.
extern "C" int sqlite3OsAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut);

// Global variables to capture state inside MockXAccess
static sqlite3_vfs *g_last_vfs = nullptr;
static const char *g_last_path = nullptr;
static int g_last_flags = 0;
static int g_last_pResOut_deref = 0;
static bool g_xAccess_was_called = false;

// Mock implementation of xAccess to verify delegation and side effects.
static int MockXAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut) {
    g_xAccess_was_called = true;
    g_last_vfs = pVfs;
    g_last_path = zPath;
    g_last_flags = flags;
    if (pResOut) {
        // Simulate a typical behavior where xAccess might set *pResOut
        *pResOut = 1234;
        g_last_pResOut_deref = *pResOut;
    } else {
        g_last_pResOut_deref = 0;
    }
    // Return a sentinel value to verify that sqlite3OsAccess forwards the result.
    return 77;
}

// Helper macro for lightweight EXPECT-like assertions that do not terminate tests.
#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "TEST_FAIL: " << msg << std::endl; test_failed = true; } } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "TEST_FAIL: " << msg << " (expected " << (a) << " == " << (b) << ")" << std::endl; test_failed = true; } } while (0)

// Test 1: Basic delegation: ensure sqlite3OsAccess calls xAccess and propagates return value and pResOut.
bool test_sqlite3OsAccess_basic_delegation() {
    bool test_failed = false;

    // Arrange
    static sqlite3_vfs mockVfs;
    mockVfs.xAccess = MockXAccess;

    // Clear captured state
    g_last_vfs = nullptr;
    g_last_path = nullptr;
    g_last_flags = 0;
    g_last_pResOut_deref = 0;
    g_xAccess_was_called = false;

    int ret = 0;
    int resOut = 0;

    // Act
    ret = sqlite3OsAccess(&mockVfs, "/tmp/file.dat", 0x01, &resOut);

    // Assert
    EXPECT_TRUE(g_xAccess_was_called, "xAccess should be called by sqlite3OsAccess");
    EXPECT_EQ(g_last_vfs, (sqlite3_vfs *)&mockVfs, "pVfs passed to xAccess must be the same instance as provided");
    EXPECT_TRUE(g_last_path && std::string(g_last_path) == "/tmp/file.dat",
                "zPath should be forwarded to xAccess");
    EXPECT_EQ(g_last_flags, 0x01, "flags should be forwarded to xAccess");
    EXPECT_EQ(resOut, 1234, "sqlite3OsAccess should pass through pResOut value set by xAccess");
    EXPECT_EQ(ret, 77, "sqlite3OsAccess should return the value produced by xAccess");

    return test_failed;
}

// Test 2: pResOut is NULL: ensure no dereferencing occurs and function still returns the xAccess value.
bool test_sqlite3OsAccess_null_pResOut() {
    bool test_failed = false;

    // Arrange
    static sqlite3_vfs mockVfs;
    mockVfs.xAccess = MockXAccess;

    // Reset and call with NULL pResOut
    g_last_vfs = nullptr;
    g_last_path = nullptr;
    g_last_flags = 0;
    g_last_pResOut_deref = 0;
    g_xAccess_was_called = false;

    int ret = 0;

    // Act
    ret = sqlite3OsAccess(&mockVfs, "path/with/null.out", 2, nullptr);

    // Assert
    EXPECT_TRUE(g_xAccess_was_called, "xAccess should be called even when pResOut is NULL");
    EXPECT_EQ(g_last_vfs, (sqlite3_vfs *)&mockVfs, "pVfs should be passed through when pResOut is NULL");
    EXPECT_TRUE(g_last_path && std::string(g_last_path) == "path/with/null.out",
                "zPath should be forwarded to xAccess");
    EXPECT_EQ(g_last_flags, 2, "flags should be forwarded to xAccess");
    EXPECT_EQ(ret, 77, "sqlite3OsAccess should return the value produced by xAccess");

    return test_failed;
}

// Test 3: Multiple calls with varying inputs to exercise consistency and state updates.
bool test_sqlite3OsAccess_multiple_calls() {
    bool test_failed = false;

    // Prepare first mock
    static sqlite3_vfs mockVfsA;
    mockVfsA.xAccess = MockXAccess;

    // First call
    g_last_vfs = nullptr;
    g_last_path = nullptr;
    g_last_flags = 0;
    g_last_pResOut_deref = 0;
    g_xAccess_was_called = false;

    int ret1 = sqlite3OsAccess(&mockVfsA, "/a/b/c.txt", 0, nullptr);

    EXPECT_TRUE(g_xAccess_was_called, "First call: xAccess should be invoked");
    EXPECT_EQ(ret1, 77, "First call should return xAccess's sentinel value");

    // Prepare second mock (simulate different vfs instance)
    static sqlite3_vfs mockVfsB;
    mockVfsB.xAccess = MockXAccess;

    // Second call with different inputs
    g_last_vfs = nullptr;
    g_last_path = nullptr;
    g_last_flags = 0;
    g_last_pResOut_deref = 0;
    g_xAccess_was_called = false;

    int resOut2 = 0;
    int ret2 = sqlite3OsAccess(&mockVfsB, "another/path.dat", 0xFF, &resOut2);

    EXPECT_TRUE(g_xAccess_was_called, "Second call: xAccess should be invoked");
    EXPECT_EQ(ret2, 77, "Second call should return xAccess's sentinel value");
    EXPECT_EQ(resOut2, 1234, "Second call: pResOut should be set by xAccess");
    EXPECT_EQ(std::string(g_last_path ? g_last_path : ""), "another/path.dat",
              "Second call: zPath should be forwarded to xAccess");

    return test_failed;
}

int main() {
    std::cout << "Running sqlite3OsAccess unit tests (C++11)..." << std::endl;

    bool any_fail = false;

    bool fail1 = test_sqlite3OsAccess_basic_delegation();
    if (fail1) {
        std::cerr << "Test 1 failed." << std::endl;
        any_fail = true;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    bool fail2 = test_sqlite3OsAccess_null_pResOut();
    if (fail2) {
        std::cerr << "Test 2 failed." << std::endl;
        any_fail = true;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    bool fail3 = test_sqlite3OsAccess_multiple_calls();
    if (fail3) {
        std::cerr << "Test 3 failed." << std::endl;
        any_fail = true;
    } else {
        std::cout << "Test 3 passed." << std::endl;
    }

    if (any_fail) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}