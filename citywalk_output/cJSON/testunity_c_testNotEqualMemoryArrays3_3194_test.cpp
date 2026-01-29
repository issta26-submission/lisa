// Expanded testNotEqualMemoryArrays3 to improve coverage for memory array comparison
// and to exercise multiple not-equal scenarios using Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END.
// This patch should be placed in testunity.c replacing the original single-case function.

void testNotEqualMemoryArrays3(void)
{
    // Test 1: Middle element mismatch (original scenario)
    {
        int p0[] = {1, 8, 987, -2};
        int p1[] = {1, 8, 986, -2};
        // Expect the assertion to fail and be aborted
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
        VERIFY_FAILS_END
    }

    // Test 2: First element mismatch
    {
        int p0[] = {5, 8, 987, -2};
        int p1[] = {6, 8, 987, -2};
        // Expect the assertion to fail due to first element difference
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
        VERIFY_FAILS_END
    }

    // Test 3: Last element mismatch
    {
        int p0[] = {1, 8, 987, -2};
        int p1[] = {1, 8, 987, -3};
        // Expect the assertion to fail due to last element difference
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
        VERIFY_FAILS_END
    }

    // Test 4: Equal arrays (positive path, should not abort)
    {
        int p0[] = {1, 2, 3, 4};
        int p1[] = {1, 2, 3, 4};
        // This should pass without aborting
        TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4);
    }
}