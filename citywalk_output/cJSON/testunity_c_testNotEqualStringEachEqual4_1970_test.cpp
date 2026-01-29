// Extended test suite additions for testNotEqualStringEachEqual4
// These tests are intended to be inserted into testunity.c within the <FOCAL_CLASS_DEP> area
// They extend coverage around TEST_ASSERT_EACH_EQUAL_STRING with 4-element arrays

// 1) All elements are the expected string -> should pass (no abort)
void testNotEqualStringEachEqual4_PassAllFoo(void)
{
    // All entries equal to "foo" -> the assertion should succeed without aborting
    const char *testStrings[] = { "foo", "foo", "foo", "foo" };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
}

// 2) Array contains a NULL entry -> should fail (abort)
void testNotEqualStringEachEqual4_WithNullInArray(void)
{
    // Including NULL in the array should cause the comparison to fail
    const char *testStrings[] = { "foo", "foo", NULL, "foo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// 3) Case mismatch in one element -> should fail (abort)
void testNotEqualStringEachEqual4_WithDifferentCase(void)
{
    // One element differs by case; should fail the equality check
    const char *testStrings[] = { "Foo", "foo", "foo", "foo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// 4) No element matches the expected string -> should fail (abort)
void testNotEqualStringEachEqual4_AllNotEqual(void)
{
    // None of the elements equal "foo"; the test should abort
    const char *testStrings[] = { "bar", "baz", "woo", "hoo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}