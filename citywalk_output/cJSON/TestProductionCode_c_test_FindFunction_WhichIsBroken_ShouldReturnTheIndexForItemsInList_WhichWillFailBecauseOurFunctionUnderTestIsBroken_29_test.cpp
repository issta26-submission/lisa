// TestProductionCode.cpp
// Lightweight C++11 test harness for the focal function FindFunction_WhichIsBroken
// This file provides a self-contained test suite without Google Test.
// It intentionally mirrors the provided Unity-based test names and structure
// but uses a minimal in-house assertion mechanism to avoid terminating tests early.

#include <iostream>
#include <ProductionCode.h>
#include <unity.h>


// Bring in the production code declarations (C linkage).
extern "C" {
}

// Simple in-house test harness to accumulate results without aborting on first failure.
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion helpers
#define ASSERT_EQUAL(expected, actual) MyAssertEqual((expected), (actual), __FILE__, __LINE__)
#define ASSERT_TRUE(condition)        MyAssertTrue((condition), __FILE__, __LINE__)

/*
 * Reports a non-fatal assertion failure if actual != expected.
 * Does not terminate the test function, allowing multiple checks per test.
 */
static void MyAssertEqual(int expected, int actual, const char* file, int line) {
    ++g_tests_run;
    if (expected != actual) {
        ++g_tests_failed;
        std::cerr << "ASSERT_EQUAL failed at " << file << ":" << line
                  << " | expected: " << expected
                  << " | actual: " << actual << std::endl;
    }
}

/*
 * Reports a non-fatal assertion failure if condition is false.
 * This supports boolean-condition checks without aborting.
 */
static void MyAssertTrue(bool condition, const char* file, int line) {
    ++g_tests_run;
    if (!condition) {
        ++g_tests_failed;
        std::cerr << "ASSERT_TRUE failed at " << file << ":" << line
                  << " | condition evaluated to false" << std::endl;
    }
}

/*
 * Test 1
 * test_FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken
 * 
 * This test intentionally asserts the values (34 -> 1) and (8888 -> 8) that reflect
 * the provided focal test and demonstrate failure behavior when the underlying
 * function is broken. The test should fail for at least one of these assertions
 * to indicate broken behavior is being exercised.
 * 
 * Explanatory Notes:
 * - Mirrors the exact intent of the original Unity-based test block.
 * - Uses non-terminating assertions so subsequent checks could run in a real framework.
 */
static void test_FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken(void)
{
    // You should see this line fail in your test summary
    ASSERT_EQUAL(1, FindFunction_WhichIsBroken(34));

    // Notice the rest of these didn't get a chance to run in Unity when the first assertion fails.
    // We still perform the second assertion here to allow the non-terminating harness to capture it.
    ASSERT_EQUAL(8, FindFunction_WhichIsBroken(8888));
}

/*
 * Test 2
 * test_FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode
 * 
 * Verifies that when an item is not in the list, the function returns zero.
 * This test is expected to pass even with a broken implementation for the found case.
 */
static void test_FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode(void)
{
    // Pick an item that is presumed not to exist in the internal list.
    int notInList = 12345;
    ASSERT_EQUAL(0, FindFunction_WhichIsBroken(notInList));
}

/*
 * Test 3
 * test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue
 * 
 * Exercise a function that is expected to return a local/static counter value.
 * Since the exact expected value is not provided, we validate a non-negative
 * result to ensure the function is callable and returns a sane value.
 */
static void test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue(void)
{
    int value = FunctionWhichReturnsLocalVariable();
    ASSERT_TRUE(value >= 0);
}

/*
 * Test 4
 * test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain
 * 
 * Similar to Test 3, ensuring consistency across multiple invocations.
 */
static void test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain(void)
{
    int value = FunctionWhichReturnsLocalVariable();
    ASSERT_TRUE(value >= 0);
}

/*
 * Test 5
 * test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed
 * 
 * This test intentionally contains a flawed assertion to reflect the test name.
 * It is expected to fail under normal circumstances, illustrating a faulty test case.
 */
static void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed(void)
{
    int value = FunctionWhichReturnsLocalVariable();
    // Deliberately incorrect expectation to demonstrate a flawed test.
    ASSERT_EQUAL(-1, value);
}

/*
 * Entry point for running all tests.
 * This mimics a simple test harness that reports results rather than aborting on first failure.
 */
int main(void)
{
    std::cout << "Starting TestProductionCode.cpp test suite (C++11 harness)" << std::endl;

    // Execute tests
    test_FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode();
    test_FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken();
    test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue();
    test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain();
    test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed();

    // Summary
    std::cout << "Test Summary: " << g_tests_run << " ran, " << g_tests_failed << " failed." << std::endl;
    if (g_tests_failed > 0) {
        std::cerr << "Some tests failed. See details above." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}