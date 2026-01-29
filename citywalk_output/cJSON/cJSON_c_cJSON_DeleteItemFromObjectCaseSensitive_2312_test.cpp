// Test suite for cJSON_DeleteItemFromObjectCaseSensitive in cJSON.c
// This test suite is written in C++11 (no GoogleTest) and uses only the C library provided by cJSON.
// It validates that cJSON_DeleteItemFromObjectCaseSensitive correctly detaches and deletes
// an item from a JSON object in a case-sensitive manner, covering both existing and non-existing keys.

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

// Lightweight test harness (non-terminating): records failures but continues execution.
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper: non-terminating assertion with diagnostic message.
static bool expect_true(bool cond, const char* desc) {
    if (!cond) {
        std::fprintf(stderr, "Assertion failed: %s\n", desc);
        return false;
    }
    return true;
}

// Test 1: Delete an existing item from object using exact case (ItemExists -> Removed)
static bool test_DeleteItemFromObjectCaseSensitive_ItemExists_Removed() {
    bool all_ok = true;
    // Create an object with two keys: "name" and "other"
    cJSON *object = cJSON_CreateObject();
    if (!expect_true(object != nullptr, "Failed to create JSON object")) {
        all_ok = false;
        return all_ok;
    }

    cJSON_AddStringToObject(object, "name", "value");
    cJSON_AddStringToObject(object, "other", "x");

    // Perform the operation under test: delete "name" (case-sensitive)
    cJSON_DeleteItemFromObjectCaseSensitive(object, "name");

    // Verify: "name" should be removed
    cJSON *item = cJSON_GetObjectItemCaseSensitive(object, "name");
    all_ok = all_ok && expect_true(item == nullptr, "\"name\" should be removed from object");

    // Verify: other item remains
    all_ok = all_ok && expect_true(cJSON_HasObjectItem(object, "other"), "\"other\" should remain in object");

    // Cleanup
    cJSON_Delete(object);
    return all_ok;
}

// Test 2: Attempt to delete a non-existent item (ItemNotExists -> No change, no crash)
static bool test_DeleteItemFromObjectCaseSensitive_ItemNotExists_NoChange() {
    bool all_ok = true;
    cJSON *object = cJSON_CreateObject();
    if (!expect_true(object != nullptr, "Failed to create JSON object (Test 2)")) {
        all_ok = false;
        return all_ok;
    }

    cJSON_AddStringToObject(object, "name", "value");

    // Attempt to delete a different case key that does not exist
    cJSON_DeleteItemFromObjectCaseSensitive(object, "NAME");

    // Verify: original "name" still exists
    all_ok = all_ok && expect_true(cJSON_HasObjectItem(object, "name"), "\"name\" should still exist after attempting to delete non-existent key");

    // Cleanup
    cJSON_Delete(object);
    return all_ok;
}

// Test 3: Ensure case sensitivity is respected when multiple keys with similar names exist
static bool test_DeleteItemFromObjectCaseSensitive_CaseSensitivityAmongKeys() {
    bool all_ok = true;
    cJSON *object = cJSON_CreateObject();
    if (!expect_true(object != nullptr, "Failed to create JSON object (Test 3)")) {
        all_ok = false;
        return all_ok;
    }

    // Add two keys that differ only by case
    cJSON_AddStringToObject(object, "a", "lowercase");
    cJSON_AddStringToObject(object, "A", "uppercase");

    // Delete the lowercase key "a"
    cJSON_DeleteItemFromObjectCaseSensitive(object, "a");

    // Verify: "a" is removed, "A" remains
    all_ok = all_ok && expect_true(cJSON_GetObjectItemCaseSensitive(object, "a") == nullptr,
                                 "\"a\" should be removed (case-sensitive)");
    all_ok = all_ok && expect_true(cJSON_HasObjectItem(object, "A"),
                                 "\"A\" should remain after deleting \"a\"");

    // Cleanup
    cJSON_Delete(object);
    return all_ok;
}

// Main test runner
int main() {
    // Run tests; non-terminating assertions ensure full coverage
    g_tests_run = 0;
    g_tests_failed = 0;

    printf("Running tests for cJSON_DeleteItemFromObjectCaseSensitive...\n");

    bool t1 = test_DeleteItemFromObjectCaseSensitive_ItemExists_Removed();
    g_tests_run++;

    if (!t1) {
        g_tests_failed++;
        printf("Test 1 FAILED.\n");
    } else {
        printf("Test 1 PASSED.\n");
    }

    bool t2 = test_DeleteItemFromObjectCaseSensitive_ItemNotExists_NoChange();
    g_tests_run++;

    if (!t2) {
        g_tests_failed++;
        printf("Test 2 FAILED.\n");
    } else {
        printf("Test 2 PASSED.\n");
    }

    bool t3 = test_DeleteItemFromObjectCaseSensitive_CaseSensitivityAmongKeys();
    g_tests_run++;

    if (!t3) {
        g_tests_failed++;
        printf("Test 3 FAILED.\n");
    } else {
        printf("Test 3 PASSED.\n");
    }

    int passed = g_tests_run - g_tests_failed;
    printf("Summary: Ran %d tests, Passed %d, Failed %d\n", g_tests_run, passed, g_tests_failed);

    // Return non-zero if any test failed to aid integration in CI
    return (g_tests_failed == 0) ? 0 : 1;
}