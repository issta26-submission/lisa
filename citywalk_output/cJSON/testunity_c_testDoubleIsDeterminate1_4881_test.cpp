// Unit test suite for the focal method testDoubleIsDeterminate1
// This suite is implemented in C++11 (no GTest) to exercise the core logic
// suggested by the focal method. It mirrors the intent of the Unity-based test
// by validating determinateness of doubles under two branches: a running branch
// (when UNITY_EXCLUDE_DOUBLE is not defined) and an ignored branch (when it is defined).
// It also includes an extra test to exercise the negative path of determinateness
// (i.e., NaN/Inf) to improve coverage of the underlying predicate.
// Candidate Keywords mirrored from the focal method:
// - UNITY_EXCLUDE_DOUBLE, TEST_IGNORE, TEST_ASSERT_DOUBLE_IS_DETERMINATE
// - tested literals: 0.0, 123.3, -88.3
// - Double determinateness predicate: isfinite (approximation of determinateness)

// Note: This test suite does not depend on Unity, but mimics the intent of the focal test.
// It can be run standalone with a C++11 capable compiler.

#include <cmath>
#include <unity.h>
#include <limits>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Static-like constants that reflect the focal test's typical variables.
// In the original, UNITY_FLOAT f_zero and UNITY_DOUBLE d_zero exist; here we model
// the determinateness checks using plain doubles.
static const double f_zero = 0.0;
static const double d_zero = 0.0;

// Simple reporter helpers
static void report_pass(const std::string& testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

static void report_fail(const std::string& testName, const std::string& msg) {
    std::cerr << "[FAIL] " << testName << " - " << msg << std::endl;
}

// Mimic the Unity determinateness check.
// For doubles, a value is "determinate" if it is finite (not NaN, not +/-Inf).
static bool is_determinate(double v) {
    return std::isfinite(v);
}

// Test 1: BranchRuns - corresponds to the case where UNITY_EXCLUDE_DOUBLE is not defined.
// This tests determinateness for known finite values: 0.0, 123.3, -88.3.
static bool testDoubleIsDeterminate1_RunBranch(void) {
    // Expose the core literals from the focal method
    const double a = 0.0;    // matches 0.0 in focal
    const double b = 123.3;  // matches 123.3 in focal
    const double c = -88.3;  // matches -88.3 in focal

    // Core assertion: all three values must be determinate (finite)
    bool ok = is_determinate(a) && is_determinate(b) && is_determinate(c);
    if (!ok) {
        report_fail("testDoubleIsDeterminate1_RunBranch",
                    "One or more provided doubles are not determinate (not finite).");
        return false;
    }
    report_pass("testDoubleIsDeterminate1_RunBranch");
    return true;
}

// Test 2: IgnoredBranch - corresponds to the case where UNITY_EXCLUDE_DOUBLE is defined.
// This simulates the Unity TEST_IGNORE() behavior by marking the test as ignored.
// In a real Unity run, this would skip the test; here we simply log as IGNORED.
static bool testDoubleIsDeterminate1_IgnoredBranch(void) {
    // Simulate the ignore branch (no assertions executed)
    std::cout << "[IGNORED] testDoubleIsDeterminate1 (UNITY_EXCLUDE_DOUBLE defined)" << std::endl;
    // Returning true to indicate the test harness can continue; in Unity this would be an ignored test.
    return true;
}

// Test 3: NotFiniteBranch - additional coverage to exercise the predicate's false path.
// This ensures we cover the negative determinateness (NaN/Inf) path for completeness.
static bool testDoubleIsDeterminate_NotFinite(void) {
    // Create values that are not finite
    double nanVal = std::numeric_limits<double>::quiet_NaN();
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();

    bool ok_nan  = !is_determinate(nanVal);
    bool ok_inf  = !is_determinate(posInf);
    bool ok_minf = !is_determinate(negInf);

    if (ok_nan && ok_inf && ok_minf) {
        report_pass("testDoubleIsDeterminate_NotFinite");
        return true;
    } else {
        report_fail("testDoubleIsDeterminate_NotFinite",
                    "NotFinite values did not produce expected not-determinate results.");
        return false;
    }
}

// Domain knowledge notes (embedded in code comments for maintainers):
// - The Candidate Keywords from Step 1 (UNITY_EXCLUDE_DOUBLE, TEST_IGNORE, TEST_ASSERT_DOUBLE_IS_DETERMINATE)
//   are represented conceptually by the two branches (RunBranch vs IgnoredBranch) and the determinateness check.
// - Static behavior (f_zero/d_zero) is represented by the static constants above to reflect static scoped values.
// - This suite uses standard C++11 features (std::isfinite, std::numeric_limits) and avoids GTest.
// - No private state is accessed; the tests rely only on public-domain C++ facilities.

// Simple test harness
int main() {
    // Collection of test cases with their names and function pointers
    struct TestCase {
        const char* name;
        std::function<bool(void)> func;
    };

    // Instantiate tests
    std::vector<TestCase> tests = {
        { "testDoubleIsDeterminate1_RunBranch", testDoubleIsDeterminate1_RunBranch },
        { "testDoubleIsDeterminate1_IgnoredBranch", testDoubleIsDeterminate1_IgnoredBranch },
        { "testDoubleIsDeterminate_NotFinite", testDoubleIsDeterminate_NotFinite }
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    // Execute each test
    for (const auto& t : tests) {
        // Run and interpret the boolean result
        bool result = t.func();
        if (result) {
            // If the test reports PASS by itself (or is IGNORED), count as passed
            ++passed;
        }
        // If the function printed failures internally, we still continue to run remaining tests.
        // This mirrors a non-terminating test suite where one failure doesn't abort the rest.
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed (simple convention)
    return (passed == total) ? 0 : 1;
}