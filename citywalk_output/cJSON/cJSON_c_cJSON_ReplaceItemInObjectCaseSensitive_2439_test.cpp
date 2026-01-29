// Unit test suite for cJSON_ReplaceItemInObjectCaseSensitive
// Follows steps: understand focal method, generate tests for core behavior,
// and refine for coverage. This test suite uses plain C++11, no GTest, and
// exercises cJSON via its public API. It relies on the C library functions
// defined in cJSON.c and cJSON.h.

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


// Include C library header within C linkage in C++ environment.
extern "C" {
}

// Lightweight test harness with non-terminating assertions.
// This enables multiple tests to run in a single invocation and reports
// failures without aborting early.

static int g_total_failures = 0;

// Expectation helpers
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "EXPECT_TRUE failed: " << msg << std::endl;
        ++g_total_failures;
    }
}

static void expect_ptr_eq(const void* a, const void* b, const char* msg) {
    if (a != b) {
        std::cerr << "EXPECT_PTR_EQ failed: " << msg
                  << " (actual=" << a << ", expected=" << b << ")" << std::endl;
        ++g_total_failures;
    }
}

static void expect_int_eq(int a, int b, const char* msg) {
    if (a != b) {
        std::cerr << "EXPECT_EQ failed: " << msg
                  << " (actual=" << a << ", expected=" << b << ")" << std::endl;
        ++g_total_failures;
    }
}

// Test 1: Basic replacement of an object key with a new item (case-sensitive match)
static bool test_basic_replace_success() {
    // Create an object with a single key-value pair: "name" -> "old"
    cJSON *obj = cJSON_CreateObject();
    cJSON *old_item = cJSON_CreateString("old");
    cJSON_AddItemToObject(obj, "name", old_item);

    // Prepare new item to replace: "name" -> 42
    cJSON *new_item = cJSON_CreateNumber(42);

    // Perform replacement with case-sensitive matching
    cJSON_bool res = cJSON_ReplaceItemInObjectCaseSensitive(obj, "name", new_item);

    // Validation
    bool ok = true;
    expect_true(res, "Replacement should return true for existing key with exact case.");
    cJSON *current = cJSON_GetObjectItemCaseSensitive(obj, "name");
    expect_ptr_eq(current, new_item, "Object should now contain the new item pointer.");
    if (current != NULL) {
        int value = (int)cJSON_GetNumberValue(current);
        expect_int_eq(value, 42, "Replaced value should be 42.");
    } else {
        ok = false;
    }

    // Cleanup
    cJSON_Delete(obj); // This should free old_item (linked/unlinked appropriately) and new_item
    // Note: If replacement attached new_item under obj, it will be freed by Delete(obj).
    // If not, it would be a leak; we assume correct library behavior.
    return ok && (g_total_failures == 0);
}

// Test 2: Case-sensitive path does not replace when case differs
static bool test_case_sensitive_no_replace() {
    // Create object with "name" key
    cJSON *obj = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("value1");
    cJSON_AddItemToObject(obj, "name", orig);

    // Attempt to replace with a different key case: "Name"
    cJSON *to_replace = cJSON_CreateString("shouldnotreplace");
    cJSON_bool res = cJSON_ReplaceItemInObjectCaseSensitive(obj, "Name", to_replace);

    // Validation
    bool ok = true;
    expect_true(res == 0, "Replacement should fail for case-mismatch key.");
    cJSON *current = cJSON_GetObjectItemCaseSensitive(obj, "name");
    expect_ptr_eq(current, orig, "Original item should remain under key 'name'.");
    if (current != NULL) {
        const char *text = cJSON_GetStringValue(current);
        expect_true(text && std::strcmp(text, "value1") == 0, "Original value should remain 'value1'.");
    } else {
        ok = false;
    }

    // Cleanup
    cJSON_Delete(obj);
    cJSON_Delete(to_replace);
    return ok && (g_total_failures == 0);
}

// Test 3: Replacing with a NULL object pointer should fail gracefully (no crash)
static bool test_null_object() {
    // Prepare a new item to replace with
    cJSON *new_item = cJSON_CreateString("x");
    // Call API with NULL object
    cJSON_bool res = cJSON_ReplaceItemInObjectCaseSensitive(nullptr, "name", new_item);

    // Validation
    bool ok = true;
    expect_true(res == 0, "Replacement with NULL object should fail (return false).");

    // Cleanup: new_item should be freed if API didn't attach it
    cJSON_Delete(new_item);
    return ok && (g_total_failures == 0);
}

// Test 4: Replacement among multiple keys ensures only the targeted key is affected
static bool test_replace_item_among_multiple() {
    // Create object with keys "a" and "b"
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "a", cJSON_CreateString("A"));
    cJSON_AddItemToObject(obj, "b", cJSON_CreateNumber(2));

    // Replace key "a" with a boolean true
    cJSON *replacement = cJSON_CreateBool(1);
    cJSON_bool res = cJSON_ReplaceItemInObjectCaseSensitive(obj, "a", replacement);

    // Validation
    bool ok = true;
    expect_true(res, "Replacement should succeed for existing key 'a'.");

    cJSON *curA = cJSON_GetObjectItemCaseSensitive(obj, "a");
    expect_ptr_eq(curA, replacement, "Key 'a' should now refer to the replacement item.");
    if (curA && cJSON_IsTrue(curA)) {
        // ok
    } else {
        ok = false;
        std::cerr << "Test 4: Replaced item should be boolean true." << std::endl;
    }

    // Cleanup
    cJSON_Delete(obj);
    // Replacement item is freed by Delete(obj)
    return ok && (g_total_failures == 0);
}

int main() {
    // Run tests
    bool t1 = test_basic_replace_success();
    bool t2 = test_case_sensitive_no_replace();
    bool t3 = test_null_object();
    bool t4 = test_replace_item_among_multiple();

    // Summary
    if (t1) std::cout << "Test 1 passed." << std::endl;
    else std::cout << "Test 1 failed." << std::endl;

    if (t2) std::cout << "Test 2 passed." << std::endl;
    else std::cout << "Test 2 failed." << std::endl;

    if (t3) std::cout << "Test 3 passed." << std::endl;
    else std::cout << "Test 3 failed." << std::endl;

    if (t4) std::cout << "Test 4 passed." << std::endl;
    else std::cout << "Test 4 failed." << std::endl;

    if (g_total_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Total failures: " << g_total_failures << std::endl;
        return 1;
    }
}