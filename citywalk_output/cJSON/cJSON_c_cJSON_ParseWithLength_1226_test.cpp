// This test suite targets the focal method cJSON_ParseWithLength within the CJSON project.
// It verifies a variety of input scenarios to ensure correct behavior of the parsing
// pathway when buffer_length is supplied. The tests are written in C++11 without GTest.
// They rely on the public API of cJSON and do not access private/static helpers directly.
// The tests are designed to be non-terminating (they collect failures and continue).

// Candidate Keywords (Step 1):
// - cJSON_ParseWithLengthOpts
// - cJSON_ParseWithLength (focal method)
// - buffer_length
// - return_parse_end
// - require_null_terminated
// - cJSON_IsNumber, cJSON_GetNumberValue
// - cJSON_IsString, cJSON_GetStringValue
// - cJSON_IsObject, cJSON_GetObjectItem
// - cJSON_IsNull
// - cJSON_IsBool, cJSON_IsTrue, cJSON_IsFalse
// - cJSON_IsArray, cJSON_GetArraySize, cJSON_GetArrayItem
// - cJSON_ParseWithOpts
// - Basic JSON types: number, string, object, array, boolean, null
// - Memory management: cJSON_Delete

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
#include <string>
#include <limits.h>


extern "C" {
}

// Simple non-terminating test framework (no GTest, no throws)
static int g_failures = 0;

#define TEST_EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << msg << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define TEST_EXPECT_NOT_NULL(ptr, msg) do { \
    if((ptr) == NULL) { \
        std::cerr << "TEST_FAIL: " << msg << " (null pointer) (line " \
                  << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define TEST_EXPECT_NULL(ptr, msg) do { \
    if((ptr) != NULL) { \
        std::cerr << "TEST_FAIL: " << msg << " (not null) (line " \
                  << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

static void test_parse_with_length_number() {
    // Verify parsing a simple integer number when buffer_length equals the number's length
    const char* json = "42";
    cJSON* item = cJSON_ParseWithLength(json, 2);
    TEST_EXPECT_NOT_NULL(item, "Parse 42 with length 2 should not be NULL");
    if (item) {
        TEST_EXPECT_TRUE(cJSON_IsNumber(item), "Parsed item should be a number");
        double val = cJSON_GetNumberValue(item);
        TEST_EXPECT_TRUE(val == 42.0, "Parsed number value should be 42");
        cJSON_Delete(item);
    }
}

static void test_parse_with_length_string() {
    // Verify parsing a JSON string value
    const char* json = "\"hello\"";
    cJSON* item = cJSON_ParseWithLength(json, 7);
    TEST_EXPECT_NOT_NULL(item, "Parse string \"hello\" with length 7 should not be NULL");
    if (item) {
        TEST_EXPECT_TRUE(cJSON_IsString(item), "Parsed item should be a string");
        const char* s = cJSON_GetStringValue(item);
        TEST_EXPECT_TRUE(s != NULL && std::strcmp(s, "hello") == 0, "String value should be 'hello'");
        cJSON_Delete(item);
    }
}

static void test_parse_with_length_object() {
    // Verify parsing a simple object {"a":1}
    const char* json = "{\"a\":1}";
    cJSON* item = cJSON_ParseWithLength(json, 7);
    TEST_EXPECT_NOT_NULL(item, "Parse object {\"a\":1} with length 7 should not be NULL");
    if (item) {
        TEST_EXPECT_TRUE(cJSON_IsObject(item), "Parsed item should be an object");
        cJSON* child = cJSON_GetObjectItem(item, "a");
        TEST_EXPECT_NOT_NULL(child, "Object should contain key 'a'");
        if (child) {
            TEST_EXPECT_TRUE(cJSON_IsNumber(child), "Value for 'a' should be a number");
            TEST_EXPECT_TRUE(cJSON_GetNumberValue(child) == 1.0, "Value for 'a' should be 1");
        }
        cJSON_Delete(item);
    }
}

static void test_parse_with_length_incomplete_false() {
    // Incomplete token should fail gracefully
    const char* json = "tru";
    cJSON* item = cJSON_ParseWithLength(json, 3);
    TEST_EXPECT_NULL(item, "Parse incomplete 'tru' should fail and return NULL");
    // No delete needed if NULL
    (void)item;
}

static void test_parse_with_length_null_with_trailing() {
    // Trailing non-JSON character within length should be treated as valid 'null' if first token is null
    const char* json = "nullX";
    cJSON* item = cJSON_ParseWithLength(json, 4);
    TEST_EXPECT_NOT_NULL(item, "Parse 'nullX' with length 4 should produce a null value");
    if (item) {
        TEST_EXPECT_TRUE(cJSON_IsNull(item), "Value should be JSON null");
        cJSON_Delete(item);
    }
}

static void test_parse_with_length_incomplete_object() {
    // Incomplete object should fail
    const char* json = "{\"a\":";
    cJSON* item = cJSON_ParseWithLength(json, 4);
    TEST_EXPECT_NULL(item, "Parse incomplete object should fail and return NULL");
    (void)item;
}

static void test_parse_with_length_true() {
    // Verify parsing of boolean true
    const char* json = "true";
    cJSON* item = cJSON_ParseWithLength(json, 4);
    TEST_EXPECT_NOT_NULL(item, "Parse 'true' with length 4 should not be NULL");
    if (item) {
        TEST_EXPECT_TRUE(cJSON_IsBool(item), "Parsed item should be a boolean");
        TEST_EXPECT_TRUE(cJSON_IsTrue(item), "Boolean value should be true");
        cJSON_Delete(item);
    }
}

static void test_parse_with_length_array() {
    // Verify parsing of an array [1,2,3]
    const char* json = "[1,2,3]";
    cJSON* item = cJSON_ParseWithLength(json, 9);
    TEST_EXPECT_NOT_NULL(item, "Parse [1,2,3] with length 9 should not be NULL");
    if (item) {
        TEST_EXPECT_TRUE(cJSON_IsArray(item), "Parsed item should be an array");
        int size = cJSON_GetArraySize(item);
        TEST_EXPECT_TRUE(size == 3, "Array size should be 3");
        for (int i = 0; i < size; ++i) {
            cJSON* elem = cJSON_GetArrayItem(item, i);
            TEST_EXPECT_NOT_NULL(elem, "Array element should not be NULL");
            TEST_EXPECT_TRUE(cJSON_IsNumber(elem), "Array element should be a number");
            double v = cJSON_GetNumberValue(elem);
            TEST_EXPECT_TRUE(v == (i + 1), "Array elements should be 1, 2, 3");
        }
        cJSON_Delete(item);
    }
}

// Entry point for tests (Step 3: Test Case Refinement)
int main() {
    std::cout << "Starting tests for cJSON_ParseWithLength..." << std::endl;

    test_parse_with_length_number();
    test_parse_with_length_string();
    test_parse_with_length_object();
    test_parse_with_length_incomplete_false();
    test_parse_with_length_null_with_trailing();
    test_parse_with_length_incomplete_object();
    test_parse_with_length_true();
    test_parse_with_length_array();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}