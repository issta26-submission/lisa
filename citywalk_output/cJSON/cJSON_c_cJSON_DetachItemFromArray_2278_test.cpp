/*
  Unit test suite for cJSON_DetachItemFromArray (CJSON public API)
  - Objective: Verify behavior for negative index, valid middle index, and out-of-bounds indices.
  - Approach: Use the public C API exposed by cJSON (Create Array, Add Items, Get Array Size, Get Item, Delete Item).
  - Environment: C++11, no GoogleTest. Lightweight in-file test runner with non-terminating assertions.
  - Notes: Tests are designed to be executable with the provided cJSON C sources and header.
*/

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstdint>
#include <limits.h>


static int g_failures = 0;

// Lightweight assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        printf("EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b) do { \
    if(((int)(a)) != ((int)(b))) { \
        printf("EXPECT_EQ_INT failed: %d != %d at %s:%d\n", (int)(a), (int)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_SIZE_T(a,b) do { \
    if(((size_t)(a)) != ((size_t)(b))) { \
        printf("EXPECT_EQ_SIZE_T failed: %zu != %zu at %s:%d\n", (size_t)(a), (size_t)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
    if((void*)(a) != (void*)(b)) { \
        printf("EXPECT_PTR_EQ failed: %p != %p at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

/* Include C header (cJSON) with C linkage in C++ translation unit. */
extern "C" {
}

/*
  Test 1: Negative index should return NULL and leave the array unchanged.
*/
void test_DetachItemFromArray_negative_index_no_change() {
    // Build an array: [1, 2, 3]
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1);
    cJSON *n2 = cJSON_CreateNumber(2);
    cJSON *n3 = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    // Negative index should produce NULL and not detach anything
    cJSON *result = cJSON_DetachItemFromArray(array, -1);
    EXPECT_TRUE(result == NULL);

    // Array should still contain all three items
    size_t size = cJSON_GetArraySize(array);
    EXPECT_EQ_SIZE_T(size, 3);

    // Cleanup
    cJSON_Delete(array); // This also deletes contained items
    // result is NULL, nothing to delete
}

/*
  Test 2: Detach a valid middle item (index 1) and verify array integrity and detached item value.
*/
void test_DetachItemFromArray_detach_middle_item() {
    // Build an array: [1, 2, 3]
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1);
    cJSON *n2 = cJSON_CreateNumber(2);
    cJSON *n3 = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    // Detach item at index 1 (value 2)
    cJSON *detached = cJSON_DetachItemFromArray(array, 1);
    EXPECT_TRUE(detached != NULL);
    // Verify detached value is 2
    double val = cJSON_GetNumberValue(detached);
    EXPECT_EQ_INT((int)val, 2);

    // Array should now have size 2 with values [1, 3]
    size_t size = cJSON_GetArraySize(array);
    EXPECT_EQ_SIZE_T(size, 2);

    cJSON *a0 = cJSON_GetArrayItem(array, 0);
    cJSON *a1 = cJSON_GetArrayItem(array, 1);
    EXPECT_TRUE(a0 != NULL);
    EXPECT_TRUE(a1 != NULL);
    EXPECT_EQ_INT((int)cJSON_GetNumberValue(a0), 1);
    // a1 should be 3
    EXPECT_EQ_INT((int)cJSON_GetNumberValue(a1), 3);

    // Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(array);
}

/*
  Test 3: Detach an item with an index equal to the current size (out-of-bounds-like scenario).
  Expect NULL and no change to the array.
*/
void test_DetachItemFromArray_index_out_of_bounds() {
    // Build an array: [1, 2, 3]
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1);
    cJSON *n2 = cJSON_CreateNumber(2);
    cJSON *n3 = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    // Detach with index equal to size (3) -> out of bounds
    cJSON *result = cJSON_DetachItemFromArray(array, 3);
    EXPECT_TRUE(result == NULL);

    // Array size should remain 3
    size_t size = cJSON_GetArraySize(array);
    EXPECT_EQ_SIZE_T(size, 3);

    // Cleanup
    cJSON_Delete(array);
    if (result) cJSON_Delete(result); // should be NULL, defensively safe
}

/*
  Test 4: Repeated operations to ensure stability when detaching multiple items sequentially.
  Detach first element, then detach new first element, verify remaining sequence.
*/
void test_DetachItemFromArray_sequential_detach() {
    // Build an array: [10, 20, 30, 40]
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateNumber(10);
    cJSON *b = cJSON_CreateNumber(20);
    cJSON *c = cJSON_CreateNumber(30);
    cJSON *d = cJSON_CreateNumber(40);
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);
    cJSON_AddItemToArray(array, c);
    cJSON_AddItemToArray(array, d);

    // Detach first item (index 0) -> value 10
    cJSON *det1 = cJSON_DetachItemFromArray(array, 0);
    EXPECT_TRUE(det1 != NULL);
    EXPECT_EQ_INT((int)cJSON_GetNumberValue(det1), 10);
    // Remaining array should be [20, 30, 40]
    EXPECT_EQ_SIZE_T(cJSON_GetArraySize(array), 3);
    EXPECT_EQ_INT((int)cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0)), 20);
    // Detach new first item (index 0) -> value 20
    cJSON *det2 = cJSON_DetachItemFromArray(array, 0);
    EXPECT_TRUE(det2 != NULL);
    EXPECT_EQ_INT((int)cJSON_GetNumberValue(det2), 20);
    // Remaining array should be [30, 40]
    EXPECT_EQ_SIZE_T(cJSON_GetArraySize(array), 2);
    EXPECT_EQ_INT((int)cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0)), 30);

    // Cleanup
    cJSON_Delete(det1);
    cJSON_Delete(det2);
    cJSON_Delete(array);
}

int main(void) {
    printf("Running unit tests for cJSON_DetachItemFromArray...\n");

    test_DetachItemFromArray_negative_index_no_change();
    test_DetachItemFromArray_detach_middle_item();
    test_DetachItemFromArray_index_out_of_bounds();
    test_DetachItemFromArray_sequential_detach();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}