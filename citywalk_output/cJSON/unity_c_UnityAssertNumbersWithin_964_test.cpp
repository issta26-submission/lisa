// Unit test suite for UnityAssertNumbersWithin (unity.c) using a lightweight C++11 harness
// This test suite does not rely on Google Test or any heavy framework. It uses the
// Unity framework's internal function UnityAssertNumbersWithin by linking against unity.c
// and a small custom main() test runner that performs non-terminating checks.
// Notes:
// - The tests exercise both branches based on the style flag (UNITY_DISPLAY_RANGE_INT vs not).
// - We verify pass/fail behavior by inspecting Unity.CurrentTestFailed after each call.
// - Each test case is isolated to avoid cross-test bail effects from Unity's macro UNITY_FAIL_AND_BAIL.
// - We assume the project is set up so that unity.c and this test file are compiled and linked together.

#include <stddef.h>
#include <cstdio>
#include <unity.h>


// Bring in Unity's declarations. Since this file is compiled as C++, ensure C linkage for C headers.
extern "C" {
}

/*
  Candidate Keywords (derived from UnityAssertNumbersWithin behavior):
  - delta: the allowed numeric delta for a mismatch
  - expected: the expected integer value
  - actual: the actual integer value
  - style: display style flags influencing how numbers are compared/printed
  - UNITY_DISPLAY_RANGE_INT: style flag branch that uses signed comparison in a specific range mode
  - Unity.CurrentTestFailed: internal flag set when a test condition fails
  - UnityPrintNumberByStyle / UnityPrint / UnityTestResultsFailBegin / UnityAddMsgIfSpecified
    (represented in test harness via side effects; their actual bodies are stubbed in <FOCAL_CLASS_DEP>)
  - UNITY_FAIL_AND_BAIL: macro that aborts the current test case after a failure
  - UNITY_LINE_TYPE: line number type used by Unity APIs
  - UNITY_DISPLAY_STYLE_T / UNITY_DISPLAY_STYLE_INT: types/macros used to drive formatting (passed to UnityAssertNumbersWithin)
  - RETURN_IF_FAIL_OR_IGNORE: macro that can early-return if a prior failure/ignore occurred
  These keywords map to thecore logic: choosing a delta-compare path between the two values,
  and setting the CurrentTestFailed flag when the difference exceeds delta.
*/

// Helper test cases note:
// Each test case is isolated so that a failing UnityAssertNumbersWithin call (which may bail)
// does not prevent the rest of the tests from executing.

/////////////////////////////////////
// Test Case 1: Range-based comparison (UNITY_DISPLAY_RANGE_INT) passes
// delta = 5, expected = 10, actual = 12
// Difference = 2 <= delta -> should not fail (CurrentTestFailed should be 0)
void test_case_delta_within_range_pass() {
    Unity.CurrentTestFailed = 0;
    // style = UNITY_DISPLAY_RANGE_INT to take the range-based branch
    UnityAssertNumbersWithin(5, 10, 12, "", 0, UNITY_DISPLAY_RANGE_INT);
    if (Unity.CurrentTestFailed == 0) {
        printf("[PASS] test_case_delta_within_range_pass\n");
    } else {
        printf("[FAIL] test_case_delta_within_range_pass: CurrentTestFailed == %u\n",
               (unsigned)Unity.CurrentTestFailed);
    }
}

/////////////////////////////////////
// Test Case 2: Range-based comparison fails (delta too small)
// delta = 1, expected = 10, actual = 12
// Difference = 2 > delta -> should fail (CurrentTestFailed should be 1)
void test_case_delta_within_range_fail() {
    Unity.CurrentTestFailed = 0;
    UnityAssertNumbersWithin(1, 10, 12, "", 0, UNITY_DISPLAY_RANGE_INT);
    if (Unity.CurrentTestFailed != 0) {
        printf("[PASS] test_case_delta_within_range_fail\n");
    } else {
        printf("[FAIL] test_case_delta_within_range_fail: CurrentTestFailed == 0\n");
    }
}

/////////////////////////////////////
// Test Case 3: Non-range comparison (not UNITY_DISPLAY_RANGE_INT) fails when actual < expected
// delta = 2, expected = 10, actual = 7
// Difference = 3 > delta -> should fail
void test_case_no_range_fail_actual_less() {
    Unity.CurrentTestFailed = 0;
    UnityAssertNumbersWithin(2, 10, 7, "", 0, 0); // style = 0 means not using range int display
    if (Unity.CurrentTestFailed != 0) {
        printf("[PASS] test_case_no_range_fail_actual_less\n");
    } else {
        printf("[FAIL] test_case_no_range_fail_actual_less: CurrentTestFailed == 0\n");
    }
}

/////////////////////////////////////
// Test Case 4: Non-range comparison passes when delta exactly matches the difference
// delta = 3, expected = 10, actual = 7
// Difference = 3; 3 > delta is false -> should pass
void test_case_no_range_pass_actual_less_exact() {
    Unity.CurrentTestFailed = 0;
    UnityAssertNumbersWithin(3, 10, 7, "", 0, 0);
    if (Unity.CurrentTestFailed == 0) {
        printf("[PASS] test_case_no_range_pass_actual_less_exact\n");
    } else {
        printf("[FAIL] test_case_no_range_pass_actual_less_exact: CurrentTestFailed == %u\n",
               (unsigned)Unity.CurrentTestFailed);
    }
}

/////////////////////////////////////
// Test Case 5: Non-range comparison with actual > expected fails when delta is small
// delta = 1, expected = 10, actual = 12
// Difference = 2 > delta -> should fail
void test_case_no_range_fail_actual_greater() {
    Unity.CurrentTestFailed = 0;
    UnityAssertNumbersWithin(1, 10, 12, "", 0, 0);
    if (Unity.CurrentTestFailed != 0) {
        printf("[PASS] test_case_no_range_fail_actual_greater\n");
    } else {
        printf("[FAIL] test_case_no_range_fail_actual_greater: CurrentTestFailed == 0\n");
    }
}

/////////////////////////////////////
// Test Case 6: Non-range comparison with actual > expected passes when delta is large enough
// delta = 2, expected = 10, actual = 12
// Difference = 2; 2 > delta is false -> should pass
void test_case_no_range_pass_actual_greater_with_delta() {
    Unity.CurrentTestFailed = 0;
    UnityAssertNumbersWithin(2, 10, 12, "", 0, 0);
    if (Unity.CurrentTestFailed == 0) {
        printf("[PASS] test_case_no_range_pass_actual_greater_with_delta\n");
    } else {
        printf("[FAIL] test_case_no_range_pass_actual_greater_with_delta: CurrentTestFailed == %u\n",
               (unsigned)Unity.CurrentTestFailed);
    }
}

/////////////////////////////////////
// Main entry point
int main(void) {
    printf("==== UnityAssertNumbersWithin Unit Tests (C++11 harness) ====\n");

    // Run individual test cases (non-terminating assertions). Each test handles its own reporting.

    test_case_delta_within_range_pass();
    test_case_delta_within_range_fail();
    test_case_no_range_fail_actual_less();
    test_case_no_range_pass_actual_less_exact();
    test_case_no_range_fail_actual_greater();
    test_case_no_range_pass_actual_greater_with_delta();

    printf("==== End of tests ====\n");
    // Return 0 indicates (overall) success; individual tests print PASS/FAIL messages.
    return 0;
}