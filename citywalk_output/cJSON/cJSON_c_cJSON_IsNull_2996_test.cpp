// This test suite verifies the behavior of the cJSON_IsNull function from cJSON.c
// It is implemented in C++11 without any external testing framework (no GTest).
// The tests focus on the exact branches of the following logic:
// if (item == NULL) return false;
// return (item->type & 0xFF) == cJSON_NULL;
//
// Explanatory notes are provided above each test function.

/*
Domain/Test Design Considerations:
- Candidate keywords inferred from the focal method: item, NULL, type, cJSON_NULL, 0xFF, cJSON_bool.
- We exercise true/false outcomes by manipulating the type field of cJSON items.
- We use cJSON_CreateNull and cJSON_CreateNumber to create valid items and then directly mutate the type field to test edge cases (high bits set, but low byte matches cJSON_NULL).
- We keep tests non-terminating (each test reports PASS/FAIL and continues).
*/

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


extern "C" {
}

// Test 1: Ensure NULL input yields false from cJSON_IsNull
// This covers the first branch: if (item == NULL) { return false; }
bool test_IsNull_nullPointer() {
    bool passed = (cJSON_IsNull(NULL) == false);
    std::cout << "[Test] cJSON_IsNull(NULL) should return false: " 
              << (passed ? "PASS" : "FAIL") << std::endl;
    return passed;
}

// Test 2: Ensure a genuine cJSON_NULL item yields true
// This covers the second branch when item is non-NULL and lower byte equals cJSON_NULL
bool test_IsNull_typeExactNull() {
    cJSON* item = cJSON_CreateNull();
    if (item == NULL) {
        std::cout << "[Test] cJSON_IsNull on CreateNull failed to allocate: FAIL" << std::endl;
        return false;
    }
    bool result = (cJSON_IsNull(item) == true);
    std::cout << "[Test] cJSON_IsNull on exact NULL type should be true: " 
              << (result ? "PASS" : "FAIL") << std::endl;
    cJSON_Delete(item);
    return result;
}

// Test 3: Ensure high-bit set does not affect the lower-byte check
// This mutates the type to have additional high bits while preserving lower 8 bits (which should still be cJSON_NULL)
bool test_IsNull_typeWithHighBitsNull() {
    cJSON* item = cJSON_CreateNull();
    if (item == NULL) {
        std::cout << "[Test] cJSON_IsNull: allocation failed for high-bits test: FAIL" << std::endl;
        return false;
    }
    int original_type = item->type;
    // Set high bits (e.g., bit 12) while keeping lower byte unchanged
    item->type |= 0x1000;

    bool result = (cJSON_IsNull(item) == true);

    // Restore original type to keep object sane for deletion
    item->type = original_type;
    cJSON_Delete(item);

    std::cout << "[Test] cJSON_IsNull with high bits set but lower byte NULL should be true: " 
              << (result ? "PASS" : "FAIL") << std::endl;
    return result;
}

// Test 4: Ensure non-NULL types (e.g., Number) return false
// This covers the case where (item->type & 0xFF) != cJSON_NULL
bool test_IsNull_typeNonNull() {
    cJSON* item = cJSON_CreateNumber(1.23);
    if (item == NULL) {
        std::cout << "[Test] cJSON_IsNull: allocation failed for non-null type test: FAIL" << std::endl;
        return false;
    }
    bool result = (cJSON_IsNull(item) == false);
    std::cout << "[Test] cJSON_IsNull on Number type should be false: " 
              << (result ? "PASS" : "FAIL") << std::endl;
    cJSON_Delete(item);
    return result;
}

int main() {
    int failures = 0;

    // Run tests in sequence
    if (!test_IsNull_nullPointer()) ++failures;
    if (!test_IsNull_typeExactNull()) ++failures;
    if (!test_IsNull_typeWithHighBitsNull()) ++failures;
    if (!test_IsNull_typeNonNull()) ++failures;

    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Total test failures: " << failures << std::endl;
        return 1;
    }
}