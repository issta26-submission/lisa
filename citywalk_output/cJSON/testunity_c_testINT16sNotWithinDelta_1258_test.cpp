// Test suite for the focal method: testINT16sNotWithinDelta (INT16 within/not-within delta checks)
// This suite targets the INT16 delta predicate macros (TEST_ASSERT_INT16_WITHIN, TEST_ASSERT_INT16_WITHIN_MESSAGE)
// and exercises both failing and passing paths to improve coverage.
// The tests are written to be compiled in a C++11 (or C) environment using the Unity test framework
// and are designed to work with the existing Unity-assisted abort/verify macros provided in the
// project's test harness (<FOCAL_CLASS_DEP> section).

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// If building as C++, ensure linkage compatibility with the Unity C framework if needed.
// The project under test may already provide the necessary macros (EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, etc.)
// We rely on those existing macros to test abort scenarios for failing assertions.

// Candidate Keywords (for reference):
// - INT16, WITHIN, NOT_WITHIN, DELTA, 4, 2, -2, 6, 0x..., boundary conditions, custom messages

// Test that a known failing INT16 within-delta check indeed aborts as expected.
// This mirrors the focal testINT16sNotWithinDelta scenario: delta=2, expected=4, actual=-2
// The absolute difference is 6, which is greater than delta, so the assertion should fail.
void test_int16s_not_within_delta_should_abort(void)
{
    // Expect the assertion to fail and abort the test, matching the focal behavior.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_WITHIN(2, 4, -2);
    VERIFY_FAILS_END
}

// Test that an INT16 within delta check passes without abort.
// Here delta=2, expected=4, actual=6 => |6 - 4| = 2 <= delta
void test_int16s_within_delta_should_pass(void)
{
    // This should pass and not abort.
    TEST_ASSERT_INT16_WITHIN(2, 4, 6);
}

// Test boundary condition: within delta exactly on the boundary (difference equals delta).
// delta=2, expected=4, actual=6
void test_int16s_within_delta_on_boundary_should_pass(void)
{
    TEST_ASSERT_INT16_WITHIN(2, 4, 6); // same as previous, explicit boundary case
}

// Test a failing within-delta with a custom message to ensure messaging does not affect control flow.
// This should abort and print the provided message on failure.
void test_int16s_not_within_delta_with_custom_message_should_abort(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_WITHIN_MESSAGE(2, 4, -2, "Custom message: INT16 within delta failed.");
    VERIFY_FAILS_END
}

// Test a passing within-delta with a custom message to ensure non-failing path with message works.
void test_int16s_within_delta_with_custom_message_should_pass(void)
{
    TEST_ASSERT_INT16_WITHIN_MESSAGE(2, 4, 6, "Custom message: INT16 within delta passed.");
}

// Test that a notably different value outside the delta still aborts, ensuring the negative path is covered.
// delta=2, expected=4, actual=9 (diff=5)
void test_int16s_not_within_delta_far_apart_should_abort(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_WITHIN(2, 4, 9);
    VERIFY_FAILS_END
}

// Test runner: initialize Unity, execute the above tests, and finalize.
// This main can be compiled standalone or integrated into an existing test suite.
// Ensure that RUN_TEST names match the function identifiers above.

int main(void)
{
    UNITY_BEGIN("test_int16s_not_within_delta_suite.cpp");

    // Focal/targeted tests for INT16 delta predicate behavior
    RUN_TEST(test_int16s_not_within_delta_should_abort);
    RUN_TEST(test_int16s_within_delta_should_pass);
    RUN_TEST(test_int16s_within_delta_on_boundary_should_pass);
    RUN_TEST(test_int16s_not_within_delta_with_custom_message_should_abort);
    RUN_TEST(test_int16s_within_delta_with_custom_message_should_pass);
    RUN_TEST(test_int16s_not_within_delta_far_apart_should_abort);

    return UNITY_END();
}