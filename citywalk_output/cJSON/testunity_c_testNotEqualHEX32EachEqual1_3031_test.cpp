/*
  Test Suite: Focused tests around the focal method logic testNotEqualHEX32EachEqual1
  This C++11 compatible test uses the Unity framework (no GTest) and mirrors
  the core behavior demonstrated by the focal test, while adding additional
  coverage tests to improve robustness.

  Step 1 (Conceptual): Candidate Keywords extracted from the focal method and
  its dependencies include:
  - TEST_ASSERT_EACH_EQUAL_HEX32
  - UNITY_UINT32 (and related UNITY_* types)
  - p0 (array under test)
  - 65132u, 8 (example hex32 values used in tests)
  - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END (control flow for expected aborts)
  - startPutcharSpy / endPutcharSpy / getBufferPutcharSpy (I/O spying hooks)
  - Macros for abort handling and Unity test state management
  The tests below cover both failing and passing scenarios for the macro
  TEST_ASSERT_EACH_EQUAL_HEX32, aligning with the focal test's intent.

  Step 2/3: The test suite here provides:
  - A wrapper test that mirrors the focal testNotEqualHEX32EachEqual1 and asserts an abort.
  - An alternate failing variant to ensure the macro aborts for a different layout.
  - A passing variant to ensure the macro succeeds when all elements are equal.
  - Lightweight stubs for Unity "spy" I/O to enable EXPECT_ABORT_BEGIN/END in C++11.

  Note: This suite is self-contained and compatible with C++11, and uses Unity's
  style assertions without GTest. It is intended to be compiled alongside the
  existing Unity sources (unity.c/h) in a C++11 build.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Lightweight stubs to satisfy EXPECT_ABORT_BEGIN / VERIFY_FAILS_END usage
void startPutcharSpy(void) {}
void endPutcharSpy(void) {}
char* getBufferPutcharSpy(void) { return nullptr; }
void putcharSpy(int c) { (void)c; }

// Optional: provide minimal setUp/tearDown if Unity expects them
void setUp(void) {}
void tearDown(void) {}

// 1) Failing variant mirroring the focal testNotEqualHEX32EachEqual1 (Variant A)
void testNotEqualHEX32EachEqual1_VariantA(void)
{
    // This array contains three instances of 65132u and one differing value (8)
    // The TEST_ASSERT_EACH_EQUAL_HEX32 should FAIL, causing an abort path.
    UNITY_UINT32 p0[] = {65132u, 8, 65132u, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
    VERIFY_FAILS_END
}

// 2) Failing variant mirroring a different position of the mismatch (Variant B)
void testNotEqualHEX32EachEqual1_VariantB(void)
{
    // Mismatch is in the last element; still should abort with the same macro
    UNITY_UINT32 p0[] = {65132u, 65132u, 65132u, 8};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
    VERIFY_FAILS_END
}

// 3) Passing variant to ensure macro succeeds when all elements are equal
void testEqualHEX32EachEqual_Pass(void)
{
    // All elements equal; macro should pass (no abort)
    UNITY_UINT32 p0[] = {65132u, 65132u, 65132u, 65132u};
    TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
}

// Optional: A direct assertion style to complement the focal behavior
void testNotEqualHEX32EachEqual_FailureWithDifferentLayout(void)
{
    // A variant where the array is not uniform, ensuring the macro aborts
    UNITY_UINT32 p0[] = {0xF001u, 0xF001u, 0xF001u, 0xF002u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX32(0xF001u, p0, 4);
    VERIFY_FAILS_END
}

// Unity requires a main to drive the tests when not using a test runner binary.
// This main executes the tests defined above at least once to ensure executable coverage.
int main(void)
{
    UNITY_BEGIN("testNotEqualHEX32EachEqual1 - focused coverage suite (C++11)");

    // Run the focused tests to ensure abort behavior and success paths
    RUN_TEST(testNotEqualHEX32EachEqual1_VariantA);
    RUN_TEST(testNotEqualHEX32EachEqual1_VariantB);
    RUN_TEST(testNotEqualHEX32EachEqual_FailureWithDifferentLayout);
    RUN_TEST(testEqualHEX32EachEqual_Pass);

    return UNITY_END();
}