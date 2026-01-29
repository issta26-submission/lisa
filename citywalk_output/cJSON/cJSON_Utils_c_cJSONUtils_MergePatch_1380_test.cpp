// Test suite for cJSONUtils_MergePatch
// This suite uses a lightweight, non-terminating assertion framework.
// It avoids GTest and relies on the project's cJSON/ cJSON_Utils API.
// The tests instantiate small JSON objects, apply a patch via cJSONUtils_MergePatch,
// and verify the resulting structure via direct cJSON queries.

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


// Include C headers with C linkage
extern "C" {
}

// Simple non-terminating test assertions
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion macro
#define EXPECT(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << "  [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    } \
} while(0)

// Helper to print test header
static void print_header(const std::string& title) {
    std::cerr << "\n=== " << title << " ===\n";
}

// Test 1: Merging patch that updates an existing key
static void test_merge_patch_updates_existing_key() {
    print_header("Test 1: Update existing key via patch");

    // target: {"a": 1}
    // patch:  {"a": 2}
    cJSON *target = cJSON_CreateObject();
    cJSON_AddNumberToObject(target, "a", 1);

    cJSON *patch = cJSON_CreateObject();
    cJSON_AddNumberToObject(patch, "a", 2);

    // Invoke the focal method
    cJSON *res = cJSONUtils_MergePatch(target, patch);

    // Validate: target's "a" should now be 2
    cJSON *aItem = cJSON_GetObjectItemCaseSensitive(target, "a");
    EXPECT(aItem != NULL, "Merged target should contain key 'a'");
    EXPECT(aItem->type == cJSON_Number, "Merged 'a' should be a number");
    EXPECT(aItem->valueint == 2, "Merged 'a' should have value 2");

    // Cleanup: assume res == target in typical usage; delete target/pach to avoid leaks
    cJSON_Delete(patch);
    cJSON_Delete(target);
    // Do not call cJSON_Delete(res) to avoid double-free if res == target
}

// Test 2: Merging patch that adds a new key
static void test_merge_patch_add_new_key() {
    print_header("Test 2: Add new key via patch");

    // target: {"a": 1}
    // patch:  {"b": 3}
    cJSON *target = cJSON_CreateObject();
    cJSON_AddNumberToObject(target, "a", 1);

    cJSON *patch = cJSON_CreateObject();
    cJSON_AddNumberToObject(patch, "b", 3);

    cJSON *res = cJSONUtils_MergePatch(target, patch);

    // Validate: target should have both "a" and "b"
    cJSON *aItem = cJSON_GetObjectItemCaseSensitive(target, "a");
    cJSON *bItem = cJSON_GetObjectItemCaseSensitive(target, "b");
    EXPECT(aItem != NULL, "Target should still contain key 'a'");
    EXPECT(aItem->valueint == 1, "Original 'a' should remain 1");
    EXPECT(bItem != NULL, "Target should contain newly added key 'b'");
    EXPECT(bItem->type == cJSON_Number, "'b' should be a number");
    EXPECT(bItem->valueint == 3, "New key 'b' should have value 3");

    cJSON_Delete(patch);
    cJSON_Delete(target);
}

// Test 3: Merging patch that removes a key (null value signals deletion)
static void test_merge_patch_remove_key() {
    print_header("Test 3: Remove key via patch (null value)");

    // target: {"a": 1, "b": 2}
    // patch:  {"a": null}
    cJSON *target = cJSON_CreateObject();
    cJSON_AddNumberToObject(target, "a", 1);
    cJSON_AddNumberToObject(target, "b", 2);

    cJSON *patch = cJSON_CreateObject();
    cJSON_AddNullToObject(patch, "a");

    cJSON *res = cJSONUtils_MergePatch(target, patch);

    // Validate: "a" should be removed; "b" remains
    cJSON *aItem = cJSON_GetObjectItemCaseSensitive(target, "a");
    cJSON *bItem = cJSON_GetObjectItemCaseSensitive(target, "b");
    EXPECT(aItem == NULL, "'a' should be removed (NULL in patch)");
    EXPECT(bItem != NULL, "Key 'b' should remain after patch");
    EXPECT(bItem->valueint == 2, "'b' should retain value 2");

    cJSON_Delete(patch);
    cJSON_Delete(target);
}

// Test 4: Nested object merge where inner object is patched
static void test_merge_patch_nested_update() {
    print_header("Test 4: Nested object patching");

    // target: {"a": {"b": 1}}
    // patch:  {"a": {"b": 2, "c": 3}}
    cJSON *target = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner, "b", 1);
    cJSON_AddItemToObject(target, "a", inner);

    cJSON *patch = cJSON_CreateObject();
    cJSON *patchInner = cJSON_CreateObject();
    cJSON_AddNumberToObject(patchInner, "b", 2);
    cJSON_AddNumberToObject(patchInner, "c", 3);
    cJSON_AddItemToObject(patch, "a", patchInner);

    cJSON *res = cJSONUtils_MergePatch(target, patch);

    // Validate: target.a.b == 2; target.a.c == 3
    cJSON *aObj = cJSON_GetObjectItemCaseSensitive(target, "a");
    EXPECT(aObj != NULL, "Target should have key 'a' after patch");
    cJSON *ab = cJSON_GetObjectItemCaseSensitive(aObj, "b");
    cJSON *ac = cJSON_GetObjectItemCaseSensitive(aObj, "c");
    EXPECT(ab != NULL, "Nested key 'a.b' should exist");
    EXPECT(ab->valueint == 2, "Nested 'a.b' should be updated to 2");
    EXPECT(ac != NULL, "Nested key 'a.c' should exist");
    EXPECT(ac->valueint == 3, "Nested 'a.c' should be 3");

    cJSON_Delete(patch);
    cJSON_Delete(target);
}

// Test 5: Nested patch merges while preserving untouched keys
static void test_merge_patch_preserve_other_keys_in_nested() {
    print_header("Test 5: Preserve untouched nested keys during patch");

    // target: {"a": {"b": 1, "e": 5}, "x": 9}
    // patch:  {"a": {"d": 4}}
    cJSON *target = cJSON_CreateObject();
    cJSON *aObj = cJSON_CreateObject();
    cJSON_AddNumberToObject(aObj, "b", 1);
    cJSON_AddNumberToObject(aObj, "e", 5);
    cJSON_AddItemToObject(target, "a", aObj);
    cJSON_AddNumberToObject(target, "x", 9);

    cJSON *patch = cJSON_CreateObject();
    cJSON *patchA = cJSON_CreateObject();
    cJSON_AddNumberToObject(patchA, "d", 4);
    cJSON_AddItemToObject(patch, "a", patchA);

    cJSON *res = cJSONUtils_MergePatch(target, patch);

    // Validate: target.a.b==1 (unchanged), target.a.e==5 (unchanged), target.a.d==4 (new)
    cJSON *aAfter = cJSON_GetObjectItemCaseSensitive(target, "a");
    EXPECT(aAfter != NULL, "Target.a should exist");
    cJSON *ab = cJSON_GetObjectItemCaseSensitive(aAfter, "b");
    cJSON *ae = cJSON_GetObjectItemCaseSensitive(aAfter, "e");
    cJSON *ad = cJSON_GetObjectItemCaseSensitive(aAfter, "d");
    EXPECT(ab != NULL && ab->valueint == 1, "Nested 'a.b' should remain 1");
    EXPECT(ae != NULL && ae->valueint == 5, "Nested 'a.e' should remain 5");
    EXPECT(ad != NULL && ad->valueint == 4, "Nested 'a.d' should be added as 4");

    cJSON_Delete(patch);
    cJSON_Delete(target);
}

// Runner
int main() {
    test_merge_patch_updates_existing_key();
    test_merge_patch_add_new_key();
    test_merge_patch_remove_key();
    test_merge_patch_nested_update();
    test_merge_patch_preserve_other_keys_in_nested();

    std::cerr << "\nTest Summary: Total=" << g_total << " Failures=" << g_failed << "\n";
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}