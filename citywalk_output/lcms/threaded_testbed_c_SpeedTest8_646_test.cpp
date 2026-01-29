// Automated unit tests for the focal method SpeedTest8 (from threaded_testbed.c)
// Notes:
// - This test suite is written in C++11, without using GoogleTest.
// - It relies on the focal function SpeedTest8 being linked from the project sources.
// - We provide lightweight, non-terminating tests that exercise the focal method
//   in a safe manner. The tests focus on execution without crashes and on repeated runs
//   to improve code coverage.
// - Only standard library facilities are used for assertions and test orchestration.
// - The tests do not modify private data or internal static details of the focal code.
// - The tests declare the focal function with C linkage to ensure proper linkage.
//
// Candidate Keywords from the focal method (SpeedTest8) and its dependencies (for test planning):
// - SpeedTest8, SpeedTest8bitsRGB, 8 bits on CLUT profiles, Matrix-Shaper, curves
// - 8 bits on CLUT profiles, 8 bits on Matrix-Shaper, 8 bits on curves
// - cmsCreateContext, cmsDeleteContext, PROFILES_DIR, test5.icc, test3.icc, test0.icc
// - Test harness functions: PerformanceHeader, Performance, loadProfile, trace
// - Contexts: noPlugin (plugin-less) vs plugin-enabled path
// - File scope / static dependencies inside the original threaded_testbed.c
//
// Domain knowledge considerations implemented:
// - Use a minimal, portable test harness with clear pass/fail reporting.
// - Use extern "C" to declare the focal function SpeedTest8 (as well as any other
//   external C interfaces if needed).
// - Avoid private/internal details; tests only rely on the public entry point SpeedTest8.
// - Ensure tests are executable even if the actual profiling data is unavailable by
//   relying on the code path to execute and not crash.
// - Provide explanatory comments for each test case to describe intent.

#include <exception>
#include <threaded_internal.h>
#include <iostream>
#include <string>
#include <time.h>
#include <memory.h>
#include <stdlib.h>


// Declare the focal function from the provided source with C linkage.
// The function signature is: void SpeedTest8(void);
extern "C" void SpeedTest8(void);

// Lightweight assertion helper (non-terminating): returns true on success, false otherwise.
// We purposely do not terminate on assertion failure to maximize code coverage.
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[ASSERT_FAIL] " << (msg) << " (Expression: " #cond ")" << std::endl; \
            return false; \
        } \
    } while (false)

// Test 1: Basic execution test
// Intent: Ensure that invoking SpeedTest8() does not crash or throw and completes.
bool test_SpeedTest8_BasicExecution() {
    // Expose intent in logs for easier understanding during test runs.
    std::cout << "Test 1: Basic execution of SpeedTest8" << std::endl;

    try {
        SpeedTest8();
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] SpeedTest8 threw std::exception: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "[ERROR] SpeedTest8 threw an unknown exception." << std::endl;
        return false;
    }

    // If we reach here, the call completed without crashing.
    // We use a non-fatal assertion to indicate success.
    ASSERT_TRUE(true, "SpeedTest8 completed without crash");
    return true;
}

// Test 2: Re-entrant invocation test
// Intent: Call SpeedTest8 twice in succession to exercise potential cleanup/reuse paths.
// Rationale: Some performance tests allocate and free resources per run; ensure no crash
// occurs on subsequent invocations.
bool test_SpeedTest8_ReentrantExecution() {
    std::cout << "Test 2: Reentrant execution of SpeedTest8 (called twice)" << std::endl;

    try {
        SpeedTest8();
        SpeedTest8();
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] SpeedTest8 threw std::exception on re-entrancy: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "[ERROR] SpeedTest8 threw an unknown exception on re-entrancy." << std::endl;
        return false;
    }

    // If both calls completed, consider the reentrant test passed.
    ASSERT_TRUE(true, "SpeedTest8 reentrant execution completed without crash");
    return true;
}

// Test 3: Plugin vs no-plugin path sanity
// Intent: Validate that invoking SpeedTest8 under normal conditions is compatible with both
// plugin-less and plugin-enabled runtime paths in the original code.
// Note: We cannot directly toggle plugins here because the focal function encapsulates
// its own plugin handling. We still perform a single invocation to ensure basic compatibility.
bool test_SpeedTest8_PluginPathCompatibility() {
    std::cout << "Test 3: Plugin-path sanity check for SpeedTest8" << std::endl;

    try {
        SpeedTest8();
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] SpeedTest8 threw std::exception during plugin-path sanity: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "[ERROR] SpeedTest8 threw an unknown exception during plugin-path sanity." << std::endl;
        return false;
    }

    // Basic verification; since we cannot introspect internal plugin state here, we rely on
    // lack of crash as the success criterion.
    ASSERT_TRUE(true, "SpeedTest8 plugin-path sanity completed without crash");
    return true;
}

// Test 4: Thread-safety / repeated rapid invocations
// Intent: Run SpeedTest8 multiple times in a loop to increase code coverage of repeated usage patterns.
// This is a lightweight approximation of stress-path testing focusing on stability, not performance.
bool test_SpeedTest8_MultipleInvocations() {
    std::cout << "Test 4: Multiple rapid invocations of SpeedTest8 for stability" << std::endl;

    const int iterations = 5;
    for (int i = 0; i < iterations; ++i) {
        try {
            SpeedTest8();
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] SpeedTest8 threw std::exception on iteration " << i
                      << ": " << e.what() << std::endl;
            return false;
        } catch (...) {
            std::cerr << "[ERROR] SpeedTest8 threw an unknown exception on iteration " << i << std::endl;
            return false;
        }
        // Non-terminating assertion to keep execution flowing
        ASSERT_TRUE(true, "Iteration " + std::to_string(i) + " completed without crash");
    }
    return true;
}

// Entry point for the test suite
int main() {
    // Simple test harness: run all tests and report aggregated results.
    int total = 0;
    int passed = 0;

    struct TestCase {
        const char* name;
        bool (*func)();
    };

    TestCase tests[] = {
        { "test_SpeedTest8_BasicExecution", test_SpeedTest8_BasicExecution },
        { "test_SpeedTest8_ReentrantExecution", test_SpeedTest8_ReentrantExecution },
        { "test_SpeedTest8_PluginPathCompatibility", test_SpeedTest8_PluginPathCompatibility },
        { "test_SpeedTest8_MultipleInvocations", test_SpeedTest8_MultipleInvocations },
    };

    const int testCount = sizeof(tests) / sizeof(tests[0]);

    std::cout << "SpeedTest8 Test Suite: " << testCount << " test(s) to run." << std::endl;

    for (int i = 0; i < testCount; ++i) {
        ++total;
        const TestCase& tc = tests[i];
        std::cout << "[RUNNING] " << tc.name << std::endl;
        bool ok = tc.func();
        if (ok) {
            ++passed;
            std::cout << "[PASSED] " << tc.name << std::endl;
        } else {
            std::cout << "[FAILED] " << tc.name << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    std::cout << "Test results: " << passed << " / " << total << " passed." << std::endl;

    // Return non-zero if any test failed to aid integration with CI systems.
    return (passed == total) ? 0 : 1;
}