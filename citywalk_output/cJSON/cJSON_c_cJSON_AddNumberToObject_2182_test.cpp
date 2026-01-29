/*
  Candidate Keywords extracted from focal method and dependencies:
  - cJSON_AddNumberToObject
  - cJSON_CreateNumber
  - add_item_to_object
  - global_hooks
  - cJSON_Delete
  - NULL
  - number_item

  This test suite targets cJSON_AddNumberToObject in cJSON.c, using C linkage
  to call into the C library from C++11. It includes minimal non-terminating
  assertions to exercise success and failure paths, including object NULL
  and multiple insertions into a JSON object.

  Note: This test harness is designed to be self-contained and not rely on GTest.
*/

#include <cmath>
#include <locale.h>
#include <math.h>
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

static int g_failures = 0;

// Simple approximate equality for doubles
static bool almost_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

// Lightweight non-terminating assertion helper
static void log_failure(const char* test_name, const char* message) {
    std::cerr << "[FAILED] " << test_name << ": " << message << std::endl;
    ++g_failures;
}

// Macro-like helper for non-terminating checks
#define TEST_ASSERT(cond, test_name, message) \
    do { \
        if(!(cond)) log_failure(test_name, message); \
    } while(0)

/*
  Test 1: AddNumberToObject should successfully add a numeric item to a valid object.
  - Create an object with cJSON_CreateObject
  - Call cJSON_AddNumberToObject with a valid name and value
  - Verify the returned pointer is non-NULL and equals the item stored in the object
  - Verify the value stored is equal to the provided number
*/
static void test_AddNumberToObject_Simple() {
    const char* test_name = "AddNumberToObject_Simple";

    cJSON *obj = cJSON_CreateObject();
    TEST_ASSERT(obj != NULL, test_name, "CreateObject failed");

    cJSON *ret = cJSON_AddNumberToObject(obj, "pi", 3.1415926535);
    TEST_ASSERT(ret != NULL, test_name, "AddNumberToObject returned NULL");

    cJSON *item = cJSON_GetObjectItem(obj, "pi");
    TEST_ASSERT(item != NULL, test_name, "Object missing key 'pi'");

    TEST_ASSERT(cJSON_IsNumber(item) != 0, test_name, "Item is not a number");

    double v = cJSON_GetNumberValue(item);
    TEST_ASSERT(almost_equal(v, 3.1415926535), test_name, "Number value mismatch");

    TEST_ASSERT(ret == item, test_name, "Returned pointer is not the same as object item");

    cJSON_Delete(obj);
}

/*
  Test 2: When the object parameter is NULL, AddNumberToObject should return NULL.
  - This exercises the false branch of the internal conditional and ensures
    proper cleanup (number_item deletion) via cJSON_Delete
*/
static void test_AddNumberToObject_ObjectNull() {
    const char* test_name = "AddNumberToObject_ObjectNull";

    cJSON *ret = cJSON_AddNumberToObject(NULL, "x", 1.0);
    TEST_ASSERT(ret == NULL, test_name, "Expected NULL when object is NULL");
}

/*
  Test 3: When the name parameter is NULL, behavior depends on add_item_to_object.
  - We expect the function to fail gracefully and return NULL, exercising the
    other branch (failure path) without crashing.
*/
static void test_AddNumberToObject_NameNull() {
    const char* test_name = "AddNumberToObject_NameNull";

    cJSON *obj = cJSON_CreateObject();
    TEST_ASSERT(obj != NULL, test_name, "CreateObject failed");

    cJSON *ret = cJSON_AddNumberToObject(obj, NULL, 2.0);
    TEST_ASSERT(ret == NULL, test_name, "Expected NULL when name is NULL");

    cJSON_Delete(obj);
}

/*
  Test 4: Add multiple numeric items to the same object and verify both entries exist
  with correct values, and that the returned pointers correspond to the stored items.
*/
static void test_AddNumberToObject_MultipleNumbers() {
    const char* test_name = "AddNumberToObject_MultipleNumbers";

    cJSON *obj = cJSON_CreateObject();
    TEST_ASSERT(obj != NULL, test_name, "CreateObject failed");

    cJSON *ret1 = cJSON_AddNumberToObject(obj, "a", 1.0);
    TEST_ASSERT(ret1 != NULL, test_name, "First Add failed");

    cJSON *ret2 = cJSON_AddNumberToObject(obj, "b", -2.5);
    TEST_ASSERT(ret2 != NULL, test_name, "Second Add failed");

    cJSON *a = cJSON_GetObjectItem(obj, "a");
    cJSON *b = cJSON_GetObjectItem(obj, "b");

    TEST_ASSERT(a != NULL && cJSON_IsNumber(a) != 0 && almost_equal(cJSON_GetNumberValue(a), 1.0),
                test_name, "First value incorrect or not a number");

    TEST_ASSERT(b != NULL && cJSON_IsNumber(b) != 0 && almost_equal(cJSON_GetNumberValue(b), -2.5),
                test_name, "Second value incorrect or not a number");

    TEST_ASSERT(ret1 == a, test_name, "Return pointer for 'a' does not match object item");
    TEST_ASSERT(ret2 == b, test_name, "Return pointer for 'b' does not match object item");

    cJSON_Delete(obj);
}

int main() {
    // Step 2: Unit Test Generation - execute test suite
    test_AddNumberToObject_Simple();
    test_AddNumberToObject_ObjectNull();
    test_AddNumberToObject_NameNull();
    test_AddNumberToObject_MultipleNumbers();

    // Step 3: Test Case Refinement - report summary
    if (g_failures == 0) {
        std::cout << "[PASSED] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] Failures: " << g_failures << std::endl;
        return 1;
    }
}