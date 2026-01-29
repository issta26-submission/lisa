// Lightweight C++11 unit tests for cJSON_GetStringValue from cJSON.c
// This test suite does not depend on any external testing framework.
// It uses a small in-house assertion mechanism that logs failures but continues
// execution to maximize coverage, as requested (non-terminating assertions).

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


// Bring in C code definitions with C linkage
extern "C" {
}

// Simple in-house test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

static void expect_true(bool condition, const char* message) {
    ++g_tests_run;
    if (condition) {
        ++g_tests_passed;
    } else {
        ++g_tests_failed;
        std::cerr << "Test FAILED: " << (message ? message : "unspecified failure") << std::endl;
    }
}

static void expect_str_eq(const char* expected, const char* actual, const char* message) {
    ++g_tests_run;
    if (actual != nullptr && std::strcmp(expected, actual) == 0) {
        ++g_tests_passed;
    } else {
        ++g_tests_failed;
        std::cerr << "Test FAILED: " << (message ? message : "string equality failed")
                  << " | Expected: \"" << (expected ? expected : "(null)") 
                  << "\" | Actual: \"" << (actual ? actual : "(null)") << "\"" << std::endl;
    }
}

static void expect_ptr_eq(const void* expected, const void* actual, const char* message) {
    ++g_tests_run;
    if (expected == actual) {
        ++g_tests_passed;
    } else {
        ++g_tests_failed;
        std::cerr << "Test FAILED: " << (message ? message : "pointer equality failed")
                  << " | Expected: " << expected << " | Actual: " << actual << std::endl;
    }
}

// Test 1: NULL input should yield NULL (non-string, NULL item)
static void test_GetStringValue_NullInput() {
    // Focal behavior: cJSON_GetStringValue should safely return NULL when item is NULL
    const char* result = cJSON_GetStringValue(nullptr);
    expect_true(result == NULL, "cJSON_GetStringValue(NULL) must return NULL");
}

// Test 2: Non-string item should yield NULL
static void test_GetStringValue_NonStringItem() {
    // Create a numeric item
    cJSON* number = cJSON_CreateNumber(42);
    // Since it's not a string, GetStringValue should return NULL
    const char* result = cJSON_GetStringValue(number);
    expect_true(result == NULL, "cJSON_GetStringValue on non-string item must return NULL");
    cJSON_Delete(number);
}

// Test 3: Valid string item should return pointer to its internal string and match content
static void test_GetStringValue_StringItem() {
    // Create a string item
    cJSON* strItem = cJSON_CreateString("hello");
    // Should return non-NULL pointer
    const char* result = cJSON_GetStringValue(strItem);
    expect_true(result != NULL, "cJSON_GetStringValue should return non-NULL for string item");
    // Content should match
    expect_str_eq("hello", result, "Returned string content must be 'hello'");
    // The returned pointer should point to the item's internal valuestring
    // Accessing item->valuestring is legitimate (public member) for testing
    expect_ptr_eq((void*)strItem->valuestring, (void*)result, "Returned pointer must equal item->valuestring");
    cJSON_Delete(strItem);
}

// Test 4: Empty string should return a pointer to an empty string
static void test_GetStringValue_EmptyString() {
    cJSON* emptyStr = cJSON_CreateString("");
    const char* result = cJSON_GetStringValue(emptyStr);
    expect_true(result != NULL, "cJSON_GetStringValue should return non-NULL for empty string");
    expect_str_eq("", result, "Returned string content must be empty");
    cJSON_Delete(emptyStr);
}

// Entry point to run all tests
int main() {
    // Run tests
    test_GetStringValue_NullInput();
    test_GetStringValue_NonStringItem();
    test_GetStringValue_StringItem();
    test_GetStringValue_EmptyString();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << " | Passed: " << g_tests_passed
              << " | Failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}