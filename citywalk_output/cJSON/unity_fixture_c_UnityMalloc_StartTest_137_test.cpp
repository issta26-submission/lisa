/*
 * Lightweight C++11 unit tests for UnityMalloc_StartTest and related memory-mock helpers
 * 
 * Notes:
 * - This test suite targets the focal function UnityMalloc_StartTest(void) and related
 *   memory-control routines exposed in unity_fixture.c (via extern "C" interfaces).
 * - We avoid any GTest/GMock and instead use a tiny, self-contained test harness based on
 *   standard C++ I/O and simple boolean assertions to maximize portability.
 * - The tests rely on the public C interfaces declared below:
 *     - void UnityMalloc_StartTest(void)
 *     - void UnityMalloc_EndTest(void)
 *     - void UnityMalloc_MakeMallocFailAfterCount(int countdown)
 *     - void* unity_malloc(size_t size)
 *     - void  unity_free(void* mem)
 * - These tests assume underlying malloc-like behavior provided by the Unity fixture (unity_fixture.c)
 *   where a countdown determines when malloc should fail, and EndTest reports leaks if any malloc
 *   occurred without a matching free.
 * - This suite does not integrate with GTest; it runs in a single executable and reports PASS/FAIL
 *   per test via stdout.
 *
 * Important:
 * - The test harness does not terminate on a single test failure; it reports all results.
 * - If the environment/compiler provides the Unity framework, these tests should still compile
 *   and link as long as the C interfaces are available with C linkage.
 */

#include <unity_internals.h>
#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unity_fixture.h>


extern "C" {
    // Public interfaces provided by unity_fixture.c (focal class)
    void UnityMalloc_StartTest(void);
    void UnityMalloc_EndTest(void);
    void UnityMalloc_MakeMallocFailAfterCount(int countdown);
    void* unity_malloc(size_t size);
    void  unity_free(void* mem);
}

/* Simple test harness state */
static bool test_passed = true;

/* Utility to print test result and reset for next test */
static void report_test(const std::string& test_name, bool passed)
{
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

/*
 * Test 1: UnityMalloc_StartTest_AllowsAllocation
 * 
 * Rationale:
 * UnityMalloc_StartTest should reset the internal tracking (conceptually) and allow allocations
 * to proceed during the test until EndTest is called (which checks for leaks).
 * We verify that a basic allocation path via the provided unity_malloc function is usable
 * immediately after StartTest.
 */
static void test_UnityMalloc_StartTest_AllowsAllocation()
{
    test_passed = true;
    UnityMalloc_StartTest();

    // Attempt a small allocation; expect a non-null pointer under normal operation.
    void* p = unity_malloc(16);
    if (p == nullptr) {
        test_passed = false;
        std::cerr << "test_UnityMalloc_StartTest_AllowsAllocation: malloc returned NULL unexpectedly." << std::endl;
    } else {
        // Free the allocated memory to avoid polluting subsequent tests
        unity_free(p);
    }

    UnityMalloc_EndTest();

    report_test("UnityMalloc_StartTest_AllowsAllocation", test_passed);
}

/*
 * Test 2: UnityMalloc_MakeMallocFailAfterCount_Behavior
 *
 * Rationale:
 * When a countdown is set via UnityMalloc_MakeMallocFailAfterCount(0), the next allocation
 * should fail (return NULL). After resetting the countdown (e.g., -1 to indicate never fail),
 * allocations should succeed again.
 */
static void test_UnityMalloc_MakeMallocFailAfterCount_Behavior()
{
    test_passed = true;

    UnityMalloc_StartTest();
    // Force the next allocation to fail
    UnityMalloc_MakeMallocFailAfterCount(0);
    void* p = unity_malloc(8);
    if (p != nullptr) {
        test_passed = false;
        std::cerr << "test_UnityMalloc_MakeMallocFailAfterCount_Behavior: expected malloc failure, got non-NULL." << std::endl;
        // If it unexpectedly succeeded, free it to avoid leaks affecting subsequent tests
        unity_free(p);
    }

    // Restore normal malloc behavior
    UnityMalloc_MakeMallocFailAfterCount(-1);
    p = unity_malloc(8);
    if (p == nullptr) {
        test_passed = false;
        std::cerr << "test_UnityMalloc_MakeMallocFailAfterCount_Behavior: expected malloc to succeed after reset, but got NULL." << std::endl;
    } else {
        unity_free(p);
    }

    UnityMalloc_EndTest();

    report_test("UnityMalloc_MakeMallocFailAfterCount_Behavior", test_passed);
}

/*
 * Test 3: UnityMalloc_StartTest_RepeatStart_NoLeak
 *
 * Rationale:
 * Starting a test multiple times should not cause crash or inconsistent behavior. This test
 * runs two back-to-back StartTest/EndTest sequences with a small allocation in between.
 */
static void test_UnityMalloc_StartTest_RepeatStart_NoLeak()
{
    test_passed = true;

    // First run
    UnityMalloc_StartTest();
    void* p1 = unity_malloc(4);
    if (p1 == nullptr) {
        test_passed = false;
        std::cerr << "test_UnityMalloc_StartTest_RepeatStart_NoLeak: first allocation failure." << std::endl;
    } else {
        unity_free(p1);
    }
    UnityMalloc_EndTest();

    // Second run
    UnityMalloc_StartTest();
    void* p2 = unity_malloc(4);
    if (p2 == nullptr) {
        test_passed = false;
        std::cerr << "test_UnityMalloc_StartTest_RepeatStart_NoLeak: second allocation failure." << std::endl;
    } else {
        unity_free(p2);
    }
    UnityMalloc_EndTest();

    report_test("UnityMalloc_StartTest_RepeatStart_NoLeak", test_passed);
}

/* Main: run all tests */
int main()
{
    std::cout << "Starting UnityMalloc_StartTest test suite (C++ harness)" << std::endl;

    test_UnityMalloc_StartTest_AllowsAllocation();
    test_UnityMalloc_MakeMallocFailAfterCount_Behavior();
    test_UnityMalloc_StartTest_RepeatStart_NoLeak();

    std::cout << "Test suite finished." << std::endl;
    return 0;
}