// Test suite for the focal method scenario: testNotEqualPtrArraysNullActual
// This C++11 test harness intentionally does not rely on GTest.
// It provides a minimal, self-contained set of tests to verify the core logic
// around comparing two pointer-arrays when the "actual" array is NULL.
// The tests focus on ensuring non-equality is correctly detected in the NULL-actual case
// and that a true-equality case is handled as well.

// Candidate Keywords (core components observed from the focal method):
// - A, B: stack-allocated sample values used to form pointer arrays
// - p0: the "actual" pointer array (often NULL in the focal test)
// - p1: the "expected" pointer array (contains &A, &B)
// - TEST_ASSERT_EQUAL_PTR_ARRAY: Unity macro used to compare pointer arrays
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: wrappers used to expect a failure (abort) in Unity
// Note: This standalone test suite re-implements the essential comparison logic in C++
// to keep it self-contained and compilable without Unity runtime dependencies.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Core comparison helper matching the conceptual behavior of TEST_ASSERT_EQUAL_PTR_ARRAY
// We compare arrays of const void* (address-based comparison) and treat NULL arrays as valid input
// with explicit lengths. If the lengths differ, or any corresponding pointers differ, they are not equal.
static bool notEqualPtrArrays(const void* actual[], std::size_t na,
                              const void* expected[], std::size_t nb)
{
    if (na != nb)
        return true; // different sizes -> not equal

    if (na == 0)
        return false; // both arrays are empty -> equal

    if (actual == nullptr || expected == nullptr)
        return true; // one side is NULL while length > 0 -> not equal

    for (std::size_t i = 0; i < na; ++i)
    {
        if (actual[i] != expected[i])
            return true; // any mismatch -> not equal
    }
    return false; // all elements equal
}

// Lightweight assertion helper for the standalone test harness
#define TEST_ASSERT(cond, msg)                                        \
    do { if (!(cond)) {                                                \
            std::cerr << "TEST FAILED: " << (msg) << "\n";            \
            return false;                                           \
        } } while (0)

// Test 1: Not equal when actual is NULL and expected has two elements
// This mirrors the focal scenario where actual pointer array is NULL while an actual two-element
// expected array exists. We expect detection of not-equality.
bool testNotEqualPtrArraysNullActual_Var1(void)
{
    // Setup: two distinct stack-allocated chars to form pointer array
    char A = 1;
    char B = 2;
    const void* p1[] = { &A, &B };    // expected array with two valid pointers
    const void** p0 = nullptr;         // actual array is NULL, length = 2

    // Conceptual assertion: notEqualPtrArrays should report "not equal" (true)
    bool notEqual = notEqualPtrArrays(reinterpret_cast<const void* const*>(p0), 2,
                                      p1, 2);

    TEST_ASSERT(notEqual, "Expected notEqualPtrArraysNullActual_Var1 to detect not-equality when actual is NULL with two expected pointers");
    return true;
}

// Test 2: Not equal when actual is NULL and expected has one element
// Additional variant to broaden coverage for the NULL-actual scenario with a different length.
bool testNotEqualPtrArraysNullActual_Var2(void)
{
    char A = 42;
    const void* p1[] = { &A };         // expected array with one pointer
    const void** p0 = nullptr;           // actual array is NULL, length = 1

    bool notEqual = notEqualPtrArrays(reinterpret_cast<const void* const*>(p0), 1,
                                      p1, 1);

    TEST_ASSERT(notEqual, "Expected notEqualPtrArraysNullActual_Var2 to detect not-equality when actual is NULL with a single expected pointer");
    return true;
}

// Test 3: Equal case should be detected as equal (notNotEqual)
// This ensures we cover the positive path where both arrays are non-NULL and contain identical pointers.
bool testEqualPtrArrays_ShouldBeEqual(void)
{
    char A = 7;
    char B = 8;
    const void* p_actual[]   = { &A, &B };
    const void* p_expected[] = { &A, &B }; // identical addresses

    bool notEqual = notEqualPtrArrays(p_actual, 2, p_expected, 2);
    TEST_ASSERT(!notEqual, "Expected equal pointer arrays to be detected as equal (notNotEqual)");
    return true;
}

// Simple test runner for the standalone harness
int main(void)
{
    bool all_passed = true;

    std::cout << "Running testNotEqualPtrArraysNullActual_Var1...\n";
    if (!testNotEqualPtrArraysNullActual_Var1()) {
        all_passed = false;
    } else {
        std::cout << "PASS: testNotEqualPtrArraysNullActual_Var1\n";
    }

    std::cout << "Running testNotEqualPtrArraysNullActual_Var2...\n";
    if (!testNotEqualPtrArraysNullActual_Var2()) {
        all_passed = false;
    } else {
        std::cout << "PASS: testNotEqualPtrArraysNullActual_Var2\n";
    }

    std::cout << "Running testEqualPtrArrays_ShouldBeEqual...\n";
    if (!testEqualPtrArrays_ShouldBeEqual()) {
        all_passed = false;
    } else {
        std::cout << "PASS: testEqualPtrArrays_ShouldBeEqual\n";
    }

    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}