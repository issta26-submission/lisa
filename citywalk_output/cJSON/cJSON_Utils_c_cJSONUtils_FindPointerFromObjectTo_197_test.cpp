// C++11 test suite for the focal C function:
// cJSONUtils_FindPointerFromObjectTo(const cJSON * const object, const cJSON * const target)
// This test suite is designed to be compiled with a C++11 compiler without GoogleTest.
// It exercises key code paths identified in the focal method and its dependencies.
// The tests use the real cJSON/CJSON_Utils facilities (no private/internal access).

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cJSON_Utils.h>
#include <limits.h>


// Bring C symbols into C++ tests
extern "C" {
}

// Simple non-terminating test harness (EXPECT_ macros)
static int g_total_tests  = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " in " << __FILE__ \
                  << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_NULL(ptr) do { \
    ++g_total_tests; \
    if((ptr) != NULL) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_NULL failed: " << #ptr << " is not NULL in " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
    ++g_total_tests; \
    if((ptr) == NULL) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_NOT_NULL failed: " << #ptr << " is NULL in " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_STR_EQ(expected, actual) do { \
    ++g_total_tests; \
    if((actual) == NULL) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_STR_EQ failed: actual is NULL, expected \"" << (expected) \
                  << "\" in " << __FILE__ << ":" << __LINE__ << std::endl; \
    } else if(std::strcmp((const char*)(actual), (const char*)(expected)) != 0) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_STR_EQ failed: expected \"" << (expected) \
                  << "\", got \"" << (const char*)actual << "\" in " << __FILE__ \
                  << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_STR_PREFIX(prefix, actual) do { \
    ++g_total_tests; \
    if((actual) == NULL) { \
        ++g_failed_tests; \
        std::cerr << "EXPECT_STR_PREFIX failed: actual is NULL (no prefix) in " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
    } else { \
        if(std::strncmp((const char*)(actual), (prefix), std::strlen(prefix)) != 0) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_STR_PREFIX failed: expected prefix \"" \
                      << (const char*)(prefix) << "\", got \"" \
                      << (const char*)actual << "\" in " << __FILE__ \
                      << ":" << __LINE__ << std::endl; \
        } \
    } \
} while(0)

// Test 1: Null checks - object == NULL or target == NULL should return NULL
void test_null_checks() {
    // Prepare a concrete target to pass alongside NULL object
    cJSON *target = cJSON_CreateNumber(1);
    // 1A: object is NULL
    char *res1 = cJSONUtils_FindPointerFromObjectTo(NULL, target);
    EXPECT_NULL(res1);

    // 1B: target is NULL
    cJSON *obj = cJSON_CreateArray();
    char *res2 = cJSONUtils_FindPointerFromObjectTo(obj, NULL);
    EXPECT_NULL(res2);

    // Cleanup
    if (target) cJSON_Delete(target);
    if (obj) cJSON_Delete(obj);
}

// Test 2: object == target should return an empty string (pointer to "").
// Verifies base case and proper memory allocation via cJSONUtils_strdup
void test_object_equals_target_base_case() {
    cJSON *obj = cJSON_CreateObject();
    // When object == target, pointer should be "" (empty string)
    char *res = cJSONUtils_FindPointerFromObjectTo(obj, obj);
    EXPECT_NOT_NULL(res);
    if (res) {
        EXPECT_STR_EQ("", res);
        free(res); // match allocation from cJSONUtils_strdup / allocator
    }
    if (obj) cJSON_Delete(obj);
}

// Test 3: Target found as first element of an array -> path should be "/0"
void test_target_in_array_first() {
    cJSON *array = cJSON_CreateArray();
    cJSON *target = cJSON_CreateNumber(123); // target node
    // Insert target as the first element
    cJSON_AddItemToArray(array, target);

    // Run the function
    char *path = cJSONUtils_FindPointerFromObjectTo(array, target);
    EXPECT_NOT_NULL(path);
    if (path) {
        EXPECT_STR_EQ("/0", path);
        free(path);
    }

    if (array) cJSON_Delete(array); // cleans up target via cJSON_Delete
    // Note: target was added to the array; deleting array frees children
}

// Test 4: Target found as second element of an array -> path should be "/1"
void test_target_in_array_second() {
    cJSON *array = cJSON_CreateArray();
    cJSON *first = cJSON_CreateNumber(42);
    cJSON *target = cJSON_CreateObject();
    // Build: [ first, target ]
    cJSON_AddItemToArray(array, first);
    cJSON_AddItemToArray(array, target);

    char *path = cJSONUtils_FindPointerFromObjectTo(array, target);
    EXPECT_NOT_NULL(path);
    if (path) {
        EXPECT_STR_EQ("/1", path);
        free(path);
    }

    if (array) cJSON_Delete(array);
}

// Test 5: Target located inside an object container -> path should start with '/' and contain encoded key
// We can't predict exact encoding, but we can validate container type and a leading '/'.
void test_target_in_object_container() {
    cJSON *root = cJSON_CreateObject();
    cJSON *target = cJSON_CreateObject();
    // Add target under key "foo"
    cJSON_AddItemToObject(root, "foo", target);

    char *path = cJSONUtils_FindPointerFromObjectTo(root, target);
    EXPECT_NOT_NULL(path);
    if (path) {
        // Basic structural check: path should begin with '/'
        EXPECT_STR_PREFIX("/", path);
        free(path);
    }

    if (root) cJSON_Delete(root);
}

// Test 6: Target not found -> NULL is returned
void test_target_not_found() {
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    // Do not insert 'child' into 'root'
    char *path = cJSONUtils_FindPointerFromObjectTo(root, child);
    EXPECT_NULL(path);
    if (path) free(path);

    if (root) cJSON_Delete(root);
    if (child) cJSON_Delete(child);
}

// Entry point for tests
int main() {
    std::cout << "Starting cJSONUtils_FindPointerFromObjectTo unit tests (C++11, no GTest)" << std::endl;

    test_null_checks();
    test_object_equals_target_base_case();
    test_target_in_array_first();
    test_target_in_array_second();
    test_target_in_object_container();
    test_target_not_found();

    std::cout << "Tests completed. Ran: " << g_total_tests << "  "
              << "Failed: " << g_failed_tests << std::endl;

    // Optionally return non-zero if tests failed
    return (g_failed_tests == 0) ? 0 : 1;
}