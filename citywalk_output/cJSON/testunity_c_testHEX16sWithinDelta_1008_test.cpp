/*
 * Test Suite: test_hex16_within_delta_suite.cpp
 * Purpose:
 *   Provide a focused, self-contained C++11 based test suite for the focal
 *   testHEX16sWithinDelta behavior (i.e., the concept of "HEX16 within Delta").
 *   This reproduces the core logic used by Unity's TEST_ASSERT_HEX16_WITHIN
 *   macro in a lightweight C++ test harness, enabling true/false coverage
 *   without requiring GTest or the Unity runtime.
 *
 * Step 1 (Program Understanding) - Candidate Keywords:
 *   - HEX16, within delta, absolute difference, 16-bit
 *   - delta, expected, actual
 *   - testHEX16sWithinDelta (focal method)
 *   - TEST_ASSERT_HEX16_WITHIN (conceptual macro)
 *   - 16-bit arithmetic, boundary behavior
 *
 * Step 2 (Unit Test Generation) - Test Suite Outline:
 *   - Validate three focal true cases (as in the original focal method):
 *       1) within delta of 1 for (5000, 5001)
 *       2) within delta of 5 for (5000, 4996)
 *       3) within delta of 5 for (5000, 5005)
 *   - Add tests to exercise false branches (cases that should fail the within-delta test).
 *   - Add boundary and edge-case tests (delta 0, delta > 0 with identical values, exact delta bound, etc).
 *   - Keep tests independent and non-destructive.
 *
 * Step 3 (Test Case Refinement) - Domain Knowledge Utilization:
 *   - Use simple unsigned 16-bit arithmetic with explicit absolute difference.
 *   - Do not assume wrap-around behavior unless explicitly required by the focal spec.
 *   - Provide descriptive comments for each test explaining intent and expectations.
 *   - Use portable C++11 facilities only (no GTest, no external libs).
 *
 * Notes:
 *   - This test is self-contained and does not rely on Unity. It demonstrates
 *     the core logical checks behind TEST_ASSERT_HEX16_WITHIN for 16-bit values.
 *   - To integrate with your existing CI, you can adapt this harness to use your
 *     preferred test runner, or export the hexWithinDelta function for reuse.
 */

#include <cmath>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Simple test harness (no GTest, no Unity). Lightweight assertions with
// explicit pass/fail reporting.
static int TestsRun = 0;
static int TestsFailed = 0;

// Convenience failure reporter
static void reportFailure(const std::string& testName, const char* file, int line, const std::string& msg) {
    std::cerr << "FAIL: " << testName << " (" << file << ":" << line << ") - " << msg << std::endl;
    ++TestsFailed;
}

// Macros to mimic basic assertion semantics
#define ASSERT_TRUE(cond, testName) do { \
    ++TestsRun; \
    if(!(cond)) { reportFailure(testName, __FILE__, __LINE__, "Condition evaluated to false"); } \
} while(0)

#define ASSERT_FALSE(cond, testName) do { \
    ++TestsRun; \
    if(cond) { reportFailure(testName, __FILE__, __LINE__, "Condition evaluated to true"); } \
} while(0)

// Compute absolute difference for 16-bit unsigned values.
// Note: This uses 32-bit intermediates to avoid overflow during subtraction.
static inline uint16_t abs16_diff(uint16_t a, uint16_t b) {
    int32_t diff = static_cast<int32_t>(a) - static_cast<int32_t>(b);
    if (diff < 0) diff = -diff;
    return static_cast<uint16_t>(diff);
}

// Core logic: whether actual is within delta of expected for HEX16 values.
// This mirrors the essential behavior of Unity's TEST_ASSERT_HEX16_WITHIN for
// non-wrap-around semantics.
static bool hex16WithinDelta(uint16_t delta, uint16_t expected, uint16_t actual) {
    uint16_t diff = abs16_diff(actual, expected);
    return diff <= delta;
}

// ---------------------------------------------------------------------------
// Test Case 1: Baseline positive cases from the focal method
//   These validate the three exact scenarios provided in testHEX16sWithinDelta:
//   (delta=1: 5000 vs 5001), (delta=5: 5000 vs 4996), (delta=5: 5000 vs 5005)
// Expected: all should pass (true)
static void testHEX16sWithinDelta_BaselinePasses(void) {
    // Within delta of 1
    ASSERT_TRUE(hex16WithinDelta(1, 5000, 5001), "HEX16 within delta with delta=1, expected=5000, actual=5001 should pass");
    // Within delta of 5 (lower bound)
    ASSERT_TRUE(hex16WithinDelta(5, 5000, 4996), "HEX16 within delta with delta=5, expected=5000, actual=4996 should pass");
    // Within delta of 5 (upper bound)
    ASSERT_TRUE(hex16WithinDelta(5, 5000, 5005), "HEX16 within delta with delta=5, expected=5000, actual=5005 should pass");
}

// ---------------------------------------------------------------------------
// Test Case 2: Boundary and edge-case coverage
//   - Exact delta boundary
//   - Delta zero with identical and different values
//   - Not within delta cases
static void testHEX16sWithinDelta_Boundaries(void) {
    // Exact delta boundary (difference equals delta)
    ASSERT_TRUE(hex16WithinDelta(5, 1000, 1005), "HEX16 within delta boundary should pass when diff == delta");
    // Delta zero with identical values should pass
    ASSERT_TRUE(hex16WithinDelta(0, 1234, 1234), "HEX16 within delta should pass when delta=0 and actual==expected");
    // Delta zero with different values should fail
    ASSERT_FALSE(hex16WithinDelta(0, 1234, 1235), "HEX16 within delta should fail when delta=0 and actual!=expected");
}

// ---------------------------------------------------------------------------
// Test Case 3: Negative/ false-branch coverage
//   Ensure cases that should fail do fail (i.e., NOT within delta)
static void testHEX16sWithinDelta_FalseCases(void) {
    // Not within delta for delta=1
    ASSERT_FALSE(hex16WithinDelta(1, 5000, 5006), "HEX16 within delta should fail for delta=1 when diff=6");
    // Not within delta for delta=0 with different values (already covered above, but explicit here)
    ASSERT_FALSE(hex16WithinDelta(0, 42, 43), "HEX16 within delta should fail for delta=0 with diff=1");
}

// ---------------------------------------------------------------------------
// Test Runner
static void runAllTests(void) {
    testHEX16sWithinDelta_BaselinePasses();
    testHEX16sWithinDelta_Boundaries();
    testHEX16sWithinDelta_FalseCases();
}

// Entry point
int main(void) {
    runAllTests();

    // Summary
    std::cout << "Tests run: " << TestsRun << ", Failures: " << TestsFailed << std::endl;
    // Return non-zero if any test failed
    return (TestsFailed == 0) ? 0 : 1;
}

/*
 * Explanation of test coverage:
 * - testHEX16sWithinDelta_BaselinePasses exercises the focal method's three true-branch cases,
 *   matching the original testHEX16sWithinDelta in testunity.c.
 * - testHEX16sWithinDelta_Boundaries ensures edge behavior:
 *     - diff exactly equals delta (true)
 *     - delta=0 with identical values (true)
 *     - delta=0 with different values (false)
 * - testHEX16sWithinDelta_FalseCases validates false branches to ensure failures occur
 *   when values are outside the allowed delta.
 *
 * This suite uses a minimal, self-contained test approach suitable for C++11 environments
 * without GTest or Unity. It can be extended to mirror more of the Unity test suite setup
 * (e.g., setup/teardown, static state) if needed for integration with an existing CI pipeline.
 *
 * Note: If you want to align with the original Unity-based test naming and macros, you can
 * adapt the hex16WithinDelta function to wrap the logic behind TEST_ASSERT_HEX16_WITHIN
 * and integrate with your existing Unity test runner. The core logical behavior remains the same.
 */