/*
Step 2 - Extended test suite for focal method: testNotEqualHex16sIfSigned
Candidate Keywords (core dependencies observed in the focal method):
- UNITY_INT16, v0, v1
- EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_HEX16, VERIFY_FAILS_END
- The test targets the not-equal condition for signed 16-bit hex comparison
- This extension adds multiple sub-cases within the same test to improve branch coverage
*/

void testNotEqualHex16sIfSigned(void)
{
    // Sub-case 1: Original scenario - two distinct negative values
    {
        UNITY_INT16 v0, v1;
        v0 = -1024;
        v1 = -1028;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX16(v0, v1);
        VERIFY_FAILS_END
    }

    // Sub-case 2: Both negative but very close values
    {
        UNITY_INT16 v0 = -1;
        UNITY_INT16 v1 = -2;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX16(v0, v1);
        VERIFY_FAILS_END
    }

    // Sub-case 3: Boundary-like values (INT16_MIN vs INT16_MAX)
    {
        UNITY_INT16 v0 = -32768; // INT16_MIN -> 0x8000 in hex
        UNITY_INT16 v1 = 32767;  // INT16_MAX -> 0x7FFF in hex
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX16(v0, v1);
        VERIFY_FAILS_END
    }

    // Sub-case 4: Small difference within negative range
    {
        UNITY_INT16 v0 = -2;
        UNITY_INT16 v1 = -3;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX16(v0, v1);
        VERIFY_FAILS_END
    }

    // Sub-case 5: Cross sign difference
    {
        UNITY_INT16 v0 = 0;
        UNITY_INT16 v1 = -1;
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_HEX16(v0, v1);
        VERIFY_FAILS_END
    }
}