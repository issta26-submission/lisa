// C++11 Test Suite for focal method: lsmMutexStatic
// This test suite exercises lsmMutexStatic by substituting a fake
// xMutexStatic function through the lsm_env interface.
// It uses a lightweight, non-terminating test harness (no GTest).

#include <lsmInt.h>
#include <iostream>
#include <cstdint>


extern "C" int lsmMutexStatic(lsm_env* pEnv, int iMutex, lsm_mutex** ppStatic);

// Global test state to capture calls to the fake function
static int g_fake_return_value = 0;
static lsm_env* g_last_env = nullptr;
static int g_last_iMutex = 0;
static lsm_mutex** g_last_pp = nullptr;

// Lightweight assertion helper (non-terminating)
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Fake implementation of the environment's mutex static provider.
// This function will record its inputs and optionally write a fake pointer
// to the provided ppStatic, simulating the behavior of a real environment.
static int fake_xMutexStatic(lsm_env* pEnv, int iMutex, lsm_mutex** ppStatic) {
    g_last_env = pEnv;
    g_last_iMutex = iMutex;
    g_last_pp = ppStatic;
    if (ppStatic != nullptr) {
        // Provide a recognizable fake pointer to verify up-stream propagation
        *ppStatic = reinterpret_cast<lsm_mutex*>(0xDEADBEEF);
    }
    return g_fake_return_value;
}

// Helper to reset tracking state between tests
static void reset_state() {
    g_last_env = nullptr;
    g_last_iMutex = 0;
    g_last_pp = nullptr;
    g_fake_return_value = 0;
}

// Test 1: Basic path where the underlying xMutexStatic returns 0.
// Verifies that lsmMutexStatic forwards parameters correctly and propagates
// the updated ppStatic pointer back to the caller.
static void test_lsmMutexStatic_basic_forwarding_zero() {
    reset_state();

    lsm_env env;
    // Install fake function into the environment
    env.xMutexStatic = fake_xMutexStatic;

    lsm_mutex* result = nullptr;
    g_fake_return_value = 0;

    int ret = lsmMutexStatic(&env, 5, &result);

    ASSERT_TRUE(ret == 0, "lsmMutexStatic should return the value from xMutexStatic (0).");
    ASSERT_TRUE(g_last_env == &env, "Environment pointer should be passed to xMutexStatic.");
    ASSERT_TRUE(g_last_iMutex == 5, "iMutex argument should be forwarded unchanged.");
    ASSERT_TRUE(g_last_pp == &result, "ppStatic pointer should be forwarded to xMutexStatic.");
    ASSERT_TRUE(result == reinterpret_cast<lsm_mutex*>(0xDEADBEEF),
                "ppStatic should be updated by fake_xMutexStatic.");
}

// Test 2: Path where the underlying xMutexStatic returns a non-zero value.
// Verifies that the return value is propagated and parameters are still forwarded.
static void test_lsmMutexStatic_basic_forwarding_nonzero() {
    reset_state();

    lsm_env env;
    env.xMutexStatic = fake_xMutexStatic;

    lsm_mutex* result = nullptr;
    g_fake_return_value = 12345;

    int ret = lsmMutexStatic(&env, -7, &result);

    ASSERT_TRUE(ret == 12345, "lsmMutexStatic should forward non-zero return value.");
    ASSERT_TRUE(g_last_env == &env, "Environment pointer should be passed to xMutexStatic.");
    ASSERT_TRUE(g_last_iMutex == -7, "iMutex argument should reflect input value (-7).");
    ASSERT_TRUE(g_last_pp == &result, "ppStatic pointer should be forwarded to xMutexStatic.");
    ASSERT_TRUE(result == reinterpret_cast<lsm_mutex*>(0xDEADBEEF),
                "ppStatic should be updated by fake_xMutexStatic even on non-zero return.");
}

// Test 3: Null ppStatic pointer should be handled gracefully without dereferencing.
// Ensures no crash and correct propagation of the return value.
static void test_lsmMutexStatic_null_pp_handling() {
    reset_state();

    lsm_env env;
    env.xMutexStatic = fake_xMutexStatic;

    g_fake_return_value = 42;

    int ret = lsmMutexStatic(&env, 3, nullptr);

    ASSERT_TRUE(ret == 42, "lsmMutexStatic should return value when ppStatic is nullptr.");
    ASSERT_TRUE(g_last_env == &env, "Environment pointer should be passed to xMutexStatic.");
    ASSERT_TRUE(g_last_iMutex == 3, "iMutex argument should be forwarded correctly.");
    ASSERT_TRUE(g_last_pp == nullptr, "ppStatic pointer should be nullptr when not provided.");
}

// Simple test runner
static void run_all_tests() {
    std::cout << "Running lsmMutexStatic unit tests..." << std::endl;
    test_lsmMutexStatic_basic_forwarding_zero();
    test_lsmMutexStatic_basic_forwarding_nonzero();
    test_lsmMutexStatic_null_pp_handling();
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
    }
}

// Main entry point: run tests without relying on a test framework
int main() {
    // Run tests
    run_all_tests();
    return (g_failures == 0) ? 0 : 1;
}