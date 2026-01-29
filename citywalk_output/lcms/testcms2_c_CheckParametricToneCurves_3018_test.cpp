// Test suite for CheckParametricToneCurves
// This test targets the focal method CheckParametricToneCurves located in the C file
// testcms2.c / related sources. The tests are written in C++11 without GoogleTest.
// The suite uses a lightweight, non-terminating assertion approach to maximize
// code coverage while avoiding abrupt termination.
//
// Domain knowledge and guidance applied:
// - The focal method runs a sequence of parametric tone curve validations via
//   various curve functions (Gamma, CIE122-1966, IEC 61966-3, IEC 61966-2.1, and
//   several param_5..param_8 variants, plus sigmoidal).
// - We expose the function via C linkage to the C test subject.
// - We cover true-path behavior and idempotence (consistent results across calls).
// - Static/internal helpers are not directly accessible; tests rely on the public
//   interface (CheckParametricToneCurves).
// - No private members are accessed; tests rely on the provided interface only.
//
// Candidate Keywords (core dependencies): Gamma, CIE122-1966, IEC61966_3, IEC61966_21,
// param_5, param_6, param_7, param_8, sigmoidal, CheckSingleParametric, CheckParametricToneCurves

#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Ensure the focal function is linked with C linkage
extern "C" int CheckParametricToneCurves(void);

// Lightweight test harness
static int gFailures = 0;

static void logPass(const char* testName, const char* detail) {
    printf("PASS: %s - %s\n", testName, detail);
}

static void logFail(const char* testName, const char* detail) {
    printf("FAIL: %s - %s\n", testName, detail);
    ++gFailures;
}

// Test 1: Verify that the focal function returns a non-zero (success) value.
// This corresponds to the "All OK" path in the function, where every parametric
// tone curve check succeeded.
static void Test_CheckParametricToneCurves_ReturnsNonZero() {
    const char* testName = "CheckParametricToneCurves_ReturnsNonZero";
    int rv = CheckParametricToneCurves();
    if (rv != 0) {
        logPass(testName, "Function returned non-zero (success) as expected.");
    } else {
        logFail(testName, "Expected non-zero return value, got 0.");
    }
}

// Test 2: Idempotence test - ensure repeated invocations produce consistent results.
// The function is deterministic for a given environment; this helps catch
// unexpected stateful side effects between calls.
static void Test_CheckParametricToneCurves_Idempotence() {
    const char* testName = "CheckParametricToneCurves_Idempotence";
    int first  = CheckParametricToneCurves();
    int second = CheckParametricToneCurves();
    if (first == second) {
        logPass(testName, "Consecutive invocations returned identical results.");
    } else {
        logFail(testName, "Consecutive invocations returned different results.");
    }
}

// Entry point for the lightweight test runner
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Silence unused parameter warnings

    printf("Starting CheckParametricToneCurves test suite (C++11)\n");

    // Run tests
    Test_CheckParametricToneCurves_ReturnsNonZero();
    Test_CheckParametricToneCurves_Idempotence();

    // Summary
    if (gFailures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", gFailures);
        return 1;
    }
}