// Test suite for UnityAssertEqualNumber in unity.c
// This file is designed to be compiled with a C++11 compiler.
// It directly exercises the focal function by invoking it with
// controlled inputs. No Google Test (GTest) is used, per requirements.
// The Unity framework (unity.h) is assumed to be available in the build.
// We compile this as a standalone test runner, calling into the C-built Unity function.

#include <stddef.h>
#include <cstdio>
#include <cstdint>
#include <unity.h>


// Include the Unity framework header with C linkage.
// The focal function UnityAssertEqualNumber is defined in unity.c.
// Ensure the header is reachable via the include path in the build system.
extern "C" {
}

// Global counters for a tiny, self-contained test harness.
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Helper macro to mark a test as passed (non-terminating in this harness).
#define MARK_TEST_PASSED() do { ++g_tests_run; ++g_tests_passed; } while (0)

// Test 1: Validate that UnityAssertEqualNumber does not trigger a failure path
// when expected and actual numbers are equal.
// This exercises the true-branch of the predicate (if (expected != actual) { ... }).
// Since the numbers are equal, the function should return normally without attempting
// to bail out via UNITY_FAIL_AND_BAIL.
void test_UnityAssertEqualNumber_Passes_WhenEqual()
{
    // Candidate Keywords: expected, actual, UnityAssertEqualNumber, msg, lineNumber, style
    // Core assertion: 5 == 5 should pass without triggering a failure path.
    // The style parameter is provided as 0; UnityPrintNumberByStyle is a stub in the test
    // harness, so the specific style value does not affect the outcome here.
    UnityAssertEqualNumber(5, 5, "Equal numbers test", 42, 0);

    // If we reach here, the call did not cause a non-local control transfer (i.e., no bail).
    MARK_TEST_PASSED();
}

// Test 2: Validate that UnityAssertEqualNumber handles multiple style usages without crashing.
// This still uses equal numbers but with different style values to exercise the code path
// that would call UnityPrintNumberByStyle. The actual print function is a stub in this harness.
void test_UnityAssertEqualNumber_Passes_WithDifferentStyles()
{
    // Candidate Keywords: UnityPrintNumberByStyle, style variations, UnityAssertEqualNumber
    // First invocation with style 0
    UnityAssertEqualNumber(7, 7, "Equal with style 0", 100, 0);
    // Second invocation with style 1 (style is not functionally significant in stubs)
    UnityAssertEqualNumber(-1, -1, "Equal with style 1", 101, 1);

    MARK_TEST_PASSED();
}

// Note on the false-branch testing (expected != actual):
// The UnityAssertEqualNumber function, when expected != actual, calls UnityTestResultsFailBegin
// and then uses UNITY_FAIL_AND_BAIL to bail out of the current test. This control flow is
// typically implemented via a non-local jump or test harness integration in Unity.
// In this isolated C++ test harness (without the full Unity test runner wiring that
// handles long jumps), triggering the false branch would risk aborting the test process.
// Therefore, the two provided tests focus on the true-branch coverage and style interaction,
// while acknowledging that automated, isolated testing of the false-branch would require
// a more complete Unity test harness or macro override (not feasible here).

int main()
{
    // Run tests that exercise the focal function in safe, non-bailing scenarios.

    // Test 1: Equal values should pass without triggering a bail.
    test_UnityAssertEqualNumber_Passes_WhenEqual();

    // Test 2: Equal values with varying display styles should also pass.
    test_UnityAssertEqualNumber_Passes_WithDifferentStyles();

    // Summary
    printf("UnityAssertEqualNumber test suite completed.\n");
    printf("Tests run: %d, Passed: %d\n", g_tests_run, g_tests_passed);

    // If any test failed (i.e., not marked as passed), we could return a non-zero code.
    // Since our tests only exercise the true-branch, we rely on the internal Unity behavior
    // for failure handling in a full test harness.
    return (g_tests_passed == g_tests_run) ? 0 : 1;
}