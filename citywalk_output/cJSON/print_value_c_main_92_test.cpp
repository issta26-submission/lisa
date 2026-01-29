// test_print_value_suite.cpp
// Purpose: Provide a comprehensive, non-GTest C++11 test harness for the
// print_value functionality that the focal main() would exercise (null,
// true/false, number, string, array, object). This harness uses the
// cJSON C library to construct values and verify their JSON representations.
// It adheres to the project's constraints: no GTest, single main entry point,
// and test methods invoked from main. Each test includes explanatory comments.
//
// Notes on alignment with the provided focal method:
// - The focal main() in print_value.c orchestrates Unity tests (print_value_should_print_*).
// - Since we cannot rely on internal static test helpers or the Unity framework
//   in a separate C++ test, this suite instead validates the underlying
//   JSON printing behavior that those tests would exercise, ensuring high
//   coverage of core dependent components (cJSON types and printing).
// - This approach still ensures true/false branches and data-type handling are covered.

#include <unity/src/unity.h>
#include <cJSON.h>
#include <unity/examples/unity_config.h>
#include <iostream>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <stdio.h>


extern "C" {
}

// Lightweight, non-terminating assertion helper.
// Keeps track of total and failed tests to provide a summary at the end.
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const std::string& test_name,
                        const std::string& message,
                        const std::string& expected,
                        const std::string& actual) {
    std::cerr << "TEST FAILED: " << test_name << "\n"
              << "  " << message << "\n"
              << "  Expected: " << expected << "\n"
              << "  Actual  : " << actual << "\n";
    ++g_failed_tests;
}

// Utility: Convert a cJSON value to its unformatted string representation.
static std::string json_to_string(cJSON* item) {
    if (!item) return "";
    char* printed = cJSON_PrintUnformatted(item);
    std::string result = printed ? printed : "";
    free(printed);
    return result;
}

// Test 1: Null value prints as "null"
static void test_null_print(void) {
    const std::string test_name = "test_null_print";
    ++g_total_tests;

    cJSON* v = cJSON_CreateNull();
    std::string s = json_to_string(v);
    cJSON_Delete(v);

    if (s != "null") {
        log_failure(test_name,
                    "Null value should print as 'null'.",
                    "null",
                    s);
    }
}

// Test 2: True boolean prints as "true"
static void test_true_print(void) {
    const std::string test_name = "test_true_print";
    ++g_total_tests;

    cJSON* v = cJSON_CreateBool(1);
    std::string s = json_to_string(v);
    cJSON_Delete(v);

    if (s != "true") {
        log_failure(test_name,
                    "Boolean true should print as 'true'.",
                    "true",
                    s);
    }
}

// Test 3: False boolean prints as "false"
static void test_false_print(void) {
    const std::string test_name = "test_false_print";
    ++g_total_tests;

    cJSON* v = cJSON_CreateBool(0);
    std::string s = json_to_string(v);
    cJSON_Delete(v);

    if (s != "false") {
        log_failure(test_name,
                    "Boolean false should print as 'false'.",
                    "false",
                    s);
    }
}

// Test 4: Number prints with canonical formatting
static void test_number_print(void) {
    const std::string test_name = "test_number_print";
    ++g_total_tests;

    double value = 123.45;
    cJSON* v = cJSON_CreateNumber(value);
    std::string s = json_to_string(v);
    cJSON_Delete(v);

    if (s != "123.45") {
        log_failure(test_name,
                    "Number should print as its canonical decimal representation.",
                    "123.45",
                    s);
    }
}

// Test 5: String value prints with quotes and proper escaping
static void test_string_print(void) {
    const std::string test_name = "test_string_print";
    ++g_total_tests;

    cJSON* v = cJSON_CreateString("hello");
    std::string s = json_to_string(v);
    cJSON_Delete(v);

    if (s != "\"hello\"") {
        log_failure(test_name,
                    "String should print with surrounding quotes.",
                    "\"hello\"",
                    s);
    }
}

// Test 6: Array prints in JSON array form with properly printed elements
static void test_array_print(void) {
    const std::string test_name = "test_array_print";
    ++g_total_tests;

    cJSON* arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToArray(arr, cJSON_CreateNull());

    std::string s = json_to_string(arr);
    cJSON_Delete(arr);

    if (s != "[1,\"two\",null]") {
        log_failure(test_name,
                    "Array should print as [1,\"two\",null].",
                    "[1,\"two\",null]",
                    s);
    }
}

// Test 7: Object prints with insertion-order keys and appropriate values
static void test_object_print(void) {
    const std::string test_name = "test_object_print";
    ++g_total_tests;

    cJSON* obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "a", 1);
    cJSON_AddStringToObject(obj, "b", "two");

    std::string s = json_to_string(obj);
    cJSON_Delete(obj);

    // cJSON preserves insertion order for object keys in unformatted output
    if (s != "{\"a\":1,\"b\":\"two\"}") {
        log_failure(test_name,
                    "Object should print with keys in insertion order.",
                    "{\"a\":1,\"b\":\"two\"}",
                    s);
    }
}

// Test 8: Nested structures (array containing an object and a boolean)
static void test_nested_structure_print(void) {
    const std::string test_name = "test_nested_structure_print";
    ++g_total_tests;

    cJSON* inner = cJSON_CreateObject();
    cJSON_AddStringToObject(inner, "k", "v");

    cJSON* outer = cJSON_CreateArray();
    cJSON_AddItemToArray(outer, inner);
    cJSON_AddItemToArray(outer, cJSON_CreateBool(1));

    std::string s = json_to_string(outer);
    cJSON_Delete(outer); // deletes inner as it's owned by outer

    if (s != "[{\"k\":\"v\"},true]") {
        log_failure(test_name,
                    "Nested structures should print correctly: [[object], true].",
                    "[{\"k\":\"v\"},true]",
                    s);
    }
}

// Test 9: Float and integer mixed inside an object
static void test_float_and_int_in_object_print(void) {
    const std::string test_name = "test_float_and_int_in_object_print";
    ++g_total_tests;

    cJSON* obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "int", 7);
    cJSON_AddNumberToObject(obj, "pi", 3.14159);

    std::string s = json_to_string(obj);
    cJSON_Delete(obj);

    if (s != "{\"int\":7,\"pi\":3.14159}") {
        log_failure(test_name,
                    "Object with int and float should print accurately.",
                    "{\"int\":7,\"pi\":3.14159}",
                    s);
    }
}

// Entry point for the test harness
int main(void) {
    // Run tests
    test_null_print();
    test_true_print();
    test_false_print();
    test_number_print();
    test_string_print();
    test_array_print();
    test_object_print();
    test_nested_structure_print();
    test_float_and_int_in_object_print();

    // Summary
    std::cout << "\nTest Summary: "
              << g_total_tests << " tests run, "
              << g_failed_tests << " failures.\n";

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}