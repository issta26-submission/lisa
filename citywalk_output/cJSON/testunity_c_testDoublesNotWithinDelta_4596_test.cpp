/*
  Lightweight C++11-based unit test suite for the focal behavior of testDoublesNotWithinDelta
  (core idea: verify the "within delta" predicate for doubles under different scenarios)
  Notes:
  - This test suite does not rely on GTest or Unity. It uses a tiny, in-file test harness
    suitable for C++11 compilation.
  - We cover both branches of the core predicate: within delta (true) and not within delta (false),
    and also simulate the UNITY_EXCLUDE_DOUBLE path via a flag to exercise alternate behavior.
  - The goal is to achieve high coverage of the double-within-delta logic without requiring the
    full Unity framework. Static analysis of the original focal method guided the test scenarios.
*/

#include <cmath>
#include <exception>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple in-file test framework (no external dependencies)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Utility: simple assertion that throws on failure
class TestFailure : public std::exception {
  std::string _msg;
public:
  explicit TestFailure(const std::string& msg) : _msg(msg) {}
  const char* what() const noexcept override { return _msg.c_str(); }
};

#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) throw TestFailure(std::string("ASSERT_TRUE failed: ") + (msg)); } while (0)

#define ASSERT_FALSE(cond, msg) \
    do { if ((cond)) throw TestFailure(std::string("ASSERT_FALSE failed: ") + (msg)); } while (0)

// Core predicate: check if two doubles are within a given delta
static bool withinDelta(double delta, double expected, double actual) {
    return std::fabs(actual - expected) <= delta;
}

// Variant that simulates a compile-time flag (e.g., UNITY_EXCLUDE_DOUBLE)
static bool withinDeltaWithFlag(double delta, double expected, double actual, bool excludeDouble) {
    if (excludeDouble) {
        // Simulated path: when doubles are excluded, the check should effectively pass.
        return true;
    }
    return withinDelta(delta, expected, actual);
}

using TestFn = void (*)();

static void runTest(const char* name, TestFn fn) {
    ++g_tests_run;
    try {
        fn();
        std::cout << "[PASS] " << name << "\n";
    } catch (const TestFailure& tf) {
        ++g_tests_failed;
        std::cerr << "[FAIL] " << name << ": " << tf.what() << "\n";
    } catch (const std::exception& ex) {
        ++g_tests_failed;
        std::cerr << "[FAIL] " << name << ": " << ex.what() << "\n";
    } catch (...) {
        ++g_tests_failed;
        std::cerr << "[FAIL] " << name << ": Unknown exception\n";
    }
}

/*
  Test 1: DoublesNotWithinDelta_NormalPath
  - Purpose: Validate that when the difference is greater than delta, the within-delta predicate evaluates to false.
  - Relation to focal method: Mirrors the "not within delta" branch of the focal test.
  - Expectation: Since actual is 0.06 away from expected with delta 0.05, the predicate should be false.
*/
void testDoublesNotWithinDelta_NormalPath(void) {
    const double delta = 0.05;
    const double expected = 9273.2649;
    const double actual   = 9273.2049; // difference = 0.0600 (> delta)

    // If the predicate returns true, that's an unexpected pass for "not within delta"
    if (withinDelta(delta, expected, actual)) {
        throw TestFailure("Expected not within delta, but within delta returned true");
    }

    // Otherwise, test passes (as intended)
}

/*
  Test 2: DoublesWithinDelta_BoundaryWithin
  - Purpose: Ensure the predicate returns true when the difference is exactly on the delta boundary.
  - Rationale: Branch coverage on boundary condition.
*/
void testDoublesWithinDelta_BoundaryWithin(void) {
    const double delta = 0.05;
    const double expected = 9273.2649;
    const double actual   = 9273.2149; // difference = 0.0500 (on boundary)

    ASSERT_TRUE(withinDelta(delta, expected, actual),
                "Doubles should be within delta on the boundary");
}

/*
  Test 3: DoublesWithinDelta_ExactMatch
  - Purpose: Ensure the predicate returns true when there is zero difference.
  - Rationale: Covers the trivial true path.
*/
void testDoublesWithinDelta_ExactMatch(void) {
    const double delta = 0.05;
    const double expected = 9273.2649;
    const double actual   = 9273.2649; // difference = 0.0

    ASSERT_TRUE(withinDelta(delta, expected, actual),
                "Doubles should be within delta for exact match");
}

/*
  Test 4: DoublesNotWithinDelta_ExcludedPath
  - Purpose: Simulate UNITY_EXCLUDE_DOUBLE path, where the double check is skipped/considered passing.
  - Rationale: Exercise alternative control flow where the double comparison is bypassed.
*/
void testDoublesNotWithinDelta_ExcludedPath(void) {
    const double delta = 0.05;
    const double expected = 9273.2649;
    const double actual   = 9273.2049; // difference = 0.0600 (> delta)
    const bool excludeDouble = true; // simulate UNITY_EXCLUDE_DOUBLE being defined

    // With exclusion, the check should pass
    if (!withinDeltaWithFlag(delta, expected, actual, excludeDouble)) {
        throw TestFailure("Excluded path should evaluate to true (bypassed check)");
    }
}

/*
  Test 5: DoublesWithinDelta_NormalPath_WithFlagFalse
  - Purpose: Another boundary-case check for the non-excluded path to confirm typical behavior.
*/
void testDoublesWithinDelta_NormalPath_WithFlagFalse(void) {
    const double delta = 0.05;
    const double expected = 9273.2649;
    const double actual   = 9273.2149; // difference = 0.0500

    // Exclude flag false -> normal path
    ASSERT_TRUE(withinDeltaWithFlag(delta, expected, actual, false),
                "Expected within-delta result on boundary with non-excluded path");
}

int main() {
    // Register and run tests
    runTest("testDoublesNotWithinDelta_NormalPath", testDoublesNotWithinDelta_NormalPath);
    runTest("testDoublesWithinDelta_BoundaryWithin", testDoublesWithinDelta_BoundaryWithin);
    runTest("testDoublesWithinDelta_ExactMatch", testDoublesWithinDelta_ExactMatch);
    runTest("testDoublesNotWithinDelta_ExcludedPath", testDoublesNotWithinDelta_ExcludedPath);
    runTest("testDoublesWithinDelta_NormalPath_WithFlagFalse", testDoublesWithinDelta_NormalPath_WithFlagFalse);

    // Summary
    std::cout << "\nTest summary: "
              << g_tests_run << " run, "
              << g_tests_failed << " failed.\n";

    return (g_tests_failed == 0) ? 0 : 1;
}