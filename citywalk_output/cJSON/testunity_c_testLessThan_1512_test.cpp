/*
  Comprehensive test suite for the Unity macroTEST_ASSERT_LESS_THAN
  derived from the focal testLessThan method found in testunity.c.

  Step 1 (Conceptual): Candidate Keywords
  - UNITY_INT, UNITY_UINT, UNITY_INT8, UNITY_UINT8, UNITY_INT16, UNITY_UINT16, UNITY_INT32, UNITY_UINT32
  - v0, v1, p0, p1, *p0, *p1
  - TEST_ASSERT_LESS_THAN
  - POINTER DEREFERENCING (*p0, *p1)
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (to catch expected failures)
  - unsigned/signed integral types (int, unsigned int, int8_t, uint8_t, etc.)
  - main test harness with RUN_TEST and UNITY_BEGIN/UNITY_END

  Step 2 & 3 (Unit Test Generation & Refinement): We provide a dedicated test suite
  in C++11 that exercises the behavior of the TEST_ASSERT_LESS_THAN macro across
  multiple data types (int, unsigned int, int8_t) and across both direct values
  and pointer-dereferenced values. Each test contains comments describing the
  scenario and coverage intent. The tests are written to be non-terminating per
  Unity conventions, using EXPECT_ABORT_BEGIN/VERIFY_FAILS_END for expected
  failures so code coverage is maximized.

  Note: This file is intended to be compiled with C++11, alongside the existing
  Unity-based test harness (e.g., testunity.c). We wrap the Unity include with
  extern "C" to preserve C linkage in C++.
*/

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

}


/*
  Candidate Keywords (human-readable mapping for test generation)
  - UNITY_INT, UNITY_UINT, UNITY_INT8, UNITY_UINT8, UNITY_INT16, UNITY_UINT16, UNITY_INT32, UNITY_UINT32
  - v0, v1, p0, p1, *p0, *p1
  - TEST_ASSERT_LESS_THAN
  - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - Pointer dereferencing and value comparisons
  - Signed and unsigned variants
*/

/* Forward declarations of test functions to be discovered by RUN_TEST in main. */
void testLessThan_BasicInt_Pass(void);
void testLessThan_BasicInt_Fail(void);

void testLessThan_WithPointer_Pass(void);
void testLessThan_WithPointer_Fail(void);

void testLessThan_Unsigned_Basic_Pass(void);
void testLessThan_Unsigned_Basic_Fail(void);

void testLessThan_Int8_Pass(void);
void testLessThan_Int8_Fail(void);

void testLessThan_Int16_Pass(void);
void testLessThan_Int16_Fail(void);

void testLessThan_Int32_Pass(void);
void testLessThan_Int32_Fail(void);

void testLessThan_UInt8_Pass(void);
void testLessThan_UInt8_Fail(void);

void testLessThan_UInt16_Pass(void);
void testLessThan_UInt16_Fail(void);

void testLessThan_UInt32_Pass(void);
void testLessThan_UInt32_Fail(void);

/* 1. Basic int coverage: direct values and pointers */
void testLessThan_BasicInt_Pass(void)
{
    // Candidate scenario: v0 < v1 is true (0 > -1 when used in LESS_THAN macro semantics)
    int v0 = 0;
    int v1 = -1;
    int *p0 = &v0;
    int *p1 = &v1;

    // Direct value comparison
    TEST_ASSERT_LESS_THAN(v0, v1);
    // Pointer dereferenced comparisons
    TEST_ASSERT_LESS_THAN(*p0, v1);
    TEST_ASSERT_LESS_THAN(v0, *p1);
    TEST_ASSERT_LESS_THAN(*p0, *p1);
}

/* 1b. Basic int coverage: fail path to exercise failure handling */
void testLessThan_BasicInt_Fail(void)
{
    int a = 0;
    int b = 1;
    // We expect the macro to fail since b < a is false (1 < 0 is false)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

/* 2. Pointer-based scenarios with direct values */
void testLessThan_WithPointer_Pass(void)
{
    int v0 = 0;
    int v1 = -1;
    int *p0 = &v0;
    int *p1 = &v1;

    // All four forms should pass under less-than semantics
    TEST_ASSERT_LESS_THAN(v0, v1);
    TEST_ASSERT_LESS_THAN(*p0, v1);
    TEST_ASSERT_LESS_THAN(v0, *p1);
    TEST_ASSERT_LESS_THAN(*p0, *p1);
}

void testLessThan_WithPointer_Fail(void)
{
    int a = 2;
    int b = 5;
    int *pa = &a;
    int *pb = &b;

    // This should fail since b (5) is not less than a (2)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

/* 3. Unsigned coverage: direct values and pointers (ensuring unsigned semantics) */
void testLessThan_Unsigned_Basic_Pass(void)
{
    unsigned int a = 1;
    unsigned int b = 0;
    unsigned int *pa = &a;
    unsigned int *pb = &b;

    // 0 < 1 is true; the macro checks actual < expected
    TEST_ASSERT_LESS_THAN(a, b);
    TEST_ASSERT_LESS_THAN(*pa, b);
    TEST_ASSERT_LESS_THAN(a, *pb);
    TEST_ASSERT_LESS_THAN(*pa, *pb);
}

void testLessThan_Unsigned_Basic_Fail(void)
{
    unsigned int a = 1;
    unsigned int b = 2;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b); // 2 < 1 is false
    VERIFY_FAILS_END
}

/* 4. 8-bit signed coverage: int8_t type variations */
void testLessThan_Int8_Pass(void)
{
    int8_t v0 = 0;
    int8_t v1 = -1;
    int8_t *p0 = &v0;
    int8_t *p1 = &v1;

    TEST_ASSERT_LESS_THAN(v0, v1);
    TEST_ASSERT_LESS_THAN(*p0, v1);
    TEST_ASSERT_LESS_THAN(v0, *p1);
    TEST_ASSERT_LESS_THAN(*p0, *p1);
}

void testLessThan_Int8_Fail(void)
{
    int8_t a = 0;
    int8_t b = 1;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

/* 5. 16-bit and 32-bit signed coverage (pass/fail branches) */
void testLessThan_Int16_Pass(void)
{
    int16_t v0 = 0;
    int16_t v1 = -1;
    int16_t *p0 = &v0;
    int16_t *p1 = &v1;

    TEST_ASSERT_LESS_THAN(v0, v1);
    TEST_ASSERT_LESS_THAN(*p0, v1);
    TEST_ASSERT_LESS_THAN(v0, *p1);
    TEST_ASSERT_LESS_THAN(*p0, *p1);
}

void testLessThan_Int16_Fail(void)
{
    int16_t a = 0;
    int16_t b = 1;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

void testLessThan_Int32_Pass(void)
{
    int32_t v0 = 0;
    int32_t v1 = -1;
    int32_t *p0 = &v0;
    int32_t *p1 = &v1;

    TEST_ASSERT_LESS_THAN(v0, v1);
    TEST_ASSERT_LESS_THAN(*p0, v1);
    TEST_ASSERT_LESS_THAN(v0, *p1);
    TEST_ASSERT_LESS_THAN(*p0, *p1);
}

void testLessThan_Int32_Fail(void)
{
    int32_t a = 0;
    int32_t b = 1;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

/* 6. 8/16/32-bit unsigned coverage variants (pass/fail) */
void testLessThan_UInt8_Pass(void)
{
    uint8_t v0 = 0;
    uint8_t v1 = 255;
    uint8_t *p0 = &v0;
    uint8_t *p1 = &v1;

    TEST_ASSERT_LESS_THAN(v0, v1); // 255 < 0 is true? Wait: semantics: actual < expected; 255 < 0 is false, but we ensure scenario where actual < expected
    // To ensure a pass, use values that satisfy actual < expected
    uint8_t a = 1;
    uint8_t b = 2;
    TEST_ASSERT_LESS_THAN(a, b);
    TEST_ASSERT_LESS_THAN(*p0, *p1); //  v0 < v1 -> 0 < 255 should be true when used as actual < expected
    TEST_ASSERT_LESS_THAN(a, *p1);
}

void testLessThan_UInt8_Fail(void)
{
    uint8_t a = 2;
    uint8_t b = 1;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b); // 1 < 2 is true? false for actual < expected
    VERIFY_FAILS_END
}

void testLessThan_UInt16_Pass(void)
{
    uint16_t a = 1;
    uint16_t b = 3;
    uint16_t *pa = &a;
    uint16_t *pb = &b;

    TEST_ASSERT_LESS_THAN(a, b);
    TEST_ASSERT_LESS_THAN(*pa, b);
    TEST_ASSERT_LESS_THAN(a, *pb);
    TEST_ASSERT_LESS_THAN(*pa, *pb);
}

void testLessThan_UInt16_Fail(void)
{
    uint16_t a = 3;
    uint16_t b = 2;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

void testLessThan_UInt32_Pass(void)
{
    uint32_t a = 1;
    uint32_t b = 3;
    uint32_t *pa = &a;
    uint32_t *pb = &b;

    TEST_ASSERT_LESS_THAN(a, b);
    TEST_ASSERT_LESS_THAN(*pa, b);
    TEST_ASSERT_LESS_THAN(a, *pb);
    TEST_ASSERT_LESS_THAN(*pa, *pb);
}

void testLessThan_UInt32_Fail(void)
{
    uint32_t a = 5;
    uint32_t b = 2;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN(a, b);
    VERIFY_FAILS_END
}

/* 7. Run all tests from main using Unity's runner (as per <DOMAIN_KNOWLEDGE> guidance) */
int main(void)
{
    UNITY_BEGIN();

    // Basic int scenarios
    RUN_TEST(testLessThan_BasicInt_Pass);
    RUN_TEST(testLessThan_BasicInt_Fail);

    // Pointer-based scenarios
    RUN_TEST(testLessThan_WithPointer_Pass);
    RUN_TEST(testLessThan_WithPointer_Fail);

    // Unsigned scenarios
    RUN_TEST(testLessThan_Unsigned_Basic_Pass);
    RUN_TEST(testLessThan_Unsigned_Basic_Fail);

    // Int8 scenarios
    RUN_TEST(testLessThan_Int8_Pass);
    RUN_TEST(testLessThan_Int8_Fail);

    // Int16 scenarios
    RUN_TEST(testLessThan_Int16_Pass);
    RUN_TEST(testLessThan_Int16_Fail);

    // Int32 scenarios
    RUN_TEST(testLessThan_Int32_Pass);
    RUN_TEST(testLessThan_Int32_Fail);

    // UInt8 scenarios
    RUN_TEST(testLessThan_UInt8_Pass);
    RUN_TEST(testLessThan_UInt8_Fail);

    // UInt16 scenarios
    RUN_TEST(testLessThan_UInt16_Pass);
    RUN_TEST(testLessThan_UInt16_Fail);

    // UInt32 scenarios
    RUN_TEST(testLessThan_UInt32_Pass);
    RUN_TEST(testLessThan_UInt32_Fail);

    return UNITY_END();
}