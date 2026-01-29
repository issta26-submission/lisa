// Additional tests to increase coverage for testNotEqualDoubleArraysNegative3
// These tests complement the focal method by validating both branches of the
// double-array equality predicate (true/false) and respecting UNITY_EXCLUDE_DOUBLE.


// Test that the equality path is taken (true branch) when two double arrays are identical.
// Expects the test to pass without aborting.
void testNotEqualDoubleArraysNegative3_PassCase_ShouldSucceed(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // If doubles are excluded, skip this test gracefully.
    TEST_IGNORE();
#else
    // Identical arrays should not trigger a failure in TEST_ASSERT_EQUAL_DOUBLE_ARRAY.
    double p0[] = {-1.0, -8.0, -25.5, -0.253};
    double p1[] = {-1.0, -8.0, -25.5, -0.253};
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    // If execution reaches here, the equality assertion behaved as expected (no abort).
#endif
}


// Test that the inequality path (false branch) is exercised and correctly aborts,
// matching the behavior in the focal testNotEqualDoubleArraysNegative3.
void testNotEqualDoubleArraysNegative3_NotEqualCase_ShouldAbort(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // If doubles are excluded, skip this test gracefully.
    TEST_IGNORE();
#else
    double p0[] = {-1.0, -8.0, -25.4, -0.253};
    double p1[] = {-1.0, -8.0, -25.5, -0.253};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}