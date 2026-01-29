/* 
   Supplemental test suite additions for testUINT32sNotWithinDelta
   Purpose: Extend coverage around the focal method by introducing
   additional scenarios that exercise the NOT-within-delta path,
   including custom messages and edge-cases, while using Unity macros
   (to be compiled together with the existing testunity.c).
   Note: This patch is intended to be inserted into the existing
   testunity.c where the FOCAL_CLASS_DEP section resides and where
   main() registers tests with RUN_TEST. Ensure to add the new tests
   to the RUN_TEST list in main() accordingly.
*/

/* Basic Not-Within-Delta: same as original but provided for alternative naming */
void testUINT32sNotWithinDelta_BasicNotWithin(void)
{
    // Verifies that a difference greater than delta triggers a failure
    // Delta = 1, expected = 2147483647, actual = 2147483649 -> diff = 2 > delta
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(1, 2147483647u, 2147483649u);
    VERIFY_FAILS_END
}

/* Basic Not-Within-Delta with Custom Message */
void testUINT32sNotWithinDelta_BasicNotWithin_WithMessage(void)
{
    // Same scenario as above but with a custom message
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message for NotWithinDelta");
    VERIFY_FAILS_END
}

/* Edge Case: Not-Within-Delta with large unsigned numbers near wrap boundary */
void testUINT32sNotWithinDelta_EdgeLargeNotWithin(void)
{
    // delta = 2, expected = 0xFFFFFFFF, actual = 0xFFFFFFFC
    // diff = 3 > delta
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(2, 0xFFFFFFFFu, 0xFFFFFFFCu);
    VERIFY_FAILS_END
}

/* Not-Within-Delta with Custom Message (Edge Case) */
void testUINT32sNotWithinDelta_EdgeLargeNotWithin_WithMessage(void)
{
    // Same as EdgeLargeNotWithin but with a message
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(2, 0xFFFFFFFFu, 0xFFFFFFFCu, "EdgeCaseNotWithinDelta");
    VERIFY_FAILS_END
}

/* Not-Within-Delta where a signed interpretation would pass small first (unsigned) */
void testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassSmallFirst_NotWithin(void)
{
    // delta = 5, expected = 1, actual = -1 (unsigned wrap to 0xFFFFFFFF)
    // diff is large, so NOT within delta should abort
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
}

/* Not-Within-Delta with Custom Message (EvenThoughASignedIntWouldPassSmallFirst) */
void testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassSmallFirst_NotWithin_WithMessage(void)
{
    // Same as above with a custom message
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, 1, -1, "SignedEdgeSmallFirstNotWithin");
    VERIFY_FAILS_END
}

/* Not-Within-Delta where a signed interpretation would pass big first (unsigned) */
void testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassBigFirst_NotWithin(void)
{
    // delta = 5, expected = -1 (unsigned 0xFFFFFFFF), actual = 1
    // diff is large, so NOT within delta should abort
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(5, -1, 1);
    VERIFY_FAILS_END
}

/* Not-Within-Delta with Custom Message (EvenThoughASignedIntWouldPassBigFirst) */
void testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassBigFirst_NotWithin_WithMessage(void)
{
    // Same as above with a custom message
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, -1, 1, "SignedEdgeBigFirstNotWithin");
    VERIFY_FAILS_END
}

/*
  Suggested main() augmentation (to be inserted where RUN_TEST calls are listed):
  RUN_TEST(testUINT32sNotWithinDelta_BasicNotWithin);
  RUN_TEST(testUINT32sNotWithinDelta_BasicNotWithin_WithMessage);
  RUN_TEST(testUINT32sNotWithinDelta_EdgeLargeNotWithin);
  RUN_TEST(testUINT32sNotWithinDelta_EdgeLargeNotWithin_WithMessage);
  RUN_TEST(testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassSmallFirst_NotWithin);
  RUN_TEST(testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassSmallFirst_NotWithin_WithMessage);
  RUN_TEST(testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassBigFirst_NotWithin);
  RUN_TEST(testUINT32sNotWithinDelta_EvenThoughASignedIntWouldPassBigFirst_NotWithin_WithMessage);
*/