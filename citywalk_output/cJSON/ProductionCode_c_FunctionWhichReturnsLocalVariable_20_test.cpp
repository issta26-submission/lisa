// Lightweight C++11 test suite for the focal function FunctionWhichReturnsLocalVariable
// This test directly exercises how the function returns the value of the Counter variable
// by manipulating Counter (extern from ProductionCode.h) and validating the returned value.
// No GTest is used; a small custom test harness is provided that uses non-terminating assertions.

#include <iostream>
#include <string>
#include <ProductionCode.h>


// Forward declaration of the C function under test.
// ProductionCode.h is the canonical header for the production code.
// We wrap the include with extern "C" guards to ensure proper linkage in C++.
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Extern declaration for the Counter variable.
// If ProductionCode.h already provides a declaration, this is harmless as an additional extern reference.
// It is required here to allow tests to set and observe Counter's value.
extern int Counter;

// Simple test framework (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define ASSERT_EQ(expected, actual, description) do { \
    ++g_totalTests; \
    if ((expected) != (actual)) { \
        std::cerr << "[FAIL] " << description << ": expected " << (expected) \
                  << ", got " << (actual) << std::endl; \
        ++g_failedTests; \
    } else { \
        std::cout << "[PASS] " << description << std::endl; \
    } \
} while(0)

#define ASSERT_TRUE(cond, description) do { \
    ++g_totalTests; \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << description << ": condition is false" << std::endl; \
        ++g_failedTests; \
    } else { \
        std::cout << "[PASS] " << description << std::endl; \
    } \
} while(0)

// Test 1: Counter = 0 -> Function should return 0
// Rationale: Verifies basic propagation of a zero value through the function.
void test_FunctionWhichReturnsLocalVariable_ReturnsCounterValue_WhenCounterIsZero() {
    // Arrange
    Counter = 0;

    // Act
    int result = FunctionWhichReturnsLocalVariable();

    // Assert
    ASSERT_EQ(0, result, "Function should return the current value of Counter when Counter = 0");
}

// Test 2: Counter = some positive value -> Function should return that value
// Rationale: Verifies correct forwarding of a positive value through the function.
void test_FunctionWhichReturnsLocalVariable_ReturnsCounterValue_WhenCounterIsPositive() {
    // Arrange
    Counter = 12345;

    // Act
    int result = FunctionWhichReturnsLocalVariable();

    // Assert
    ASSERT_EQ(12345, result, "Function should return the current value of Counter when Counter is positive");
}

// Test 3: Counter = negative value -> Function should return that negative value
// Rationale: Verifies correct handling of negative integers.
void test_FunctionWhichReturnsLocalVariable_ReturnsCounterValue_WhenCounterIsNegative() {
    // Arrange
    Counter = -987;

    // Act
    int result = FunctionWhichReturnsLocalVariable();

    // Assert
    ASSERT_EQ(-987, result, "Function should return the current value of Counter when Counter is negative");
}

// Test 4: Function does not modify Counter (read-only behavior)
// Rationale: Ensure the function does not have side effects on Counter.
void test_FunctionWhichReturnsLocalVariable_DoesNotModifyCounter() {
    // Arrange
    Counter = 42;
    int before = Counter;

    // Act
    int result = FunctionWhichReturnsLocalVariable();

    // Assert
    ASSERT_EQ(42, result, "Function result should reflect the value of Counter before call");
    ASSERT_EQ(before, Counter, "Counter should remain unchanged after FunctionWhichReturnsLocalVariable call");
}

// Test runner
void run_all_tests() {
    std::cout << "Starting tests for FunctionWhichReturnsLocalVariable...\n";
    test_FunctionWhichReturnsLocalVariable_ReturnsCounterValue_WhenCounterIsZero();
    test_FunctionWhichReturnsLocalVariable_ReturnsCounterValue_WhenCounterIsPositive();
    test_FunctionWhichReturnsLocalVariable_ReturnsCounterValue_WhenCounterIsNegative();
    test_FunctionWhichReturnsLocalVariable_DoesNotModifyCounter();
    std::cout << "Tests completed. Total: " << g_totalTests
              << ", Passed: " << (g_totalTests - g_failedTests)
              << ", Failed: " << g_failedTests << "\n";
}

// Entry point
int main() {
    // Execute all tests
    run_all_tests();
    // Return non-zero if any test failed to aid in CI systems that rely on exit codes.
    return g_failedTests ? 1 : 0;
}