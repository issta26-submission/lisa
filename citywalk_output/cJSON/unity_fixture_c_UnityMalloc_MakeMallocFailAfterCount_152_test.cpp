// Test suite for UnityMalloc_MakeMallocFailAfterCount in unity_fixture.c
// This test is written in C++11 but targets the C API exposed by unity_fixture.c.
// It exercises how the malloc failure countdown is configured and how it affects
// allocations performed via the Unity malloc wrapper (unity_malloc).
// The tests avoid terminating assertions; instead, they return boolean results
// and a small harness reports pass/fail status to maximize coverage across runs.

#include <unity_internals.h>
#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <unity_fixture.h>


// Forward declare the C API from the Unity test framework.
// These functions are defined in unity_fixture.c and have C linkage.
extern "C" {
    void UnityMalloc_StartTest(void);
    void UnityMalloc_EndTest(void);
    void UnityMalloc_MakeMallocFailAfterCount(int countdown);
    void* unity_malloc(size_t size);
}

// Simple test harness state
struct TestResult {
    const char* name;
    bool passed;
};

// Test 1: When countdown is negative (MALLOC_DONT_FAIL), allocations should succeed.
bool test_MakeMallocFailAfterCount_NegativeDisablesFailure() {
    // Reset test state
    UnityMalloc_StartTest();
    // MALLOC_DONT_FAIL is defined as -1 in the file; use -1 to indicate no failures
    UnityMalloc_MakeMallocFailAfterCount(-1);

    void* p = unity_malloc(32); // attempt a small allocation
    bool ok = (p != nullptr);

    // Clean up if allocation unexpectedly succeeded
    if (p != nullptr) {
        free(p);
    }

    return ok;
}

// Test 2: Countdown = 0 means the very first allocation should fail.
bool test_MakeMallocFailAfterCount_ZeroFailsFirst() {
    UnityMalloc_StartTest();
    UnityMalloc_MakeMallocFailAfterCount(0);

    void* p1 = unity_malloc(16); // first allocation should fail (return NULL)
    bool firstFailed = (p1 == nullptr);

    void* p2 = unity_malloc(16); // subsequent allocation may succeed depending on impl
    bool secondMaybeSucceeds = (p2 != nullptr);

    if (p1 != nullptr) free(p1); // defensive
    if (p2 != nullptr) free(p2);

    // Expect the first call to fail
    return firstFailed;
}

// Test 3: Countdown = 1 should fail on the first allocation, and succeed on the second.
bool test_MakeMallocFailAfterCount_FailsThenSucceeds() {
    UnityMalloc_StartTest();
    UnityMalloc_MakeMallocFailAfterCount(1);

    void* p1 = unity_malloc(8);  // first allocation expected to fail
    bool firstFailed = (p1 == nullptr);

    void* p2 = unity_malloc(8);  // second allocation should succeed
    bool secondSucceeds = (p2 != nullptr);

    if (p1 != nullptr) free(p1);
    if (p2 != nullptr) free(p2);

    // Both conditions must hold for this test to pass
    return firstFailed && secondSucceeds;
}

// Utility to run all tests and report results
int main() {
    TestResult results[3] = {
        {"Negative countdown disables fail (no failure expected)", false},
        {"Countdown 0 causes first allocation to fail", false},
        {"Countdown 1 fails first then succeeds", false}
    };

    // Run Test 1
    results[0].passed = test_MakeMallocFailAfterCount_NegativeDisablesFailure();

    // Run Test 2
    results[1].passed = test_MakeMallocFailAfterCount_ZeroFailsFirst();

    // Run Test 3
    results[2].passed = test_MakeMallocFailAfterCount_FailsThenSucceeds();

    // Report results
    bool allPassed = true;
    std::cout << "UnityMalloc_MakeMallocFailAfterCount test suite results:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "  - " << results[i].name << ": "
                  << (results[i].passed ? "PASS" : "FAIL") << "\n";
        allPassed = allPassed && results[i].passed;
    }

    if (!allPassed) {
        std::cout << "Some tests failed. See logs for details.\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}