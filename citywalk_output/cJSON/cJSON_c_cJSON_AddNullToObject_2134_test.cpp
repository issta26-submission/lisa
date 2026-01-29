// Unit test suite for cJSON_AddNullToObject (C JSON library) using a lightweight C++11 test harness.
// This test suite is designed to be executed without GoogleTest and uses non-terminating
// assertions to maximize code coverage.
// It targets the focal method described in <FOCAL_METHOD> and exercises true/false branches,
// including edge cases such as NULL object and duplicate keys.

// Step 1 (Insight): The focal method creates a JSON null item, attempts to attach it to the
// provided object under a given key via add_item_to_object, and returns the created item on success.
// - Success path (true branch): returns the created null item.
// - Failure path (false branch): deletes the created item and returns NULL.
// - Edge cases include NULL object, NULL key, and duplicate keys (which can cause failure in
//   the addition to the object).

// Candidate Keywords (core dependencies to test): cJSON_CreateNull, add_item_to_object, global_hooks,
// cJSON_Delete, cJSON_IsNull, cJSON_GetObjectItem, cJSON_CreateObject, cJSON_AddNullToObject.

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


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static std::vector<std::string> g_failures;

// Log a failure message without terminating the program
static void logFailure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Assertion macro: record failure with a human-readable note if condition is false
#define EXPECT_TRUE(cond, note) do { \
    if(!(cond)) { logFailure(std::string("EXPECT_TRUE failed: ") + (note)); } \
} while(0)

// Test 1: True branch - when the object is valid and the key is new.
// Verify that a NULL value is created, attached to the object under the provided name,
// and that the returned pointer equals the object's child for that key.
static void test_AddNullToObject_true_branch() {
    // Create a new JSON object
    cJSON *object = cJSON_CreateObject();
    EXPECT_TRUE(object != NULL, "object should be created successfully");

    // Add a NULL value under key "foo"
    cJSON *result = cJSON_AddNullToObject(object, "foo");
    EXPECT_TRUE(result != NULL, "result should not be NULL on success");
    EXPECT_TRUE(cJSON_IsNull(result) != 0, "result should represent a JSON null value");

    // The object should contain the same item under key "foo" as the returned result
    cJSON *item_in_object = cJSON_GetObjectItem(object, "foo");
    EXPECT_TRUE(item_in_object != NULL, "object should contain key 'foo'");
    EXPECT_TRUE(item_in_object == result, "returned item should equal the object's child for key 'foo'");

    // Cleanup
    cJSON_Delete(object);
}

// Test 2: Edge case - NULL object pointer should fail gracefully and return NULL
static void test_AddNullToObject_null_object() {
    cJSON *result = cJSON_AddNullToObject(NULL, "foo");
    EXPECT_TRUE(result == NULL, "return NULL when object is NULL");
}

// Test 3: False branch - duplicate key should cause add to fail and return NULL
static void test_AddNullToObject_duplicate_key_false_branch() {
    cJSON *object = cJSON_CreateObject();
    EXPECT_TRUE(object != NULL, "object should be created successfully for duplicate-key test");

    // First insertion should succeed
    cJSON *first = cJSON_AddNullToObject(object, "dup");
    EXPECT_TRUE(first != NULL, "first insert should succeed for key 'dup'");

    // Second insertion with the same key should fail and return NULL
    cJSON *second = cJSON_AddNullToObject(object, "dup");
    EXPECT_TRUE(second == NULL, "second insert with duplicate key should fail and return NULL");

    // Cleanup
    cJSON_Delete(object);
}

// Test 4: Edge case - NULL key name should fail gracefully and return NULL
static void test_AddNullToObject_null_name() {
    cJSON *object = cJSON_CreateObject();
    EXPECT_TRUE(object != NULL, "object should be created successfully for NULL-name test");

    cJSON *res = cJSON_AddNullToObject(object, NULL);
    EXPECT_TRUE(res == NULL, "return NULL when key name is NULL");

    // Cleanup
    cJSON_Delete(object);
}

// Run all tests and report results
static void runAllTests() {
    test_AddNullToObject_true_branch();
    test_AddNullToObject_null_object();
    test_AddNullToObject_duplicate_key_false_branch();
    test_AddNullToObject_null_name();
}

int main() {
    // Execute tests
    runAllTests();

    // Summary
    if(g_failures.empty()) {
        std::cout << "All tests passed for cJSON_AddNullToObject.\n";
        return 0;
    } else {
        std::cout << g_failures.size() << " test(s) failed for cJSON_AddNullToObject:\n";
        for(const auto& fail : g_failures) {
            std::cout << " - " << fail << "\n";
        }
        return 1;
    }
}