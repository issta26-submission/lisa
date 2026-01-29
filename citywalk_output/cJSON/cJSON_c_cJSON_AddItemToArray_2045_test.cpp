// This test suite targets the focal function:
// CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToArray(cJSON *array, cJSON *item)
// which delegates to the static helper add_item_to_array in cJSON.c.
// The tests are written in C++11, do not rely on Google Test, and use a lightweight
// non-terminating assertion mechanism to maximize code coverage.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


// Include the C API for cJSON. Ensure the project links against cJSON.c
extern "C" {
}

// Global test state
static int g_failures = 0;

// Lightweight non-terminating assertions
static void EXPECT_TRUE(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "EXPECT_TRUE FAILED: " << (msg ? msg : "") << std::endl;
        ++g_failures;
    }
}
static void EXPECT_EQ_INT(int a, int b, const char* msg) {
    if (a != b) {
        std::cerr << "EXPECT_EQ_INT FAILED: " 
                  << (msg ? msg : "") 
                  << " (expected " << a << ", got " << b << ")" << std::endl;
        ++g_failures;
    }
}
static void EXPECT_NOT_NULL(void* p, const char* msg) {
    if (p == nullptr) {
        std::cerr << "EXPECT_NOT_NULL FAILED: " << (msg ? msg : "") << std::endl;
        ++g_failures;
    }
}
static void EXPECT_NULL(const void* p, const char* msg) {
    if (p != nullptr) {
        std::cerr << "EXPECT_NULL FAILED: " << (msg ? msg : "") << std::endl;
        ++g_failures;
    }
}

// Domain-oriented notes for maintainers:
// - We test the public wrapper cJSON_AddItemToArray, which should append 'item' to the end of
//   the JSON array pointed by 'array' and return a non-zero boolean on success.
// - We verify both content (values inside the JSON items) and structure (array size).
// - We cover true and false branches by testing valid and invalid usage (null/incorrect types).
// - We only use the public API; static internal helpers are not directly invoked.
// - We do not terminate tests on first failure to maximize code coverage.

// Test 1: Basic use - add a single item to a new array and verify contents
static void test_AddItemToArray_Basic() {
    // Step 2: Unit under test - cJSON_AddItemToArray should append item to array
    cJSON *array = cJSON_CreateArray();
    EXPECT_NOT_NULL(array, "Failed to create JSON array for basic test");

    cJSON *item = cJSON_CreateNumber(42);
    EXPECT_NOT_NULL(item, "Failed to create numeric JSON item for basic test");

    int result = static_cast<int>(cJSON_AddItemToArray(array, item));
    EXPECT_TRUE(result == 1, "AddItemToArray should return true (1) on success");

    // The first (and only) element should be accessible at index 0
    cJSON *elem = cJSON_GetArrayItem(array, 0);
    EXPECT_NOT_NULL(elem, "Array item at index 0 should exist after add");

    double val = cJSON_GetNumberValue(elem);
    EXPECT_EQ_INT(static_cast<int>(val), 42, "First array item should have value 42");

    cJSON_Delete(array);
}

// Test 2: Add multiple items and verify size and order
static void test_AddItemToArray_Multiple() {
    cJSON *array = cJSON_CreateArray();
    EXPECT_NOT_NULL(array, "Failed to create JSON array for multi-item test");

    cJSON *item1 = cJSON_CreateNumber(1);
    cJSON *item2 = cJSON_CreateNumber(2);
    EXPECT_NOT_NULL(item1, "Failed to create first numeric item");
    EXPECT_NOT_NULL(item2, "Failed to create second numeric item");

    EXPECT_TRUE(static_cast<int>(cJSON_AddItemToArray(array, item1)) == 1,
                "First add should succeed");
    EXPECT_TRUE(static_cast<int>(cJSON_AddItemToArray(array, item2)) == 1,
                "Second add should succeed");

    int size = cJSON_GetArraySize(array);
    EXPECT_EQ_INT(size, 2, "Array size should be 2 after adding two items");

    cJSON *elem0 = cJSON_GetArrayItem(array, 0);
    cJSON *elem1 = cJSON_GetArrayItem(array, 1);
    EXPECT_NOT_NULL(elem0, "Element 0 should exist");
    EXPECT_NOT_NULL(elem1, "Element 1 should exist");

    EXPECT_EQ_INT(static_cast<int>(cJSON_GetNumberValue(elem0)), 1, "Element 0 should be 1");
    EXPECT_EQ_INT(static_cast<int>(cJSON_GetNumberValue(elem1)), 2, "Element 1 should be 2");

    cJSON_Delete(array);
}

// Test 3: Attempt to add to a non-array object should fail (false branch)
static void test_AddItemToArray_WrongContainer() {
    cJSON *object = cJSON_CreateObject();
    EXPECT_NOT_NULL(object, "Failed to create JSON object for wrong-container test");

    cJSON *item = cJSON_CreateNumber(3);
    EXPECT_NOT_NULL(item, "Failed to create item for wrong-container test");

    int result = static_cast<int>(cJSON_AddItemToArray(object, item));
    EXPECT_TRUE(result == 0, "AddItemToArray should fail when container is not an array");

    cJSON_Delete(object); // item has not been added, safe to delete
}

// Test 4: Null container should gracefully fail (handled by public API)
static void test_AddItemToArray_NullContainer() {
    cJSON *item = cJSON_CreateNumber(5);
    EXPECT_NOT_NULL(item, "Failed to create item for null-container test");

    int result = static_cast<int>(cJSON_AddItemToArray(nullptr, item));
    EXPECT_TRUE(result == 0, "AddItemToArray should fail when container is NULL");

    cJSON_Delete(item);
}

// Test 5: Null item should gracefully fail (non-terminating test path)
static void test_AddItemToArray_NullItem() {
    cJSON *array = cJSON_CreateArray();
    EXPECT_NOT_NULL(array, "Failed to create JSON array for null-item test");

    int result = static_cast<int>(cJSON_AddItemToArray(array, nullptr));
    // Expect a non-zero/zero depending on implementation; typical behavior is 0 (failure)
    EXPECT_TRUE(result == 0, "AddItemToArray should fail when item is NULL");

    cJSON_Delete(array);
}

// Main entry point to run tests
int main() {
    // Run individual test functions
    test_AddItemToArray_Basic();
    test_AddItemToArray_Multiple();
    test_AddItemToArray_WrongContainer();
    test_AddItemToArray_NullContainer();
    test_AddItemToArray_NullItem();

    // Summary
    std::cout << "Total test failures: " << g_failures << std::endl;
    return g_failures ? 1 : 0;
}