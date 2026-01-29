// Step 1: Program Understanding and Candidate Keywords
// - Focal method: testEqualFloatArraysInf in testunity.c
// - Core dependencies and concepts:
//   - UNITY_FLOAT / f_zero and INF handling (IEEE754 infinity via 1.0f / f_zero)
//   - Unity testing macros: TEST_ASSERT_EQUAL_FLOAT_ARRAY
//   - Conditional compilation: UNITY_EXCLUDE_FLOAT guard
//   - Unity test harness: UNITY_BEGIN / RUN_TEST / UNITY_END
// - Domain context: test suite targets floating-point array equality including infinities

// Step 2: Unit Test Generation (C++11 test suite for the focal method)
// This test suite uses the Unity C framework from testunity.c to exercise
// the focal function testEqualFloatArraysInf. It also adds a second test that
// demonstrates an explicit INF-based equality using a standalone test function
// (to broaden coverage around INF handling in Unity's float assertions).

#include <cmath>
#include <limits>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


extern "C" {
}

// Forward declaration of the focal test function defined in testunity.c
// Ensures the Unity test harness can invoke it as a test case.
extern void testEqualFloatArraysInf(void);

// Additional test to directly exercise an explicit float INF comparison
// using Unity's assertion macros, independent of the focal function.
// Implemented in C++ to leverage std::numeric_limits for INF generation.
extern "C" void testExplicitInfFloatArraysPass(void);

int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the focal method test that checks equality of two arrays containing +INF
    // This exercises the true-branch where both arrays are identical including INF entries.
    RUN_TEST(testEqualFloatArraysInf, __LINE__);

    // Run an additional test that builds two arrays containing +INF using
    // std::numeric_limits<float>::infinity() to demonstrate explicit INF handling
    // with Unity's TEST_ASSERT_EQUAL_FLOAT_ARRAY macro.
    RUN_TEST(testExplicitInfFloatArraysPass, __LINE__);

    // Finish Unity test run
    return UNITY_END();
}

// Test: Explicitly compare two equal arrays that contain +INF using Unity's macro.
// Purpose: Validate Unity's float array equality path when INF values are present.
// Rationale: Ensures correct behavior beyond the static arrays in the focal test.
extern "C" void testExplicitInfFloatArraysPass(void)
{
    // Create two arrays with an INF element produced by IEEE754 infinity
    const float a[] = {1.0f, std::numeric_limits<float>::infinity(), 25.4f, 0.253f};
    const float b[] = {1.0f, std::numeric_limits<float>::infinity(), 25.4f, 0.253f};

    // This assertion should pass, since both arrays are identical including INF positions
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(a, b, 4);
}

// Note: The focal function testEqualFloatArraysInf defined in testunity.c remains
// the primary subject under test. It uses 1.0f / f_zero to generate +INF in the
// second element, and asserts equality across the 4 elements. The two tests
// above together provide coverage for both the focal path and explicit INF usage
// via Unity's float equality macros.