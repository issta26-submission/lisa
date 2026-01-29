// Minimal C++11 unit test suite for cJSON_ReplaceItemViaPointer
// No external test framework is used. A lightweight in-process test runner is provided.

#include <locale.h>
#include <math.h>
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


static int g_test_failures = 0;

// Lightweight assertion macro that does not terminate execution on failure
#define TT_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAILURE] " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

extern "C" {
}

// Helper to safely delete JSON items if not NULL
static void SafeDelete(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

// Test 1: Null checks and invalid inputs should return false
static void test_null_checks() {
    // All NULL -> false
    TT_ASSERT(cJSON_ReplaceItemViaPointer(NULL, NULL, NULL) == false,
              "Expected false when all pointers are NULL");

    // parent non-NULL, but parent->child is NULL -> false
    cJSON *parent = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("item");
    cJSON *replacement = cJSON_CreateString("replacement");
    // Do not add any child to parent so parent->child is NULL
    TT_ASSERT(cJSON_ReplaceItemViaPointer(parent, item, replacement) == false,
              "Expected false when parent->child is NULL");
    SafeDelete(parent);
    SafeDelete(item);
    SafeDelete(replacement);

    // replacement is NULL -> false
    {
        cJSON *array = cJSON_CreateArray();
        cJSON *i1 = cJSON_CreateString("A");
        cJSON *i2 = cJSON_CreateString("B");
        cJSON_AddItemToArray(array, i1);
        cJSON_AddItemToArray(array, i2);
        TT_ASSERT(cJSON_ReplaceItemViaPointer(array, i1, NULL) == false,
                  "Expected false when replacement is NULL");
        SafeDelete(array); // i1 and i2 are deleted by cJSON_Delete
    }

    // item is NULL -> false
    {
        cJSON *array = cJSON_CreateArray();
        cJSON *i1 = cJSON_CreateString("A");
        cJSON_AddItemToArray(array, i1);
        TT_ASSERT(cJSON_ReplaceItemViaPointer(array, NULL, cJSON_CreateString("X")) == false,
                  "Expected false when item is NULL");
        SafeDelete(array); // i1 deleted by cJSON_Delete
    }
}

// Test 2: Replacing an item with itself should return true and not modify the structure
static void test_same_item_returns_true() {
    // Build an array with two items
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateString("one");
    cJSON *b = cJSON_CreateString("two");
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);

    // Replacing a with itself should be a no-op and return true
    bool res = cJSON_ReplaceItemViaPointer(array, a, a);
    TT_ASSERT(res == true, "Replacing item with itself should return true");

    // Ensure list structure unchanged
    TT_ASSERT(array->child == a, "First item should remain 'a'");
    TT_ASSERT(a->next == b, "First item next should be second item (b)");
    TT_ASSERT(b->prev == a, "Second item prev should be first item (a)");

    // Clean up
    SafeDelete(array); // This deletes a and b
}

// Test 3: Replacing the first item in an array should update head and linkage
static void test_replace_in_array_first_item() {
    // Build an array with two items: [ "one", "two" ]
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateString("one");
    cJSON *b = cJSON_CreateString("two");
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);

    // Replacement item
    cJSON *replacement = cJSON_CreateString("ONE");

    bool res = cJSON_ReplaceItemViaPointer(array, a, replacement);
    TT_ASSERT(res == true, "Replacing first item in array should return true");

    // Validate new head and linkage
    TT_ASSERT(array->child == replacement, "Replacement should become the first item (head)");
    TT_ASSERT(replacement->prev == replacement, "First item prev should point to itself (circular head)");
    TT_ASSERT(replacement->next == b, "Replacement next should be the second item (b)");
    TT_ASSERT(b->prev == replacement, "Second item prev should now be the replacement");

    // Clean up
    SafeDelete(array);
}

// Test 4: Replacing a non-first item in an array should fix tail pointer and links
static void test_replace_in_array_non_first_item() {
    // Build an array with two items: [ "first", "second" ]
    cJSON *array = cJSON_CreateArray();
    cJSON *first = cJSON_CreateString("first");
    cJSON *second = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, first);
    cJSON_AddItemToArray(array, second);

    // Replacement for the second item
    cJSON *replacement = cJSON_CreateString("SECOND_REPLACED");

    bool res = cJSON_ReplaceItemViaPointer(array, second, replacement);
    TT_ASSERT(res == true, "Replacing non-first item in array should return true");

    // Validate structure
    TT_ASSERT(array->child == first, "Head should remain the same (first)");
    TT_ASSERT(first->next == replacement, "First item should point to the replacement as next");
    TT_ASSERT(replacement->prev == first, "Replacement prev should be the first item");
    TT_ASSERT(replacement->next == NULL, "Replacement should be the last item (next == NULL)");
    TT_ASSERT(array->child->prev == replacement, "Head's prev should point to the new last item (replacement)");

    // Clean up
    SafeDelete(array);
}

// Test 5: Replacing the first item in an object (cJSON object) should update head and linkage
static void test_replace_in_object_first_item() {
    // Build an object with two members
    cJSON *object = cJSON_CreateObject();
    cJSON *first_item = cJSON_CreateString("A"); // value
    cJSON *second_item = cJSON_CreateString("B");
    // Add with keys to form a real object
    cJSON_AddItemToObject(object, "first", first_item);
    cJSON_AddItemToObject(object, "second", second_item);

    // Replacement for the first item
    cJSON *replacement = cJSON_CreateString("A_REPLACED");

    bool res = cJSON_ReplaceItemViaPointer(object, first_item, replacement);
    TT_ASSERT(res == true, "Replacing first item in object should return true");

    // Validate linkage
    TT_ASSERT(object->child == replacement, "Object head should be the replacement");
    TT_ASSERT(replacement->prev == replacement, "First item in object should have prev pointing to itself");
    TT_ASSERT(replacement->next == second_item, "Replacement next should point to the second item");
    TT_ASSERT(second_item->prev == replacement, "Second item prev should point to the replacement");

    // Clean up
    SafeDelete(object);
}

// Main test runner
int main() {
    // Run tests
    test_null_checks();
    test_same_item_returns_true();
    test_replace_in_array_first_item();
    test_replace_in_array_non_first_item();
    test_replace_in_object_first_item();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[TEST SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST SUMMARY] Failures: " << g_test_failures << std::endl;
        return 1;
    }
}