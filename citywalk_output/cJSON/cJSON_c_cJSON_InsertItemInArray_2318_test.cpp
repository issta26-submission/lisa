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
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion helper for test clarity
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("    [FAIL] %s\n", (msg)); \
        ++g_failures; \
    } else { \
        printf("    [PASS] %s\n", (msg)); \
    } \
} while(0)

/*
Step 1 & Step 2: Understanding the focal method and dependencies
- Focal method: cJSON_InsertItemInArray
  - Inserts newitem into array at position which.
  - Branches:
    1) which < 0 or newitem == NULL -> return false (no operation)
    2) after_inserted = get_array_item(array, which); if NULL -> return add_item_to_array(array, newitem)
    3) If after_inserted is not the first item and its prev == NULL -> return false (corrupted item)
    4) Normal insertion: relink pointers (newitem->next, newitem->prev) around after_inserted
       and update array->child if inserting before the first element; else adjust surrounding links
  - Dependencies in cJSON.c/cJSON.h relevant to testing:
    - cJSON_CreateArray, cJSON_AddItemToArray
    - cJSON_GetArraySize, cJSON_GetArrayItem
    - cJSON_CreateNumber, cJSON_Delete, cJSON_... basic cJSON struct fields (child, next, prev)
    - No need to mock; use actual C-API and internal struct fields if necessary (static helpers are hidden)
- We will test various branches by constructing arrays and items, manipulating pointers where required
  to exercise true/false branches without relying on private methods beyond the public API.

/*
Step 3: Test Suite (C++11, no GTest)
- We implement test cases as separate functions returning bool (pass/fail)
- A small main() runs all tests and prints a final summary
- We only use the C API provided by cJSON and standard C/C++ standard library
- Tests are designed to be executable with a standard C++11 toolchain against cJSON.c
*/

// Helper: create a simple 2-item array [A, B] for tests requiring multiple initial elements
static cJSON* create_two_item_array(cJSON* A, cJSON* B) {
    cJSON* arr = cJSON_CreateArray();
    if (!arr) return NULL;
    if (A) cJSON_AddItemToArray(arr, A);
    if (B) cJSON_AddItemToArray(arr, B);
    return arr;
}

// Test 1: Inserting with a negative index should return false and not modify the array
static bool test_InsertItemInArray_negative_index_returns_false(void) {
    // Setup: array with one element
    cJSON* array = cJSON_CreateArray();
    cJSON* A = cJSON_CreateNumber(1);
    cJSON_AddItemToArray(array, A);

    // new item to attempt insertion
    cJSON* newitem = cJSON_CreateNumber(99);

    // Act
    cJSON_bool result = cJSON_InsertItemInArray(array, -1, newitem);

    // Assert
    bool ok = (result == false) && (cJSON_GetArraySize(array) == 1) && (cJSON_GetArrayItem(array, 0) == A);

    // Cleanup: newitem was not inserted; delete it separately
    cJSON_Delete(newitem);
    cJSON_Delete(array);
    return ok;
}

// Test 2: Inserting with a NULL newitem should return false and not modify the array
static bool test_InsertItemInArray_null_newitem_returns_false(void) {
    // Setup: array with one element
    cJSON* array = cJSON_CreateArray();
    cJSON* A = cJSON_CreateNumber(10);
    cJSON_AddItemToArray(array, A);

    // Act
    cJSON_bool result = cJSON_InsertItemInArray(array, 0, NULL);

    // Assert
    bool ok = (result == false) && (cJSON_GetArraySize(array) == 1) && (cJSON_GetArrayItem(array, 0) == A);

    // Cleanup
    cJSON_Delete(A);
    cJSON_Delete(array);
    return ok;
}

// Test 3: When which points past the end (after_inserted == NULL), item should be appended
static bool test_InsertItemInArray_append_when_after_inserted_null(void) {
    // Setup: array with one element
    cJSON* array = cJSON_CreateArray();
    cJSON* A = cJSON_CreateNumber(5);
    cJSON_AddItemToArray(array, A);

    // new item to append
    cJSON* newitem = cJSON_CreateNumber(7);

    // which is beyond current size -> after_inserted == NULL => append
    cJSON_bool result = cJSON_InsertItemInArray(array, 5, newitem);

    // Assert
    bool ok = (result == true) &&
              (cJSON_GetArraySize(array) == 2) &&
              (cJSON_GetArrayItem(array, 1) == newitem);

    // Cleanup
    cJSON_Delete(array); // deletes A and newitem
    return ok;
}

// Test 4: Corrupted array item: after_inserted != array->child and after_inserted->prev == NULL should return false
static bool test_InsertItemInArray_corrupted_after_inserted_prev_null(void) {
    // Setup: array with two elements
    cJSON* A = cJSON_CreateNumber(1);
    cJSON* B = cJSON_CreateNumber(2);
    cJSON* array = create_two_item_array(A, B);

    // Corrupt B by breaking its prev pointer
    B->prev = NULL;

    // new item to insert
    cJSON* newitem = cJSON_CreateNumber(99);

    // which = 1 -> after_inserted is B (not head) but B->prev is NULL (corrupted)
    cJSON_bool result = cJSON_InsertItemInArray(array, 1, newitem);

    // Assert
    bool ok = (result == false) && (cJSON_GetArraySize(array) == 2) &&
              (cJSON_GetArrayItem(array, 0) == A) &&
              (cJSON_GetArrayItem(array, 1) == B);

    // Cleanup
    cJSON_Delete(array); // A and B are freed
    // newitem was not inserted
    cJSON_Delete(newitem);
    return ok;
}

// Test 5: Normal insertion before a middle item
static bool test_InsertItemInArray_insert_before_middle(void) {
    // Setup: array [A, B]
    cJSON* A = cJSON_CreateNumber(11);
    cJSON* B = cJSON_CreateNumber(22);
    cJSON* array = create_two_item_array(A, B);

    // new item to insert before B (which = 1)
    cJSON* newitem = cJSON_CreateNumber(15);
    cJSON_bool result = cJSON_InsertItemInArray(array, 1, newitem);

    // Assert structure: [A, newitem, B]
    cJSON* item0 = cJSON_GetArrayItem(array, 0);
    cJSON* item1 = cJSON_GetArrayItem(array, 1);
    cJSON* item2 = cJSON_GetArrayItem(array, 2);

    bool ok = (result == true) &&
              (item0 == A) &&
              (item1 == newitem) &&
              (item2 == B) &&
              (B->prev == newitem) &&
              (newitem->prev == A) &&
              (A->next == newitem) &&
              (newitem->next == B);

    // Cleanup
    cJSON_Delete(array);
    // A, B, newitem are freed by array, no separate delete needed
    return ok;
}

// Test 6: Inserting before the first element (which = 0)
static bool test_InsertItemInArray_insert_before_first(void) {
    // Setup: array [A, B]
    cJSON* A = cJSON_CreateNumber(100);
    cJSON* B = cJSON_CreateNumber(200);
    cJSON* array = create_two_item_array(A, B);

    // new item to insert before first (which = 0)
    cJSON* newitem = cJSON_CreateNumber(-5);
    cJSON_bool result = cJSON_InsertItemInArray(array, 0, newitem);

    // Assert: order should be [newitem, A, B]
    cJSON* item0 = cJSON_GetArrayItem(array, 0);
    cJSON* item1 = cJSON_GetArrayItem(array, 1);
    cJSON* item2 = cJSON_GetArrayItem(array, 2);

    bool ok = (result == true) &&
              (item0 == newitem) &&
              (item1 == A) &&
              (item2 == B) &&
              (newitem->prev == NULL) &&
              (A->prev == newitem) &&
              (newitem->next == A) &&
              (A->next == B);

    // Cleanup
    cJSON_Delete(array);
    return ok;
}

// Test 7: Insertion before last in a 3-item array: [A, B, C], insert D before C -> [A, B, D, C]
static bool test_InsertItemInArray_insert_before_last(void) {
    // Setup: array [A, B, C]
    cJSON* A = cJSON_CreateNumber(1);
    cJSON* B = cJSON_CreateNumber(2);
    cJSON* C = cJSON_CreateNumber(3);
    cJSON* array = create_two_item_array(A, B);
    // Manually extend to 3 items by adding C
    cJSON_AddItemToArray(array, C);

    // new item to insert before last (which = 2 -> before C)
    cJSON* D = cJSON_CreateNumber(9);
    cJSON_bool result = cJSON_InsertItemInArray(array, 2, D);

    // Assert order: [A, B, D, C]
    cJSON* item0 = cJSON_GetArrayItem(array, 0);
    cJSON* item1 = cJSON_GetArrayItem(array, 1);
    cJSON* item2 = cJSON_GetArrayItem(array, 2);
    cJSON* item3 = cJSON_GetArrayItem(array, 3);

    bool ok = (result == true) &&
              (item0 == A) &&
              (item1 == B) &&
              (item2 == D) &&
              (item3 == C) &&
              (B->next == D) &&
              (D->prev == B) &&
              (D->next == C);

    // Cleanup
    cJSON_Delete(array);
    return ok;
}

// Main: run all tests and report summary
int main(void) {
    printf("Starting cJSON_InsertItemInArray unit tests (C++, no GTest)\n");

    g_failures = 0;

    bool t1 = test_InsertItemInArray_negative_index_returns_false();
    TEST_ASSERT(t1, "Negative index should return false and not modify array");

    bool t2 = test_InsertItemInArray_null_newitem_returns_false();
    TEST_ASSERT(t2, "NULL newitem should return false and not modify array");

    bool t3 = test_InsertItemInArray_append_when_after_inserted_null();
    TEST_ASSERT(t3, "When which points past end, newitem should be appended to array");

    bool t4 = test_InsertItemInArray_corrupted_after_inserted_prev_null();
    TEST_ASSERT(t4, "Corrupted after_inserted (prev == NULL) should return false");

    bool t5 = test_InsertItemInArray_insert_before_middle();
    TEST_ASSERT(t5, "Insert before middle item should relink pointers correctly");

    bool t6 = test_InsertItemInArray_insert_before_first();
    TEST_ASSERT(t6, "Insert before first should update array->child to newitem and relink");

    bool t7 = test_InsertItemInArray_insert_before_last();
    TEST_ASSERT(t7, "Insert before last in 3-item array should relink all pointers correctly");

    if (g_failures == 0) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_failures);
        return 1;
    }
}