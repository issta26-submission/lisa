// C++11 Test Suite for cJSONUtils_ApplyPatches (no GTest; plain C++ test harness)
// This test suite targets the focal function:
//     CJSON_PUBLIC(int) cJSONUtils_ApplyPatches(cJSON * const object, const cJSON * const patches)
// The tests are designed to work with the C-based cJSON library and the cJSON_Utils.c implementation.
//
// How this test aligns with the provided guidance:
// - Step 1 (Program Understanding): The core behavior under test is that ApplyPatches expects an array
//   of patches and applies them sequentially. If the patches argument is not an array, it should return 1.
//   If patches is an empty array, it should return 0. When patches contain items, it calls apply_patch for each.
// - Step 2 (Unit Test Generation): We create tests that verify non-array handling, empty array handling,
//   and a simple patch that adds a string value at a given path. This targets the main control flow and
//   basic patch application semantics without relying on private static helpers.
// - Step 3 (Test Case Refinement): The tests exercise true/false branches and ensure no crashes or leaks.
//   We avoid private access, rely only on publicly exposed API from cJSON and cJSON_Utils, and use a
//   lightweight, non-terminating test harness (no terminating asserts).
//
// Notes:
// - The test uses the C API of cJSON. We include the headers with extern "C" linkage when compiling as C++.
// - We purposefully craft a patch JSON object that uses the JSON Patch style: op = "add", path = "/foo", value = "patched".
// - The tests do not require GMock/GTest; a small harness prints results and aggregates pass/fail counts.

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cJSON_Utils.h>
#include <limits.h>


extern "C" {
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const std::string &test_name, const std::string &message) {
    std::cerr << "[FAIL] " << test_name << ": " << message << "\n";
}

static void log_success(const std::string &test_name) {
    std::cout << "[PASS] " << test_name << "\n";
}

// Test 1: patches is not an array -> Expect return value 1
bool test_ApplyPatches_not_array() {
    const char* test_name = "ApplyPatches_not_array";
    ++g_total_tests;

    // Prepare a non-array patches object
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) {
        log_failure(test_name, "Failed to allocate object.");
        ++g_failed_tests;
        return false;
    }

    cJSON *patches = cJSON_CreateString("not an array");
    if (patches == NULL) {
        cJSON_Delete(object);
        log_failure(test_name, "Failed to allocate patches string.");
        ++g_failed_tests;
        return false;
    }

    int status = cJSONUtils_ApplyPatches(object, patches);

    bool pass = (status == 1);

    if (!pass) {
        log_failure(test_name, "Expected status 1 for non-array patches, got " + std::to_string(status));
    } else {
        log_success(test_name);
    }

    cJSON_Delete(patches);
    cJSON_Delete(object);
    if (!pass) ++g_failed_tests;
    return pass;
}

// Test 2: patches is an empty array -> Expect return value 0 (no changes)
bool test_ApplyPatches_empty_array() {
    const char* test_name = "ApplyPatches_empty_array";
    ++g_total_tests;

    // Prepare an object (can contain existing fields; not important for empty patch case)
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) {
        log_failure(test_name, "Failed to allocate object.");
        ++g_failed_tests;
        return false;
    }

    // Prepare an empty patches array
    cJSON *patches = cJSON_CreateArray();
    if (patches == NULL) {
        cJSON_Delete(object);
        log_failure(test_name, "Failed to allocate patches array.");
        ++g_failed_tests;
        return false;
    }

    int status = cJSONUtils_ApplyPatches(object, patches);

    bool pass = (status == 0);

    if (!pass) {
        log_failure(test_name, "Expected status 0 for empty patches, got " + std::to_string(status));
    } else {
        log_success(test_name);
    }

    // Ensure object remains unchanged (empty)
    cJSON *foo = cJSON_GetObjectItem(object, "foo");
    if (foo != NULL) {
        log_failure(test_name, "Object should not have new properties after empty patches.");
        pass = false;
        ++g_failed_tests;
    }

    cJSON_Delete(patches);
    cJSON_Delete(object);
    if (!pass) {
        // already logged
        ;
    }
    return pass;
}

// Test 3: single patch that adds a string value at path "/foo" -> Expect object.foo == "patched"
bool test_ApplyPatches_add_string_patch() {
    const char* test_name = "ApplyPatches_add_string_patch";
    ++g_total_tests;

    // Prepare an empty object
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) {
        log_failure(test_name, "Failed to allocate object.");
        ++g_failed_tests;
        return false;
    }

    // Prepare patches array with one patch: {"op":"add","path":"/foo","value":"patched"}
    cJSON *patches = cJSON_CreateArray();
    if (patches == NULL) {
        cJSON_Delete(object);
        log_failure(test_name, "Failed to allocate patches array.");
        ++g_failed_tests;
        return false;
    }

    cJSON *patch = cJSON_CreateObject();
    if (patch == NULL) {
        cJSON_Delete(patches);
        cJSON_Delete(object);
        log_failure(test_name, "Failed to allocate patch object.");
        ++g_failed_tests;
        return false;
    }

    cJSON *op = cJSON_CreateString("add");
    cJSON *path = cJSON_CreateString("/foo");
    cJSON *value = cJSON_CreateString("patched");
    if (op == NULL || path == NULL || value == NULL) {
        cJSON_Delete(patch);
        cJSON_Delete(patches);
        cJSON_Delete(object);
        log_failure(test_name, "Failed to create patch components.");
        ++g_failed_tests;
        return false;
    }

    cJSON_AddItemToObject(patch, "op", op);
    cJSON_AddItemToObject(patch, "path", path);
    cJSON_AddItemToObject(patch, "value", value);

    cJSON_AddItemToArray(patches, patch);

    // Apply patches
    int status = cJSONUtils_ApplyPatches(object, patches);

    bool pass = (status == 0);
    if (!pass) {
        log_failure(test_name, "Expected status 0 for successful patch application, got " + std::to_string(status));
    }

    // Validate object.foo == "patched"
    cJSON *foo = cJSON_GetObjectItem(object, "foo");
    if (foo == NULL) {
        log_failure(test_name, "Expected object to have key 'foo' after patch.");
        pass = false;
    } else if (!cJSON_IsString(foo)) {
        log_failure(test_name, "Value at 'foo' is not a string as expected.");
        pass = false;
    } else if (strcmp(foo->valuestring, "patched") != 0) {
        log_failure(test_name, std::string("Value at 'foo' is '") + (foo->valuestring ? foo->valuestring : "NULL") +
                               "', expected 'patched'.");
        pass = false;
    }

    if (pass) {
        log_success(test_name);
    }

    // Cleanup
    // Note: cJSON_Delete will recursively free all allocated children
    cJSON_Delete(patches);
    cJSON_Delete(object);

    if (!pass) ++g_failed_tests;
    return pass;
}

// Main function to run tests
int main() {
    // Run tests
    bool results[] = {
        test_ApplyPatches_not_array(),
        test_ApplyPatches_empty_array(),
        test_ApplyPatches_add_string_patch()
    };

    // Summary
    int total = g_total_tests;
    int failed = g_failed_tests;
    int passed = total - failed;

    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}