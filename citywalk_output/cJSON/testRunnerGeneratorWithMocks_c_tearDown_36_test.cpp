/*
Unit test suite for the focal method tearDown() found in testRunnerGeneratorWithMocks.c

Overview:
- The focal function tearDown() simply assigns CounterTeardown = 1.
- We test that no matter the initial value of CounterTeardown, after calling tearDown() the value becomes 1.
- We implement a small, self-contained test harness in C++11 (no GTest) that calls the C function tearDown through C linkage.
- We cover representative scenarios:
  1) Initial value 0
  2) Initial value negative
  3) Initial value positive
  4) Repeated invocations to ensure idempotence toward value 1

Notes on dependencies (from <FOCAL_CLASS_DEP> context):
- The global symbol CounterTeardown is defined in the C test context (likely in Defs.h or the C file). We access it with C linkage in the C++ test code.
- The focal function tearDown() is a C function with C linkage. We declare it in an extern "C" block to link correctly from C++.
- This test suite assumes a non-GTest environment and uses simple, non-terminating assertions that accumulate failures and report at the end.

Usage:
- Compile this test file together with testRunnerGeneratorWithMocks.c (or its linked object) under a C++11 capable compiler.
- The test harness prints PASS/FAIL messages per test and a final summary.

*/

// Include standard I/O for reporting
#include <unity.h>
#include <mockMock.h>
#include <iostream>
#include <cstdio>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// Declare C linkage for the focal function and its global state
extern "C" {
    // Focal method under test
    void tearDown(void);

    // Global state manipulated by tearDown (as seen in the focal file)
    extern int CounterTeardown;
}

// Simple, non-terminating assertion helper
// Returns true if assertion passes, false otherwise
static bool assert_equal_int(int expected, int actual, const char* test_name) {
    if (expected == actual) {
        std::cout << "[PASS] " << test_name << std::endl;
        return true;
    } else {
        std::cerr << "[FAIL] " << test_name
                  << " | Expected: " << expected
                  << " | Actual: " << actual << std::endl;
        return false;
    }
}

// Global test failure counter to accumulate results
static int g_totalFailures = 0;

// Macro-like helper to aggregate failures and print a summary-friendly message
#define RUN_TEST(test_func) \
    do { \
        test_func(); \
    } while (0)

// Test 1: tearDown should set CounterTeardown to 1 when initial value is 0
static void test_teardown_sets_counter_to_one_from_zero(void) {
    // Arrange
    CounterTeardown = 0; // ensure a known starting state
    // Act
    tearDown();
    // Assert
    const bool ok = assert_equal_int(1, CounterTeardown,
        "tearDown should set CounterTeardown to 1 when starting from 0");
    if (!ok) g_totalFailures++;
    // Teardown not required for C units; state is tested via global var
}

// Test 2: tearDown should set CounterTeardown to 1 when initial value is negative
static void test_teardown_sets_counter_to_one_from_negative(void) {
    // Arrange
    CounterTeardown = -5;
    // Act
    tearDown();
    // Assert
    const bool ok = assert_equal_int(1, CounterTeardown,
        "tearDown should set CounterTeardown to 1 when starting from negative value");
    if (!ok) g_totalFailures++;
}

// Test 3: tearDown should set CounterTeardown to 1 when initial value is positive
static void test_teardown_sets_counter_to_one_from_positive(void) {
    // Arrange
    CounterTeardown = 42;
    // Act
    tearDown();
    // Assert
    const bool ok = assert_equal_int(1, CounterTeardown,
        "tearDown should set CounterTeardown to 1 when starting from positive value");
    if (!ok) g_totalFailures++;
}

// Test 4: Repeated invocations should keep CounterTeardown at 1 after tearDown
static void test_teardown_is_idempotent_after_multiple_calls(void) {
    // Arrange
    CounterTeardown = 9;
    // Act
    tearDown();
    // Assert first invocation
    const bool first_ok = assert_equal_int(1, CounterTeardown,
        "First tearDown should set CounterTeardown to 1 (from 9)");
    if (!first_ok) g_totalFailures++;

    // Act again to check idempotence
    tearDown();
    // Assert second invocation
    const bool second_ok = assert_equal_int(1, CounterTeardown,
        "Second tearDown should still set CounterTeardown to 1");
    if (!second_ok) g_totalFailures++;
}

// Main function to run tests
int main() {
    std::cout << "Running tearDown() unit tests (C intervention via extern):" << std::endl;

    // Execute tests
    RUN_TEST(test_teardown_sets_counter_to_one_from_zero);
    RUN_TEST(test_teardown_sets_counter_to_one_from_negative);
    RUN_TEST(test_teardown_sets_counter_to_one_from_positive);
    RUN_TEST(test_teardown_is_idempotent_after_multiple_calls);

    // Summary
    if (g_totalFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_totalFailures << " test(s) failed." << std::endl;
        return 1;
    }
}