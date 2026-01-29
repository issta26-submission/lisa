// Test suite for sqlite3OsDlError using real project types (sqliteInt.h) when available.
// This test does not use GoogleTest. It relies on the actual sqlite3_vfs type defined by
// the project and the focal function sqlite3OsDlError which forwards to pVfs->xDlError.
//
// Build notes:
// - Ensure this file is compiled together with the project's os.c (or a compatible object)
//   so that sqlite3OsDlError is linked.
// - Include paths should allow "sqliteInt.h" to be found (as per the provided dependencies).
// - We use a small, non-terminating test harness style (EXPECT-like macros) to collect
//   and report results without terminating early.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <climits>


// Import the project's internal definitions to obtain the true sqlite3_vfs type.
// This header normally provides the exact layout and the xDlError member, among others.

// Forward declaration: sqlite3OsDlError is defined in os.c, and is linked in the test binary.
// The header usage ensures the correct signature is observed.
extern "C" void sqlite3OsDlError(sqlite3_vfs *pVfs, int nByte, char *zBufOut);

// Global capture for the mock xDlError invocation.
static sqlite3_vfs g_mockVfs;              // The mocked VFS instance used in tests
static sqlite3_vfs *g_invokedVfs = nullptr; // Captured pVfs passed to xDlError
static int g_invokedNByte = 0;             // Captured nByte
static char *g_invokedZBufOut = nullptr;   // Captured zBufOut

// The test implementation of xDlError to capture parameters when sqlite3OsDlError is invoked.
extern "C" void test_xDlError(sqlite3_vfs *pVfs, int nByte, char *zBufOut) {
    g_invokedVfs = pVfs;
    g_invokedNByte = nByte;
    g_invokedZBufOut = zBufOut;
}

// Simple non-terminating assertion helper
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, desc) do {                                       \
    ++g_total;                                                               \
    if (!(cond)) {                                                           \
        ++g_failed;                                                          \
        std::cerr << "EXPECT_FAIL: " << desc << std::endl;                 \
    }                                                                        \
} while(0)

static void run_single_test_case_setup() {
    // Reset capture state before each test case
    g_invokedVfs = nullptr;
    g_invokedNByte = 0;
    g_invokedZBufOut = nullptr;
}

// Entry point for running tests (called from main)
static void run_tests() {
    // Prepare a mock VFS and assign the test xDlError function
    // Initialize the mockVfs structure. The rest of its fields are not accessed by
    // sqlite3OsDlError; only xDlError is required for this test.
    // The sqlite3_vfs struct is typically large; zero initialization is safe in this context.
    // We rely on the actual header to define the correct type.
    // Reset to a clean state
    std::memset(&g_mockVfs, 0, sizeof(g_mockVfs));

    // Attach the test implementation to the xDlError function pointer
    // Note: We cast to the appropriate type via assignment; the exact signature is resolved by the header.
    // Use C linkage for the test function to avoid C++ name mangling issues.
    // (Earlier we declared test_xDlError with extern "C")
    // Set the function pointer if the member exists; in practice, sqliteInt.h defines it.
    g_mockVfs.xDlError = test_xDlError;

    // Test Case 1: Normal forwarding with non-null buffer
    run_single_test_case_setup();
    char buf1[8] = "libtest";
    sqlite3OsDlError(&g_mockVfs, 42, buf1);

    EXPECT_TRUE(g_invokedVfs == &g_mockVfs, "Test1: pVfs passed to xDlError should be the same mockVfs");
    EXPECT_TRUE(g_invokedNByte == 42, "Test1: nByte forwarded to xDlError");
    EXPECT_TRUE(g_invokedZBufOut == buf1, "Test1: zBufOut pointer forwarded to xDlError");

    // Test Case 2: zBufOut is nullptr
    run_single_test_case_setup();
    sqlite3OsDlError(&g_mockVfs, 0, nullptr);

    EXPECT_TRUE(g_invokedVfs == &g_mockVfs, "Test2: pVfs should still be the mockVfs when zBufOut is NULL");
    EXPECT_TRUE(g_invokedNByte == 0, "Test2: nByte forwarded correctly when zBufOut is NULL");
    EXPECT_TRUE(g_invokedZBufOut == nullptr, "Test2: zBufOut should be forwarded (NULL) when input is NULL");

    // Test Case 3: Negative nByte value forwarding
    run_single_test_case_setup();
    char buf3[4] = "abc";
    sqlite3OsDlError(&g_mockVfs, -1, buf3);

    EXPECT_TRUE(g_invokedVfs == &g_mockVfs, "Test3: pVfs forwarded with negative nByte");
    EXPECT_TRUE(g_invokedNByte == -1, "Test3: negative nByte forwarded correctly");
    EXPECT_TRUE(g_invokedZBufOut == buf3, "Test3: zBufOut forwarded correctly for negative nByte");

    // Summary
    std::cout << "Total tests run: " << g_total << ", Failures: " << g_failed << std::endl;
}

int main() {
    // Run the test suite for sqlite3OsDlError
    run_tests();

    // Return non-zero if there were failures to aid CI systems
    return (g_failed > 0) ? 1 : 0;
}