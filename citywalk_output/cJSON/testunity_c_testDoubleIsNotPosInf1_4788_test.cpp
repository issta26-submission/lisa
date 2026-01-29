/*
  Step 1 - Candidate Keywords (reflecting the focal method's core dependencies and logic)
  - UNITY_EXCLUDE_DOUBLE: Compile-time flag to exclude double-related tests
  - TEST_IGNORE: Mark test as ignored when doubles are excluded
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: Mechanism to assert that code path aborts
  - TEST_ASSERT_DOUBLE_IS_INF: Assertion that a double value is positive/negative infinity
  - 2.0: Sample non-infinite double value used to verify failure path
  - testDoubleIsNotPosInf1: Focal test existing in testunity.c (not duplicated here)
  - testDoubleIsNotPosInf2: Additional test to extend coverage for the same predicate
  - TEST_ASSERT_DOUBLE_IS_INF(2.0): Core predicate under test
  Notes:
  - The test suite relies on the Unity testing framework semantics (Unity macros).
  - This snippet is designed to be added alongside the existing focal test to broaden coverage.
  - Static/global state handling is intentionally minimal since this test focuses on the same predicate.
*/

/* Additional coverage test for the focal method testDoubleIsNotPosInf1
   This mirrors the original test body but provides an additional explicit test
   case (testDoubleIsNotPosInf2) that exercises the same predicate under the
   same compile-time branch conditions. */

void testDoubleIsNotPosInf2(void)
{
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // If double tests are excluded, this test should be ignored.
    TEST_IGNORE();
#else
    // When doubles are included, ensure that TEST_ASSERT_DOUBLE_IS_INF(2.0)
    // correctly triggers a failure (since 2.0 is not infinity), and that
    // Unity reports the abort as expected.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_INF(2.0);
    VERIFY_FAILS_END
#endif
}
}