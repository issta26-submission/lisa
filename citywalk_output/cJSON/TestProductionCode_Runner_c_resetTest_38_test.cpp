/***************************************
 * Test Suite for resetTest(void)       *
 * Focal method located in TestProductionCode_Runner.c
 * - This suite verifies that resetTest correctly
 *   performs a tearDown followed by a setUp, and
 *   that multiple invocations accumulate the expected
 *   number of setup/teardown calls.
 *
 * Domain knowledge applied:
 * - Use Unity as the C unit testing framework (no GTest).
 * - Expose setUp/tearDown to count invocations for precise verification.
 * - Call resetTest via extern linkage (the focal method is defined in
 *   the production/test runner file).
 * - Compile in a C++11 compatible environment if needed.
 ***************************************/

#include <setjmp.h>
#include <ProductionCode.h>
#include <stdio.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Focal function under test (defined in TestProductionCode_Runner.c)
// Declared here so this test can link against it.
void resetTest(void);

// Lifecycle hooks used by resetTest to reset the test environment.
// We declare them here so that the linker resolves them to our counters.
void setUp(void);
void tearDown(void);

#ifdef __cplusplus
}
#endif

// Internal counters to track how many times setup/teardown have been invoked.
// These will help us verify that resetTest triggers the expected calls.
static int g_setup_calls = 0;
static int g_teardown_calls = 0;

// Override the standard Unity lifecycle hooks to count calls.
// These will be linked with resetTest's invocation sequence when compiled together.
void setUp(void) { g_setup_calls++; }
void tearDown(void) { g_teardown_calls++; }

/*
 * Test 1: resetTest_increments_setup_and_teardown_once
 * 
 * Rationale:
 * - When resetTest is invoked, it should perform exactly one
 *   tearDown() followed by one setUp() relative to the current test state.
 * - The Unity harness also calls setUp() before the test and tearDown() after the test,
 *   so we measure the delta caused solely by the resetTest invocation.
 */
void test_resetTest_increments_setup_and_teardown_once(void)
{
  int setups_before = g_setup_calls;
  int teardowns_before = g_teardown_calls;

  // Act: reset the test environment
  resetTest();

  // Assert: resetTest caused exactly one additional setUp and one additional tearDown
  TEST_ASSERT_EQUAL_INT(setups_before + 1, g_setup_calls);
  TEST_ASSERT_EQUAL_INT(teardowns_before + 1, g_teardown_calls);
}

/*
 * Test 2: resetTest_can_be_called_twice_in_one_test
 * 
 * Rationale:
 * - Calling resetTest twice within the same test should increment
 *   the setup and teardown counters by two overall across both calls.
 * - Verify the incremental behavior across consecutive resets.
 */
void test_resetTest_can_be_called_twice_in_one_test(void)
{
  int s0 = g_setup_calls;
  int t0 = g_teardown_calls;

  // First reset
  resetTest();
  int s1 = g_setup_calls;
  int t1 = g_teardown_calls;

  // Second reset
  resetTest();
  int s2 = g_setup_calls;
  int t2 = g_teardown_calls;

  // After first reset: one more setup and one more teardown
  TEST_ASSERT_EQUAL_INT(s0 + 1, s1);
  TEST_ASSERT_EQUAL_INT(t0 + 1, t1);

  // After second reset: another +1 for both counters
  TEST_ASSERT_EQUAL_INT(s1 + 1, s2);
  TEST_ASSERT_EQUAL_INT(t1 + 1, t2);
}

/*
 * Main test runner
 * - Uses Unity's RUN_TEST macro to execute each test.
 * - Reports results in a consolidated manner.
 */
int main(void)
{
  // Initialize counters for a clean run
  g_setup_calls = 0;
  g_teardown_calls = 0;

  UnityBegin("TestProductionCode_Runner_ResetTestSuite");

  // Register tests
  RUN_TEST(test_resetTest_increments_setup_and_teardown_once, __LINE__);
  RUN_TEST(test_resetTest_can_be_called_twice_in_one_test, __LINE__);

  return UnityEnd();
}