// Unit test suite for the focal method setUp() found in testRunnerGenerator.c
// This test suite is written in C++11 and does not rely on GoogleTest.
// It directly calls the C function setUp() and inspects the (extern) global
// CounterSetup to verify the expected behavior: setUp() should set CounterSetup to 1.

#include <unity.h>
#include <iostream>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// Linkage to the C function under test and external state.
// The focal method is:
// void setUp(void) { { CounterSetup = 1; } }
// We assume CounterSetup is a global variable defined in the tested C/C project.
extern "C" void setUp(void);
extern "C" int CounterSetup;

// ----------------------
// Test Case 1: Initializes CounterSetup to 1
// Before calling setUp(), CounterSetup is set to an arbitrary non-1 value.
// After setUp(), CounterSetup must be exactly 1.
// This ensures the primary side effect of setUp() is performed.
bool test_SetUp_InitializesCounter(void)
{
    CounterSetup = -42;  // arbitrary non-1 value to ensure change occurs
    setUp();
    return CounterSetup == 1;
}

// ----------------------
// Test Case 2: Resets previous value to 1
// If CounterSetup was previously 0 (or any value), calling setUp() should reset it to 1.
bool test_SetUp_ResetsPreviousValue(void)
{
    CounterSetup = 0;
    setUp();
    return CounterSetup == 1;
}

// ----------------------
// Test Case 3: Idempotence across multiple calls
// Calling setUp() multiple times should keep CounterSetup at 1 (no further changes).
bool test_SetUp_MultipleCallsStayOne(void)
{
    CounterSetup = 99;
    setUp();
    bool first = (CounterSetup == 1);
    setUp();
    bool second = (CounterSetup == 1);
    return first && second;
}

// ----------------------
// Simple test harness
// Runs the defined tests and reports pass/fail without terminating on first failure.
// This aligns with the requirement to use non-terminating assertions to maximize coverage.

int main(void)
{
    int total = 0;
    int passed = 0;
    int failed = 0;

    auto runTest = [&](const char* name, bool (*testFunc)(void)) -> void {
        total++;
        bool ok = testFunc();
        if (ok) {
            passed++;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            failed++;
            std::cout << "[FAIL] " << name << std::endl;
        }
    };

    // Execute tests with explanatory comments for maintainability.
    runTest("test_SetUp_InitializesCounter", test_SetUp_InitializesCounter);
    // Verifies that setUp resets CounterSetup to 1 starting from a non-1 value.
    runTest("test_SetUp_ResetsPreviousValue", test_SetUp_ResetsPreviousValue);
    // Verifies that multiple consecutive invocations do not alter the value beyond 1.
    runTest("test_SetUp_MultipleCallsStayOne", test_SetUp_MultipleCallsStayOne);

    std::cout << "Total tests run: " << total
              << ", Passed: " << passed
              << ", Failed: " << failed << std::endl;

    // Return non-zero if any test failed to aid integration in build systems.
    return (failed == 0) ? 0 : 1;
}