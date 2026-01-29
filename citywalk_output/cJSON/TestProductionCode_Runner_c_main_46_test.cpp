// TestProductionCode_Runner.c
// Comprehensive test suite for the focal methods exposed in ProductionCode.h
// This file follows the Unity framework structure and mirrors the main() runner
// provided in the focal method description. Each test includes explanatory comments.

#include <setjmp.h>
#include <ProductionCode.h>
#include <stdio.h>
#include <unity.h>


// Forward declarations of test helpers (as required by Unity-style runners)
void setUp(void);
void tearDown(void);
void resetTest(void);

// Local test helpers (if needed in the future)
static int NotInListValue(void) { return 9999; }      // Value assumed not in the internal list
static int InListValue(void) { return 1; }            // Value assumed to be in the internal list (first item)
static void LogTestStart(const char* testName) { /* Optional logging hook */ }

// SetUp/TearDown hooks (Unity calls these around each test; kept minimal here)
void setUp(void) { /* intentionally left blank; add per-test setup if needed */ }
void tearDown(void) { /* intentionally left blank; add per-test teardown if needed */ }

// The resetTest function is provided by the test harness in the original snippet.
// Here we keep it as a placeholder to reflect the contract.
void resetTest(void) {
    // If ProductionCode provides a reset function for static state, call it here.
    // For example: resetCounter();
}

// Test 1: Ensure that a value not present in the internal list yields 0.
// This validates the "not found" branch of the broken FindFunction.
void test_FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode(void)
{
    // Arrange: a value that should not be in the internal list
    int notInList = NotInListValue();

    // Act: invoke the function under test
    int result = FindFunction_WhichIsBroken(notInList);

    // Assert: 0 is expected to indicate "not found" per the test description
    TEST_ASSERT_EQUAL_INT(0, result);
}

/*
  Test 2: Validate that an item known to be in the internal list yields its 1-based index.
  The test name indicates this behavior will fail due to a known flaw in the function,
  but we still implement it to exercise the true/false branch and maximize coverage.
*/
void test_FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken(void)
{
    // Arrange: assume the first item has 1-based index 1
    int inList = InListValue();

    // Act
    int index = FindFunction_WhichIsBroken(inList);

    // Assert: expect index 1 for the first item (demonstrates coverage of true branch)
    TEST_ASSERT_EQUAL_INT(1, index);
}

/*
  Test 3: Validate that the function returning the current counter value behaves
  reasonably across consecutive invocations. This exercises the "current" value
  semantics of a local/static counter within ProductionCode.
*/
void test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue(void)
{
    // Act: capture two successive values
    int firstValue = FunctionWhichReturnsLocalVariable();
    int secondValue = FunctionWhichReturnsLocalVariable();

    // Assert: the two consecutive calls should not yield a decreasing value.
    // If the counter increments on each call, secondValue >= firstValue.
    TEST_ASSERT_TRUE(secondValue >= firstValue);

    // Additional defensive check: ensure non-negative values
    TEST_ASSERT_TRUE(firstValue >= 0);
    TEST_ASSERT_TRUE(secondValue >= 0);
}

/*
  Test 4: Re-validate the counter behavior with a subsequent pair of calls to
  ensure continued monotonicity or non-decreasing behavior.
*/
void test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain(void)
{
    // Act: two more successive invocations
    int prev = FunctionWhichReturnsLocalVariable();
    int next = FunctionWhichReturnsLocalVariable();

    // Assert: monotonic behavior is preserved if the counter increments per call
    TEST_ASSERT_TRUE(next >= prev);
}

/*
  Test 5: This test is intentionally flawed to illustrate a non-robust test case.
  It demonstrates a scenario where the test expectation does not reflect actual
  runtime behavior of the function, which is useful for teaching/debugging.
*/
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed(void)
{
    int v = FunctionWhichReturnsLocalVariable();

    // Intentionally flawed assertion: this will fail under normal circumstances
    // and is used to illustrate a flawed test case in the suite.
    // The assertion below would logically always be false unless v == v + 1.
    TEST_ASSERT_EQUAL_INT(v, v + 1);
}

// The Unity RUN_TEST macro requires line numbers for test mapping.
// The following main mirrors the structure provided in the focal method description.
int main(void)
{
{
  UnityBegin("test/TestProductionCode.c");

  // Each test corresponds to a RUN_TEST invocation with an associated line number.
  // The line numbers are included to reflect the original test file structure.
  RUN_TEST(test_FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode, 20);
  RUN_TEST(test_FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken, 30);
  RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue, 41);
  RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain, 51);
  RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed, 57);

  // End of test suite
  return (UnityEnd());
}
}