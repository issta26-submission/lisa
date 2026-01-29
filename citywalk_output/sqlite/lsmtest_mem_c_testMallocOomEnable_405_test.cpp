// test_lsmtest_mem.cpp
// A lightweight C++11 test harness for the focal method:
//   void testMallocOomEnable(lsm_env *pEnv, int bEnable)
// The test suite is designed to work without GoogleTest and uses
// simple asserts and a small test runner.
// 
// Key dependencies:
// - lsm.h must be available in the include path (provides lsm_env type).
// - The focal function is testMallocOomEnable(lsm_env*, int) defined in the
//   provided C file (lsmtest_mem.c). We declare it with C linkage for
//   correct linkage from C++.
// - The tests focus on calling the function with both true (non-zero) and
//   false (zero) values to cover potential branches in the callee chain.
// - We avoid relying on internal static state and instead exercise the API
//   interface (as requested by the constraints). Observability is inferred
//   by ensuring no crashes occur and that the function accepts the inputs
//   without undefined behavior. Static analysis guarantees are applied
//   via compile-time type checks through lsm_env.
// 
// Notes on test strategy (aligned with DOMAIN_KNOWLEDGE):
// - Import dependencies via lsm.h (correct paths).
// - Exercise true and false branches by passing 1 and 0 to the focal method.
// - Use the simple, non-terminating assertions approach (ASSERT macro) to
//   maximize code execution coverage without terminating the test program on
//   first failure (this allows multiple test cases to run).
// - Do not touch private implementations; only use the exported interface.
// - The tests instantiate lsm_env via the library's header (no static access).

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <lsm.h>
#include <cassert>


// Include the library header that defines lsm_env and related types.
// The actual layout of lsm_env is defined in the project headers (lsm.h).

extern "C" {
    // The focal function lives in a C source file and uses C linkage.
    // We declare it here to call from C++ test code.
    void testMallocOomEnable(lsm_env *pEnv, int bEnable);
}

// Simple assertion helper to provide clear test failure messages
#define ASSERT(cond, msg)                                  \
    do {                                                     \
        if(!(cond)) {                                        \
            fprintf(stderr, "ASSERT FAILED: %s\n", msg);     \
            return false;                                    \
        }                                                    \
    } while(0)

static bool testMallocOomEnable_EnableFalse_WithValidEnv() {
    // Domain: true/false coverage
    // Setup a minimal lsm_env with a valid pMemCtx (opaque region used by C code).
    lsm_env env;
    std::memset(&env, 0, sizeof(env));

    // Allocate a reasonably large memory region to back the opaque pMemCtx.
    // The actual C code will cast pMemCtx to a TmGlobal* and access bEnable.
    // Providing a larger buffer ensures safe memory region for the cast.
    const size_t BUF_SIZE = 1024 * 1024; // 1 MB
    void *memRegion = std::malloc(BUF_SIZE);
    ASSERT(memRegion != nullptr, "Failed to allocate memory region for pMemCtx");

    // Point environment to the allocated memory
    // The actual structure of lsm_env is defined in lsm.h; we assume pMemCtx exists.
    env.pMemCtx = memRegion;

    // Call the focal function with bEnable = 0 (disable oom)
    testMallocOomEnable(&env, 0);

    // If there were no crashes or asserts inside, this path is considered OK.
    // Clean up
    std::free(memRegion);

    return true;
}

static bool testMallocOomEnable_EnableTrue_WithValidEnv() {
    lsm_env env;
    std::memset(&env, 0, sizeof(env));

    const size_t BUF_SIZE = 1024 * 1024; // 1 MB
    void *memRegion = std::malloc(BUF_SIZE);
    ASSERT(memRegion != nullptr, "Failed to allocate memory region for pMemCtx");

    env.pMemCtx = memRegion;

    // Call the focal function with bEnable = 1 (enable oom)
    testMallocOomEnable(&env, 1);

    // Clean up
    std::free(memRegion);

    return true;
}

static bool testMallocOomEnable_EnableBothValuesSequentially() {
    // Ensure that calling with both 0 and 1 sequentially does not crash
    // and can be handled in a single test run.
    lsm_env env;
    std::memset(&env, 0, sizeof(env));

    const size_t BUF_SIZE = 1024 * 1024;
    void *memRegion = std::malloc(BUF_SIZE);
    ASSERT(memRegion != nullptr, "Failed to allocate memory region for pMemCtx");
    env.pMemCtx = memRegion;

    // First enable-off cycle
    testMallocOomEnable(&env, 1);
    // Then disable
    testMallocOomEnable(&env, 0);

    std::free(memRegion);
    return true;
}

// A simple test runner that aggregates the results from individual tests.
// Tests are deliberately independent to avoid cascading failures.
static int runAllTests() {
    int testsRun = 0;
    int testsFailed = 0;

    auto runOne = [&](const char* name, bool ok) {
        testsRun++;
        if(!ok) {
            fprintf(stderr, "Test FAILED: %s\n", name);
            testsFailed++;
        } else {
            printf("Test PASSED: %s\n", name);
        }
    };

    // Test 1: Enable false with a valid environment/context
    runOne("testMallocOomEnable_EnableFalse_WithValidEnv",
           testMallocOomEnable_EnableFalse_WithValidEnv());

    // Test 2: Enable true with a valid environment/context
    runOne("testMallocOomEnable_EnableTrue_WithValidEnv",
           testMallocOomEnable_EnableTrue_WithValidEnv());

    // Test 3: Sequential enable values to exercise both sides in one go
    runOne("testMallocOomEnable_EnableBothValuesSequentially",
           testMallocOomEnable_EnableBothValuesSequentially());

    // Summary
    printf("Total tests: %d, Failures: %d\n", testsRun, testsFailed);
    return testsFailed;
}

int main() {
    // Run the test suite. Return non-zero on failure to integrate easily
    // with CI systems that check exit codes.
    int failures = runAllTests();
    return failures > 0 ? 1 : 0;
}