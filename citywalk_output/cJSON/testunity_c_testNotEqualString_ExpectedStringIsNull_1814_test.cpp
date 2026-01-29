// This C++11 test harness targets the focal testNotEqualString_ExpectedStringIsNull
// from testunity.c (Unity-based test). We run the focal test in isolation using
// Unity's RUN_TEST mechanism, capturing its abort behavior via EXPECT_ABORT_BEGIN/VERIFY_FAILS_END
// as defined inside the focal test itself.
// The goal is to exercise the core dependency: TEST_ASSERT_EQUAL_STRING against a NULL
// expected string and a non-NULL actual string, ensuring the test aborts as designed.

#include <cstddef>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Bring in the Unity framework (C header). Use extern "C" to prevent name mangling in C++.
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal test function (defined in testunity.c).
// It uses Unity abort-control macros to verify failure when expected string is NULL.
void testNotEqualString_ExpectedStringIsNull(void);
#ifdef __cplusplus
}
#endif

// Simple test harness using Unity's API, compiled under C++11.
// This main function runs the focal test case and reports the result.
// We deliberately keep the harness minimal to avoid altering the behavior of the focal test.
int main(void) {
    // Begin a Unity test run for the focal test suite.
    // The string here identifies the test suite; it is arbitrary but descriptive.
    UnityBegin("FocalSuite_TestNotEqualString_ExpectedStringIsNull");

    // Run the focal test. The test itself uses EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
    // to validate a failure path when TEST_ASSERT_EQUAL_STRING(NULL, "bar") is evaluated.
    RUN_TEST(testNotEqualString_ExpectedStringIsNull);

    // End the test run and return the appropriate exit code.
    return UnityEnd();
}

// Explanations for the test strategy (embedded as comments for maintainers):
//
// - Focus: testNotEqualString_ExpectedStringIsNull validates that testing utilities correctly
//   handle a NULL expected string compared against a non-NULL actual string using
//   TEST_ASSERT_EQUAL_STRING. The test is expected to fail (abort) and Unity's
//   EXPECT_ABORT_BEGIN/VERIFY_FAILS_END macros ensure that the failure is captured without
//   terminating the entire test suite.
//
// - Core dependencies exercised:
//   - TEST_ASSERT_EQUAL_STRING: primary comparison primitive under test.
//   - NULL vs non-NULL pointer handling: ensures NULL-pointer scenarios are rejected.
//   - Unity's abort/guard macros within the focal test: ensures controlled abort behavior
//     and proper test state update (Unity.CurrentTestFailed, etc.).
//
// - Design note: Additional variations (e.g., NULL actual vs non-NULL expected) are already
//   covered by the focal test's own structure and are exercised when this test is run.
//   If more coverage is required, consider adding similar harness entries for related focal
//   tests (e.g., testNotEqualStringLen_ExpectedStringIsNull) following the same RUN_TEST approach.