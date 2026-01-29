// test_suite.cpp
// A C++11 compatible test harness for the focal Unity-based testNotEqualFloatArrays1
// This harness uses the Unity framework (C) and calls into the focal test
// function testNotEqualFloatArrays1 as declared in testunity.c.
// It also exercises additional not-equal float array scenarios to improve coverage.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Declarations of focal tests from testunity.c
void testNotEqualFloatArrays1(void);
void testNotEqualFloatArrays2(void);
void testNotEqualFloatArrays3(void);

#ifdef __cplusplus
}
#endif

// Additional direct test to independently verify not-equal float array behavior
// This test mirrors the pattern used in the focal test when floats are enabled.
// It ensures that two nearby-but-different float arrays are reported as not equal
// by Unity's TEST_ASSERT_EQUAL_FLOAT_ARRAY macro.
extern "C" void testDirectFloatNotEqual(void)
{
#ifndef UNITY_EXCLUDE_FLOAT
    // Two arrays that differ in the last element, mirroring the focal test intent
    float a[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float b[] = {1.0f, 2.0f, 3.0f, 4.1f};
    // We expect the assertion to fail and abort, so we wrap with EXPECT_ABORT_BEGIN/END
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(a, b, 4);
    VERIFY_FAILS_END
#else
    // If float support is excluded, mark as ignored to preserve coverage in that configuration
    TEST_IGNORE_MESSAGE("FLOAT support excluded via UNITY_EXCLUDE_FLOAT");
#endif
}

// Note: The focal function testNotEqualFloatArrays1 is defined in testunity.c and
// contains its own EXPECT_ABORT_BEGIN/VERIFY_FAILS_END guards. This wrapper
// ensures we have an additional independent test path for coverage.

int main(void)
{
    // Initialize Unity test framework
    UnityBegin("NotEqualFloatArrays1 - Focal Method Suite (C Unity tests in C++ harness)");

    // Run focal tests (as declared with C linkage)
    RUN_TEST(testNotEqualFloatArrays1);
    RUN_TEST(testNotEqualFloatArrays2);
    RUN_TEST(testNotEqualFloatArrays3);

    // Run additional direct test to validate independent not-equality detection
    RUN_TEST(testDirectFloatNotEqual);

    // Finalize Unity and return
    return UnityEnd();
}