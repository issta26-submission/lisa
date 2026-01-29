/************** Candidate Keywords and Program Understanding **************/
//
// Step 1: From the focal method testLessThanUINT16, the core dependent components are:
// - UNITY_UINT16 type (primary data type under test)
// - TEST_ASSERT_LESS_THAN_UINT16 macro (the function under test logic)
// - Pointer handling: using UNITY_UINT16* p0, p1 and dereferencing (*p0, *p1)
// - Basic test structure: direct values and pointer-based values
// - Test harness helpers: RUN_TEST, UNITY_BEGIN/END in Unity framework
// - Test control for failures: EXPECT_ABORT_BEGIN / VERIFY_FAILS_END semantics (non-terminating failures)
// - Boundary values for UINT16: 0, 65535 (UINT16 max)
// - Namespacing: C-style macros used in a C/C++ project (no access to private internals)
// - Static/dynamic data handling: using local stack variables for values and pointers
//
// Step 2 targets: reproduce and extend coverage for testLessThanUINT16 by adding
// variations that exercise both direct values and pointer-based values, including
// passing cases and failing cases (equal and reversed orders). The tests below
// are designed to compile in C++11 without GTest, using the Unity framework
// conventions as shown in the focal class dependency block.
--------------------------------------------------------------------------


// test_less_than_uint16_suite.cpp
// A small, self-contained Unity-based test suite focusing on testLessThanUINT16
// by exercising true/false branches, direct values, and pointer dereferencing.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// If the project uses the Unity test runner provided by C sources, include paths
// should be configured so that UNITY_PROTECT macros and RUN_TEST are available.
// The tests below follow the same style as the existing Unity-based tests.

#ifdef __cplusplus
}
#endif

// 1) Direct values: should pass when first value < second value
void testLessThanUINT16_Direct_Pass_SmallAndBig(void)
{
    // Arrange
    UNITY_UINT16 a = 0;
    UNITY_UINT16 b = 65535;

    // Act & Assert: a < b should pass
    TEST_ASSERT_LESS_THAN_UINT16(a, b);
}

// 2) Direct values: another positive case (middle-range)
void testLessThanUINT16_Direct_Pass_Mid(void)
{
    // Arrange
    UNITY_UINT16 a = 100;
    UNITY_UINT16 b = 1000;

    // Act & Assert: a < b should pass
    TEST_ASSERT_LESS_THAN_UINT16(a, b);
}

// 3) Pointer-based values: both pointers are valid and dereferenced values satisfy a < b
void testLessThanUINT16_Pointer_Pass(void)
{
    // Arrange
    UNITY_UINT16 x = 2;
    UNITY_UINT16 y = 1000;
    UNITY_UINT16 *px = &x;
    UNITY_UINT16 *py = &y;

    // Act & Assert: *px < *py should pass
    TEST_ASSERT_LESS_THAN_UINT16(*px, *py);
}

// 4) Negative test: equal values should fail (not less-than)
void testLessThanUINT16_Direct_Fail_Equal(void)
{
    // Arrange
    UNITY_UINT16 a = 5;
    UNITY_UINT16 b = 5;

    // Act & Assert: expect failure since a is not less than b
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT16(a, b);
    VERIFY_FAILS_END
}

// 5) Negative test: pointer-based equal values should fail
void testLessThanUINT16_Pointer_Fail_Equal(void)
{
    // Arrange
    UNITY_UINT16 x = 7;
    UNITY_UINT16 y = 7;
    UNITY_UINT16 *px = &x;
    UNITY_UINT16 *py = &y;

    // Act & Assert: expect failure since *px == *py
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT16(*px, *py);
    VERIFY_FAILS_END
}

// 6) Negative test: reversed order with large/small values should fail
void testLessThanUINT16_Direct_Fail_LargeSmall(void)
{
    // Arrange
    UNITY_UINT16 a = 65535;
    UNITY_UINT16 b = 0;

    // Act & Assert: a is not less than b; should fail
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_UINT16(a, b);
    VERIFY_FAILS_END
}


// Main entry point to run the focused test suite
int main(void)
{
    UNITY_BEGIN();

    // Register tests related to testLessThanUINT16
    RUN_TEST(testLessThanUINT16_Direct_Pass_SmallAndBig);
    RUN_TEST(testLessThanUINT16_Direct_Pass_Mid);
    RUN_TEST(testLessThanUINT16_Pointer_Pass);
    RUN_TEST(testLessThanUINT16_Direct_Fail_Equal);
    RUN_TEST(testLessThanUINT16_Pointer_Fail_Equal);
    RUN_TEST(testLessThanUINT16_Direct_Fail_LargeSmall);

    return UNITY_END();
}