// Unit test suite for CJSON_CreateFalse (cJSON_CreateFalse) using C++11.
// Purpose: verify that cJSON_CreateFalse correctly creates a JSON boolean item
// of type false, and interacts properly with related API functions.
// No GTest is used; a lightweight, non-terminating test harness is provided.
//
// Notes aligned with the required steps:
// - Step 1 (Program Understanding): The focal function creates a new cJSON item via
//   cJSON_New_Item(&global_hooks) and, if successful, sets item->type = cJSON_False.
// - Step 2 (Unit Test Generation): Tests focus on the public API: cJSON_CreateFalse,
//   and cross-checks with related public APIs (e.g., cJSON_CreateBool(false), cJSON_IsBool,
//   cJSON_IsFalse). We avoid private/static internals and rely on the public interface.
// - Step 3 (Test Case Refinement): Use a minimal harness that records failures without
//   aborting, ensuring broader code coverage. Tests run from main() without external
//   testing frameworks. Assertions are non-terminating to maximize execution paths.
// 
// Build note: Compile with a C/C++ compiler that can link against cJSON.c and cJSON.h
// in the project. Example (adjust as needed):
//   g++ -std=c++11 -I. test_cjson_create_false.cpp cJSON.c -o test_cjson_create_false

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

// Lightweight, non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Record the result of a test without terminating execution
static void record_test(const std::string& test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: Basic creation properties of cJSON_CreateFalse
// - Ensures a non-null item is returned
// - Ensures the item's type is cJSON_False
// - Ensures that the item is not pre-populated with a string or child
// - Verifies boolean-specific predicates (IsBool, IsFalse)
static void test_CreateFalse_basic_properties() {
    // Explain test intent
    // This test validates the primary success path of cJSON_CreateFalse:
    // a newly allocated item with type set to cJSON_False.
    cJSON *item = cJSON_CreateFalse();

    bool ok = (item != nullptr) &&
              (item->type == cJSON_False) &&
              (item->child == nullptr) &&            // No children for a boolean
              (item->string == nullptr || item->string == NULL) && // No string for a boolean
              cJSON_IsBool(item) &&                     // Public API: is boolean
              cJSON_IsFalse(item) &&                    // Public API: is specifically false
              !cJSON_IsTrue(item);                      // Public API: is not true

    // Cleanup allocated memory if creation succeeded
    if (item) {
        cJSON_Delete(item);
    }

    record_test("test_CreateFalse_basic_properties", ok);
}

// Test 2: Consistency with cJSON_CreateBool(false)
// - Verifies that creating a false boolean via cJSON_CreateFalse matches behavior
//   and type with cJSON_CreateBool(false)
static void test_CreateFalse_vs_CreateBool_false_consistency() {
    cJSON *false_from_create_false = cJSON_CreateFalse();
    cJSON *false_from_create_bool = cJSON_CreateBool(false);

    bool ok = true;
    if (false_from_create_false == nullptr || false_from_create_bool == nullptr) {
        ok = false; // both should be non-null to compare meaningfully
    } else {
        // Both should represent a boolean false
        ok = (false_from_create_false->type == cJSON_False) &&
             (false_from_create_bool->type == cJSON_False) &&
             cJSON_IsBool(false_from_create_false) &&
             cJSON_IsBool(false_from_create_bool) &&
             cJSON_IsFalse(false_from_create_false) &&
             cJSON_IsFalse(false_from_create_bool);
    }

    // Cleanup
    if (false_from_create_false) cJSON_Delete(false_from_create_false);
    if (false_from_create_bool) cJSON_Delete(false_from_create_bool);

    record_test("test_CreateFalse_vs_CreateBool_false_consistency", ok);
}

// Main function to run the tests
int main() {
    // Step 2/3 integration: run a small, self-contained test suite without external frameworks
    test_CreateFalse_basic_properties();                 // Step 2: cover true path + basic checks
    test_CreateFalse_vs_CreateBool_false_consistency(); // Step 2: cross-validate against related API

    // Summary of test results
    std::cout << "\nTest Summary: "
              << (g_total_tests - g_failed_tests) << "/" << g_total_tests
              << " passed; " << g_failed_tests << " failed.\n";

    // Non-zero exit code on failure to aid automation
    return (g_failed_tests == 0) ? 0 : 1;
} 

// End of test suite for cJSON_CreateFalse