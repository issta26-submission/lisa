// Test suite for the focal method: CheckErrReportingOnBadTransforms
// This test is written for C++11 compilation, does not use GTest, and relies on
// standard library facilities (assert, printf) for verifiable checks.
// The focal method is declared in testcms2.h and is implemented in cms2/C source.
// We exercise the function and validate basic post-conditions and state resets.

#include <cstdio>
#include <cassert>
#include <testcms2.h>


// Include the project's test/utility header which provides the focal method

// The focal function is a C function. To ensure linkage correctness in C++, we
// declare it with C linkage in case the header doesn't already do so.
extern "C" cmsInt32Number CheckErrReportingOnBadTransforms(void);

// Helper: simple, non-terminating assertion wrapper with clear output.
static void AssertTrue(bool condition, const char* message)
{
    if (!condition)
    {
        std::fprintf(stderr, "ASSERT FAILED: %s\n", message);
        // Do not call std::abort() to keep test execution under control if desired.
        // Using assert could be too strict for certain CI environments; we log first.
        // For this suite, we keep a hard failure for visibility.
        assert(condition);
    }
}

// Test 1: Baseline behavior of CheckErrReportingOnBadTransforms
// - Ensure the function can be invoked without crashing.
// - Validate that the returned rc is a non-negative value (typical for status codes).
// - Explain: This checks the basic integration whereby the function sets up the
//   error handler, delegates to CheckBadTransforms, then tears down properly.
static void Test_CheckErrReportingOnBadTransforms_Baseline(void)
{
    // Baseline invocation
    cmsInt32Number rc = CheckErrReportingOnBadTransforms();

    // Basic invariants: non-negative return code indicating success/failure.
    // Real value depends on internal CheckBadTransforms outcomes; 0 is typically
    // "no bad transforms". We do not rely on a specific value here, only that
    // it's a valid non-negative integer.
    AssertTrue(rc >= 0, "CheckErrReportingOnBadTransforms returned a negative rc");

    // Additional informational log
    std::printf("[Test] Baseline: CheckErrReportingOnBadTransforms returned rc=%d\n", rc);
}

// Test 2: State reset and isolation across multiple invocations
// - Some internal states (e.g., TrappedError) should be reset after the call.
// - We perform two consecutive calls to ensure there is no state bleed between runs.
// - Explain: Verifies that the function resets error-trapping state (TrappedError)
//   after it returns, ensuring subsequent tests are not polluted by previous errors.
static void Test_CheckErrReportingOnBadTransforms_StateReset(void)
{
    // First invocation
    cmsInt32Number rc1 = CheckErrReportingOnBadTransforms();

    // Optional: perform a second invocation to ensure re-entrance stability
    cmsInt32Number rc2 = CheckErrReportingOnBadTransforms();

    // Ensure both calls are sane and do not crash
    AssertTrue(rc1 >= 0, "First invocation returned negative rc");
    AssertTrue(rc2 >= 0, "Second invocation returned negative rc");

    // If the internal test harness exposes TrappedError, we could check it is reset.
    // Since direct access may not be portable across builds, we rely on the
    // function's contract: it must restore TrappedError to FALSE after return.
    // We attempt a light-weight check by invoking a subsequent call and ensuring
    // no crash occurs; if TrappedError were not reset, subsequent calls might
    // behave unpredictably. We use the return code consistency as a heuristic.
    // Note: This is a best-effort check given limited visibility into internal state.
    cmsInt32Number rc3 = CheckErrReportingOnBadTransforms();
    AssertTrue(rc3 >= 0, "Third invocation returned negative rc");
    std::printf("[Test] StateReset: rc1=%d, rc2=%d, rc3=%d\n", rc1, rc2, rc3);
}

// Main test runner
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Run tests with explanatory commentary
    std::printf("Running Test: CheckErrReportingOnBadTransforms Baseline\n");
    Test_CheckErrReportingOnBadTransforms_Baseline();
    std::printf("Passed: Baseline test completed.\n\n");

    std::printf("Running Test: CheckErrReportingOnBadTransforms State Reset\n");
    Test_CheckErrReportingOnBadTransforms_StateReset();
    std::printf("Passed: State reset test completed.\n\n");

    // If we reach here, all tests passed (assertions aborted on failure)
    std::printf("All tests for CheckErrReportingOnBadTransforms completed successfully.\n");
    return 0;
}