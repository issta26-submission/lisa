// Unit test suite for cJSON_CreateFloatArray (C API) using C++11, no GTest.
// This test harness exercises the focal method and its dependent
// behaviors while avoiding termination on assertion failures.
// All tests use simple, non-terminating CHECK macros to accumulate results
// and print a summary at the end.
//
// The tests focus on: normal usage, edge cases (zero-length), invalid inputs
// (negative count, NULL numbers), and internal linkage behavior (first.prev == last).
//
// Assumptions:
// - The project exposes a C-compatible API via cJSON.h with extern "C" when used from C++.
// - Memory management via cJSON_Delete is available to cleanup created JSON objects.

#include <cmath>
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


// Include the cJSON header. Adjust the path if needed for your project setup.

// Simple non-terminating test harness

static int g_failures = 0;
static std::vector<std::string> g_messages;

// Non-terminating assertion macro: records failure but continues execution.
#define CHECK(cond, desc) do { \
    if (!(cond)) { \
        ++g_failures; \
        g_messages.push_back(std::string("FAIL: ").append(desc)); \
    } else { \
        g_messages.push_back(std::string("PASS: ").append(desc)); \
    } \
} while(0)

// Test 1: Normal usage with 3 numbers.
// Verifies array size, element types, value accuracy, and linkage (first->prev points to last).
static void test_CreateFloatArray_NormalThree() {
    const float nums[3] = {1.0f, 2.0f, 3.0f};
    cJSON *arr = cJSON_CreateFloatArray(nums, 3);
    CHECK(arr != NULL, "CreateFloatArray returns non-NULL for valid input (3 numbers)");
    if (arr) {
        int size = cJSON_GetArraySize(arr);
        CHECK(size == 3, "Array size should be 3");

        for (int i = 0; i < 3; ++i) {
            cJSON *elem = cJSON_GetArrayItem(arr, i);
            CHECK(elem != NULL, "Element should exist");
            if (elem) {
                CHECK(cJSON_IsNumber(elem), "Element should be a number");
                double val = cJSON_GetNumberValue(elem);
                double expected = static_cast<double>(nums[i]);
                CHECK(std::abs(val - expected) < 1e-6, "Element value matches input within tolerance");
            }
        }

        // Check linkage: first element's prev should point to the last element.
        cJSON *first = cJSON_GetArrayItem(arr, 0);
        cJSON *last_expected = cJSON_GetArrayItem(arr, size - 1);
        CHECK(first != NULL && last_expected != NULL, "First and last items exist for linkage test");
        if (first && last_expected) {
            CHECK(first->prev == last_expected, "First item's prev should point to last item");
        }

        cJSON_Delete(arr);
    }
}

// Test 2: Zero-length array (count = 0) with non-NULL numbers pointer.
// Expect a non-NULL array object with size 0.
static void test_CreateFloatArray_ZeroLength() {
    const float nums[1] = {0.0f}; // non-NULL pointer; content unused for zero length
    cJSON *arr = cJSON_CreateFloatArray(nums, 0);
    CHECK(arr != NULL, "CreateFloatArray returns non-NULL for zero count with non-NULL numbers pointer");
    if (arr) {
        int size = cJSON_GetArraySize(arr);
        CHECK(size == 0, "Empty array should have size 0");
        cJSON_Delete(arr);
    }
}

// Test 3: Negative count should return NULL.
static void test_CreateFloatArray_NegativeCount() {
    const float nums[5] = {0, 0, 0, 0, 0};
    cJSON *arr = cJSON_CreateFloatArray(nums, -1);
    CHECK(arr == NULL, "CreateFloatArray returns NULL for negative count");
}

// Test 4: NULL numbers pointer should return NULL (even if count > 0).
static void test_CreateFloatArray_NullNumbers() {
    const float *null_ptr = NULL;
    cJSON *arr = cJSON_CreateFloatArray(null_ptr, 3);
    CHECK(arr == NULL, "CreateFloatArray returns NULL when numbers pointer is NULL");
}

// Test 5: Single-element array linkage test (prev should point to itself).
static void test_CreateFloatArray_SingleElementPrevPointer() {
    const float nums[1] = {7.25f};
    cJSON *arr = cJSON_CreateFloatArray(nums, 1);
    if (arr) {
        cJSON *elem = cJSON_GetArrayItem(arr, 0);
        CHECK(elem != NULL, "Single-element array should have one element");
        if (elem) {
            // In single-element case, first element's prev should be the element itself.
            CHECK(elem->prev == elem, "Single-element prev should point to itself");
        }
        cJSON_Delete(arr);
    } else {
        CHECK(false, "Single-element array creation should not be NULL");
    }
}

// Driver to run all tests and print results
int main() {
    // Run tests
    test_CreateFloatArray_NormalThree();
    test_CreateFloatArray_ZeroLength();
    test_CreateFloatArray_NegativeCount();
    test_CreateFloatArray_NullNumbers();
    test_CreateFloatArray_SingleElementPrevPointer();

    // Output test results
    std::cout << "cJSON_CreateFloatArray test results:\n";
    for (const auto &msg : g_messages) {
        std::cout << msg << "\n";
    }
    std::cout << "Total failures: " << g_failures << "\n";

    // Cleanup exit code: 0 if all tests passed, non-zero otherwise
    return g_failures ? 1 : 0;
}