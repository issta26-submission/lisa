/*
Step 1 & 2: Understanding and Candidate Keywords
Candidate Keywords representing the core dependent components of the focal method:
- cJSON_GetObjectItemCaseSensitive
- cJSON_DetachItemViaPointer
- cJSON_DetachItemFromObjectCaseSensitive (focal method under test)
- cJSON_CreateObject
- cJSON_AddItemToObject
- cJSON_CreateString
- cJSON_GetStringValue
- cJSON_IsString / cJSON_GetObjectItemCaseSensitive (used for verification)
- cJSON_Delete / cJSON_Duplicate (memory management utilities)
- cJSON_PUBLIC (visibility macro, not directly used in tests)

This test suite focuses on cJSON_DetachItemFromObjectCaseSensitive behavior by constructing objects with items, detaching by exact case-sensitive key, and verifying object state and returned item integrity.

Step 3: Tests are implemented in C++11 with a small custom test harness (no GTest) and extern "C" integration for the C library (cJSON).
*/

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


// Ensure C linkage for the C library
extern "C" {
}

// Simple non-terminating test harness
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, desc) do { \
    if (cond) { \
        ++g_pass; \
    } else { \
        ++g_fail; \
        std::cerr << "[FAILED] " << __FUNCTION__ << ": " << desc << std::endl; \
    } \
} while (0)

// Test 1: Detach an existing item by case-sensitive key; ensure object no longer contains the key
void test_detach_existing_item() {
    // Create root object and add an item with key "name" and value "value"
    cJSON *object = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("value");
    cJSON_AddItemToObject(object, "name", item);

    // Act: detach by exact key (case-sensitive)
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(object, "name");

    // Assert: returned item is non-NULL and has correct value
    CHECK(detached != NULL, "Detached item should be non-NULL");
    if (detached) {
        const char* s = cJSON_GetStringValue(detached);
        CHECK(s != NULL && std::strcmp(s, "value") == 0, "Detached item should have value 'value'");
    }

    // Assert: object should no longer contain the key "name"
    cJSON *lookup_after = cJSON_GetObjectItemCaseSensitive(object, "name");
    CHECK(lookup_after == NULL, "Object should not contain key 'name' after detachment");

    // Cleanup
    if (detached) {
        cJSON_Delete(detached);
    }
    if (object) {
        cJSON_Delete(object);
    }
}

// Test 2: Detach non-existing item should return NULL and not alter the object
void test_detach_nonexisting_item() {
    // Create root object with a single existing item
    cJSON *object = cJSON_CreateObject();
    cJSON *existing = cJSON_CreateString("val");
    cJSON_AddItemToObject(object, "existing", existing);

    // Act: attempt to detach a non-existing key
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(object, "missing");

    // Assert: NULL returned and existing item remains
    CHECK(detached == NULL, "Detaching non-existing item should return NULL");
    cJSON *still_there = cJSON_GetObjectItemCaseSensitive(object, "existing");
    CHECK(still_there != NULL, "Existing item should remain in object");
    if (still_there) {
        const char* s = cJSON_GetStringValue(still_there);
        CHECK(s != NULL && std::strcmp(s, "val") == 0, "Existing item should still have value 'val'");
    }

    // Cleanup
    if (detached) {
        cJSON_Delete(detached); // Should be NULL; safe
    }
    if (object) {
        cJSON_Delete(object);
    }
}

// Test 3: Ensure case-sensitive behavior with multiple items; detach one and verify others remain
void test_detach_case_sensitive_multiple_items() {
    // Create object with two keys: "One" and "name" (lowercase)
    cJSON *object = cJSON_CreateObject();
    cJSON *one = cJSON_CreateString("A");
    cJSON *name = cJSON_CreateString("B");
    cJSON_AddItemToObject(object, "One", one);
    cJSON_AddItemToObject(object, "name", name);

    // Act: detach the key "name" (lowercase)
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(object, "name");

    // Assert: detached item is correct
    CHECK(detached != NULL, "Detached item for key 'name' should be non-NULL");
    if (detached) {
        const char* s = cJSON_GetStringValue(detached);
        CHECK(s != NULL && std::strcmp(s, "B") == 0, "Detached item should have value 'B'");
    }

    // Assert: object should still contain key "One" and not contain "name"
    cJSON *lookup_one = cJSON_GetObjectItemCaseSensitive(object, "One");
    CHECK(lookup_one != NULL, "Object should still contain key 'One' after detaching 'name'");
    if (lookup_one) {
        const char* s = cJSON_GetStringValue(lookup_one);
        CHECK(s != NULL && std::strcmp(s, "A") == 0, "Value of 'One' should be 'A'");
    }

    cJSON *lookup_name = cJSON_GetObjectItemCaseSensitive(object, "name");
    CHECK(lookup_name == NULL, "Object should no longer contain key 'name' after detachment");

    // Cleanup
    if (detached) {
        cJSON_Delete(detached);
    }
    if (object) {
        cJSON_Delete(object);
    }
}

// Test 4: Verify case sensitivity by having similar keys with different cases
void test_detach_case_sensitivity_false_positive() {
    // Object contains "Name" (capital N)
    cJSON *object = cJSON_CreateObject();
    cJSON *name_cap = cJSON_CreateString("C");
    cJSON_AddItemToObject(object, "Name", name_cap);

    // Attempt to detach with lowercase "name" (should not find)
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(object, "name");

    // Assert: NULL and original item remains
    CHECK(detached == NULL, "Detaching with incorrect case should return NULL due to case sensitivity");
    cJSON *lookup_Name = cJSON_GetObjectItemCaseSensitive(object, "Name");
    CHECK(lookup_Name != NULL, "Original item with key 'Name' should remain");
    if (lookup_Name) {
        const char* s = cJSON_GetStringValue(lookup_Name);
        CHECK(s != NULL && std::strcmp(s, "C") == 0, "Value of 'Name' should be 'C'");
    }

    // Cleanup
    if (detached) {
        cJSON_Delete(detached);
    }
    if (object) {
        cJSON_Delete(object);
    }
}

int main() {
    // Run all tests
    test_detach_existing_item();
    test_detach_nonexisting_item();
    test_detach_case_sensitive_multiple_items();
    test_detach_case_sensitivity_false_positive();

    // Summary
    std::cout << "Test Summary: Passed " << g_pass << ", Failed " << g_fail << ", Total " << (g_pass + g_fail) << std::endl;
    if (g_fail > 0) {
        return 1;
    }
    return 0;
}