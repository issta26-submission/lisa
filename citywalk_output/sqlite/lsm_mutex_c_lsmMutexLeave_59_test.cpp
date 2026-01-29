// Unit test suite for the focal method lsmMutexLeave
// This test harness is written for C++11 compatibility without using GTest.
// It mocks the environment (lsm_env) and the mutex (lsm_mutex) to verify
// that lsmMutexLeave correctly invokes pEnv->xMutexLeave(pMutex).

#include <lsmInt.h>
#include <iostream>


// Domain knowledge notes embedded as comments for maintainability.
// We assume the focal function signature matches:
// extern "C" void lsmMutexLeave(lsm_env *pEnv, lsm_mutex *pMutex);

// Minimal structural definitions to enable testing against the focal method.
// These definitions are designed to be compatible with the function under test.
struct lsm_mutex {
    int dummy; // opaque placeholder
};

// The environment structure expected by lsmMutexLeave.
// It contains a single function pointer xMutexLeave which is invoked by the focal method.
struct lsm_env {
    void (*xMutexLeave)(lsm_mutex *pMutex);
};

// Declaration of the focal method to be tested.
// The real implementation is expected to be linked in during the build.
extern "C" void lsmMutexLeave(lsm_env *pEnv, lsm_mutex *pMutex);

// -------- Mock infrastructure to capture calls --------

static struct CallbackRecorder {
    int callCount;
    lsm_mutex *lastArg;
} g_recorder;

// Mock function to replace pEnv->xMutexLeave during tests.
// It records the number of invocations and the last mutex pointer received.
static void MockMutexLeave(lsm_mutex *pMutex) {
    g_recorder.callCount += 1;
    g_recorder.lastArg = pMutex;
}

// Simple, non-terminating assertion helpers to allow test continuation after failures.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do {                                     \
    ++g_tests_run;                                                        \
    if(!(cond)) {                                                         \
        ++g_tests_failed;                                                 \
        std::cerr << "[FAIL] " << msg << " (condition: " #cond ")" << std::endl; \
    }                                                                     \
} while(0)

#define EXPECT_EQ(a, b, msg) do {                                        \
    ++g_tests_run;                                                        \
    if(!((a) == (b))) {                                                 \
        ++g_tests_failed;                                                 \
        std::cerr << "[FAIL] " << msg << " (expected: " << (b)           \
                  << ", actual: " << (a) << ")" << std::endl;           \
    }                                                                     \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do {                                    \
    ++g_tests_run;                                                        \
    if((a) != (b)) {                                                    \
        ++g_tests_failed;                                                 \
        std::cerr << "[FAIL] " << msg << " (expected pointer: "                \
                  << (void*)(b) << ", actual: " << (void*)(a) << ")" << std::endl; \
    }                                                                     \
} while(0)

// -------- Test cases for lsmMutexLeave --------

/*
 Test 1: Basic call should forward the mutex pointer to the env's xMutexLeave
 Description:
 - Set up a mock environment where xMutexLeave points to MockMutexLeave.
 - Call lsmMutexLeave with a real mutex object.
 - Verify that the callback was invoked exactly once and with the correct mutex pointer.
*/
static void test_basic_call_forwards_pointer() {
    // Prepare recorder
    g_recorder.callCount = 0;
    g_recorder.lastArg = nullptr;

    // Prepare env with the mock function
    lsm_env env;
    env.xMutexLeave = MockMutexLeave;

    // Create a dummy mutex
    lsm_mutex m;
    m.dummy = 123;

    // Act
    lsmMutexLeave(&env, &m);

    // Assert
    EXPECT_EQ(g_recorder.callCount, 1, "Expected xMutexLeave to be called exactly once");
    EXPECT_PTR_EQ(g_recorder.lastArg, &m, "Expected xMutexLeave to receive the same mutex pointer");
}

/*
 Test 2: Multiple consecutive calls should invoke the callback for each call
 Description:
 - Reuse the same environment and two different mutex instances.
 - Ensure that the recorder captures two distinct calls and the lastArg matches the second mutex.
*/
static void test_multiple_calls_increment_and_capture_last() {
    // Reset recorder
    g_recorder.callCount = 0;
    g_recorder.lastArg = nullptr;

    // Prepare env with the mock function
    lsm_env env;
    env.xMutexLeave = MockMutexLeave;

    // First mutex
    lsm_mutex m1;
    m1.dummy = 1;

    // Second mutex
    lsm_mutex m2;
    m2.dummy = 2;

    // Act: two sequential calls
    lsmMutexLeave(&env, &m1);
    lsmMutexLeave(&env, &m2);

    // Assert
    EXPECT_EQ(g_recorder.callCount, 2, "Expected two calls to xMutexLeave");
    EXPECT_PTR_EQ(g_recorder.lastArg, &m2, "Expected last argument to be the second mutex pointer");
}

/*
 Test 3: Null mutex pointer should be forwarded as null
 Description:
 - Pass a null pointer for pMutex and verify that the callback receives nullptr.
*/
static void test_null_mutex_pointer_handling() {
    // Reset recorder
    g_recorder.callCount = 0;
    g_recorder.lastArg = nullptr;

    // Prepare env with the mock function
    lsm_env env;
    env.xMutexLeave = MockMutexLeave;

    // Act: pass nullptr
    lsmMutexLeave(&env, nullptr);

    // Assert
    EXPECT_EQ(g_recorder.callCount, 1, "Expected xMutexLeave to be called even with nullptr mutex");
    EXPECT_EQ(g_recorder.lastArg, nullptr, "Expected lastArg to be nullptr when pMutex is nullptr");
}

// -------- Main launcher for tests --------

int main() {
    // Run tests
    test_basic_call_forwards_pointer();
    test_multiple_calls_increment_and_capture_last();
    test_null_mutex_pointer_handling();

    // Summary
    std::cout << "LSM Mutex Leave tests run: " << g_tests_run
              << ", failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return g_tests_failed != 0 ? 1 : 0;
}