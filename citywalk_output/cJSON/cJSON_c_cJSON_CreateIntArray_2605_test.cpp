// This test suite targets the function
// CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count)
// from the cJSON.c / cJSON.h project. It uses a small in-file test framework
// (no Google Test) and runs under C++11 as requested.
// The tests focus on ensuring correct behavior for valid inputs, and proper
// NULL handling for invalid inputs as per the function's predicates.

#include <locale.h>
#include <math.h>
#include <cstring>
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


// Bring C code into C++ compilation unit
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

static void report_assertion(bool condition, const char* message, const char* file, int line)
{
    ++g_total_asserts;
    if (!condition) {
        ++g_failed_asserts;
        std::cerr << "ASSERTION FAILED: " << (message ? message : "")
                  << " (at " << file << ":" << line << ")\n";
    }
}

#define EXPECT(cond, msg) report_assertion((cond), (msg), __FILE__, __LINE__)

// Test 1: Null numbers pointer should yield NULL regardless of count
void test_CreateIntArray_NullNumbers_ReturnNull()
{
    // Domain knowledge: count can be any non-negative number; numbers NULL -> NULL
    int sample_count = 3;
    cJSON *result = cJSON_CreateIntArray(nullptr, sample_count);
    EXPECT(result == nullptr, "Expected NULL when numbers is NULL");
    // No allocation expected; nothing to clean up
    (void)result; // suppress unused warning if compiled differently
}

// Test 2: Negative count should yield NULL even with a valid numbers array
void test_CreateIntArray_NegativeCount_ReturnNull()
{
    int numbers[] = {1, 2, 3};
    cJSON *result = cJSON_CreateIntArray(numbers, -5);
    EXPECT(result == nullptr, "Expected NULL when count < 0");
    (void)result;
}

// Test 3: Zero count should return a valid (empty) array
void test_CreateIntArray_ZeroCount_ReturnEmptyArray()
{
    int numbers[] = {10, 20, 30};
    cJSON *result = cJSON_CreateIntArray(numbers, 0);
    EXPECT(result != nullptr, "Expected non-NULL for count == 0 (empty array)");
    if (result != nullptr) {
        int size = cJSON_GetArraySize(result);
        EXPECT(size == 0, "Expected array size 0 for count 0");
        cJSON_Delete(result);
    }
}

// Test 4: Single element array should contain exactly that value
void test_CreateIntArray_SingleElement()
{
    int numbers[] = {42};
    cJSON *result = cJSON_CreateIntArray(numbers, 1);
    EXPECT(result != nullptr, "Expected non-NULL for single element");
    if (result != nullptr) {
        int size = cJSON_GetArraySize(result);
        EXPECT(size == 1, "Expected array size 1 for single element");
        cJSON *item = cJSON_GetArrayItem(result, 0);
        EXPECT(item != nullptr, "Expected first item to exist");
        if (item != nullptr) {
            double value = cJSON_GetNumberValue(item);
            EXPECT(value == 42.0, "Expected first element value to be 42");
        }
        cJSON_Delete(result);
    }
}

// Test 5: Multiple elements should preserve order and values
void test_CreateIntArray_MultipleElements()
{
    const int numbers[] = {1, 2, 3, -4, 0};
    const int count = 5;
    cJSON *result = cJSON_CreateIntArray(numbers, count);
    EXPECT(result != nullptr, "Expected non-NULL for multiple elements");
    if (result != nullptr) {
        int size = cJSON_GetArraySize(result);
        EXPECT(size == count, "Expected array size equal to input count");
        for (int i = 0; i < count; ++i) {
            cJSON *item = cJSON_GetArrayItem(result, i);
            EXPECT(item != nullptr, "Expected item to exist at index");
            if (item != nullptr) {
                double value = cJSON_GetNumberValue(item);
                EXPECT(value == static_cast<double>(numbers[i]),
                       "Element value mismatch at index " && std::to_string(i));
            }
        }
        cJSON_Delete(result);
    }
}

// Simple helper to run all tests
void run_all_tests()
{
    test_CreateIntArray_NullNumbers_ReturnNull();
    test_CreateIntArray_NegativeCount_ReturnNull();
    test_CreateIntArray_ZeroCount_ReturnEmptyArray();
    test_CreateIntArray_SingleElement();
    test_CreateIntArray_MultipleElements();
}

// Entry point
int main()
{
    run_all_tests();

    std::cout << "Total assertions: " << g_total_asserts
              << ", Failures: " << g_failed_asserts << "\n";

    if (g_failed_asserts == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}