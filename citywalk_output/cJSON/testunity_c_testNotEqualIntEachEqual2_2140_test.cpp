/*
  Additional comprehensive tests for the focal method:
  testNotEqualIntEachEqual2 in testunity.c

  Rationale:
  - Validate that TEST_ASSERT_EACH_EQUAL_INT(-5, p0, 4) aborts when any element differs.
  - Extend coverage by varying which element differs from the expected value (-5).
  - Keep using the Unity-style abort-guard macros (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
    to ensure behavior mirrors the original focal test.

  Note:
  These tests are intended to augment the existing testNotEqualIntEachEqual2 scenario
  in testunity.c and rely on the same macro definitions and test harness as used there.
  They should be compiled alongside the existing Unity test suite.
*/

// Test: First element differs from the expected value (-5)
void testNotEqualIntEachEqual2_FirstElementMismatch_Variant(void)
{
    // p0 has a mismatch at the first element
    int p0[] = {-6, -5, -5, -5};
    // Expect the test to abort because not all elements equal -5
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(-5, p0, 4);
    VERIFY_FAILS_END
}

// Test: Last element differs from the expected value (-5)
void testNotEqualIntEachEqual2_LastElementMismatch_Variant(void)
{
    // p0 has a mismatch at the last element
    int p0[] = {-5, -5, -5, -6};
    // Expect the test to abort because not all elements equal -5
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(-5, p0, 4);
    VERIFY_FAILS_END
}

// Test: Middle element differs from the expected value (-5)
void testNotEqualIntEachEqual2_MiddleMismatch_Variant(void)
{
    // p0 has a mismatch in the middle (third element)
    int p0[] = {-5, -5, -6, -5};
    // Expect the test to abort because not all elements equal -5
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT(-5, p0, 4);
    VERIFY_FAILS_END
}