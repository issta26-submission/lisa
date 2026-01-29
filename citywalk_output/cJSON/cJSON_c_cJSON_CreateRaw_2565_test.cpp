// Test suite for the focal method: cJSON_CreateRaw (from cJSON.c)
// This test suite is written for C++11 (no Google Test). It directly exercises
// the C API provided by cJSON.h. The tests are designed to run in a C++
// environment and rely on the existing cJSON.c implementation (which uses
// global_hooks and the internal cJSON structure).

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


// Include the C library header. Ensure C linkage for C++ compilation.
extern "C" {
}

// Global failure counter for test results
static int g_failures = 0;

// Lightweight non-terminating assertion macro.
// If condition fails, log the message and increment the failure counter.
// Execution continues to allow broader coverage in a single run.
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (Condition: " #cond ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Step 2: Unit Test Generation for cJSON_CreateRaw
// Candidate Keywords (from Step 1): cJSON_New_Item, global_hooks, cJSON_Raw, valuestring,
// cJSON_strdup, cJSON_Delete, internal_hooks, NULL return path.

// Test 1: Basic positive path - ensure that a non-NULL input yields a valid RAW item.
static void test_CreateRaw_basic() {
    // Given a simple non-empty input
    const char* raw = "hello";

    // When creating a raw JSON item
    cJSON *item = cJSON_CreateRaw(raw);

    // Then the item should be non-NULL
    CHECK(item != NULL, "cJSON_CreateRaw should return non-NULL for non-NULL input");

    // And its type should be RAW
    if (item) {
        CHECK(item->type == cJSON_Raw, "item->type should be cJSON_Raw");

        // And the valuestring should be allocated and equal to the input
        CHECK(item->valuestring != NULL, "item->valuestring should be non-NULL");

        if (item && item->valuestring) {
            CHECK(std::strcmp(item->valuestring, raw) == 0,
                  "item->valuestring should match the input string exactly");
        }

        // Cleanup
        cJSON_Delete(item);
    }
}

// Test 2: Null input path - ensure that NULL input returns NULL (core branch of the code).
static void test_CreateRaw_null_input() {
    // When input is NULL
    cJSON *item = cJSON_CreateRaw(NULL);

    // Then the function should return NULL
    CHECK(item == NULL, "cJSON_CreateRaw(NULL) should return NULL when input is NULL");

    // No cleanup needed if NULL
}

// Test 3: Empty string path - edge case to ensure empty string is handled gracefully.
static void test_CreateRaw_empty_string() {
    const char* raw = "";

    cJSON *item = cJSON_CreateRaw(raw);

    // Expect non-NULL with an empty string in valuestring
    CHECK(item != NULL, "cJSON_CreateRaw should succeed for empty string input");

    if (item) {
        CHECK(item->valuestring != NULL, "valuestring should be non-NULL for empty input");
        if (item->valuestring) {
            CHECK(std::strcmp(item->valuestring, raw) == 0,
                  "valuestring should be an empty string for empty input");
        }
        cJSON_Delete(item);
    }
}

// Step 3: Test Case Refinement
// Additional test to ensure the function handles a non-trivial string with special chars.
static void test_CreateRaw_special_characters() {
    const char* raw = "line1\nline2\tend";

    cJSON *item = cJSON_CreateRaw(raw);

    CHECK(item != NULL, "cJSON_CreateRaw should handle strings with special characters");

    if (item) {
        CHECK(item->type == cJSON_Raw, "type should be cJSON_Raw for special characters input");
        CHECK(item->valuestring != NULL, "valuestring should be non-NULL for special characters input");
        if (item->valuestring) {
            CHECK(std::strcmp(item->valuestring, raw) == 0,
                  "valuestring should preserve special characters exactly");
        }
        cJSON_Delete(item);
    }
}

// Main entrypoint to run all tests.
// In case of future expansion, more test cases can be added and called here.
int main() {
    std::cout << "Running tests for cJSON_CreateRaw (C API)..." << std::endl;

    // Step 2: Execute test cases
    test_CreateRaw_basic();
    test_CreateRaw_null_input();
    test_CreateRaw_empty_string();
    test_CreateRaw_special_characters();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed for cJSON_CreateRaw." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed for cJSON_CreateRaw." << std::endl;
        return 1;
    }
}

/*
Notes:
- This test suite uses direct C API calls and validates both true and false branches
  of the conditional logic inside cJSON_CreateRaw (as far as we can exercise without
  manipulating malloc failures or internal hooks).
- We intentionally avoid accessing private/static helpers and focus on observable
  behavior via public C API (as per the provided domain knowledge).
- The test harness uses simple non-terminating checks to maximize test execution
  coverage within a single run and to allow subsequent tests to execute even if one
  assertion fails.
- To compile:
  - Ensure cJSON.c and cJSON.h are available in the project.
  - Compile with a C++11 compiler, for example:
      g++ -std=c++11 -I. test_cjson_raw.cpp cJSON.c -o test_cjson_raw
  - Run: ./test_cjson_raw
*/