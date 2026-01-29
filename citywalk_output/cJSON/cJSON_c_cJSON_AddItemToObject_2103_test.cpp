// Unit test suite for cJSON_AddItemToObject in cJSON.c
// Target: CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
// Approach: Use a small, self-contained test harness (no GTest) in C++11 that links against the C implementation.
// Notes:
// - Tests are written to be robust regarding ownership/memory: the object takes ownership of the added item.
// - We exercise true/false branches where sensible (valid inputs, NULL inputs, existing key replacement).
// - Static helpers within cJSON.c are not directly tested; we interact via the public API.
// - All tests use a lightweight CHECK macro that records failures without terminating the test run.

#include <sstream>
#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Ensure C linkage for the C library header
extern "C" {
}

// Global test failure collector
static std::vector<std::string> gFailures;

// Lightweight assertion helper (non-terminating)
#define CHECK(cond, msg) \
    do { \
        if(!(cond)) { \
            std::ostringstream oss; \
            oss << "Failure: " << (msg) << " in " << __FUNCTION__ << " (line " << __LINE__ << ")"; \
            gFailures.push_back(oss.str()); \
        } \
    } while(false)

// Test 1: Basic success path: add a string item to a valid object and verify retrieval
static void test_AddItemToObject_BasicSuccess() {
    // Create a JSON object
    cJSON *object = cJSON_CreateObject();
    // Create an item to add
    cJSON *item = cJSON_CreateString("value");
    // Add to object
    int res = cJSON_AddItemToObject(object, "key", item);
    CHECK(res == 1, "cJSON_AddItemToObject should return true (1) on success");

    // Verify the item is present under the key and has expected value
    cJSON *retrieved = cJSON_GetObjectItem(object, "key");
    CHECK(retrieved != nullptr, "Object should contain key 'key' after AddItemToObject");
    const char *str = cJSON_GetStringValue(retrieved);
    CHECK(str != nullptr && std::strcmp(str, "value") == 0,
          "Value retrieved for 'key' should be 'value'");

    // Cleanup
    cJSON_Delete(object);
    // Note: item is owned by object now; no separate delete for item
}

// Test 2: Adding to a NULL object should fail gracefully (no crash, returns false)
static void test_AddItemToObject_NullObject() {
    cJSON *item = cJSON_CreateString("val");
    int res = cJSON_AddItemToObject(nullptr, "key", item);
    CHECK(res == 0, "cJSON_AddItemToObject should return false (0) when object is NULL");
    // Item should not be added; free it manually
    cJSON_Delete(item);
}

// Test 3: Adding with a NULL key should fail gracefully
static void test_AddItemToObject_NullKey() {
    cJSON *object = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("val");
    int res = cJSON_AddItemToObject(object, nullptr, item);
    CHECK(res == 0, "cJSON_AddItemToObject should return false (0) when key is NULL");
    // Since the item was not added, it must be freed manually
    cJSON_Delete(item);
    cJSON_Delete(object);
}

// Test 4: Adding a NULL item should fail gracefully
static void test_AddItemToObject_NullItem() {
    cJSON *object = cJSON_CreateObject();
    int res = cJSON_AddItemToObject(object, "key", nullptr);
    CHECK(res == 0, "cJSON_AddItemToObject should return false (0) when item is NULL");
    cJSON_Delete(object);
}

// Test 5: Replacing an existing key should replace the value under that key
static void test_AddItemToObject_ReplaceExistingKey() {
    cJSON *object = cJSON_CreateObject();

    // First item
    cJSON *item1 = cJSON_CreateString("first");
    int r1 = cJSON_AddItemToObject(object, "dup", item1);
    CHECK(r1 == 1, "First AddItemToObject should succeed");

    // Capture the value before replacement (as a non-owning read)

    cJSON *found_before = cJSON_GetObjectItem(object, "dup");
    std::string value_before;
    if (found_before != nullptr) {
        const char *s = cJSON_GetStringValue(found_before);
        value_before = s ? s : "";
    }

    // Second item to replace the first
    cJSON *item2 = cJSON_CreateString("second");
    int r2 = cJSON_AddItemToObject(object, "dup", item2);
    CHECK(r2 == 1, "Second AddItemToObject (replace) should succeed");

    // Verify final value is "second"
    cJSON *found_after = cJSON_GetObjectItem(object, "dup");
    const char *s_after = found_after ? cJSON_GetStringValue(found_after) : nullptr;
    CHECK(s_after != nullptr && std::strcmp(s_after, "second") == 0,
          "After replacement, value for 'dup' should be 'second'");

    // Optional: confirm the old value was captured as 'first' (non-fatal if memory freed)
    if (!value_before.empty()) {
        // We compare against the captured value to ensure the old string content was indeed 'first'
        CHECK(value_before == "first", "The previous value captured should be 'first' before replacement");
    }

    // Cleanup
    cJSON_Delete(object);
}

// Runner that executes all tests
static void RunAllTests() {
    test_AddItemToObject_BasicSuccess();
    test_AddItemToObject_NullObject();
    test_AddItemToObject_NullKey();
    test_AddItemToObject_NullItem();
    test_AddItemToObject_ReplaceExistingKey();
}

int main() {
    // Run tests
    RunAllTests();

    // Report results
    if (gFailures.empty()) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << gFailures.size() << " test(s) failed:" << std::endl;
        for (const auto &msg : gFailures) {
            std::cout << "  - " << msg << std::endl;
        }
        return 1;
    }
}