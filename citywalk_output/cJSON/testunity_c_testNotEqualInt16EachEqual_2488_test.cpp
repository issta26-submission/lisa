/*
Candidate Keywords extracted for focusing testNotEqualInt16EachEqual:
- TEST_ASSERT_EACH_EQUAL_INT16
- UNITY_INT16
- p0 (input arrays)
- 4 (length parameter)
- EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (aborting test expectation)
- NULL pointer handling for p0
- Negative / boundary values for 16-bit integers
- Variants of not-equal scenarios to exercise abort path
- Test naming conventions: testNotEqualInt16EachEqual_*
- Macro usage within Unity framework
*/

//
// Additional coverage extensions for the focal method testNotEqualInt16EachEqual
// These tests augment coverage by exercising additional not-equal scenarios
// for 16-bit integer arrays using TEST_ASSERT_EACH_EQUAL_INT16.
// They are intended to be compiled alongside the existing testunity.c
// and invoked by the Unity test runner.
//
 
// Extended 1: last element differs in a mostly-equal 4-element array
// This confirms the abort path when only one element mismatches.
void testNotEqualInt16EachEqual_Extended1(void)
{
    /* Variation: one element differs (third element is 126 instead of 127) */
    UNITY_INT16 p0[] = {127, 127, 126, 127};
    
    // Expect the assertion to abort due to a mismatch
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT16(127, p0, 4);
    VERIFY_FAILS_END
}

// Extended 2: negative values across the array with a single mismatch
// Ensures proper handling of signed 16-bit values in the not-equal path.
void testNotEqualInt16EachEqual_Extended2(void)
{
    /* Variation: all elements are -1 except the last which is -2 */
    UNITY_INT16 p0[] = {-1, -1, -1, -2};
    
    // Expect the assertion to abort due to a mismatch
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT16(-1, p0, 4);
    VERIFY_FAILS_END
}

// Extended 3: zeroes with a single differing element
// Verifies that differences near the boundary of zero cause failure as expected.
void testNotEqualInt16EachEqual_Extended3(void)
{
    /* Variation: first three elements are 0, last is 1 */
    UNITY_INT16 p0[] = {0, 0, 0, 1};
    
    // Expect the assertion to abort due to a mismatch
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT16(0, p0, 4);
    VERIFY_FAILS_END
}

// Extended 4: maximal positive 16-bit values with a single mismatch
// Tests behavior when using boundary values of int16.
void testNotEqualInt16EachEqual_Extended4(void)
{
    /* Variation: first three elements are 32767, last is 0 */
    UNITY_INT16 p0[] = {32767, 32767, 32767, 0};
    
    // Expect the assertion to abort due to a mismatch
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT16(32767, p0, 4);
    VERIFY_FAILS_END
}