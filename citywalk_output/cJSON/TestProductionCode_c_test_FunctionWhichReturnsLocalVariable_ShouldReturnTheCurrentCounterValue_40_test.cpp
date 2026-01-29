/*
 * TestProductionCode.c
 *
 * This test suite targets the focal method:
 *   FunctionWhichReturnsLocalVariable()
 * and its dependency, the global/static counter named Counter.
 *
 * The tests are designed using the Unity framework (C unit testing),
 * compatible with C++11 builds as well. No GTest or GMock is used.
 *
 * Notes:
 * - We explicitly reset Counter within each test to ensure test isolation.
 * - The suite demonstrates coverage for the primary path of the focal method
 *   (returning the current counter value) and a few related scenarios.
 * - The file includes explanatory comments for each test to clarify intent.
 *
 * Candidate Keywords (Step 1 insights):
 * - FunctionWhichReturnsLocalVariable
 * - Counter
 * - ProductionCode.h
 * - setUp / tearDown
 * - Hexadecimal value assertions (TEST_ASSERT_EQUAL_HEX)
 * - State reset / independence between tests
 * - Boundary values (e.g., 0x0000, 0x5a5a, 0x1234, 0xABCD, 0xFFFF)
 *
 * Step 2/3 guidance incorporated: tests focus on the focal method and its
 * core dependent components, with clear and executable Unity assertions.
 */

#include <ProductionCode.h>
#include <unity.h>


// Step 3: Test harness setup/teardown
void setUp(void)
{
    // Intentionally left minimal. Individual tests reset Counter to known values
    // to ensure test independence and determinism.
}

void tearDown(void)
{
    // No teardown required for these tests
}

/*
 * Test: test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue
 * Purpose: Validate that the focal function returns the current value of Counter.
 * - Initialize Counter to a known value (0x5a5a) and verify the return.
 * - Change Counter and verify the function reflects the updated value.
 */
void test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue(void)
{
    // Ensure a known starting state for the test
    Counter = 0x5a5a;
    // The function should return the current counter value (0x5a5a)
    TEST_ASSERT_EQUAL_HEX(0x5a5a, FunctionWhichReturnsLocalVariable());

    // Update the counter and verify the function returns the new value
    Counter = 0x1234;
    TEST_ASSERT_EQUAL_HEX(0x1234, FunctionWhichReturnsLocalVariable());
}

/*
 * Test: test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain
 * Purpose: Confirm consistency across consecutive reads with no intermediate changes.
 * - Reset Counter to a known value before assertions.
 * - Verify repeated calls yield the same value.
 * - Change Counter and verify the updated value is returned.
 */
void test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain(void)
{
    // Reset to a known state to keep this test independent of previous tests
    Counter = 0x5a5a;
    TEST_ASSERT_EQUAL_HEX(0x5a5a, FunctionWhichReturnsLocalVariable());
    // Consecutive call without changing Counter should yield the same result
    TEST_ASSERT_EQUAL_HEX(0x5a5a, FunctionWhichReturnsLocalVariable());

    // Change the counter and verify the updated value is returned
    Counter = 0xABCD;
    TEST_ASSERT_EQUAL_HEX(0xABCD, FunctionWhichReturnsLocalVariable());
}

/*
 * Test: test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed
 * Purpose: Provide an additional coverage scenario while keeping the test deterministic.
 * Note: The test name suggests a flawed scenario, but the implementation is kept
 *       deterministic and asserts a valid condition under controlled state.
 */
void test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed(void)
{
    // Set Counter to a known value and verify the focal function returns it
    Counter = 0x0000;
    TEST_ASSERT_EQUAL_HEX(0x0000, FunctionWhichReturnsLocalVariable());

    // Do not modify Counter after the assertion to preserve stability
    TEST_ASSERT_EQUAL_HEX(0x0000, FunctionWhichReturnsLocalVariable());
}

/*
 * Optional extra test: verify handling of a larger value boundary.
 * This helps ensure hex-based values beyond common small constants are handled.
 */
void test_FunctionWhichReturnsLocalVariable_ShouldHandleLargeCounterValue(void)
{
    Counter = 0xFFFF;
    TEST_ASSERT_EQUAL_HEX(0xFFFF, FunctionWhichReturnsLocalVariable());
}

/* Unity test runner */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue);
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain);
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed);
    RUN_TEST(test_FunctionWhichReturnsLocalVariable_ShouldHandleLargeCounterValue);
    return UNITY_END();
}