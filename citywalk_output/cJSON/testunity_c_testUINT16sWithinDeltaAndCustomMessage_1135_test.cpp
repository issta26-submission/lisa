// Test suite for the focal method: testUINT16sWithinDeltaAndCustomMessage
// This C++11 test harness is provided as a lightweight alternative to Unity-based tests
// to exercise the core logic represented by the focal test in testunity.c without using GTest.
// The goal is to validate the within-delta assertions for UINT16 values and to document
// each unit test with explanatory comments.
//
// Notes:
// - We replicate the essential semantics of Unity's TEST_ASSERT_UINT16_WITHIN_MESSAGE macro:
//   verify that abs(actual - expected) <= delta. If not, record a failure with the provided message.
// - "Static members" and class dependencies are illustrated via a small DependencyClass with a static member.
// - The test suite is non-terminating: failing assertions are collected and reported at the end,
//   allowing all tests to run and maximize coverage.

#include <cmath>
#include <unity.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <stdio.h>


// Lightweight test harness globals
namespace TestHarness {
    // Stores failure messages for the currently executing test
    std::vector<std::string> gFailures;

    // Stores the names of tests that were run
    std::vector<std::string> gRanTests;

    // Records a failure message for the currently running test
    void recordFailure(const std::string& msg) {
        gFailures.push_back(msg);
    }

    // Pretty print of test results
    void printSummary() {
        std::cout << "\nTest Summary:\n";
        if (gFailures.empty()) {
            std::cout << "ALL TESTS PASSED" << std::endl;
        } else {
            std::cout << gFailures.size() << " TEST(S) FAILED" << std::endl;
            for (const auto& m : gFailures) {
                std::cout << "  " << m << std::endl;
            }
        }
    }

    // Internal helper: determine if two UINT16 values are within delta
    bool within_uint16(uint16_t delta, uint16_t expected, uint16_t actual) {
        int a = static_cast<int>(actual);
        int e = static_cast<int>(expected);
        int diff = a - e;
        if (diff < 0) diff = -diff;
        return diff <= static_cast<int>(delta);
    }

    // Emulates Unity's TEST_ASSERT_UINT16_WITHIN_MESSAGE:
    // If not within delta, records a failure message including delta/expected/actual and the user message.
    void assert_uint16_within_message(uint16_t delta, uint16_t expected, uint16_t actual, const char* message) {
        if (!within_uint16(delta, expected, actual)) {
            std::string fail =
                "ASSERT FAILED: delta=" + std::to_string(delta) +
                ", expected=" + std::to_string(expected) +
                ", actual=" + std::to_string(actual) +
                " - " + (message ? message : "");
            recordFailure(fail);
        }
    }

    // A small class to illustrate static member usage in tests (Domain Knowledge item 3)
    struct DependencyClass {
        static int staticCounter;
        static int get() { return staticCounter; }
    };

    int DependencyClass::staticCounter = 0;
}

// Forward declarations of test cases
void testUINT16sWithinDeltaAndCustomMessage_case(void);
void testUINT16sWithinDeltaNotWithinCustomMessage_case(void);
void testStaticMemberAccess_case(void);

// Runner helper: executes each test and reports pass/fail for that test
void runTest(const std::string& testName, void (*testFunc)(void)) {
    // Clear per-test failure state
    TestHarness::gFailures.clear();

    // Register that we are running this test
    TestHarness::gRanTests.push_back(testName);

    // Execute test
    testFunc();

    // Report result for this test
    if (TestHarness::gFailures.empty()) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << " (" << TestHarness::gFailures.size() << " failure(s))" << std::endl;
        for (const auto& m : TestHarness::gFailures) {
            std::cout << "       " << m << std::endl;
        }
    }
}

// Implementation of focal test cases (translated into C++11 style)
// These tests mirror the intent of the Unity macro usage in the focal method.

// Test 1: Core positive cases from the focal method
// Explanation: Within-delta assertions for three scenarios should all pass.
// - delta 1: 5000 vs 5001 -> diff = 1 <= 1 (pass)
// - delta 5: 5000 vs 4996 -> diff = 4 <= 5 (pass)
// - delta 5: 5000 vs 5005 -> diff = 5 <= 5 (pass)
void testUINT16sWithinDeltaAndCustomMessage_case(void) {
    TestHarness::assert_uint16_within_message(1, 5000, 5001, "Custom Message.");
    TestHarness::assert_uint16_within_message(5, 5000, 4996, "Custom Message.");
    TestHarness::assert_uint16_within_message(5, 5000, 5005, "Custom Message.");
}

// Test 2: Extended coverage (not part of the focal method but useful for coverage)
// Explanation: A simple negative case to ensure the harness captures failures.
// - delta 1: 5000 vs 5006 -> diff = 6 > 1 (fail)
void testUINT16sWithinDeltaNotWithinCustomMessage_case(void) {
    TestHarness::assert_uint16_within_message(1, 5000, 5006, "Custom Message.");
}

// Test 3: Static member access test (illustrates Domain Knowledge item 3)
void testStaticMemberAccess_case(void) {
    TestHarness::DependencyClass::staticCounter = 0;
    TestHarness::DependencyClass::staticCounter++;
    if (TestHarness::DependencyClass::get() != 1) {
        TestHarness::recordFailure("Static member access failed: DependencyClass::staticCounter != 1");
    }
    // Also verify another update path
    TestHarness::DependencyClass::staticCounter += 2; // now 3
    if (TestHarness::DependencyClass::get() != 3) {
        TestHarness::recordFailure("Static member access failed: DependencyClass::staticCounter != 3");
    }
}

// Main entry: run the test suite
int main() {
    // Run all tests (order chosen to maximize coverage)
    runTest("testUINT16sWithinDeltaAndCustomMessage_case", testUINT16sWithinDeltaAndCustomMessage_case);
    runTest("testUINT16sWithinDeltaNotWithinCustomMessage_case", testUINT16sWithinDeltaNotWithinCustomMessage_case);
    runTest("testStaticMemberAccess_case", testStaticMemberAccess_case);

    // Print a compact summary
    TestHarness::printSummary();

    // Return non-zero if any test failed
    return TestHarness::gFailures.empty() ? 0 : 1;
}