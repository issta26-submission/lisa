// Expanded test suite additions for the focal method: testNotEqualHEX16Arrays2
 // These tests extend coverage around the non-equality condition of two HEX16 arrays.
 // They are intended to be appended to the existing Unity-based testunity.c
 // and compiled alongside the original tests. Each test uses the same
 // EXPECT_ABORT_BEGIN / TEST_ASSERT_EQUAL_HEX16_ARRAY / VERIFY_FAILS_END
 // macros so behavior is consistent with the focal test.

 // Test 1: Not equal when the first element differs (identical to the original case but added as a separate test)
 // This reinforces that a difference at the first element correctly triggers a failure.
void testNotEqualHEX16Arrays2_VariantFirstElementDifferent(void)
{
    // Given two 4-element arrays where the first element differs
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u};
    // Expect the assertion to abort (fail) since arrays are not equal
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 2: Not equal when the second element differs
// Validates that a difference in a non-first position still causes the proper abort.
void testNotEqualHEX16Arrays2_VariantSecondElementDifferent(void)
{
    // Given two 4-element arrays differing at index 1
    unsigned short p0[] = {1, 9, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65132u};
    // Expect failure due to inequality
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 3: Not equal when the last element differs
// Ensures differences at the trailing element are still detected and reported as failures.
void testNotEqualHEX16Arrays2_VariantLastElementDifferent(void)
{
    // Given two 4-element arrays differing at the last index
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65133u};
    // Expect failure because the last element does not match
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 4: Not equal when multiple elements differ (stress non-equality across array)
// This checks that the comparator properly identifies inequality even when more than one element differs.
void testNotEqualHEX16Arrays2_MultipleDifferences(void)
{
    // Given two arrays with multiple differing elements
    unsigned short p0[] = {0x0001, 0x0008, 0x03E3, 0xFE10};
    unsigned short p1[] = {0x0002, 0x0009, 0x03E2, 0xFE11};
    // Expect the test to abort due to inequality
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Notes for integration:
// - These tests rely on the Unity test harness macros defined in testunity.c:
//   EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_HEX16_ARRAY, and VERIFY_FAILS_END.
// - The tests preserve the same data types (unsigned short) and array length (4)
//   as used in the focal method to ensure consistency with the original behavior.
// - If you want to increase coverage further, you can add tests that use boundary
//   values for unsigned short (e.g., 0x0000 and 0xFFFF) in the differing position.