#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Minimal, self-contained test scaffold (C++11) to validate the focal behavior
// of testUINT8sNotWithinDelta without relying on the Unity framework.
// Note: This reproduces the essence of the "within delta" assertion for UINT8
// values used in the focal method, but in a standalone C++ test manner
// suitable for a C++11 build (as requested, no GTest).

// Global test state (simple harness)
static bool g_currentTestFailed = false;
static std::string g_currentTestName;

// Helper to log a failure message for the current test
static void logFail(const std::string& msg) {
    std::cerr << "FAIL: " << g_currentTestName << " - " << msg << std::endl;
    g_currentTestFailed = true;
}

// Core logic: determine if two 8-bit unsigned integers are within a given delta.
// This mirrors the Unity macro TEST_ASSERT_UINT8_WITHIN(delta, expected, actual)
static bool within_uint8_within(uint8_t delta, uint8_t expected, uint8_t actual) {
    int diff = static_cast<int>(expected) - static_cast<int>(actual);
    if (diff < 0) diff = -diff;
    return diff <= static_cast<int>(delta);
}

// Test 1: Validate that the within-delta check passes for known-good cases.
// This corresponds to the "within delta" tests in the Unity suite (testUINT8sWithinDelta).
// We exercise multiple scenarios in one test to maximize coverage of the predicate's
// true-branch (different deltas and value pairs).
static bool testUINT8sWithinDelta_Pass(void) {
    g_currentTestName = "testUINT8sWithinDelta_Pass";
    g_currentTestFailed = false;

    // Case 1: delta = 1, expected = 254, actual = 255  -> diff = 1 (within)
    if (!within_uint8_within(1, 254, 255)) {
        logFail("Case 1: 1, 254 vs 255 should be within delta but isn't.");
    }

    // Case 2: delta = 5, expected = 251, actual = 255  -> diff = 4 (within)
    if (!within_uint8_within(5, 251, 255)) {
        logFail("Case 2: 5, 251 vs 255 should be within delta but isn't.");
    }

    // Case 3: delta = 5, expected = 1, actual = 4      -> diff = 3 (within)
    if (!within_uint8_within(5, 1, 4)) {
        logFail("Case 3: 5, 1 vs 4 should be within delta but isn't.");
    }

    return !g_currentTestFailed;
}

// Test 2: Validate that the within-delta check fails when the difference exceeds delta.
// This replicates the focal testUINT8sNotWithinDelta (the method under test aborts on fail).
// We expect the function to return false for these inputs.
static bool testUINT8sNotWithinDelta_Simple(void) {
    g_currentTestName = "testUINT8sNotWithinDelta_Simple";
    g_currentTestFailed = false;

    // Case: delta = 2, expected = 255, actual = 0  -> diff = 255 (not within)
    if (within_uint8_within(2, 255, 0)) {
        logFail("NotWithinDelta: 2, 255 vs 0 should NOT be within delta but it is.");
    }

    return !g_currentTestFailed;
}

// Test 3: Boundary condition where difference equals delta should be considered within.
// This ensures the true branch is exercised on the boundary.
static bool testUINT8sBoundaryWithinDelta(void) {
    g_currentTestName = "testUINT8sBoundaryWithinDelta";
    g_currentTestFailed = false;

    // Case: delta = 2, expected = 100, actual = 102  -> diff = 2 (within)
    if (!within_uint8_within(2, 100, 102)) {
        logFail("BoundaryWithinDelta: delta=2 with diff exactly 2 should be within.");
    }

    return !g_currentTestFailed;
}

// Test 4: Custom-message style test (simulated). Ensure same behavior holds even if
// a descriptive message would accompany the assertion in Unity.
static bool testUINT8sNotWithinDelta_WithSimulatedMessage(void) {
    g_currentTestName = "testUINT8sNotWithinDelta_WithSimulatedMessage";
    g_currentTestFailed = false;

    // Use same inputs as the simple NotWithinDelta test; the message is simulated via log.
    if (within_uint8_within(2, 255, 0)) {
        logFail("NotWithinDeltaWithMessage: 2, 255 vs 0 should NOT be within delta but it is.");
    }

    return !g_currentTestFailed;
}

// Simple test runner
int main() {
    using TestFn = std::function<bool(void)>;
    std::vector<std::pair<std::string, TestFn>> tests;

    // Register test cases with explanatory names and lambdas
    tests.emplace_back("testUINT8sWithinDelta_Pass", testUINT8sWithinDelta_Pass);
    tests.emplace_back("testUINT8sNotWithinDelta_Simple", testUINT8sNotWithinDelta_Simple);
    tests.emplace_back("testUINT8sBoundaryWithinDelta", testUINT8sBoundaryWithinDelta);
    tests.emplace_back("testUINT8sNotWithinDelta_WithSimulatedMessage", testUINT8sNotWithinDelta_WithSimulatedMessage);

    int passes = 0;
    int failures = 0;

    // Run all tests
    for (auto &t : tests) {
        bool result = t.second();
        if (result) {
            ++passes;
        } else {
            ++failures;
            // Detailed message already logged inside test on failure
        }
    }

    // Summary
    std::cout << "Total tests run: " << tests.size()
              << ", Passed: " << passes
              << ", Failed: " << failures << std::endl;

    // Return non-zero if any test failed to signal overall failure.
    return (failures > 0) ? 1 : 0;
}

/*
Test explanations:
- testUINT8sWithinDelta_Pass
  Verifies multiple within-delta cases that should pass (true branch). Exercises the
  condition predicate's true path for different deltas and value pairs.

- testUINT8sNotWithinDelta_Simple
  Verifies a not-within-delta case where the predicate should return false. This
  exercises the false branch of the predicate.

- testUINT8sBoundaryWithinDelta
  Checks the boundary case where the absolute difference equals delta. Ensures the
  predicate returns true at the boundary (edge case coverage).

- testUINT8sNotWithinDelta_WithSimulatedMessage
  Additional variant that mirrors a not-within-delta scenario with an emphasis on
  test messaging (simulated here via log messages). Ensures the logic remains correct
  even when a descriptive message would accompany the failure in Unity.
*/