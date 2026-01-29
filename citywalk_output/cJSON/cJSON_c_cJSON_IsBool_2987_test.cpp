/*
Step 1 (Program Understanding) – Candidate Keywords derived from the focal method:
- item (pointer to cJSON)
- NULL check
- item->type
- cJSON_True, cJSON_False flag bits
- bitwise OR/AND logic
- cJSON_IsBool semantics (true when either True or False bit is set)

Step 2 (Unit Test Generation) – Tests target cJSON_IsBool in cJSON.c using public API only.
Notes:
- Use public API: cJSON_CreateTrue, cJSON_CreateFalse, cJSON_CreateNumber, etc.
- Validate NULL, true, false, non-bool types, and a mutated type that artificially sets a bool flag.
- No GTest; implement a lightweight test harness with non-terminating assertions.
- Use C++11, include C headers with C linkage when needed.

Step 3 (Test Case Refinement) – Tests include proper cleanup and cover both content and address-level behaviors via public API.
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


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if(!(cond)) { \
        ++tests_failed; \
        std::cerr << "FAIL: " << msg << std::endl; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)

#define EXPECT_EQ_INT(expected, actual, msg) do { \
    ++tests_run; \
    if((int)(expected) != (int)(actual)) { \
        ++tests_failed; \
        std::cerr << "FAIL: " << msg << " | expected: " << (expected) << " actual: " << (actual) << std::endl; \
    } \
} while(0)

// Test 1: NULL input should return false
void test_IsBool_null_input() {
    // When item is NULL, cJSON_IsBool must return false
    cJSON_bool result = cJSON_IsBool(NULL);
    EXPECT_TRUE(result == 0, "cJSON_IsBool(NULL) should be false");
}

// Test 2: True item should be recognized as bool
void test_IsBool_true_item() {
    cJSON *item = cJSON_CreateTrue();
    if (item == NULL) {
        std::cerr << "WARN: cJSON_CreateTrue returned NULL (allocation issue in environment)" << std::endl;
        // Do not fail tests on allocation issues; skip this test gracefully
        return;
    }
    cJSON_bool result = cJSON_IsBool(item);
    EXPECT_TRUE(result != 0, "cJSON_IsBool should be true for an item created with cJSON_CreateTrue");
    cJSON_Delete(item);
}

// Test 3: False item should be recognized as bool
void test_IsBool_false_item() {
    cJSON *item = cJSON_CreateFalse();
    if (item == NULL) {
        std::cerr << "WARN: cJSON_CreateFalse returned NULL (allocation issue in environment)" << std::endl;
        return;
    }
    cJSON_bool result = cJSON_IsBool(item);
    EXPECT_TRUE(result != 0, "cJSON_IsBool should be true for an item created with cJSON_CreateFalse");
    cJSON_Delete(item);
}

// Test 4: Non-bool types should not be considered bool
void test_IsBool_non_bool_type() {
    cJSON *number_item = cJSON_CreateNumber(123.0);
    if (number_item == NULL) {
        std::cerr << "WARN: cJSON_CreateNumber failed (allocation issue in environment)" << std::endl;
        return;
    }
    cJSON_bool result = cJSON_IsBool(number_item);
    EXPECT_TRUE(result == 0, "cJSON_IsBool should be false for a numeric item");
    cJSON_Delete(number_item);
}

// Test 5: Mutating a non-bool item to include a bool flag should be detected
void test_IsBool_mutated_bit_flag() {
    // Create a non-bool item (string) and artificially set a bool flag bit in its type
    cJSON *string_item = cJSON_CreateString("example");
    if (string_item == NULL) {
        std::cerr << "WARN: cJSON_CreateString failed (allocation issue in environment)" << std::endl;
        return;
    }
    // Intentionally OR the type with cJSON_True bit to simulate a bool flag presence
    // Note: This tests the predicate's reliance on type bit flags, not the semantic type.
    string_item->type |= cJSON_True;
    cJSON_bool result = cJSON_IsBool(string_item);
    EXPECT_TRUE(result != 0, "cJSON_IsBool should detect bool flag when type is OR'ed with cJSON_True");
    cJSON_Delete(string_item);
}

int main() {
    // Run all tests
    test_IsBool_null_input();
    test_IsBool_true_item();
    test_IsBool_false_item();
    test_IsBool_non_bool_type();
    test_IsBool_mutated_bit_flag();

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}