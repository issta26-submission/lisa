// C++11 Test Suite for cJSON_AddItemReferenceToObject from cJSON.c
// This test harness uses the public C API exposed by cJSON.h.
// It does not rely on Google Test; instead, it uses a lightweight
// custom assertion framework that continues execution on failures.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <limits.h>


// Simple non-terminating test assertion framework
static int g_passed = 0;
static int g_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) \
                  << " (in " << __FILE__ << ":" << __LINE__ \
                  << ", function " << __FUNCTION__ << ")" << std::endl; \
        ++g_failed; \
    } else { \
        ++g_passed; \
    } \
} while (0)

static void test_summary() {
    std::cout << "\nTest summary: " << g_passed << " passed, " << g_failed << " failed." << std::endl;
}

// Test 1: Null object parameter should return false
// This validates the first branch of the predicate in cJSON_AddItemReferenceToObject.
void test_AddItemReferenceToObject_NullObject_ReturnsFalse() {
    // Prepare a valid item to avoid affecting this test
    cJSON *item = cJSON_CreateString("value");
    // Case: object is NULL
    cJSON_bool result = cJSON_AddItemReferenceToObject(nullptr, "key", item);
    TEST_ASSERT(result == false, "Expected false when object is NULL");

    // Cleanup
    cJSON_Delete(item);
}

// Test 2: Null string parameter should return false
// This validates the second branch of the predicate.
void test_AddItemReferenceToObject_NullString_ReturnsFalse() {
    cJSON *object = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("value");
    // Case: string is NULL
    cJSON_bool result = cJSON_AddItemReferenceToObject(object, nullptr, item);
    TEST_ASSERT(result == false, "Expected false when string is NULL");

    // Cleanup
    cJSON_Delete(object);
    cJSON_Delete(item);
}

// Test 3: Normal case - adding a reference to an object and validating JSON output
// This ensures that a valid key-item reference is added and prints correctly.
void test_AddItemReferenceToObject_ValidAddition_PrintUnformattedMatches() {
    cJSON *object = cJSON_CreateObject();
    cJSON *original = cJSON_CreateString("value123");

    // Add a reference to a new key
    cJSON_bool result = cJSON_AddItemReferenceToObject(object, "refKey", original);
    TEST_ASSERT(result == true, "Expected true for valid addition of item reference to object");

    // Print the object in unformatted form and compare to expected JSON
    char *printed = cJSON_PrintUnformatted(object);
    TEST_ASSERT(printed != nullptr, "PrintUnformatted should return a non-null string");

    // Expected: {"refKey":"value123"}
    std::string expected = "{\"refKey\":\"value123\"}";
    std::string actual(printed);

    TEST_ASSERT(actual == expected, "Printed JSON does not match expected unformatted JSON");

    // Cleanup
    free(printed); // cJSON_PrintUnformatted allocates with malloc
    cJSON_Delete(object);
    cJSON_Delete(original);
}

// Test 4: Multiple references in one object maintain insertion order
// This ensures that adding multiple references preserves the order in the output.
void test_AddItemReferenceToObject_MultipleReferences_OrderPreserved() {
    cJSON *object = cJSON_CreateObject();

    cJSON *first = cJSON_CreateString("alpha");
    cJSON *second = cJSON_CreateString("beta");

    cJSON_bool r1 = cJSON_AddItemReferenceToObject(object, "first", first);
    cJSON_bool r2 = cJSON_AddItemReferenceToObject(object, "second", second);

    TEST_ASSERT(r1 == true, "First reference addition should succeed");
    TEST_ASSERT(r2 == true, "Second reference addition should succeed");

    char *printed = cJSON_PrintUnformatted(object);
    TEST_ASSERT(printed != nullptr, "PrintUnformatted should return a non-null string");

    // Expected order: {"first":"alpha","second":"beta"}
    std::string expected = "{\"first\":\"alpha\",\"second\":\"beta\"}";
    std::string actual(printed);
    TEST_ASSERT(actual == expected, "Printed JSON does not preserve insertion order for references");

    // Cleanup
    free(printed);
    cJSON_Delete(object);
    cJSON_Delete(first);
    cJSON_Delete(second);
}

// Entry point to run all tests
int main() {
    std::cout << "Starting tests for cJSON_AddItemReferenceToObject..." << std::endl;

    test_AddItemReferenceToObject_NullObject_ReturnsFalse();
    test_AddItemReferenceToObject_NullString_ReturnsFalse();
    test_AddItemReferenceToObject_ValidAddition_PrintUnformattedMatches();
    test_AddItemReferenceToObject_MultipleReferences_OrderPreserved();

    test_summary();
    // Return non-zero if any tests failed
    return (g_failed == 0) ? 0 : 1;
}