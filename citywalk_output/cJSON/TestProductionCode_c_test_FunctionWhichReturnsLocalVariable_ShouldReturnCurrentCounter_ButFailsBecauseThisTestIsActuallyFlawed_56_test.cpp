// TestProductionCode.cpp
// Unit test suite for the focal method: FunctionWhichReturnsLocalVariable
// This suite is designed to be compiled with a C++11 compiler without GoogleTest.
// It uses the Unity framework for assertions and test harness integration.
// The focal test under scrutiny is test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFlaws,
// which intentionally contains a flawed assertion to illustrate improper test setup.
// Explanatory comments are added to each unit test to clarify intent and coverage.

#include <iostream>
#include <ProductionCode.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declarations of test functions (Unity will call these via RUN_TEST in main)
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounterValue(void);
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounterValueAgain(void);
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFlawed(void);
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_Incrementally(void);

// Unity's required setup/teardown (can be empty for these tests)
void setUp(void)   { /* Optional: initialize test state before each test */ }
void tearDown(void) { /* Optional: clean up after each test */ }

// Test 1: Validate that successive calls return incrementing counter values.
// Rationale: If the focal function exposes a static counter that increments on each call,
// then the second call should yield the first value plus one.
// This test contributes to baseline functional coverage of the local-variable counter behavior.
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounterValue(void)
{
    int first = FunctionWhichReturnsLocalVariable();
    int second = FunctionWhichReturnsLocalVariable();

    // Expect the counter to have incremented by one between calls.
    TEST_ASSERT_EQUAL_INT(first + 1, second);
}

// Test 2: Validate that multiple sequential calls continue to increment consistently.
// Rationale: A second pair of successive calls should also reflect contiguous increments,
// indicating stable progression of the internal counter across calls.
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounterValueAgain(void)
{
    int v1 = FunctionWhichReturnsLocalVariable();
    int v2 = FunctionWhichReturnsLocalVariable();
    int v3 = FunctionWhichReturnsLocalVariable();
    int v4 = FunctionWhichReturnsLocalVariable();

    // Expect a sequence: v2 == v1 + 1 and v4 == v3 + 1
    TEST_ASSERT_EQUAL_INT(v1 + 1, v2);
    TEST_ASSERT_EQUAL_INT(v3 + 1, v4);
}

// Test 3: Flawed test preserved from the focal method.
// This test intentionally asserts a hard-coded hex value (0x1234) against the
// current counter value. It is expected to fail if the initial condition/setup
// is not perfectly aligned with the hard-coded expectation.
// Rationale: Demonstrates how a test can be misleading if initial state assumptions are incorrect.
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFlawed(void)
{
    // Flawed assertion copied from the focal test description.
    TEST_ASSERT_EQUAL_HEX(0x1234, FunctionWhichReturnsLocalVariable());
}

// Test 4: Additional coverage to exercise stable, incremental behavior across multiple calls.
// Rationale: Verifies that repeated runs (more than two) maintain the incrementing contract,
// which strengthens confidence in the monotonic progression of the internal counter.
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_Incrementally(void)
{
    int a = FunctionWhichReturnsLocalVariable();
    int b = FunctionWhichReturnsLocalVariable();
    int c = FunctionWhichReturnsLocalVariable();

    // Expected increments: a -> b (+1), b -> c (+1)
    TEST_ASSERT_EQUAL_INT(a + 1, b);
    TEST_ASSERT_EQUAL_INT(b + 1, c);
}

// Main entry point: Unity test runner.
// Note: If GoogleTest or GMock are disallowed, tests are invoked from this main function.
// This harness ensures all tests are executed in a single run for maximum coverage.
// The tests exercise the focal method and its interaction with the static/local counter mechanism.

int main(void)
{
    UnityBegin("TestProductionCode");

    // Run the tests in a defined sequence
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounterValue);
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounterValueAgain);
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFlawed);
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_Incrementally);

    return UnityEnd();
}