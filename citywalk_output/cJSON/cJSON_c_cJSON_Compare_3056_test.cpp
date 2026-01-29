// Test suite for cJSON_Compare (as declared in cJSON.h / implemented in cJSON.c)
// NOTE: This test suite is written in C++11 (no GTest) and uses the public cJSON API.
// It creates various cJSON objects and asserts expected outcomes, covering true/false
// branches, edge cases, array/object recursive comparisons, and case sensitivity behavior.

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
#include <cstdlib>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
}
#else
#endif

// Simple non-terminating test harness (like EXPECT_*) to maximize coverage.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAILED] " << msg << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)


// Test 1: Null pointers should yield false
void test_null_pointers() {
    // Both NULL
    EXPECT_FALSE(cJSON_Compare(nullptr, nullptr, true), "Both NULL should yield false");
    // One NULL, other non-NULL
    cJSON *a = cJSON_CreateNumber(1.0);
    EXPECT_FALSE(cJSON_Compare(a, nullptr, true), "First non-NULL, second NULL should yield false");
    EXPECT_FALSE(cJSON_Compare(nullptr, a, true), "First NULL, second non-NULL should yield false");
    cJSON_Delete(a);
}

// Test 2: Type mismatch should yield false
void test_type_mismatch() {
    cJSON *a = cJSON_CreateNumber(42.0);
    cJSON *b = cJSON_CreateString("forty-two");
    // Different base types should be false
    EXPECT_FALSE(cJSON_Compare(a, b, true), "Different types should yield false");
    cJSON_Delete(a);
    cJSON_Delete(b);
}

// Test 3: Invalid/unknown type should yield false
void test_invalid_type() {
    // Manually construct items with an invalid type (not in 0..0xFF mapping of valid cJSON types)
    cJSON *ia = (cJSON*)malloc(sizeof(cJSON));
    cJSON *ib = (cJSON*)malloc(sizeof(cJSON));
    if (ia && ib) {
        std::memset(ia, 0, sizeof(cJSON));
        std::memset(ib, 0, sizeof(cJSON));
        ia->type = 0x123; // invalid
        ib->type = 0x123; // invalid
        // Should return false due to default case in switch
        EXPECT_FALSE(cJSON_Compare(ia, ib, true), "Invalid type should yield false");
        free(ia);
        free(ib);
    } else {
        // If allocation failed, report as test skip (not fatal)
        std::cerr << "[WARN] malloc failed for invalid type test; skipping." << std::endl;
    }
}

// Test 4: Booleans and Nulls compare by type (true/false/null should be equal if same type)
void test_boolean_and_null_equal() {
    // False vs False -> true
    cJSON *af = cJSON_CreateFalse();
    cJSON *bf = cJSON_CreateFalse();
    EXPECT_TRUE(cJSON_Compare(af, bf, true), "False vs False should be equal");
    cJSON_Delete(af);
    cJSON_Delete(bf);

    // True vs True -> true
    af = cJSON_CreateTrue();
    bf = cJSON_CreateTrue();
    EXPECT_TRUE(cJSON_Compare(af, bf, true), "True vs True should be equal");
    cJSON_Delete(af);
    cJSON_Delete(bf);

    // Null vs Null -> true
    af = cJSON_CreateNull();
    bf = cJSON_CreateNull();
    EXPECT_TRUE(cJSON_Compare(af, bf, true), "Null vs Null should be equal");
    cJSON_Delete(af);
    cJSON_Delete(bf);
}

// Test 5: Numeric values equal vs not equal
void test_numbers() {
    // Equal numbers
    cJSON *a = cJSON_CreateNumber(3.14159);
    cJSON *b = cJSON_CreateNumber(3.14159);
    EXPECT_TRUE(cJSON_Compare(a, b, true), "Identical numbers should compare equal");
    cJSON_Delete(a);
    cJSON_Delete(b);

    // Different numbers
    a = cJSON_CreateNumber(1.0);
    b = cJSON_CreateNumber(2.0);
    EXPECT_FALSE(cJSON_Compare(a, b, true), "Different numbers should not be equal");
    cJSON_Delete(a);
    cJSON_Delete(b);

    // Number vs String should be false (different types)
    a = cJSON_CreateNumber(1.0);
    b = cJSON_CreateString("1");
    EXPECT_FALSE(cJSON_Compare(a, b, true), "Number vs String should yield false");
    cJSON_Delete(a);
    cJSON_Delete(b);
}

// Test 6: String and Raw compare by value, including NULL string handling
void test_strings_and_raw() {
    // Equal strings
    cJSON *a = cJSON_CreateString("hello");
    cJSON *b = cJSON_CreateString("hello");
    EXPECT_TRUE(cJSON_Compare(a, b, true), "Identical strings should be equal");
    cJSON_Delete(a);
    cJSON_Delete(b);

    // Different strings
    a = cJSON_CreateString("foo");
    b = cJSON_CreateString("bar");
    EXPECT_FALSE(cJSON_Compare(a, b, true), "Different strings should not be equal");
    cJSON_Delete(a);
    cJSON_Delete(b);

    // One NULL valuestring should yield false
    a = (cJSON*)malloc(sizeof(cJSON));
    b = cJSON_CreateString("exists");
    if (a) {
        std::memset(a, 0, sizeof(cJSON));
        a->type = cJSON_String;
        a->valuestring = NULL; // simulate NULL string
        EXPECT_FALSE(cJSON_Compare(a, b, true), "Null string in one side should yield false");
        free(a);
        cJSON_Delete(b);
    } else {
        std::cerr << "[WARN] malloc failed for string NULL test; skipping." << std::endl;
        cJSON_Delete(b);
    }
}

// Test 7: Arrays - identical arrays and length mismatch
void test_arrays() {
    // Equal arrays [1,2,3] vs [1,2,3]
    cJSON *a = cJSON_CreateArray();
    cJSON_AddItemToArray(a, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(a, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(a, cJSON_CreateNumber(3));

    cJSON *b = cJSON_CreateArray();
    cJSON_AddItemToArray(b, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(b, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(b, cJSON_CreateNumber(3));

    EXPECT_TRUE(cJSON_Compare(a, b, true), "Identical arrays should be equal");

    cJSON_Delete(a);
    cJSON_Delete(b);

    // Length mismatch: [1,2] vs [1,2,3]
    a = cJSON_CreateArray();
    cJSON_AddItemToArray(a, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(a, cJSON_CreateNumber(2));

    b = cJSON_CreateArray();
    cJSON_AddItemToArray(b, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(b, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(b, cJSON_CreateNumber(3));

    EXPECT_FALSE(cJSON_Compare(a, b, true), "Arrays of different lengths should not be equal");

    cJSON_Delete(a);
    cJSON_Delete(b);
}

// Test 8: Objects - identical objects, missing key, extra key in B, and case sensitivity
void test_objects() {
    // Identical objects {"a":1, "b":2}
    cJSON *a = cJSON_CreateObject();
    cJSON_AddNumberToObject(a, "a", 1);
    cJSON_AddNumberToObject(a, "b", 2);

    cJSON *b = cJSON_CreateObject();
    cJSON_AddNumberToObject(b, "a", 1);
    cJSON_AddNumberToObject(b, "b", 2);

    EXPECT_TRUE(cJSON_Compare(a, b, true), "Identical objects should be equal");

    cJSON_Delete(a);
    cJSON_Delete(b);

    // Missing key in B
    a = cJSON_CreateObject();
    cJSON_AddNumberToObject(a, "a", 1);
    cJSON_AddNumberToObject(a, "b", 2);

    b = cJSON_CreateObject();
    cJSON_AddNumberToObject(b, "a", 1);
    // b missing "b"

    EXPECT_FALSE(cJSON_Compare(a, b, true), "A has a key not in B should yield false");

    cJSON_Delete(a);
    cJSON_Delete(b);

    // Extra key in B
    a = cJSON_CreateObject();
    cJSON_AddNumberToObject(a, "a", 1);

    b = cJSON_CreateObject();
    cJSON_AddNumberToObject(b, "a", 1);
    cJSON_AddNumberToObject(b, "extra", 5);

    EXPECT_FALSE(cJSON_Compare(a, b, true), "B has extra key not in A should yield false");

    cJSON_Delete(a);
    cJSON_Delete(b);

    // Case sensitivity: a has "Key" and b has "key"
    a = cJSON_CreateObject();
    cJSON_AddStringToObject(a, "Key", "val");

    b = cJSON_CreateObject();
    cJSON_AddStringToObject(b, "key", "val");

    // case_sensitive = true should fail (no exact match for "Key")
    EXPECT_FALSE(cJSON_Compare(a, b, true), "Case-sensitive mismatch should yield false");

    // case_sensitive = false should treat keys as equal (if implementation supports it)
    EXPECT_TRUE(cJSON_Compare(a, b, false), "Case-insensitive match should yield true");

    cJSON_Delete(a);
    cJSON_Delete(b);
}

// Test 9: Self-comparison (pointer equality) should return true
void test_self_comparison() {
    cJSON *a = cJSON_CreateNull();
    EXPECT_TRUE(cJSON_Compare(a, a, true), "Same pointer should be equal (self-comparison)");
    cJSON_Delete(a);
}

int main() {
    test_null_pointers();
    test_type_mismatch();
    test_invalid_type();
    test_boolean_and_null_equal();
    test_numbers();
    test_strings_and_raw();
    test_arrays();
    test_objects();
    test_self_comparison();

    std::cout << "CJSON_Compare test results: "
              << g_tests_run << " tests run, "
              << g_tests_failed << " failures." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}