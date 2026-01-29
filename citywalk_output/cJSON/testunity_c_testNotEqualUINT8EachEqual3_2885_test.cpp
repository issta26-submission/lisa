/*
  Test Suite: NotEqualUINT8EachEqual3 variants

  Step 1 - Candidate Keywords (derived from focal method):
  - p0: unsigned char array
  - 55u: target value
  - TEST_ASSERT_EACH_EQUAL_UINT8: core dependency under test
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: abort-on-failure handling
  - 4: length of the array

  Step 2 - Focus: Extend coverage around testNotEqualUINT8EachEqual3 to exercise
  failure paths of TEST_ASSERT_EACH_EQUAL_UINT8 for 4-element unsigned 8-bit arrays,
  plus a positive path to demonstrate a successful case.

  Step 3 - Test Case Refinement:
  - Three negative-variant tests asserting aborts with arrays not all equal to 55
  - One positive test asserting the macro passes when all elements are 55
  - All tests are self-contained and do not rely on private state
  - Compatible with Unity framework (C) and usable in a C++11 build
  - Tests use extern "C" linkage for Unity headers when compiled as C++
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

// Expose testable variants for C++ compilation environments
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations (if needed by the test harness)
void testNotEqualUINT8EachEqual3_Variant1(void);
void testNotEqualUINT8EachEqual3_Variant2(void);
void testNotEqualUINT8EachEqual3_Variant3(void);
void testNotEqualUINT8EachEqual3_AllEqualShouldPass(void);

#ifdef __cplusplus
}
#endif

/* Minimal setUp/tearDown for Unity tests */
void setUp(void)  { }
void tearDown(void) { }

/*
  Variant 1:
  - Array: {54, 55, 55, 55}
  - Expect TEST_ASSERT_EACH_EQUAL_UINT8(55, p0, 4) to abort
*/
void testNotEqualUINT8EachEqual3_Variant1(void)
{
    unsigned char p0[] = {54u, 55u, 55u, 55u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(55u, p0, 4);
    VERIFY_FAILS_END
}

/*
  Variant 2:
  - Array: {55, 54, 55, 55}
  - Expect abort since not all elements are 55
*/
void testNotEqualUINT8EachEqual3_Variant2(void)
{
    unsigned char p0[] = {55u, 54u, 55u, 55u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(55u, p0, 4);
    VERIFY_FAILS_END
}

/*
  Variant 3:
  - Array: {54, 56, 55, 55}
  - Expect abort since not all elements are 55
*/
void testNotEqualUINT8EachEqual3_Variant3(void)
{
    unsigned char p0[] = {54u, 56u, 55u, 55u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(55u, p0, 4);
    VERIFY_FAILS_END
}

/*
  Positive control:
  - Array: {55, 55, 55, 55}
  - Expect macro to pass (not abort)
*/
void testNotEqualUINT8EachEqual3_AllEqualShouldPass(void)
{
    unsigned char p0[] = {55u, 55u, 55u, 55u};
    TEST_ASSERT_EACH_EQUAL_UINT8(55u, p0, 4);
}

/* 
  Main test runner
*/
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testNotEqualUINT8EachEqual3_Variant1);
    RUN_TEST(testNotEqualUINT8EachEqual3_Variant2);
    RUN_TEST(testNotEqualUINT8EachEqual3_Variant3);
    RUN_TEST(testNotEqualUINT8EachEqual3_AllEqualShouldPass);
    return UNITY_END();
}