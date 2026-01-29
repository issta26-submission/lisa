// test_suite.cpp
// Purpose: Drive the focal Unity C test function testNotEqualFloatEachEqual1(void)
//          from C++11, using Unity's test runner (no GTest).
// Notes:
// - This file treats the provided focal test as a black-box C unit test.
// - We rely on Unity's RUN_TEST mechanism to properly manage aborts
//   produced by EXPECT_ABORT_BEGIN / VERIFY_FAILS_END inside the focal test.
// - The testNotEqualFloatEachEqual1 under Unity_FLOAT enabled path is expected
//   to abort due to a non-equal element in the array (one value != 0.253f).
// - When UNITY_EXCLUDE_FLOAT is defined, the focal test will invoke TEST_IGNORE();
//   in that case, the runner will record the test as ignored rather than failed.
// - This approach keeps the surface area small (only one test) while ensuring
//   the focal method's intended control-flow (abort-on-failure) is exercised.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declaration of the focal test from the C source (testunity.c)
#ifdef __cplusplus
extern "C" {
#endif
void testNotEqualFloatEachEqual1(void);
#ifdef __cplusplus
}
#endif

// Wrapper test function to adapt the C focal test into Unity's RUN_TEST mechanism.
// This wrapper is necessary because the focal test itself is written as a
// Unity-based test and expects to be executed by the Unity harness.
// The focal test checks that not all elements in the array are equal to 0.253f.
// It should abort (and be considered a successful negative-path check) when
// the inequality is detected. Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END pattern
// ensures that the abort is handled gracefully within the test framework.
static void test_NotEqualFloatEachEqual1_Wrapper(void)
{
    // Expose the focal test to Unity's test runner.
    // The testNotEqualFloatEachEqual1 function:
    // - defines a 4-element float array: {0.253f, 8.0f, 0.253f, 0.253f}
    // - uses TEST_ASSERT_EACH_EQUAL_FLOAT(0.253f, p0, 4)
    // - The array contains 8.0f, so the assertion should fail, causing an abort.
    // - The EXPECT_ABORT_BEGIN / VERIFY_FAILS_END should catch this abort and
    //   mark the test as passed (the abort is the expected behavior for this test).
    testNotEqualFloatEachEqual1();
}

// Optional: If you want to separately test the behavior when UNITY_EXCLUDE_FLOAT
// is defined (the test would be ignored), you can compile with that macro.
// The wrapper above remains valid; Unity will treat it as an ignored test in that case.

// Main entry for the test runner.
// This executes the focal test within the Unity framework without GTest.
int main(void)
{
    // Begin a Unity test run named after this suite/file.
    UnityBegin("test_suite_not_equal_float_each_equal1.cpp");

    // Register and run the focal test wrapper.
    // This ensures we exercise the exact code path in testNotEqualFloatEachEqual1.
    RUN_TEST(test_NotEqualFloatEachEqual1_Wrapper);

    // End the test run and return the status.
    return UnityEnd();
}

// Notes for maintainers (mapping to Candidate Keywords and domain knowledge):
// - Focal elements: TEST_ASSERT_EACH_EQUAL_FLOAT, 0.253f, p0, 4
// - Expected behavior: mismatch triggers an abort, which Unity catches
//   via EXPECT_ABORT_BEGIN / VERIFY_FAILS_END pattern inside the focal test.
// - Static/global aspects: testNotEqualFloatEachEqual1 does not require
//   modification here; static members, if any, are in the C file scope and
//   are not directly accessed by this wrapper.
// - Namespace: C linkage for the focal function is ensured via extern "C".
// - Data types: uses float; the wrapper delegates to the focal test as-is.