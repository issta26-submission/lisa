// Test suite for cJSON_IsTrue function in cJSON.c
// This test suite is implemented in C++11 without any testing framework (no GoogleTest).
// It relies on the public C API provided by cJSON.h and uses a small, non-terminating
// assertion mechanism to maximize coverage while continuing execution.
//
// Notes:
// - The tests exercise true/false branches and NULL input.
// - A hacky but effective test is included to verify that only the lower 8 bits of the
//   type field are considered by cJSON_IsTrue (masking with 0xff).
// - Static helper functions within cJSON.c are not directly tested; only the public API
//   is exercised.
// - All tests are designed to compile in a C++11 environment with the C codebase.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// If cJSON.h provides C interfaces, ensure C linkage when including in C++
extern "C" {
}

// Lightweight, non-terminating test assertion mechanism
static int g_test_failures = 0;

#define TEST_OK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST_FAIL] " << msg << std::endl; \
        ++g_test_failures; \
    } else { \
        std::cout << "[TEST_PASS] " << msg << std::endl; \
    } \
} while (0)

#define TEST_EXPECT_TRUE(cond, msg) TEST_OK((cond), msg)
#define TEST_EXPECT_FALSE(cond, msg) TEST_OK(!(cond), msg)

//
// Step 2: Unit Test Generation
// Using knowledge of cJSON_IsTrue implementation:
//   - It returns false if item == NULL.
//   - Otherwise, it returns ((item->type & 0xff) == cJSON_True).
// We will cover true/false branches and NULL input, plus an additional
// test to ensure the lower 8 bits are what determine the result
// (upper bits should be ignored due to the 0xff mask).
//

// Test 1: NULL input should return false
static void test_IsTrue_with_null_input() {
    // Focal behavior: NULL pointer -> false
    cJSON_bool result = cJSON_IsTrue(NULL);
    TEST_EXPECT_FALSE(result, "cJSON_IsTrue(NULL) should be false");
}

// Test 2: True item should return true
static void test_IsTrue_with_true_item() {
    // Create a JSON true item via public API and verify IsTrue returns true
    cJSON *true_item = cJSON_CreateTrue();
    TEST_EXPECT_TRUE(cJSON_IsTrue(true_item), "cJSON_IsTrue should return true for a true item");
    cJSON_Delete(true_item);
}

// Test 3: Non-true items (e.g., false) should return false
static void test_IsTrue_with_false_item() {
    // Create a false item and verify IsTrue returns false
    cJSON *false_item = cJSON_CreateFalse();
    TEST_EXPECT_FALSE(cJSON_IsTrue(false_item), "cJSON_IsTrue should return false for a false item");
    cJSON_Delete(false_item);
}

// Test 4: Upper bits should be ignored; lower 8 bits determine the result
static void test_IsTrue_upper_bits_ignored() {
    // Create a true item
    cJSON *item = cJSON_CreateTrue();
    // Manipulate the type to set upper bits without changing lower 8 bits
    item->type |= 0x1000;   // set an upper bit
    // The result should still be true because (type & 0xff) == cJSON_True
    TEST_EXPECT_TRUE(cJSON_IsTrue(item), "cJSON_IsTrue should ignore upper bits and still be true");
    cJSON_Delete(item);
}

// Test 5: If we alter the lower 8 bits to something other than cJSON_True, IsTrue should be false
static void test_IsTrue_lower_bits_changed() {
    // Create a true item
    cJSON *item = cJSON_CreateTrue();
    // Change the lower 8 bits to 0 (which should not equal cJSON_True)
    item->type = (item->type & ~0xff); // clear lower 8 bits
    // Now effectively lower 8 bits are 0; IsTrue should return false
    TEST_EXPECT_FALSE(cJSON_IsTrue(item), "cJSON_IsTrue should return false when lower 8 bits are not cJSON_True");
    cJSON_Delete(item);
}

// Entry point for running all tests
int main() {
    std::cout << "Running tests for cJSON_IsTrue..." << std::endl;

    // Run individual test cases
    test_IsTrue_with_null_input();
    test_IsTrue_with_true_item();
    test_IsTrue_with_false_item();
    test_IsTrue_upper_bits_ignored();
    test_IsTrue_lower_bits_changed();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] " << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}