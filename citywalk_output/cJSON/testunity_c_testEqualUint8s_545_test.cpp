// Test runner for the focal Unity test function: testEqualUint8s
// This wrapper is implemented in C++11 (compilable under gnu++11) and uses the Unity test framework
// to execute the focal test from testunity.c without introducing GTest or other frameworks.
//
// Step 1 (Conceptual): The focal method testEqualUint8s exercises a sequence of equality checks
// on 8-bit unsigned integers, including direct values, pointers, and constants.
// Key elements observed (Candidate Keywords):
// - UNITY_UINT8, v0, v1, p0, p1
// - 0x22 (and 0x22 literal usage)
// - TEST_ASSERT_EQUAL_UINT8, TEST_ASSERT_EQUAL_UINT8(*p0, *p1), etc.
// - Dereferencing pointers: *p0, *p1
// - Address/value equivalence via TEST_ASSERT_EQUAL_UINT8(v0, v1) and TEST_ASSERT_EQUAL_UINT8(*p0, v1)
// - The function validates both value equality and pointer-based equality

#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Ensure C-linkage for the focal test function when included from C++
// The focal test function is defined in testunity.c (C file)
#ifdef __cplusplus
extern "C" {
#endif
void testEqualUint8s(void);
#ifdef __cplusplus
}
#endif

// Simple harness using Unity to run only the focal test function.
// This enables high-coverage, non-GTest-based testing in a C++11 project.
int main(void)
{
    // Initialize the Unity test framework
    UNITY_BEGIN();

    // Run the focal test function testEqualUint8s from testunity.c
    // This exercises the exact code path of the focal method, ensuring its behavior remains correct.
    RUN_TEST(testEqualUint8s);

    // Finalize and return Unity's test results
    return UNITY_END();
}