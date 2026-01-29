// Step 1 (Program Understanding) - Candidate Keywords:
// - lsm_env: the environment structure containing a pointer to the close function (xClose).
// - lsm_file: the file handle/opaque type passed to xClose.
// - xClose: function pointer inside lsm_env used by lsmEnvClose to perform the actual close.
// - IOERR_WRAPPER: wrapper function used inside lsmEnvClose (the test assumes it behaves as an identity wrapper for the purpose of verification).
// - lsmEnvClose: the focal method under test which delegates to pEnv->xClose(pFile) and wraps the return with IOERR_WRAPPER.
// - The unit tests aim to verify that lsmEnvClose forwards the pFile argument correctly and propagates the return value from xClose through IOERR_WRAPPER.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Step 2 (Unit Test Generation) - Provide a C/C++ level test harness for lsmEnvClose.
// We declare the C-compatible interfaces expected by lsmEnvClose. The actual implementation
// of lsmEnvClose resides in the C source (lsm_file.c) and uses IOERR_WRAPPER internally.
// To keep tests self-contained, we rely on the fact that IOERR_WRAPPER is the identity transform
// in our test scenario (i.e., IOERR_WRAPPER(rc) == rc). If the real project changes IOERR_WRAPPER,
// the tests can be adjusted accordingly to reflect the actual wrapping behavior.

extern "C" {

// Forward declare the opaque types as the real project would expose them.
// In the real codebase these definitions come from the relevant headers, but for the
// purposes of unit testing we only need the interface shape.
typedef struct lsm_file lsm_file;

// The lsm_env structure in the real code contains a function pointer xClose with the
// signature int xClose(lsm_file *pFile). We mirror that here to allow test composition.
typedef struct lsm_env {
    int (*xClose)(lsm_file *pFile);
} lsm_env;

// The focal function under test. It is defined in lsm_file.c and linked in the build.
int lsmEnvClose(lsm_env *pEnv, lsm_file *pFile);

} // extern "C"

//
// Global test hooks to observe behavior of mock xClose implementation.
//
static lsm_file g_testFile1;
static lsm_file g_testFile2;
static lsm_file *g_lastClosedFile = nullptr; // Observes the argument passed to xClose
static int g_mockReturnValue = 0;           // What the mock xClose returns

// Mock implementation for the xClose function. It records the pointer it receives and
// returns a value controlled by g_mockReturnValue.
static int mock_xClose(lsm_file *pFile) {
    g_lastClosedFile = pFile;
    return g_mockReturnValue;
}

// Simple non-terminating assertion helper to report failures without aborting tests.
#define EXPECT_EQ(expected, actual, msg)                                            \
    do {                                                                               \
        if ((expected) != (actual)) {                                                \
            std::cerr << "ASSERT FAILED: " << (msg) << " | Expected: "             \
                      << (expected) << " | Actual: " << (actual) << std::endl;      \
            ++g_total_failures;                                                    \
        } else {                                                                     \
            std::cout << "ASSERT PASSED: " << (msg) << std::endl;                   \
        }                                                                            \
    } while (0)

static int g_total_failures = 0;

// Step 3 (Test Case Refinement) - Individual test cases with explanatory comments.
// The tests intentionally cover multiple scenarios to exercise the forwarding behavior
// and the value propagation through the IOERR_WRAPPER (assumed identity in this test).
//
// Test 1: xClose returns 0; ensure lsmEnvClose forwards the same return value and passes the
//         correct file pointer to xClose.
static void test_lsmEnvClose_forwards_rc_zero_and_file_ptr() {
    // Arrange
    g_lastClosedFile = nullptr;
    g_mockReturnValue = 0;

    lsm_env env;
    env.xClose = mock_xClose;

    // Use a concrete file object
    lsm_file file;
    // Act
    int rc = lsmEnvClose(&env, &file);

    // Assert
    EXPECT_EQ(0, rc, "lsmEnvClose should return 0 when xClose returns 0");
    EXPECT_EQ(&file, g_lastClosedFile, "lsmEnvClose should pass the original file pointer to xClose");
}

// Test 2: xClose returns a negative error code; ensure lsmEnvClose forwards it correctly.
static void test_lsmEnvClose_forwards_negative_rc_and_file_ptr() {
    // Arrange
    g_lastClosedFile = nullptr;
    g_mockReturnValue = -5;

    lsm_env env;
    env.xClose = mock_xClose;

    lsm_file file;
    // Act
    int rc = lsmEnvClose(&env, &file);

    // Assert
    EXPECT_EQ(-5, rc, "lsmEnvClose should bubble up the negative return value from xClose");
    EXPECT_EQ(&file, g_lastClosedFile, "lsmEnvClose should pass the file pointer to xClose");
}

// Test 3: pFile is NULL; ensure xClose is still called with a NULL pointer and wrapper returns value.
static void test_lsmEnvClose_with_null_pFile() {
    // Arrange
    g_lastClosedFile = nullptr;
    g_mockReturnValue = 1; // non-zero to verify propagation

    lsm_env env;
    env.xClose = mock_xClose;

    // Act
    int rc = lsmEnvClose(&env, nullptr);

    // Assert
    EXPECT_EQ(1, rc, "lsmEnvClose should propagate value when pFile is NULL and xClose returns 1");
    EXPECT_EQ(nullptr, g_lastClosedFile, "lsmEnvClose should pass NULL when pFile is NULL");
}

// Run all tests and print a summary
static void run_all_tests() {
    std::cout << "Running tests for lsmEnvClose (lsm_file.c)..." << std::endl;

    test_lsmEnvClose_forwards_rc_zero_and_file_ptr();
    test_lsmEnvClose_forwards_negative_rc_and_file_ptr();
    test_lsmEnvClose_with_null_pFile();

    if (g_total_failures == 0) {
        std::cout << "ALL TESTS PASSED." << std::endl;
    } else {
        std::cout << "TOTAL FAILURES: " << g_total_failures << std::endl;
    }
}

// Provide a portable main to execute tests without a dedicated test framework.
// This adheres to the constraint of not using GTest/GMock and keeps tests executable
// from a standard C++ runtime entry point.
int main() {
    run_all_tests();
    return g_total_failures;
}