/*
Unit Test Suite for cJSON_AddRawToObject (C++11)

Step 1 - Program Understanding (Summary embedded as comments)
- Focal method: cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw)
  - Creates a raw JSON item with cJSON_CreateRaw(raw)
  - Attempts to insert it into an object with add_item_to_object(object, name, raw_item, &global_hooks, false)
  - If insertion succeeds, returns the newly created raw item
  - If insertion fails, deletes the created raw item and returns NULL

- Key dependent components ( Candidate Keywords ):
  - cJSON_CreateRaw
  - add_item_to_object
  - cJSON_Delete
  - cJSON_IsRaw / cJSON_IsObject / cJSON_CreateObject
  - cJSON_GetObjectItem
  - Memory management via cJSON_Delete on failure
  - Edge cases: valid object + valid key (success branch), NULL key (failure branch), non-object container (failure branch)

Step 2 - Unit Test Generation (Strategy)
- Cover true path: valid object, valid name, non-null raw -> expect non-NULL return and the object contains a raw item under the given key
- Cover false path 1: name is NULL -> expect NULL return and object remains unchanged (printed JSON should be "{}")
- Cover false path 2: object is not a JSON object (e.g., an array) -> expect NULL return and container remains unchanged (printed JSON should be "[]")
- Use minimal test harness (no GTest). Use a small in-house EXPECT-like mechanism that logs failures but continues execution
- Validate object state via object printing (cJSON_PrintUnformatted) to ensure no unintended mutations

Step 3 - Test Case Refinement (Domain Knowledge)
- Ensure static linkage constraints are respected by limited API calls and public functions only
- Use only C/C++ standard library + provided cJSON APIs
- Validate pointer equivalence for success path (returned pointer should be the same as the one stored in the object)
- Ensure proper cleanup to avoid leaks

The following code is the complete test suite runnable in a C++11 environment, linking against cJSON.c/.h

*/

// Include necessary headers
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
#include <limits.h>

extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { ++g_total; if (cond) { ++g_passed; } else { ++g_failed; printf("[FAIL] %s\n", msg); } } while(0)
#define EXPECT_FALSE(cond, msg) do { ++g_total; if (!(cond)) { ++g_passed; } else { ++g_failed; printf("[FAIL] %s\n", msg); } } while(0)
#define EXPECT_NOT_NULL(p, msg) do { ++g_total; if ((p) != NULL) { ++g_passed; } else { ++g_failed; printf("[FAIL] %s\n", msg); } } while(0)
#define EXPECT_NULL(p, msg) do { ++g_total; if ((p) == NULL) { ++g_passed; } else { ++g_failed; printf("[FAIL] %s\n", msg); } } while(0)
#define EXPECT_EQ_PTR(a, b, msg) do { ++g_total; if ((a) == (b)) { ++g_passed; } else { ++g_failed; printf("[FAIL] %s\n", msg); } } while(0)

static void test_AddRawToObject_Success()
{
    // Test the successful true-branch: valid object, valid name, valid raw
    // Expected: non-NULL return, and the object should contain a RAW item under the given key
    cJSON *object = cJSON_CreateObject();
    EXPECT_NOT_NULL(object, "CreateObject() should succeed");

    const char *name = "rawKey";
    const char *raw = "RAW_CONTENT";

    cJSON *ret = cJSON_AddRawToObject(object, name, raw);
    EXPECT_NOT_NULL(ret, "cJSON_AddRawToObject should return non-NULL on success");

    cJSON *looked = cJSON_GetObjectItem(object, name);
    EXPECT_NOT_NULL(looked, "Object should contain the added item with the given key");
    EXPECT_EQ_PTR(ret, looked, "Returned pointer should be the same as the object's item");

    // Verify the added item is of RAW type
    int isRaw = cJSON_IsRaw(looked);
    EXPECT_TRUE(isRaw != 0, "Added item should be of RAW type");

    // Cleanup
    cJSON_Delete(object);
}

static void test_AddRawToObject_NullName()
{
    // Test failure path: NULL name should cause early failure
    // Expected: NULL return and object remains unchanged (printing should yield "{}")
    cJSON *object = cJSON_CreateObject();
    EXPECT_NOT_NULL(object, "CreateObject() should succeed");

    cJSON *ret = cJSON_AddRawToObject(object, NULL, "RAW_CONTENT");
    EXPECT_NULL(ret, "cJSON_AddRawToObject with NULL name should return NULL");

    // Ensure object remains unchanged
    char *json_before = cJSON_PrintUnformatted(object);
    EXPECT_NOT_NULL(json_before, "Print before should succeed");
    if (json_before) {
        EXPECT_TRUE(std::strcmp(json_before, "{}") == 0, "Object should remain unchanged as {} after failed insert");
        cJSON_free(json_before);
    }

    // Cleanup
    cJSON_Delete(object);
}

static void test_AddRawToObject_ObjectNotObject()
{
    // Test failure path: container is not a JSON object (e.g., an array)
    // Expected: NULL return and array remains unchanged (printing should yield "[]")
    cJSON *array = cJSON_CreateArray();
    EXPECT_NOT_NULL(array, "CreateArray() should succeed");

    cJSON *ret = cJSON_AddRawToObject(array, "name", "RAW_CONTENT");
    EXPECT_NULL(ret, "cJSON_AddRawToObject with non-object container should return NULL");

    // Ensure array remains unchanged
    char *json_before = cJSON_PrintUnformatted(array);
    EXPECT_NOT_NULL(json_before, "Print before should succeed for array");
    if (json_before) {
        EXPECT_TRUE(std::strcmp(json_before, "[]") == 0, "Array should remain unchanged as [] after failed insert");
        cJSON_free(json_before);
    }

    // Cleanup
    cJSON_Delete(array);
}

int main()
{
    // Run tests
    test_AddRawToObject_Success();
    test_AddRawToObject_NullName();
    test_AddRawToObject_ObjectNotObject();

    // Summary
    printf("Tests run: %d, Passed: %d, Failed: %d\n", g_total, g_passed, g_failed);
    if (g_failed == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed. See details above.\n");
    }
    // Return non-zero if any test failed
    return g_failed;
}