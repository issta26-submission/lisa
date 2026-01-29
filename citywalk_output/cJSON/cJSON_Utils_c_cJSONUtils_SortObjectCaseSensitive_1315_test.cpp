/*
Step 1 - Candidate Keywords (from focal method and dependencies)
- cJSON: core JSON object structure and traversal (cJSON_CreateObject, cJSON_AddItemToObject, cJSON_CreateString, etc.)
- cJSONUtils_SortObjectCaseSensitive: public API under test; sorts a cJSON object by keys in a case-sensitive manner.
- sort_object: internal sorting routine invoked by the public API with case_sensitive = true
- cJSON object item structure: object->child linked list of items; each item has a key (item->string) and a value (item->child for nested, valuestring for string payload)
- Case sensitivity handling: actual comparison logic exercised via different key cases (e.g., Apple vs apple)
- Public API exposure via C linkage (CJSON_PUBLIC-like macro in the C file)
- Test objective: ensure SortObjectCaseSensitive sorts top-level object keys in ASCII order with case sensitivity, handles empty/mixed-case scenarios, and preserves values

Notes: We will write a light C++11 test harness (no GTest) that links against the existing C code. We create objects with predetermined key orders, invoke cJSONUtils_SortObjectCaseSensitive, and verify the new key order matches expectations. We will not call private/static helpers directly; we will exercise via the public API.
*/

#include <cstring>
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
#include <cJSON_Utils.h>
#include <limits.h>


// Bring C headers into C++ with C linkage
extern "C" {
}

// Simple non-terminating assertion helper (prints but does not exit)
static bool g_current_test_passed = true;
#define EXPECT(cond) do { if(!(cond)) { \
    std::cerr << "Expectation failed: " #cond << " in " __FILE__ ":" << __LINE__ << "\n"; \
    g_current_test_passed = false; \
} } while(0)

// Helper to extract the top-level object keys in iteration order
static std::vector<std::string> get_object_keys(cJSON* object) {
    std::vector<std::string> keys;
    if (!object) return keys;
    for (cJSON* item = object->child; item != nullptr; item = item->next) {
        if (item->string) {
            keys.emplace_back(item->string);
        }
    }
    return keys;
}

// Helper to create a simple string-valued key-value pair in an object
static void add_string_pair(cJSON* object, const char* key, const char* value) {
    cJSON* item = cJSON_CreateString(value);
    cJSON_AddItemToObject(object, key, item);
}

// Test 1: Empty object should remain empty after sort
static bool test_sort_empty_object() {
    g_current_test_passed = true;

    cJSON* obj = cJSON_CreateObject();
    // Perform sort (should be no-op for empty object)
    cJSONUtils_SortObjectCaseSensitive(obj);

    auto keys = get_object_keys(obj);
    EXPECT(keys.empty());

    cJSON_Delete(obj);
    return g_current_test_passed;
}

// Test 2: Single-key object should remain with that single key
static bool test_sort_single_key() {
    g_current_test_passed = true;

    cJSON* obj = cJSON_CreateObject();
    add_string_pair(obj, "alpha", "value1");

    cJSONUtils_SortObjectCaseSensitive(obj);

    auto keys = get_object_keys(obj);
    std::vector<std::string> expected = { "alpha" };
    EXPECT(keys == expected);

    cJSON_Delete(obj);
    return g_current_test_passed;
}

// Test 3: Basic multi-key sort with mixed-case to exercise ASCII and case-sensitive ordering
// Keys inserted in a random order; expected order respects ASCII and case-sensitivity:
// "Banana" (B) < "Cherry" (C) < "apple" (a) < "delta" (d)
static bool test_sort_multi_keys_basic_case_sensitive() {
    g_current_test_passed = true;

    cJSON* obj = cJSON_CreateObject();
    add_string_pair(obj, "apple", "v1");
    add_string_pair(obj, "Banana", "v2");
    add_string_pair(obj, "delta", "v4");
    add_string_pair(obj, "Cherry", "v3");

    cJSONUtils_SortObjectCaseSensitive(obj);

    auto keys = get_object_keys(obj);
    std::vector<std::string> expected = { "Banana", "Cherry", "apple", "delta" };
    EXPECT(keys == expected);

    cJSON_Delete(obj);
    return g_current_test_passed;
}

// Test 4: Ensure case-sensitive distinction is exercised by including Apple vs apple
// Expected order: "Apple" < "Banana" < "Cherry" < "apple" < "delta"
static bool test_sort_case_sensitive_with_mixed_case() {
    g_current_test_passed = true;

    cJSON* obj = cJSON_CreateObject();
    add_string_pair(obj, "apple", "v1");
    add_string_pair(obj, "Apple", "v2");
    add_string_pair(obj, "Banana", "v3");
    add_string_pair(obj, "Cherry", "v4");
    add_string_pair(obj, "delta", "v5");

    cJSONUtils_SortObjectCaseSensitive(obj);

    auto keys = get_object_keys(obj);
    std::vector<std::string> expected = { "Apple", "Banana", "Cherry", "apple", "delta" };
    EXPECT(keys == expected);

    cJSON_Delete(obj);
    return g_current_test_passed;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running cJSONUtils_SortObjectCaseSensitive test suite (C/C++11, no GTest)..." << std::endl;

    // Run Test 1
    total++;
    if (test_sort_empty_object()) {
        passed++;
        std::cout << "[PASS] test_sort_empty_object" << std::endl;
    } else {
        std::cout << "[FAIL] test_sort_empty_object" << std::endl;
    }

    // Run Test 2
    total++;
    if (test_sort_single_key()) {
        passed++;
        std::cout << "[PASS] test_sort_single_key" << std::endl;
    } else {
        std::cout << "[FAIL] test_sort_single_key" << std::endl;
    }

    // Run Test 3
    total++;
    if (test_sort_multi_keys_basic_case_sensitive()) { // Note: function name aligns with description
        passed++;
        std::cout << "[PASS] test_sort_multi_keys_basic_case_sensitive" << std::endl;
    } else {
        std::cout << "[FAIL] test_sort_multi_keys_basic_case_sensitive" << std::endl;
    }

    // Run Test 4
    total++;
    if (test_sort_case_sensitive_with_mixed_case()) {
        passed++;
        std::cout << "[PASS] test_sort_case_sensitive_with_mixed_case" << std::endl;
    } else {
        std::cout << "[FAIL] test_sort_case_sensitive_with_mixed_case" << std::endl;
    }

    std::cout << "Test results: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}

// Note: The test suite focuses on the public API cJSONUtils_SortObjectCaseSensitive by constructing cJSON objects,
// invoking the sort, and validating the resulting key order. It covers empty, single-key, multiple-keys with
// mixed case, and explicit mixed-case scenarios to exercise the case-sensitive comparison logic.