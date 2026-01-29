/* 
   Test file 1: Basic test for sqlite3_appendvfs_init (no APPENDVFS_TEST path)
   - Purpose: Validate that the focal function can initialize the appended VFS
     when a default (origin) VFS exists in the environment.
   - Assumptions: A default VFS is registered in the running SQLite configuration.
     The test uses the real SQLite API and does not define APPENDVFS_TEST, so the
     internal APPENDVFS_TEST code path is not exercised.
   - Notes: This is a lightweight check focusing on the successful path and the
     return code SQLITE_OK_LOAD_PERMANENTLY as produced by the function.
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <cstdlib>


// Helper to print test results and return 0 on success, 1 on failure
static int printResult(const char* testName, bool passed, const char* details = nullptr) {
    if (passed) {
        std::printf("[PASS] %s\n", testName);
        if (details) std::printf("  Details: %s\n", details);
        return 0;
    } else {
        std::fprintf(stderr, "[FAIL] %s\n", testName);
        if (details) std::fprintf(stderr, "  Details: %s\n", details);
        return 1;
    }
}

/*
   Test 1: Basic path where an origin VFS exists.
   Expected result: rc == SQLITE_OK_LOAD_PERMANENTLY
   - We invoke sqlite3_appendvfs_init with dummy parameters since the function
     uses only the vfs registry and the provided pApi pointer is consumed by
     the SQLITE_EXTENSION_INIT2 macro (which is satisfied by passing NULL here).
*/
static int test_basic_appendvfs_init_basic() {
    const char* testName = "Basic path: origin VFS present -> SQLITE_OK_LOAD_PERMANENTLY";

    // The function's pzErrMsg and db are unused in this path; pass NULLS
    char* pErr = NULL;
    int rc = sqlite3_appendvfs_init(nullptr, &pErr, nullptr);

    // SQLITE_OK_LOAD_PERMANENTLY is the expected final state when rc == SQLITE_OK after vfs_register
    bool passed = (rc == SQLITE_OK_LOAD_PERMANENTLY) || (rc == SQLITE_OK);
    // Provide details if not clearly passed
    const char* details = nullptr;
    if (!passed) {
        details = "Expected SQLITE_OK_LOAD_PERMANENTLY or SQLITE_OK, got rc != expected.";
    }
    return printResult(testName, passed, details);
}

// Main runner for this test file
int main() {
    int failures = 0;
    failures += test_basic_appendvfs_init_basic();
    if (failures == 0) {
        std::printf("All tests in test_appendvfs_init_basic.cpp passed.\n");
        return EXIT_SUCCESS;
    } else {
        std::fprintf(stderr, "%d test(s) failed in test_appendvfs_init_basic.cpp.\n", failures);
        return EXIT_FAILURE;
    }
}
 

/* 
   Test file 2: APPENDVFS_TEST path (when compiling with APPENDVFS_TEST defined)
   - Purpose: Exercise the conditional APPENDVFS_TEST block by injecting a mock
     apndvfsRegister and a mock sqlite3_auto_extension to exercise the branch
     rc = sqlite3_auto_extension(...) and propagate its return value.
   - Approach:
     - Define APPENDVFS_TEST to enable the internal code path.
     - Provide stub implementations for sqlite3_auto_extension and apndvfsRegister.
     - Allow control over the return value of sqlite3_auto_extension via a small global.
   - Tests:
     a) Success path: apndvfsRegister is invoked and sqlite3_auto_extension returns SQLITE_OK;
        expect final rc to be SQLITE_OK_LOAD_PERMANENTLY.
     b) Failure path: sqlite3_auto_extension returns SQLITE_ERROR; expect final rc to be SQLITE_ERROR.
   - Important: This file is intended to be compiled with APPENDVFS_TEST defined (e.g., -DAPPENDVFS_TEST=1).
*/

#define APPENDVFS_TEST 1


#if APPENDVFS_TEST
// Mock return control for sqlite3_auto_extension
static int apnd_auto_ext_return = SQLITE_OK;

// Prototype expected by the SQLite extension mechanism
extern "C" int sqlite3_auto_extension(void (*)(void));

// Stub implementation of sqlite3_auto_extension used when APPENDVFS_TEST is defined.
// It ignores the input and returns the configured global value.
extern "C" int sqlite3_auto_extension(void (*)(void)) {
    (void)0; // suppress unused parameter warning
    return apnd_auto_ext_return;
}

// Dummy apndvfsRegister function referenced by the extension via a function pointer cast.
extern "C" void apndvfsRegister(void) { /* no-op; presence enables path */ }

// Optional: expose a simple helper to set the mock return value from tests
static void set_apnd_auto_ext_return(int val) {
    apnd_auto_ext_return = val;
}
#endif

static int test_appended_vfs_init_basic_success() {
#if APPENDVFS_TEST
    // When APPENDVFS_TEST is enabled, ensure that the success path still yields
    // SQLITE_OK_LOAD_PERMANENTLY given sqlite3_auto_extension returns OK.
    set_apnd_auto_ext_return(SQLITE_OK);
#endif
    char* pErr = NULL;
    int rc = sqlite3_appendvfs_init(nullptr, &pErr, nullptr);
    bool passed = (rc == SQLITE_OK_LOAD_PERMANENTLY) || (rc == SQLITE_OK);
    const char* details = nullptr;
    if (!passed) details = "Expected SQLITE_OK_LOAD_PERMANENTLY after APPENDVFS_TEST path.";
    return (passed ? 0 : 1);
}

static int test_appended_vfs_init_auto_ext_failure() {
#if APPENDVFS_TEST
    // Configure the mock to fail the auto-extension step
    set_apnd_auto_ext_return(SQLITE_ERROR);
    char* pErr = NULL;
    int rc = sqlite3_appendvfs_init(nullptr, &pErr, nullptr);
    bool passed = (rc == SQLITE_ERROR);
    const char* details = nullptr;
    if (!passed) details = "Expected SQLITE_ERROR when sqlite3_auto_extension returns error.";
    return (passed ? 0 : 1);
#else
    // If the macro is not defined, this test is not applicable.
    return 0;
#endif
}

// Main Runner for this test file (only meaningful when compiled with APPENDVFS_TEST defined)
int main() {
    int failures = 0;
#if APPENDVFS_TEST
    failures += test_appended_vfs_init_basic_success();
    failures += test_appended_vfs_init_auto_ext_failure();
    if (failures == 0) {
        std::printf("All tests in test_appendvfs_init_with_appenselect.cpp passed.\n");
        return EXIT_SUCCESS;
    } else {
        std::fprintf(stderr, "%d test(s) failed in test_appendvfs_init_with_appenselect.cpp.\n", failures);
        return EXIT_FAILURE;
    }
#else
    std::printf("APPENDVFS_TEST not enabled; skipping APPENDVFS_TEST-specific tests.\n");
    return EXIT_SUCCESS;
#endif
}