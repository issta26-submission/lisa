// test_equal_int32s_suite.cpp
// A focused test suite for the focal method testEqualInt32s from testunity.c
// This suite targets the core behavior exercised by UNITY_INT32 comparisons,
// including literal vs literal, variable vs variable, literal/variable vs pointers,
// and negative values. It uses the Unity framework (no GTest) and is compatible
// with a C++11 build environment.
// 
// Candidate Keywords derived from the focal method:
// - UNITY_INT32, v0, v1, p0, p1, literals, hex literals, dereferenced pointers
// - TEST_ASSERT_EQUAL_INT32, pointer dereference, value equality, hex equality
// - memory indirections via *p0, *p1, and comparisons with constants

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

// Forward declarations for test functions to be used by the Unity runner
#ifdef __cplusplus
extern "C" {
#endif

void test_equal_int32s_basic(void);
void test_equal_int32s_with_hex_and_variables(void);
void test_equal_int32s_with_negative_values(void);
void test_equal_int32s_with_pointers_and_literals(void);

#ifdef __cplusplus
}
#endif

// Implementations
#ifdef __cplusplus
extern "C" {
#endif

// Test 1: Basic equalities using literals, variables, and pointers
// This mirrors the structure seen in testEqualInt32s where v0 and v1 are assigned
// the same value and all variants of equality are exercised.
void test_equal_int32s_basic(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = 0x78760000;
    v1 = 0x78760000;

    p0 = &v0;
    p1 = &v1;

    // literal vs literal
    TEST_ASSERT_EQUAL_INT32(0x78760000, 0x78760000);

    // variable vs variable
    TEST_ASSERT_EQUAL_INT32(v0, v1);

    // literal vs variable
    TEST_ASSERT_EQUAL_INT32(0x78760000, v1);

    // variable vs literal
    TEST_ASSERT_EQUAL_INT32(v0, 0x78760000);

    // pointer dereference vs variable
    TEST_ASSERT_EQUAL_INT32(*p0, v1);

    // pointer vs pointer
    TEST_ASSERT_EQUAL_INT32(*p0, *p1);

    // dereferenced pointer vs literal
    TEST_ASSERT_EQUAL_INT32(*p0, 0x78760000);
}

// Test 2: Equalities using hex literals and mixed references
// Ensures coverage of hex-literal based comparisons and cross-checks with
// variables and pointers.
void test_equal_int32s_with_hex_and_variables(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = -123456789;          // negative value
    v1 = -123456789;          // same value

    p0 = &v0;
    p1 = &v1;

    // literal hex vs hex literal
    TEST_ASSERT_EQUAL_INT32(-123456789, -123456789);

    // variable vs variable
    TEST_ASSERT_EQUAL_INT32(v0, v1);

    // literal vs variable
    TEST_ASSERT_EQUAL_INT32(-123456789, v1);

    // variable vs literal
    TEST_ASSERT_EQUAL_INT32(v0, -123456789);

    // pointer dereference checks
    TEST_ASSERT_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_EQUAL_INT32(*p0, -123456789);
}

// Test 3: Equalities with negative values and boundary-like values
// This expands coverage to include negative numbers and ensures proper handling
// of signed 32-bit range semantics.
void test_equal_int32s_with_negative_values(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = INT32_MIN + 123; // some negative value near min
    v1 = INT32_MIN + 123; // identical

    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT32(-2147481525, -2147481525);
    TEST_ASSERT_EQUAL_INT32(v0, v1);
    TEST_ASSERT_EQUAL_INT32(-2147481525, v1);
    TEST_ASSERT_EQUAL_INT32(v0, -2147481525);
    TEST_ASSERT_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_EQUAL_INT32(*p0, -2147481525);
}

// Test 4: Equalities with pointers and literals to check dereferencing behavior
// Reiterates pointer-based equality ensuring *p0 and *p1 are treated as 32-bit ints.
void test_equal_int32s_with_pointers_and_literals(void)
{
    UNITY_INT32 v0, v1;
    UNITY_INT32 *p0, *p1;

    v0 = 0x12345678;
    v1 = 0x12345678;

    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_EQUAL_INT32(0x12345678, 0x12345678);
    TEST_ASSERT_EQUAL_INT32(v0, v1);
    TEST_ASSERT_EQUAL_INT32(0x12345678, v1);
    TEST_ASSERT_EQUAL_INT32(v0, 0x12345678);
    TEST_ASSERT_EQUAL_INT32(*p0, v1);
    TEST_ASSERT_EQUAL_INT32(*p0, *p1);
    TEST_ASSERT_EQUAL_INT32(*p0, 0x12345678);
}

#ifdef __cplusplus
}
#endif

// Main runner for this dedicated test suite
// This allows compiling this file as a C++11 translation unit and running
// the subset of tests focused on testEqualInt32s behavior.
#ifdef __cplusplus
int main(void)
{
    UNITY_BEGIN();
    // Run tests that cover core dependent components of testEqualInt32s
    RUN_TEST(test_equal_int32s_basic);
    RUN_TEST(test_equal_int32s_with_hex_and_variables);
    RUN_TEST(test_equal_int32s_with_negative_values);
    RUN_TEST(test_equal_int32s_with_pointers_and_literals);
    return UNITY_END();
}
#else
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_equal_int32s_basic);
    RUN_TEST(test_equal_int32s_with_hex_and_variables);
    RUN_TEST(test_equal_int32s_with_negative_values);
    RUN_TEST(test_equal_int32s_with_pointers_and_literals);
    return UNITY_END();
}
#endif