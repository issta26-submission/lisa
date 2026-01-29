/*
This test suite targets the focal method:
    CJSON_PUBLIC(cJSON *) cJSONUtils_GenerateMergePatch(cJSON * const from, cJSON * const to)

The function delegates to a private static helper generate_merge_patch(...).
We create various from/to inputs using the cJSON library and verify that the produced
merge patch conforms to JSON Merge Patch semantics (RFC 7396):
- If a value differs between from and to, the patch contains the new value.
- If a value exists in to but not in from, the patch contains that value (add).
- If a value exists in from but is nullified in to, the patch contains null for that key (delete).
- Nested structures are patched recursively.
- Arrays are replaced wholesale when changed.
- If there is no difference, the patch can be NULL or an empty object (handled by the tests).

Notes:
- This test suite uses plain C++11 without GoogleTest (as requested).
- It uses the cJSON library (cJSON.h, cJSON_Utils.h).
- Static/internal helpers are avoided; we only rely on the public API (cJSONUtils_GenerateMergePatch).
- Memory management: all created cJSON objects are properly deleted after tests.

The tests are executed from main() with simple boolean checks and explanatory comments.
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


extern "C" {
}

// Helper: Deep equality check between two cJSON trees
static bool json_equal(const cJSON *a, const cJSON *b);

// Helper: Create an empty patch expectation
static cJSON* empty_object() {
    return cJSON_CreateObject();
}

// Helper: Determine if a patch is effectively empty (NULL or {}
static bool is_patch_empty(const cJSON* patch) {
    if (patch == NULL) return true;
    if (patch->type != cJSON_Object) return false;
    return patch->child == NULL;
}

// Implementation of deep equality for cJSON structures
static bool json_equal(const cJSON *a, const cJSON *b) {
    if (a == b) return true; // same pointer or both NULL
    if (a == NULL || b == NULL) return false;
    if (a->type != b->type) return false;

    // Objects
    if (a->type == cJSON_Object) {
        // Ensure all keys in a exist in b with equal values
        for (cJSON *child = a->child; child != NULL; child = child->next) {
            const char *key = child->string;
            if (key == NULL) return false;
            cJSON *other = cJSON_GetObjectItemCaseSensitive((cJSON*)b, key);
            if (other == NULL) return false;
            if (!json_equal((const cJSON*)child, (const cJSON*)other)) return false;
        }
        // Ensure b has no extra keys
        for (cJSON *child = b->child; child != NULL; child = child->next) {
            const char *key = child->string;
            if (key == NULL) return false;
            cJSON *other = cJSON_GetObjectItemCaseSensitive((cJSON*)a, key);
            if (other == NULL) return false;
        }
        return true;
    }

    // Arrays
    if (a->type == cJSON_Array) {
        int sizeA = cJSON_GetArraySize((cJSON*)a);
        int sizeB = cJSON_GetArraySize((cJSON*)b);
        if (sizeA != sizeB) return false;
        for (int i = 0; i < sizeA; ++i) {
            cJSON *Aitem = cJSON_GetArrayItem((cJSON*)a, i);
            cJSON *Bitem = cJSON_GetArrayItem((cJSON*)b, i);
            if (!json_equal((const cJSON*)Aitem, (const cJSON*)Bitem)) return false;
        }
        return true;
    }

    // Numbers
    if (a->type == cJSON_Number) {
        double na = cJSON_GetNumberValue(a);
        double nb = cJSON_GetNumberValue(b);
        // Use exact equality; the inputs are simple integers in tests
        return na == nb;
    }

    // Strings
    if (a->type == cJSON_String) {
        const char* sa = a->valuestring;
        const char* sb = b->valuestring;
        if (sa == NULL || sb == NULL) return false;
        return std::strcmp(sa, sb) == 0;
    }

    // Booleans and Nulls
    if (a->type == cJSON_True || a->type == cJSON_False) {
        // Compare booleans by evaluated truthiness
        bool va = cJSON_IsTrue(a);
        bool vb = cJSON_IsTrue(b);
        return va == vb;
    }
    if (a->type == cJSON_NULL) {
        return b->type == cJSON_NULL;
    }

    // If none matched, consider equal only if pointers match (rare)
    return a == b;
}

// Test 1: Simple replace of a top-level key
static bool test_generate_merge_patch_simple_replace() {
    // from: {"a":1}
    // to:   {"a":2}
    // patch: {"a":2}
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);

    cJSON *to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 2);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON_AddNumberToObject(expected, "a", 2);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 2: Add a new key
static bool test_generate_merge_patch_add_key() {
    // from: {}
    // to:   {"b": 2}
    // patch: {"b": 2}
    cJSON *from = cJSON_CreateObject();
    cJSON *to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "b", 2);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON_AddNumberToObject(expected, "b", 2);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 3: Remove a key by setting its value to null
static bool test_generate_merge_patch_remove_key() {
    // from: {"a":1}
    // to:   {}
    // patch: {"a": null}
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);

    cJSON *to = cJSON_CreateObject();

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON *nullval = cJSON_CreateNull();
    cJSON_AddItemToObject(expected, "a", nullval);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 4: Nested simple replace
static bool test_generate_merge_patch_nested_replace() {
    // from: {"a":{"b":1}}
    // to:   {"a":{"b":2}}
    // patch: {"a":{"b":2}}
    cJSON *from = cJSON_CreateObject();
    cJSON *inner_from = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner_from, "b", 1);
    cJSON_AddItemToObject(from, "a", inner_from);

    cJSON *to = cJSON_CreateObject();
    cJSON *inner_to = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner_to, "b", 2);
    cJSON_AddItemToObject(to, "a", inner_to);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON *inner_expected = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner_expected, "b", 2);
    cJSON_AddItemToObject(expected, "a", inner_expected);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 5: Nested delete (nullify nested value)
static bool test_generate_merge_patch_nested_delete() {
    // from: {"a":{"b":1}}
    // to:   {"a":{"b":null}}
    // patch: {"a":{"b":null}}
    cJSON *from = cJSON_CreateObject();
    cJSON *inner_from = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner_from, "b", 1);
    cJSON_AddItemToObject(from, "a", inner_from);

    cJSON *to = cJSON_CreateObject();
    cJSON *inner_to = cJSON_CreateObject();
    cJSON_AddNullToObject(inner_to, "b"); // b: null
    cJSON_AddItemToObject(to, "a", inner_to);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON *inner_expected = cJSON_CreateObject();
    cJSON_AddNullToObject(inner_expected, "b");
    cJSON_AddItemToObject(expected, "a", inner_expected);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 6: Array replacement
static bool test_generate_merge_patch_array_replace() {
    // from: {"a":[1,2]}
    // to:   {"a":[1,3]}
    // patch: {"a":[1,3]}
    cJSON *from = cJSON_CreateObject();
    cJSON *arr_from = cJSON_CreateIntArray((const int[]){1,2}, 2);
    cJSON_AddItemToObject(from, "a", arr_from);

    int tmp[] = {1,3};
    cJSON *to = cJSON_CreateObject();
    cJSON *arr_to = cJSON_CreateIntArray(tmp, 2);
    cJSON_AddItemToObject(to, "a", arr_to);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON *exp_arr = cJSON_CreateIntArray((const int[]){1,3}, 2);
    cJSON_AddItemToObject(expected, "a", exp_arr);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 7: Add nested object
static bool test_generate_merge_patch_add_nested() {
    // from: {}
    // to:   {"x":{"y":5}}
    // patch: {"x":{"y":5}}
    cJSON *from = cJSON_CreateObject();
    cJSON *to = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "y", 5);
    cJSON_AddItemToObject(to, "x", nested);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON *exp_nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(exp_nested, "y", 5);
    cJSON_AddItemToObject(expected, "x", exp_nested);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 8: Change type at a key (from number to object)
static bool test_generate_merge_patch_type_change() {
    // from: {"a":1}
    // to:   {"a":{"b":2}}
    // patch: {"a":{"b":2}}
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);

    cJSON *to = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner, "b", 2);
    cJSON_AddItemToObject(to, "a", inner);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    cJSON *expected = cJSON_CreateObject();
    cJSON *exp_inner = cJSON_CreateObject();
    cJSON_AddNumberToObject(exp_inner, "b", 2);
    cJSON_AddItemToObject(expected, "a", exp_inner);

    bool ok = json_equal(patch, expected);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);
    cJSON_Delete(expected);

    return ok;
}

// Test 9: No change (patch is NULL or empty object)
static bool test_generate_merge_patch_no_change() {
    // from: {"a":1,"c":3}
    // to:   {"a":1,"c":3}
    // patch: {} or NULL
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);
    cJSON_AddNumberToObject(from, "c", 3);

    cJSON *to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 1);
    cJSON_AddNumberToObject(to, "c", 3);

    cJSON *patch = cJSONUtils_GenerateMergePatch(from, to);

    bool ok = is_patch_empty(patch);

    cJSON_Delete(from);
    cJSON_Delete(to);
    cJSON_Delete(patch);

    return ok;
}

// Main: run all tests and report
int main() {
    int passed = 0;
    int total = 9;

    if (test_generate_merge_patch_simple_replace()) {
        std::cout << "[PASS] Test 1: Simple replace" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 1: Simple replace" << std::endl;
    }

    if (test_generate_merge_patch_add_key()) {
        std::cout << "[PASS] Test 2: Add key" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 2: Add key" << std::endl;
    }

    if (test_generate_merge_patch_remove_key()) {
        std::cout << "[PASS] Test 3: Remove key via null" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 3: Remove key via null" << std::endl;
    }

    if (test_generate_merge_patch_nested_replace()) {
        std::cout << "[PASS] Test 4: Nested replace" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 4: Nested replace" << std::endl;
    }

    if (test_generate_merge_patch_nested_delete()) {
        std::cout << "[PASS] Test 5: Nested delete" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 5: Nested delete" << std::endl;
    }

    if (test_generate_merge_patch_array_replace()) {
        std::cout << "[PASS] Test 6: Array replace" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 6: Array replace" << std::endl;
    }

    if (test_generate_merge_patch_add_nested()) {
        std::cout << "[PASS] Test 7: Add nested object" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 7: Add nested object" << std::endl;
    }

    if (test_generate_merge_patch_type_change()) {
        std::cout << "[PASS] Test 8: Type change at key" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 8: Type change at key" << std::endl;
    }

    if (test_generate_merge_patch_no_change()) {
        std::cout << "[PASS] Test 9: No change (empty patch or NULL)" << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Test 9: No change (empty patch or NULL)" << std::endl;
    }

    std::cout << "Summary: Passed " << passed << " / " << total << " tests." << std::endl;
    return (passed == total) ? 0 : 1;
}