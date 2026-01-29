/*
  Lightweight C++11 test suite for cJSON_GetArrayItem in cJSON.c

  - Uses only standard C++11 facilities and the provided C API from cJSON.
  - No Google Test or other frameworks; tests are invoked from main().
  - Each test is self-contained and uses non-terminating assertions to maximize coverage.
  - Tests rely on public API only (no private/static helpers accessed).
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
#include <limits.h>


// Include C header within C++ translation unit
extern "C" {
}

static int g_total = 0;
static int g_failed = 0;

// Lightweight assertion helpers that do not abort on failure
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    ++g_total; \
    if ((void*)(a) != (void*)(b)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_NULL(p, msg) do { \
    ++g_total; \
    if ((p) != nullptr) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Test 1: Negative index must return NULL
// This exercises the early guard: if (index < 0) return NULL;
void test_GetArrayItem_NegativeIndex() {
    // Arrange: create a small array with one element
    cJSON *array = cJSON_CreateArray();
    cJSON *elem = cJSON_CreateNumber(42);
    cJSON_AddItemToArray(array, elem);

    // Act: request a negative index
    cJSON *result = cJSON_GetArrayItem(array, -1);

    // Assert: result should be NULL
    EXPECT_NULL(result, "GetArrayItem with negative index should return NULL");

    // Cleanup
    cJSON_Delete(array);
}

// Test 2: Basic retrieval by index identity
// The returned pointer should be identical to the actual array child pointer.
void test_GetArrayItem_BasicIdentity() {
    cJSON *array = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, item0);

    cJSON *result = cJSON_GetArrayItem(array, 0);

    EXPECT_PTR_EQ(result, item0, "First element pointer should be identical to original item");
    cJSON_Delete(array);
}

// Test 3: Mixed types in array and correct index resolution
void test_GetArrayItem_MixedTypes() {
    cJSON *array = cJSON_CreateArray();

    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *obj = cJSON_CreateObject(); // empty object

    cJSON_AddItemToArray(array, num);
    cJSON_AddItemToArray(array, str);
    cJSON_AddItemToArray(array, obj);

    // Validate each index returns the exact same item pointer
    EXPECT_PTR_EQ(cJSON_GetArrayItem(array, 0), num, "Index 0 should be the number item");
    EXPECT_PTR_EQ(cJSON_GetArrayItem(array, 1), str, "Index 1 should be the string item");
    EXPECT_PTR_EQ(cJSON_GetArrayItem(array, 2), obj, "Index 2 should be the object item");

    cJSON_Delete(array);
}

// Test 4: Out-of-bounds positive index returns NULL
void test_GetArrayItem_OutOfBounds() {
    cJSON *array = cJSON_CreateArray();
    cJSON *one = cJSON_CreateNumber(1);
    cJSON_AddItemToArray(array, one);

    // Index beyond the size should yield NULL
    cJSON *result = cJSON_GetArrayItem(array, 5);
    EXPECT_NULL(result, "Out-of-bounds index should return NULL");

    cJSON_Delete(array);
}

// Test 5: Empty array returns NULL for any non-negative index
void test_GetArrayItem_EmptyArray() {
    cJSON *array = cJSON_CreateArray();

    cJSON *result0 = cJSON_GetArrayItem(array, 0);
    EXPECT_NULL(result0, "Empty array: index 0 should return NULL");

    cJSON_Delete(array);
}

// Entry point for all tests
int main() {
    std::cout << "Starting tests for cJSON_GetArrayItem...\n";

    test_GetArrayItem_NegativeIndex();
    test_GetArrayItem_BasicIdentity();
    test_GetArrayItem_MixedTypes();
    test_GetArrayItem_OutOfBounds();
    test_GetArrayItem_EmptyArray();

    std::cout << "Tests completed. Total: " << g_total << ", Failed: " << g_failed << "\n";

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}