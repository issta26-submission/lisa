// Comprehensive unit tests for lsmMutexHeld in C/C++11 without using GTest.
// The tests are written in plain C++ and use a small custom test harness
// with non-terminating assertions to ensure broad coverage.
// The tests exercise both branches of lsmMutexHeld:
// 1) When pEnv->xMutexHeld is non-null, it should call the function pointer and return its value.
// 2) When pEnv->xMutexHeld is null, it should return 1.
// Additionally, tests verify that the function pointer receives the correct mutex argument.

#include <lsmInt.h>
#include <iostream>


// Import C headers with C linkage to ensure proper linkage with C implementations.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_passed = 0;
static int g_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        ++g_passed; \
    } else { \
        ++g_failed; \
        std::cerr << "ASSERT FAILED: " << msg << std::endl; \
    } \
} while (0)

#define TEST_SUMMARY() \
    std::cerr << "TEST SUMMARY: " << g_passed << " passed, " << g_failed << " failed." << std::endl;

// Global state for capturing callback behavior in tests
static lsm_mutex *g_lastCalledMutex = nullptr;
static int g_lastCallbackReturnValue = 0;

// Test 1: xMutexHeld is non-null; lsmMutexHeld should call the callback and return its value,
// and pass the same mutex pointer to the callback.
static int test_xMutexHeld_calledWithMutex(lsm_mutex *pMutex) {
    g_lastCalledMutex = pMutex;
    return 12345; // Arbitrary non-default return value for verification
}

static void run_test_mutexHeld_calls_callback() {
    // Arrange
    g_lastCalledMutex = nullptr;
    lsm_env env = {};
    env.xMutexHeld = test_xMutexHeld_calledWithMutex;

    lsm_mutex m = {}; // dummy mutex instance

    // Act
    int result = lsmMutexHeld(&env, &m);

    // Assert
    TEST_ASSERT(result == 12345,
                "lsmMutexHeld should return the value from xMutexHeld callback when it's non-null");
    TEST_ASSERT(g_lastCalledMutex == &m,
                "lsmMutexHeld should pass the exact mutex pointer to the xMutexHeld callback");
}

// Test 2: xMutexHeld is null; lsmMutexHeld should return 1 (default behavior)
static void run_test_mutexHeld_nullCallbackReturnsOne() {
    // Arrange
    lsm_env env = {};
    env.xMutexHeld = nullptr;

    lsm_mutex m = {};

    // Act
    int result = lsmMutexHeld(&env, &m);

    // Assert
    TEST_ASSERT(result == 1,
                "lsmMutexHeld should return 1 when xMutexHeld callback is null");
}

// Test 3: Callback is non-null; ensure that pMutex can be null and is passed through to callback
static int test_xMutexHeld_handlesNullMutex(lsm_mutex *pMutex) {
    g_lastCalledMutex = pMutex; // capture the passed pointer (likely nullptr in this test)
    return 7;
}

static void run_test_mutexHeld_passesNullMutexWhenRequested() {
    // Arrange
    g_lastCalledMutex = reinterpret_cast<lsm_mutex*>(0xDEADBEEF); // sentinel
    lsm_env env = {};
    env.xMutexHeld = test_xMutexHeld_handlesNullMutex;

    // Act
    int result = lsmMutexHeld(&env, nullptr);

    // Assert
    TEST_ASSERT(result == 7,
                "lsmMutexHeld should return the callback's value when provided a null pMutex");
    TEST_ASSERT(g_lastCalledMutex == nullptr,
                "lsmMutexHeld should pass nullptr as the mutex pointer to the callback");
}

// Main function to run tests
int main() {
    std::cerr << "Starting tests for lsmMutexHeld..." << std::endl;

    run_test_mutexHeld_calls_callback();
    run_test_mutexHeld_nullCallbackReturnsOne();
    run_test_mutexHeld_passesNullMutexWhenRequested();

    TEST_SUMMARY();
    // Return non-zero if there were failures to aid integration in CI systems
    return (g_failed == 0) ? 0 : 1;
}