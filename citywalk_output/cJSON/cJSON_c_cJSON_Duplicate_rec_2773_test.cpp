#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Include the public cJSON API
extern "C" {
}

// Expose the internal recursive duplicate function for testing via C linkage
#ifdef __cplusplus
extern "C" {
#endif
cJSON * cJSON_Duplicate_rec(const cJSON *item, size_t depth, cJSON_bool recurse);
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness
static int g_test_failures = 0;
static void expect_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "Test failure: " << message << std::endl;
        ++g_test_failures;
    }
}

// Test 1: Shallow duplicate (recurse = false) should copy top-level fields but not children
void test_shallow_duplicate() {
    // Create a simple object with one string member
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "k", "v");

    // Perform shallow duplicate
    cJSON *dup = cJSON_Duplicate(root, false);

    // Validate: dup should exist, but should not have any children (no members)
    expect_true(dup != NULL, "Shallow duplicate should not be NULL");
    // root's child chain should be null in the shallow copy
    expect_true(dup->child == NULL, "Shallow duplicate should not copy children (no members at top level)");

    // Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
}

// Test 2: Deep duplicate (recurse = true) should duplicate entire structure
void test_deep_duplicate() {
    // Build a nested structure: { "child": { "name2": "inner" }, "name": "root" }
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "root");

    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "name2", "inner");

    // Deep duplicate
    cJSON *dup = cJSON_Duplicate(root, true);

    // Validate: structure copied
    expect_true(dup != NULL, "Deep duplicate should not be NULL");
    cJSON *dup_child = cJSON_GetObjectItemCaseSensitive(dup, "child");
    expect_true(dup_child != NULL, "Deep duplicate should contain 'child' object");
    cJSON *dup_name2 = cJSON_GetObjectItemCaseSensitive(dup_child, "name2");
    expect_true(dup_name2 != NULL, "Deep duplicate's child should have 'name2' key");
    expect_true(dup_name2->valuestring != NULL, "Deep duplicate's child 'name2' should have a string value");
    expect_true(std::strcmp(dup_name2->valuestring, "inner") == 0, "Deep duplicate's inner value should be 'inner'");

    // Ensure that pointers are different (strings are duplicated)
    cJSON *orig_child = cJSON_GetObjectItemCaseSensitive(root, "child");
    cJSON *orig_name2 = cJSON_GetObjectItemCaseSensitive(orig_child, "name2");
    expect_true(dup_child != orig_child, "Duplicated 'child' object should be a different pointer");
    expect_true(dup_name2->valuestring != orig_name2->valuestring, "Duplicated string should be a different memory instance");

    // Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
}

// Test 3: Duplicate with NULL input should return NULL
void test_duplicate_null_input() {
    cJSON *res = cJSON_Duplicate(NULL, true);
    expect_true(res == NULL, "Duplicating NULL input should return NULL");
}

// Test 4: Internal recursive duplicate should fail when depth exceeds CJSON_CIRCULAR_LIMIT
void test_depth_limit_violation() {
    // Create a simple scalar item (no children needed)
    cJSON *item = cJSON_CreateNumber(42);

    // Force depth to be very large to trigger the depth check against CJSON_CIRCULAR_LIMIT
    // The exact macro value is internal; using a large depth value should exceed it.
    cJSON *res = cJSON_Duplicate_rec(item, 1000, true);

    // Expect failure (NULL) due to circular limit check
    expect_true(res == NULL, "cJSON_Duplicate_rec should fail (return NULL) when depth >= CJSON_CIRCULAR_LIMIT");

    // Cleanup
    cJSON_Delete(item);
}

// Test 5: Ensure IsReference bit is cleared in the duplicate (static flag handling)
void test_is_reference_flag_cleared() {
    // Create a string item
    cJSON *str = cJSON_CreateString("hello");
    // Artificially set the IsReference flag to simulate a non-owned reference
    str->type |= cJSON_IsReference;

    // Duplicate
    cJSON *dup = cJSON_Duplicate(str, true);
    expect_true(dup != NULL, "Duplicate of string with IsReference flag should succeed");

    // The IsReference flag should be cleared in the duplicate
    expect_true((dup->type & cJSON_IsReference) == 0, "Duplicated item should have IsReference flag cleared");

    // The value should be preserved
    expect_true(dup->valuestring != NULL && std::strcmp(dup->valuestring, "hello") == 0, "Duplicated string value should be preserved");

    // Ensure original value pointer is different from duplicated value pointer
    expect_true(dup->valuestring != str->valuestring, "Duplicated string memory should be separate from original");

    // Cleanup
    cJSON_Delete(str);
    cJSON_Delete(dup);
}

// Entry point for tests
int main() {
    std::cout << "Starting cJSON_Duplicate_rec test suite (C++11, no GTest)..." << std::endl;

    test_shallow_duplicate();
    test_deep_duplicate();
    test_duplicate_null_input();
    test_depth_limit_violation();
    test_is_reference_flag_cleared();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Total failures: " << g_test_failures << std::endl;
        return 1;
    }
}