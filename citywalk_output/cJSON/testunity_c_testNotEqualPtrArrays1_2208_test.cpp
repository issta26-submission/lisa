// Test Suite: Focused wrapper for the focal Unity test function
// Goal: Execute testNotEqualPtrArrays1 from testunity.c within a C++11 environment
// without using GTest, following the provided guidelines.
// - This wrapper uses Unity's C API (unity.h) to run the focal test.
// - The focal test verifies a negative (abort) case for pointer array equality.
// - We expose the focal function with C linkage to avoid name mangling in C++.
// - The test suite ensures executable, self-contained test execution from main.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal Unity test function located in testunity.c
// It uses Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to validate expected failure.
void testNotEqualPtrArrays1(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Initialize Unity test framework
    UnityBegin("testNotEqualPtrArrays1_suite.cpp");

    // Run the focal test. This test intentionally triggers a failure (abort)
    // and Unity will verify that the failure occurred as expected.
    RUN_TEST(testNotEqualPtrArrays1);

    // Finalize Unity test run and return status
    return UnityEnd();
}