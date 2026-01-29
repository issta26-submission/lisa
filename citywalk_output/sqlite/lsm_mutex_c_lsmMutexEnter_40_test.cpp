// Test suite for lsmMutexEnter in lsm_mutex.c using a lightweight, non-GTest framework.
// This test assumes the project provides lsm_env and lsm_mutex types (as declared in lsmInt.h)
// and that lsmMutexEnter is compiled in C (exposed to C++ via extern "C").

#include <lsmInt.h>
#include <iostream>
#include <cstdint>


// Include project headers to obtain type definitions (lsm_env, lsm_mutex, etc.)

// Ensure the C function is visible to this C++ translation unit
extern "C" void lsmMutexEnter(lsm_env *pEnv, lsm_mutex *pMutex);

// Global test state for assertions (non-terminating)
static int g_testFailures = 0;

// Global test state to observe calls and parameters passed to xMutexEnter
static int g_enterCallCount = 0;
static lsm_mutex *g_lastEnteredMutex = nullptr;

// Two mock implementations for xMutexEnter to verify forwarding behavior
static void mock_xMutexEnter_v1(lsm_mutex *pMutex) {
    g_lastEnteredMutex = pMutex;
    g_enterCallCount += 1; // simple increment to track calls
}

static void mock_xMutexEnter_v2(lsm_mutex *pMutex) {
    g_lastEnteredMutex = pMutex;
    g_enterCallCount += 10; // distinguish from v1 by larger increment
}

// Lightweight assertion helpers (non-terminating, print-only on failure)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] EXPECT_TRUE: " << msg << " (in " << __FUNCTION__ << ")\n"; \
        ++g_testFailures; \
    } \
} while(0)

#define EXPECT_EQ_INT(expected, actual, msg) do { \
    if((int)(expected) != (int)(actual)) { \
        std::cerr << "[FAIL] EXPECT_EQ_INT: " << msg \
                  << " | Expected: " << (expected) << " Actual: " << (actual) \
                  << " (in " << __FUNCTION__ << ")\n"; \
        ++g_testFailures; \
    } \
} while(0)

#define EXPECT_EQ_PTR(expected, actual, msg) do { \
    if((void*)(expected) != (void*)(actual)) { \
        std::cerr << "[FAIL] EXPECT_EQ_PTR: " << msg \
                  << " | Expected: " << (void*)(expected) \
                  << " Actual: " << (void*)(actual) \
                  << " (in " << __FUNCTION__ << ")\n"; \
        ++g_testFailures; \
    } \
} while(0)

// Global test environment to reuse across tests
static lsm_env g_env;

// Test 1: Basic forwarding - ensure lsmMutexEnter forwards the call to pEnv->xMutexEnter
// with the same pMutex pointer provided by the caller.
static void test_lsmMutexEnter_basic_forwarding() {
    // Reset state
    g_enterCallCount = 0;
    g_lastEnteredMutex = nullptr;

    // Initialize environment with first mock
    g_env = {}; // zero-initialize
    g_env.xMutexEnter = mock_xMutexEnter_v1;

    lsm_mutex m1; // first mutex object

    // Execute the focal function
    lsmMutexEnter(&g_env, &m1);

    // Validate that xMutexEnter was called exactly once with &m1
    EXPECT_EQ_INT(1, g_enterCallCount, "Expected xMutexEnter to be called exactly once (v1).");
    EXPECT_EQ_PTR(&m1, g_lastEnteredMutex, "Expected passed mutex to be exactly &m1.");
}

// Test 2: Function pointer switching - verify that changing the xMutexEnter
// function pointer on the environment is honored by lsmMutexEnter.
static void test_lsmMutexEnter_switch_function() {
    // Reset state
    g_enterCallCount = 0;
    g_lastEnteredMutex = nullptr;

    // Reuse environment, switch to second mock
    g_env.xMutexEnter = mock_xMutexEnter_v2;

    lsm_mutex m2;

    lsmMutexEnter(&g_env, &m2);

    // Since v2 increments by 10, total should be previous 1 + 10 = 11
    EXPECT_EQ_INT(11, g_enterCallCount, "Expected xMutexEnter to be called and increment by 10 (v2).");
    EXPECT_EQ_PTR(&m2, g_lastEnteredMutex, "Expected passed mutex to be exactly &m2.");
}

// Test 3: Null mutex passed through - ensure that a NULL pMutex is forwarded
// to the environment's xMutexEnter. This validates that the parameter is forwarded
// unchanged, even when NULL. (The actual behavior for NULL is defined by the user-provided callback.)
static void test_lsmMutexEnter_null_mutex() {
    // Reset state
    g_enterCallCount = 0;
    g_lastEnteredMutex = nullptr;

    // Use the first mock again
    g_env.xMutexEnter = mock_xMutexEnter_v1;

    lsm_mutex *nullMutex = nullptr;

    lsmMutexEnter(&g_env, nullMutex);

    // Expect the callback to have been invoked and lastEnteredMutex to be NULL
    EXPECT_EQ_INT(1, g_enterCallCount, "Expected xMutexEnter to be called once with NULL mutex.");
    EXPECT_EQ_PTR(nullptr, g_lastEnteredMutex, "Expected passed mutex to be NULL.");
}

// Helper to run all tests and summarize
static void run_all_tests() {
    test_lsmMutexEnter_basic_forwarding();
    test_lsmMutexEnter_switch_function();
    test_lsmMutexEnter_null_mutex();

    if(g_testFailures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << g_testFailures << " test(s) failed.\n";
    }
}

// Main entry point to satisfy the requirement "call test methods from the main function"
int main() {
    std::cout << "Running lsmMutexEnter test suite...\n";
    run_all_tests();
    return g_testFailures == 0 ? 0 : 1;
}