/*
Unit test suite for lsmFree in lsm_mem.c

Overview:
- Test that lsmFree calls the env's xFree with the provided pointer (truthy path).
- Test that lsmFree aborts when pEnv is NULL (assert path) by catching SIGABRT with setjmp/longjmp.
- Test that lsmFree passes NULL as the pointer to xFree when given a NULL p.
Notes:
- No GoogleTest is used. A small lightweight testing harness is implemented.
- The tests rely on the actual lsm_env type defined in lsmInt.h and the function
  lsmFree to be linked from lsm_mem.c during compilation.
- All tests are wrapped in C++11 compatible code and include explanatory comments.
*/

#include <lsmInt.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <csetjmp>


extern "C" {
}

// Declare the focal function (C linkage) to be tested
extern "C" void lsmFree(lsm_env *pEnv, void *p);

// Capture points for mocking xFree callback
static void *last_freed_ptr = nullptr;
static lsm_env *last_freed_env = nullptr;

// C-style callback to mock env->xFree and verify parameters
static void free_cb(lsm_env *pEnv, void *p) {
    last_freed_env = pEnv;
    last_freed_ptr = p;
}

// Test 1: Ensure lsmFree calls env->xFree with the exact pointer provided
// This covers the "true" branch of the predicate (pEnv != NULL).
static bool test_lsmFree_calls_xFree() {
    // Reset capture
    last_freed_ptr = nullptr;
    last_freed_env = nullptr;

    // Prepare a fake environment with our callback
    lsm_env env;
    env.xFree = free_cb;

    // Allocate a memory block to be "freed" and pass to lsmFree
    void *mem = malloc(64);
    if (!mem) {
        printf("TEST: Memory allocation failed\n");
        return false;
    }

    lsmFree(&env, mem);

    bool ok = (last_freed_env == &env) && (last_freed_ptr == mem);

    free(mem);
    return ok;
}

// Test 2: Ensure lsmFree aborts when pEnv is NULL (assert path)
// We catch the abort via signal handling (SIGABRT) and setjmp/longjmp to
// continue test execution without terminating the test suite.
static volatile int test2_aborted = 0;
static jmp_buf test2_jmpbuf;

static void abort_handler(int /*sig*/) {
    test2_aborted = 1;
    longjmp(test2_jmpbuf, 1);
}

// Test 2 implementation
static bool test_lsmFree_assert_on_null_env() {
    test2_aborted = 0;

    // Install a temporary abort handler
    std::signal(SIGABRT, abort_handler);

    int jmpRes = setjmp(test2_jmpbuf);
    if (jmpRes == 0) {
        // This should abort; if it returns, the test should fail
        lsmFree(nullptr, (void*)0xDEADBEEF);
        // If we get here, abort did not occur as expected
        std::signal(SIGABRT, SIG_DFL);
        return false;
    } else {
        // We returned here due to abort
        std::signal(SIGABRT, SIG_DFL);
        return test2_aborted == 1;
    }
}

// Test 3: Ensure lsmFree passes NULL as p to xFree when p is NULL
// This further validates the interaction with the callback.
static bool test_lsmFree_pass_null_ptr() {
    // Reset capture
    last_freed_ptr = nullptr;
    last_freed_env = nullptr;

    lsm_env env;
    env.xFree = free_cb;

    // Pass NULL as the pointer to be freed
    lsmFree(&env, nullptr);

    bool ok = (last_freed_env == &env) && (last_freed_ptr == nullptr);
    return ok;
}

// Lightweight test harness
static void run_and_report(const char* test_name, bool result) {
    printf("%s: %s\n", test_name, result ? "PASS" : "FAIL");
}

// Entry point for running tests
int main() {
    int total = 0;
    int passed = 0;

    // Test 1
    bool r1 = test_lsmFree_calls_xFree();
    run_and_report("test_lsmFree_calls_xFree", r1);
    total++; if (r1) passed++;

    // Test 2
    bool r2 = test_lsmFree_assert_on_null_env();
    run_and_report("test_lsmFree_assert_on_null_env", r2);
    total++; if (r2) passed++;

    // Test 3
    bool r3 = test_lsmFree_pass_null_ptr();
    run_and_report("test_lsmFree_pass_null_ptr", r3);
    total++; if (r3) passed++;

    // Summary
    printf("Total tests: %d, Passed: %d, Failed: %d\n", total, passed, total - passed);
    return (total == passed) ? 0 : 1;
}