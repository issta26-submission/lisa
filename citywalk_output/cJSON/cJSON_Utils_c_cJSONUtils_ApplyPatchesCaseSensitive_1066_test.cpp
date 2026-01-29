// C++11 unit test suite for cJSONUtils_ApplyPatchesCaseSensitive
// Focus: validate behavior of patch application on cJSON objects.
// Note: This test suite uses the public C API of cJSON and cJSON_Utils.h without GTest.
// It includes explanatory comments for each test case and aims for broad coverage.

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <functional>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <cJSON_Utils.h>
#include <string>
#include <limits.h>


// Include C headers with C linkage
extern "C" {
}

// Simple non-terminating test harness
static int g_failures = 0;
static void log_failure(const char* msg) {
    std::cerr << "Test Failure: " << msg << std::endl;
    ++g_failures;
}
static void log_pass(const char* msg) {
    std::cout << "Test Passed: " << msg << std::endl;
}

// Deep comparison between two cJSON trees (content comparison, not pointer identity)
static bool json_equals(const cJSON* a, const cJSON* b);

// Helper: deep copy via cJSON_Duplicate
static cJSON* json_deep_copy(const cJSON* src) {
    if (!src) return NULL;
    return cJSON_Duplicate(const_cast<cJSON*>(src), 1);
}

// Helper: free a cJSON object safely
static void json_free(cJSON* json) {
    if (json) cJSON_Delete(json);
}

// Helper: compare two cJSON values recursively
static bool json_equals(const cJSON* a, const cJSON* b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    if (a->type != b->type) return false;

    switch (a->type) {
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
            // These types carry no payload; type equality suffices
            return true;

        case cJSON_Number:
            // Compare both valuedouble (usually used)
            if (a->valuedouble != b->valuedouble) return false;
            // Some builds also populate valueint; compare if present
            if ((a->type == cJSON_Number) && (b->type == cJSON_Number)) {
                if (a->valueint != b->valueint) return false;
            }
            return true;

        case cJSON_String:
            if (a->valuestring == NULL && b->valuestring == NULL) return true;
            if (a->valuestring == NULL || b->valuestring == NULL) return false;
            return std::strcmp(a->valuestring, b->valuestring) == 0;

        case cJSON_Array: {
            const cJSON* pa = a->child;
            const cJSON* pb = b->child;
            while (pa != NULL && pb != NULL) {
                if (!json_equals(pa, pb)) return false;
                pa = pa->next;
                pb = pb->next;
            }
            return (pa == NULL && pb == NULL);
        }

        case cJSON_Object: {
            // Compare by iterating keys in 'a' and matching in 'b'
            int countA = 0;
            for (const cJSON* ia = a->child; ia; ia = ia->next) countA++;
            int countB = 0;
            for (const cJSON* ib = b->child; ib; ib = ib->next) countB++;
            if (countA != countB) return false;

            for (const cJSON* ia = a->child; ia; ia = ia->next) {
                const char* key = ia->string;
                const cJSON* ib = cJSON_GetObjectItemCaseSensitive((cJSON*)b, key);
                if (ib == NULL) return false;
                if (!json_equals(ia, ib)) return false;
            }
            return true;
        }

        default:
            // Unknown type; be conservative
            return false;
    }
}

// Test 1: patches is not an array -> should return 1 (malformed patches)
static bool test_patches_not_array() {
    // Build a simple object
    cJSON* obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "a", 1);

    // Patches is an object, not an array
    cJSON* patches = cJSON_CreateObject();

    int status = cJSONUtils_ApplyPatchesCaseSensitive(obj, patches);

    bool pass = (status == 1);
    if (!pass) log_failure("test_patches_not_array: Expected status 1 for non-array patches.");
    log_pass("test_patches_not_array");
    json_free(obj);
    json_free(patches);
    return pass;
}

// Test 2: patches == NULL -> should return 1
static bool test_patches_null() {
    cJSON* obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "a", 1);

    int status = cJSONUtils_ApplyPatchesCaseSensitive(obj, NULL);

    bool pass = (status == 1);
    if (!pass) log_failure("test_patches_null: Expected status 1 when patches is NULL.");
    log_pass("test_patches_null");
    json_free(obj);
    return pass;
}

// Test 3: Empty array patches should yield success (no changes)
static bool test_patches_empty_array_no_op() {
    cJSON* obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "a", 1);

    cJSON* patches = cJSON_CreateArray(); // empty

    int status = cJSONUtils_ApplyPatchesCaseSensitive(obj, patches);

    bool pass = (status == 0);
    if (!pass) log_failure("test_patches_empty_array_no_op: Expected status 0 for empty array patches.");
    // Ensure object unchanged
    cJSON* a_item = cJSON_GetObjectItemCaseSensitive(obj, "a");
    if (!a_item || a_item->valueint != 1) {
        log_failure("test_patches_empty_array_no_op: Object modified unexpectedly.");
        pass = false;
    }
    log_pass("test_patches_empty_array_no_op");
    json_free(obj);
    json_free(patches);
    return pass;
}

// Test 4: Generate patches and apply -> object should become the target
static bool test_generate_and_apply_patches() {
    // from: {"a":1,"b":2}
    cJSON* from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);
    cJSON_AddNumberToObject(from, "b", 2);

    // to: {"a":1,"b":3,"c":4}
    cJSON* to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 1);
    cJSON_AddNumberToObject(to, "b", 3);
    cJSON_AddNumberToObject(to, "c", 4);

    // Generate patches from "from" to "to"
    cJSON* patches = cJSONUtils_GeneratePatches(from, to);
    if (!patches || !cJSON_IsArray(patches)) {
        log_failure("test_generate_and_apply_patches: Failed to generate patches as an array.");
        json_free(from);
        json_free(to);
        json_free(patches);
        return false;
    }

    // Apply patches to a copy of "from"
    cJSON* object_copy = json_deep_copy(from);
    int status = cJSONUtils_ApplyPatchesCaseSensitive(object_copy, patches);

    bool pass = (status == 0) && json_equals(object_copy, to);
    if (!pass) {
        log_failure("test_generate_and_apply_patches: Applied result did not match target or status != 0.");
    }
    log_pass("test_generate_and_apply_patches");
    json_free(from);
    json_free(to);
    json_free(patches);
    json_free(object_copy);
    return pass;
}

// Test 5: Patches with an invalid operation should stop and return non-zero
static bool test_invalid_patch_stops_execution() {
    // from: {"a":1,"b":2}
    cJSON* from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);
    cJSON_AddNumberToObject(from, "b", 2);

    // Build patches: first a valid patch to update "b", second with an invalid operation
    cJSON* patch1 = cJSON_CreateObject();
    cJSON_AddStringToObject(patch1, "op", "replace");
    cJSON_AddStringToObject(patch1, "path", "/b");
    cJSON_AddNumberToObject(patch1, "value", 3);

    cJSON* patch2 = cJSON_CreateObject();
    cJSON_AddStringToObject(patch2, "op", "invalid_op"); // intentionally invalid
    cJSON_AddStringToObject(patch2, "path", "/a");
    cJSON_AddNumberToObject(patch2, "value", 999);

    cJSON* patches = cJSON_CreateArray();
    cJSON_AddItemToArray(patches, patch1);
    cJSON_AddItemToArray(patches, patch2);

    int status = cJSONUtils_ApplyPatchesCaseSensitive(from, patches);

    bool pass = (status != 0);
    if (!pass) log_failure("test_invalid_patch_stops_execution: Expected non-zero status due to invalid patch.");
    log_pass("test_invalid_patch_stops_execution");

    // Cleanup
    json_free(from);
    json_free(patch1);
    json_free(patch2);
    json_free(patches);
    return pass;
}

int main() {
    // Run all tests
    std::vector<std::function<bool()>> tests = {
        test_patches_not_array,
        test_patches_null,
        test_patches_empty_array_no_op,
        test_generate_and_apply_patches,
        test_invalid_patch_stops_execution
    };

    int total = static_cast<int>(tests.size());
    int ok = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        bool result = tests[i]();
        if (result) ++ok;
    }

    std::cout << "\nTest Summary: " << ok << " / " << total << " passed. "
              << g_failures << " failures reported." << std::endl;

    // Return 0 if all tests pass; otherwise return 1 to indicate failure
    return (g_failures == 0) ? 0 : 1;
}