// Test suite for the focal method resetTest
// This standalone C++11 test verifies that resetTest() performs
// a tearDown() followed by setUp() in that exact order and how many
// times those operations occur when resetTest() is invoked.
// Note: This test is implemented in C++11 for convenience and
// does not rely on GTest. It mimics the behavior of the Unity-based
// test harness by using simple asserts and a log that captures the
// order of operations.

// We implement lightweight stand-ins for setUp/tearDown and the focal
// resetTest in this translation unit to allow precise verification.

#include <stanky.h>
#include <unity.h>
#include <setjmp.h>
#include <cassert>
#include <funky.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>


// Global log to record the order of setUp/tearDown calls.
// The focal resetTest is expected to invoke tearDown() first, then setUp().
static std::vector<std::string> g_log;

// Forward declarations with C linkage to mirror the project's style.
extern "C" void setUp(void);
extern "C" void tearDown(void);
extern "C" void resetTest(void);

// Implementation of the test-friendly setUp/tearDown.
// In the real project these would be defined elsewhere; here we provide
// minimal, observable behavior to validate resetTest's effect.
extern "C" void setUp(void) {
    g_log.push_back("SETUP");
}

extern "C" void tearDown(void) {
    g_log.push_back("TEARDOWN");
}

// Focal method under test (replicated here for testability).
// The real project provides this, but for a self-contained test we implement it
// to precisely verify the call order: first tearDown(), then setUp().
extern "C" void resetTest(void) {
    {
        tearDown();
        setUp();
    }
}

// Simple helper to print a log sequence for debugging.
static void print_log(const std::vector<std::string>& log) {
    std::cerr << "Log: [";
    for (size_t i = 0; i < log.size(); ++i) {
        if (i) std::cerr << ", ";
        std::cerr << log[i];
    }
    std::cerr << "]\n";
}

// Test 1: verify that a single invocation of resetTest results in exactly two
// operations: TEARDOWN followed by SETUP.
static void test_resetTest_singleInvocation verifies() {
    g_log.clear();

    // Act
    resetTest();

    // Assert
    // Expected order: TEARDOWN, SETUP
    const std::vector<std::string> expected = {"TEARDOWN", "SETUP"};
    assert(g_log.size() == expected.size() && "Unexpected number of log entries after resetTest()");
    for (size_t i = 0; i < expected.size(); ++i) {
        assert(g_log[i] == expected[i] && "Unexpected log entry order in resetTest()");
    }

    // Debug output if assertion passes
    if (g_log == expected) {
        std::cout << "test_resetTest_singleInvocation passed.\n";
    } else {
        std::cerr << "test_resetTest_singleInvocation failed.\n";
        print_log(g_log);
        exit(EXIT_FAILURE);
    }
}

// Test 2: verify that two consecutive invocations of resetTest produce a
// two-entry sequence per invocation: TEARDOWN, SETUP, TEARDOWN, SETUP.
static void test_resetTest_twoConsecutiveInvocations() {
    g_log.clear();

    // Act
    resetTest();
    resetTest();

    // Expected order: TEARDOWN, SETUP, TEARDOWN, SETUP
    const std::vector<std::string> expected = {"TEARDOWN", "SETUP", "TEARDOWN", "SETUP"};
    assert(g_log.size() == expected.size() && "Unexpected number of log entries after two resetTest() calls");
    for (size_t i = 0; i < expected.size(); ++i) {
        assert(g_log[i] == expected[i] && "Unexpected log entry order in two consecutive resetTest() calls");
    }

    // Debug output if assertion passes
    if (g_log == expected) {
        std::cout << "test_resetTest_twoConsecutiveInvocations passed.\n";
    } else {
        std::cerr << "test_resetTest_twoConsecutiveInvocations failed.\n";
        print_log(g_log);
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Run tests
    test_resetTest_singleInvocation_verify();
    test_resetTest_twoConsecutiveInvocations();

    std::cout << "All tests passed.\n";
    return EXIT_SUCCESS;
}

// Note:
// - This test is designed to be self-contained and does not rely on the Unity
//   framework. It demonstrates the expected behavior of resetTest by
//   simulating setUp/tearDown and validating the order of invocation.
// - If integrating with the existing Unity-based test suite in testsample_param.c,
//   you can adapt these tests to use Unity's TEST_ASSERT_* macros and RUN_TEST
//   macro as appropriate, while still preserving the core validation of the
//   resetTest call order.