/*
 * Test suite for the focal method setUp (and its dependent global state)
 * Using Unity framework (C) in a C++11 compatible file (no GTest)
 * The tests focus on ensuring setUp() resets the relevant global flags used by tearDown logic.
 * This file assumes the following globals and function are defined in the focal implementation
 * (as hinted by the provided <FOCAL_CLASS_DEP> snippet):
 *   - int SetToOneToFailInTearDown;
 *   - int SetToOneMeanWeAlreadyCheckedThisGuy;
 *   - void setUp(void);
 * Note: In a full project, these would be defined in the focal source file. We declare them here as extern.
 */

#include <setjmp.h>
#include <unity.h>
#include <cstdio>
#include <stdio.h>


// Unity framework header (C). Include with C linkage in C++ translation units.
extern "C" {
}

// External globals and function under test (as per the focal code snippet).
extern int SetToOneToFailInTearDown;
extern int SetToOneMeanWeAlreadyCheckedThisGuy;
extern void setUp(void);

// ----------------------------------------------------------------------------
// Test: setUp resets tracking flags to zero regardless of their prior state.
// This validates the primary behavior of the focal method under test.
// ----------------------------------------------------------------------------

/*
 * Test 1: Ensure setUp() resets non-zero global flags to zero.
 * Precondition: Both flags are non-zero.
 * Action: Call setUp().
 * Assertion: Both flags must be 0 after setUp().
 */
void test_SetUp_ResetsGlobals(void)
{
    // Arrange: put non-zero values to simulate prior state
    SetToOneToFailInTearDown = 5;
    SetToOneMeanWeAlreadyCheckedThisGuy = 7;

    // Act: invoke the focal method
    setUp();

    // Assert: verify that setUpReset both flags to 0
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, SetToOneToFailInTearDown, "SetToOneToFailInTearDown should be reset to 0 by setUp");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, SetToOneMeanWeAlreadyCheckedThisGuy, "SetToOneMeanWeAlreadyCheckedThisGuy should be reset to 0 by setUp");
}

/*
 * Test 2: Idempotence of setUp: if flags are already zero, setUp should leave them zero.
 * Precondition: Both flags are 0.
 * Action: Call setUp().
 * Assertion: Both flags remain 0.
 */
void test_SetUp_Idempotence(void)
{
    // Arrange: flags already zero
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;

    // Act
    setUp();

    // Assert
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, SetToOneToFailInTearDown, "SetToOneToFailInTearDown should remain 0 after setUp");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, SetToOneMeanWeAlreadyCheckedThisGuy, "SetToOneMeanWeAlreadyCheckedThisGuy should remain 0 after setUp");
}

/*
 * Test 3: setUp handles larger/sentinel-like values gracefully by resetting to zero.
 * Precondition: Flags are set to large non-zero values.
 * Action: Call setUp().
 * Assertion: Flags are reset to 0.
 */
void test_SetUp_WithVariousInitialValues(void)
{
    // Arrange: large initial values to simulate edge-case states
    SetToOneToFailInTearDown = 1024;
    SetToOneMeanWeAlreadyCheckedThisGuy = 65535;

    // Act
    setUp();

    // Assert
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, SetToOneToFailInTearDown, "SetToOneToFailInTearDown should be reset to 0 from a large initial value");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, SetToOneMeanWeAlreadyCheckedThisGuy, "SetToOneMeanWeAlreadyCheckedThisGuy should be reset to 0 from a large initial value");
}

// ----------------------------------------------------------------------------
// Main entry: register tests and run them.
// ----------------------------------------------------------------------------
int main(void)
{
    UNITY_BEGIN();

    // Run tests specifically targeting the setUp() behavior and its side effects
    RUN_TEST(test_SetUp_ResetsGlobals);
    RUN_TEST(test_SetUp_Idempotence);
    RUN_TEST(test_SetUp_WithVariousInitialValues);

    return UNITY_END();
}