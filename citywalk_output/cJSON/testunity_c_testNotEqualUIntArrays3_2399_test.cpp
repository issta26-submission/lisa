// test_not_equal_uint_arrays3_suite.cpp
// A minimal Unity-based C++11 compatible test runner that executes the focal test
// method: testNotEqualUIntArrays3 from testunity.c.
// This harness uses a single test invocation to ensure the focal test behaves as expected
// (i.e., it aborts as the two arrays differ at index 2). It relies on the existing Unity
// infrastructure and the macros defined in testunity.c (e.g., EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_UINT_ARRAY, etc.),
// which are linked together in the build.
//
// Rationale and coverage:
// - The focal testNotEqualUIntArrays3 asserts that two slightly different unsigned int arrays are not equal
//   and that the test correctly aborts (because of the expected failure).
// - This harness simply invokes that focal test within the standard Unity test flow to validate its behavior
//   in isolation. The Unity framework handles the abort-protection mechanics via the macros used inside
//   testunity.c.
//
// How to build (example):
// - Compile testNotEqualUIntArrays3 with the Unity test runner (the project already contains Unity's
//   implementation and the required test macros).
// - Link against testunity.c (or the translation unit that provides testNotEqualUIntArrays3) and
//   any required Unity support files.
// - Execute the resulting binary; it will run the focal test and verify it aborts as intended.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal test imported from testunity.c
// The symbol is provided by the existing Unity-based test file in the project.
void testNotEqualUIntArrays3(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Begin a Unity test run for the NotEqualUIntArrays3 focal test
    // This wraps the test in Unity's framework, enabling proper setup/teardown and result reporting.
    UnityBegin("NotEqualUIntArrays3_Focal_Suite");

    // Execute the focal test. The test itself uses EXPECT_ABORT_BEGIN/VERIFY_FAILS_END
    // to confirm that TEST_ASSERT_EQUAL_UINT_ARRAY fails when arrays differ.
    RUN_TEST(testNotEqualUIntArrays3);

    // End the test run and return the result.
    return UnityEnd();
}

// End of test harness for testNotEqualUIntArrays3
// Explanatory note: The actual verification of the abort behavior is encapsulated within
// the focal test's body in testunity.c. This harness only drives its execution
// and preserves consistency with the project's test organization.