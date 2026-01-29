/*
  Test Suite: Variants of testNotEqualUINT8EachEqual2
  Context:
  - Focal method under test is the Unity-based assertion path TEST_ASSERT_EACH_EQUAL_UINT8
    used inside testNotEqualUINT8EachEqual2 in testunity.c.
  - The original testNotEqualUINT8EachEqual2 verifies that the assertion aborts when
    not all elements in a UINT8 array are equal to the expected value (1 in the example).

  Objective:
  - Provide additional, self-contained tests that exercise similar abort behavior for
    TEST_ASSERT_EACH_EQUAL_UINT8 with different input arrays. This enhances coverage
    for the Not Equal scenario across common edge configurations (different positions of the
    non-matching element, and variations in the non-matching value).

  Notes:
  - This file is intended to be compiled with Unity (unity.c/.h) and linked along with
    the existing testunity.c so that a single Unity test-runner executes all tests.
  - Each test uses Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END wrappers to capture
    the expected abort without terminating the entire test suite.
  - The tests focus on UINT8 data (unsigned char) to mirror TEST_ASSERT_EACH_EQUAL_UINT8 usage.

  Candidate Keywords identified from the focal method:
  - TEST_ASSERT_EACH_EQUAL_UINT8
  - UINT8 (unsigned char)
  - 4 (length) as an example
  - Expect abort behavior on mismatch
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (abort-capturing wrappers)

  This file can be appended to the project and built with the existing Unity setup.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// If the project uses setUp/tearDown, provide no-op implementations here.
void setUp(void)   { }
void tearDown(void){ }

/*
  Test 1: VariantLastChanged
  Rationale: Last element is different from the expected value (1), causing an abort.
*/
void testNotEqualUINT8EachEqual2_VariantLastChanged(void)
{
    // p0 contains 1,1,1,0 where the last element deviates from 1
    unsigned char p0[] = {1, 1, 1, 0};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(1, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 2: VariantFirstChanged
  Rationale: First element is different from the expected value (1), causing an abort.
*/
void testNotEqualUINT8EachEqual2_VariantFirstChanged(void)
{
    // p0 contains 0,1,1,1 where the first element deviates from 1
    unsigned char p0[] = {0, 1, 1, 1};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(1, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 3: VariantMiddleChanged
  Rationale: A middle element is different from the expected value (1), causing an abort.
*/
void testNotEqualUINT8EachEqual2_VariantMiddleChanged(void)
{
    // p0 contains 1,1,2,1 where the middle element deviates from 1
    unsigned char p0[] = {1, 1, 2, 1};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(1, p0, 4);
    VERIFY_FAILS_END
}

/*
  Optional: You can add more edge-case variants if needed, such as:
  - All elements equal except for a value outside the UINT8 range (not applicable here since
    TEST_ASSERT_EACH_EQUAL_UINT8 compares within 0..255, but could be simulated with 0xFF+1
    not representable in a uint8_t type, so we keep within valid UINT8 range).
*/

/*
  Registration and test runner:
  - The following main can be used to run just these three variants in isolation.
    If you already have a combined test runner (e.g., in testunity.c), you can
    instead add RUN_TEST calls for these three tests there to include them in the
    existing suite.
*/


int main(void)
{
    UnityBegin("testNotEqualUINT8EachEqual2_variants.c");

    // Register and run tests
    // If using an existing test harness with RUN_TEST macros, replace with:
    // RUN_TEST(testNotEqualUINT8EachEqual2_VariantLastChanged);
    // RUN_TEST(testNotEqualUINT8EachEqual2_VariantFirstChanged);
    // RUN_TEST(testNotEqualUINT8EachEqual2_VariantMiddleChanged);

    // Direct invocation (standalone runner):
    testNotEqualUINT8EachEqual2_VariantLastChanged();
    testNotEqualUINT8EachEqual2_VariantFirstChanged();
    testNotEqualUINT8EachEqual2_VariantMiddleChanged();

    return UnityEnd();
}