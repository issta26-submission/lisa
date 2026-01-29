/*
  Additional test cases to enhance coverage for the focal method testLessThanHEX32
  These tests exercise both true and false branches of the hex32 less-than predicate
  using value-pairs and pointer-pairs, including edge-like scenarios.
  Comment blocks explain the intent of each test.
*/

void testLessThanHEX32_Passes_AllVariants(void)
{
    // Fallback test: ensure that when a < b, the macro passes in all forms
    UNITY_UINT32 v0 = 0x00000001;       // small value
    UNITY_UINT32 v1 = 0x00000002;       // slightly larger value
    UNITY_UINT32 *p0 = &v0;
    UNITY_UINT32 *p1 = &v1;

    // Direct comparison
    TEST_ASSERT_LESS_THAN_HEX32(v0, v1);      // 1 < 2
    // Pointer dereference on left
    TEST_ASSERT_LESS_THAN_HEX32(*p0, v1);     // 1 < 2
    // Pointer dereference on right
    TEST_ASSERT_LESS_THAN_HEX32(v0, *p1);     // 1 < 2
    // Both sides dereferenced
    TEST_ASSERT_LESS_THAN_HEX32(*p0, *p1);    // 1 < 2
}

void testLessThanHEX32_FailsForGreaterThan(void)
{
    // Ensure that a > b triggers a failure (abort path guarded with EXPECT_ABORT_BEGIN)
    UNITY_UINT32 v0 = 0xFFFFFFFF;
    UNITY_UINT32 v1 = 0x00000000;
    UNITY_UINT32 *p0 = &v0;
    UNITY_UINT32 *p1 = &v1;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX32(v0, v1);        // 0xFFFFFFFF < 0x00000000 => false
    VERIFY_FAILS_END
}

void testLessThanHEX32_PointerVsPointer_Variant(void)
{
    // Ensure true path when both are pointers and left < right
    UNITY_UINT32 a = 0x00000001;
    UNITY_UINT32 b = 0x00000010;
    UNITY_UINT32 *pa = &a;
    UNITY_UINT32 *pb = &b;

    // Pointer vs pointer where *pa < *pb
    TEST_ASSERT_LESS_THAN_HEX32(*pa, *pb);
}

void testLessThanHEX32_EqualCase_Fails(void)
{
    // Equal values must fail the less-than check
    UNITY_UINT32 a = 0x12345678;
    UNITY_UINT32 b = 0x12345678;
    UNITY_UINT32 *pa = &a;
    UNITY_UINT32 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX32(*pa, *pb);
    VERIFY_FAILS_END
}

void testLessThanHEX32_BoundsEdgeCase(void)
{
    // Edge-like case: 0 vs max to ensure boundary behavior
    UNITY_UINT32 a = 0;
    UNITY_UINT32 b = 0xFFFFFFFF;
    UNITY_UINT32 *pa = &a;
    UNITY_UINT32 *pb = &b;

    // 0 < 0xFFFFFFFF should pass
    TEST_ASSERT_LESS_THAN_HEX32(a, b);
    TEST_ASSERT_LESS_THAN_HEX32(*pa, *pb);

    // Reversing should fail
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_HEX32(*pb, a);
    VERIFY_FAILS_END
}