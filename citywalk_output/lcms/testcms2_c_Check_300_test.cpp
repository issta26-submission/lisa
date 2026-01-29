// C++11 unit test suite for the focal method Check defined in testcms2.c
// This test suite does not depend on Google Test. It uses the project's existing
// test infrastructure (testcms2.h) and verifies the behavior of Check under
// different Fn() outcomes.

// Notes:
// - We rely on the provided TestFn typedef from the project's headers.
// - We implement three C-callable Fn functions to simulate different scenarios.
// - We reset global counters before each test to measure isolated behavior.
// - We use non-terminating assertions (print-based) to allow all tests to run.

#include <cstdio>
#include <testcms2.h>


// Include project header to obtain TestFn type, global buffers, and Check signature.
// This header is expected to declare: TestFn, TrappedError, ReasonToFailBuffer,
// SubTestBuffer, TotalTests, TotalFail, TRUE, FALSE, etc.

extern "C" {
    // Scenario 1: Fn returns true and does not trigger TrappedError.
    // This should take the "memory check" path inside Check and count as a success.
    cmsBool FnCheck_Good_NoTrap(void) {
        TrappedError = FALSE; // Ensure no trap is reported
        return TRUE;           // Indicate the test function succeeded
    }

    // Scenario 2: Fn returns true but sets TrappedError to true.
    // This should cause Check to treat it as a failure (since !TrappedError is false).
    cmsBool FnCheck_Good_WithTrap(void) {
        TrappedError = TRUE; // Simulate an error trap set by the test
        return TRUE;          // Function would succeed, but TrappedError triggers failure
    }

    // Scenario 3: Fn returns false (regardless of TrappedError).
    // This should immediately fail the test.
    cmsBool FnCheck_Bad(void) {
        TrappedError = FALSE;
        return FALSE; // Indicate failure
    }
}

// Simple non-terminating assertion helper
static void assert_true(bool cond, const char* msg) {
    if (!cond) {
        // Do not terminate; just log the failure to stdout/stderr
        fprintf(stderr, "ASSERT FAILED: %s\n", msg);
    }
}

int main(void) {
    int totalFailureCount = 0;

    printf("Starting Check() unit tests...\n");

    // Test 1: Good path without trapped error
    // Expected: TotalTests == 1, TotalFail == 0
    TotalTests = 0;
    TotalFail = 0;
    Check("GoodPath_NoTrap", (TestFn)FnCheck_Good_NoTrap);
    bool test1_ok = (TotalTests == 1) && (TotalFail == 0);
    assert_true(test1_ok, "Test 1 failed: expected 1 total test and 0 failures for Good_NoTrap path");
    if (!test1_ok) totalFailureCount++;

    // Test 2: Fn reports a trap, causing overall failure despite Fn returning true
    // Expected: TotalTests == 1, TotalFail == 1
    TotalTests = 0;
    TotalFail = 0;
    Check("GoodPath_WithTrap", (TestFn)FnCheck_Good_WithTrap);
    bool test2_ok = (TotalTests == 1) && (TotalFail == 1);
    assert_true(test2_ok, "Test 2 failed: expected 1 total test and 1 failure when TrappedError is set");
    if (!test2_ok) totalFailureCount++;

    // Test 3: Fn returns false, immediate failure
    // Expected: TotalTests == 1, TotalFail == 1
    TotalTests = 0;
    TotalFail = 0;
    Check("BadPath", (TestFn)FnCheck_Bad);
    bool test3_ok = (TotalTests == 1) && (TotalFail == 1);
    assert_true(test3_ok, "Test 3 failed: expected 1 total test and 1 failure when Fn returns false");
    if (!test3_ok) totalFailureCount++;

    printf("Check() unit tests completed with %d failure(s).\n", totalFailureCount);
    return totalFailureCount;
}