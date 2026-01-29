/*
Unit test suite for cJSON_DetachItemFromObject (from cJSON.c)

Notes:
- This test suite is designed to be compiled as C++11 code and linked against the
  cJSON.c source (header: cJSON.h).
- No external testing framework is used (GTest-free). A lightweight non-terminating
  assertion mechanism is implemented to maximize code coverage while continuing
  test execution.
- Tests cover:
  1) Detaching an existing item from an object (valid path).
  2) Detaching a non-existent key (null return path).
  3) Detaching the only item in an object (object becomes empty).
  4) Case-insensitive key matching behavior of cJSON_GetObjectItem (via Detach).
- Static helpers inside cJSON.c are not accessed directly; tests interact with the
  public API only as per domain knowledge restrictions.

Compilation:
- Ensure cJSON.c is compiled and linked with this test file, and cJSON.h is accessible.
*/

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Lightweight non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total; \
        if(!(cond)) { \
            ++g_failed; \
            std::fprintf(stderr, "EXPECT_TRUE FAILED: %s (line %d)\n", (msg), __LINE__); \
        } \
    } while(0)

#define EXPECT_NULL(p, msg) \
    do { \
        ++g_total; \
        if((p) != nullptr) { \
            ++g_failed; \
            std::fprintf(stderr, "EXPECT_NULL FAILED: %s (line %d)\n", (msg), __LINE__); \
        } \
    } while(0)

#define EXPECT_NOT_NULL(p, msg) \
    do { \
        ++g_total; \
        if((p) == nullptr) { \
            ++g_failed; \
            std::fprintf(stderr, "EXPECT_NOT_NULL FAILED: %s (line %d)\n", (msg), __LINE__); \
        } \
    } while(0)

#define EXPECT_EQ(p1, p2, msg) \
    do { \
        ++g_total; \
        if((p1) != (p2)) { \
            ++g_failed; \
            std::fprintf(stderr, "EXPECT_EQ FAILED: %s (line %d)\n", (msg), __LINE__); \
        } \
    } while(0)

// Test 1: Detach an existing item from an object
// Purpose: Ensure that the returned pointer is the same as the original item and
// that the object no longer contains the detached key.
static void test_detach_existing_item(void)
{
    // Create an object with two items: "a" (string) and "b" (number)
    cJSON *root = cJSON_CreateObject();
    cJSON *itemA = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "a", itemA);
    cJSON *itemB = cJSON_CreateNumber(42);
    cJSON_AddItemToObject(root, "b", itemB);

    // Detach existing item "a"
    cJSON *detached = cJSON_DetachItemFromObject(root, "a");

    // Validate: detached should be non-null and equal to the original itemA
    EXPECT_NOT_NULL(detached, "Detached pointer for existing key 'a' should not be NULL");
    EXPECT_EQ(detached, itemA, "Detached pointer should equal original itemA");

    // Validate: root should no longer contain key "a"
    cJSON *checkA = cJSON_GetObjectItem(root, "a");
    EXPECT_NULL(checkA, "Root should not contain key 'a' after detach");

    // Validate: root still contains key "b"
    cJSON *checkB = cJSON_GetObjectItem(root, "b");
    EXPECT_NOT_NULL(checkB, "Root should still contain key 'b' after detach");

    // Cleanup: delete detached item and the root
    cJSON_Delete(root);     // Detaches do not delete the detached item
    cJSON_Delete(detached);  // Free the detached item
}

// Test 2: Detach a non-existent key
// Purpose: Ensure that NULL is returned and the original object remains unchanged.
static void test_detach_nonexistent_item(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("value");
    cJSON_AddItemToObject(root, "existing", item);

    // Attempt to detach a non-existent key
    cJSON *detached = cJSON_DetachItemFromObject(root, "nonexistent");

    // Validate: NULL return
    EXPECT_NULL(detached, "Detaching a non-existent key should return NULL");

    // Validate: object unchanged (key "existing" should still be present)
    cJSON *check = cJSON_GetObjectItem(root, "existing");
    EXPECT_NOT_NULL(check, "Object should still contain key 'existing' after failed detach");

    // Cleanup: delete root (will free "existing" item as well)
    cJSON_Delete(root);
}

// Test 3: Detach the only item in an object
// Purpose: Ensure that after detaching the only item, the object is effectively empty.
static void test_detach_only_item(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *only = cJSON_CreateNumber(7);
    cJSON_AddItemToObject(root, "only", only);

    // Detach the only item
    cJSON *detached = cJSON_DetachItemFromObject(root, "only");

    // Validate: non-null and pointer equality
    EXPECT_NOT_NULL(detached, "Detached pointer should not be NULL for the only item");
    EXPECT_EQ(detached, only, "Detached pointer should equal the only item pointer");

    // Validate: object should not contain "only" anymore
    cJSON *check = cJSON_GetObjectItem(root, "only");
    EXPECT_NULL(check, "Root should not contain key 'only' after detach");

    // Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
}

// Test 4: Case-insensitive detachment
// Purpose: Verify that detaching with a differently cased key succeeds (default behavior is case-insensitive).
static void test_detach_case_insensitive(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("Bob");
    cJSON_AddItemToObject(root, "Name", item);

    // Detach using a different case
    cJSON *detached = cJSON_DetachItemFromObject(root, "name");

    // Validate
    EXPECT_NOT_NULL(detached, "Detached pointer should not be NULL for case-insensitive match");
    EXPECT_EQ(detached, item, "Detached pointer should equal the original item with key 'Name'");

    // Ensure the original case key is gone
    cJSON *check = cJSON_GetObjectItem(root, "Name");
    EXPECT_NULL(check, "Root should not contain key 'Name' after detach (case-insensitive match)");

    // Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
}

// Additional helper: a simple summary printer
static void print_summary(void)
{
    std::cout << "CJSON DetachItemFromObject test summary: "
              << g_total - g_failed << "/" << g_total << " passed, "
              << g_failed << " failed." << std::endl;
}

int main(void)
{
    // Run tests
    test_detach_existing_item();
    test_detach_nonexistent_item();
    test_detach_only_item();
    test_detach_case_insensitive();

    // Print summary
    print_summary();
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}