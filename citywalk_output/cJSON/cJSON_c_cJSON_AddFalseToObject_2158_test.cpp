/*
 * Unit test suite for cJSON_AddFalseToObject (C API, tested via C++11 driver)
 *
 * Step 1 - Program Understanding (high level)
 * - Focused method: cJSON_AddFalseToObject(cJSON * const object, const char * const name)
 * - Behaviour:
 *     - Creates a new JSON false item: cJSON_CreateFalse()
 *     - Attempts to attach it to the provided object under the given key using add_item_to_object(...)
 *     - If attachment succeeds, returns the newly created item
 *     - If attachment fails, deletes the created item and returns NULL
 *
 * Step 2 - Unit Test Generation (guided by dependencies)
 * - We exercise:
 *     - True path: valid object and valid name -> returns non-NULL and item appears in object
 *     - False path: invalid object type (e.g., an array) -> returns NULL
 *     - False path: NULL object pointer -> returns NULL
 *     - False path: NULL name pointer -> returns NULL (key must be valid)
 *
 * Step 3 - Test Case Refinement
 * - Self-contained test harness (no GTest)
 * - Non-terminating assertions: we accumulate failures and report at the end
 * - Use only standard library facilities and the provided cJSON API
 * - Access static-like behavior via public API; do not rely on private internals
 *
 * Notes:
 * - The tests rely on cJSON.h / cJSON.c being part of the build. Link with cJSON.c when compiling.
 * - Tests are written for C++11, but call into a C API. Wrap the C headers with extern "C" when including in C++.
 * - The test suite prints a summary and returns non-zero on failure.
 */

// Candidate Keywords extracted from the focal method and dependencies
// { cJSON_AddFalseToObject, cJSON_CreateFalse, add_item_to_object, global_hooks, cJSON, NULL, object, name, false_item, cJSON_Delete }

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Include the C library header with C linkage
extern "C" {
}

static int tests_run = 0;
static int tests_failed = 0;

// Lightweight non-terminating assertion helper
static inline void expect_true(bool condition, const char* message)
{
    ++tests_run;
    if (!condition) {
        ++tests_failed;
        fprintf(stderr, "Test failure: %s\n", message);
    }
}

// Test 1: Success path
// - Create a JSON object, add a false value under key "flag"
// - Expect non-NULL return, and that the object contains an item named "flag" with value false
// - The returned pointer should point to the same item stored in the object (pointer equality)
static void test_AddFalseToObject_Succeeds(void)
{
    cJSON *obj = cJSON_CreateObject();
    expect_true(obj != NULL, "CreateObject failed unexpectedly");

    cJSON *ret = cJSON_AddFalseToObject(obj, "flag");
    expect_true(ret != NULL, "AddFalseToObject should return non-NULL on valid object");

    // Verify the object now contains the key and that its value is false
    cJSON *item_in_object = cJSON_GetObjectItemCaseSensitive(obj, "flag");
    expect_true(item_in_object != NULL, "Object should contain key 'flag' after addition");
    // cJSON provides a helper to check for boolean false
    bool is_false = cJSON_IsFalse(item_in_object) ? true : false;
    expect_true(is_false, "The value associated with 'flag' should be false");

    // The returned pointer should point to the same item as retrieved from the object
    expect_true(ret == item_in_object, "Returned item should be the same as the object item");

    cJSON_Delete(obj);
}

// Test 2: Failure path - invalid object type (array instead of object)
// - Expect NULL and no item added to the array
static void test_AddFalseToObject_WithArrayFails(void)
{
    cJSON *arr = cJSON_CreateArray();
    expect_true(arr != NULL, "CreateArray failed unexpectedly");

    cJSON *ret = cJSON_AddFalseToObject(arr, "flag");
    expect_true(ret == NULL, "AddFalseToObject should return NULL when object is not an object");

    cJSON_Delete(arr);
}

// Test 3: Failure path - NULL object pointer
// - Expect NULL and no crash
static void test_AddFalseToObject_WithNullObjectFails(void)
{
    cJSON *ret = cJSON_AddFalseToObject(NULL, "flag");
    expect_true(ret == NULL, "AddFalseToObject should return NULL when object is NULL");
}

// Test 4: Failure path - NULL name pointer
// - Expect NULL and no crash
static void test_AddFalseToObject_WithNullNameFails(void)
{
    cJSON *obj = cJSON_CreateObject();
    expect_true(obj != NULL, "CreateObject failed unexpectedly");

    cJSON *ret = cJSON_AddFalseToObject(obj, NULL);
    expect_true(ret == NULL, "AddFalseToObject should return NULL when name is NULL");

    cJSON_Delete(obj);
}

// Entry point for running all tests
int main(void)
{
    test_AddFalseToObject_Succeeds();
    test_AddFalseToObject_WithArrayFails();
    test_AddFalseToObject_WithNullObjectFails();
    test_AddFalseToObject_WithNullNameFails();

    // Summary
    printf("Total tests run: %d\n", tests_run);
    printf("Total failures: %d\n", tests_failed);

    // Return non-zero if any test failed
    return (tests_failed != 0) ? 1 : 0;
}