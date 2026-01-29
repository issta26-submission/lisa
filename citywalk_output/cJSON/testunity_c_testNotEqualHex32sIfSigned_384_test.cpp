/*
  Supplemental test cases to augment coverage for the focal method:
  void testNotEqualHex32sIfSigned(void)
  (located in testunity.c)

  Rationale:
  - The focal test asserts that two distinct signed 32-bit values are not equal
    when using TEST_ASSERT_EQUAL_HEX32, wrapped in EXPECT_ABORT_BEGIN / VERIFY_FAILS_END.
  - Add additional scenarios to exercise the failure path with a variety of signed values,
    including edge and boundary cases, to ensure robust coverage of the not-equal condition.

  Note:
  - These tests follow the same Unity convention as the focal test and use
    EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to verify that the assertion fails as expected.
  - They rely on standard integer literals and macros (INT32_MIN, INT32_MAX) from <stdint.h>.
  - They are intended to be appended to testunity.c alongside the existing focal test.
*/

/* Test 1: Zero vs negative one should fail the hex32 equality assertion. */
void testNotEqualHex32sIfSigned_ZeroVsNegativeOne(void)
{
    UNITY_INT32 v0, v1;
    v0 = 0;
    v1 = -1;
    // Expect TEST_ASSERT_EQUAL_HEX32 to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
}

/* Test 2: Negative one vs positive one should fail the hex32 equality assertion. */
void testNotEqualHex32sIfSigned_NegOneVsPosOne(void)
{
    UNITY_INT32 v0, v1;
    v0 = -1;
    v1 = 1;
    // Expect TEST_ASSERT_EQUAL_HEX32 to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
}

/* Test 3: INT32_MIN vs INT32_MIN + 1 should fail (boundary case). */
void testNotEqualHex32sIfSigned_IntMinPlusOne(void)
{
    UNITY_INT32 v0, v1;
    v0 = INT32_MIN;
    v1 = INT32_MIN + 1;
    // Expect TEST_ASSERT_EQUAL_HEX32 to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
}

/* Test 4: Negative large magnitude vs small negative to ensure failure across ranges. */
void testNotEqualHex32sIfSigned_LargeNegVsSmallNeg(void)
{
    UNITY_INT32 v0, v1;
    v0 = -900000;
    v1 = -900001;
    // Expect TEST_ASSERT_EQUAL_HEX32 to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32(v0, v1);
    VERIFY_FAILS_END
}