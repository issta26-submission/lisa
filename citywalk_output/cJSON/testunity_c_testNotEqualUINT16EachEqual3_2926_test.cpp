/*
Candidate Keywords:
- testNotEqualUINT16EachEqual3
- UINT16, unsigned short
- TEST_ASSERT_EACH_EQUAL_UINT16
- 65132u, 65133u, 65132u[]
- 4 (array length)
- EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
- ALL_EQUAL vs NOT_EQUAL scenarios
- 0/65535 boundary values
- UNITY macros (Unity test framework)
- test harness main (RUN_TEST, UnityBegin/End)
- C++11 compatibility (extern "C" wrappers for Unity includes if compiled as C++)
Notes:
- This file provides an independent test suite targeting the focal method
  testNotEqualUINT16EachEqual3 from testunity.c by exercising both the
  failure path (abort) and the success path (no abort) for the
  TEST_ASSERT_EACH_EQUAL_UINT16 macro with UINT16 data.
- The suite includes variations to cover edge values and typical failure points
  (e.g., last element differs, multiple elements differ, boundary values).
- It uses non-terminating assertions (where applicable) to maximize coverage,
  but also includes a dedicated abort-based test mirroring the focal function.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
}
#endif

// Candidate Keywords implemented via tests below:
// - UINT16, unsigned short
// - TEST_ASSERT_EACH_EQUAL_UINT16
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
// - 4-element arrays with a single differing element
// - Boundary values: 0, 65535, and typical values like 65132/65133
// - Both failing (abort) and passing (no abort) test cases

// Forward declarations for Unity setup/teardown if needed
void setUp(void);
void tearDown(void);

// Variants of testNotEqualUINT16EachEqual3 to increase coverage around the focal case

// 1) Original focal case reasserting the expected failure path
//    This mirrors the exact scenario in the focal method under test.
void testNotEqualUINT16EachEqual3_Original(void)
{
    unsigned short p0[] = {65132u, 65132u, 65132u, 65133u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// 2) All elements equal to the expected value -> should pass (no abort)
void testNotEqualUINT16EachEqual3_AllEqualShouldPass(void)
{
    unsigned short p0[] = {65132u, 65132u, 65132u, 65132u};
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
}

// 3) Second element differs -> should abort
void testNotEqualUINT16EachEqual3_SecondDiffShouldAbort(void)
{
    unsigned short p0[] = {65132u, 65131u, 65132u, 65133u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// 4) Last element differs while others are equal -> should abort
void testNotEqualUINT16EachEqual3_LastDiffShouldAbort(void)
{
    unsigned short p0[] = {0u, 0u, 0u, 1u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(0u, p0, 4);
    VERIFY_FAILS_END
}

// 5) Boundary test: first three equal to 65535, last differs -> abort
void testNotEqualUINT16EachEqual3_BoundaryLastDiffAbort(void)
{
    unsigned short p0[] = {65535u, 65535u, 65535u, 65534u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65535u, p0, 4);
    VERIFY_FAILS_END
}

// 6) All elements different from the expected value -> abort
void testNotEqualUINT16EachEqual3_AllDifferentAbort(void)
{
    unsigned short p0[] = {1000u, 2000u, 3000u, 4000u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(1000u, p0, 4);
    VERIFY_FAILS_END
}

// 7) Minimal size variation: still 4 elements but with mixed values around the expected
void testNotEqualUINT16EachEqual3_MixedDiffAbort(void)
{
    unsigned short p0[] = {65132u, 65132u, 65131u, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// 8) Larger UINT16 value near max but with a mismatch to trigger abort
void testNotEqualUINT16EachEqual3_MaxNearBoundaryAbort(void)
{
    unsigned short p0[] = {65535u, 65535u, 65534u, 65535u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65535u, p0, 4);
    VERIFY_FAILS_END
}

// Test runner: main that executes the focused test suite
int main(void)
{
    UnityBegin("testNotEqualUINT16EachEqual3_suite");

    // Focal test cases (covering both ABORT path and PASS path)
    RUN_TEST(testNotEqualUINT16EachEqual3_Original);
    RUN_TEST(testNotEqualUINT16EachEqual3_AllEqualShouldPass);
    RUN_TEST(testNotEqualUINT16EachEqual3_SecondDiffShouldAbort);
    RUN_TEST(testNotEqualUINT16EachEqual3_LastDiffShouldAbort);
    RUN_TEST(testNotEqualUINT16EachEqual3_BoundaryLastDiffAbort);
    RUN_TEST(testNotEqualUINT16EachEqual3_AllDifferentAbort);
    RUN_TEST(testNotEqualUINT16EachEqual3_MixedDiffAbort);
    RUN_TEST(testNotEqualUINT16EachEqual3_MaxNearBoundaryAbort);

    return UnityEnd();
}