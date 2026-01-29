// This is a self-contained C++11 test suite focused on the core behavior
// of pointer-array equality checks that underlie the focal test
// testNotEqualPtrArraysNullExpected from testunity.c.
// The goal is to exercise the scenarios around NULL vs non-NULL pointer arrays
// and basic element-wise pointer equality without relying on GTest or the
// Unity test harness (as per constraints). The suite is designed to be
// compilable with C++11.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (representing the method's core dependencies)
/*
  - A, B, C: sample data elements (chars) used to form addresses for pointers
  - p0: expected pointer-array (char** in the focal code)
  - p1, p2, p3: actual pointer-arrays used in tests
  - NULL: to exercise NULL vs non-NULL path coverage
  - TEST_ASSERT_EQUAL_PTR_ARRAY: conceptual primitive; replaced with
    our own equal_ptr_arrays logic
  - equal_ptr_arrays: function that mimics element-wise pointer equality check
  - length: number of pointers to compare
  - FALSE/TRUE: expected outcomes of the equality checks
*/

// Core utility: checks if two arrays of const char* pointers are equal
// with the given length. Handles NULL inputs similarly to Unity's tests:
// - If both pointers are NULL, they are considered equal.
// - If one is NULL and the other is not, they are not equal.
// - Otherwise, compare element-by-element by pointer value (address equality).
static bool equal_ptr_arrays(const char** a, const char** b, std::size_t length) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    for (std::size_t i = 0; i < length; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Test 1: NotEqual Ptr Arrays - Expected is NULL, Actual is non-NULL
// This mirrors the focal testNotEqualPtrArraysNullExpected: a NULL expected
// array should not be considered equal to a non-NULL actual array.
static bool testNotEqualPtrArraysNullExpected(void) {
    // Setup two sample data elements
    char A = 1;
    char B = 2;

    // p0 is the expected array (NULL for this test)
    const char** p0 = nullptr;

    // p1 is the actual array containing two addresses
    const char* p1[] = { &A, &B };

    // Expect NOT equal (NULL vs non-NULL) -> should be false equality
    bool are_equal = equal_ptr_arrays(p0, p1, 2);
    if (are_equal) {
        // Failure: should not be equal
        std::cout << "[FAIL] testNotEqualPtrArraysNullExpected: expected NULL array "
                     "not to be equal to non-NULL actual array.\n";
        return false;
    }
    // Pass: correctly identified as not equal
    return true;
}

// Test 2: NotEqual Ptr Arrays - Actual is NULL, Expected is non-NULL
// Mirrors testNotEqualPtrArraysNullActual: a non-NULL expected array should
// not be considered equal to a NULL actual array.
static bool testNotEqualPtrArraysNullActual(void) {
    char A = 1;
    char B = 2;

    // p0 is the expected non-NULL array
    const char* p0[] = { &A, &B };
    const char** p0ptr = p0;

    // p1 is the actual NULL array
    const char** p1 = nullptr;

    bool are_equal = equal_ptr_arrays(p0ptr, p1, 2);
    if (are_equal) {
        std::cout << "[FAIL] testNotEqualPtrArraysNullActual: expected non-NULL array "
                     "not to be equal to NULL actual array.\n";
        return false;
    }
    return true;
}

// Test 3: NotEqual Ptr Arrays - Mismatch at last element (4-element arrays)
static bool testNotEqualPtrArrays1(void) {
    char A = 1;
    char B = 2;
    char C = 3;
    // p0: {A, B, C, B}
    const char* p0[] = { &A, &B, &C, &B };
    // p1: {A, B, C, A}
    const char* p1[] = { &A, &B, &C, &A };

    bool are_equal = equal_ptr_arrays(p0, p1, 4);
    if (are_equal) {
        std::cout << "[FAIL] testNotEqualPtrArrays1: last-element mismatch not detected.\n";
        return false;
    }
    return true;
}

// Test 4: NotEqual Ptr Arrays - Mismatch at first element
static bool testNotEqualPtrArrays2(void) {
    char A = 1;
    char B = 2;
    char C = 3;
    // p0: {B, B, C, A}
    const char* p0[] = { &B, &B, &C, &A };
    // p1: {A, B, C, A}
    const char* p1[] = { &A, &B, &C, &A };

    bool are_equal = equal_ptr_arrays(p0, p1, 4);
    if (are_equal) {
        std::cout << "[FAIL] testNotEqualPtrArrays2: first-element mismatch not detected.\n";
        return false;
    }
    return true;
}

// Test 5: NotEqual Ptr Arrays - Mismatch at middle element
static bool testNotEqualPtrArrays3(void) {
    char A = 1;
    char B = 2;
    char C = 3;
    // p0: {A, B, B, A}
    const char* p0[] = { &A, &B, &B, &A };
    // p1: {A, B, C, A}
    const char* p1[] = { &A, &B, &C, &A };

    bool are_equal = equal_ptr_arrays(p0, p1, 4);
    if (are_equal) {
        std::cout << "[FAIL] testNotEqualPtrArrays3: middle-element mismatch not detected.\n";
        return false;
    }
    return true;
}

// Test 6: Equal Ptr Arrays - All elements match (3-element arrays)
// This checks the positive path where arrays are considered equal.
static bool testEqualPtrArrays(void) {
    char A = 1;
    char B = 2;
    char C = 3;
    const char* p0[] = { &A, &B, &C };
    const char* p1[] = { &A, &B, &C };

    bool are_equal = equal_ptr_arrays(p0, p1, 3);
    if (!are_equal) {
        std::cout << "[FAIL] testEqualPtrArrays: identical pointer arrays reported as not equal.\n";
        return false;
    }
    return true;
}

// Simple test runner
int main(void) {
    using TestFn = std::function<bool(void)>;

    std::vector<std::pair<std::string, TestFn>> tests = {
        { "testNotEqualPtrArraysNullExpected", testNotEqualPtrArraysNullExpected },
        { "testNotEqualPtrArraysNullActual", testNotEqualPtrArraysNullActual },
        { "testNotEqualPtrArrays1", testNotEqualPtrArrays1 },
        { "testNotEqualPtrArrays2", testNotEqualPtrArrays2 },
        { "testNotEqualPtrArrays3", testNotEqualPtrArrays3 },
        { "testEqualPtrArrays", testEqualPtrArrays }
    };

    int passed = 0;
    for (const auto& t : tests) {
        bool result = t.second();
        if (result) {
            std::cout << "[PASS] " << t.first << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << t.first << "\n";
        }
    }
    std::cout << "\nSummary: " << passed << " / " << tests.size() << " tests passed.\n";

    return (passed == static_cast<int>(tests.size())) ? 0 : 1;
}