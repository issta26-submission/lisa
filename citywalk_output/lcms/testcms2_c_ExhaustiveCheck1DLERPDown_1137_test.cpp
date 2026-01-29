// Test suite for ExhaustiveCheck1DLERPDown using a linker wrap-based mock of Check1D.
// This test suite is designed to be compiled with C++11 and linked with the C source
// containing ExhaustiveCheck1DLERPDown (from testcms2.c). To enable function interposition,
// build with the linker option: -Wl,--wrap=Check1D
// Example (Linux with GCC/Clang): 
//   g++ -std=c++11 -c test_exhaustive_dl_erpdown.cpp
//   gcc -c testcms2.c
//   g++ test_exhaustive_dl_erpdown.o testcms2.o -Wl,--wrap=Check1D -o test_exhaustive_dl_erpdown
//
// The tests use non-terminating assertions to maximize coverage and ensure execution of the loop.

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Ensure we have C linkage for the ExhaustiveCheck1DLERPDown function when linking.
extern "C" cmsInt32Number ExhaustiveCheck1DLERPDown(void);

// Global control flags for the wrap-based tests
static int g_currentTest = 0;     // 1 = all succeed, 2 = fail on first call, others = default success
static int g_wrapCallCount = 0;   // Counts how many times Check1D is invoked
static int g_totalTestsRun = 0;   // Total tests executed
static int g_failures = 0;          // Non-terminating failure counter

// Lightweight non-terminating assertion macro to accumulate failures without aborting tests
#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Test failed: " << msg << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Linker-wrapped Check1D: this function will interpose all calls to Check1D inside ExhaustiveCheck1DLERPDown.
// The -Wl,--wrap=Check1D option ensures references to Check1D are redirected to __wrap_Check1D.
extern "C" cmsInt32Number __wrap_Check1D(cmsInt32Number nNodesToCheck, cmsBool Down, cmsInt32Number max_err) {
    // Count how many times Check1D is invoked to verify loop behavior
    ++g_wrapCallCount;

    // Silently ignore input values for the purposes of unit testing coverage
    (void)nNodesToCheck;
    (void)Down;
    (void)max_err;

    // Behavior controlled by test scenario
    if (g_currentTest == 1) {
        // Test 1: Always succeed
        return 1;
    } else if (g_currentTest == 2) {
        // Test 2: Fail on the very first call to Check1D
        return 0;
    } else {
        // Default: succeed
        return 1;
    }
}

// Test Case 1: All j iterations succeed (Check1D returns 1 for every j from 10 to 4096)
// Expected outcome: ExhaustiveCheck1DLERPDown returns 1 and Make sure the loop iterated 4087 times.
void test_exhaustive_down_all_pass() {
    // Reset state for a clean test run
    g_wrapCallCount = 0;
    g_currentTest = 1;
    g_totalTestsRun++;

    cmsInt32Number result = ExhaustiveCheck1DLERPDown();

    TEST_ASSERT(result == 1, "ExhaustiveCheck1DLERPDown should return 1 when every Check1D call returns 1");
    TEST_ASSERT(g_wrapCallCount == 4087, "Check1D should be called exactly once per loop iteration (4087 times) for j in [10, 4096]");
}

// Test Case 2: Check1D fails on the very first call
// Expected outcome: ExhaustiveCheck1DLERPDown returns 0 immediately and only the first call to Check1D occurs.
void test_exhaustive_down_first_fail() {
    // Reset state for a clean test run
    g_wrapCallCount = 0;
    g_currentTest = 2;
    g_totalTestsRun++;

    cmsInt32Number result = ExhaustiveCheck1DLERPDown();

    TEST_ASSERT(result == 0, "ExhaustiveCheck1DLERPDown should return 0 if the first Check1D call fails");
    TEST_ASSERT(g_wrapCallCount == 1, "Check1D should be invoked exactly once when failure occurs on first iteration");
}

// Main function: executes all test cases and reports summary.
// Per domain knowledge, avoid terminating on first failure to maximize coverage.
int main() {
    // Run tests
    test_exhaustive_down_all_pass();
    test_exhaustive_down_first_fail();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed (" << g_totalTestsRun << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed out of " << g_totalTestsRun << "." << std::endl;
        return 1;
    }
}