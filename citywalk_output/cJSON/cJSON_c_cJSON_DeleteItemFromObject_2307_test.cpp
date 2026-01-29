/*
  Unit test suite for the focal method:
  CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string)

  Summary of the approach:
  - Create a CJSON object with known keys and values.
  - Exercise the single-path delete: delete an existing key and verify
    the key is removed and no crash occurs.
  - Exercise the non-existing-key path: attempt to delete a key that doesn't exist
    and verify that the object remains intact (no crashes, existing keys still present).
  - Verify that only the targeted item is removed when multiple items exist.

  This test suite is implemented in C++11, without any external testing framework
  (no GTest). It uses a tiny assertion harness that does not terminate on failure,
  allowing broad coverage in a single run.
  It relies on cJSON.h/cJSON.c being part of the build as a C library.
*/

/* Candidate Keywords (Step 1): cJSON_DeleteItemFromObject, cJSON_DetachItemFromObject, cJSON_CreateObject,
   cJSON_AddStringToObject, cJSON_AddNumberToObject, cJSON_GetObjectItem, cJSON_GetStringValue,
   cJSON_GetNumberValue, cJSON_Delete, memory management, object-key operations, test harness */

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

/* Lightweight test framework (non-terminating assertions)
   - Uses C++ standard library only
   - Reports failures but continues execution to maximize code coverage
*/
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_asserts; \
    if(!(cond)) { \
        ++g_failed_asserts; \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " [Line: " << __LINE__ << "]" << std::endl; \
    } \
} while(0)

#define EXPECT_NULL(ptr, msg) EXPECT_TRUE((ptr) == nullptr, (std::string("NULL check: ") + (msg)).c_str())
#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, (std::string("NOT NULL check: ") + (msg)).c_str())

/*
  Test 1: Deleting an existing key from a cJSON object should remove the key
  and leave the rest of the object intact.
*/
static void test_DeleteItemFromObject_RemovesExistingKey() {
    // Setup: create an object and add a key/value pair
    cJSON *object = cJSON_CreateObject();
    EXPECT_NOT_NULL(object, "Failed to create cJSON object for Test 1");

    // Add key "foo" with value "bar"
    EXPECT_NOT_NULL(cJSON_AddStringToObject(object, "foo", "bar"), "Failed to add string 'foo':'bar' to object");

    // Validate presence of the key before deletion
    cJSON *item_before = cJSON_GetObjectItem(object, "foo");
    EXPECT_NOT_NULL(item_before, "Pre-condition: item 'foo' should exist before deletion");
    const char *before_value = cJSON_GetStringValue(item_before);
    EXPECT_TRUE(before_value && std::string(before_value) == "bar", "Pre-condition value of 'foo' should be 'bar'");

    // Action: delete the item
    cJSON_DeleteItemFromObject(object, "foo");

    // Verify the key is removed
    cJSON *item_after = cJSON_GetObjectItem(object, "foo");
    EXPECT_NULL(item_after, "Post-condition: item 'foo' should be removed (GetObjectItem should be NULL)");

    // Cleanup: delete the object (this should be safe since 'foo' is already removed)
    cJSON_Delete(object);
}

/*
  Test 2: Deleting a non-existing key should not modify the object.
*/
static void test_DeleteItemFromObject_NonExistingKey_NoChange() {
    // Setup: create an object with two known keys
    cJSON *object = cJSON_CreateObject();
    EXPECT_NOT_NULL(object, "Failed to create cJSON object for Test 2");

    // Add two items
    EXPECT_NOT_NULL(cJSON_AddNumberToObject(object, "a", 1), "Failed to add number 'a':1");
    EXPECT_NOT_NULL(cJSON_AddStringToObject(object, "b", "two"), "Failed to add string 'b':'two'");

    // Action: attempt to delete a non-existent item
    cJSON_DeleteItemFromObject(object, "notexist");

    // Verify existing keys are still present and unchanged
    cJSON *itemA = cJSON_GetObjectItem(object, "a");
    cJSON *itemB = cJSON_GetObjectItem(object, "b");
    EXPECT_NOT_NULL(itemA, "After non-existing delete: key 'a' should still exist");
    EXPECT_NOT_NULL(itemB, "After non-existing delete: key 'b' should still exist");

    double valueA = cJSON_GetNumberValue(itemA);
    EXPECT_TRUE(valueA == 1.0, "Value of 'a' should remain 1");

    const char *valueB = cJSON_GetStringValue(itemB);
    EXPECT_TRUE(valueB && std::string(valueB) == "two", "Value of 'b' should remain 'two'");

    // Cleanup
    cJSON_Delete(object);
}

/*
  Test 3: Deleting a key should only remove that key when multiple keys exist.
*/
static void test_DeleteItemFromObject_RemovesOnlyTargetKeyAmongMany() {
    // Setup: create an object with multiple keys
    cJSON *object = cJSON_CreateObject();
    EXPECT_NOT_NULL(object, "Failed to create cJSON object for Test 3");

    EXPECT_NOT_NULL(cJSON_AddNumberToObject(object, "x", 42), "Failed to add numeric 'x':42");
    EXPECT_NOT_NULL(cJSON_AddStringToObject(object, "y", "yes"), "Failed to add string 'y':'yes'");
    EXPECT_NOT_NULL(cJSON_AddBoolToObject(object, "z", 1), "Failed to add boolean 'z':true");

    // Action: delete key 'y'
    cJSON_DeleteItemFromObject(object, "y");

    // Verify 'y' is removed
    cJSON *itemY = cJSON_GetObjectItem(object, "y");
    EXPECT_NULL(itemY, "After deletion: key 'y' should be removed");

    // Verify other keys remain
    cJSON *itemX = cJSON_GetObjectItem(object, "x");
    cJSON *itemZ = cJSON_GetObjectItem(object, "z");
    EXPECT_NOT_NULL(itemX, "Key 'x' should still exist after deleting 'y'");
    EXPECT_NOT_NULL(itemZ, "Key 'z' should still exist after deleting 'y'");

    double valueX = cJSON_GetNumberValue(itemX);
    EXPECT_TRUE(valueX == 42.0, "Value of 'x' should remain 42");

    cJSON_bool valueZ = cJSON_IsTrue(itemZ) ? 1 : 0;
    EXPECT_TRUE(valueZ == 1, "Value of 'z' should remain true");

    // Cleanup
    cJSON_Delete(object);
}

int main() {
    std::cout << "Starting tests for cJSON_DeleteItemFromObject..." << std::endl;

    test_DeleteItemFromObject_RemovesExistingKey();
    test_DeleteItemFromObject_NonExistingKey_NoChange();
    test_DeleteItemFromObject_RemovesOnlyTargetKeyAmongMany();

    std::cout << "Tests completed. Total asserts: " << g_total_asserts
              << ", Failures: " << g_failed_asserts << std::endl;

    if (g_failed_asserts != 0) {
        std::cerr << "Some tests failed. Please review the failure messages above." << std::endl;
        return 2;
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}