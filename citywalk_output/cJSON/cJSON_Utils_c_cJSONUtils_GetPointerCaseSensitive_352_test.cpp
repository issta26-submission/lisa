/*
Candidate Keywords (Step 1):
- cJSON, cJSONUtils_GetPointerCaseSensitive
- cJSONUtils_GetPointerCaseSensitive -> get_item_from_pointer
- pointer (JSON Pointer syntax), case_sensitive
- object, NULL object, nested objects, arrays
- cJSON_CreateObject, cJSON_CreateArray, cJSON_CreateString, cJSON_AddItemToObject, cJSON_AddItemToArray
- cJSON_Delete, item types (cJSON_String, etc.)
- NULL checks, path resolution, non-existent paths
- public API vs static helpers (GetPointerCaseSensitive is a wrapper)
*/

/*
Domain knowledge (Step 3) considerations implemented:
- Create valid CJSON objects and nested structures to exercise /foo/bar, /arr/1, etc.
- Test true-case sensitivity by using exact vs mismatched key names.
- Test behavior on NULL object input.
- Implement a lightweight non-terminating test harness (no aborts on failures) and aggregate results.
- Use C linkage for C headers in C++ test code.
- Avoid accessing private/static internal helpers directly; test only public API function cJSONUtils_GetPointerCaseSensitive.
*/

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON_Utils.h>
#include <limits.h>


// Ensure C headers are linked with C linkage when included from C++
extern "C" {
}

// Lightweight non-terminating test harness
static int g_test_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        ++g_test_failures; \
        std::cerr << "Test failure: " << (msg) << " [File: " << __FILE__ << ", Line: " << __LINE__ << "]\n"; \
    } \
} while(0)

// Test 1: Basic retrieval of a nested string using a proper JSON Pointer with exact case
void test_GetPointerCaseSensitive_basic() {
    // Build JSON: { "foo": { "bar": "hello" } }
    cJSON* root = cJSON_CreateObject();
    cJSON* foo = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "foo", foo);
    cJSON* bar = cJSON_CreateString("hello");
    cJSON_AddItemToObject(foo, "bar", bar);

    // Retrieve with exact path
    cJSON* result = cJSONUtils_GetPointerCaseSensitive(root, "/foo/bar");
    CHECK(result != NULL, "Expected non-NULL result for path '/foo/bar'");
    if (result != NULL) {
        CHECK(result->type == cJSON_String, "Expected result to be a string type at '/foo/bar'");
        CHECK(result->valuestring != NULL && std::strcmp(result->valuestring, "hello") == 0,
              "Expected value 'hello' at '/foo/bar'");
    }

    cJSON_Delete(root);
}

// Test 2: Case sensitivity should cause mismatch when path key casing differs
void test_GetPointerCaseSensitive_case_mismatch() {
    // Build JSON: { "foo": { "baz": "qux" } }
    cJSON* root = cJSON_CreateObject();
    cJSON* child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "foo", child);
    cJSON* val = cJSON_CreateString("qux");
    cJSON_AddItemToObject(child, "baz", val);

    // Retrieve with different case: "FOO" vs "foo" should fail due to case sensitivity
    cJSON* result = cJSONUtils_GetPointerCaseSensitive(root, "/FOO/baz");
    CHECK(result == NULL, "Expected NULL result due to case-sensitive mismatch on key '/FOO/baz'");

    cJSON_Delete(root);
}

// Test 3: Retrieval from an array using an index in the pointer
void test_GetPointerCaseSensitive_array() {
    // Build JSON: { "arr": ["zero", "one"] }
    cJSON* root = cJSON_CreateObject();
    cJSON* arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("zero"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));

    // Retrieve second element
    cJSON* result = cJSONUtils_GetPointerCaseSensitive(root, "/arr/1");
    CHECK(result != NULL, "Expected non-NULL result for path '/arr/1'");
    if (result != NULL) {
        CHECK(result->type == cJSON_String, "Expected result to be a string type at '/arr/1'");
        CHECK(result->valuestring != NULL && std::strcmp(result->valuestring, "one") == 0,
              "Expected value 'one' at '/arr/1'");
    }

    cJSON_Delete(root);
}

// Test 4: Non-existent path should return NULL
void test_GetPointerCaseSensitive_nonexistent() {
    // Build JSON: { "a": 1 }
    cJSON* root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "a", cJSON_CreateNumber(1));

    // Retrieve non-existent path
    cJSON* result = cJSONUtils_GetPointerCaseSensitive(root, "/b");
    CHECK(result == NULL, "Expected NULL for non-existent path '/b'");

    cJSON_Delete(root);
}

// Test 5: NULL object input should return NULL
void test_GetPointerCaseSensitive_null_object() {
    cJSON* result = cJSONUtils_GetPointerCaseSensitive(NULL, "/any");
    CHECK(result == NULL, "Expected NULL when input object is NULL");
}

// Main test runner
int main() {
    // Run tests
    test_GetPointerCaseSensitive_basic();
    test_GetPointerCaseSensitive_case_mismatch();
    test_GetPointerCaseSensitive_array();
    test_GetPointerCaseSensitive_nonexistent();
    test_GetPointerCaseSensitive_null_object();

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}