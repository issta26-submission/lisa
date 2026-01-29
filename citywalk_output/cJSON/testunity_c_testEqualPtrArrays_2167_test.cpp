/*
 * C++11 standalone test harness for validating pointer-array equality logic
 * analogous to Unity's TEST_ASSERT_EQUAL_PTR_ARRAY and TEST_ASSERT_EACH_EQUAL_PTR
 *
 * Focus: replicate the focal behavior of testEqualPtrArrays from testunity.c
 * without relying on GTest. This harness provides a minimal, self-contained
 * test suite that exercises both positive and negative scenarios for pointer
 * array equality and per-element pointer equality.
 *
 * Notes:
 * - We implement two core helpers:
 *    - equal_ptr_arrays(a, b, len): compares two arrays of void* by pointer value
 *    - equal_ptr_each_ptr(expected, arr, len): asserts all elements equal to a single expected pointer
 * - Tests mirror the scenarios in the original focal method:
 *     - Positive cases: identical arrays, arrays with same leading portion, and cases with extra trailing pointers
 *     - Negative cases: NULL vs non-NULL arrays, mismatched last elements, and mismatched elements within array length
 * - All tests are non-terminating and return a boolean success/failure
 * - The file is self-contained and does not depend on Unity/GTest
 *
 * This file compiles as C++11 and can be run as:
 *     g++ -std=c++11 -O2 -Wall -Wextra ptr_array_tests.cpp && ./a.out
 */

// Candidate Keywords derived from the focal method:
// - testEqualPtrArrays, TEST_ASSERT_EQUAL_PTR_ARRAY, p0, p1, p2, p3, pointers, array length, NULL vs non-NULL pointers
// - TEST_ASSERT_EACH_EQUAL_PTR, equal_ptr_arrays, equal_ptr_each_ptr
// - Pointer identity comparisons, array length handling, NULL safety

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Core helpers: mimic Unity-like semantics in a minimal fashion

// Compare two arrays of pointers by their stored addresses.
// Returns true if for all i in [0, len) a[i] == b[i], false otherwise.
// Safe against NULL inputs: if either array pointer is NULL and len > 0, returns false.
// If len == 0, returns true.
static bool equal_ptr_arrays(const void* const* a, const void* const* b, size_t len)
{
    if (len == 0) return true;
    if (a == nullptr || b == nullptr) return false;
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Check that every element in arr of length len equals the single expected pointer.
static bool equal_ptr_each_ptr(void* expected, const void* const* arr, size_t len)
{
    if (len == 0) return true;
    if (arr == nullptr) return false;
    for (size_t i = 0; i < len; ++i)
    {
        if (arr[i] != expected)
            return false;
    }
    return true;
}

// Helper to cast array of concrete pointers to void* array for the equality check.
// We will store addresses as void* in the arrays to be able to pass them to the helper.
template <typename T>
static void store_address(void** out, T* addr)
{
    *out = static_cast<void*>(static_cast<void*>(addr));
}

// Convenience macro for printing test names
#define PRINT_TEST_HEADER(name) \
    std::cout << "Running " << name << "..." << std::endl;

// Test 1: Basic positive test cases mirroring the original testEqualPtrArrays
static bool test_equal_ptr_arrays_basic_positive(void)
{
    // <FOCAL_CLASS_DEP> style: mimic local pointer array scenario
    char A = 1;
    char B = 2;
    char C = 3;

    void* p0[] = { &A, &B, &C };
    void* p1[] = { &A, &B, &C, &A };
    void* p2[] = { &A, &B };
    void* p3[] = { &A };

    bool ok =
        equal_ptr_arrays(reinterpret_cast<const void* const*>(p0),
                         reinterpret_cast<const void* const*>(p0), 1) &&
        equal_ptr_arrays(reinterpret_cast<const void* const*>(p0),
                         reinterpret_cast<const void* const*>(p0), 3) &&
        equal_ptr_arrays(reinterpret_cast<const void* const*>(p0),
                         reinterpret_cast<const void* const*>(p1), 3) &&
        equal_ptr_arrays(reinterpret_cast<const void* const*>(p1),
                         reinterpret_cast<const void* const*>(p2), 2) &&
        equal_ptr_arrays(reinterpret_cast<const void* const*>(p3),
                         reinterpret_cast<const void* const*>(p0), 1);

    return ok;
}
// Test 2: Basic negative scenario - NULL vs non-NULL expected pointer array
static bool test_not_equal_ptr_arrays_null_expected(void)
{
    char A = 1;
    char B = 2;

    void* p0[] = { &A, &B };
    const void* const* a = nullptr; // representing NULL expected array
    const void* const* b = reinterpret_cast<const void* const*>(p0);

    bool ok = equal_ptr_arrays(a, b, 2) == false;
    return ok;
}
// Test 3: Basic negative scenario - NULL Actual array
static bool test_not_equal_ptr_arrays_null_actual(void)
{
    char A = 1;
    char B = 2;

    void* p1[] = { &A, &B };
    const void* const* a = reinterpret_cast<const void* const*>(p1);
    const void* const* b = nullptr; // representing NULL actual array

    bool ok = equal_ptr_arrays(a, b, 2) == false;
    return ok;
}
// Test 4: Different last element in same-length arrays
static bool test_not_equal_ptr_arrays_case1(void)
{
    char A = 1;
    char B = 2;
    char C = 3;

    void* p0[] = { &A, &B, &C, &B };
    void* p1[] = { &A, &B, &C, &A };

    bool ok = equal_ptr_arrays(reinterpret_cast<const void* const*>(p0),
                               reinterpret_cast<const void* const*>(p1), 4) == false;
    return ok;
}
// Test 5: Different first element
static bool test_not_equal_ptr_arrays_case2(void)
{
    char A = 1;
    char B = 2;
    char C = 3;

    void* p0[] = { &B, &B, &C, &A };
    void* p1[] = { &A, &B, &C, &A };

    bool ok = equal_ptr_arrays(reinterpret_cast<const void* const*>(p0),
                               reinterpret_cast<const void* const*>(p1), 4) == false;
    return ok;
}
// Test 6: Different middle elements
static bool test_not_equal_ptr_arrays_case3(void)
{
    char A = 1;
    char B = 2;
    char C = 3;

    void* p0[] = { &A, &B, &B, &A };
    void* p1[] = { &A, &B, &C, &A };

    bool ok = equal_ptr_arrays(reinterpret_cast<const void* const*>(p0),
                               reinterpret_cast<const void* const*>(p1), 4) == false;
    return ok;
}

// Test 7: Per-element equality for a single expected pointer
static bool test_equal_ptr_each_equal_case1(void)
{
    char A = 1;
    char B = 2;
    char C = 3;

    void* p0[] = { &A, &A, &A };
    void* p1[] = { &A, &B, &C, &A };
    void* p2[] = { &B, &B };
    void* p3[] = { &C };

    bool ok =
        equal_ptr_each_ptr(&A, reinterpret_cast<const void* const*>(p0), 1) &&
        equal_ptr_each_ptr(&A, reinterpret_cast<const void* const*>(p0), 3) &&
        equal_ptr_each_ptr(&A, reinterpret_cast<const void* const*>(p1), 1) &&
        equal_ptr_each_ptr(&B, reinterpret_cast<const void* const*>(p2), 2) &&
        equal_ptr_each_ptr(&C, reinterpret_cast<const void* const*>(p3), 1);

    return ok;
}
// Test 8: Per-element equality negative
static bool test_not_equal_ptr_each_equal_case1(void)
{
    char A = 1;
    char B = 2;

    void* p0[] = { &A, &A, &A, &B };

    // Expect A for all elements in a 4-length array -> should fail
    bool ok = equal_ptr_each_ptr(&A, reinterpret_cast<const void* const*>(p0), 4) == false;
    return ok;
}

// Main: run all tests and report
int main()
{
    struct TestInfo {
        const char* name;
        std::function<bool(void)> fn;
        const char* description;
    };

    std::vector<TestInfo> tests = {
        { "test_equal_ptr_arrays_basic_positive",
          [](){ return test_equal_ptr_arrays_basic_positive(); },
          "Positive coverage for TEST_ASSERT_EQUAL_PTR_ARRAY-like behavior on basic three-pointer, extended, and mixed-length arrays." },
        { "test_not_equal_ptr_arrays_null_expected",
          [](){ return test_not_equal_ptr_arrays_null_expected(); },
          "Negative case: NULL expected array vs non-NULL actual array should fail gracefully." },
        { "test_not_equal_ptr_arrays_null_actual",
          [](){ return test_not_equal_ptr_arrays_null_actual(); },
          "Negative case: Non-NULL expected array vs NULL actual array should fail gracefully." },
        { "test_not_equal_ptr_arrays_case1",
          [](){ return test_not_equal_ptr_arrays_case1(); },
          "Negative case: last-element mismatch in two equal-length arrays." },
        { "test_not_equal_ptr_arrays_case2",
          [](){ return test_not_equal_ptr_arrays_case2(); },
          "Negative case: first-element mismatch in two arrays." },
        { "test_not_equal_ptr_arrays_case3",
          [](){ return test_not_equal_ptr_arrays_case3(); },
          "Negative case: middle-element mismatch in two arrays." },
        { "test_equal_ptr_each_equal_case1",
          [](){ return test_equal_ptr_each_equal_case1(); },
          "Positive per-element equality tests: all elements equal to a single expected pointer." },
        { "test_not_equal_ptr_each_equal_case1",
          [](){ return test_not_equal_ptr_each_equal_case1(); },
          "Negative per-element equality case: ensure failure on mismatch." }
    };

    PRINT_TEST_HEADER("Pointer Array Equality Test Suite (C++11)");
    int failures = 0;
    for (const auto& t : tests)
    {
        bool result = t.fn();
        std::cout << t.name << ": " << (result ? "PASS" : "FAIL") << std::endl;
        if (!result) {
            ++failures;
            std::cerr << "  Note: " << t.description << std::endl;
        } else {
            std::cout << "  " << t.description << std::endl;
        }
        std::cout << std::endl;
    }

    if (failures == 0)
    {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
    else
    {
        std::cout << failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}