/*
 * Test suite for cJSON_ReplaceItemInObject in cJSON.c
 * - Uses C API from cJSON without GTest
 * - A lightweight test harness is implemented to allow non-terminating assertions
 * - Tests cover: basic replacement, non-existent key, case-insensitive vs case-sensitive behavior,
 *   and null object handling.
 * - All tests are executable under C++11 with C linkage to the C library.
 */

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

/* Lightweight test harness with non-terminating assertions */
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: %s (at %s:%d)\n", #cond, __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    ++g_total; \
    if(cond) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: not(%s) (at %s:%d)\n", #cond, __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_NOT_NULL(p) do { \
    ++g_total; \
    if((p) == NULL) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: %s is not NULL (at %s:%d)\n", #p, __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_EQUAL_PTR(a, b) do { \
    ++g_total; \
    if((a) != (b)) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: %s == %s (addresses %p != %p) (at %s:%d)\n", #a, #b, (void*)(a), (void*)(b), __FILE__, __LINE__); \
    } \
} while(0)

static void test_replace_item_in_object_basic() {
    // Purpose: Verify that replacing an existing key updates the object to point to the new item
    // and that the old item is detached from the object.
    cJSON *object = cJSON_CreateObject();
    cJSON *olditem = cJSON_CreateNumber(42);
    cJSON_AddItemToObject(object, "answer", olditem);

    cJSON *newitem = cJSON_CreateString("forty-two");
    cJSON_bool res = cJSON_ReplaceItemInObject(object, "answer", newitem);
    EXPECT_TRUE(res);

    cJSON *item = cJSON_GetObjectItem(object, "answer");
    EXPECT_NOT_NULL(item);
    EXPECT_EQUAL_PTR(item, newitem);

    // Validate content
    const char *str = cJSON_GetStringValue(item);
    EXPECT_TRUE(str != NULL);
    EXPECT_TRUE(std::strcmp(str, "forty-two") == 0);

    // Cleanup
    cJSON_Delete(object); // should delete newitem as it is part of the object
    cJSON_Delete(olditem); // olditem is detached; ensure it is freed
}

static void test_replace_item_in_object_nonexistent_key() {
    // Purpose: Replacing a non-existent key should return false and not alter existing keys
    cJSON *object = cJSON_CreateObject();
    cJSON *existing = cJSON_CreateNumber(7);
    cJSON_AddItemToObject(object, "existing", existing);

    cJSON *newitem = cJSON_CreateString("new-value");
    cJSON_bool res = cJSON_ReplaceItemInObject(object, "missing", newitem);
    EXPECT_FALSE(res);

    cJSON *item = cJSON_GetObjectItem(object, "existing");
    EXPECT_NOT_NULL(item);
    EXPECT_EQUAL_PTR(item, existing); // should still be the old item

    // Cleanup
    cJSON_Delete(object); // existing is freed as part of the object
    cJSON_Delete(newitem); // not used in object
    // Note: 'existing' is freed by cJSON_Delete(object)
}

static void test_replace_item_in_object_case_insensitive_and_sensitive() {
    // Purpose: Validate case-insensitive replacement via standard API and case-sensitive replacement via dedicated API
    // 1) Case-insensitive replace using cJSON_ReplaceItemInObject
    cJSON *object1 = cJSON_CreateObject();
    cJSON *old1 = cJSON_CreateNumber(1);
    cJSON_AddItemToObject(object1, "Alpha", old1);

    cJSON *newitem1 = cJSON_CreateString("one");
    cJSON_bool res_insensitive = cJSON_ReplaceItemInObject(object1, "ALPHA", newitem1); // uppercase key, case-insensitive match
    EXPECT_TRUE(res_insensitive);

    cJSON *item1 = cJSON_GetObjectItem(object1, "Alpha");
    EXPECT_NOT_NULL(item1);
    EXPECT_EQUAL_PTR(item1, newitem1);

    // 2) Case-sensitive replacement via cJSON_ReplaceItemInObjectCaseSensitive
    cJSON *object2 = cJSON_CreateObject();
    cJSON *old2 = cJSON_CreateNumber(5);
    cJSON_AddItemToObject(object2, "Beta", old2);

    cJSON *newitem2 = cJSON_CreateString("five");
    cJSON_bool res_case = cJSON_ReplaceItemInObjectCaseSensitive(object2, "Beta", newitem2);
    EXPECT_TRUE(res_case);

    cJSON *item2 = cJSON_GetObjectItem(object2, "Beta");
    EXPECT_NOT_NULL(item2);
    EXPECT_EQUAL_PTR(item2, newitem2);

    // Cleanup
    cJSON_Delete(object1);
    cJSON_Delete(old1); // detached from object1
    cJSON_Delete(newitem1); // freed as part of object1

    cJSON_Delete(object2);
    cJSON_Delete(old2);
    cJSON_Delete(newitem2);
}

static void test_replace_item_in_object_null_object() {
    // Purpose: Replacing on a NULL object pointer should safely return false
    cJSON *newitem = cJSON_CreateString("value");
    cJSON_bool res = cJSON_ReplaceItemInObject(NULL, "any", newitem);
    EXPECT_FALSE(res);
    cJSON_Delete(newitem);
}

int main() {
    // Run tests
    test_replace_item_in_object_basic();
    test_replace_item_in_object_nonexistent_key();
    test_replace_item_in_object_case_insensitive_and_sensitive();
    test_replace_item_in_object_null_object();

    // Summary
    printf("cJSON_ReplaceItemInObject test results: total=%d, failed=%d\n", g_total, g_failed);
    if(g_failed) {
        fprintf(stderr, "Some tests FAILED.\n");
        return EXIT_FAILURE;
    }
    printf("All tests PASSED.\n");
    return EXIT_SUCCESS;
}