// Test suite for the focal method: testDoubleInfIsNotNan
// This suite leverages the existing Unity-based test in testunity.c.
// It includes an additional direct IEEE-754 based check to improve coverage.
// Notes:
// - The focal method's behavior depends on the compile-time flag UNITY_EXCLUDE_DOUBLE.
//   If UNITY_EXCLUDE_DOUBLE is defined, the test should be ignored by TEST_IGNORE().
//   If not defined, the test asserts that 1.0 / d_zero (0.0) results in NaN, which it should not.
// - To exercise both branches, build/test once with -DUNITY_EXCLUDE_DOUBLE and once without it.

#include <cmath>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal method from testunity.c (C linkage)
void testDoubleInfIsNotNan(void);
#ifdef __cplusplus
}
#endif

// Additional independent test: verify that 1.0 / 0.0 yields a value that is not NaN.
// This uses IEEE-754 semantics (Infinity is produced for division by zero).
void testDoubleInfIsNotNan_Direct_IsNotNaN(void)
{
    // Compute 1.0 / 0.0 directly.
    double val = 1.0 / 0.0;
    // Ensure the result is NOT NaN. If the platform uses IEEE 754, this should be Inf, not NaN.
    TEST_ASSERT_FALSE(std::isnan(val));
    // We avoid asserting isinf here to maintain portability across potential non-IEEE platforms.
    // The key property tested: division by zero does not yield NaN in IEEE 754 environments.
}

// Note: In order to cover the compile-time branches (UNITY_EXCLUDE_DOUBLE defined vs not defined),
// you would typically need to compile and run this test in two separate builds:
// 1) g++ -DUNITY_EXCLUDE_DOUBLE ... to exercise the ignore path inside testDoubleInfIsNotNan
// 2) without -DUNITY_EXCLUDE_DOUBLE to exercise the abort path inside testDoubleInfIsNotNan

int main(void)
{
    UNITY_BEGIN();

    // Run the focal method as defined in testunity.c.
    // This will exercise either the ignore path or the abort path depending on compile flags.
    RUN_TEST(testDoubleInfIsNotNan);

    // Run an independent direct-IEEE-754-based check to complement coverage.
    RUN_TEST(testDoubleInfIsNotNan_Direct_IsNotNaN);

    return UNITY_END();
}