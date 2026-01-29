/*
  Test Suite: NotEqualUINT16EachEqual1 Variants
  Context:
  - Focus on the focal behavior tested by testNotEqualUINT16EachEqual1 in testunity.c
  - The underlying assertion is TEST_ASSERT_EACH_EQUAL_UINT16(expected, array, count)
  - The test is expected to abort (fail) because at least one element does not match 'expected'
  - This suite adds additional variant scenarios to improve coverage for different mismatch positions
  - Built to compile with C++11, using Unity (no GTest)
  - Static class members and private data are not directly touched in these tests; Unity handles the test harness
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Note: This test file is compiled alongside testunity.c (which provides the focal tests).
// We do not rely on the focal function directly; instead we replicate the failure scenarios
// to ensure coverage of the failure paths for TEST_ASSERT_EACH_EQUAL_UINT16 with 4 elements.

#ifdef __cplusplus
extern "C" {
#endif
// If needed by the build system, expose the focal test symbol.
// void testNotEqualUINT16EachEqual1(void);
#ifdef __cplusplus
}
#endif

// Candidate Keywords (for traceability in documentation):
// - TEST_ASSERT_EACH_EQUAL_UINT16
// - UINT16 / unsigned short
// - 65132u (expected value)
// - 4 (count)
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (non-terminating abort checks)


// Variant 1: Mismatch at the first element
// Expect the assertion to fail because the first element (1) is not equal to the expected 65132u
void testNotEqualUINT16EachEqual1_Variant_FirstElementDiff(void)
{
    unsigned short p0[] = {1, 65132u, 65132u, 65132u};
    // We expect the test to abort due to a mismatch in the first element
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// Variant 2: Mismatch at the second element
// Expect the assertion to fail because the second element (0x0000) is not equal to 65132u
void testNotEqualUINT16EachEqual1_Variant_SecondElementDiff(void)
{
    unsigned short p0[] = {65132u, 0, 65132u, 65132u};
    // We expect the test to abort due to a mismatch in the second element
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// Variant 3: Mismatch at the last element
// Expect the assertion to fail because the last element (0) is not equal to 65132u
void testNotEqualUINT16EachEqual1_Variant_LastElementDiff(void)
{
    unsigned short p0[] = {65132u, 65132u, 65132u, 0};
    // We expect the test to abort due to a mismatch in the last element
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}


// Minimal test runner to execute the above variants
int main(void)
{
    UnityBegin("test_NotEqualUINT16EachEqual1_variants_suite_cpp");

    RUN_TEST(testNotEqualUINT16EachEqual1_Variant_FirstElementDiff);
    RUN_TEST(testNotEqualUINT16EachEqual1_Variant_SecondElementDiff);
    RUN_TEST(testNotEqualUINT16EachEqual1_Variant_LastElementDiff);

    return UnityEnd();
}