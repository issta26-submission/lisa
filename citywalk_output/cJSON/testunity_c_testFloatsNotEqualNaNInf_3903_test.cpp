// Test harness in C++11 for the focal Unity test method: testFloatsNotEqualNaNInf
// This harness uses Unity's C API to execute the focal test function defined in testunity.c.
// The goal is to verify that comparing NaN vs Inf via TEST_ASSERT_EQUAL_FLOAT triggers
// a failure (abort) when floating-point tests are enabled, matching the behavior of
// testFloatsNotEqualNaNInf in the provided source.

// Step information and candidate keywords (explanatory comments):
// - Focal method under test: testFloatsNotEqualNaNInf
// - Core dependent components: UNITY_EXCLUDE_FLOAT gating, NaN/Inf generation (0.0f/0.0f and 1.0f/0.0f),
//   UNITY assertion TEST_ASSERT_EQUAL_FLOAT, abort verification via EXPECT_ABORT_BEGIN/VERIFY_FAILS_END.
// - Class dependencies (from <FOCAL_CLASS_DEP>): Unity testing framework (unity.h), static f_zero constant,
//   floating-point path selection, and abort/verify scaffolding macros.
// - Domain knowledge considerations: ensure path coverage for enabled/disabled floating-point tests, respect
//   static/global test state, and use non-private interfaces only. Since this is a ported Unity test, we rely
//   on the existing Unity harness to capture failures rather than throwing exceptions.
// - Test strategy: execute the focal test exactly as defined, capturing whether it aborts as expected when
//   floating-point is enabled, or is ignored when UNITY_EXCLUDE_FLOAT is defined during compilation of
//   testunity.c.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// The focal function under test is defined in testunity.c
void testFloatsNotEqualNaNInf(void);
#ifdef __cplusplus
}
#endif

int main(void) {
    // Begin Unity test run
    UNITY_BEGIN();

    // Explanatory note: This runs the focal test that asserts two expressions yield
    // different floating-point values (NaN vs Inf). If floating-point tests are enabled
    // in the Unity setup, the assertion TEST_ASSERT_EQUAL_FLOAT should fail, causing an abort
    // which Unity captures as a test failure. If UNITY_EXCLUDE_FLOAT is defined at compile time
    // for testunity.c, the test path will be skipped (TEST_IGNORE), which will be reported accordingly.
    //
    // Candidate Keywords in action:
    // - testFloatsNotEqualNaNInf: focal test function
    // - RUN_TEST: executes the test under Unity's harness
    // - Unity's abort/verify path: exercised by the focal test when floats are enabled
    RUN_TEST(testFloatsNotEqualNaNInf);

    // End Unity test run and return failure count
    return UNITY_END();
}