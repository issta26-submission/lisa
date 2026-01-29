// Unit test suite for the focal method lsmEnvLock in lsm_file.c
// This test suite is written in C++11 (no GTest). It uses a small, self-contained
// test harness with simple boolean checks and explanatory comments.
// The tests assume the following minimal C interface (mirroring the actual usage):
//   typedef struct lsm_file lsm_file;
//   typedef struct lsm_env { int (*xLock)(lsm_file*, int, int); } lsm_env;
//   int lsmEnvLock(lsm_env *pEnv, lsm_file *pFile, int iLock, int eLock);
//   #define LSM_OK 0
// We implement a minimal compatible version of these types in the test for isolation.

#include <sys/types.h>
#include <lsmInt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Minimal C-compatible declarations to enable testing of lsmEnvLock.
// We assume the actual implementation uses a similar layout for lsm_env with xLock as the first member.
typedef struct lsm_file lsm_file;
typedef struct lsm_env {
    int (*xLock)(lsm_file*, int, int); // function pointer used by lsmEnvLock
} lsm_env;

#define LSM_OK 0

// Forward declaration of the function under test.
// It is declared with C linkage to ensure proper linkage with the C implementation.
extern "C" int lsmEnvLock(lsm_env *pEnv, lsm_file *pFile, int iLock, int eLock);

// ---------------------------------------------------------------------------
// Test scaffolding (very small harness)
// ---------------------------------------------------------------------------

static int tests_run = 0;
static int tests_failed = 0;

static void log_failure(const char* test_name, const char* message) {
    std::cerr << "FAIL: " << test_name << " - " << message << "\n";
}

static bool run_and_report_bool(const char* test_name, bool result) {
    tests_run++;
    if (result) {
        std::cout << "PASS: " << test_name << "\n";
        return true;
    } else {
        log_failure(test_name, "test condition false");
        tests_failed++;
        return false;
    }
}

// ---------------------------------------------------------------------------
// Test 1: Null file pointer should yield LSM_OK regardless of environment.
// This tests the true branch of the if (pFile==0) condition inside lsmEnvLock.
// ---------------------------------------------------------------------------

static bool test_lsmEnvLock_null_file() {
    // When pFile is null, lsmEnvLock should return LSM_OK (0) without dereferencing pEnv.
    int result = lsmEnvLock(nullptr, nullptr, 0, 0);
    bool ok = (result == LSM_OK);
    return ok;
}

// ---------------------------------------------------------------------------
// Test 2: Non-null file pointer should delegate to pEnv->xLock with correct args
// This tests the false branch of the condition and verifies that the callback is invoked
// with the exact same parameters that were passed to lsmEnvLock.
// We provide a mock xLock implementation that records its input and returns a sentinel value.
// ---------------------------------------------------------------------------

static int mock_last_file_addr = 0;
static int mock_last_iLock = 0;
static int mock_last_eLock = 0;
static int mock_xLock(lsm_file *pFile, int iLock, int eLock) {
    // Record the arguments for later verification
    mock_last_file_addr = reinterpret_cast<int>(pFile);
    mock_last_iLock = iLock;
    mock_last_eLock = eLock;
    return 12345; // sentinel return value to verify the delegation
}

static bool test_lsmEnvLock_calls_xLock_and_returns_value() {
    // Prepare environment with a mock xLock
    lsm_env env;
    env.xLock = mock_xLock;

    // Prepare a dummy file object
    lsm_file file;

    // Call the function under test
    int ret = lsmEnvLock(&env, &file, 7, 8);

    // Validate the delegation happened and the return value matches the mock
    bool ok = (ret == 12345) &&
              (mock_last_file_addr == reinterpret_cast<int>(&file)) &&
              (mock_last_iLock == 7) &&
              (mock_last_eLock == 8);

    return ok;
}

// ---------------------------------------------------------------------------
// Main: Run tests and report results
// ---------------------------------------------------------------------------

int main() {
    // Run tests with explanatory comments
    bool t1 = test_lsmEnvLock_null_file();
    run_and_report_bool("test_lsmEnvLock_null_file", t1);

    bool t2 = test_lsmEnvLock_calls_xLock_and_returns_value();
    run_and_report_bool("test_lsmEnvLock_calls_xLock_and_returns_value", t2);

    // Final summary
    std::cout << "Tests run: " << tests_run << ", Passed: " 
              << (tests_run - tests_failed) << ", Failed: " << tests_failed << "\n";

    return (tests_failed == 0) ? 0 : 1;
}

// Explanatory notes for reviewers:
// - Test 1 covers the true branch (pFile == 0) of lsmEnvLock, ensuring LSM_OK is returned
//   without dereferencing pEnv (even if pEnv is null).
// - Test 2 covers the false branch, confirming that lsmEnvLock delegates to the xLock
//   callback of the lsm_env structure and that the exact parameters are propagated.
// - The mock_xLock function records input parameters and returns a known sentinel value
//   to facilitate precise verification.
// - This test suite avoids any private/static member access beyond what is exposed via
//   the public interface under test and remains compatible with C linkage for the
//   function under test.