// Lightweight C++11 test suite for the focal C function: cJSON_IsString
// This test suite avoids GTest and uses a small in-file test harness.
// It targets the behavior of cJSON_IsString as implemented in cJSON.c

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


extern "C" {
}

// Test 1: Validate that passing NULL returns false
// Rationale: The function explicitly handles NULL input and should return false.
bool test_IsString_nullPointer() {
    if (cJSON_IsString(NULL) != false) {
        printf("[FAIL] test_IsString_nullPointer: Expected false for NULL input.\n");
        return false;
    }
    // PASS
    return true;
}

// Test 2: Validate that a genuine string item returns true
// Rationale: An item created as a string should satisfy the string predicate.
bool test_IsString_stringItem_true() {
    cJSON *item = cJSON_CreateString("hello");
    if (item == NULL) {
        printf("[FAIL] test_IsString_stringItem_true: Failed to allocate string item.\n");
        return false;
    }
    bool res = cJSON_IsString(item);
    cJSON_Delete(item);
    if (res != true) {
        printf("[FAIL] test_IsString_stringItem_true: Expected true for string item, got false.\n");
        return false;
    }
    // PASS
    return true;
}

// Test 3: Validate that a non-string numeric item returns false
// Rationale: A number item should not be recognized as a string.
bool test_IsString_numberItem_false() {
    cJSON *item = cJSON_CreateNumber(123.45);
    if (item == NULL) {
        printf("[FAIL] test_IsString_numberItem_false: Failed to allocate number item.\n");
        return false;
    }
    bool res = cJSON_IsString(item);
    cJSON_Delete(item);
    if (res != false) {
        printf("[FAIL] test_IsString_numberItem_false: Expected false for number item, got true.\n");
        return false;
    }
    // PASS
    return true;
}

// Test 4: Validate that a synthetic item with non-string lower bits returns false
// Rationale: If we fabricate a cJSON object with a non-string type, the function should return false.
// This exercises handling of arbitrary item structures and confirms no accidental truthiness.
bool test_IsString_fakeItem_false() {
    // Allocate a raw cJSON object and zero it to avoid UB on uninitialized fields
    cJSON *fake = (cJSON*)malloc(sizeof(cJSON));
    if (fake == NULL) {
        printf("[FAIL] test_IsString_fakeItem_false: malloc failed.\n");
        return false;
    }
    std::memset(fake, 0, sizeof(cJSON));
    fake->type = 0; // Explicitly non-string type in lower 8 bits
    bool res = cJSON_IsString(fake);
    free(fake);
    if (res != false) {
        printf("[FAIL] test_IsString_fakeItem_false: Expected false for fake item, got true.\n");
        return false;
    }
    // PASS
    return true;
}

// Test 5: Validate that high-order bits do not affect the result (masking with 0xFF)
// Rationale: The implementation masks with 0xFF; higher bits should be ignored.
// We create a real string item and artificially set high bits to verify masking behavior.
bool test_IsString_stringItem_highbits_true() {
    cJSON *item = cJSON_CreateString("highbits");
    if (item == NULL) {
        printf("[FAIL] test_IsString_stringItem_highbits_true: Failed to allocate string item.\n");
        return false;
    }
    // Set a non-functional high-order bit; lower 8 bits remain the string type
    item->type |= 0x1000;
    bool res = cJSON_IsString(item);
    cJSON_Delete(item);
    if (res != true) {
        printf("[FAIL] test_IsString_stringItem_highbits_true: Expected true even with high bits set, got false.\n");
        return false;
    }
    // PASS
    return true;
}

// Simple test runner
int main() {
    int failures = 0;

    printf("Running tests for cJSON_IsString...\n");

    if (!test_IsString_nullPointer()) failures++;
    if (!test_IsString_stringItem_true()) failures++;
    if (!test_IsString_numberItem_false()) failures++;
    if (!test_IsString_fakeItem_false()) failures++;
    if (!test_IsString_stringItem_highbits_true()) failures++;

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("%d TEST(S) FAILED\n", failures);
    }

    return failures;
}