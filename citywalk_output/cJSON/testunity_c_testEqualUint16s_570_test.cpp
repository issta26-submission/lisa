// test_equal_uint16s_suite.cpp
// A small C++11 test harness that runs the focal Unity test
// function: testEqualUint16s from testunity.c using the Unity framework.
// This file treats the focal test as a test case and executes it
// under a C++11 build without GTest.

// Domain notes:
// - This harness imports Unity via unity.h and calls the focal test
//   function testEqualUint16s(void) as an external C function.
// - Additional non-focal test (testEqualUint16sVariant1) is provided to
//   increase coverage by re-implementing the same equality checks in a
//   separate test function, ensuring true-path coverage of the equality
//   assertions used inside the focal test.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal test function from testunity.c
// It uses Unity's TEST_ASSERT_EQUAL_UINT16 and related macros to validate
// various ways of comparing UNITY_UINT16 values.
void testEqualUint16s(void);
#ifdef __cplusplus
}
#endif

// Optional: provide an additional test that mirrors the focal test's checks
// to broaden coverage of equality comparisons for 16-bit unsigned integers.
void testEqualUint16sVariant1(void)
{
    // This test duplicates the focal test's equality checks but via a separate
    // test function to exercise the same assertion macros in a distinct
    // test case.
    // Local variables emulate the focal test's scenario.
    unsigned short v0 = 0x9876;
    unsigned short v1 = 0x9876;
    unsigned short *p0 = &v0;
    unsigned short *p1 = &v1;

    // Direct literal vs literal comparison
    TEST_ASSERT_EQUAL_UINT16(0x9876, 0x9876);

    // Equality between two separate variables with identical values
    TEST_ASSERT_EQUAL_UINT16(v0, v1);

    // Equality between a literal and a variable
    TEST_ASSERT_EQUAL_UINT16(0x9876, v1);
    TEST_ASSERT_EQUAL_UINT16(v0, 0x9876);

    // Equality using dereferenced pointers
    TEST_ASSERT_EQUAL_UINT16(*p0, v1);
    TEST_ASSERT_EQUAL_UINT16(*p0, *p1);
    TEST_ASSERT_EQUAL_UINT16(*p0, 0x9876);
}

// Entry point for running the test suite.
// We invoke the focal test (testEqualUint16s) as well as the additional
// variant test to maximize coverage of the relevant equality macros.
int main(void)
{
    UNITY_BEGIN();

    // Run the focal test under testEqualUint16s
    // This exercises the exact code path under test in the provided focal method.
    RUN_TEST(testEqualUint16s);

    // Run an additional variant test to improve coverage of the equality
    // assertions for 16-bit unsigned integers.
    RUN_TEST(testEqualUint16sVariant1);

    return UNITY_END();
}