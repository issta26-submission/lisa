// Floating point delta-within tests rewritten in C++11 without GTest.
// This test suite focuses on the focal method testFloatsWithinDelta found in testunity.c.
// The original relies on Unity macros (TEST_ASSERT_FLOAT_WITHIN, UNITY_EXCLUDE_FLOAT, etc.).
// Here we implement a small, self-contained test harness to validate the same semantics
// in a C++11 environment, with explanatory comments for each unit test.
//
// Key ideas (from Step 1):
// - Core dependent components: floating point delta predicate, exact values used in focal test.
// - Candidate Keywords: FLOAT, DELTA, WITHIN, values (187245.03485, 187245.03488, etc.), boundary checks.
//
// Domain knowledge notes (Step 3):
// - We implement a lightweight withinDelta(delta, a, b) predicate using std::fabs.
// - We provide two variants per the original: an Active variant (performs assertions) and
//   an Ignored/Excluded variant (simulates UNITY_EXCLUDE_FLOAT being defined).
// - Tests are non-terminating in the sense that they return a status instead of exiting
//   the process, allowing collection of multiple results in one run.

#include <cmath>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight assertion helper.
// Throws an exception on failure to allow test harness to collect failures.
class TestFailure : public std::exception {
public:
    explicit TestFailure(const std::string& m) : message(m) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

// Simple test harness to collect and print test results.
class SimpleTestRunner {
public:
    using TestFn = std::function<void()>;

    void addTest(const std::string& name, TestFn fn) {
        tests.emplace_back(name, std::move(fn));
    }

    int run() {
        int passed = 0;
        int failed = 0;
        for (auto& t : tests) {
            try {
                t.second(); // Execute test
                std::cout << "[PASSED] " << t.first << "\n";
                ++passed;
            } catch (const TestFailure& f) {
                std::cout << "[FAILED] " << t.first << " - " << f.what() << "\n";
                ++failed;
            } catch (const std::exception& e) {
                std::cout << "[FAILED] " << t.first << " - Exception: " << e.what() << "\n";
                ++failed;
            } catch (...) {
                std::cout << "[FAILED] " << t.first << " - Unknown exception\n";
                ++failed;
            }
        }
        std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";
        return failed;
    }

private:
    std::vector<std::pair<std::string, TestFn>> tests;
};

// Core utility: within delta predicate, mirroring TEST_ASSERT_FLOAT_WITHIN semantics.
static inline bool withinDelta(float delta, float expected, float actual) {
    // Use a straightforward absolute difference comparison.
    return std::fabs(expected - actual) <= delta;
}

// Global flag to simulate UNITY_EXCLUDE_FLOAT behavior (runtime-ish switch for testing).
// In the original Unity, UNITY_EXCLUDE_FLOAT is a compile-time guard.
// Here we emulate two test paths: Active path and Ignored path.
static bool g_ignoreFloatsPath = false;

// Focal method under test (re-implemented for this test suite).
// Matches semantics of the original testFloatsWithinDelta function in testunity.c.
// When g_ignoreFloatsPath is true, the function simulates UNITY_EXCLUDE_FLOAT by doing nothing.
static void testFloatsWithinDelta_ActivePath_impl() {
    // Original values from the focal test:
    //   1) delta 0.00003, 187245.03485 vs 187245.03488
    //   2) delta 1.0,     187245.0f vs 187246.0f
    //   3) delta 0.05,      9273.2549f vs 9273.2049f
    //   4) delta 0.007,    -726.93724f vs -726.94424f
    if (!withinDelta(0.00003f, 187245.03485f, 187245.03488f)) {
        throw TestFailure("First float within-delta assertion failed.");
    }
    if (!withinDelta(1.0f, 187245.0f, 187246.0f)) {
        throw TestFailure("Second float within-delta assertion failed.");
    }
    if (!withinDelta(0.05f, 9273.2549f, 9273.2049f)) {
        throw TestFailure("Third float within-delta assertion failed.");
    }
    if (!withinDelta(0.007f, -726.93724f, -726.94424f)) {
        throw TestFailure("Fourth float within-delta assertion failed.");
    }
}

// Ignored path: simulate UNITY_EXCLUDE_FLOAT by taking no action.
static void testFloatsWithinDelta_IgnoredPath_impl() {
    // Intentionally empty to mimic TEST_IGNORE() behavior.
    (void)0;
}

// Public wrapper for active path (used by test harness)
static void testFloatsWithinDelta_ActivePath() {
    g_ignoreFloatsPath = false;
    testFloatsWithinDelta_ActivePath_impl();
}

// Public wrapper for ignored path (used by test harness)
static void testFloatsWithinDelta_IgnoredPath() {
    g_ignoreFloatsPath = true;
    testFloatsWithinDelta_IgnoredPath_impl();
}

// Additional refined tests (Step 3: Test Case Refinement)
// These tests broaden coverage around the delta predicate without depending on the Unity framework.
// They are designed to be non-destructive and to run in a single process, reporting PASS/FAIL.

static void testFloatsWithinDelta_BoundaryCases() {
    // Boundary check: exactly at delta boundary should pass
    if (!withinDelta(0.5f, 1.0f, 1.0f + 0.5f)) {
        throw TestFailure("Boundary case failed: exact delta boundary should pass.");
    }
    // Slightly outside boundary should fail
    if (withinDelta(0.5f, 1.0f, 1.0f + 0.5001f)) {
        throw TestFailure("Boundary case failed: outside delta should fail.");
    }
}

static void testFloatsWithinDelta_BasicSmallDelta() {
    // Small delta checks guaranteed to pass with identical numbers
    if (!withinDelta(0.00001f, 0.0f, 0.0f)) {
        throw TestFailure("Tiny delta: identical values should pass.");
    }
    // Slight difference within small delta
    if (!withinDelta(0.00002f, 0.0f, 0.00001f)) {
        throw TestFailure("Tiny delta: small difference within delta should pass.");
    }
}

// Entry point that composes the test suite
int main() {
    SimpleTestRunner runner;

    // Core focal test: testFloatsWithinDelta
    // Step 2: We provide two variants to simulate both the active and the "excluded" paths.
    runner.addTest("testFloatsWithinDelta_ActivePath", []() {
        // Comment: This path exercises the four within-delta assertions exactly as in the focal code.
        testFloatsWithinDelta_ActivePath();
    });

    // Simulated UNITY_EXCLUDE_FLOAT path (ignore path)
    runner.addTest("testFloatsWithinDelta_IgnoredPath", []() {
        // Comment: This path simulates UNITY_EXCLUDE_FLOAT by performing no checks.
        testFloatsWithinDelta_IgnoredPath();
    });

    // Step 3: Refined test cases to increase coverage around the delta predicate
    runner.addTest("testFloatsWithinDelta_BoundaryCases", []() {
        testFloatsWithinDelta_BoundaryCases();
    });

    runner.addTest("testFloatsWithinDelta_BasicSmallDelta", []() {
        testFloatsWithinDelta_BasicSmallDelta();
    });

    // Run the suite and exit with the number of failures as exit code (convenient for CI)
    int failures = runner.run();

    // Return non-zero on failure to indicate test suite problems.
    return failures;
}

/*
Notes for integration and alignment with the provided focal method:
- The focal method testFloatsWithinDelta resides within testunity.c and uses Unity macros.
- The above C++ test suite does not rely on Unity; instead, it re-implements the delta-predicate
  semantics (withinDelta) and tests the same four values used in the original focal test.
- We preserved the two-path concept (Active vs Ignored) by using a global flag and two wrapper
  tests: testFloatsWithinDelta_ActivePath and testFloatsWithinDelta_IgnoredPath.
- Static members in the original code (e.g., SetToOneToFailInTearDown) are not applicable here,
  so they are not represented; this test suite focuses on the core float-within-delta logic
  and its coverage needs as described in the instructions.
- The tests are designed to be non-GTest, self-contained, and compilable with C++11.
- Explanatory comments accompany each unit test to describe purpose and expected behavior.
*/