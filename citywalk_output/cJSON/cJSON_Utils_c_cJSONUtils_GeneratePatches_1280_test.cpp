// This is a lightweight C++11 test suite for the C function:
//     cJSONUtils_GeneratePatches(cJSON * const from, cJSON * const to)
// The tests are self-contained and do not rely on GoogleTest.
// They exercise basic success and failure paths, and validate that the
// function returns a non-null JSON array when given valid inputs.
// The tests call into the C code via extern "C" to avoid name mangling.

// Domain knowledge notes embedded as comments:
// - Candidate Keywords: from, to, patches, cJSON_CreateArray, create_patches, NULL checks, path handling
// - We verify NULL handling, basic non-NULL behavior, and array-ness of the result
// - Static helpers within the focal C file are not directly testable (static scope),
//   so tests focus on the observable behavior of cJSONUtils_GeneratePatches

#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cJSON_Utils.h>
#include <limits.h>


extern "C" {
// Include C headers with C linkage
}

// Simple non-terminating assertion framework (prints failures but continues)
// to maximize code execution paths for coverage.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, message) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "ASSERT FAILED: " << (message) << " (Line "<< __LINE__ <<")" << std::endl; \
    } \
} while(0)

#define TEST_OK(message) do { \
    ++g_total_tests; \
    std::cout << "TEST PASSED: " << (message) << std::endl; \
} while(0)

// Test 1: NULL from should return NULL
static void test_GeneratePatches_NULL_from_returns_NULL()
{
    // Prepare a non-NULL 'to' object to isolate the failure mode to the 'from' being NULL
    cJSON *to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "value", 1);

    cJSON *patches = cJSONUtils_GeneratePatches(NULL, to);

    TEST_ASSERT(patches == NULL, "GeneratePatches should return NULL when 'from' is NULL");

    // Cleanup
    if (patches) cJSON_Delete(patches);
    if (to) cJSON_Delete(to);

    TEST_OK("test_GeneratePatches_NULL_from_returns_NULL completed");
}

// Test 2: NULL to should return NULL
static void test_GeneratePatches_NULL_to_returns_NULL()
{
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "value", 1);

    cJSON *patches = cJSONUtils_GeneratePatches(from, NULL);

    TEST_ASSERT(patches == NULL, "GeneratePatches should return NULL when 'to' is NULL");

    // Cleanup
    if (patches) cJSON_Delete(patches);
    if (from) cJSON_Delete(from);

    TEST_OK("test_GeneratePatches_NULL_to_returns_NULL completed");
}

// Test 3: Basic difference - from and to differ on a top-level key
static void test_GeneratePatches_BasicDiff()
{
    // from: {"a":1}
    // to:   {"a":2}
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);

    cJSON *to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 2);

    cJSON *patches = cJSONUtils_GeneratePatches(from, to);

    TEST_ASSERT(patches != NULL, "GeneratePatches should return a non-NULL array for valid inputs");
    TEST_ASSERT(cJSON_IsArray(patches), "Generated patches should be a JSON array");

    // Cleanup
    if (patches) cJSON_Delete(patches);
    if (to) cJSON_Delete(to);
    if (from) cJSON_Delete(from);

    TEST_OK("test_GeneratePatches_BasicDiff completed");
}

// Test 4: No change (identical inputs) yields an array as well (behavior may vary, but should be non-NULL)
static void test_GeneratePatches_NoChange()
{
    // from and to are identical
    cJSON *from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "x", 42);
    cJSON *to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "x", 42);

    cJSON *patches = cJSONUtils_GeneratePatches(from, to);

    TEST_ASSERT(patches != NULL, "GeneratePatches should return a non-NULL array even when there is no change");
    TEST_ASSERT(cJSON_IsArray(patches), "Generated patches should be a JSON array");

    // Cleanup
    if (patches) cJSON_Delete(patches);
    if (to) cJSON_Delete(to);
    if (from) cJSON_Delete(from);

    TEST_OK("test_GeneratePatches_NoChange completed");
}

// Test 5: Nested structures to ensure recursion/complex paths are handled
static void test_GeneratePatches_NestedDiff()
{
    // from: {"a": {"b": 1, "c": 3}, "d": 4}
    // to:   {"a": {"b": 2, "c": 3}, "d": 4, "e": 5}
    cJSON *from = cJSON_CreateObject();
    cJSON *nestedFrom = cJSON_CreateObject();
    cJSON_AddNumberToObject(nestedFrom, "b", 1);
    cJSON_AddNumberToObject(nestedFrom, "c", 3);
    cJSON_AddItemToObject(from, "a", nestedFrom);
    cJSON_AddNumberToObject(from, "d", 4);

    cJSON *to = cJSON_CreateObject();
    cJSON *nestedTo = cJSON_CreateObject();
    cJSON_AddNumberToObject(nestedTo, "b", 2);
    cJSON_AddNumberToObject(nestedTo, "c", 3);
    cJSON_AddItemToObject(to, "a", nestedTo);
    cJSON_AddNumberToObject(to, "d", 4);
    cJSON_AddNumberToObject(to, "e", 5);

    cJSON *patches = cJSONUtils_GeneratePatches(from, to);

    TEST_ASSERT(patches != NULL, "GeneratePatches should return a non-NULL array for nested differences");
    TEST_ASSERT(cJSON_IsArray(patches), "Generated patches should be a JSON array");

    // Cleanup
    if (patches) cJSON_Delete(patches);
    if (to) cJSON_Delete(to); // This is safe as nestedTo was moved into 'to'
    if (from) cJSON_Delete(from); // This will recursively free nestedFrom

    TEST_OK("test_GeneratePatches_NestedDiff completed");
}

// Entry point for the test suite
int main()
{
    std::cout << "Starting test suite for cJSONUtils_GeneratePatches...\n";

    test_GeneratePatches_NULL_from_returns_NULL();
    test_GeneratePatches_NULL_to_returns_NULL();
    test_GeneratePatches_BasicDiff();
    test_GeneratePatches_NoChange();
    test_GeneratePatches_NestedDiff();

    std::cout << "Test suite completed. Total: " << g_total_tests
              << " Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}