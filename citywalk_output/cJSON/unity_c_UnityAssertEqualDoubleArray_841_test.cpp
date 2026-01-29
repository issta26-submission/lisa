// Test suite for UnityAssertEqualDoubleArray in unity.c
// Target: C++11 compatible tests (no GTest), using Unity's own test framework conventions.
// This test file exercises several key paths of UnityAssertEqualDoubleArray:
// - Success when expected and actual pointers are the same (early return).
// - Success when elements are within the double tolerance.
// - Failure when an element is outside the tolerance.
// - Failure when one of the arrays is NULL (null-handling path).
// Notes:
// - The tests are designed to be run with the provided unity.c and unity.h in the same project.
// - We use Unity's RUN_TEST framework to automatically isolate failures per test.
// - We wrap the Unity headers in extern "C" to ensure correct linkage from C++.

#include <stddef.h>
#include <cstddef>
#include <unity.h>


extern "C" {
}

// No setup/teardown actions required for these unit tests.
void setUp(void) {}
void tearDown(void) {}

/*
Test 1: Succeeds when expected and actual pointers are the same.
Rationale: UnityAssertEqualDoubleArray has an early return if (expected == actual).
We expect the call to complete without reporting a failure.
*/
void test_UnityAssertEqualDoubleArray_Succeeds_WhenIdenticalPointers(void)
{
    // Prepare a small array of doubles.
    const double data[3] = {1.0, 2.0, 3.0};

    // Call with identical pointers; should be a fast path success.
    // num_elements > 0 to avoid pointless bailout path.
    UnityAssertEqualDoubleArray(data, data, 3, "identical pointers should pass",  __LINE__, 0);
}

/*
Test 2: Succeeds when elements are within tolerance.
Rationale: UnityDoublesWithin is used to compare each pair with a tolerance derived from the expected value.
We craft values within the tolerance to exercise the positive path inside the loop.
*/
void test_UnityAssertEqualDoubleArray_Succeeds_WhenWithinTolerance(void)
{
    // expected and actual are close per Unity's tolerance (UNITY_DOUBLE_PRECISION factor).
    const double expected[3] = {1.0, 2.0, 3.0};
    // Offset by a small amount less than the tolerance (e.g., ~4e-7 if UNITY_DOUBLE_PRECISION ~ 1e-6).
    const double actual[3]   = {1.0, 2.0000004, 3.0};

    // Use ARRAY_TO_ARRAY flag to exercise that branch (pointer arithmetic on expected).
    UnityAssertEqualDoubleArray(expected, actual, 3, "elements within tolerance should pass", __LINE__, UNITY_ARRAY_TO_ARRAY);
}

/*
Test 3: Fails when an element exceeds the tolerance.
Rationale: The core functionality should detect a significant mismatch and bail with a failure.
We intentionally create a large difference in the first element.
*/
void test_UnityAssertEqualDoubleArray_Fails_WhenDifferenceExceedsTolerance(void)
{
    const double expected[3] = {0.0, 2.0, 3.0};
    const double actual[3]   = {1.0, 2.0, 3.0}; // First element differs by 1.0

    // Expect the function to trigger a failure for the first element and bail.
    UnityAssertEqualDoubleArray(expected, actual, 3, "failure when first element is outside tolerance", __LINE__, 0);
}

/*
Test 4: Fails when one array is NULL (null array handling path).
Rationale: If one array is NULL and the other is not, UnityIsOneArrayNull should report a mismatch.
*/
void test_UnityAssertEqualDoubleArray_Fails_WhenOneArrayNull(void)
{
    const double actual[3] = {0.0, 0.0, 0.0};

    // expected is NULL, actual is non-NULL -> should trigger a null-array condition.
    UnityAssertEqualDoubleArray((const double*)NULL, actual, 3, "one array is NULL", __LINE__, 0);
}

// Main test runner
int main(void)
{
    UnityBegin("UnityAssertEqualDoubleArray tests");

    // Run individual tests. Each macro handles setup/teardown and captures failures.
    RUN_TEST(test_UnityAssertEqualDoubleArray_Succeeds_WhenIdenticalPointers, __LINE__);
    RUN_TEST(test_UnityAssertEqualDoubleArray_Succeeds_WhenWithinTolerance, __LINE__);
    RUN_TEST(test_UnityAssertEqualDoubleArray_Fails_WhenDifferenceExceedsTolerance, __LINE__);
    RUN_TEST(test_UnityAssertEqualDoubleArray_Fails_WhenOneArrayNull, __LINE__);

    return UnityEnd();
}