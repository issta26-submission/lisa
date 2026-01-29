// Test suite for cJSON_AddItemToObjectCS in cJSON.c
// This test suite is written for C++11 without a third-party test framework.
// It uses the C API of cJSON and a small, self-contained test harness.

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
#include <limits.h>


// Include the C library headers with C linkage
extern "C" {
}

// Simple test harness (non-terminating assertions)
static int tests_passed = 0;
static int tests_total = 0;

#define ASSERT_TRUE(cond, msg) do { \
    tests_total++; \
    if (cond) { \
        ++tests_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT_FALSE(cond, msg) ASSERT_TRUE(!(cond), msg)

#define ASSERT_PTR_EQ(a, b, msg) do { \
    tests_total++; \
    if ((a) == (b)) { ++tests_passed; } \
    else { std::cerr << "Test failed: " << (msg) \
        << " (expected " << (void*)(b) << ", got " << (void*)(a) << ")" << std::endl; } \
} while (0)

#define ASSERT_STR_EQ(a, b, msg) do { \
    tests_total++; \
    if ((a) != nullptr && (b) != nullptr && std::strcmp((a), (b)) == 0) { ++tests_passed; } \
    else { std::cerr << "Test failed: " << (msg) \
        << " (expected \"" << (b) << "\", got \"" << (a) << "\")" << std::endl; } \
} while (0)

// Test 1: Basic addition of a value to an object using the CS variant.
// Verifies that the item is added with the correct key and that the pointer
// to the value remains the same (no extra copies or reallocation).
static void test_basic_addition() {
    // Create a new JSON object and a value to insert
    cJSON *object = cJSON_CreateObject();
    cJSON *value = cJSON_CreateString("hello");

    // Act: add item to object with a constant key via AddItemToObjectCS
    cJSON_bool res = cJSON_AddItemToObjectCS(object, "greeting", value);

    // Assert: operation succeeded
    ASSERT_TRUE(res, "cJSON_AddItemToObjectCS should return true for valid inputs");

    // Retrieve by key and verify it's the same pointer as inserted
    cJSON *retrieved = cJSON_GetObjectItem(object, "greeting");
    ASSERT_PTR_EQ(retrieved, value, "Retrieved item should be identical to the inserted item");

    // Also verify the value content is as expected (the string value of the item)
    const char *value_str = cJSON_GetStringValue(retrieved);
    ASSERT_STR_EQ(value_str, "hello", "Inserted item's value string should be 'hello'");

    // Cleanup
    cJSON_Delete(object);
}

// Test 2: Null inputs should fail gracefully (no crashes, return false).
static void test_null_inputs() {
    // Case A: NULL object
    // No value needed; operation should fail
    cJSON_bool res1 = cJSON_AddItemToObjectCS(NULL, "k", NULL);
    ASSERT_FALSE(res1, "AddItemToObjectCS with NULL object should fail");

    // Prepare a valid object and a real value for subsequent sub-cases
    cJSON *obj = cJSON_CreateObject();
    cJSON *val = cJSON_CreateString("dummy");

    // Case B: NULL key string
    cJSON_bool res2 = cJSON_AddItemToObjectCS(obj, NULL, val);
    ASSERT_FALSE(res2, "AddItemToObjectCS with NULL key should fail");

    // Case C: NULL item
    cJSON_bool res3 = cJSON_AddItemToObjectCS(obj, "key", NULL);
    ASSERT_FALSE(res3, "AddItemToObjectCS with NULL item should fail");

    // Cleanup any allocated objects
    cJSON_Delete(val);
    cJSON_Delete(obj);
}

// Test 3: Replacing an existing key should update the value to the new item.
static void test_existing_key_replacement() {
    cJSON *object = cJSON_CreateObject();

    // Insert first item with key "dup"
    cJSON *first = cJSON_CreateString("first");
    cJSON_bool r1 = cJSON_AddItemToObjectCS(object, "dup", first);
    ASSERT_TRUE(r1, "First insertion should succeed");

    // Insert second item with the same key "dup"
    cJSON *second = cJSON_CreateString("second");
    cJSON_bool r2 = cJSON_AddItemToObjectCS(object, "dup", second);
    ASSERT_TRUE(r2, "Second insertion with same key should succeed and replace");

    // Retrieve by key and verify replacement
    cJSON *retrieved = cJSON_GetObjectItem(object, "dup");
    ASSERT_PTR_EQ(retrieved, second, "After replacement, value should be the new item");

    // Cleanup: object will free its children
    cJSON_Delete(object);
}

// Test 4: When the object provided is not actually an object, the operation should fail.
static void test_not_object_fails() {
    // Create something that is not an object
    cJSON *notObject = cJSON_CreateString("not object");
    cJSON *value = cJSON_CreateString("value");

    cJSON_bool res = cJSON_AddItemToObjectCS(notObject, "k", value);
    ASSERT_FALSE(res, "AddItemToObjectCS should fail when the target is not an object (non-object input)");

    // Cleanup
    cJSON_Delete(notObject); // value is not attached to non-object in this failing scenario
    cJSON_Delete(value);
}

// Main function: run all tests and report summary
int main() {
    test_basic_addition();
    test_null_inputs();
    test_existing_key_replacement();
    test_not_object_fails();

    std::cout << "Tests run: " << tests_total << ", Passed: " << tests_passed
              << ", Failed: " << (tests_total - tests_passed) << std::endl;

    // Return non-zero if any test failed, to aid integration with simple pipelines
    return (tests_total == tests_passed) ? 0 : 1;
}