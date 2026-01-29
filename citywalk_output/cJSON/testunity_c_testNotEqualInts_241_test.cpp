// Test suite for the focal method testNotEqualInts in testunity.c
// This suite introduces additional checks to exercise the failure-path
// of the NOT EQUAL INT assertion pattern used by the focal method.
// It adheres to a Unity-based testing approach without GTest.
// The tests are written for C++11 compatibility.

#include <cstddef>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

// Include Unity framework (C header)

// Prototypes for functions used by Unity's abort-expect macros defined in
// testunity.c. They are implemented in the Unity test harness and linked
// with this test suite at build time.
void startPutcharSpy(void);
void endPutcharSpy(void);
char* getBufferPutcharSpy(void);

// The focal method under test (as defined in testunity.c) is testNotEqualInts.
// We will not call it directly (it contains the aborting pattern). Instead,
// we replicate the critical failure-pattern in additional, separate tests
// to verify the abort semantics of the TEST_ASSERT_EQUAL_INT macro.

// Forward declarations of the new test cases.
void testNotEqualInts_WrapsCorrectly(void);
void testNotEqualInts_WrapsWithDifferentValues(void);

#ifdef __cplusplus
}
#endif

// Test 1: Replicate focal's failure path using the same NOT EQUAL INT assertion.
// This confirms that TEST_ASSERT_EQUAL_INT with unequal values triggers an abort
// as expected by the focal method's pattern.
void testNotEqualInts_WrapsCorrectly(void)
{
    // Candidate Keywords:
    // - TEST_ASSERT_EQUAL_INT
    // - EXPECT_ABORT_BEGIN
    // - VERIFY_FAILS_END
    // - startPutcharSpy / endPutcharSpy (IO capture helpers)
    //
    // Domain rationale:
    // - Ensure that the focal assertion (3982 vs 3983) indeed causes a failure.
    // - This mirrors the behavior tested by testNotEqualInts in testunity.c.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(3982, 3983);
    VERIFY_FAILS_END
}

// Test 2: Additional variation to validate the abort mechanism with another pair
// of unequal integers. This strengthens coverage of the failure-path behavior.
void testNotEqualInts_WrapsWithDifferentValues(void)
{
    // Re-use the same failure-pattern with a different numeric pair.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(12345, 12346);
    VERIFY_FAILS_END
}

// Minimal Unity test runner for the new suite.
// We intentionally avoid GTest and rely on Unity's own RUN_TEST mechanism.

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations for the Unity-style test runner functions (if provided
// by the build system). These are often part of Unity's runtime; if your
// build environment provides a different entry, adjust accordingly.
int main(void);

#ifdef __cplusplus
}
#endif

// Implement a simple main that runs the two tests above.
// This mirrors the guidance to "call test methods from the main function" in the domain knowledge.
int main(void)
{
    // Initialize Unity test framework.
    UnityBegin("testNotEqualInts_suite.cpp");

    // Run tests that exercise the focal failure-path behavior.
    RUN_TEST(testNotEqualInts_WrapsCorrectly);
    RUN_TEST(testNotEqualInts_WrapsWithDifferentValues);

    // Conclude tests and return status.
    return UnityEnd();
}

// Note on dependencies and keywords:
// - The focal test relies on Unity's EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_INT,
//   and VERIFY_FAILS_END macros to validate that an assertion failure is handled
//   as expected by the test harness (without terminating the entire suite).
// - IO spying helpers (startPutcharSpy, endPutcharSpy) are declared extern "C"
//   to align with the C linkage used by the Unity runtime in testunity.c.
// - This file does not touch private members or private methods; it only
//   exercises the abort-path semantics of a specific assertion pattern.
// - The test suite is designed to be compiled alongside the original testunity.c
//   source (and Unity's runtime) under C++11, without GTest.