// Test suite for cJSON_Delete in cJSON.c
// This test suite is written for C++11 (no GoogleTest). It exercises
// various code paths inside cJSON_Delete, including recursive deletion,
// references handling, and NULL handling. It uses a custom memory deallocation
// hook to count dealloc calls and verify expected behavior.

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cassert>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

// Global counter to track deallocation calls via the hook
static size_t g_dealloc_count = 0;

// Custom deallocate hook to intercept memory frees performed by cJSON
static void test_deallocate(void* ptr) {
    // Avoid unused parameter warning in some compilers
    (void)ptr;
    ++g_dealloc_count;
    free(ptr);
}

// Initialize cJSON memory hooks to route deallocation through test_deallocate
static void init_test_hooks() {
    // Define a test hooks structure and point allocate/deallocate to our test functions.
    // The exact type name 'cJSON_Hooks' is used here to align with the project's API.
    // If your environment uses 'internal_hooks', adapt accordingly.
    static cJSON_Hooks test_hooks;
    test_hooks.allocate = malloc;
    test_hooks.deallocate = test_deallocate;

    // Initialize the global hooks used by cJSON
    cJSON_InitHooks(&test_hooks);

    // Reset deallocation counter for each test
    g_dealloc_count = 0;
}

// Simple test assertion macro that logs failures but does not abort
static int test_failures = 0;
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { ++test_failures; std::cerr << "TEST FAILED: " << msg << std::endl; } } while(0)


// Test 1: Basic recursive deletion of a parent object with a child value and its strings
// Expected behavior:
// - The child item is deleted recursively
// - The child's valuestring and string are deallocated
// - The child item is deallocated
// - The parent object itself is deallocated
static void test_delete_basic_recursion_with_child_and_strings() {
    init_test_hooks(); // install test hooks and reset counters

    // Create an object with a single member: {"childKey": "childValue"}
    cJSON *root = cJSON_CreateObject();
    TEST_ASSERT(root != NULL, "Failed to create root object in Test 1");

    cJSON *child = cJSON_CreateString("childValue"); // valuestring allocated
    TEST_ASSERT(child != NULL, "Failed to create child string in Test 1");

    // Attach the child to the root object with key "childKey"
    int added = cJSON_AddItemToObject(root, "childKey", child);
    TEST_ASSERT(added != 0, "Failed to add child to object in Test 1");

    // Now delete the root, which should recursively delete the child as well
    cJSON_Delete(root);

    // We expect: 4 dealloc calls
    // - child's valuestring
    // - child's string (key)
    // - child item itself
    // - root item
    TEST_ASSERT(g_dealloc_count == 4, "Test 1: Expected 4 deallocations (child valuestring, child string, child, root)");
}

// Test 2: Ensure that IsReference items skip deallocation of their child and strings
// Behavior:
// - The referenced item (child) should be deallocated (the item itself), but
//   its valuestring and string should not be deallocated.
// - The root object should still be deallocated.
static void test_delete_isreference_skips_strings_and_child() {
    init_test_hooks(); // install test hooks and reset counters

    cJSON *root = cJSON_CreateObject();
    TEST_ASSERT(root != NULL, "Failed to create root object in Test 2");

    cJSON *ref_child = cJSON_CreateString("refValue");
    TEST_ASSERT(ref_child != NULL, "Failed to create ref_child in Test 2");

    // Add as object member {"refKey": "refValue"} and then mark the child as a reference
    int added = cJSON_AddItemToObject(root, "refKey", ref_child);
    TEST_ASSERT(added != 0, "Failed to add ref_child to object in Test 2");

    // Mark the child as a reference so deallocation of its strings is skipped
    ref_child->type |= cJSON_IsReference;

    // Delete root; only the child item should be freed as an item,
    // not its strings (valuestring or string)
    cJSON_Delete(root);

    // Expected deallocations:
    // - ref_child item itself (since root's traversal deallocates the child item)
    // - root object
    // We do not deallocate ref_child->valuestring or ref_child->string due to IsReference
    TEST_ASSERT(g_dealloc_count == 2, "Test 2: Expected 2 deallocations (ref_child item, root)");
}

// Test 3: Deleting NULL should be a no-op (no deallocation should occur)
static void test_delete_null_noop() {
    init_test_hooks();

    // Deleting NULL should not crash or call the deallocation hook
    cJSON_Delete(NULL);

    TEST_ASSERT(g_dealloc_count == 0, "Test 3: Deleting NULL should not deallocate anything");
}

// Test 4: Deleting a standalone null item should deallocate only the item itself
static void test_delete_null_item_only() {
    init_test_hooks();

    cJSON *null_item = cJSON_CreateNull();
    TEST_ASSERT(null_item != NULL, "Failed to create null item in Test 4");

    cJSON_Delete(null_item);

    // Only the item itself should be deallocated
    TEST_ASSERT(g_dealloc_count == 1, "Test 4: Expected 1 deallocation (null item)");
}

// Test 5: Deeply nested structure to verify multi-level recursion
// Structure:
// {
//   "outer": {
//     "inner": "deep"
//   }
// }
static void test_delete_deep_recursion() {
    init_test_hooks();

    cJSON *root = cJSON_CreateObject();
    TEST_ASSERT(root != NULL, "Failed to create root in Test 5");

    cJSON *outer = cJSON_CreateObject();
    TEST_ASSERT(outer != NULL, "Failed to create outer object in Test 5");

    cJSON *inner = cJSON_CreateString("deep");
    TEST_ASSERT(inner != NULL, "Failed to create inner string in Test 5");

    int added_inner = cJSON_AddItemToObject(outer, "inner", inner);
    TEST_ASSERT(added_inner != 0, "Failed to add inner to outer in Test 5");

    int added_outer = cJSON_AddItemToObject(root, "outer", outer);
    TEST_ASSERT(added_outer != 0, "Failed to add outer to root in Test 5");

    // Delete root; should recursively delete inner, outer, and root
    cJSON_Delete(root);

    // Expected deallocations:
    // - inner valuestring ("deep")
    // - inner string (key "inner")
    // - inner item
    // - outer's string (key "outer")
    // - outer item
    // - root item
    // Total: 6 deallocations
    TEST_ASSERT(g_dealloc_count == 6, "Test 5: Expected 6 deallocations for deep nested structure");
}


// Main function to run all tests
int main() {
    // Run tests
    test_delete_basic_recursion_with_child_and_strings();
    test_delete_isreference_skips_strings_and_child();
    test_delete_null_noop();
    test_delete_null_item_only();
    test_delete_deep_recursion();

    // Summarize results
    if (test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Total test failures: " << test_failures << std::endl;
        return 1;
    }
}