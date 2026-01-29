// Test suite for focal method lsmRealloc in lsm_mem.c
// Implements non-GTest, lightweight unit tests in C++11.
// The tests focus on verifying that lsmRealloc correctly delegates
// to the environment's xRealloc function and propagates return values.
// Comments provide explanation for each test case.

#include <cstddef>
#include <lsmInt.h>
#include <iostream>


// Include C headers with C linkage for lsmRealloc and lsm_env.
// We wrap the C include to ensure proper linkage in C++.
extern "C" {
}

// ---------------------------------------------------------------------------
// Test scaffolding to observe the behavior of lsmRealloc.
// We rely on a custom xRealloc implementation to capture parameters.
// The focal method lsmRealloc simply calls pEnv->xRealloc(pEnv, p, N).
// By mocking xRealloc, we can validate that delegation and argument forwarding
// behave as expected without linking to the full allocation subsystem.
// ---------------------------------------------------------------------------

// Global observables for test probe
static void* g_returnPtr = nullptr;          // What xRealloc should return
static lsm_env* g_lastEnv = nullptr;         // The env pointer received by xRealloc
static void* g_lastP = nullptr;               // The pointer parameter received by xRealloc
static size_t g_lastN = 0;                    // The size parameter received by xRealloc

// Mock implementation of xRealloc to observe inputs and control return value.
// This function must have the same signature as the actual xRealloc callback
// (lsm_env*, void*, size_t) and is used to validate lsmRealloc's behavior.
extern "C" void* test_realloc_impl(lsm_env *env, void *p, size_t N) {
    g_lastEnv = env;
    g_lastP = p;
    g_lastN = N;
    return g_returnPtr; // Return value is controlled by the test case
}

// ---------------------------------------------------------------------------
// Test 1: Forwarding and return value propagation
// Purpose: Validate that lsmRealloc forwards its arguments to pEnv->xRealloc
//          and that the return value from xRealloc is correctly propagated
//          back to the caller of lsmRealloc.
// Preconditions: A valid lsm_env with xRealloc set to test_realloc_impl.
// Expected: The result should equal g_returnPtr, and all forwarded arguments
//           (env, p, N) should match the inputs.
// ---------------------------------------------------------------------------

static bool test_lsmRealloc_forwards_arguments_and_return() {
    // Arrange
    g_returnPtr = reinterpret_cast<void*>(0x12345678); // deterministic fake return

    g_lastEnv = nullptr;
    g_lastP = nullptr;
    g_lastN = 0;

    lsm_env env{};                    // Aggregate initialization to zero fields
    env.xRealloc = &test_realloc_impl; // Point to our mock

    void* p = reinterpret_cast<void*>(0xDEADBEEF);
    size_t N = 42;

    // Act
    void* result = lsmRealloc(&env, p, N);

    // Assert (non-terminating, print-on-failure for visibility)
    bool ok = (result == g_returnPtr)
              && (g_lastEnv == &env)
              && (g_lastP == p)
              && (g_lastN == N);

    if (!ok) {
        std::cerr << "Detail - test_lsmRealloc_forwards_arguments_and_return:\n"
                  << "  result=" << result
                  << ", expected=" << g_returnPtr << "\n"
                  << "  lastEnv=" << static_cast<void*>(g_lastEnv) << ", &env=" << static_cast<void*>(&env) << "\n"
                  << "  lastP=" << g_lastP << ", expected=" << p << "\n"
                  << "  lastN=" << g_lastN << ", expected=" << N << "\n";
    }
    return ok;
}

// ---------------------------------------------------------------------------
// Test 2: Forwarding when xRealloc returns NULL
// Purpose: Ensure that lsmRealloc propagates a NULL return from xRealloc,
//          i.e., the caller receives NULL when the underlying realloc fails.
// Preconditions: A valid lsm_env with xRealloc set to test_realloc_impl.
// Expected: The result should be nullptr and arguments forwarded correctly.
// ---------------------------------------------------------------------------

static bool test_lsmRealloc_handles_null_return() {
    // Arrange
    g_returnPtr = nullptr;

    g_lastEnv = nullptr;
    g_lastP = nullptr;
    g_lastN = 0;

    lsm_env env{};
    env.xRealloc = &test_realloc_impl;

    void* p = nullptr;
    size_t N = 0;

    // Act
    void* result = lsmRealloc(&env, p, N);

    // Assert
    bool ok = (result == nullptr)
              && (g_lastEnv == &env)
              && (g_lastP == p)
              && (g_lastN == N);

    if (!ok) {
        std::cerr << "Detail - test_lsmRealloc_handles_null_return:\n"
                  << "  result=" << result << ", expected=nullptr\n"
                  << "  lastEnv=" << static_cast<void*>(g_lastEnv) << ", &env=" << static_cast<void*>(&env) << "\n"
                  << "  lastP=" << g_lastP << ", expected=" << p << "\n"
                  << "  lastN=" << g_lastN << ", expected=" << N << "\n";
    }
    return ok;
}

// ---------------------------------------------------------------------------
// Main test runner
// Executes the defined tests and reports results.
// ---------------------------------------------------------------------------

int main() {
    int failures = 0;

    // Run Test 1
    if (test_lsmRealloc_forwards_arguments_and_return()) {
        std::cout << "PASSED: test_lsmRealloc_forwards_arguments_and_return\n";
    } else {
        std::cerr << "FAILED: test_lsmRealloc_forwards_arguments_and_return\n";
        ++failures;
    }

    // Run Test 2
    if (test_lsmRealloc_handles_null_return()) {
        std::cout << "PASSED: test_lsmRealloc_handles_null_return\n";
    } else {
        std::cerr << "FAILED: test_lsmRealloc_handles_null_return\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << failures << " TEST(S) FAILED" << std::endl;
        return failures;
    }
}