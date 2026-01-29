// C++11 unit tests for cJSONUtils_GetPointer (wrapper around get_item_from_pointer)
//
// Notes:
// - This test suite uses the public C API provided by cJSON and cJSON_Utils.
// - No GTest is used; a lightweight in-file test harness is implemented.
// - Tests are invoked from main(), as required when GTest is not allowed.
// - Each test contains explanatory comments describing its purpose and expectations.

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON_Utils.h>
#include <limits.h>


extern "C" {
}

// Simple non-terminating test assertion macro.
// It logs a failure but does not abort test execution.
#define TEST_ASSERT(cond, msg) do { \
    tests_run++; \
    if(!(cond)) { \
        tests_failed++; \
        std::cerr << "TEST FAILED: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

// Global counters to report overall test results.
static int tests_run = 0;
static int tests_failed = 0;

// Helper to print a final summary.
static void print_summary() {
    std::cerr << "Test Summary: ran " << tests_run << " tests, "
              << tests_failed << " failed." << std::endl;
}

// Test 1: Basic retrieval of a top-level string using pointer "/name".
// Expected: returns the cJSON string object with value "ChatGPT".
static void test_GetPointer_basic_name() {
    // DOMAIN_KNOWLEDGE: Create a simple object with a single key-value pair.
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "ChatGPT");

    // Call the function under test using a JSON Pointer to the top-level key.
    cJSON *res = cJSONUtils_GetPointer(root, "/name");

    bool ok = (res != NULL
               && res->type == cJSON_String
               && res->valuestring != NULL
               && std::strcmp(res->valuestring, "ChatGPT") == 0);

    // Explanatory assertion
    TEST_ASSERT(ok, "GetPointer: expected '/name' to yield string 'ChatGPT'");

    cJSON_Delete(root);
}

// Test 2: Nested object access using pointer "/child/value".
// Object structure: { "child": { "value": "ok" } }
// Expected: returns the string "ok".
static void test_GetPointer_nested() {
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "value", "ok");
    cJSON_AddItemToObject(root, "child", child);

    cJSON *res = cJSONUtils_GetPointer(root, "/child/value");

    bool ok = (res != NULL
               && res->type == cJSON_String
               && res->valuestring != NULL
               && std::strcmp(res->valuestring, "ok") == 0);

    TEST_ASSERT(ok, "GetPointer: nested path '/child/value' should yield 'ok'");

    cJSON_Delete(root);
}

// Test 3: Array access using pointer "/arr/1".
// Object structure: { "arr": ["first", "second"] }
// Expected: returns the string "second" (second element).
static void test_GetPointer_array_index() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));
    cJSON_AddItemToObject(root, "arr", arr);

    cJSON *res = cJSONUtils_GetPointer(root, "/arr/1");

    bool ok = (res != NULL
               && res->type == cJSON_String
               && res->valuestring != NULL
               && std::strcmp(res->valuestring, "second") == 0);

    TEST_ASSERT(ok, "GetPointer: array path '/arr/1' should yield 'second'");

    cJSON_Delete(root);
}

// Test 4: Case-insensitive pointer matching using GetPointer (false => case-insensitive).
// Path uses uppercase letters for keys that exist in lowercase in the object.
static void test_GetPointer_case_insensitive() {
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "value", "ok");
    cJSON_AddItemToObject(root, "child", child);

    // Use uppercase path to verify case-insensitive behavior.
    cJSON *res = cJSONUtils_GetPointer(root, "/CHILD/VALUE");

    bool ok = (res != NULL
               && res->type == cJSON_String
               && res->valuestring != NULL
               && std::strcmp(res->valuestring, "ok") == 0);

    TEST_ASSERT(ok, "GetPointer: case-insensitive path '/CHILD/VALUE' should yield 'ok'");

    cJSON_Delete(root);
}

// Test 5: Non-existing path should return NULL (no such item).
static void test_GetPointer_non_existing() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "ChatGPT");

    cJSON *res = cJSONUtils_GetPointer(root, "/doesnotexist");

    TEST_ASSERT(res == NULL, "GetPointer: non-existing path '/doesnotexist' should yield NULL");

    cJSON_Delete(root);
}

// Test 6: Static pointer-sensitive behavior via GetPointerCaseSensitive.
// Uppercase path should fail under case-sensitive lookup, while lowercase path should succeed.
static void test_GetPointerCaseSensitive_behavior() {
    // Build a simple tree
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "value", "ok");
    cJSON_AddItemToObject(root, "child", child);

    // Case-sensitive lookup with uppercase path should fail
    cJSON *res_upper = cJSONUtils_GetPointerCaseSensitive(root, "/CHILD/VALUE");
    TEST_ASSERT(res_upper == NULL, "GetPointerCaseSensitive: '/CHILD/VALUE' should fail (case-sensitive)");

    // Case-sensitive lookup with correct lowercase path should succeed
    cJSON *res_lower = cJSONUtils_GetPointerCaseSensitive(root, "/child/value");
    bool ok = (res_lower != NULL
               && res_lower->type == cJSON_String
               && res_lower->valuestring != NULL
               && std::strcmp(res_lower->valuestring, "ok") == 0);
    TEST_ASSERT(ok, "GetPointerCaseSensitive: '/child/value' should yield 'ok'");

    cJSON_Delete(root);
}

// Test 7: Null object handling.
// If object is NULL, GetPointer should return NULL without crashing.
static void test_GetPointer_null_object() {
    cJSON *res = cJSONUtils_GetPointer(NULL, "/name");
    TEST_ASSERT(res == NULL, "GetPointer: NULL object should return NULL (no crash)");
}

// Run all tests and print summary via main().
int main() {
    // Execute tests
    test_GetPointer_basic_name();
    test_GetPointer_nested();
    test_GetPointer_array_index();
    test_GetPointer_case_insensitive();
    test_GetPointer_non_existing();
    test_GetPointerCaseSensitive_behavior();
    test_GetPointer_null_object();

    // Print final summary
    print_summary();

    // Return non-zero if any test failed to indicate failure to test harness
    return (tests_failed == 0) ? 0 : 1;
}