/*
 * C++11 unit test suite for the focal method: CheckComputeIncrements
 * 
 * Notes:
 * - The focal method is defined in a C source (fast_float_testbed.c) and the
 *   function signature in the C world is cmsInt32Number CheckComputeIncrements(void);
 * - We assume cmsInt32Number maps to a 32-bit int, which is compatible with C/C++ int.
 * - We link against the C file by declaring the function with C linkage in this C++ test.
 * - The test harness is a lightweight non-terminating harness (no GTest) that
 *   reports pass/fail via standard output and returns a non-zero exit code on failure.
 * - We focus on exercising the focal method invocation path and verify the return value.
 * - We provide explanatory comments for each test case.
 */

#include <fast_float_internal.h>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>


// Forward declaration of the focal function from the C file.
// We assume cmsInt32Number maps to a 32-bit int.
extern "C" int CheckComputeIncrements(void);

// Lightweight non-terminating test harness
namespace TestHarness {

struct TestCase {
    std::string name;
    std::function<bool()> func;
};

static int g_total = 0;
static int g_failed = 0;

static void log_pass(const std::string& name) {
    std::cout << "[PASSED] " << name << "\n";
}

static void log_fail(const std::string& name, const std::string& reason) {
    ++g_failed;
    std::cerr << "[FAILED] " << name << " : " << reason << "\n";
}

static void run_case(const TestCase& tc) {
    ++g_total;
    bool ok = false;
    try {
        ok = tc.func();
    } catch (...) {
        ok = false;
        log_fail(tc.name, "exception thrown");
    }
    if (ok) log_pass(tc.name);
    else log_fail(tc.name, "test returned false");
}

static void summarize() {
    std::cout << "Test summary: " << g_total << " case(s) run, "
              << (g_failed) << " failure(s)\n";
}

} // namespace TestHarness

// Test 1: Validate that the focal function CheckComputeIncrements returns 1
// This directly asserts the primary contract of the function as seen in the focal code.
static bool test_CheckComputeIncrements_ReturnsOne() {
    int res = CheckComputeIncrements();
    if (res != 1) {
        // Provide a detailed reason for failure without terminating the test run.
        std::cerr << "CheckComputeIncrements() returned " << res
                  << " but expected 1.\n";
        return false;
    }
    return true;
}

// Test 2: Call the focal function multiple times to ensure stability over repeated invocations.
// The function body in the focal code does not seem to depend on internal state across calls,
// but this test ensures no crash or undefined behavior occurs under repeated use.
static bool test_CheckComputeIncrements_MultipleCalls() {
    const int repeats = 5;
    for (int i = 0; i < repeats; ++i) {
        int res = CheckComputeIncrements();
        if (res != 1) {
            std::cerr << "Iteration " << i << ": CheckComputeIncrements() returned "
                      << res << ", expected 1.\n";
            return false;
        }
    }
    return true;
}

// Test 3: Basic smoke test to ensure the function is callable and does not reject with non-zero.
// This is a guard against accidental changes that could cause the function to bail out.
static bool test_CheckComputeIncrements_Smoke() {
    // A single invocation to check for stability.
    int res = CheckComputeIncrements();
    if (res != 1) {
        std::cerr << "Smoke test failed: CheckComputeIncrements() returned "
                  << res << ", expected 1.\n";
        return false;
    }
    // A second invocation to exercise potential internal state transitions.
    res = CheckComputeIncrements();
    if (res != 1) {
        std::cerr << "Smoke test (second call) failed: returned "
                  << res << ", expected 1.\n";
        return false;
    }
    return true;
}

int main() {
    using namespace TestHarness;

    // Assemble test suite with explanatory comments per test
    std::vector<TestCase> suite = {
        { "CheckComputeIncrements_ReturnsOne", test_CheckComputeIncrements_ReturnsOne },
        { "CheckComputeIncrements_MultipleCalls", test_CheckComputeIncrements_MultipleCalls },
        { "CheckComputeIncrements_Smoke", test_CheckComputeIncrements_Smoke }
    };

    // Run tests
    for (const auto& tc : suite) {
        run_case(tc);
    }

    summarize();

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}