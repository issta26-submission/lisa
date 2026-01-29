// This test suite targets the UnityAssertDoublesWithin function from unity.c.
// It aims to cover both branches: the "pass" path (values within delta) and
// the "fail" path (values outside delta), including usage of a non-empty message.
// The tests are written in C++11 but exercise a C function from the Unity framework.
// Note: UnityDoublesWithin is a static helper inside unity.c, so we cannot call it directly;
// we rely on UnityAssertDoublesWithin's observable behavior (test pass/fail).

extern "C" {
#include <stddef.h>
#include <unity.h>

}

// Candidate Keywords (core dependencies) considered while designing tests:
// - UnityAssertDoublesWithin(delta, expected, actual, msg, lineNumber)
// - UnityDoublesWithin(delta, expected, actual)       // static inside unity.c
// - UnityTestResultsFailBegin(lineNumber)              // invoked on failure
// - UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT(expected, actual) // prints comparison
// - UnityAddMsgIfSpecified(msg)                         // appends custom message
// - RETURN_IF_FAIL_OR_IGNORE                              // macro guarding test flow

// Basic setup/teardown (no specific fixtures needed for these tests)
void setUp(void) {}
void tearDown(void) {}

/*
Test 1: Pass when actual is within delta of expected.
- This exercises the true branch of the conditional inside UnityAssertDoublesWithin.
- No failure should be recorded; the test should pass.
*/
void test_UnityAssertDoublesWithin_PassWithinDelta(void)
{
    // delta small enough that actual is within tolerance
    const double delta = 0.0005;
    const double expected = 3.1415926535;
    const double actual   = 3.1415926536; // within delta

    // If within delta, UnityAssertDoublesWithin should not trigger a failure.
    UnityAssertDoublesWithin(delta, expected, actual, "Should pass when values are within delta", __LINE__);
}

/*
Test 2: Fail when actual is outside delta with a non-empty message.
- This exercises the false branch and ensures message handling works.
- The test will be marked as failed if UnityAssertDoublesWithin triggers failure.
*/
void test_UnityAssertDoublesWithin_FailOutsideDelta_WithMsg(void)
{
    // delta too small for actual difference
    const double delta = 0.0001;
    const double expected = 1.0;
    const double actual   = 1.02; // outside delta

    // Provide a non-empty message to exercise UnityAddMsgIfSpecified(msg)
    UnityAssertDoublesWithin(delta, expected, actual, "Fail expected: actual outside delta", __LINE__);
}

/*
Test 3: Pass when delta is zero and actual equals expected.
- Boundary case to verify behavior at delta = 0.0.
- Should pass since actual == expected.
*/
void test_UnityAssertDoublesWithin_BoundaryZeroDelta(void)
{
    const double delta    = 0.0;
    const double expected = -2.5;
    const double actual   = -2.5;

    UnityAssertDoublesWithin(delta, expected, actual, "Boundary test with zero delta", __LINE__);
}

/*
Test 4: Pass when actual is exactly at the delta boundary (within tolerance).
- Ensures that values exactly delta apart are treated as within the delta.
*/
void test_UnityAssertDoublesWithin_BoundaryAtDelta(void)
{
    const double delta    = 0.01;
    const double expected = 10.0;
    const double actual   = 10.01; // exactly delta away

    UnityAssertDoublesWithin(delta, expected, actual, "Boundary equality test", __LINE__);
}

// Main test runner (C++ entry point) compatible with Unity's RUN_TEST macro
int main(void)
{
    // Begin the Unity test run
    UnityBegin("unity_assert_doubles_within_test.c");

    // Run individual tests
    RUN_TEST(test_UnityAssertDoublesWithin_PassWithinDelta);
    RUN_TEST(test_UnityAssertDoublesWithin_FailOutsideDelta_WithMsg);
    RUN_TEST(test_UnityAssertDoublesWithin_BoundaryZeroDelta);
    RUN_TEST(test_UnityAssertDoublesWithin_BoundaryAtDelta);

    // End the Unity test run and return status
    return UnityEnd();
}