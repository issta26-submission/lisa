// Additional coverage for testNotEqualHex8sIfSigned
// This suite expands on the focal test by exercising multiple signed 8-bit value
// pairs that should not be equal, ensuring the assertion fails as expected
// (i.e., Unity reports a test failure via abort mechanism).

// Each block represents an independent scenario to maximize branch/condition coverage.

void testNotEqualHex8sIfSigned_MultipleScenarios(void)
{
    // Scenario A: v0 is negative, v1 is positive and different (-2 vs 2)
    {
        UNITY_INT8 v0 = -2;
        UNITY_INT8 v1 = 2;
        // Expect the equality assertion to fail (abort), since -2 != 2 as HEX8
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX8(v0, v1);
        VERIFY_FAILS_END
    }

    // Scenario B: v0 is negative, v1 is positive and different (-1 vs 1)
    {
        UNITY_INT8 v0 = -1;
        UNITY_INT8 v1 = 1;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX8(v0, v1);
        VERIFY_FAILS_END
    }

    // Scenario C: Edge case -128 (minimum) vs 0
    {
        UNITY_INT8 v0 = -128;
        UNITY_INT8 v1 = 0;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX8(v0, v1);
        VERIFY_FAILS_END
    }

    // Scenario D: Two distinct negative values (-127 vs -1)
    {
        UNITY_INT8 v0 = -127;
        UNITY_INT8 v1 = -1;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX8(v0, v1);
        VERIFY_FAILS_END
    }

    // Scenario E: Distinct sign edge (-10 vs 10)
    {
        UNITY_INT8 v0 = -10;
        UNITY_INT8 v1 = 10;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX8(v0, v1);
        VERIFY_FAILS_END
    }

    // Scenario F: Distinct negative values (-2 vs -4)
    {
        UNITY_INT8 v0 = -2;
        UNITY_INT8 v1 = -4;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX8(v0, v1);
        VERIFY_FAILS_END
    }
}