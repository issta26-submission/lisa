// File: test_double_determination_suite.cpp
// This test suite targets the core concept illustrated by testDoubleIsNotDeterminate2 from the focal file.
// It exercises the determinacy check for double values, focusing on finite vs non-finite results.
// Note: This test is written in C++11 and uses a minimal, self-contained harness (no GoogleTest or GMock).
// It mirrors the intent of the Unity-based test by validating the behavior around finite, infinite and NaN values.

#include <cmath>
#include <unity.h>
#include <cassert>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (from Step 1): UNITY_EXCLUDE_DOUBLE, d_zero, TEST_ASSERT_DOUBLE_IS_DETERMINATE,
// isfinite/isfinite-like checks, infinite/double division by zero, NaN handling, determinacy concept.

// Helper: mimic the focal macro's intent by providing a small determinacy gate.
// In the Unity-based test, "determinacy" would be true for finite numbers and false for Inf/NaN.
static inline bool isDoubleDeterminate(double d)
{
    // std::isfinite returns true for finite values, false for +/-Inf and NaN.
    return std::isfinite(d);
}

// Test 1: Finite doubles should be considered determinate.
void test_finite_value_is_determinate(void)
{
    // Arranges a finite value
    double v = 3.14159;
    // Act + Assert
    assert(isDoubleDeterminate(v) == true);
    // If assertion passes, test passes silent; otherwise program aborts.
}

// Test 2: -1.0 / 0.0 yields -Inf in IEEE-754; should be not determinate.
void test_negative_infinity_is_not_determinate(void)
{
    // Arrange: division by zero to produce infinity (negative infinity here due to -1.0)
    double zero = 0.0;
    double d_zero = zero; // alias for clarity with the focal code naming
    double val = -1.0 / d_zero;
    // Act & Assert
    assert(isDoubleDeterminate(val) == false);
    // If assertion passes, test passes; otherwise aborts.
}

// Test 3: NaN should be not determinate.
void test_nan_is_not_determinate(void)
{
    // Arrange: NaN generation
    double nanVal = std::nan("");

    // Act & Assert
    assert(isDoubleDeterminate(nanVal) == false);
}

// Test runner
int main(void)
{
    // Run tests
    test_finite_value_is_determinate();
    test_negative_infinity_is_not_determinate();
    test_nan_is_not_determinate();

    std::cout << "All determinacy tests passed." << std::endl;
    return 0;
}
//
// Explanation:
// - This suite validates the core dependent behavior: finite -> determinate, Inf/NaN -> not determinate.
// - It intentionally mirrors the intent of testDoubleIsNotDeterminate2 but uses a self-contained C++11 harness
//   to maximize portability without requiring the Unity framework at build time.
// - It covers true and false branches of the determinacy predicate for double values.
//
// Notes for integration with the focal Unity-based project (optional):
// - If you compile and link this alongside the Unity-based test suite, you can guard the Inf/NaN tests
//   behind a macro (e.g., UNITY_EXCLUDE_DOUBLE) to mirror the original branch behavior.
// - To align with Step 3's guidance, static members, macro branches, and domain-relevant types
//   are treated with standard C++ facilities (std::isfinite, std::nan).
//

// File: test_double_determination_suite_with_exclusion.cpp
// This variant demonstrates how the same determinacy tests could be compiled under UNITY_EXCLUDE_DOUBLE,
// which, in the Unity framework, would cause the test under testDoubleIsNotDeterminate2 to be ignored.
// Here we emulate the same gating in a portable C++11 manner.


// Helper: same as in the previous file
static inline bool isDoubleDeterminate(double d)
{
    return std::isfinite(d);
}

// If UNITY_EXCLUDE_DOUBLE is defined, we skip the "infinite" test to mimic TEST_IGNORE behavior.
// Otherwise, we perform the full determinacy checks.
#ifdef UNITY_EXCLUDE_DOUBLE
void test_infinite_value_is_not_determinate_excluded(void)
{
    // Emulate test ignore by reporting a skipped test.
    std::cout << "[SKIPPED] test_infinite_value_is_not_determinate_excluded (UNITY_EXCLUDE_DOUBLE defined)" << std::endl;
}
#else
void test_infinite_value_is_not_determinate_excluded(void)
{
    // This test actually executes the infinite value determinacy check when doubles are not excluded.
    double val = -1.0 / 0.0; // -Inf
    assert(isDoubleDeterminate(val) == false);
}
#endif

// Finite value test (unaffected by UNITY_EXCLUDE_DOUBLE)
void test_finite_value_is_determinate_excluded(void)
{
    double v = 2.71828;
    // If UNITY_EXCLUDE_DOUBLE is defined, this test still validates the finite path.
    // It should remain determinate.
    assert(isDoubleDeterminate(v) == true);
}

// NaN test (unaffected by UNITY_EXCLUDE_DOUBLE)
void test_nan_is_not_determinate_excluded(void)
{
    double nanVal = std::nan("");

    assert(isDoubleDeterminate(nanVal) == false);
}

// Test runner
int main(void)
{
    test_finite_value_is_determinate_excluded();
#ifndef UNITY_EXCLUDE_DOUBLE
    test_infinite_value_is_not_determinate_excluded();
#else
    // When excluded, still report the skip message
    test_infinite_value_is_not_determinate_excluded();
#endif
    test_nan_is_not_determinate_excluded();

    std::cout << "Determinacy-exclusion suite finished." << std::endl;
    return 0;
}