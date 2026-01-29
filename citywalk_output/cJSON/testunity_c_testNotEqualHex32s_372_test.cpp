/*
 * Test Suite: NotEqualHex32s Focal Method
 * Context:
 * - According to the provided focal method (testNotEqualHex32s) in <FOCAL_METHOD>, the test validates
 *   that two different 32-bit hexadecimal values are not considered equal, i.e., TEST_ASSERT_EQUAL_HEX32
 *   should fail and Unity should abort the test.
 * - This suite is intended for a C++11 project and uses the Unity testing framework (no GTest).
 * - The focal dependencies (as seen in <FOCAL_CLASS_DEP>) include Unity macros (EXPECT_ABORT_BEGIN, VERIFY_FAILS_END),
 *   type aliases (UNITY_UINT32), and the assertion TEST_ASSERT_EQUAL_HEX32. The tests rely on the
 *   same abort-handling mechanism employed by the focal method.
 *
 * Step 1 (Program Understanding) keywords mapped from the focal method:
 * - UNITY_UINT32, v0, v1
 * - EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_HEX32, VERIFY_FAILS_END
 * - Dependency on Unity abort/verification flow
 * - startPutcharSpy / endPutcharSpy (Unity’s abort output interception)
 *
 * Step 2 (Unit Test Generation)
 * - We create a dedicated test runner for the focal method by exposing extern "C" entry points of:
 *     - testNotEqualHex32s (the focal test)
 *     - testNotEqualHex32sIfSigned (the signed variant already present in the focal class deps)
 * - The runner uses UnityBegin/RUN_TEST to execute the focal tests as part of a composite suite.
 *
 * Step 3 (Test Case Refinement)
 * - The suite ensures both focal tests (unsigned and signed variants) are exercised in a minimal, isolated
 *   manner, allowing the Unity harness to capture abort conditions without terminating the entire test
 *   run. Explanatory comments accompany each test invocation.
 *
 * Note:
 * - This test file is designed to be linked with the existing Unity framework source (unity.c) and the
 *   focal test source (testunity.c). It declares the focal tests as extern "C" to ensure proper linkage
 *   in a C++ compilation environment.
 * - The main function provided here is a standalone runner and may be swapped with the project’s standard
 *   test harness if one exists. The RUN_TEST calls mimic Unity’s standard usage.
 *
 * Require:
 * - Unity (unity.c/.h) compiled and linked with this test file.
 * - The focal tests testNotEqualHex32s and testNotEqualHex32sIfSigned must be visible to the linker.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal tests (extern to allow linkage from this separate test file)
void testNotEqualHex32s(void);
void testNotEqualHex32sIfSigned(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Step 2: Unit Test Registration and Execution
    // - This runner executes the focal unsigned not-equal-HEX32 test.
    // - It also executes the signed variant to broaden coverage across type usage.
    UnityBegin("NotEqualHex32s_Focal_Suite_CPP11");

    // Test 1: NotEqualHex32s (unsigned case)
    // Expects TEST_ASSERT_EQUAL_HEX32(v0, v1) to fail and Unity to flag the test as failed accordingly.
    // This directly exercises the focal method's abort-on-failure path.
    RUN_TEST(testNotEqualHex32s);

    // Test 2: NotEqualHex32sIfSigned (signed variant)
    // Ensures similar abort-on-failure behavior when dealing with signed 32-bit values.
    RUN_TEST(testNotEqualHex32sIfSigned);

    return UnityEnd();
}

/*
 * Explanation of test coverage decisions:
 * - The focal test testNotEqualHex32s uses v0 = 900000 and v1 = 900001 and asserts that equalHex32 should fail.
 *   This is a direct check of the 'not equal' path for 32-bit hex assertions.
 * - The neighboring dependency testNotEqualHex32sIfSigned exercises a signed variant to verify that the
 *   same failure semantics hold when a signed 32-bit value is involved.
 * - The tests rely on Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END to capture the expected abort without
 *   terminating the entire test suite, allowing multiple tests to run in a single session.
 * - All tests exercise the same domain (32-bit hex equality) but through unsigned and signed lenses to maximize
 *   coverage of the underlying macro behavior.
 *
 * Potential extension (not included to keep scope focused):
 * - Add additional focal tests for other hex sizes (HEX8, HEX16, HEX32) if separate focal methods exist
 *   to broaden coverage across the suite. This would require exposing their corresponding focal test entry points
 *   and registering them similarly with RUN_TEST.
 */