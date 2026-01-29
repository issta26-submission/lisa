// FOCAL Keywords and test focus (Step 1)
//
// Candidate Keywords representing the focal method's core components:
// - UNITY_EXCLUDE_DOUBLE: conditional compilation flag that excludes double support
// - UNITY_DOUBLE: typedef for double used by Unity
// - d_zero: std-like constant representing zero for doubles in the focal test
// - 0.0 / d_zero: the operation that yields NaN in the NaN scenario
// - TEST_ASSERT_EQUAL_DOUBLE: Unity assertion for double equality
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: Unity macros to capture expected aborts
// - TEST_IGNORE / TEST_IGNORE_MESSAGE: Unity mechanism to ignore a test
//
// Note: The real focal test resides in testunity.c as testDoublesNotEqualExpectedNaN.
// Here we build a targeted C++11 test to exercise the same behavior in a separate
// unit-test file, respecting the project's constraint of not using GTest.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Expose a local d_zero to mirror the FOCAL_CLASS_DEP dependency (static/dedicated per test file).
#ifndef UNITY_EXCLUDE_DOUBLE
static const UNITY_DOUBLE d_zero = 0.0;
#endif

#ifdef __cplusplus
}
#endif

/*
   Test suite focused on the focal method: testDoublesNotEqualExpectedNaN
   Behavior:
   - When doubles are supported (UNITY_EXCLUDE_DOUBLE not defined), the test
     asserts that 0.0 / d_zero results in NaN, and comparing that NaN to a
     finite constant should fail (abort as per Unity's test macro).
   - When doubles are excluded (UNITY_EXCLUDE_DOUBLE defined), the focal test
     path is compiled to TEST_IGNORE (no abort expected).
*/

// Normal path: doubles are included, and the assertion should fail (abort as expected).
#ifndef UNITY_EXCLUDE_DOUBLE
void testDoublesNotEqualExpectedNaN_NormalPath(void)
{
    // Explain: This mirrors the focal test's non-excluded path.
    // We expect the comparison TEST_ASSERT_EQUAL_DOUBLE( NaN, 85.963 ) to fail.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE(0.0 / d_zero, 85.963);
    VERIFY_FAILS_END
}
#else
// When doubles are excluded, this test is not meaningful; provide a placeholder.
void testDoublesNotEqualExpectedNaN_NormalPath(void)
{
    // Intentionally left empty when doubles are excluded.
}
#endif

// Excluded path: compile-time branch when UNITY_EXCLUDE_DOUBLE is defined.
// This ensures we still exercise the code-path that ignores the test in the
// absence of double support (via TEST_IGNORE()).
#ifdef UNITY_EXCLUDE_DOUBLE
void testDoublesNotEqualExpectedNaN_ExcludedPath(void)
{
    // The focal behavior in this path is to ignore the test.
    TEST_IGNORE();
}
#else
void testDoublesNotEqualExpectedNaN_ExcludedPath(void)
{
    // Placeholder to keep function present if needed; not used when doubles are present.
}
#endif

// Main runner: register and execute tests
int main(void)
{
    UNITY_BEGIN();

#ifndef UNITY_EXCLUDE_DOUBLE
    RUN_TEST(testDoublesNotEqualExpectedNaN_NormalPath);
#else
    // When UNITY_EXCLUDE_DOUBLE is defined, still run a test that explicitly ignores.
    RUN_TEST(testDoublesNotEqualExpectedNaN_ExcludedPath);
#endif

    // In the excluded-double branch, we also provide a separate test entry to
    // exercise the ignore-path in environments that compile both branches.
#ifdef UNITY_EXCLUDE_DOUBLE
    RUN_TEST(testDoublesNotEqualExpectedNaN_ExcludedPath);
#endif

    return UNITY_END();
}

/*
Explanation of test layout (Step 3 refinement):
- The NormalPath test (when doubles are enabled) directly mirrors the focal logic:
  division by zero leads to NaN, and the double-equality assertion is expected to abort.
  This ensures coverage of the true code path and its abort-on-false condition.

- The ExcludedPath test (when doubles are disabled) exercises the compile-time
  branch via TEST_IGNORE() to verify that the test is safely ignored and that
  the test suite continues to execute other tests.

- Static/dedicated test variable d_zero is declared in this test file to reflect the
  focal dependency without requiring access to the original file's static symbol.

- The tests are written to be compilable under C++11 (as requested) and do not rely
  on GTest; they rely on Unity's C-style testing framework, which is compatible
  with C++ builds when included in a C++ source file.

- All tests include explanatory comments detailing intent, per-domain guidance.
*/