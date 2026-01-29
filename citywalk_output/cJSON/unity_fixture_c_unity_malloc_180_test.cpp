/*
Unit test suite for the focal method:
- unity_malloc(size_t size)

Notes:
- The focal code is part of Unity's C-based test harness. We provide a small C++11 test suite
  that exercises the expected behavior of unity_malloc via its C linkage.
- Tests avoid GTest and use a lightweight, non-terminating assertion mechanism to maximize code coverage.
- We rely on the existing control functions exposed by the focal module:
  - unity_malloc(size_t)
  - unity_free(void*)
  - UnityMalloc_StartTest()
  - UnityMalloc_EndTest()
  - UnityMalloc_MakeMallocFailAfterCount(int)
- The tests are designed to compile in a project where unity_fixture.c is part of the build.
  They should be compiled with a C++11 capable toolchain and linked against the Unity fixture
  (which provides the C implementations and control hooks).
- To explore the alternative path that uses the Unity heap (UNITY_EXCLUDE_STDLIB_MALLOC),
  you can compile the test with -DUNITY_EXCLUDE_STDLIB_MALLOC to exercise the heap-backed path
  in unity_malloc. This test suite focuses primarily on the public behavior that remains
  consistent across configurations.

Usage:
- Build with the Unity fixture in your project. If you want to exercise the non-stdlib malloc
  path, compile with -D UNITY_EXCLUDE_STDLIB_MALLOC as needed.
*/

#include <unity_internals.h>
#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <unity_fixture.h>


// Expose C linkage for Unity's memory management functions
extern "C" {
    void* unity_malloc(size_t size);
    void  unity_free(void* mem);

    // Control hooks provided by the Unity fixture
    void  UnityMalloc_StartTest(void);
    void  UnityMalloc_EndTest(void);
    void  UnityMalloc_MakeMallocFailAfterCount(int countdown);
}

/* Lightweight test harness
   - Non-terminating assertions: tests continue after a failure to maximize coverage.
   - Failure count is tracked and reported at the end.
*/
static int g_testFailures = 0;

#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILED] " << (msg) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_testFailures; \
    } \
} while(0)

// Test 1: Basic allocation path
// - Start a test, allocate a non-zero size, verify pointer is non-NULL, write/read
//   within allocated region, then free and end the test.
void test_unity_malloc_basic_allocation()
{
    UnityMalloc_StartTest();            // reset malloc counters for a clean test
    void* ptr = unity_malloc(16);       // allocate 16 bytes
    TEST_ASSERT(ptr != NULL, "unity_malloc should return non-NULL for size 16");

    if (ptr != NULL)
    {
        // Quick sanity check: write/read within allocated bounds.
        char* c = static_cast<char*>(ptr);
        c[0]  = 'A';
        c[15] = 'Z';
        TEST_ASSERT(c[0] == 'A' && c[15] == 'Z', "Allocated memory should be writable and readable");
    }

    if (ptr != NULL) {
        unity_free(ptr);                  // free to avoid leaking in EndTest
    }
    UnityMalloc_EndTest();
}

// Test 2: Zero-sized allocation should return NULL
void test_unity_malloc_zero_size()
{
    UnityMalloc_StartTest();
    void* ptr = unity_malloc(0);
    TEST_ASSERT(ptr == NULL, "unity_malloc(0) should return NULL");
    UnityMalloc_EndTest();
}

// Test 3: Malloc fail after countdown reaches zero
// - Set countdown to 0 and ensure unity_malloc returns NULL immediately.
void test_unity_malloc_fail_after_zero()
{
    // Force the malloc to fail immediately on the next call
    UnityMalloc_MakeMallocFailAfterCount(0);

    UnityMalloc_StartTest();
    void* ptr = unity_malloc(4);
    TEST_ASSERT(ptr == NULL, "unity_malloc should return NULL when countdown == 0");
    UnityMalloc_EndTest();
}

// Test 4: Multiple allocations and proper freeing to avoid leaks
void test_unity_malloc_multiple_allocs_and_free()
{
    UnityMalloc_StartTest();
    void* p1 = unity_malloc(8);
    void* p2 = unity_malloc(12);
    TEST_ASSERT(p1 != NULL && p2 != NULL, "Two consecutive allocations should succeed");

    if (p1 != NULL) unity_free(p1);
    if (p2 != NULL) unity_free(p2);
    UnityMalloc_EndTest();
}

// Simple test runner
int main()
{
    std::cout << "Running unity_malloc test suite (C++11, no GTest)..." << std::endl;

    test_unity_malloc_basic_allocation();
    test_unity_malloc_zero_size();
    test_unity_malloc_fail_after_zero();
    test_unity_malloc_multiple_allocs_and_free();

    if (g_testFailures > 0) {
        std::cerr << "Total test failures: " << g_testFailures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}