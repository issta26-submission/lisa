// Unit test suite for cJSON_DeleteItemFromArray (CJSON public API)
// Target: ensure correct behavior when deleting items from a cJSON array.
// This test suite is written in C++11 (no Google Test) and relies on the
// provided cJSON library (cJSON.h/.c). The tests exercise true/false branches
// by creating arrays of numbers/strings, performing deletions, and asserting
// structural invariants without terminating on first failure.

// Candidate Keywords extracted from focal method and dependencies:
// cJSON_DeleteItemFromArray, cJSON_DetachItemFromArray, cJSON_Delete, cJSON_GetArraySize,
// cJSON_GetArrayItem, cJSON_CreateArray, cJSON_AddItemToArray, cJSON_CreateNumber,
// cJSON_CreateString, cJSON_GetNumberValue, cJSON_GetStringValue, cJSON_DeleteItemFromArray
// These are used to form test scenarios that cover typical and boundary cases.

#include <cmath>
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


extern "C" {
}

// Simple non-terminating assertion framework (prints on failure, continues execution)
static int g_pass = 0;
static int g_fail = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (File " << __FILE__ << ", Line " << __LINE__ << ")" << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

#define ASSERT_EQ_I(actual, expected, msg) do { \
    if(!((actual) == (expected))) { \
        std::cerr << "[FAIL] " << (msg) << " Expected: " << (expected) << " Actual: " << (actual) \
                  << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

#define ASSERT_STR_EQ(actual, expected, msg) do { \
    if(((actual) == nullptr) || (std::strcmp((actual), (expected)) != 0)) { \
        std::cerr << "[FAIL] " << (msg) << " Expected: \"" << (expected) \
                  << "\" Actual: \"" << (actual ? actual : "NULL") << "\" (Line " << __LINE__ << ")" \
                  << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

#define ASSERT_DOUBLE_EQ(actual, expected, msg) do { \
    if(std::fabs((double)(actual) - (double)(expected)) > 1e-9) { \
        std::cerr << "[FAIL] " << (msg) << " Expected: " << (expected) \
                  << " Actual: " << (actual) << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

// Helper to cleanly free a cJSON array (and all its items)
static void FreeArray(cJSON *array) {
    if (array) {
        cJSON_Delete(array);
    }
}

// Test 1: Delete middle item from a 2-element numeric array -> remaining element should be the second value
static void test_DeleteItemFromArray_MiddleTwoNumbers() {
    cJSON *array = cJSON_CreateArray();
    cJSON *first = cJSON_CreateNumber(1.0);
    cJSON *second = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(array, first);
    cJSON_AddItemToArray(array, second);

    // Action under test
    CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array_, int which);
    cJSON_DeleteItemFromArray(array, 0);

    // Assertions
    int size = cJSON_GetArraySize(array);
    ASSERT_EQ_I(size, 1, "Array size should be 1 after deleting first element in 2-element numeric array");

    cJSON *remaining = cJSON_GetArrayItem(array, 0);
    double val = cJSON_GetNumberValue(remaining);
    ASSERT_DOUBLE_EQ(val, 2.0, "Remaining value after deletion should be 2.0");

    FreeArray(array);
}

// Test 2: Delete last item from a 3-element numeric array -> remaining order preserved
static void test_DeleteItemFromArray_LastThreeNumbers() {
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(30.0));

    cJSON_DeleteItemFromArray(array, 2); // delete the last element (30)

    int size = cJSON_GetArraySize(array);
    ASSERT_EQ_I(size, 2, "Array size should be 2 after deleting last element from 3-element numeric array");

    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    ASSERT_DOUBLE_EQ(cJSON_GetNumberValue(first), 10.0, "First item should be 10.0");
    ASSERT_DOUBLE_EQ(cJSON_GetNumberValue(second), 20.0, "Second item should be 20.0");

    FreeArray(array);
}

// Test 3: Delete with an invalid index should be a no-op (array remains unchanged)
static void test_DeleteItemFromArray_InvalidIndex_NoOp() {
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(5.5));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(6.6));

    // Invalid index
    cJSON_DeleteItemFromArray(array, -1);

    int size = cJSON_GetArraySize(array);
    ASSERT_EQ_I(size, 2, "Array size should remain 2 after invalid delete index");

    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    ASSERT_DOUBLE_EQ(cJSON_GetNumberValue(first), 5.5, "First item should remain 5.5");
    ASSERT_DOUBLE_EQ(cJSON_GetNumberValue(second), 6.6, "Second item should remain 6.6");

    FreeArray(array);
}

// Test 4: Delete item from array of strings and verify remaining ordering/content
static void test_DeleteItemFromArray_Strings() {
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(array, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(array, cJSON_CreateString("gamma"));

    cJSON_DeleteItemFromArray(array, 1); // remove "beta"

    int size = cJSON_GetArraySize(array);
    ASSERT_EQ_I(size, 2, "Array size should be 2 after deleting 'beta' from string array");

    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);

    ASSERT_STR_EQ(cJSON_GetStringValue(first), "alpha", "First item should be 'alpha'");
    ASSERT_STR_EQ(cJSON_GetStringValue(second), "gamma", "Second item should be 'gamma'");

    FreeArray(array);
}

// Test 5: Deleting from a single-element array yields an empty array
static void test_DeleteItemFromArray_SingleElementResultsEmpty() {
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(42.0));

    cJSON_DeleteItemFromArray(array, 0);

    int size = cJSON_GetArraySize(array);
    ASSERT_EQ_I(size, 0, "Array should be empty after deleting the only element");

    FreeArray(array);
}

// Main test runner
int main() {
    // Disable iostream synchronization for faster tests (optional)
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Run tests
    test_DeleteItemFromArray_MiddleTwoNumbers();
    test_DeleteItemFromArray_LastThreeNumbers();
    test_DeleteItemFromArray_InvalidIndex_NoOp();
    test_DeleteItemFromArray_Strings();
    test_DeleteItemFromArray_SingleElementResultsEmpty();

    // Summary
    std::cout << "Test Summary: Passed " << g_pass << ", Failed " << g_fail << std::endl;
    // Return non-zero if any test failed
    return (g_fail > 0) ? 1 : 0;
}