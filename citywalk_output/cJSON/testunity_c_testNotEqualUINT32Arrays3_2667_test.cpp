/*
  Candidate Keywords:
  - UNITY_UINT32, TEST_ASSERT_EQUAL_UINT32_ARRAY
  - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - p0, p1 (array variables)
  - 4 (array length)
  - TEST_ASSERT_EQUAL_UINT32_ARRAY (comparison primitive)
  - testNotEqualUINT32Arrays3 (focal method inspiration)
  - TEST_ABORT semantics (abort on mismatch)
  - UNITY_BEGIN/UNITY_END / RUN_TEST (test harness)
  - extern "C" linkage for Unity headers when compiled as C++
*/

/*
  This test suite targets the focal method scenario testNotEqualUINT32Arrays3
  by providing multiple concrete sub-tests that exercise not-equal detection
  for 4-element uint32 arrays using Unity's TEST_ASSERT_EQUAL_UINT32_ARRAY macro.
  It also includes a pass-case to verify the positive path (equal arrays).
  The tests are written in C++11 compatible style but rely on Unity's C API.
*/

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Test 1: First element differs -> should abort (not equal detected)
void testNotEqualUINT32Arrays3_Variant_FirstElementDiff(void)
{
    // Core scenario: arrays differ at index 0
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {2, 8, 987, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 2: Third element differs -> should abort (not equal detected)
void testNotEqualUINT32Arrays3_Variant_ThirdElementDiff(void)
{
    // Core scenario: arrays differ at index 2
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 986, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 3: Fourth element differs -> should abort (not equal detected)
void testNotEqualUINT32Arrays3_Variant_FourthElementDiff(void)
{
    // Core scenario: arrays differ at index 3
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65131u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 4: Multiple differences -> should abort (not equal detected)
void testNotEqualUINT32Arrays3_Variant_MultipleDiffs(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {0, 7, 986, 65131u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 5: Pass-case (arrays are equal) to ensure non-abort path is exercised
void testEqualUINT32Arrays3_Pass(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65132u};
    // This should pass without abort
    TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4);
}

int main(void)
{
    UNITY_BEGIN();

    // Register tests
    RUN_TEST(testNotEqualUINT32Arrays3_Variant_FirstElementDiff);
    RUN_TEST(testNotEqualUINT32Arrays3_Variant_ThirdElementDiff);
    RUN_TEST(testNotEqualUINT32Arrays3_Variant_FourthElementDiff);
    RUN_TEST(testNotEqualUINT32Arrays3_Variant_MultipleDiffs);
    RUN_TEST(testEqualUINT32Arrays3_Pass);

    return UNITY_END();
}