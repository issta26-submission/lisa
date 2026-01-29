// Unit test suite for cJSON_CreateDoubleArray (CJSON.c) using a lightweight, custom test harness.
// This avoids GoogleTest and relies solely on the C/C++ standard library plus the provided cJSON API.

#include <cmath>
#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


// Include C header for cJSON with C linkage when compiling as C++
extern "C" {
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Lightweight assertion macro: increments total, marks failure but does not abort execution
#define TS_ASSERT(cond, msg) do { \
    g_total_tests++; \
    if(!(cond)) { \
        g_failed_tests++; \
        fprintf(stderr, "ASSERT FAILED: %s\n", msg); \
    } \
} while(0)

// Test: negative count should return NULL
static void test_CreateDoubleArray_negative_count() {
    static double nums[1] = {1.0};
    cJSON *arr = cJSON_CreateDoubleArray(nums, -1);
    TS_ASSERT(arr == NULL, "cJSON_CreateDoubleArray should return NULL for negative count");
    if (arr) {
        cJSON_Delete(arr);
    }
}

// Test: NULL numbers pointer should return NULL (even if count > 0)
static void test_CreateDoubleArray_null_numbers() {
    cJSON *arr = cJSON_CreateDoubleArray(NULL, 5);
    TS_ASSERT(arr == NULL, "cJSON_CreateDoubleArray should return NULL when numbers is NULL");
    if (arr) {
        cJSON_Delete(arr);
    }
}

// Test: zero count should return a non-NULL empty array
static void test_CreateDoubleArray_zero_count() {
    static double dummy = 0.0;
    double *nums = &dummy;
    cJSON *arr = cJSON_CreateDoubleArray(nums, 0);
    TS_ASSERT(arr != NULL, "zero-count should return a non-NULL array");
    if (arr) {
        TS_ASSERT(cJSON_GetArraySize(arr) == 0, "array size should be 0 for zero elements");
        cJSON_Delete(arr);
    }
}

// Test: single element array contains the exact value and circular linkage
static void test_CreateDoubleArray_single_element() {
    double nums[1] = {3.5};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 1);
    TS_ASSERT(arr != NULL, "single-element array should not be NULL");
    if (arr) {
        TS_ASSERT(cJSON_GetArraySize(arr) == 1, "array size should be 1");
        cJSON *item = cJSON_GetArrayItem(arr, 0);
        TS_ASSERT(item != NULL, "first item should exist");
        if (item) {
            bool is_num = (cJSON_IsNumber(item) != 0);
            TS_ASSERT(is_num, "first item should be a number");
            if (is_num) {
                double val = cJSON_GetNumberValue(item);
                TS_ASSERT(std::fabs(val - 3.5) < 1e-12, "first item value should be 3.5");
            }
            // For a single element, its prev should point to itself (circular list behavior)
            TS_ASSERT(item->prev == item, "single element's prev should point to itself");
        }
        cJSON_Delete(arr);
    }
}

// Test: multiple elements keep order and first.prev points to last
static void test_CreateDoubleArray_multiple_elements() {
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    TS_ASSERT(arr != NULL, "array with three elements should not be NULL");
    if (arr) {
        TS_ASSERT(cJSON_GetArraySize(arr) == 3, "array size should be 3");
        for (int i = 0; i < 3; ++i) {
            cJSON *it = cJSON_GetArrayItem(arr, i);
            TS_ASSERT(it != NULL, "array item should exist");
            if (it) {
                bool is_num = (cJSON_IsNumber(it) != 0);
                TS_ASSERT(is_num, "array item should be a number");
                if (is_num) {
                    double expected = nums[i];
                    double actual = cJSON_GetNumberValue(it);
                    TS_ASSERT(std::fabs(actual - expected) < 1e-12, "array item value mismatch");
                }
            }
        }
        // Verify circular linkage: first element's prev points to the last element
        cJSON *first = cJSON_GetArrayItem(arr, 0);
        cJSON *last = cJSON_GetArrayItem(arr, 2);
        if (first && last) {
            TS_ASSERT(first->prev == last, "first element's prev should point to the last element");
        }
        cJSON_Delete(arr);
    }
}

// Helper to print final summary and exit status
static void print_summary() {
    int passed = g_total_tests - g_failed_tests;
    printf("Test summary: total=%d, passed=%d, failed=%d\n", g_total_tests, passed, g_failed_tests);
}

// Main function to run tests
int main() {
    printf("Starting cJSON_CreateDoubleArray unit tests (CJSON_CreateDoubleArray) ...\n");

    test_CreateDoubleArray_negative_count();
    test_CreateDoubleArray_null_numbers();
    test_CreateDoubleArray_zero_count();
    test_CreateDoubleArray_single_element();
    test_CreateDoubleArray_multiple_elements();

    print_summary();

    return (g_failed_tests > 0) ? 1 : 0;
}