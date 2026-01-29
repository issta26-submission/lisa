/*
  Test suite for the focal method: custom_setup(void)

  This test targets the function defined in the focal module:
    void custom_setup(void)
  which, per the provided snippet, sets CounterSetup = 2.

  Candidate Keywords extracted from the focal context:
  - CounterSetup
  - custom_setup
  - Defs.h
  - mockMock.h
  - Unity
  - CException (when applicable)
  - testRunnerGeneratorWithMocks.c (source location context)
  - SetUp/TearDown hooks (Unity)

  Note: The tests use the Unity framework for C (non-GTest). They exercise the
  effect on the global CounterSetup variable to verify correct behavior of
  custom_setup.
*/

#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


/* Extern declarations to access symbols defined in the focal module.
   CounterSetup is assumed to be an int global variable modified by custom_setup. */
extern int CounterSetup;
extern void custom_setup(void);

void setUp(void)   { /* Unity setup hook; intentionally empty for this suite */ }
void tearDown(void) { /* Unity teardown hook; intentionally empty for this suite */ }

/*
  Test 1: Ensure that custom_setup always assigns CounterSetup to 2
  regardless of its prior value (basic behavioral correctness).
*/
void test_custom_setup_sets_CounterSetup_to_2_from_zero(void)
{
    CounterSetup = 0;       // Pre-condition: initial value is zero
    custom_setup();           // Act: invoke the focal method
    TEST_ASSERT_EQUAL_INT(2, CounterSetup); // Assert: CounterSetup becomes 2
}

/*
  Test 2: Validate behavior across a range of initial values.
  This checks robustness across negative, zero and positive values.
*/
void test_custom_setup_sets_CounterSetup_to_2_from_various_values(void)
{
    int test_values[] = { -10, -1, 0, 1, 1000, 2147483647 };
    size_t count = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < count; ++i)
    {
        CounterSetup = test_values[i]; // Pre-condition: vary initial value
        custom_setup();                   // Act
        TEST_ASSERT_EQUAL_INT(2, CounterSetup); // Assert: always becomes 2
    }
}

/*
  Test 3: Idempotence check - calling custom_setup multiple times should
  maintain the expected outcome (CounterSetup remains 2 after subsequent calls).
*/
void test_custom_setup_is_idempotent(void)
{
    CounterSetup = 42;  // Pre-condition: arbitrary non-zero value
    custom_setup();       // Act first invocation
    custom_setup();       // Act second invocation
    TEST_ASSERT_EQUAL_INT(2, CounterSetup); // Assert: still 2 after multiple calls
}

int main(void)
{
    UnityBegin("test_custom_setup_suite");

    RUN_TEST(test_custom_setup_sets_CounterSetup_to_2_from_zero);
    RUN_TEST(test_custom_setup_sets_CounterSetup_to_2_from_various_values);
    RUN_TEST(test_custom_setup_is_idempotent);

    return UnityEnd();
}