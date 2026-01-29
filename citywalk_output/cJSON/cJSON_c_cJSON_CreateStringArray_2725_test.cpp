/*
Unit Test Suite for the focal method:
  cJSON_CreateStringArray(const char *const *strings, int count)

Step 1 (Program Understanding) highlights:
- Core action: build a cJSON array where each element is a string created from strings[i].
- Handles edge cases:
  - If count < 0 or strings == NULL -> return NULL
  - For each element, if cJSON_CreateString(strings[i]) fails -> delete the array and return NULL
  - On success, first element is the array's child; subsequent elements are linked via suffix_object
  - At end, if array non-empty, set first item's prev to last item (circular linkage)
- Key dependencies:
  -> cJSON_CreateArray
  -> cJSON_CreateString
  -> cJSON_Delete
  -> cJSON_GetArraySize
  -> cJSON_GetArrayItem
  -> cJSON_GetStringValue
  -> suffix_object (internal helper; used by implementation)
  -> The test should verify true/false branches by exercising negatives, zero size, and normal paths.

Candidate Keywords (mapped to the method's core dependent components):
- cJSON_CreateArray
- cJSON_CreateString
- cJSON_Delete
- cJSON_GetArraySize
- cJSON_GetArrayItem
- cJSON_GetStringValue
- suffix_object
- NULL checks (strings == NULL)
- count negative handling
- zero-length array behavior
- memory cleanup after success/failure

Step 2 (Unit Test Generation) - Focused test plan for cJSON_CreateStringArray:
Tests to cover:
1) Negative count with non-NULL strings -> expect NULL (path where count < 0 short-circuits).
2) NULL strings pointer with non-negative count -> expect NULL (path where strings == NULL short-circuits).
3) Zero count with non-NULL strings -> expect an empty array (size 0) and non-NULL return.
4) Valid normal path with multiple strings -> expect proper array size, and each element's string value matches input.
5) A string array containing a NULL entry -> expect NULL (to exercise early failure cleanup path when a string cannot be created).
Additional notes:
- Do not rely on private/internal behavior beyond public API. Use public helpers: cJSON_GetArraySize, cJSON_GetArrayItem, cJSON_GetStringValue.
- Ensure memory cleanup with cJSON_Delete where applicable to avoid leaks.
- Tests are implemented in C++11, using a lightweight, non-GTest custom harness.

Step 3 (Test Case Refinement) - Domain knowledge considerations:
- Use only the C++ standard library and the provided cJSON interface (no mocking).
- Distinguish content equality (string values) versus address equality when validating results.
- Access to static/internal helpers is not needed; rely on public API.
- Test harness prints per-test results and aggregates summary without terminating on first failure.

Now, the code: a self-contained C++11 test suite (no GTest) for the focal method, with explanatory comments on each test case.

*/

// Note: Compile with C++11 and link against the cJSON library/object file that provides cJSON.h/cJSON.c
// Example (depending on your build system): g++ -std=c++11 -I. test_cjson_stringarray.cpp cJSON.o -o test_cjson_stringarray

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Include the cJSON public header. If your header uses extern "C" guards, this is safe in C++.
extern "C" {
}

// Simple, non-terminating test harness (no GTest). Tracks totals and failures.
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to print per-test result (keeps test output readable in CI logs)
static void print_test_result(const char* test_name, bool passed, const char* details = nullptr) {
    if (passed) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s%s\n", test_name, details ? details : "");
    }
}

// Test 1: Negative count with non-NULL strings should return NULL
static bool test_CreateStringArray_negative_count_returns_NULL() {
    const char* sample[] = { "alpha", "beta" };
    g_total_tests++;

    cJSON* result = cJSON_CreateStringArray(sample, -1);
    bool pass = (result == NULL);
    if (result != NULL) {
        // Clean up if something went wrong
        cJSON_Delete(result);
    }
    print_test_result("CreateStringArray_negative_count_returns_NULL", pass);
    if (!pass) {
        printf("  Expected NULL when count < 0. Got non-NULL.\n");
        g_failed_tests++;
    }
    return pass;
}

// Test 2: NULL strings pointer with non-negative count should return NULL
static bool test_CreateStringArray_null_strings_returns_NULL() {
    g_total_tests++;
    cJSON* result = cJSON_CreateStringArray(NULL, 2);
    bool pass = (result == NULL);
    if (result != NULL) {
        cJSON_Delete(result);
    }
    print_test_result("CreateStringArray_null_strings_returns_NULL", pass);
    if (!pass) {
        printf("  Expected NULL when strings == NULL. Got non-NULL.\n");
        g_failed_tests++;
    }
    return pass;
}

// Test 3: Zero count with non-NULL strings returns an empty array (size 0)
static bool test_CreateStringArray_zero_count_empty_array() {
    const char* sample[] = { "one", "two" };
    g_total_tests++;

    cJSON* result = cJSON_CreateStringArray(sample, 0);
    bool pass = (result != NULL) && (cJSON_GetArraySize(result) == 0);
    if (result != NULL) {
        cJSON_Delete(result);
    }
    print_test_result("CreateStringArray_zero_count_empty_array", pass);
    if (!pass) {
        if (result == NULL) {
            printf("  Expected non-NULL array with size 0. Got NULL.\n");
        } else {
            printf("  Expected size 0, got %d.\n", cJSON_GetArraySize(result));
        }
        g_failed_tests++;
    }
    return pass;
}

// Test 4: Valid normal path - multiple strings -> verify size and contents
static bool test_CreateStringArray_valid_strings_contents() {
    const char* strings[] = { "apple", "banana", "cherry" };
    const int count = 3;
    g_total_tests++;

    cJSON* arr = cJSON_CreateStringArray(strings, count);
    bool pass = true;
    if (arr == NULL) {
        pass = false;
        printf("  Expected non-NULL array for valid input, got NULL.\n");
    } else {
        int size = cJSON_GetArraySize(arr);
        if (size != count) {
            pass = false;
            printf("  Expected size %d, got %d.\n", count, size);
        } else {
            for (int i = 0; i < count; ++i) {
                cJSON* item = cJSON_GetArrayItem(arr, i);
                const char* val = cJSON_GetStringValue(item);
                if (val == NULL || std::strcmp(val, strings[i]) != 0) {
                    pass = false;
                    printf("  Mismatch at index %d: expected \"%s\", got \"%s\".\n",
                           i, strings[i], val ? val : "NULL");
                    break;
                }
            }
        }
        cJSON_Delete(arr);
    }

    print_test_result("CreateStringArray_valid_strings_contents", pass);
    if (!pass) {
        g_failed_tests++;
    }
    return pass;
}

// Test 5: Presence of NULL within strings array should cause NULL return (simulate failure path)
static bool test_CreateStringArray_null_entry_in_strings_causes_NULL() {
    const char* strings[] = { "first", NULL, "third" };
    const int count = 3;
    g_total_tests++;

    cJSON* result = cJSON_CreateStringArray(strings, count);
    bool pass = (result == NULL);
    if (result != NULL) {
        cJSON_Delete(result);
    }
    print_test_result("CreateStringArray_null_entry_in_strings_causes_NULL", pass);
    if (!pass) {
        printf("  Expected NULL when one entry in strings cannot be created.\n");
        g_failed_tests++;
    }
    return pass;
}

// Main harness to run all tests
int main() {
    printf("Starting test suite for cJSON_CreateStringArray (C API) using C++11 test harness.\n");

    bool ok;
    ok = test_CreateStringArray_negative_count_returns_NULL();
    // If any test fails, record but continue running others to maximize coverage.

    ok = test_CreateStringArray_null_strings_returns_NULL();

    ok = test_CreateStringArray_zero_count_empty_array();

    ok = test_CreateStringArray_valid_strings_contents();

    ok = test_CreateStringArray_null_entry_in_strings_causes_NULL();

    // Summary
    printf("\nTest Summary: %d run, %d failed\n", g_total_tests, g_failed_tests);
    if (g_failed_tests == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed. Review the detailed messages above.\n");
    }

    // Return non-zero if any test failed, to integrate with CI systems.
    return g_failed_tests;
}

/*
Notes for maintainers and reviewers:
- This test suite avoids GTest by using a tiny, custom harness with per-test pass/fail reporting.
- Each test is designed to cover true and false branches in cJSON_CreateStringArray:
  - Negative count and NULL strings paths (short-circuit returns).
  - Zero-count path yielding an empty array.
  - Normal path building a valid array and verifying element content via GetArraySize/GetArrayItem/GetStringValue.
  - Failure path when a string cannot be created (simulated by NULL entry in inputs).
- Assertions are non-terminating; tests run to completion and the summary reports all outcomes.
- Static/internal helpers in cJSON (like suffix_object) are not directly invoked by test code; we rely on public API behavior.
- If you need more coverage, you can add tests for:
  - Very large counts (stress path) to ensure no overflow issues (within reasonable bounds).
  - Strings containing special characters to verify string integrity (escaping is handled by cJSON itself).
- This file assumes cJSON.h/cJSON.c are available in the build and that cJSON_GetArraySize, cJSON_GetArrayItem, and cJSON_GetStringValue provide the public API required for validation.
*/