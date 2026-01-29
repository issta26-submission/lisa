// High-quality C++11 unit tests for the focal function: cJSON_CreateString
// This test suite exercises various code paths of the C function under test
// without using GTest. A lightweight, non-terminating assertion framework is used.

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
#include <limits.h>


extern "C" {
}

// Lightweight test harness
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "CHECK FAILED: %s (in %s:%d)\n", (msg), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

// Test 1: Basic path - creating a simple string should produce a non-NULL item with type String
// and a valid valuestring that matches the input.
void test_CreateString_basic() {
    // Given
    const char* input = "hello";

    // When
    cJSON* item = cJSON_CreateString(input);

    // Then
    CHECK(item != NULL, "cJSON_CreateString(\"hello\") returned NULL");
    if(item != NULL) {
        CHECK(item->type == cJSON_String, "item->type should be cJSON_String");
        CHECK(item->valuestring != NULL, "item->valuestring should be non-NULL");
        if(item->valuestring != NULL) {
            CHECK(strcmp(item->valuestring, input) == 0,
                  "item->valuestring content should match input");
        }
        cJSON_Delete(item);
    }
}

// Test 2: Empty string input should still yield a valid item with an empty valuestring
void test_CreateString_empty_input() {
    // Given
    const char* input = "";

    // When
    cJSON* item = cJSON_CreateString(input);

    // Then
    CHECK(item != NULL, "cJSON_CreateString(\"\") returned NULL");
    if(item != NULL) {
        CHECK(item->type == cJSON_String, "item->type should be cJSON_String for empty string");
        CHECK(item->valuestring != NULL, "item->valuestring should be non-NULL even for empty string");
        if(item->valuestring != NULL) {
            CHECK(strcmp(item->valuestring, input) == 0,
                  "item->valuestring content should match empty input");
        }
        cJSON_Delete(item);
    }
}

// Test 3: NULL input should gracefully fail, returning NULL as per the implementation path
// where cJSON_strdup would receive NULL and return NULL.
void test_CreateString_null_input_returns_null() {
    // Given (NULL input)
    const char* input = NULL;

    // When
    cJSON* item = cJSON_CreateString(input);

    // Then
    CHECK(item == NULL, "cJSON_CreateString(NULL) should return NULL");
    // No cleanup needed since item is NULL
}

// Test 4: Simulate allocation failure via custom hooks to ensure the function
// properly returns NULL when memory allocation fails during item creation.
static void* fail_malloc(size_t) { return nullptr; }
static void  fail_free(void*) { /* no-op */ }
static void* fail_realloc(void*, size_t) { return nullptr; }

void test_CreateString_allocation_failure() {
    // Given: replace global allocation hooks with ones that fail
    cJSON_Hooks failing_hooks;
    failing_hooks.malloc_fn = fail_malloc;
    failing_hooks.free_fn   = fail_free;
    failing_hooks.realloc_fn= fail_realloc;
    cJSON_InitHooks(&failing_hooks);

    // When: attempt to create a string with allocation failure
    const char* input = "will fail";
    cJSON* item = cJSON_CreateString(input);

    // Then: item must be NULL due to allocation failure
    CHECK(item == NULL, "cJSON_CreateString should return NULL when malloc fails via hooks");

    // Cleanup: restore default allocation hooks to avoid side effects on other tests
    cJSON_InitHooks(NULL);
    // No item to delete here since result should be NULL
}

// Entry point for running tests
int main() {
    // Run tests
    test_CreateString_basic();
    test_CreateString_empty_input();
    test_CreateString_null_input_returns_null();
    test_CreateString_allocation_failure();

    // Summary
    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}