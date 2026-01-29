// Test suite for the focal method lsm_malloc in lsm_mem.c
// This test is self-contained: it provides mock implementations for the
// dependencies (lsmMalloc, lsm_default_env) used by lsm_malloc to verify
// correct forwarding behavior (null vs non-null pEnv) without requiring
// the real project build.
//
// The approach follows these ideas:
// - Implement a deterministic lsmMalloc that records the last received env and N.
// - Implement a deterministic lsm_default_env() returning a static default env.
// - Implement lsm_malloc exactly as in the focal method to delegate to lsmMalloc
//   using the provided env or the default one.
// - Verify that when pEnv is non-null, lsm_malloc forwards that env to lsmMalloc.
// - Verify that when pEnv is null, lsm_malloc forwards lsm_default_env() to lsmMalloc.
// - Use non-terminating checks and report a concise summary at the end.

#include <cstddef>
#include <lsmInt.h>
#include <iostream>


// Forward declare the env type (surface only for testing)
struct lsm_env;

// -----------------------------------------------------------------------------
// Mocked dependencies to support testing of lsm_malloc
// -----------------------------------------------------------------------------

// Backing storage for the default environment instance (static to simulate a singleton)
static lsm_env g_default_env_instance;

// Forward declaration of the default env accessor used by lsm_malloc
extern "C" lsm_env *lsm_default_env();

// Global state to verify lsmMalloc receives the expected parameters
static lsm_env *g_last_malloc_env = nullptr;
static size_t g_last_malloc_N = 0;

// Sentinel value returned by our mocked lsmMalloc to verify the flow
static void* g_lsmMalloc_return_sentinel = reinterpret_cast<void*>(0xDEADBEEF);

// Mocked lsmMalloc: records inputs and returns a sentinel pointer
extern "C" void *lsmMalloc(struct lsm_env *pEnv, size_t N) {
    g_last_malloc_env = pEnv;
    g_last_malloc_N = N;
    return g_lsmMalloc_return_sentinel;
}

// The actual function under test is lsm_malloc, defined in the focal code.
// It should forward to lsmMalloc(pEnv ? pEnv : lsm_default_env(), N)
extern "C" void *lsm_malloc(struct lsm_env *pEnv, size_t N);

// Implementation of lsm_default_env: returns the static default environment
extern "C" lsm_env *lsm_default_env() {
    return &g_default_env_instance;
}

// -----------------------------------------------------------------------------
// Minimal test framework (non-terminating, prints per-test results)
// -----------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

static void test_pass(const char* testName) {
    ++g_total_tests;
    std::cout << "[PASS] " << testName << std::endl;
}
static void test_fail(const char* testName, const char* reason) {
    ++g_total_tests;
    ++g_failed_tests;
    std::cerr << "[FAIL] " << testName << " - " << reason << std::endl;
}

// Helper assertion helpers
static void expect_ptr_equal(const char* testName, void* a, void* b) {
    if (a != b) {
        test_fail(testName, "pointer values are not equal");
    } else {
        test_pass(testName);
    }
}
static void expect_env_equal(const char* testName, lsm_env* a, lsm_env* b) {
    if (a != b) {
        test_fail(testName, "env pointers are not equal");
    } else {
        test_pass(testName);
    }
}
static void expect_true(const char* testName, bool cond) {
    if (!cond) {
        test_fail(testName, "condition is false");
    } else {
        test_pass(testName);
    }
}

// -----------------------------------------------------------------------------
// Test cases
// -----------------------------------------------------------------------------

// Test 1: Non-null pEnv should be forwarded to lsmMalloc without using the default env
// Steps:
// - Prepare a non-null env pointer
// - Call lsm_malloc(env1, N1)
// - Verify: last malloc env is env1, last N is N1, and return value is sentinel
static void test_lsm_malloc_forwards_non_null_env() {
    // Arrange
    g_last_malloc_env = nullptr;
    g_last_malloc_N = 0;
    void* expectedReturn = g_lsmMalloc_return_sentinel;
    lsm_env* env1 = reinterpret_cast<lsm_env*>(0x12345678);
    size_t N1 = 1234;

    // Act
    void* result = lsm_malloc(env1, N1);

    // Assert
    expect_ptr_equal("test_lsm_malloc_forwards_non_null_env: result is sentinel", result, expectedReturn);
    expect_env_equal("test_lsm_malloc_forwards_non_null_env: forwarded_env == env1", g_last_malloc_env, env1);
    // Also ensure N is forwarded
    if (g_last_malloc_N != N1) {
        test_fail("test_lsm_malloc_forwards_non_null_env: forwarded_N", "N value not forwarded correctly");
    } else {
        test_pass("test_lsm_malloc_forwards_non_null_env: forwarded_N");
    }
}

// Test 2: Null pEnv should use lsm_default_env(), not a null env
// Steps:
// - Call lsm_malloc(nullptr, N2)
// - Verify: last malloc env is lsm_default_env(), last N is N2, and return value is sentinel
static void test_lsm_malloc_forwards_null_env_to_default() {
    // Arrange
    g_last_malloc_env = nullptr;
    g_last_malloc_N = 0;
    void* expectedReturn = g_lsmMalloc_return_sentinel;
    size_t N2 = 5678;

    // Act
    void* result = lsm_malloc(nullptr, N2);

    // Assert
    expect_ptr_equal("test_lsm_malloc_forwards_null_env_to_default: result is sentinel", result, expectedReturn);
    expect_env_equal("test_lsm_malloc_forwards_null_env_to_default: forwarded_env == default_env", g_last_malloc_env, lsm_default_env());
    if (g_last_malloc_N != N2) {
        test_fail("test_lsm_malloc_forwards_null_env_to_default: forwarded_N", "N value not forwarded correctly");
    } else {
        test_pass("test_lsm_malloc_forwards_null_env_to_default: forwarded_N");
    }
}

// Test 3: Reuse of default_env across multiple null calls (consistency check for default env pointer)
// Steps:
// - Call lsm_malloc(nullptr, N3) twice and ensure the env pointer equals lsm_default_env() each time
static void test_lsm_malloc_default_env_consistency() {
    void* sentinel = g_lsmMalloc_return_sentinel;
    lsm_env* first_default = lsm_default_env();

    void* r1 = lsm_malloc(nullptr, 1);
    expect_ptr_equal("test_lsm_malloc_default_env_consistency: first result", r1, sentinel);
    expect_env_equal("test_lsm_malloc_default_env_consistency: first env equals default env", g_last_malloc_env, first_default);

    void* r2 = lsm_malloc(nullptr, 2);
    expect_ptr_equal("test_lsm_malloc_default_env_consistency: second result", r2, sentinel);
    // The env pointer should always be exactly lsm_default_env(), which is the same address across calls
    expect_env_equal("test_lsm_malloc_default_env_consistency: second env equals default env", g_last_malloc_env, lsm_default_env());

    // Also verify the default env pointer didn't unexpectedly change between calls
    expect_env_equal("test_lsm_malloc_default_env_consistency: default env is stable", lsm_default_env(), first_default);
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main() {
    // Run tests
    test_lsm_malloc_forwards_non_null_env();
    test_lsm_malloc_forwards_null_env_to_default();
    test_lsm_malloc_default_env_consistency();

    // Summary
    std::cout << "----------------------------------------" << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "[SUMMARY] All tests passed: " << g_total_tests << " tests." << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] Failures: " << g_failed_tests
                  << " / " << g_total_tests << " tests." << std::endl;
        return 1;
    }
}