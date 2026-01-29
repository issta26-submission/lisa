/*
Unit test suite for cJSON_AddStringToObject in cJSON.c
- No GTest; a lightweight test harness is implemented in plain C++11.
- Tests focus on the behavior of cJSON_AddStringToObject:
  1) Success path: when adding to a real JSON object.
  2) Failure path: when the target is not an object (e.g., an array).
- The tests are designed to be executable with standard C++11 build tooling and
  link against the cJSON C source (e.g., by compiling cJSON.c and this test together).
- Explanatory comments are provided for each unit test.
*/

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>

extern "C" {
    // Include the cJSON API (C header)
    #include "cJSON.h"
}

// Lightweight test harness (non-terminating assertions)
static void report_success(const std::string& test_name) {
    std::cout << "[TEST PASS] " << test_name << std::endl;
}
static void report_failure(const std::string& test_name, const std::string& reason) {
    std::cerr << "[TEST FAIL] " << test_name << " - " << reason << std::endl;
}

// Test 1: Successful addition to a valid JSON object
// - Creates a JSON object
// - Calls cJSON_AddStringToObject with a name and string value
// - Expects a non-NULL return pointer to the newly created string item
// - Verifies: the returned pointer is the same as the object's member for the given key
// - Verifies: the stored value matches the input string
static void test_AddStringToObject_Success() {
    const std::string test_name = "AddStringToObject_Success";

    // Arrange: a valid JSON object
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) {
        report_failure(test_name, "Failed to create object");
        return;
    }

    // Act: add a string to the object
    const char* key = "greeting";
    const char* value = "Hello";
    cJSON *result = cJSON_AddStringToObject(object, key, value);

    // Assert: result should be non-NULL
    if (result == NULL) {
        report_failure(test_name, "Expected non-NULL result from cJSON_AddStringToObject");
        cJSON_Delete(object);
        return;
    }

    // Additional Assert: the object's item for the key should be the same pointer
    cJSON *child = cJSON_GetObjectItemCaseSensitive(object, key);
    if (child == NULL) {
        report_failure(test_name, "Expected object to contain the key after insertion");
        cJSON_Delete(object);
        return;
    }

    if (result != child) {
        report_failure(test_name, "Returned pointer does not match the object's child for the given key");
        cJSON_Delete(object);
        return;
    }

    // Assert: the value stored matches
    const char *stored = cJSON_GetStringValue(child);
    if (stored == NULL || std::string(stored) != value) {
        report_failure(test_name, "Stored string value does not match input value");
        cJSON_Delete(object);
        return;
    }

    // Cleanup
    cJSON_Delete(object);

    // If reached here, test passes
    report_success(test_name);
}

// Test 2: Failure path when the target is not a JSON object (e.g., an array)
// - Creates a JSON array
// - Calls cJSON_AddStringToObject which should fail and return NULL
// - Verifies that NULL is returned
// - Verifies that the array size remains unchanged (i.e., zero)
static void test_AddStringToObject_Failure_ArrayTarget() {
    const std::string test_name = "AddStringToObject_Failure_ArrayTarget";

    // Arrange: create an array instead of an object
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        report_failure(test_name, "Failed to create array");
        return;
    }

    // Act: attempt to add string to array as if it were an object
    const char* key = "name";
    const char* value = "value";
    cJSON *result = cJSON_AddStringToObject(array, key, value);

    // Assert: result should be NULL on failure
    if (result != NULL) {
        report_failure(test_name, "Expected NULL result when target is not an object");
        cJSON_Delete(array);
        return;
    }

    // Also ensure array size remains unchanged (still 0)
    int size = cJSON_GetArraySize(array);
    if (size != 0) {
        report_failure(test_name, "Array size changed after failed insertion; expected 0");
        cJSON_Delete(array);
        return;
    }

    // Cleanup
    cJSON_Delete(array);

    // If reached here, test passes
    report_success(test_name);
}

int main() {
    // Run tests
    test_AddStringToObject_Success();
    test_AddStringToObject_Failure_ArrayTarget();

    // Note: This lightweight harness prints results to stdout/stderr.
    // A full test runner could collect counts, but this satisfies the requirement
    // of executable, non-terminating tests in a single file.

    return 0;
}