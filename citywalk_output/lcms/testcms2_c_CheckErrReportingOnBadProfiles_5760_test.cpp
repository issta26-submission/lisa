/*
  Unit test suite for the focal method:
  cmsInt32Number CheckErrReportingOnBadProfiles(void)

  Approach:
  - Use a lightweight, non-GTest test harness suitable for C++11.
  - Validate key behavior via observable side-effects and return values:
    1) TrappedError is reset to FALSE after the call.
    2) The return value matches a direct call to CheckBadProfiles().
    3) Repeated invocations do not crash and preserve invariants.
  - Rely on the provided test harness header (testcms2.h) for C functions and global state.
  - All tests are executed from main() per domain guidelines.
  - Explanatory comments are included for each test.
*/

#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Import the test harness and declarations of the focal method and dependencies.
// The header is expected to expose C-style interfaces for the LittleCMS-like test suite.

static int g_failed_tests = 0;

// Simple assertion helper that logs failures but does not terminate the test run.
// This enables multiple tests to run to completion.
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL: %s\n", msg); \
        ++g_failed_tests; \
    } else { \
        fprintf(stdout, "PASS: %s\n", msg); \
    } \
} while (0)

/*
  Test 1: CheckErrReportingOnBadProfiles_ResetTrappedError
  - Precondition: Set TrappedError to TRUE to simulate an error-armed state.
  - Action: Call CheckErrReportingOnBadProfiles().
  - Postconditions:
    - TrappedError must be FALSE after the call (the code resets the error state).
  - Rationale: Validates the side-effect that ensures the error state is cleared after reporting.
*/
static void test_CheckErrReportingOnBadProfiles_ResetTrappedError(void)
{
    // Arrange: simulate an active error state
    TrappedError = TRUE;

    // Act: exercise the focal method
    cmsInt32Number rc = CheckErrReportingOnBadProfiles();

    // Assert: error state should be reset
    EXPECT_TRUE(TrappedError == FALSE, "TrappedError should be reset to FALSE after CheckErrReportingOnBadProfiles");

    // Optional: ensure rc is a non-negative value (no crash and valid return)
    EXPECT_TRUE(rc >= 0, "CheckErrReportingOnBadProfiles returns non-negative rc");
}

/*
  Test 2: CheckErrReportingOnBadProfiles_ReturnValueMatchesCheckBadProfiles
  - Rationale: The wrapper should forward the same rc as a direct CheckBadProfiles() call,
    since the error handling is orthogonal to the computation of the return code.
  - Approach:
    - Compute rcDirect = CheckBadProfiles()
    - Compute rcWrapped = CheckErrReportingOnBadProfiles()
    - Assert rcDirect == rcWrapped
*/
static void test_CheckErrReportingOnBadProfiles_ReturnValueMatchesCheckBadProfiles(void)
{
    cmsInt32Number rcDirect = CheckBadProfiles();
    cmsInt32Number rcWrapped = CheckErrReportingOnBadProfiles();

    EXPECT_TRUE(rcDirect == rcWrapped,
                "CheckErrReportingOnBadProfiles should return the same rc as direct CheckBadProfiles");
}

/*
  Test 3: CheckErrReportingOnBadProfiles_MultipleCallsDoesNotCrash
  - Rationale: Ensure stability over repeated invocations.
  - Approach:
    - Call the focal function multiple times, resetting TrappedError before each call.
    - Validate that rc is non-negative and TrappedError is reset after each call.
*/
static void test_CheckErrReportingOnBadProfiles_MultipleCallsDoesNotCrash(void)
{
    for (int i = 0; i < 5; ++i) {
        TrappedError = FALSE;
        cmsInt32Number rc = CheckErrReportingOnBadProfiles();

        if (rc < 0) {
            fprintf(stderr, "FAIL: Iteration %d produced negative rc (%d)\n", i, static_cast<int>(rc));
            ++g_failed_tests;
            // Do not early-return; continue exercising other iterations
        }

        if (TrappedError != FALSE) {
            fprintf(stderr, "FAIL: Iteration %d left TrappedError != FALSE after call\n", i);
            ++g_failed_tests;
        } else {
            fprintf(stdout, "PASS: Iteration %d completed with TrappedError reset\n", i);
        }
    }

    // If any iteration failed, the macro prints a fail message above.
    // We still emit a summary line for this test.
    if (g_failed_tests == 0) {
        fprintf(stdout, "PASS: All iterations of CheckErrReportingOnBadProfiles_MultipleCallsDoesNotCrash completed successfully\n");
    } else {
        fprintf(stderr, "NOTE: Some iterations in CheckErrReportingOnBadProfiles_MultipleCallsDoesNotCrash failed\n");
    }
}

/*
  Main entry point: run all tests and report a final summary.
*/
int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    fprintf(stdout, "Starting unit tests for CheckErrReportingOnBadProfiles...\n");

    test_CheckErrReportingOnBadProfiles_ResetTrappedError();
    test_CheckErrReportingOnBadProfiles_ReturnValueMatchesCheckBadProfiles();
    test_CheckErrReportingOnBadProfiles_MultipleCallsDoesNotCrash();

    if (g_failed_tests == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", g_failed_tests);
        return 1;
    }
}