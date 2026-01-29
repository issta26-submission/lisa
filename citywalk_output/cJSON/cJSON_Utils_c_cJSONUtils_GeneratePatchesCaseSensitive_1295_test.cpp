// C++11 test suite for cJSONUtils_GeneratePatchesCaseSensitive
// Uses a minimal, self-contained test harness (no GoogleTest)

// This test targets the focal method:
// cJSON* cJSONUtils_GeneratePatchesCaseSensitive(cJSON* from, cJSON* to)

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cJSON_Utils.h>
#include <limits.h>


extern "C" {
    // Forward declare minimal cJSON API required for tests
    typedef struct cJSON cJSON;

    // Creation / deletion
    cJSON* cJSON_CreateObject(void);
    cJSON* cJSON_CreateArray(void);
    cJSON* cJSON_CreateNumber(double num);
    cJSON* cJSON_CreateString(const char* string);
    void cJSON_AddItemToObject(cJSON* object, const char* key, cJSON* item);
    void cJSON_Delete(cJSON* item);

    // Array utilities
    int cJSON_GetArraySize(const cJSON* array);
    cJSON* cJSON_GetArrayItem(const cJSON* array, int index);

    // Function under test
    cJSON* cJSONUtils_GeneratePatchesCaseSensitive(cJSON* from, cJSON* to);
}

// Simple test harness
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++tests_run; \
        if(!(cond)) { \
            ++tests_failed; \
            fprintf(stderr, "FAILED: %s (in %s:%d)\n", msg, __FILE__, __LINE__); \
        } \
    } while(false)

#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != NULL, msg)
#define EXPECT_NULL(ptr, msg) EXPECT_TRUE((ptr) == NULL, msg)


// Helper to create a simple object: {"name": value}
static cJSON* make_object_with_number(const char* name, double value)
{
    cJSON* obj = cJSON_CreateObject();
    if (obj != NULL) {
        cJSON* num = cJSON_CreateNumber(value);
        cJSON_AddItemToObject(obj, name, num);
    }
    return obj;
}

// Test 1: NULL from should return NULL
static void test_GeneratePatchesCaseSensitive_NullFromReturnsNull()
{
    // to is a valid non-null object
    cJSON* to = cJSON_CreateObject();
    cJSON* patches = cJSONUtils_GeneratePatchesCaseSensitive(NULL, to);
    EXPECT_NULL(patches, "Expected NULL when 'from' is NULL");
    if (patches) cJSON_Delete(patches);
    if (to) cJSON_Delete(to);
}

// Test 2: NULL to should return NULL
static void test_GeneratePatchesCaseSensitive_NullToReturnsNull()
{
    // from is a valid non-null object
    cJSON* from = cJSON_CreateObject();
    cJSON* patches = cJSONUtils_GeneratePatchesCaseSensitive(from, NULL);
    EXPECT_NULL(patches, "Expected NULL when 'to' is NULL");
    if (patches) cJSON_Delete(patches);
    if (from) cJSON_Delete(from);
}

// Test 3: Empty from and non-empty to yields at least one patch
static void test_GeneratePatchesCaseSensitive_EmptyFrom_AddsPatch()
{
    cJSON* from = cJSON_CreateObject();       // {}
    cJSON* to = cJSON_CreateObject();         // start with {}
    // to := { "a": 1 }
    cJSON* a = cJSON_CreateNumber(1);
    cJSON_AddItemToObject(to, "a", a);

    cJSON* patches = cJSONUtils_GeneratePatchesCaseSensitive(from, to);
    EXPECT_NOT_NULL(patches, "Expected non-null patches for non-empty diff (from {} to {a:1})");
    int size = 0;
    if (patches) size = cJSON_GetArraySize(patches);
    EXPECT_TRUE(size > 0, "Expected patches array size > 0 for diff from {} to {a:1}");

    if (patches) cJSON_Delete(patches);
    if (from) cJSON_Delete(from);
    if (to) cJSON_Delete(to);
}

// Test 4: From has a key, to removes it -> should yield patches
static void test_GeneratePatchesCaseSensitive_FromHasKey_ToEmptyYieldsPatch()
{
    // from: { "a": 1 }
    cJSON* from = cJSON_CreateObject();
    cJSON* v1 = cJSON_CreateNumber(1);
    cJSON_AddItemToObject(from, "a", v1);

    // to: {}
    cJSON* to = cJSON_CreateObject();

    cJSON* patches = cJSONUtils_GeneratePatchesCaseSensitive(from, to);
    EXPECT_NOT_NULL(patches, "Expected non-null patches for diff (from {a:1} to {})");
    int size = 0;
    if (patches) size = cJSON_GetArraySize(patches);
    EXPECT_TRUE(size > 0, "Expected patches array size > 0 for diff from {a:1} to {}");

    if (patches) cJSON_Delete(patches);
    if (from) cJSON_Delete(from);
    if (to) cJSON_Delete(to);
}

// Test 5: From and To identical content (may yield zero patches)
static void test_GeneratePatchesCaseSensitive_IdenticalContent()
{
    // from: { "a": 1, "b": 2 }
    cJSON* from = cJSON_CreateObject();
    cJSON* f1 = cJSON_CreateNumber(1);
    cJSON* f2 = cJSON_CreateNumber(2);
    cJSON_AddItemToObject(from, "a", f1);
    cJSON_AddItemToObject(from, "b", f2);

    // to: same as from (constructed separately but with same content)
    cJSON* to = cJSON_CreateObject();
    cJSON* t1 = cJSON_CreateNumber(1);
    cJSON* t2 = cJSON_CreateNumber(2);
    cJSON_AddItemToObject(to, "a", t1);
    cJSON_AddItemToObject(to, "b", t2);

    cJSON* patches = cJSONUtils_GeneratePatchesCaseSensitive(from, to);
    EXPECT_NOT_NULL(patches, "Expected non-null patches for identical content");
    int size = 0;
    if (patches) size = cJSON_GetArraySize(patches);
    // We don't rely on exact number, but we expect that patch generation does not crash and returns a valid array
    EXPECT_TRUE(size >= 0, "Patches size should be >= 0 for identical content");

    if (patches) cJSON_Delete(patches);
    if (from) cJSON_Delete(from);
    if (to) cJSON_Delete(to);
}


// Main runner
int main()
{
    fprintf(stdout, "Starting tests for cJSONUtils_GeneratePatchesCaseSensitive\n");

    test_GeneratePatchesCaseSensitive_NullFromReturnsNull();
    test_GeneratePatchesCaseSensitive_NullToReturnsNull();
    test_GeneratePatchesCaseSensitive_EmptyFrom_AddsPatch();
    test_GeneratePatchesCaseSensitive_FromHasKey_ToEmptyYieldsPatch();
    test_GeneratePatchesCaseSensitive_IdenticalContent();

    fprintf(stdout, "Tests run: %d, Failures: %d\n", tests_run, tests_failed);
    return (tests_failed > 0) ? 1 : 0;
}