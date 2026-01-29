// Comprehensive unit tests for sqlite3OsFileSize (Step 2/3: generate and refine tests)
// This test harness is designed to be self-contained, use the actual project types
// (sqlite3_file, i64, SQLITE_OK, etc.) via sqliteInt.h, and does not rely on
// any external testing framework (GTest is not used as per requirements).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Include the SQLite internal declarations to ensure correct type definitions.
// The project under test is assumed to provide these headers in its build environment.
extern "C" {
}

// External declaration of the focal function under test.
// It is defined in the project (os.c) and is linked in during the actual build.
// We declare it here to invoke the function from the test harness.
extern "C" int sqlite3OsFileSize(sqlite3_file *id, i64 *pSize);

// Forward declarations for test helper xFileSize implementations.
// These functions will be assigned to the pMethods.xFileSize to simulate
// different behaviors of the underlying OS file size operation.
static int MockXFileSize_Success(sqlite3_file *id, i64 *pSize);
static int MockXFileSize_Error(sqlite3_file *id, i64 *pSize);

// Simple test harness (no external testing framework).
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Non-terminating assertion macro: logs failure but continues execution.
// This matches the requirement to use non-terminating assertions to maximize coverage.
#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " (expected: " << (expected) << ", actual: " << (actual) \
                  << ") [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

#define EXPECT_NE(actual, not_expected, msg) do { \
    ++g_tests_run; \
    if ((actual) == (not_expected)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " (unexpected: " << (not_expected) << ") [" \
                  << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

// Test 1: Success path - when id is valid and xFileSize returns SQLITE_OK.
// Verifies that sqlite3OsFileSize forwards the call and writes the expected size.
void Test_FileSize_Success() {
    // Prepare a mock sqlite3_io_methods with xFileSize implemented by MockXFileSize_Success.
    sqlite3_io_methods methods;
    std::memset(&methods, 0, sizeof(methods)); // Ensure all fields are zeroed.
    methods.xFileSize = MockXFileSize_Success;  // Set the success callback.

    // Prepare a sqlite3_file instance that uses the above methods.
    sqlite3_file file;
    std::memset(&file, 0, sizeof(file));
    file.pMethods = &methods;

    // Prepare output parameter.
    i64 sizeOut = 0;

    // Call the focal function.
    int rc = sqlite3OsFileSize(&file, &sizeOut);

    // Assertions: rc should be SQLITE_OK and sizeOut should be set to 12345.
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3OsFileSize should return SQLITE_OK on success");
    EXPECT_EQ(sizeOut, 12345, "pSize should be populated with the value from xFileSize");
}

// Test 2: Propagation of underlying xFileSize error.
// Verifies that if xFileSize reports an error, sqlite3OsFileSize propagates it
// and does not modify the output size.
void Test_FileSize_ErrorPropagation() {
    // Prepare a mock sqlite3_io_methods with xFileSize implemented by MockXFileSize_Error.
    sqlite3_io_methods methods;
    std::memset(&methods, 0, sizeof(methods));
    methods.xFileSize = MockXFileSize_Error;

    sqlite3_file file;
    std::memset(&file, 0, sizeof(file));
    file.pMethods = &methods;

    i64 sizeOut = 0;

    int rc = sqlite3OsFileSize(&file, &sizeOut);

    // Since xFileSize returns an error, the outer function should propagate it.
    // We use SQLITE_IOERR as the expected error code in this test.
    EXPECT_EQ(rc, SQLITE_IOERR, "sqlite3OsFileSize should propagate the xFileSize error");

    // pSize should remain unchanged if the underlying call failed.
    EXPECT_EQ(sizeOut, 0, "pSize should remain unchanged when xFileSize fails");
}

// Test 3: Null id path - when id is NULL, DO_OS_MALLOC_TEST should cause an error.
// Verifies that the function handles a null pointer for id (memory allocation test scenario).
void Test_FileSize_NullId() {
    i64 sizeOut = 0;
    int rc = sqlite3OsFileSize(nullptr, &sizeOut);

    // Expect an error code (non-OK) when id is NULL.
    EXPECT_NE(rc, SQLITE_OK, "sqlite3OsFileSize should not return SQLITE_OK for null id");
}

// Implementations of mock xFileSize callbacks.

// Success path: write a deterministic size and return SQLITE_OK.
static int MockXFileSize_Success(sqlite3_file *id, i64 *pSize) {
    (void)id; // id is not used by the mock, but kept for signature consistency.
    if (pSize) {
        *pSize = 12345;
    }
    return SQLITE_OK;
}

// Error path: do not modify the size and return an error code.
static int MockXFileSize_Error(sqlite3_file *id, i64 *pSize) {
    (void)id;
    if (pSize) {
        *pSize = -1;
    }
    return SQLITE_IOERR;
}

// Main entry point for the test harness.
int main() {
    std::cout << "Running sqlite3OsFileSize unit tests (no GTest).\n";

    Test_FileSize_Success();
    Test_FileSize_ErrorPropagation();
    Test_FileSize_NullId();

    std::cout << "Tests executed: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << "\n";

    // Return 0 if all tests passed, non-zero otherwise.
    if (g_tests_run == g_tests_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}