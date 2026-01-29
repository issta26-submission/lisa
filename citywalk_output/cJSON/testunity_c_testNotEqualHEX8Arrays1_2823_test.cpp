// This test suite targets the focal method testNotEqualHEX8Arrays1 from testunity.c.
// Step 1 (Program Understanding) highlights core components:
// - Candidate Keywords: TEST_ASSERT_EQUAL_HEX8_ARRAY, p0, p1, 4, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
// - The focal test asserts inequality between two HEX8 arrays and expects Unity to abort the test.
// - The class dependencies (as seen in <FOCAL_CLASS_DEP>) refer to the Unity testing framework, macros, and
//   standard headers used to perform array equality checks in a testing context.
// Step 2 (Unit Test Generation) builds a small C++-driven Unity test harness that calls the focal test
//         function(s) via RUN_TEST to exercise the abort-on-failure behavior.
// Step 3 (Test Case Refinement) applies domain knowledge to ensure multiple related not-equal HEX8 array tests
//         are covered (HEX8 arrays with single-element differences, etc.) while keeping to C++11 and Unity.

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Bring in Unity's C headers into C++ linkage space.
extern "C" {
}

// Forward declarations for the focal tests from testunity.c (as they are defined with C linkage).
extern "C" void testNotEqualHEX8Arrays1(void);
// Additional related tests to broaden coverage in HEX8 not-equal scenarios.
extern "C" void testNotEqualHEX8Arrays2(void);
extern "C" void testNotEqualHEX8Arrays3(void);

int main(void)
{
    // Initialize the Unity test framework for the suite.
    // The name helps identify the test suite in reports.
    UNITY_BEGIN("testNotEqualHEX8Arrays1_Suite");

    // Explanation for each unit test:
    // - testNotEqualHEX8Arrays1: Validates that two HEX8 arrays differing in the last byte cause TEST_ASSERT_EQUAL_HEX8_ARRAY
    //   to fail and abort the test as expected (via EXPECT_ABORT_BEGIN / VERIFY_FAILS_END in the focal method).
    // - testNotEqualHEX8Arrays2/3: Extend coverage by exercising additional not-equal HEX8 array scenarios
    //   that follow the same expectation pattern (abort-on-failure) to increase overall coverage of the HEX8 path.
    //
    // Note: These tests rely on the Unity framework macros and the testunity.c provided tests, which implement
    // the abort-handling and failure-reporting behavior. We simply invoke them here from C++ to satisfy the
    // requirement of a GTest-free, Unity-based test harness.

    // Run the focal test: should abort due to inequality and be considered a passed test because the abort is expected.
    RUN_TEST(testNotEqualHEX8Arrays1);
    // Run additional related non-equal HEX8 array tests to ensure broader coverage of the same code paths.
    RUN_TEST(testNotEqualHEX8Arrays2);
    RUN_TEST(testNotEqualHEX8Arrays3);

    // Finalize Unity and return the aggregated test results.
    return UNITY_END();
}

// Domain knowledge notes (for maintainers):
// - The tests exercise non-terminating assertions in the sense of Unity's EXPECT_ABORT_BEGIN mechanism,
//   but the test harness itself uses non- GTest infrastructure.
// - Static file-scope members in testunity.c are already handled by the Unity framework; tests should be
//   compiled together with testunity.c to ensure proper linkage.
// - If additional coverage is desired, similar wrappers can be added for other "Not Equal" HEX8 array tests
//   or repeated patterns with slight variations in the last differing element.