// Test suite: Unit tests for cJSON_CreateBool in cJSON.c
// This test file is intended to be compiled as C++11 code and linked with the C source
// file that provides cJSON.c (the focal implementation).
//
// Approach outline (embedded as comments for clarity):
// - Step 1 (Program Understanding): The focal method creates a new cJSON item via
//   cJSON_New_Item(&global_hooks) and sets its type to cJSON_True or cJSON_False
//   depending on the boolean parameter. If item allocation fails, it returns NULL.
//   Key dependent components: cJSON_New_Item, global_hooks (via internal hooks), 
//   cJSON_True, cJSON_False, and the cJSON_CreateBool wrapper.
// - Step 2 (Unit Test Generation): Test true branch, false branch, and allocation
//   failure path (by injecting a failing malloc through cJSON_InitHooks and
//   cJSON_Hooks).
// - Step 3 (Test Case Refinement): Include tests for basic boolean properties
//   (IsBool, IsTrue/IsFalse) and ensure proper cleanup. Use a minimal in-process
//   test harness (no GTest) with non-terminating assertions that accumulate
//   failures and print a summary at the end.
//
// Candidate Keywords derived from the focal method and its dependencies:
// - cJSON_CreateBool, cJSON_New_Item, global_hooks, internal_hooks, cJSON_True, cJSON_False
// - cJSON_IsBool, cJSON_IsTrue, cJSON_IsFalse, cJSON_Delete
// - cJSON_Hooks, cJSON_InitHooks for dependency injection (malloc_fn, free_fn)
// - Allocation failure scenario via custom malloc_fn
// - Non-terminating test assertions (log failures, return status at end)

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Global test failure counter to allow non-terminating test execution.
static int g_test_failures = 0;

// A failing malloc function to simulate allocation failure in cJSON_CreateBool.
// Must match the signature: void* (*malloc_fn)(size_t)
static void* fail_malloc(size_t /*size*/) {
    return NULL;
}

// Test 1: Verify that cJSON_CreateBool(true) creates a non-null item with type cJSON_True
// and that it is recognized as a boolean value by the helper predicates.
static void test_CreateBool_true() {
    std::cout << "Running test_CreateBool_true..." << std::endl;

    cJSON *item = cJSON_CreateBool(true);
    if (item == NULL) {
        std::cerr << "test_CreateBool_true: FAILED - cJSON_CreateBool(true) returned NULL" << std::endl;
        ++g_test_failures;
        return;
    }

    // Expect the internal type to be cJSON_True
    if (item->type != cJSON_True) {
        std::cerr << "test_CreateBool_true: FAILED - item->type expected cJSON_True, got " << item->type << std::endl;
        ++g_test_failures;
    }

    // Expect the boolean predicates to reflect true
    if (!cJSON_IsBool(item)) {
        std::cerr << "test_CreateBool_true: FAILED - cJSON_IsBool failed for true item" << std::endl;
        ++g_test_failures;
    }
    if (!cJSON_IsTrue(item)) {
        std::cerr << "test_CreateBool_true: FAILED - cJSON_IsTrue returned false for true item" << std::endl;
        ++g_test_failures;
    }

    cJSON_Delete(item);
    std::cout << "test_CreateBool_true completed." << std::endl;
}

// Test 2: Verify that cJSON_CreateBool(false) creates a non-null item with type cJSON_False
// and that it is recognized as a boolean value by the helper predicates.
static void test_CreateBool_false() {
    std::cout << "Running test_CreateBool_false..." << std::endl;

    cJSON *item = cJSON_CreateBool(false);
    if (item == NULL) {
        std::cerr << "test_CreateBool_false: FAILED - cJSON_CreateBool(false) returned NULL" << std::endl;
        ++g_test_failures;
        return;
    }

    // Expect the internal type to be cJSON_False
    if (item->type != cJSON_False) {
        std::cerr << "test_CreateBool_false: FAILED - item->type expected cJSON_False, got " << item->type << std::endl;
        ++g_test_failures;
    }

    // Expect the boolean predicates to reflect false
    if (!cJSON_IsBool(item)) {
        std::cerr << "test_CreateBool_false: FAILED - cJSON_IsBool failed for false item" << std::endl;
        ++g_test_failures;
    }
    if (!cJSON_IsFalse(item)) {
        std::cerr << "test_CreateBool_false: FAILED - cJSON_IsFalse returned false for false item" << std::endl;
        ++g_test_failures;
    }

    cJSON_Delete(item);
    std::cout << "test_CreateBool_false completed." << std::endl;
}

// Test 3: Allocation failure path by injecting a failing malloc via cJSON_InitHooks.
// This ensures cJSON_CreateBool returns NULL when memory allocation fails.
static void test_CreateBool_allocation_failure() {
    std::cout << "Running test_CreateBool_allocation_failure..." << std::endl;

    // Install failing allocator
    cJSON_Hooks failing_hooks;
    failing_hooks.malloc_fn = fail_malloc;
    failing_hooks.free_fn = free;
    cJSON_InitHooks(&failing_hooks);

    cJSON *item = cJSON_CreateBool(true);
    if (item != NULL) {
        std::cerr << "test_CreateBool_allocation_failure: FAILED - expected NULL due to allocation failure, got non-null" << std::endl;
        ++g_test_failures;
        cJSON_Delete(item); // cleanup if it unexpectedly allocated
    }

    // Revert to default allocator by passing NULL hooks (as per library behavior)
    cJSON_Hooks default_hooks = { NULL, NULL };
    cJSON_InitHooks(&default_hooks);

    std::cout << "test_CreateBool_allocation_failure completed." << std::endl;
}

// Simple test harness
int main() {
    std::cout << "Starting test suite for cJSON_CreateBool...\n";

    test_CreateBool_true();
    test_CreateBool_false();
    test_CreateBool_allocation_failure();

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
    }

    // Return nonzero if any test failed
    return g_test_failures;
}