/*
  This file provides a minimal C++11-based test harness that targets the focal
  behavior of the Unity test wrapper shown in testunity.c for the function
  testDoubleIsPosInf1.

  Step 1: Candidate Keywords extracted from the focal method
  - UNITY_EXCLUDE_DOUBLE
  - TEST_ASSERT_DOUBLE_IS_INF
  - d_zero (zero double, as defined in the focal file)
  - 2.0 (floating point literal used in the test)
  - Inf / +Inf (positive infinity)
  - isinf (IEEE754 infinity check)
  - d_zero division (2.0 / d_zero)
  - Test harness macros (or their semantic equivalent)

  Step 2/3: A lightweight, non-GTest C++11 test that exercises the focal logic
  - If UNITY_EXCLUDE_DOUBLE is defined, the test is treated as ignored/skipped.
  - Otherwise, it computes 2.0 / d_zero and asserts the result is +INF.
  - A secondary test asserts that std::numeric_limits<double>::infinity() is +INF,
    serving as a basic sanity check for the INF representation.

  Note: This is a self-contained test harness suitable for C++11 compilation.
  It does not depend on GTest and uses standard library facilities only.
*/

#include <cmath>
#include <unity.h>
#include <limits>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>


// Candidate constants mimicking the Unity environment in the focal test
static const double d_zero = 0.0;

/*
  Test: testDoubleIsPosInf1
  Follows the focal logic:
  - If UNITY_EXCLUDE_DOUBLE is defined, the test is ignored.
  - Otherwise, verifies that (2.0 / d_zero) yields positive Infinity.
*/
void testDoubleIsPosInf1(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Mimic TEST_IGNORE: the test should be skipped when double support is excluded
    std::cout << "testDoubleIsPosInf1: IGNORED (UNITY_EXCLUDE_DOUBLE defined)" << std::endl;
#else
    // Core test: 2.0 / d_zero should produce +INF under IEEE 754
    double val = 2.0 / d_zero;

    // Use isinf to check infinity, and sign to ensure positive infinity
    if (!(std::isinf(val) && val > 0.0))
    {
        std::cerr << "testDoubleIsPosInf1: FAIL - expected +Inf, got " << val << std::endl;
        std::exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "testDoubleIsPosInf1: PASS" << std::endl;
    }
#endif
}

/*
  Additional sanity test:
  - Verifies that the standard library represents positive infinity correctly
  using a well-known source (numeric_limits).
  This ensures INF semantics are as expected in the environment.
*/
void testPosInfLiteral(void)
{
    double infFromLiteral = std::numeric_limits<double>::infinity();
    if (!(std::isinf(infFromLiteral) && infFromLiteral > 0.0))
    {
        std::cerr << "testPosInfLiteral: FAIL - numeric_limits_INFINITY not positive INF as expected, got "
                  << infFromLiteral << std::endl;
        std::exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "testPosInfLiteral: PASS" << std::endl;
    }
}

int main(void)
{
    // Run the focal test variant
    testDoubleIsPosInf1();

    // Run an additional INF sanity check
    testPosInfLiteral();

    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
}