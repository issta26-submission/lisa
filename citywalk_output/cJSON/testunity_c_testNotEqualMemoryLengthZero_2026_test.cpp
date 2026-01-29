/* 
   Extended test suite additions for the focal method:
   testNotEqualMemoryLengthZero located in testunity.c

   This patch adds multiple additional test scenarios to exercise
   memory length zero interactions with TEST_ASSERT_EQUAL_MEMORY.

   Notes:
   - The tests rely on the same Unity-based testing harness used by
     testunity.c (i.e., test protection wrappers around abortable asserts).
   - Each test includes comments explaining the intent and the expected
     behavior as understood from the focal method's semantics.
   - Do not remove existing testNotEqualMemoryLengthZero; these tests
     augment coverage around zero-length memory comparisons with
     NULL and non-NULL pointers.
*/

/*  Candidate Keywords (for traceability of the focal method's dependencies)
    - TEST_ASSERT_EQUAL_MEMORY
    - memory comparison
    - NULL pointers
    - zero length (0) memory length
    - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (abort capture)
    - startPutcharSpy / endPutcharSpy (I/O capture in Unity harness)
    - test protection and tearDown handling
*/

/* -------------------------------------------------------------------------- */
/* Additional test cases for testNotEqualMemoryLengthZero                      */
/* These tests explore various pointer combinations with a zero length.       */
/* They are intended to broaden coverage where memory length is zero.          */
/* -------------------------------------------------------------------------- */

/* 
   Test: Non-null pointers with zero length should behave consistently with
   zero-length comparisons. Depending on the Unity harness' internals, this
   may succeed (not abort) or be treated as a special case. We document the
   expectation in comments, and the test asserts aborting to align with the
   behavior observed in testNotEqualMemoryLengthZero for NULL pointers.
*/
void testNotEqualMemoryLengthZero_NonNullPointersZeroLength(void)
{
    // Intent: verify behavior when both pointers are non-NULL but length == 0
    // This may or may not trigger a failure depending on internal checks.
    // We follow the same abort-check pattern as testNotEqualMemoryLengthZero.
    EXPECT_ABORT_BEGIN
    {
        int a = 0;
        int b = 1;
        TEST_ASSERT_EQUAL_MEMORY(&a, &b, 0);
    }
    VERIFY_FAILS_END
}

/* 
   Test: NULL actual pointer with NULL expected pointer and zero length
   (Redundant with the focal test, but ensures consistency across
    differing code paths in the harness when both inputs are NULL).
*/
void testNotEqualMemoryLengthZero_BothNullsZeroLength_Variant(void)
{
    // Intent: ensure identical zero-length comparison with NULL inputs follows
    // the same abort/handling semantics as the primary test.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, NULL, 0);
    VERIFY_FAILS_END
}

/* 
   Test: NULL expected pointer and non-NULL actual pointer with zero length
   This checks that the harness maintains consistent behavior when one
   input is NULL and the other is non-NULL for a zero-length comparison.
*/
void testNotEqualMemoryLengthZero_NullExpected_NonNullActualZeroLength(void)
{
    // Intent: one side NULL, the other non-NULL, length == 0
    // Expect abort to mirror the zero-length comparison semantics.
    EXPECT_ABORT_BEGIN
    {
        int actual = 42;
        TEST_ASSERT_EQUAL_MEMORY(NULL, &actual, 0);
    }
    VERIFY_FAILS_END
}

/* 
   Test: Non-NULL actual and NULL expected pointer with zero length
   Mirror of previous test, verifying symmetry of the zero-length path.
*/
void testNotEqualMemoryLengthZero_NonNullActual_NullExpectedZeroLength(void)
{
    // Intent: one side NULL, the other non-NULL, length == 0
    // Expect abort to mirror the zero-length comparison semantics.
    EXPECT_ABORT_BEGIN
    {
        int actual = 123;
        TEST_ASSERT_EQUAL_MEMORY(&actual, NULL, 0);
    }
    VERIFY_FAILS_END
}

/* End of extended test cases for testNotEqualMemoryLengthZero */
/* -------------------------------------------------------------------------- */