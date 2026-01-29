// Expanded unit tests for the focal method: testNotEqualFloatEachEqualNegative1
// These additions extend coverage for scenarios where TEST_ASSERT_EACH_EQUAL_FLOAT
// should detect inequality among a set of floats and abort as expected.
// The tests follow the same Unity testing pattern used in the provided focal file.

/*
  Note on structure:
  - The tests respect the existing UNITY_EXCLUDE_FLOAT branch. If UNITY_EXCLUDE_FLOAT
    is defined, the tests will be ignored (as with the original Negative1 test).
  - Each test intentionally provides a float array containing at least one value that
    does not match the target value (-0.253f). The assertion should fail, triggering
    an abort, which is verified by the test harness.
  - The extra braces follow the surrounding style observed in the provided code.
*/

void testNotEqualFloatEachEqualNegative2(void)
{
{
#ifdef UNITY_EXCLUDE_FLOAT
    // If float tests are excluded, mark this test to be ignored
    TEST_IGNORE();
#else
    // Array where not all elements are -0.253f
    // Elements: -1.0f, -0.253f, -0.253f, -0.254f
    // The test asserts that all entries equal to -0.253f, which is false for p0[0] and p0[3]
    float p0[] = {-1.0f, -0.253f, -0.253f, -0.254f};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(-0.253f, p0, 4);
    VERIFY_FAILS_END
#endif
}
}

/*
  Additional variation to exercise the detector when the first element differs.
  This ensures the abort path is taken even if the array starts with an unequal value.
*/
void testNotEqualFloatEachEqualNegative3(void)
{
{
#ifdef UNITY_EXCLUDE_FLOAT
    // If float tests are excluded, mark this test to be ignored
    TEST_IGNORE();
#else
    // Array where the first element is not -0.253f
    // Elements: -0.253f, -0.254f, -0.253f, -0.253f
    float p0[] = {-0.253f, -0.254f, -0.253f, -0.253f};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(-0.253f, p0, 4);
    VERIFY_FAILS_END
#endif
}
}