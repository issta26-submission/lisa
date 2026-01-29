// Additional test cases to improve coverage for testNotEqualUINT8EachEqual1.
// These tests are intended to be appended to testunity.c so they share the same
// macro definitions (EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, TEST_ASSERT_EACH_EQUAL_UINT8, etc.)

// 1) Pass case: all elements equal to the expected value should pass.
void testNotEqualUINT8EachEqual1_PassCase(void)
{
    // Objective: Verify that TEST_ASSERT_EACH_EQUAL_UINT8 passes when all elements
    // are equal to the expected value (127u) across the entire array.
    unsigned char p0[] = {127u, 127u, 127u, 127u};
    // Expect no abort; all elements match the expected value.
    TEST_ASSERT_EACH_EQUAL_UINT8(127u, p0, 4);
}

// 2) Fail case: exactly like the original scenario, where one element differs (128u).
void testNotEqualUINT8EachEqual1_FailCase(void)
{
    // Objective: Replicate the original failing path where at least one element
    // does not match the expected value, causing the assertion to abort.
    unsigned char p0[] = {127u, 127u, 128u, 127u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(127u, p0, 4);
    VERIFY_FAILS_END
}

// 3) Length-zero case: with count = 0, the assertion should be a no-op and pass.
void testNotEqualUINT8EachEqual1_LengthZero(void)
{
    // Objective: Ensure that a zero-length check does not access memory and passes.
    unsigned char p0[] = {127u}; // dummy data; won't be read due to length 0
    TEST_ASSERT_EACH_EQUAL_UINT8(127u, p0, 0);
}

// 4) Null pointer with zero length: should also be safe and pass (no dereference occurs).
void testNotEqualUINT8EachEqual1_NullPtrLengthZero(void)
{
    // Objective: Verify that passing a NULL pointer with length 0 does not crash.
    TEST_ASSERT_EACH_EQUAL_UINT8(0u, NULL, 0);
}