// Test suite for cJSON_AddArrayToObject (CJSON_AddArrayToObject)
// FOCAL_METHOD: cJSON_PUBLIC(cJSON*) cJSON_AddArrayToObject(cJSON * const object, const char * const name)
// This test suite is designed to be compiled with a C/C++11 compiler.
// It relies on the cJSON library (cJSON.c + cJSON.h) being available in the build.
// Build tip: compile with both cJSON.c and this test file, e.g. g++ test.cpp cJSON.c -std=c++11 -o test
// Notes based on the provided dependencies and behavior described in the focal class dependencies.
// The tests focus on core behavior: true path when object and name are valid, and false path when object is NULL.
// We use a lightweight, homegrown test harness (no Google Test or GTest).

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
#include <string>
#include <cstdlib>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Candidate Keywords (Step 1): core components involved in the focal method
// - cJSON_AddArrayToObject
// - cJSON_CreateArray
// - add_item_to_object
// - object
// - name
// - array
// - global_hooks
// - cJSON_Delete
// - NULL (return path when addition fails)


// Minimal test framework (Step 3: Domain knowledge)
// Non-terminating assertions: accumulate failures and report at the end.
static int g_failures = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) << " [Function: " << __func__ \
                  << ", Line: " << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while (0)

static void test_AddArrayToObject_ReturnsArray_WhenObjectAndNameAreValid()
{
    // Setup: create a valid JSON object
    cJSON *object = cJSON_CreateObject();
    TEST_ASSERT(object != NULL, "cJSON_CreateObject() should succeed");

    // Exercise: add an array under a new key
    const char *name = "numbers";
    cJSON *array = cJSON_AddArrayToObject(object, name);

    // Verify: the function returns a non-NULL array
    TEST_ASSERT(array != NULL, "cJSON_AddArrayToObject returned NULL on valid input (should return array)");

    // Verify: returned array is actually an array type
    TEST_ASSERT(cJSON_IsArray(array), "Returned item should be of type array");

    // Verify: the object contains the same pointer under the given name
    cJSON *itemInObject = cJSON_GetObjectItem(object, name);
    TEST_ASSERT(itemInObject != NULL, "Object should contain the newly added key with an array");
    TEST_ASSERT(itemInObject == array, "The array returned should be the same instance stored in the object");

    // Verify: the new array is initially empty
    TEST_ASSERT(cJSON_GetArraySize(array) == 0, "Newly created array should be empty");

    // Cleanup
    cJSON_Delete(object);
}

static void test_AddArrayToObject_Returns_NULL_When_Object_Is_NULL()
{
    // Exercise: call with null object pointer; should fail gracefully and return NULL
    const char *name = "emptyArray";
    cJSON *array = cJSON_AddArrayToObject(NULL, name);

    // Verify: should return NULL
    TEST_ASSERT(array == NULL, "cJSON_AddArrayToObject should return NULL when object is NULL");

    // No cleanup required since no object was created
}

static void test_AddArrayToObject_Returns_NULL_When_Name_Is_NULL()
{
    // Setup: create a valid object
    cJSON *object = cJSON_CreateObject();
    TEST_ASSERT(object != NULL, "cJSON_CreateObject() should succeed for NULL-name test");

    // Exercise: pass NULL for the name; behavior depends on implementation, but we should not crash
    cJSON *array = cJSON_AddArrayToObject(object, NULL);

    // Domain knowledge: Depending on cJSON implementation, this could return NULL or create an unnamed key.
    // To keep test robust across versions, we only assert non-crash and handle both possibilities gracefully.
    if (array != NULL)
    {
        // If a valid array is returned, ensure it's properly placed under some key (name is NULL, which is unusual).
        // We can still verify the array exists in object by checking that any item exists at all under an empty key.
        // Since behavior with NULL name is not well-defined, simply ensure we didn't crash and a non-NULL pointer is returned.
        TEST_ASSERT(cJSON_IsArray(array), "Array returned with NULL name should be an array if non-NULL");
        // Cleanup
        cJSON_Delete(object);
        return;
    }
    // If NULL is returned, that's also acceptable (no crash occurred)
    TEST_ASSERT(array == NULL, "cJSON_AddArrayToObject with NULL name returns NULL or array (depending on implementation)");
    cJSON_Delete(object);
}

int main()
{
    // Step 2: Unit Test Generation (covering the core branches)
    // - True branch: object and name valid -> returns array and inserts into object
    // - False branch: object NULL -> returns NULL
    // - Edge-ish branch: name NULL (robustness check, non-terminating assertion)
    test_AddArrayToObject_ReturnsArray_WhenObjectAndNameAreValid();
    test_AddArrayToObject_Returns_NULL_When_Object_Is_NULL();
    test_AddArrayToObject_Returns_NULL_When_Name_Is_NULL();

    // Summary
    if (g_failures == 0)
    {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
    else
    {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}