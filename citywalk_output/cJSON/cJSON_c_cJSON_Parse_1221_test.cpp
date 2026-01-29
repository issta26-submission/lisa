// Lightweight C++11 test suite for the focal function cJSON_Parse
// This test suite exercises cJSON_Parse (the wrapper around cJSON_ParseWithOpts)
// by feeding various JSON snippets and validating the resulting cJSON structures.
// No Google Test is used; a minimal in-house test harness is implemented.
// The tests rely solely on the public cJSON API (plus a few accessors) and
// do not access private/static internals of the library.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


extern "C" {
}

// Simple in-house test harness that uses non-terminating expectations
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "FAIL: " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { \
        ++g_failed_tests; \
        std::cerr << "FAIL: " << msg << " (line " << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    if (!((actual) == (expected))) { \
        ++g_failed_tests; \
        std::cerr << "FAIL: " << msg << " -- expected: " << (expected) << ", actual: " << (actual) \
                  << " (line " << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_STR_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    if (!(actual) || !(expected) || std::string(actual) != std::string(expected)) { \
        ++g_failed_tests; \
        std::cerr << "FAIL: " << msg << " -- expected: \"" << (expected) \
                  << "\", actual: \"" << (actual ? actual : "NULL") << "\" (line " << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while (0)

// Helper to safely parse and then cleanup
static cJSON* parse_and_handle(const char* json_text) {
    cJSON* item = cJSON_Parse(json_text);
    // Do not rely on any private behavior; simply delete if non-null.
    if (item) {
        cJSON_Delete(item);
    }
    return item;
}

// Test 1: Parse a simple integer
static void test_parse_simple_number() {
    const char* json = "123";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse a simple number");
    if (item) {
        // Validate type and value
        EXPECT_TRUE(cJSON_IsNumber(item), "Parsed item is a number");
        double val = cJSON_GetNumberValue(item);
        EXPECT_EQ(static_cast<long long>(val), 123, "Number value should be 123");
        cJSON_Delete(item);
    }
}

// Test 2: Parse a string value
static void test_parse_string() {
    const char* json = "\"hello\"";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse a string value");
    if (item) {
        EXPECT_TRUE(cJSON_IsString(item), "Parsed item is a string");
        const char* str = cJSON_GetStringValue(item);
        EXPECT_STR_EQ(str, "hello", "String value should be 'hello'");
        cJSON_Delete(item);
    }
}

// Test 3: Parse boolean true
static void test_parse_true() {
    const char* json = "true";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse 'true'");
    if (item) {
        EXPECT_TRUE(cJSON_IsBool(item), "Parsed item is a boolean");
        EXPECT_TRUE(cJSON_IsTrue(item), "Boolean should be true");
        cJSON_Delete(item);
    }
}

// Test 4: Parse boolean false
static void test_parse_false() {
    const char* json = "false";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse 'false'");
    if (item) {
        EXPECT_TRUE(cJSON_IsBool(item), "Parsed item is a boolean");
        EXPECT_TRUE(cJSON_IsFalse(item), "Boolean should be false");
        cJSON_Delete(item);
    }
}

// Test 5: Parse null value
static void test_parse_null() {
    const char* json = "null";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse 'null'");
    if (item) {
        EXPECT_TRUE(cJSON_IsNull(item), "Parsed item is null");
        cJSON_Delete(item);
    }
}

// Test 6: Parse an array
static void test_parse_array() {
    const char* json = "[1, 2, 3]";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse an array");
    if (item) {
        EXPECT_TRUE(cJSON_IsArray(item), "Parsed item is an array");
        int size = cJSON_GetArraySize(item);
        EXPECT_EQ(size, 3, "Array size should be 3");
        cJSON_Delete(item);
    }
}

// Test 7: Parse an object with simple members
static void test_parse_object() {
    const char* json = "{\"a\":1, \"b\":2}";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse an object");
    if (item) {
        EXPECT_TRUE(cJSON_IsObject(item), "Parsed item is an object");
        cJSON* a = cJSON_GetObjectItem(item, "a");
        EXPECT_TRUE(a != nullptr, "Object has member 'a'");
        if (a) {
            EXPECT_TRUE(cJSON_IsNumber(a), "Member 'a' is a number");
            EXPECT_EQ(static_cast<long long>(cJSON_GetNumberValue(a)), 1, "a == 1");
        }
        cJSON_Delete(item);
    }
}

// Test 8: Invalid JSON should return null
static void test_parse_invalid() {
    const char* json = "{";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item == nullptr, "cJSON_Parse should return null for invalid JSON");
    // item is null, nothing to delete
}

// Test 9: Whitespace handling around a value
static void test_parse_with_whitespace() {
    const char* json = "  \t\n  42  \r\n";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should ignore surrounding whitespace");
    if (item) {
        EXPECT_TRUE(cJSON_IsNumber(item), "Parsed item is a number after whitespace");
        EXPECT_EQ(static_cast<long long>(cJSON_GetNumberValue(item)), 42, "Value should be 42");
        cJSON_Delete(item);
    }
}

// Test 10: Nested structures: object inside object with array inside
static void test_parse_nested() {
    const char* json = "{\"outer\": {\"inner\": [1, 2, 3]}}";
    cJSON* item = cJSON_Parse(json);
    EXPECT_TRUE(item != nullptr, "cJSON_Parse should parse nested structures");
    if (item) {
        EXPECT_TRUE(cJSON_IsObject(item), "Top-level is an object");
        cJSON* outer = cJSON_GetObjectItem(item, "outer");
        EXPECT_TRUE(outer != nullptr, "Has 'outer' object");
        if (outer && cJSON_IsObject(outer)) {
            cJSON* inner = cJSON_GetObjectItem(outer, "inner");
            EXPECT_TRUE(inner != nullptr, "Has 'inner' array inside 'outer'");
            if (inner && cJSON_IsArray(inner)) {
                int sz = cJSON_GetArraySize(inner);
                EXPECT_EQ(sz, 3, "Inner array size should be 3");
            }
        }
        cJSON_Delete(item);
    }
}

// Test 11: cJSON_Parse consistency with simple use of ParseWithOpts (no return end, no null termination required)
// This test ensures wrapper behavior does not crash when length opts would be used; we only call Parse to ensure it behaves equivalently for basic inputs.
static void test_parse_consistency_with_opts_basic() {
    const char* json = "{\"a\": 1}";
    // Basic valid input
    cJSON* item1 = cJSON_Parse(json);
    cJSON* item2 = cJSON_ParseWithOpts(json, nullptr, 0);

    EXPECT_TRUE(item1 != nullptr, "Parse with wrapper should return non-null");
    EXPECT_TRUE(item2 != nullptr, "ParseWithOpts with minimal args should return non-null");
    if (item1) cJSON_Delete(item1);
    if (item2) cJSON_Delete(item2);
}

// Runner to execute all tests
static void run_all_tests() {
    std::cout << "Starting cJSON_Parse test suite (C++11)..." << std::endl;

    test_parse_simple_number();
    test_parse_string();
    test_parse_true();
    test_parse_false();
    test_parse_null();
    test_parse_array();
    test_parse_object();
    test_parse_invalid();
    test_parse_with_whitespace();
    test_parse_nested();
    test_parse_consistency_with_opts_basic();

    std::cout << "Tests completed. Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests
              << ", Total: " << g_total_tests << std::endl;
}

int main() {
    run_all_tests();
    return g_failed_tests != 0 ? 1 : 0;
}