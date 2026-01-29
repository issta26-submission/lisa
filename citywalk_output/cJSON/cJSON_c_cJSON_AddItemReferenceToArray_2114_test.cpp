// Candidate Keywords extracted from the focal method and its dependencies
// - cJSON_AddItemReferenceToArray
// - array (cJSON array to receive a reference item)
// - item (the cJSON item to reference)
// - create_reference (static helper creating a reference wrapper for 'item')
// - global_hooks (static/global configuration for memory handling)
// - add_item_to_array (static helper to append an item to an array)
// - NULL check branch (early return false when array is NULL)

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


// Lightweight test harness (non-terminating assertions) suitable for C/C++11
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        std::cerr << "Test failed: " << (msg) \
                  << " [Line " << __LINE__ << "]" << std::endl; \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if (!((a) == (b))) { \
        std::cerr << "Test failed: " << (msg) \
                  << " (expected: " << (b) << ", actual: " << (a) << ")" \
                  << " [Line " << __LINE__ << "]" << std::endl; \
        ++g_failed_tests; \
    } \
} while (0)

static void test_AddItemReferenceToArray_NullArray_ReturnsFalse()
{
    // Test: When the provided array is NULL, cJSON_AddItemReferenceToArray
    // should return false (0). This covers the true/false branch of the NULL check.
    // Domain relevance: validate guard clause behavior.
    cJSON *item = cJSON_CreateNumber(1);
    EXPECT_TRUE(item != NULL, "Item creation should succeed");

    int result = cJSON_AddItemReferenceToArray(NULL, item);
    EXPECT_FALSE(result, "cJSON_AddItemReferenceToArray should return false when array is NULL");

    // Cleanup
    cJSON_Delete(item);
}

static void test_AddItemReferenceToArray_ValidArray_ReturnsTrue_and_SizeOne()
{
    // Test: When a valid array and a valid item are provided, the function should
    // return true and the array size should increase to 1.
    // Additionally, verify that the operation doesn't crash and basic array semantics hold.
    cJSON *array = cJSON_CreateArray();
    cJSON *item = cJSON_CreateNumber(42);

    EXPECT_TRUE(array != NULL, "Array creation should succeed");
    EXPECT_TRUE(item != NULL, "Item creation should succeed");

    int result = cJSON_AddItemReferenceToArray(array, item);
    EXPECT_TRUE(result == 1, "cJSON_AddItemReferenceToArray should return true for valid inputs");

    int size = cJSON_GetArraySize(array);
    EXPECT_EQ(size, 1, "Array size should be 1 after adding a reference item");

    // Cleanup: delete both the array and the original item.
    // The array holds a reference wrapper; freeing the array should not delete the original item.
    cJSON_Delete(array);
    cJSON_Delete(item);
}

static void test_AddItemReferenceToArray_AdditionalCoverage_DeleteSequence()
{
    // Additional coverage scenario:
    //  - Ensure that after a successful add, the array can be deleted without crashing
    //    and the original item can still be cleaned up independently.
    cJSON *array = cJSON_CreateArray();
    cJSON *item = cJSON_CreateString("hello");

    EXPECT_TRUE(array != NULL, "Array creation should succeed");
    EXPECT_TRUE(item != NULL, "Item creation should succeed");

    int result = cJSON_AddItemReferenceToArray(array, item);
    EXPECT_TRUE(result == 1, "cJSON_AddItemReferenceToArray should return true for valid inputs");

    int size = cJSON_GetArraySize(array);
    EXPECT_EQ(size, 1, "Array size should be 1 after adding a reference item");

    // Delete array first to ensure it handles internal references gracefully
    cJSON_Delete(array);

    // Then delete the original item
    cJSON_Delete(item);
}

// Helper to print a succinct summary
static void print_summary()
{
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
}

int main()
{
    // Run tests
    test_AddItemReferenceToArray_NullArray_ReturnsFalse();
    test_AddItemReferenceToArray_ValidArray_ReturnsTrue_and_SizeOne();
    test_AddItemReferenceToArray_AdditionalCoverage_DeleteSequence();

    // Summary
    print_summary();

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}