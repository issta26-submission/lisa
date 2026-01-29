// Test suite for cJSONUtils_SortObject (CJSON_UTILS) using C++11, without GoogleTest.
// This file creates a minimal test harness to exercise the focal method
// cJSONUtils_SortObject and its related wrapper cJSONUtils_SortObjectCaseSensitive.
// The tests are designed to be executable in a C++11 environment and link against
// the C implementations of cJSON and cJSON_Utils (via included headers).

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


#ifdef __cplusplus
extern "C" {
}
#else
#endif

// Simple non-terminating test assertion macro.
// It prints a message and records a failure but does not abort the test prematurely.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_tests_failed; \
    } \
} while(0)

// Helper: verify that object keys (top-level, in order) equal the expected sequence.
// Returns true if matches exactly, including length.
static bool object_keys_in_expected_order(const cJSON* object, const std::vector<std::string>& expected_order)
{
    if (!object) return false;
    const cJSON* item = object->child;
    for (size_t i = 0; i < expected_order.size(); ++i) {
        if (!item) return false;
        if (!item->string) return false;
        if (std::strcmp(item->string, expected_order[i].c_str()) != 0) return false;
        item = item->next;
    }
    // Ensure there are no extra items after the expected ones
    if (item != nullptr) return false;
    return true;
}

// Candidate Keywords mapping (for Step 1): cJSONUtils_SortObject, sort_object,
// cJSON, object, case_sensitive, false, cJSONUtils_SortObjectCaseSensitive.

//
// Test 1: Basic sorting of a simple object
// - Build an object with keys in unsorted order: "b", "a", "d", "c".
// - Call cJSONUtils_SortObject to sort (case_sensitive = false).
// - Expect the top-level keys to be in lexical order: "a", "b", "c", "d".
// This covers the non-case-sensitive path and validates that the public API reorders keys correctly.
//
static void test_sort_object_basic_order()
{
    // Create object and add unsorted keys
    cJSON* obj = cJSON_CreateObject();
    TEST_ASSERT(obj != nullptr, "Failed to create cJSON object for Test 1");

    TEST_ASSERT(cJSON_AddItemToObject(obj, "b", cJSON_CreateNumber(1)) != nullptr,
                "Failed to add key 'b' to object in Test 1");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "a", cJSON_CreateNumber(2)) != nullptr,
                "Failed to add key 'a' to object in Test 1");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "d", cJSON_CreateNumber(3)) != nullptr,
                "Failed to add key 'd' to object in Test 1");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "c", cJSON_CreateNumber(4)) !=nullptr,
                "Failed to add key 'c' to object in Test 1");

    // Act: sort the object (non-case-sensitive wrapper)
    cJSONUtils_SortObject(obj);

    // Assert: keys are ordered as {"a","b","c","d"}
    std::vector<std::string> expected = {"a","b","c","d"};
    bool ok = object_keys_in_expected_order(obj, expected);
    TEST_ASSERT(ok, "Test 1 failed: object keys are not in lexicographic order after SortObject");

    // Cleanup
    cJSON_Delete(obj);
}

// Test 2: Idempotence when object is already sorted (basic case, all lowercase keys)
// - Build an object with keys already sorted: "a","b","c".
// - Sort and ensure order remains unchanged.
static void test_sort_object_idempotence_basic_sorted()
{
    cJSON* obj = cJSON_CreateObject();
    TEST_ASSERT(obj != nullptr, "Failed to create cJSON object for Test 2");

    TEST_ASSERT(cJSON_AddItemToObject(obj, "a", cJSON_CreateNumber(1)) != nullptr,
                "Failed to add key 'a' to object in Test 2");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "b", cJSON_CreateNumber(2)) != nullptr,
                "Failed to add key 'b' to object in Test 2");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "c", cJSON_CreateNumber(3)) != nullptr,
                "Failed to add key 'c' to object in Test 2");

    // Act: sort
    cJSONUtils_SortObject(obj);

    // Assert: order remains "a","b","c"
    std::vector<std::string> expected = {"a","b","c"};
    bool ok = object_keys_in_expected_order(obj, expected);
    TEST_ASSERT(ok, "Test 2 failed: object order changed after sorting already-sorted object");

    cJSON_Delete(obj);
}

// Test 3: Null input safety
// - Ensure that calling SortObject with a null pointer does not crash.
// - Also verify the CaseSensitive variant does not crash on null input.
static void test_sort_object_null_safety()
{
    // Act: should handle null gracefully
    cJSONUtils_SortObject(nullptr);
    cJSONUtils_SortObjectCaseSensitive(nullptr);

    // If we reach here, null handling is assumed non-crashing (no assertion needed beyond not crashing).
    TEST_ASSERT(true, "Test 3: null input handled without crash");
}

// Test 4: Case-sensitive wrapper behavior with lowercase-only keys
// - Build an object with lowercase keys to ensure both SortObject and SortObjectCaseSensitive
//   produce the same deterministic order when all keys are lowercase.
// - Expect lexicographic order "a","b","c".
static void test_sort_object_case_sensitive_wrapper_with_lowercase_keys()
{
    cJSON* obj = cJSON_CreateObject();
    TEST_ASSERT(obj != nullptr, "Failed to create cJSON object for Test 4");

    TEST_ASSERT(cJSON_AddItemToObject(obj, "b", cJSON_CreateNumber(1)) != nullptr,
                "Failed to add key 'b' to object in Test 4");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "a", cJSON_CreateNumber(2)) != nullptr,
                "Failed to add key 'a' to object in Test 4");
    TEST_ASSERT(cJSON_AddItemToObject(obj, "c", cJSON_CreateNumber(3)) != nullptr,
                "Failed to add key 'c' to object in Test 4");

    // Act: sort using the case-sensitive wrapper
    cJSONUtils_SortObjectCaseSensitive(obj);

    // Assert: order should be "a","b","c" regardless of case-sensitivity in this specific lowercase-only scenario
    std::vector<std::string> expected = {"a","b","c"};
    bool ok = object_keys_in_expected_order(obj, expected);
    TEST_ASSERT(ok, "Test 4 failed: CaseSensitive sort with lowercase keys did not produce expected order");

    cJSON_Delete(obj);
}

// Test 5: Ensure stability of nested values (top-level sort only affects key order, not inner values)
// - Build an object with nested object as the value for keys to ensure that sorting top-level keys
//   does not corrupt or modify nested structures.
static void test_sort_object_top_level_sort_only_affects_key_order()
{
    // Outer object with nested values
    cJSON* inner1 = cJSON_CreateObject();
    cJSON_AddItemToObject(inner1, "inner_key1", cJSON_CreateNumber(100));

    cJSON* inner2 = cJSON_CreateObject();
    cJSON_AddItemToObject(inner2, "inner_key2", cJSON_CreateString("val"));

    cJSON* outer = cJSON_CreateObject();
    TEST_ASSERT(cJSON_AddItemToObject(outer, "b", inner1) != nullptr,
                "Failed to add key 'b' with nested object to outer in Test 5");
    TEST_ASSERT(cJSON_AddItemToObject(outer, "a", inner2) != nullptr,
                "Failed to add key 'a' with nested object to outer in Test 5");
    TEST_ASSERT(cJSON_AddItemToObject(outer, "c", cJSON_CreateNumber(3)) != nullptr,
                "Failed to add key 'c' to outer in Test 5");

    // Capture pointers to nested values for later verification
    cJSON* nested_b = NULL;
    cJSON* nested_a = NULL;
    // After building, perform sort
    cJSONUtils_SortObject(outer);

    // Find reordered items and verify nested pointers remained intact
    for (cJSON* item = outer->child; item != nullptr; item = item->next) {
        if (std::strcmp(item->string, "a") == 0) nested_a = item->child;
        if (std::strcmp(item->string, "b") == 0) nested_b = item->child;
    }

    TEST_ASSERT(nested_a != NULL && nested_b != NULL, "Test 5: Failed to locate nested objects after sort");

    // Basic sanity: the nested objects should still contain their original keys
    TEST_ASSERT(cJSON_GetObjectItemCaseSensitive(nested_a, "inner_key2") != nullptr,
                "Test 5: Nested object under 'a' lost its contents after sort");
    TEST_ASSERT(cJSON_GetObjectItemCaseSensitive(nested_b, "inner_key1") != nullptr,
                "Test 5: Nested object under 'b' lost its contents after sort");

    cJSON_Delete(outer);
}

// Entry point: run all tests and report result.
int main()
{
    std::cout << "Running tests for cJSONUtils_SortObject (CJSON Utils)..." << std::endl;

    test_sort_object_basic_order();
    test_sort_object_idempotence_basic_sorted();
    test_sort_object_null_safety();
    test_sort_object_case_sensitive_wrapper_with_lowercase_keys();
    test_sort_object_top_level_sort_only_affects_key_order();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

    if (g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
}