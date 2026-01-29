// Additional test cases for testINT32sWithinDelta to improve coverage
// These tests complement the existing ones by exercising true/false branches
// and boundary conditions for INT32 within a given delta.

void testINT32sWithinDelta_AdditionalCases(void)
{
    // True cases: values are equal or within the specified delta
    // Boundary case: delta 0 with identical values
    TEST_ASSERT_INT32_WITHIN(0, 0, 0);                 // identical values, delta 0
    TEST_ASSERT_INT32_WITHIN(0, 123456789, 123456789); // identical values, delta 0
    // Boundary around zero and small delta
    TEST_ASSERT_INT32_WITHIN(3, -1000, -997);          // diff 3, within delta
    TEST_ASSERT_INT32_WITHIN(3, 1000, 997);            // diff 3, within delta
    // Crossing zero with positive numbers
    TEST_ASSERT_INT32_WITHIN(5, -2, 3);                // diff 5, within delta
    // Positive/negative mix still within delta
    TEST_ASSERT_INT32_WITHIN(5, -10, -5);              // diff 5, within delta

    // False cases: ensure failure paths are exercised (with expected aborts)
    // 1) diff greater than delta
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(1, 5000, 5002);            // diff is 2, delta is 1 -> should fail
    VERIFY_FAILS_END

    // 2) diff greater than delta with negatives
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(2, -100, -103);           // diff is 3, delta is 2 -> should fail
    VERIFY_FAILS_END

    // 3) zero delta but values differ
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT32_WITHIN(0, 1, 2);                 // diff is 1 > delta 0 -> should fail
    VERIFY_FAILS_END
}