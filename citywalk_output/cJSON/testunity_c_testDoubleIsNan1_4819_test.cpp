// File: test_double_is_nan_included.cpp
// Purpose: Test suite for the focal method testDoubleIsNan1 when UNITY_EXCLUDE_DOUBLE is NOT defined.
// This emulates the behavior of Unity's TEST_ASSERT_DOUBLE_IS_NAN on the expression 0.0 / d_zero.

#include <cmath>
#include <unity.h>
#include <cassert>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


//
// Replicate the static constants from the focal class dependency.
// In the Unity-based test, d_zero is defined as 0.0 (double).
//
static double d_zero = 0.0; // UNITY_DOUBLE replacement in this test harness

// Test: testDoubleIsNan1_Included
// - Expected: 0.0 / d_zero yields NaN.
// - Validation: std::isnan(value) must be true.
// - Additional check: NaN should not compare equal to itself.
void testDoubleIsNan1_Included()
{
    // Compute NaN via division by zero as in the focal test
    double value = 0.0 / d_zero;

    // Primary assertion: value must be NaN
    if (!std::isnan(value))
    {
        std::cerr << "testDoubleIsNan1_Included: expected NaN, got " << value << std::endl;
        // Use abort to indicate test failure in a simple harness
        std::abort();
    }

    // Secondary check: NaN is not equal to itself
    if (value == value)
    {
        std::cerr << "testDoubleIsNan1_Included: NaN should not compare equal to itself" << std::endl;
        std::abort();
    }

    // If we reach here, the NaN behavior is as expected.
    // In a Unity-based test, this would be a passing TEST_ASSERT_DOUBLE_IS_NAN.
}

int main()
{
    // Run the included-branch test
    testDoubleIsNan1_Included();
    // If all checks pass, exit with success
    return 0;
}


// File: test_double_is_nan_excluded.cpp
// Purpose: Test suite for the focal method testDoubleIsNan1 when UNITY_EXCLUDE_DOUBLE is defined.
// This simulates the "ignore" path of the focal test.


int main()
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Simulate Unity's TEST_IGNORE() by reporting the ignore and exiting gracefully.
    std::cout << "testDoubleIsNan1 is ignored due to UNITY_EXCLUDE_DOUBLE" << std::endl;
    return 0; // success: test was ignored as intended
#else
    // If UNITY_EXCLUDE_DOUBLE is not defined, this file should not exercise the test path.
    // Return success to avoid false negatives in environments that compile this file anyway.
    return 0;
#endif
}