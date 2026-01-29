// This test suite targets the focal method:
 // CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse)
 // which delegates to cJSON_Duplicate_rec(item, 0, recurse).
 // We validate deep vs shallow duplication behavior across primitives, arrays, and objects.
 // Dependencies (public API) used in tests:
 // - cJSON_CreateNumber, cJSON_CreateString, cJSON_CreateArray, cJSON_CreateObject
 // - cJSON_AddItemToArray, cJSON_AddNumberToObject, cJSON_AddStringToObject
 // - cJSON_Duplicate, cJSON_IsNumber, cJSON_GetNumberValue
 // - cJSON_GetArraySize, cJSON_GetArrayItem
 // - cJSON_GetObjectItem, cJSON_GetStringValue
 // - cJSON_ReplaceItemInArray, cJSON_ReplaceItemInObject
 // - cJSON_Delete
 // - cJSON_IsString
 // - cJSON_Duplicate(NULL, ...) should return NULL
 // Note: This test code is written for a C++11 environment, without Google Test (GTest).

#include <cmath>
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


extern "C" {
}

// Simple test harness (non-terminating): prints PASS/FAIL for each test.
// It relies only on the C API provided by cJSON.h.

static bool g_any_failure = false;

// Helper: robust double comparison
static bool almost_equal_double(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

/*
 Candidate Keywords (from Step 1):
 - cJSON_Duplicate, cJSON_Duplicate_rec
 - cJSON_CreateNumber, cJSON_CreateString, cJSON_CreateArray, cJSON_CreateObject
 - cJSON_AddItemToArray, cJSON_AddNumberToObject, cJSON_AddStringToObject
 - cJSON_GetArraySize, cJSON_GetArrayItem
 - cJSON_GetObjectItem, cJSON_GetStringValue
 - cJSON_ReplaceItemInArray, cJSON_ReplaceItemInObject
 - cJSON_IsNumber, cJSON_IsString
 - cJSON_Delete
 - cJSON_IsObject, cJSON_IsArray
 - cJSON_IsNull
 - cJSON_Duplicate(NULL, ...) behavior
 - Indirect testing of deep vs shallow copy via recursion flag
*/

// Test 1: Duplicate a primitive number with no recursion
static bool test_duplicate_primitive_no_recursion() {
    // Primitive values should copy their value and type regardless of recursion (no children to copy)
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *dup = cJSON_Duplicate(num, 0); // no recursion
    bool ok = (dup != NULL) && cJSON_IsNumber(dup) && almost_equal_double(cJSON_GetNumberValue(dup), 42.0);

    if (!ok) {
        std::cerr << "FAIL: test_duplicate_primitive_no_recursion\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_primitive_no_recursion\n";
    }

    cJSON_Delete(num);
    cJSON_Delete(dup);
    return ok;
}

// Test 2: Duplicate a primitive number with recursion (no further effect for primitive)
static bool test_duplicate_primitive_with_recursion() {
    cJSON *num = cJSON_CreateNumber(-3.14);
    cJSON *dup = cJSON_Duplicate(num, 1); // recursive copy
    bool ok = (dup != NULL) && cJSON_IsNumber(dup) && almost_equal_double(cJSON_GetNumberValue(dup), -3.14);

    if (!ok) {
        std::cerr << "FAIL: test_duplicate_primitive_with_recursion\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_primitive_with_recursion\n";
    }

    cJSON_Delete(num);
    cJSON_Delete(dup);
    return ok;
}

// Test 3: Duplicate an array with no recursion -> should yield an empty array
static bool test_duplicate_array_no_recursion_empty() {
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    cJSON *dup = cJSON_Duplicate(arr, 0);
    bool ok = (dup != NULL) && cJSON_IsArray(dup) && cJSON_GetArraySize(dup) == 0;

    if (!ok) {
        std::cerr << "FAIL: test_duplicate_array_no_recursion_empty\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_array_no_recursion_empty\n";
    }

    cJSON_Delete(arr);
    cJSON_Delete(dup);
    return ok;
}

// Test 4: Duplicate an array with recursion -> elements should be duplicated (deep copy)
static bool test_duplicate_array_with_recursion_deep() {
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    cJSON *dup = cJSON_Duplicate(arr, 1);
    bool ok = false;
    if (dup != NULL && cJSON_IsArray(dup) && cJSON_GetArraySize(dup) == 2) {
        cJSON *e0 = cJSON_GetArrayItem(dup, 0);
        cJSON *e1 = cJSON_GetArrayItem(dup, 1);
        bool e0_ok = cJSON_IsNumber(e0) && almost_equal_double(cJSON_GetNumberValue(e0), 1.0);
        bool e1_ok = cJSON_IsString(e1) && (std::strcmp(cJSON_GetStringValue(e1), "two") == 0);
        ok = e0_ok && e1_ok;
    }

    if (!ok) {
        std::cerr << "FAIL: test_duplicate_array_with_recursion_deep\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_array_with_recursion_deep\n";
    }

    // Independence: modify original; duplicate should stay unchanged
    cJSON *first_orig = cJSON_GetArrayItem(arr, 0);
    cJSON_ReplaceItemInArray(arr, 0, cJSON_CreateNumber(99.0));

    // verify dup's first element remains 1
    cJSON *dup_first = cJSON_GetArrayItem(dup, 0);
    bool independent = cJSON_IsNumber(dup_first) && almost_equal_double(cJSON_GetNumberValue(dup_first), 1.0);

    if (!independent) {
        std::cerr << "FAIL: test_duplicate_array_with_recursion_deep_independence\n";
        g_any_failure = true;
        ok = false;
    } else {
        std::cout << "PASS: test_duplicate_array_with_recursion_deep_independence\n";
    }

    (void)first_orig; // suppress unused warning in some compilers
    cJSON_Delete(arr);
    cJSON_Delete(dup);
    return ok && independent;
}

// Test 5: Duplicate an object with recursion -> members should be copied
static bool test_duplicate_object_with_recursion_deep() {
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "x", 5);
    cJSON_AddStringToObject(obj, "y", "hi");

    cJSON *dup = cJSON_Duplicate(obj, 1);
    bool ok = false;
    if (dup != NULL && cJSON_IsObject(dup)) {
        cJSON *dx = cJSON_GetObjectItem(dup, "x");
        cJSON *dy = cJSON_GetObjectItem(dup, "y");
        bool dx_ok = cJSON_IsNumber(dx) && almost_equal_double(cJSON_GetNumberValue(dx), 5.0);
        bool dy_ok = cJSON_IsString(dy) && (std::strcmp(cJSON_GetStringValue(dy), "hi") == 0);
        ok = dx_ok && dy_ok;
    }

    if (!ok) {
        std::cerr << "FAIL: test_duplicate_object_with_recursion_deep\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_object_with_recursion_deep\n";
    }

    // Independence: modify original's "x" and ensure dup's "x" remains 5
    cJSON_ReplaceItemInObject(obj, "x", cJSON_CreateNumber(42));
    cJSON *dup_x = cJSON_GetObjectItem(dup, "x");
    bool independent = cJSON_IsNumber(dup_x) && almost_equal_double(cJSON_GetNumberValue(dup_x), 5.0);
    if (!independent) {
        std::cerr << "FAIL: test_duplicate_object_with_recursion_deep_independence\n";
        g_any_failure = true;
        ok = false;
    } else {
        std::cout << "PASS: test_duplicate_object_with_recursion_deep_independence\n";
    }

    cJSON_Delete(obj);
    cJSON_Delete(dup);
    return ok && independent;
}

// Test 6: Duplicate NULL pointer should produce NULL
static bool test_duplicate_null_pointer() {
    cJSON *dup = cJSON_Duplicate(NULL, 1);
    bool ok = (dup == NULL);
    if (!ok) {
        std::cerr << "FAIL: test_duplicate_null_pointer\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_null_pointer\n";
    }
    cJSON_Delete(dup);
    return ok;
}

// Test 7: Deep nested structure test (outer -> inner -> val) and independence
static bool test_duplicate_deep_nested_independence() {
    // Build: {"outer": {"inner": {"val": 3}}}
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner, "val", 3);

    cJSON *outer = cJSON_CreateObject();
    cJSON_AddItemToObject(outer, "inner", inner);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "outer", outer);

    cJSON *dup_root = cJSON_Duplicate(root, 1);

    bool ok = false;
    if (dup_root != NULL) {
        cJSON *dup_outer = cJSON_GetObjectItem(dup_root, "outer");
        if (dup_outer && cJSON_IsObject(dup_outer)) {
            cJSON *dup_inner = cJSON_GetObjectItem(dup_outer, "inner");
            if (dup_inner && cJSON_IsObject(dup_inner)) {
                cJSON *dup_val = cJSON_GetObjectItem(dup_inner, "val");
                ok = cJSON_IsNumber(dup_val) && almost_equal_double(cJSON_GetNumberValue(dup_val), 3.0);
            }
        }
    }

    if (!ok) {
        std::cerr << "FAIL: test_duplicate_deep_nested_independence (pre-check)\n";
        g_any_failure = true;
    } else {
        std::cout << "PASS: test_duplicate_deep_nested_independence (pre-check)\n";
    }

    // Independence: modify original's inner val to 7, ensure dup stays 3
    cJSON *orig_inner_val = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "outer"), "inner"), "val");
    if (orig_inner_val && cJSON_IsNumber(orig_inner_val)) {
        // Change value in the original
        cJSON_ReplaceItemInObject(cJSON_GetObjectItem(root, "outer"), "inner",
                                cJSON_CreateObject()); // placeholder to ensure API path exists (not used further)
        // Simpler: replace the inner val directly
        cJSON *new_inner = cJSON_CreateObject();
        cJSON_AddNumberToObject(new_inner, "val", 7);
        cJSON_ReplaceItemInObject(cJSON_GetObjectItem(root, "outer"), "inner", new_inner);
        // Check dup remains unchanged
        cJSON *dup_inner_val = cJSON_GetObjectItem(cJSON_GetObjectItem(dup_root, "outer"), "inner");
        cJSON *dup_val_node = cJSON_GetObjectItem(dup_inner_val, "val");
        bool independent = cJSON_IsNumber(dup_val_node) && almost_equal_double(cJSON_GetNumberValue(dup_val_node), 3.0);
        if (!independent) {
            std::cerr << "FAIL: test_duplicate_deep_nested_independence_independence\n";
            g_any_failure = true;
            ok = false;
        } else {
            std::cout << "PASS: test_duplicate_deep_nested_independence_independence\n";
        }
    } else {
        std::cerr << "FAIL: test_duplicate_deep_nested_independence_setup\n";
        g_any_failure = true;
        ok = false;
    }

    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    return ok;
}

// Main: run all tests
int main() {
    std::cout << "Starting cJSON_Duplicate unit tests (C++11 environment)...\n";

    bool all_ok = true;
    all_ok &= test_duplicate_primitive_no_recursion();
    all_ok &= test_duplicate_primitive_with_recursion();
    all_ok &= test_duplicate_array_no_recursion_empty();
    all_ok &= test_duplicate_array_with_recursion_deep();
    all_ok &= test_duplicate_object_with_recursion_deep();
    all_ok &= test_duplicate_null_pointer();
    all_ok &= test_duplicate_deep_nested_independence();

    if (g_any_failure) {
        std::cerr << "One or more tests FAILED.\n";
        return 1;
    } else {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
}