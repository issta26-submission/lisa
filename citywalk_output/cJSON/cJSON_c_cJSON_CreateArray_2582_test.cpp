/*
Unit test suite for the focal method:
    CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void)

Goal:
- Validate that cJSON_CreateArray returns a non-null cJSON item with type set to cJSON_Array.
- Exercise the "false" branch of the internal allocation by injecting a failing allocator via cJSON_InitHooks.
- Ensure multiple invocations yield distinct objects.
- Use a lightweight, non-GTest, C++11 compatible test harness with explanatory comments for each test.

Notes:
- The test relies on cJSON_InitHooks to install custom memory hooks (malloc_fn) for simulating allocation failures.
- All tests are non-terminating (they report pass/fail and continue).
- After tests that alter hooks, the test restores default behavior with a reset (cJSON_InitHooks(NULL)) when supported by the library.
*/

#include <cstddef>
#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// If the project exposes the hooks through an extern, declare it to be accessible.
// (Some cJSON builds expose this; if not, cJSON_InitHooks mechanism is used in tests.)
/* We intentionally avoid relying on a specific internal symbol layout beyond what's provided
   by the public API (cJSON_InitHooks and cJSON_Hooks). */

// Utility to report test results in a non-terminating fashion
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_PASS(name) \
    do { \
        std::cout << "[PASS] " << name << "\n"; \
        ++g_tests_run; \
        ++g_tests_passed; \
    } while (0)

#define TEST_FAIL(name, msg) \
    do { \
        std::cout << "[FAIL] " << name << ": " << msg << "\n"; \
        ++g_tests_run; \
    } while (0)

#define ASSERT_TRUE(cond, name) \
    do { \
        if (cond) { TEST_PASS(name); } \
        else { TEST_FAIL(name, "condition is false"); } \
    } while (0)

#define ASSERT_NULL(ptr, name) \
    do { \
        if ((ptr) == NULL) { TEST_PASS(name); } \
        else { TEST_FAIL(name, "pointer is non-null"); } \
    } while (0)

#define ASSERT_NOT_NULL(ptr, name) \
    do { \
        if ((ptr) != NULL) { TEST_PASS(name); } \
        else { TEST_FAIL(name, "pointer is null"); } \
    } while (0)

static void* failing_malloc(size_t /*size*/) {
    // Simulate allocation failure
    return NULL;
}
static void failing_free(void* /*ptr*/) {
    // No-op for this test
}

// Test 1: Basic positive path
// - When memory allocation succeeds, cJSON_CreateArray should return a non-null item.
// - The returned item's type must be cJSON_Array.
bool test_CreateArray_ReturnsArrayWithCorrectType() {
    // Attempt to create a new array object
    cJSON *array = cJSON_CreateArray();

    // Validate non-null and correct type
    bool ok = (array != NULL) && (array->type == cJSON_Array);

    // Cleanup
    if (array) {
        cJSON_Delete(array);
    }

    return ok;
}

// Test 2: False branch via allocation failure
// - Inject a failing malloc via cJSON_InitHooks to ensure cJSON_CreateArray returns NULL when allocation fails.
bool test_CreateArray_FalseBranchOnMallocFailure() {
    // Prepare hooks that force malloc to fail
    cJSON_Hooks hooks;
    hooks.malloc_fn = failing_malloc;
    hooks.free_fn = failing_free;

    // Install failing hooks
    cJSON_InitHooks(&hooks);

    // Now, cJSON_CreateArray should attempt allocation and fail, returning NULL
    cJSON *array = cJSON_CreateArray();

    bool ok = (array == NULL);

    // Cleanup and restore default hooks
    if (array) {
        cJSON_Delete(array);
    }
    // Restore to default memory hooks (best-effort; pass NULL if allowed by library)
    cJSON_InitHooks(NULL);

    return ok;
}

// Test 3: Multiple calls produce distinct objects
// - Ensure that consecutive calls yield different memory addresses (distinct items).
bool test_CreateArray_MultipleCallsAreDistinct() {
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();

    bool distinct = false;
    if (array1 != NULL && array2 != NULL) {
        distinct = (array1 != array2);
        // Cleanup
        cJSON_Delete(array1);
        cJSON_Delete(array2);
    } else {
        // If either failed to allocate, consider test failed for distinctness
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
    }

    return distinct;
}

int main() {
    std::cout << "Running tests for cJSON_CreateArray...\n";

    int total = 0;
    int passed = 0;

    // Test 1
    {
        ++total;
        bool result = test_CreateArray_ReturnsArrayWithCorrectType();
        if (result) { ++passed; TEST_PASS("Test_CreateArray_ReturnsArrayWithCorrectType"); }
        else { TEST_FAIL("Test_CreateArray_ReturnsArrayWithCorrectType", "unexpected result"); }
    }

    // Test 2
    {
        ++total;
        bool result = test_CreateArray_FalseBranchOnMallocFailure();
        if (result) { ++passed; TEST_PASS("Test_CreateArray_FalseBranchOnMallocFailure"); }
        else { TEST_FAIL("Test_CreateArray_FalseBranchOnMallocFailure", "allocation failure not detected"); }
    }

    // Test 3
    {
        ++total;
        bool result = test_CreateArray_MultipleCallsAreDistinct();
        if (result) { ++passed; TEST_PASS("Test_CreateArray_MultipleCallsAreDistinct"); }
        else { TEST_FAIL("Test_CreateArray_MultipleCallsAreDistinct", "consecutive arrays are not distinct"); }
    }

    // Summary
    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";
    // Return non-zero if any test failed, to integrate with simple CI that checks exit code.
    return (passed == total) ? 0 : 1;
}