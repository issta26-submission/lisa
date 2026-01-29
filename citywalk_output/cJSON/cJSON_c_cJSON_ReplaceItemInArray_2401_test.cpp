/*
Unit Test Suite for cJSON_ReplaceItemInArray (CJSON C API)
Provider: C++11 test harness (no Google Test, no GMock)
Target function: cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem)

Step 1 - Program Understanding (Key Dependent Components)
- Core function under test: cJSON_ReplaceItemInArray
- Dependencies (from FOCAL_CLASS_DEP): 
  - get_array_item(array, index) - helper that fetches an item by index in a cJSON array
  - cJSON_ReplaceItemViaPointer(parent, item, replacement) - replaces a given item with a replacement in the container
  - cJSON_CreateArray, cJSON_AddItemToArray - to build test arrays
  - cJSON_Delete, cJSON_GetArrayItem, cJSON_GetArraySize, cJSON_IsNumber, cJSON_GetNumberValue - for verification and cleanup
- Observations: 
  - If which < 0, function returns false immediately.
  - Otherwise, it delegates to cJSON_ReplaceItemViaPointer with the target item found by get_array_item(array, (size_t)which)

Potential edge cases to cover:
- which < 0 => should return false (no modification)
- which within bounds => should replace and return true
- which out of bounds => get_array_item returns NULL; behavior depends on cJSON_ReplaceItemViaPointer handling; we expect false and no modification
- Ensure proper memory management to avoid leaks during tests

Candidate Keywords (core components to drive test scenarios): cJSON_ReplaceItemInArray, which, get_array_item, cJSON_ReplaceItemViaPointer, cJSON_AddItemToArray, cJSON_GetArrayItem, cJSON_GetArraySize, cJSON_CreateNumber, cJSON_CreateArray, cJSON_Delete

Step 2 - Unit Test Generation (Plan)
Create a small suite of tests in C++11 that uses the C API of cJSON (no GTest). Tests will:
- Build simple arrays (with numbers) using cJSON_CreateArray and cJSON_CreateNumber, then replace by index and verify via array accessors.
- Validate true/false return values for each scenario.
- Validate array content/size after operations.
- Ensure no leaks by proper cleanup (delete unused items and the array).

Test cases to implement:
1) test_replace_first_item_basic
   - Build array [1, 2, 3], replace index 0 with 10
   - Expect true, element 0 becomes 10, array size remains 3
   - Cleanup: delete old first item (since it’s detached after replacement) and array (which will delete replacement)

2) test_replace_negative_index_returns_false
   - Build array [1, 2], attempt to replace index -1 with 99
   - Expect false, array remains [1, 2], size 2
   - Cleanup: delete newitem (not inserted)

3) test_replace_out_of_bounds_returns_false
   - Build array [1, 2, 3], attempt to replace index 5 with 99
   - Expect false, array remains [1, 2, 3], size 3
   - Cleanup: delete newitem

4) test_replace_last_item_basic
   - Build array [4, 5, 6], replace index 2 with 7
   - Expect true, element 2 becomes 7, size 3
   - Cleanup: delete old last item and array (replacement is owned by array)

Step 3 - Test Case Refinement
- Use non-terminating checks (return status per test, do not exit on first failure).
- Access and verify by content (GetNumberValue) rather than pointer equality where possible; however, for replacement validation, pointer equality with GetArrayItem is a robust check.
- Ensure static/global state is not relied upon; tests operate on freshly created arrays.
- All tests compile under C++11; tests use only C++ standard library for harness output.

Below is the standalone test harness implementing the plan. It uses a minimal in-file test framework (no GTest) and prints per-test results. It relies on the public C API of cJSON (extern "C" linkage where needed).

Important: Ensure your build links against the cJSON library and that "cJSON.h" is accessible in include path.

Code starts here:

*/

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <functional>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


extern "C" {
}

// Lightweight test harness (non-terminating, does not abort on failure)
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

// Helper to run a test and capture result
static TestResult run_test(const std::string& test_name, const std::function<void(TestResult&)>& runner) {
    TestResult tr;
    tr.name = test_name;
    tr.passed = true;
    tr.message.clear();
    runner(tr);
    return tr;
}

// Test 1: Replace first item in a 3-element array
static void test_replace_first_item_basic(TestResult& tr) {
    // Setup: create array [1,2,3]
    cJSON *array = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateNumber(1);
    cJSON *item1 = cJSON_CreateNumber(2);
    cJSON *item2 = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(array, item0);
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);

    // Keep pointer to old first item to free it later (after replacement it's detached)
    cJSON *old_first = item0;

    // Act: replace index 0 with 10
    cJSON *replacement = cJSON_CreateNumber(10);
    cJSON_bool result = cJSON_ReplaceItemInArray(array, 0, replacement);

    // Assert: replacement happened and return is true
    if (!result) {
        tr.passed = false;
        tr.message = "test_replace_first_item_basic: cJSON_ReplaceItemInArray returned false for valid index 0";
        cJSON_Delete(array);
        // free replacement since not owned by array in error path
        cJSON_Delete(replacement);
        cJSON_Delete(old_first); // though should be detached, safe to cleanup
        return;
    }
    // Verify content at index 0 is the replacement
    cJSON *at0 = cJSON_GetArrayItem(array, 0);
    if (at0 != replacement) {
        tr.passed = false;
        tr.message = "test_replace_first_item_basic: The item at index 0 is not the replacement pointer.";
        cJSON_Delete(array);
        cJSON_Delete(old_first);
        // do not delete replacement here; array owns it
        return;
    }

    // Verify numeric value of new first element
    if (!cJSON_IsNumber(at0) || cJSON_GetNumberValue(at0) != 10.0) {
        tr.passed = false;
        tr.message = "test_replace_first_item_basic: Replacement item value is not 10.";
        // cleanup
        cJSON_Delete(array);
        cJSON_Delete(old_first);
        return;
    }

    // Cleanup: old first is detached; array owns replacement
    cJSON_Delete(old_first); // safe to delete detached old item
    cJSON_Delete(array);
    // No explicit delete for replacement; deleted with array
    tr.passed = true;
}

// Test 2: Replacement with negative index should fail and not modify the array
static void test_replace_negative_index_returns_false(TestResult& tr) {
    // Setup: create array [1, 2]
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateNumber(1);
    cJSON *b = cJSON_CreateNumber(2);
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);

    // New item to attempt replacement with
    cJSON *newitem = cJSON_CreateNumber(99);

    // Act
    cJSON_bool result = cJSON_ReplaceItemInArray(array, -1, newitem);

    // Assert: should be false
    if (result) {
        tr.passed = false;
        tr.message = "test_replace_negative_index_returns_false: Expected false for negative index, got true.";
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return;
    }

    // Ensure array unchanged: size 2 and contents 1, 2
    int size = cJSON_GetArraySize(array);
    if (size != 2) {
        tr.passed = false;
        tr.message = "test_replace_negative_index_returns_false: Array size changed after failed replacement.";
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return;
    }
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    if (!first || !second ||
        !cJSON_IsNumber(first) || cJSON_GetNumberValue(first) != 1.0 ||
        !cJSON_IsNumber(second) || cJSON_GetNumberValue(second) != 2.0) {
        tr.passed = false;
        tr.message = "test_replace_negative_index_returns_false: Array contents changed after failed replacement.";
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return;
    }

    // Cleanup
    cJSON_Delete(array);
    cJSON_Delete(newitem);
    tr.passed = true;
}

// Test 3: Replacement with an out-of-bounds index should fail and not modify the array
static void test_replace_out_of_bounds_returns_false(TestResult& tr) {
    // Setup: create array [1, 2, 3]
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1);
    cJSON *n2 = cJSON_CreateNumber(2);
    cJSON *n3 = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    cJSON *newitem = cJSON_CreateNumber(42);

    // Act: index 5 (out of bounds)
    cJSON_bool result = cJSON_ReplaceItemInArray(array, 5, newitem);

    // Assert: should be false
    if (result) {
        tr.passed = false;
        tr.message = "test_replace_out_of_bounds_returns_false: Expected false for out-of-bounds index, got true.";
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return;
    }

    // Ensure array unchanged: size 3 and contents 1,2,3
    int size = cJSON_GetArraySize(array);
    if (size != 3) {
        tr.passed = false;
        tr.message = "test_replace_out_of_bounds_returns_false: Array size changed after failed replacement.";
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return;
    }
    if (!cJSON_IsNumber(cJSON_GetArrayItem(array,0)) || cJSON_GetNumberValue(cJSON_GetArrayItem(array,0)) != 1.0 ||
        !cJSON_IsNumber(cJSON_GetArrayItem(array,1)) || cJSON_GetNumberValue(cJSON_GetArrayItem(array,1)) != 2.0 ||
        !cJSON_IsNumber(cJSON_GetArrayItem(array,2)) || cJSON_GetNumberValue(cJSON_GetArrayItem(array,2)) != 3.0) {
        tr.passed = false;
        tr.message = "test_replace_out_of_bounds_returns_false: Array contents changed after failed replacement.";
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return;
    }

    // Cleanup
    cJSON_Delete(array);
    cJSON_Delete(newitem);
    tr.passed = true;
}

// Test 4: Replace last item in a 3-element array (additional coverage)
static void test_replace_last_item_basic(TestResult& tr) {
    // Setup: create array [4, 5, 6]
    cJSON *array = cJSON_CreateArray();
    cJSON *a = cJSON_CreateNumber(4);
    cJSON *b = cJSON_CreateNumber(5);
    cJSON *c = cJSON_CreateNumber(6);
    cJSON_AddItemToArray(array, a);
    cJSON_AddItemToArray(array, b);
    cJSON_AddItemToArray(array, c);

    cJSON *old_last = c; // last item
    cJSON *replacement = cJSON_CreateNumber(9);

    // Act
    cJSON_bool result = cJSON_ReplaceItemInArray(array, 2, replacement);

    // Assert
    if (!result) {
        tr.passed = false;
        tr.message = "test_replace_last_item_basic: Replacement failed for last index 2.";
        cJSON_Delete(array);
        cJSON_Delete(replacement);
        cJSON_Delete(old_last);
        return;
    }

    cJSON *at2 = cJSON_GetArrayItem(array, 2);
    if (at2 != replacement) {
        tr.passed = false;
        tr.message = "test_replace_last_item_basic: Replacement item at index 2 is not the expected pointer.";
        cJSON_Delete(array);
        cJSON_Delete(replacement);
        cJSON_Delete(old_last);
        return;
    }
    if (!cJSON_IsNumber(at2) || cJSON_GetNumberValue(at2) != 9.0) {
        tr.passed = false;
        tr.message = "test_replace_last_item_basic: Replacement item does not have value 9.";
        cJSON_Delete(array);
        cJSON_Delete(replacement);
        cJSON_Delete(old_last);
        return;
    }

    // Cleanup: detach old_last is no longer in array
    cJSON_Delete(old_last);
    cJSON_Delete(array);
    // replacement is owned by array and freed with array
    tr.passed = true;
}

// Main: run all tests and report
int main() {
    std::vector<TestResult> results;

    results.push_back(run_test("test_replace_first_item_basic", test_replace_first_item_basic));
    results.back().name = "test_replace_first_item_basic";
    // Actually the above call returns a TestResult; but we prepared a function expecting a lambda.
    // To adhere to our design, we should instead invoke via lambdas. We'll reconstruct properly below.

    // Redo proper invocation with lambdas for accurate results:
    results.clear();

    results.push_back(run_test("test_replace_first_item_basic",
        [](TestResult& tr){
            test_replace_first_item_basic(tr);
        }
    ));

    results.push_back(run_test("test_replace_negative_index_returns_false",
        [](TestResult& tr){
            test_replace_negative_index_returns_false(tr);
        }
    ));

    results.push_back(run_test("test_replace_out_of_bounds_returns_false",
        [](TestResult& tr){
            test_replace_out_of_bounds_returns_false(tr);
        }
    ));

    results.push_back(run_test("test_replace_last_item_basic",
        [](TestResult& tr){
            test_replace_last_item_basic(tr);
        }
    ));

    // Report summary
    int passed = 0;
    for (const auto &r : results) {
        if (r.passed) {
            std::cout << "[PASS] " << r.name << std::endl;
            ++passed;
        } else {
            std::cout << "[FAIL] " << r.name << " - " << r.message << std::endl;
        }
    }
    std::cout << "Summary: " << passed << "/" << results.size() << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == (int)results.size()) ? 0 : 1;
}

/*
Notes for test maintenance:
- These tests rely on the public cJSON API. They do not access any private/static file-scope helpers.
- Memory management follows the cJSON ownership model: arrays own their items. When an item is replaced, the old item is detached; it should be freed by the caller if not re-used. Tests reflect this by freeing old items when appropriate.
- The tests avoid terminating on first failure and provide per-test pass/fail messages to aid debugging.
- If you extend cJSON with more edge cases (e.g., non-numeric replacements, NULL replacements, or larger arrays), mirror the above pattern to ensure coverage of true/false branches and boundary conditions.
*/