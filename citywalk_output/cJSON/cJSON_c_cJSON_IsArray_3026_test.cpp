// Unit test suite for cJSON_IsArray function
// This test suite follows the provided instructions to generate
// high-quality tests without using GTest, using a lightweight
// C++11 test harness and the C API of cJSON.

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


// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_EXPECT_TRUE(cond) \
    do { \
        ++g_tests_run; \
        if(!(cond)) { \
            ++g_tests_failed; \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected true: " #cond << std::endl; \
        } \
    } while(0)

#define TEST_EXPECT_FALSE(cond) \
    do { \
        ++g_tests_run; \
        if(cond) { \
            ++g_tests_failed; \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected false: " #cond << std::endl; \
        } \
    } while(0)

// Candidate Keywords and reasoning (for traceability in tests)
// - cJSON_IsArray: the focal method under test
// - item: input cJSON item to inspect
// - NULL / nullptr: edge case to exercise null-ptr predicate
// - item->type & 0xFF: internal type mask used by cJSON_IsArray
// - cJSON_Array: the target lower-byte value for array types
// - cJSON_CreateArray / cJSON_CreateNumber: public API used to construct test items
// - cJSON_Delete: cleanup to avoid leaks

// Test 1: Null input should yield false (edge-case handling)
static void test_IsArray_withNullptr() {
    // When item is NULL, function should return false
    TEST_EXPECT_FALSE(cJSON_IsArray(nullptr));
}

// Test 2: True path - actual array item should be detected as array
static void test_IsArray_withActualArray() {
    cJSON *array = cJSON_CreateArray();
    // Ensure allocation succeeded
    if (array == nullptr) {
        std::cerr << "[WARN] Failed to allocate test array item" << std::endl;
        // Do not mark as failed since allocation failure is environment-dependent
        return;
    }
    // The core expectation: cJSON_IsArray should return true for an array
    TEST_EXPECT_TRUE(cJSON_IsArray(array));
    cJSON_Delete(array);
}

// Test 3: Non-array item (e.g., a number) should return false
static void test_IsArray_withNumber() {
    cJSON *num = cJSON_CreateNumber(3.14);
    if (num == nullptr) {
        std::cerr << "[WARN] Failed to allocate test number item" << std::endl;
        return;
    }
    // The core expectation: a number is not an array
    TEST_EXPECT_FALSE(cJSON_IsArray(num));
    cJSON_Delete(num);
}

// Test 4: Manipulate type lower-byte to verify bitmask logic (type & 0xFF)
static void test_IsArray_withManipulatedType() {
    // Create a number item and then artificially force the lower byte to be cJSON_Array
    cJSON *item = cJSON_CreateNumber(42.0);
    if (item == nullptr) {
        std::cerr << "[WARN] Failed to allocate test item for type manipulation" << std::endl;
        return;
    }

    // By default, this is not an array
    TEST_EXPECT_FALSE(cJSON_IsArray(item));

    // Save original type and then modify lower byte to simulate an array tag
    int original_type = item->type;
    item->type = (original_type & ~0xFF) | cJSON_Array; // set lower byte to cJSON_Array

    // Now the lower byte matches cJSON_Array, so IsArray should be true
    TEST_EXPECT_TRUE(cJSON_IsArray(item));

    // Restore and cleanup
    item->type = original_type;
    cJSON_Delete(item);
}

// Test 5: Boundary case - ensure that upper bits do not affect the result
// by combining a non-array value with an upper-bit flag and verifying behavior
static void test_IsArray_upperBitsIgnored() {
    cJSON *object = cJSON_CreateObject();
    if (object == nullptr) {
        std::cerr << "[WARN] Failed to allocate test object" << std::endl;
        return;
    }

    // By default, object is not an array
    TEST_EXPECT_FALSE(cJSON_IsArray(object));

    // Manually set upper bits (simulated non-lower-byte effect)
    // For demonstration, set some high bits while keeping lower byte 0 (not Array)
    object->type |= 0x1000; // arbitrary upper bit

    // Should still be false because lower byte remains non-array
    TEST_EXPECT_FALSE(cJSON_IsArray(object));

    // Now set lower byte to Array and ensure true
    object->type = (object->type & ~0xFF) | cJSON_Array;
    TEST_EXPECT_TRUE(cJSON_IsArray(object));

    cJSON_Delete(object);
}

// Driver to run all tests
static void run_all_tests() {
    test_IsArray_withNullptr();
    test_IsArray_withActualArray();
    test_IsArray_withNumber();
    test_IsArray_withManipulatedType();
    test_IsArray_upperBitsIgnored();
}

// Entry point
int main() {
    run_all_tests();

    if (g_tests_failed == 0) {
        std::cout << "All tests passed: " << g_tests_run << " tests." << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " tests failed out of " << g_tests_run << "." << std::endl;
        return 1;
    }
}