// Unit tests for cJSON_DetachItemViaPointer
// Objective: Validate behavior of detaching items via pointer in various scenarios
// without relying on Google Test. A lightweight custom test harness is used.

#include <cmath>
#include <locale.h>
#include <math.h>
#include <limits>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness
static int g_failures = 0;
static int g_tests_run = 0;

static void log_fail(const std::string& test_name, const std::string& msg) {
    ++g_failures;
    std::cerr << "[FAIL] " << test_name << ": " << msg << std::endl;
}
static void log_pass(const std::string& test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}
static void log_skip(const std::string& test_name, const std::string& reason) {
    std::cout << "[SKIP] " << test_name << ": " << reason << std::endl;
}

// Convenience macro for non-terminating checks
#define CHECK(cond, test_name, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { log_fail(test_name, msg); } else { log_pass(test_name); } \
} while(0)

// Test 1: Detach the first item from a JSON array (A, B, C) -> detach A
static bool test_detach_first_item_from_array() {
    const std::string test_name = "test_detach_first_item_from_array";

    // Setup: create array with three numeric items
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateNumber(1);
    cJSON *b = cJSON_CreateNumber(2);
    cJSON *c = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);
    cJSON_AddItemToArray(array, c);

    // Act: detach the first item using pointer
    cJSON *ret = cJSON_DetachItemViaPointer(array, a);

    // Assert: detached item is the same pointer, and its links are cleared
    CHECK(ret == a, test_name, "Returned pointer should be the detached item (a).");
    if (ret != NULL) {
        // The API contract specifies that the detached item should not be linked anymore
        CHECK((ret->prev == NULL) && (ret->next == NULL),
              test_name,
              "Detached item's prev/next should be NULL.");
        // The item value should still be accessible (sanity check)
        CHECK(cJSON_GetNumberValue(ret) == 1.0, test_name, "Detached item should preserve its number value (1).");
    }

    // Assert: parent should now have 2 items: [2, 3]
    CHECK(cJSON_GetArraySize(array) == 2, test_name, "Parent array size should be 2 after detaching first item.");
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    CHECK(first == b, test_name, "First item after detach should be the original second item (2).");
    CHECK(second == c, test_name, "Second item after detach should be the original third item (3).");

    // Cleanup: free allocated items
    cJSON_Delete(array); // frees remaining children
    cJSON_Delete(a);     // detached item
    // b and c were already freed by deleting the array, but since we deleted a, ensure no double-free
    // Note: In typical cJSON usage, deleting the parent frees its children; since a is detached, delete it separately.

    return true;
}

// Test 2: Detach the middle item from an array (A, B, C) -> detach B
static bool test_detach_middle_item_from_array() {
    const std::string test_name = "test_detach_middle_item_from_array";

    // Setup: create array with three numeric items
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateNumber(10);
    cJSON *b = cJSON_CreateNumber(20);
    cJSON *c = cJSON_CreateNumber(30);
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);
    cJSON_AddItemToArray(array, c);

    // Act: detach the middle item (B)
    cJSON *ret = cJSON_DetachItemViaPointer(array, b);

    // Assert: returned pointer is the middle item and array now has [A, C]
    CHECK(ret == b, test_name, "Returned pointer should be the detached middle item (b).");
    if (ret != NULL) {
        CHECK((ret->prev == NULL) && (ret->next == NULL), test_name, "Detached middle item prev/next should be NULL.");
        CHECK(cJSON_GetNumberValue(ret) == 20.0, test_name, "Detached middle item should preserve value (20).");
    }

    CHECK(cJSON_GetArraySize(array) == 2, test_name, "Array size should be 2 after detaching middle item.");
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    CHECK(first == a, test_name, "First item after detach should be A (10).");
    CHECK(second == c, test_name, "Second item after detach should be C (30).");

    // Cleanup
    cJSON_Delete(array);
    cJSON_Delete(b);

    return true;
}

// Test 3: Detach the last item from an array (A, B, C) -> detach C
static bool test_detach_last_item_from_array() {
    const std::string test_name = "test_detach_last_item_from_array";

    // Setup: create array with three numeric items
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateNumber(5);
    cJSON *b = cJSON_CreateNumber(6);
    cJSON *c = cJSON_CreateNumber(7);
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);
    cJSON_AddItemToArray(array, c);

    // Act: detach the last item (C)
    cJSON *ret = cJSON_DetachItemViaPointer(array, c);

    // Assert: returned pointer is the last item and array now has [A, B]
    CHECK(ret == c, test_name, "Returned pointer should be the detached last item (c).");
    if (ret != NULL) {
        CHECK((ret->prev == NULL) && (ret->next == NULL), test_name, "Detached last item prev/next should be NULL.");
        CHECK(cJSON_GetNumberValue(ret) == 7.0, test_name, "Detached last item should preserve value (7).");
    }

    CHECK(cJSON_GetArraySize(array) == 2, test_name, "Array size should be 2 after detaching last item.");
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    CHECK(first == a, test_name, "First item should be A (5).");
    CHECK(second == b, test_name, "Second item should be B (6).");

    // Cleanup
    cJSON_Delete(array);
    cJSON_Delete(c);

    return true;
}

// Test 4: Invalid scenarios - null parent or null item should return NULL and not crash
static bool test_invalid_parent_or_item_returns_null() {
    const std::string test_name = "test_invalid_parent_or_item_returns_null";

    // Subtest a: null parent
    cJSON *item = cJSON_CreateNull();
    cJSON *ret_null_parent = cJSON_DetachItemViaPointer(NULL, item);
    CHECK(ret_null_parent == NULL, test_name, "Detaching with NULL parent should return NULL.");
    cJSON_Delete(item);

    // Subtest b: null item
    cJSON *parent = cJSON_CreateArray();
    cJSON *child = cJSON_CreateNumber(1);
    cJSON_AddItemToArray(parent, child);
    cJSON *ret_null_item = cJSON_DetachItemViaPointer(parent, NULL);
    CHECK(ret_null_item == NULL, test_name, "Detaching with NULL item should return NULL.");
    cJSON_Delete(parent);
    cJSON_Delete(child); // child was detached? It was added to parent; it's safe to delete

    return true;
}

// Test 5: Detach item from a different parent should not corrupt the original parent's list
static bool test_detach_item_from_different_parent() {
    const std::string test_name = "test_detach_item_from_different_parent";

    // Parent 1: [A]
    cJSON *p1 = cJSON_CreateArray();
    cJSON *A = cJSON_CreateNumber(100);
    cJSON_AddItemToArray(p1, A);

    // Parent 2: [B]
    cJSON *p2 = cJSON_CreateArray();
    cJSON *B = cJSON_CreateNumber(200);
    cJSON_AddItemToArray(p2, B);

    // Act: attempt to detach B using pointer from p1
    cJSON *ret = cJSON_DetachItemViaPointer(p1, B);

    // Assert: function returns non-NULL (B) and p1's structure remains effectively intact (size still 1)
    CHECK(ret == B, test_name, "Detaching item from a different parent should return the item (B).");
    CHECK(cJSON_GetArraySize(p1) == 1, test_name, "Parent 1 size should remain 1 after invalid detach.");
    cJSON *only = cJSON_GetArrayItem(p1, 0);
    CHECK(only == A, test_name, "Parent 1's only item should remain A.");

    // Cleanup
    cJSON_Delete(p1); // should free A
    cJSON_Delete(p2); // free B
    // B is part of p2; ensure it's not double-freed; nothing else to do

    return true;
}

// Entry point
int main() {
    std::cout << "Running cJSON_DetachItemViaPointer unit tests (C++11, non-GTest).\n";

    // Run tests
    bool ok = true;
    ok &= test_detach_first_item_from_array();
    ok &= test_detach_middle_item_from_array();
    ok &= test_detach_last_item_from_array();
    ok &= test_invalid_parent_or_item_returns_null();
    ok &= test_detach_item_from_different_parent();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;

    // Return non-zero if any test failed
    if (g_failures > 0) {
        return 1;
    }
    return 0;
}