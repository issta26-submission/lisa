/*
Unit test suite for cJSONUtils_AddPatchToArray (CJSON_Utils.c)

Step 1 - Program Understanding (Notes for test author)
- Focal method under test:
  void cJSONUtils_AddPatchToArray(cJSON * const array, const char * const operation, const char * const path, const cJSON * const value)
  It delegates to a static helper: compose_patch(...) to append a patch object to the given array.
- Expected patch structure (typical cJSON_Utils behavior):
  [
    {
      "op": "<operation>",
      "path": "<path>",
      "value": <value>   // value can be omitted or NULL in some edge cases; here we test the common path
    },
    ...
  ]
- Core dependencies (from FOCAL_CLASS_DEP region):
  - cJSON library API (types: cJSON, functions: cJSON_CreateArray, cJSON_CreateString, cJSON_AddItemToArray, cJSON_GetArraySize, cJSON_GetArrayItem, cJSON_IsObject, cJSON_GetObjectItem, cJSON_IsString, cJSON_Delete, etc.)
  - Public header: cJSON_Utils.h (declaration of cJSONUtils_AddPatchToArray)
- Candidate Keywords (key dependent components and interactions):
  cJSON, cJSON_CreateArray, cJSON_CreateString, cJSON_AddItemToArray, cJSON_GetArraySize, cJSON_GetArrayItem, cJSON_IsObject, cJSON_GetObjectItem, cJSON_IsString, valuestring, op/path/value fields, memory ownership (ownership transfer of value into patch)

Step 2 - Unit Test Generation (targeting cJSONUtils_AddPatchToArray)
- We implement a small C++11 test harness (no Google Test) that links against the C library (cJSON and cJSON_Utils).
- Tests cover:
  1) Basic single patch creation: add operation, path, and value; verify resulting patch fields.
  2) Multiple patches: ensure consecutive patches are appended properly and each has correct fields.
- We use non-terminating assertions: on failure we log and continue where possible, but for clarity in this suite we will terminate only at the end with a summary; individual tests emit pass/fail messages.
- Static helper functions in the focal file are not invoked directly (as they are static). We exercise the public API.

Step 3 - Test Case Refinement (domain knowledge)
- We adhere to C++11, use only standard library facilities and the provided C libraries.
- We access C APIs with C linkage via extern "C".
- We ensure proper memory management by deleting the root array (which will cascade delete patches and their nested values).

Code (test suite)

*/

// Candidate keywords and dependencies are reflected in the test comments above.
// Below is a self-contained test program that exercises cJSONUtils_AddPatchToArray.

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cJSON_Utils.h>
#include <limits.h>


extern "C" {
}

// Simple non-terminating test assertion macro.
// Logs failures but allows subsequent tests to run.
#define TEST_EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            printf("FAIL: %s\n", msg); \
            g_failures++; \
        } else { \
            printf("PASS: %s\n", msg); \
        } \
    } while (0)

static int g_failures = 0;

// Test 1: Basic AddPatchToArray usage
// - Creates a JSON array.
// - Creates a JSON value (string).
// - Calls cJSONUtils_AddPatchToArray with op="add" and path="/a/b".
// - Verifies array size is 1 and the patch object has fields op, path, and value with expected content.
static void test_AddPatchToArray_Basic(void) {
    printf("Running test_AddPatchToArray_Basic...\n");

    cJSON *array = cJSON_CreateArray();
    TEST_EXPECT_TRUE(array != NULL, "CreateArray succeeds");

    cJSON *value = cJSON_CreateString("value1");
    TEST_EXPECT_TRUE(value != NULL, "CreateString value1");

    // Invoke the function under test
    cJSONUtils_AddPatchToArray(array, "add", "/a/b", value);

    // Validate the resulting array
    int size = cJSON_GetArraySize(array);
    TEST_EXPECT_TRUE(size == 1, "Array size should be 1 after one patch");

    cJSON *patch = cJSON_GetArrayItem(array, 0);
    TEST_EXPECT_TRUE(patch != NULL, "First patch should exist");
    TEST_EXPECT_TRUE(cJSON_IsObject(patch), "First patch should be an object");

    cJSON *opItem = cJSON_GetObjectItem(patch, "op");
    TEST_EXPECT_TRUE(opItem != NULL, "Patch has 'op' item");
    TEST_EXPECT_TRUE(cJSON_IsString(opItem) && strcmp(opItem->valuestring, "add") == 0,
                   "Patch 'op' should be string 'add'");

    cJSON *pathItem = cJSON_GetObjectItem(patch, "path");
    TEST_EXPECT_TRUE(pathItem != NULL, "Patch has 'path' item");
    TEST_EXPECT_TRUE(cJSON_IsString(pathItem) && strcmp(pathItem->valuestring, "/a/b") == 0,
                   "Patch 'path' should be string '/a/b'");

    cJSON *valueItem = cJSON_GetObjectItem(patch, "value");
    TEST_EXPECT_TRUE(valueItem != NULL, "Patch has 'value' item");
    TEST_EXPECT_TRUE(cJSON_IsString(valueItem) && strcmp(valueItem->valuestring, "value1") == 0,
                   "Patch 'value' should be the string 'value1'");

    // Cleanup
    cJSON_Delete(array);
}

// Test 2: Multiple patches appended correctly
// - Adds two patches sequentially and verifies both exist with correct fields.
static void test_AddPatchToArray_Multiple(void) {
    printf("Running test_AddPatchToArray_Multiple...\n");

    cJSON *array = cJSON_CreateArray();
    TEST_EXPECT_TRUE(array != NULL, "CreateArray for multiple patches");

    cJSON *v1 = cJSON_CreateNumber(42);
    TEST_EXPECT_TRUE(v1 != NULL, "CreateNumber 42 for first patch");
    cJSONUtils_AddPatchToArray(array, "add", "/num", v1);

    cJSON *v2 = cJSON_CreateString("second");
    TEST_EXPECT_TRUE(v2 != NULL, "CreateString 'second' for second patch");
    cJSONUtils_AddPatchToArray(array, "replace", "/str", v2);

    int size = cJSON_GetArraySize(array);
    TEST_EXPECT_TRUE(size == 2, "Array size should be 2 after two patches");

    // Validate first patch
    cJSON *p0 = cJSON_GetArrayItem(array, 0);
    TEST_EXPECT_TRUE(p0 != NULL, "First patch exists");
    TEST_EXPECT_TRUE(cJSON_IsObject(p0), "First patch is an object");
    cJSON *op0 = cJSON_GetObjectItem(p0, "op");
    cJSON *path0 = cJSON_GetObjectItem(p0, "path");
    cJSON *val0 = cJSON_GetObjectItem(p0, "value");
    TEST_EXPECT_TRUE(op0 != NULL && cJSON_IsString(op0) && strcmp(op0->valuestring, "add") == 0,
                   "First patch op == 'add'");
    TEST_EXPECT_TRUE(path0 != NULL && cJSON_IsString(path0) && strcmp(path0->valuestring, "/num") == 0,
                   "First patch path == '/num'");

    // Validate second patch
    cJSON *p1 = cJSON_GetArrayItem(array, 1);
    TEST_EXPECT_TRUE(p1 != NULL, "Second patch exists");
    TEST_EXPECT_TRUE(cJSON_IsObject(p1), "Second patch is an object");
    cJSON *op1 = cJSON_GetObjectItem(p1, "op");
    cJSON *path1 = cJSON_GetObjectItem(p1, "path");
    cJSON *val1 = cJSON_GetObjectItem(p1, "value");
    TEST_EXPECT_TRUE(op1 != NULL && cJSON_IsString(op1) && strcmp(op1->valuestring, "replace") == 0,
                   "Second patch op == 'replace'");
    TEST_EXPECT_TRUE(path1 != NULL && cJSON_IsString(path1) && strcmp(path1->valuestring, "/str") == 0,
                   "Second patch path == '/str'");

    // Cleanup
    cJSON_Delete(array);
}

// Runner and main
static void run_all_tests(void) {
    g_failures = 0;
    test_AddPatchToArray_Basic();
    test_AddPatchToArray_Multiple();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TESTS COMPLETED with %d FAILURE(S)\n", g_failures);
    }
}

int main(void) {
    // Run tests in main to satisfy environments without a test framework
    run_all_tests();
    // Return non-zero if any test failed
    return g_failures;
}