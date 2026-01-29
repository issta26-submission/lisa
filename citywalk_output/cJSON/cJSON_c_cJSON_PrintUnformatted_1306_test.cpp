/*
Unit Test Suite for cJSON_PrintUnformatted (C++11)

Overview:
- This test suite targets the focal method: cJSON_PrintUnformatted(const cJSON *item)
  which delegates to print(item, false, &global_hooks) and returns a char*.
- Tests are written in C++11 (no GTest). A lightweight, non-terminating
  assertion mechanism is used to maximize code coverage across tests.
- Tests rely on the public cJSON API (create, modify, print, parse, free).
- Each test includes explanatory comments describing its purpose.

Notes:
- All memory allocated by cJSON_PrintUnformatted should be freed with cJSON_free.
- The test harness does not require compilation of cJSON.c within the test file;
  ensure to compile and link with cJSON.c (or the corresponding library) alongside this test.
*/

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

// Lightweight, non-terminating test harness
static int g_failures = 0;

#define CHECK(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } } while(0)

static void test_PrintUnformatted_SimpleString() {
    // Purpose: Verify that a simple string item is printed and can be parsed back correctly.
    // The unformatted print should wrap the string with quotes.
    cJSON* item = cJSON_CreateString("hello");
    char* out = cJSON_PrintUnformatted(item);
    CHECK(out != nullptr, "cJSON_PrintUnformatted should not return NULL for a valid string");

    // Validate by parsing back
    cJSON* parsed = cJSON_Parse(out);
    CHECK(parsed != nullptr, "Parsed output should be valid JSON");
    if (parsed) {
        cJSON* strItem = cJSON_GetStringValue(parsed) ? parsed : nullptr;
        // Use GetStringValue on a valid string item
        const char* value = cJSON_GetStringValue(parsed);
        CHECK(value != nullptr && std::strcmp(value, "hello") == 0,
              "Parsed string value should be 'hello'");
        cJSON_Delete(parsed);
    }

    cJSON_free(out);
    cJSON_Delete(item);
}

static void test_PrintUnformatted_Object() {
    // Purpose: Ensure an object with primitive members is printed unformatted
    // and can be parsed back with correct member values.
    cJSON* obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "name", "Alice");
    cJSON_AddNumberToObject(obj, "age", 30);

    char* out = cJSON_PrintUnformatted(obj);
    CHECK(out != nullptr, "cJSON_PrintUnformatted should not be NULL for a valid object");

    cJSON* parsed = cJSON_Parse(out);
    CHECK(parsed != nullptr, "Parsed object should be valid JSON");
    if (parsed) {
        cJSON* nameItem = cJSON_GetObjectItem(parsed, "name");
        CHECK(nameItem != nullptr && cJSON_IsString(nameItem),
              "Object should contain string member 'name'");
        if (nameItem) {
            const char* nameVal = cJSON_GetStringValue(nameItem);
            CHECK(nameVal != nullptr && std::strcmp(nameVal, "Alice") == 0,
                  "Name value should be 'Alice'");
        }

        cJSON* ageItem = cJSON_GetObjectItem(parsed, "age");
        CHECK(ageItem != nullptr && cJSON_IsNumber(ageItem),
              "Object should contain numeric member 'age'");
        if (ageItem) {
            double v = cJSON_GetNumberValue(ageItem);
            CHECK(v == 30, "Age value should be 30");
        }

        cJSON_Delete(parsed);
    }

    cJSON_free(out);
    cJSON_Delete(obj);
}

static void test_PrintUnformatted_Array() {
    // Purpose: Verify printing of an array with mixed types and subsequent parsing integrity.
    cJSON* arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    char* out = cJSON_PrintUnformatted(arr);
    CHECK(out != nullptr, "cJSON_PrintUnformatted should not be NULL for a valid array");

    cJSON* parsed = cJSON_Parse(out);
    CHECK(parsed != nullptr, "Parsed array should be valid JSON");
    if (parsed) {
        CHECK(cJSON_IsArray(parsed), "Parsed root should be an array");
        int size = cJSON_GetArraySize(parsed);
        CHECK(size == 2, "Array should have 2 elements");

        cJSON* first = cJSON_GetArrayItem(parsed, 0);
        CHECK(first != nullptr && cJSON_IsNumber(first) && cJSON_GetNumberValue(first) == 1,
              "First element should be number 1");

        cJSON* second = cJSON_GetArrayItem(parsed, 1);
        CHECK(second != nullptr && cJSON_IsString(second) && std::strcmp(cJSON_GetStringValue(second), "two") == 0,
              "Second element should be string 'two'");

        cJSON_Delete(parsed);
    }

    cJSON_free(out);
    cJSON_Delete(arr);
}

static void test_PrintUnformatted_EmptyObject() {
    // Purpose: Ensure an empty object prints as "{}"
    cJSON* obj = cJSON_CreateObject();

    char* out = cJSON_PrintUnformatted(obj);
    CHECK(out != nullptr, "Empty object should produce non-NULL output");
    if (out) {
        CHECK(std::strcmp(out, "{}") == 0, "Empty object unformatted should be '{}'");        
        cJSON_free(out);
    }

    cJSON_Delete(obj);
}

static void test_PrintUnformatted_EmptyArray() {
    // Purpose: Ensure an empty array prints as "[]"
    cJSON* arr = cJSON_CreateArray();

    char* out = cJSON_PrintUnformatted(arr);
    CHECK(out != nullptr, "Empty array should produce non-NULL output");
    if (out) {
        CHECK(std::strcmp(out, "[]") == 0, "Empty array unformatted should be '[]'");
        cJSON_free(out);
    }

    cJSON_Delete(arr);
}

static void test_PrintUnformatted_NestedObject() {
    // Purpose: Ensure nested objects are printed correctly without whitespace.
    cJSON* root = cJSON_CreateObject();
    cJSON* inner = cJSON_CreateObject();
    cJSON_AddStringToObject(inner, "innerKey", "value");
    cJSON_AddItemToObject(root, "outer", inner);

    char* out = cJSON_PrintUnformatted(root);
    CHECK(out != nullptr, "Nested object: output should be non-NULL");
    if (out) {
        // Basic sanity: output should contain both object braces and inner key
        CHECK(std::strstr(out, "\"outer\"") != nullptr,
              "Output should contain outer key");
        CHECK(std::strstr(out, "\"innerKey\"") != nullptr,
              "Output should contain innerKey inside nested object");

        cJSON* parsed = cJSON_Parse(out);
        CHECK(parsed != nullptr, "Parsed nested object should be valid JSON");
        if (parsed) {
            cJSON* outer = cJSON_GetObjectItem(parsed, "outer");
            CHECK(outer != nullptr && cJSON_IsObject(outer),
                  "Outer item should be an object");
            if (outer) {
                cJSON* innerParsed = cJSON_GetObjectItem(outer, "innerKey");
                CHECK(innerParsed != nullptr && cJSON_IsString(innerParsed) &&
                          std::strcmp(cJSON_GetStringValue(innerParsed), "value") == 0,
                      "innerKey in nested object should be 'value'");
            }
            cJSON_Delete(parsed);
        }

        cJSON_free(out);
    }

    // Cleanup
    cJSON_Delete(root); // This also frees inner
}

int main() {
    // Step 2/3: Execute the comprehensive test suite for cJSON_PrintUnformatted
    // The tests cover: simple string, object, array, empty structures, and nested structures.

    test_PrintUnformatted_SimpleString();
    test_PrintUnformatted_Object();
    test_PrintUnformatted_Array();
    test_PrintUnformatted_EmptyObject();
    test_PrintUnformatted_EmptyArray();
    test_PrintUnformatted_NestedObject();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}