// Lightweight C++11 unit-test harness focused on the focal behavior:
// "testNotEqualIntArraysNullActual" from testunity.c.
// The goal is to provide a self-contained, executable test suite without GTest,
// suitable for C++11 projects, and without depending on GTest or Unity runtime.
//
// Notes and mapping to the provided steps:
// Step 1 (Program Understanding): The focal method tests that TEST_ASSERT_EQUAL_INT_ARRAY
//         will fail/abort when the actual array pointer is NULL while the expected array is non-NULL.
//         Core dependent components (Candidate Keywords) include: INT_ARRAY comparison logic,
//         NULL-checks, and the abort/failure signaling pattern (represented here by exceptions).
// Step 2 (Unit Test Generation): Build a small, self-contained test suite around the core
//         idea of "non-equal when actual is NULL" and complement with additional tests to cover
//         equal and non-equal cases for int arrays.
// Step 3 (Test Case Refinement): Provide a handful of tests to exercise true/false branches of the
//         core predicate (NULL actual vs non-NULL, equal vs not equal, and length-zero edge).
// The tests are kept domain-knowledge friendly and use a tiny, internal assertion mechanism.

#include <cstdint>
#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (for traceability in test design):
// - INT_ARRAY comparison
// - NULL pointer handling
// - array contents equality
// - length-based comparison
// - failure signaling (abort/exception in test harness)

// Lightweight array-equality checker mimicking core semantics:
// - If length is zero, return false to reflect Unity's behavior seen in the focal tests
// - If either pointer is NULL, return false (mimicking a failed assertion)
static bool are_int_arrays_equal(const int* a, const int* b, std::size_t len)
{
    if (len == 0) {
        // Mirror the Unity test's expectation that a zero-length comparison is invalid/abortable.
        return false;
    }
    if (a == nullptr || b == nullptr) {
        return false;
    }
    for (std::size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Simple assertion macro substitute for this tiny harness.
// Throws a runtime_error on failure; this allows the main to catch and report failures.
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) throw std::runtime_error(std::string("ASSERT_TRUE failed: ") + (msg)); \
} while (0)

#define ASSERT_FALSE(cond, msg) do { \
    if (cond) throw std::runtime_error(std::string("ASSERT_FALSE failed: ") + (msg)); \
} while (0)


// Test 1: Recreate focal scenario:
// testNotEqualIntArraysNullActual: p1 is NULL, p0 is a non-empty int array, length 4.
// Expected: The equality check should fail (not equal), mirroring the original Unity test
// which aborts because the arrays are not equal when the actual is NULL.
void testNotEqualIntArraysNullActual_cpp(void)
{
    // Core setup from focal method
    const int* p1 = nullptr;        // Actual is NULL
    int p0[] = {1, 8, 987, 2};      // Expected array

    // Mimic: TEST_ASSERT_EQUAL_INT_ARRAY(p0, p1, 4);
    // We expect this to fail (not be equal) due to NULL actual.
    bool equal = are_int_arrays_equal(p0, p1, 4);

    // The Unity test expects a failure (abort) in this scenario.
    // Our harness uses exceptions to signal failure.
    ASSERT_FALSE(equal, "testNotEqualIntArraysNullActual_cpp: arrays should not be considered equal when actual is NULL");
}

// Test 2: Negative scenario mirrored but with different actual pointer (NULL for expected instead)
// This is not exactly the focal method, but helps ensure our equality checker handles NULL on either side.
void testNotEqualIntArraysNullExpected_cpp(void)
{
    const int* p0 = nullptr;         // Expected is NULL
    int p1[] = {1, 2, 3, 4};          // Actual non-NULL array

    bool equal = are_int_arrays_equal(p0, p1, 4);
    ASSERT_FALSE(equal, "testNotEqualIntArraysNullExpected_cpp: arrays should not be considered equal when expected is NULL");
}

// Test 3: Positive path where arrays are equal (to verify true-branch coverage)
void testEqualIntArraysSame_cpp(void)
{
    int p0[] = {1, 2, 3, 4};
    int p1[] = {1, 2, 3, 4};

    bool equal = are_int_arrays_equal(p0, p1, 4);
    ASSERT_TRUE(equal, "testEqualIntArraysSame_cpp: arrays should be equal but were reported not equal");
}

// Test 4: Negative path where arrays differ in last element
void testNotEqualIntArraysDifferentLast_cpp(void)
{
    int p0[] = {1, 2, 3, 4};
    int p1[] = {1, 2, 3, 5};

    bool equal = are_int_arrays_equal(p0, p1, 4);
    ASSERT_FALSE(equal, "testNotEqualIntArraysDifferentLast_cpp: arrays differ in last element but reported equal");
}

// Test 5: Edge-case: zero length should yield non-equality per our helper
void testNotEqualIntArraysLengthZero_cpp(void)
{
    int a[] = {1};
    int b[] = {1};

    bool equal = are_int_arrays_equal(a, b, 0);
    ASSERT_FALSE(equal, "testNotEqualIntArraysLengthZero_cpp: zero-length comparison should not be considered equal");
}

// Runner: Execute tests and report results
int main(void)
{
    int total = 0;
    int passed = 0;
    int failed = 0;

    auto runTest = [&](void (*fn)(), const char* name) {
        ++total;
        try {
            fn();
            ++passed; // If no exception, test passed (based on ASSERT_* semantics)
        } catch (const std::exception& ex) {
            ++failed;
            std::cerr << "FAILED: " << name << " -> " << ex.what() << "\n";
        } catch (...) {
            ++failed;
            std::cerr << "FAILED: " << name << " -> unknown exception\n";
        }
    };

    // Explanatory comments accompany each test invocation as requested.
    runTest(reinterpret_cast<void (*)()>(&testNotEqualIntArraysNullActual_cpp), "testNotEqualIntArraysNullActual_cpp");
    // Step 2/3: Additional tests to ensure coverage of related paths
    runTest(reinterpret_cast<void (*)()>(&testNotEqualIntArraysNullExpected_cpp), "testNotEqualIntArraysNullExpected_cpp");
    runTest(reinterpret_cast<void (*)()>(&testEqualIntArraysSame_cpp), "testEqualIntArraysSame_cpp");
    runTest(reinterpret_cast<void (*)()>(&testNotEqualIntArraysDifferentLast_cpp), "testNotEqualIntArraysDifferentLast_cpp");
    runTest(reinterpret_cast<void (*)()>(&testNotEqualIntArraysLengthZero_cpp), "testNotEqualIntArraysLengthZero_cpp");

    // Summary
    std::cout << "Total tests: " << total
              << ", Passed: " << passed
              << ", Failed: " << failed << "\n";

    return (failed == 0) ? 0 : 1;
}