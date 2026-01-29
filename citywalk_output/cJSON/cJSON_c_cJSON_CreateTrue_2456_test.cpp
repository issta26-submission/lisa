// Test suite for cJSON_CreateTrue using C++11 (no GTest). 
// This file exercises the focal method by linking against the C cJSON library.

#include <cstddef>
#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Ensure C linkage for C library functions when compiling as C++
extern "C" {
}

// ------------- Test Helpers -------------
// Global counters to report test results without terminating on first failure.
static int tests_run = 0;
static int tests_failed = 0;

// Non-terminating assertion: true check
#define EXPECT_TRUE(cond, msg) \
    do { \
        ++tests_run; \
        if(!(cond)) { \
            ++tests_failed; \
            std::cerr << "EXPECT_TRUE failed: " << (msg) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        } \
    } while (false)

// Non-terminating assertion: non-null pointer
#define EXPECT_NOT_NULL(ptr, msg) \
    do { \
        ++tests_run; \
        if((ptr) == nullptr) { \
            ++tests_failed; \
            std::cerr << "EXPECT_NOT_NULL failed: " << (msg) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        } \
    } while (false)


// ------------- Test Utilities for Allocation Failure -------------
// Provide a custom allocation function to simulate allocation failure.
// The internal hooks used by cJSON_New_Item are supplied via cJSON_InitHooks.
static void* test_malloc(size_t size)
{
    (void)size;
    // Simulate allocation failure by always returning NULL.
    return NULL;
}

static void test_free(void* ptr)
{
    // No-op since allocation never succeeded; provided for API completeness.
    (void)ptr;
}

// ------------- Test Cases -------------
/*
    Test 1: Normal path of cJSON_CreateTrue
    - Expect a non-NULL cJSON item.
    - Expect the item's type to be cJSON_True.
    - Validate with cJSON_IsTrue(item) as an additional correctness check.
    - Cleanup with cJSON_Delete to avoid memory leaks.
*/
static void test_CreateTrue_NormalPath()
{
    // Exercise: direct call to create a true value
    cJSON *item = cJSON_CreateTrue();

    EXPECT_NOT_NULL(item, "cJSON_CreateTrue should return a non-NULL item on normal path");
    if (item != nullptr)
    {
        EXPECT_TRUE(item->type == cJSON_True, "Item type should be cJSON_True after cJSON_CreateTrue");
        // Additional semantic check using public API
        EXPECT_TRUE(cJSON_IsTrue(item), "cJSON_IsTrue should return true for a true item");
        cJSON_Delete(item);
    }
}

/*
    Test 2: Allocation failure path of cJSON_CreateTrue
    - Inject a failing malloc_fn via cJSON_InitHooks to force cJSON_New_Item to fail.
    - Expect cJSON_CreateTrue to return NULL.
    - Reset hooks to default to avoid affecting other tests.
*/
static void test_CreateTrue_AllocationFailure()
{
    // Prepare hooks with a malloc_fn that always fails
    cJSON_Hooks hooks;
    hooks.malloc_fn = test_malloc;
    hooks.free_fn   = test_free;
    hooks.realloc_fn = NULL; // Some builds may have this field; set to NULL if present.

    // Install the failing allocator
    cJSON_InitHooks(&hooks);

    // Now, allocating a new item inside cJSON_CreateTrue should fail
    cJSON *item = cJSON_CreateTrue();
    EXPECT_TRUE(item == nullptr, "cJSON_CreateTrue should return NULL when allocation fails");

    // Reset to default hooks to avoid side effects for subsequent tests
    cJSON_InitHooks(NULL);
}


// ------------- Main Runner -------------
int main()
{
    std::cout << "Running test suite for cJSON_CreateTrue (C API) using C++11." << std::endl;

    test_CreateTrue_NormalPath();
    test_CreateTrue_AllocationFailure();

    if (tests_failed == 0)
    {
        std::cout << "All tests passed (" << tests_run << " checks)." << std::endl;
        return 0;
    }
    else
    {
        std::cerr << tests_failed << " tests failed out of " << tests_run << " checks." << std::endl;
        return 1;
    }
}