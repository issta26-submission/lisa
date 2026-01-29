/*
Step 1 & 2: Focal method understanding and test plan
- Focal method under test: testNotEqualUINT32Arrays2
  - It creates two Unity UINT32 arrays p0 and p1 that differ in the first element.
  - It then calls TEST_ASSERT_EQUAL_UINT32_ARRAY(p0, p1, 4) inside EXPECT_ABORT_BEGIN / VERIFY_FAILS_END wrappers.
  - The test asserts that the UNITY assertion fails (the arrays are not equal) and thus the test intentionally triggers a failure.
- Core candidate keywords derived from the focal method:
  - UNITY_UINT32, p0, p1, TEST_ASSERT_EQUAL_UINT32_ARRAY, 4
  - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - TEST framework macros for array comparison of 32-bit unsigned integers
- Class dependencies (Unity framework primitives) are referenced via the macros above; the static helpers (startPutcharSpy, endPutcharSpy) and test harness plumbing are provided by the existing testunity.c in the project.
- This suite will wrap the focal test in a plain C/C++11 entrypoint (no GTest) and invoke it via Unity's RUN_TEST mechanism.

Note: The requested test suite will use a C++11 file that links against the existing C Unity test code (testunity.c) and its support functions. We expose the focal test via an extern "C" declaration to ensure correct linkage.

Now providing the test harness code (as requested: code only with explanatory comments for the unit test).
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
extern "C" {
#endif
// Forward declare the focal test function defined in testunity.c
void testNotEqualUINT32Arrays2(void);
#ifdef __cplusplus
}
#endif

/*
 Candidate Keywords (for traceability):
 - UNITY_UINT32, p0, p1
 - TEST_ASSERT_EQUAL_UINT32_ARRAY, 4
 - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
 - RUN_TEST, UNITY_BEGIN, UNITY_END
 - No GTest; plain Unity usage
*/

/*
Step 3: Test Case - Wrap the focal test in a minimal runner using Unity's RUN_TEST.
Explanation:
- This harness runs the focal test (which is designed to fail because p0 != p1).
- The test is expected to abort due to inequality, which is captured by the Unity test framework via the EXPECT_ABORT_BEGIN/VERIFY_FAILS_END wrappers in the focal test.
- The wrapper ensures the test harness executes the focal test in a controlled environment and reports the outcome accordingly.
*/

// Main entry point for the test executable
int main(void)
{
    // Begin the Unity test framework
    UNITY_BEGIN();

    /*
     Executed test:
     - testNotEqualUINT32Arrays2: Verifies that TEST_ASSERT_EQUAL_UINT32_ARRAY reports a failure
       when two UINT32 arrays differ (first element differs in p0 vs p1).
     This matches the intended behaviour of the focal test.
    */
    RUN_TEST(testNotEqualUINT32Arrays2);

    // End the Unity test framework and return the test results
    return UNITY_END();
}