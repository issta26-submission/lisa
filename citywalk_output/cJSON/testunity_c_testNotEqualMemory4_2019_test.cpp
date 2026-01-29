/*
  High-level test suite for the focal method testNotEqualMemory4
  - Context: The focal method is a Unity-based test case that asserts
    a memory comparison should fail when the expected and actual pointers
    differ (specifically: comparing "fool" with NULL for 4 bytes).
  - Targeted environment: C++11, but using the Unity C unit-test framework
    (no GTest). The focal test itself is located in testunity.c and is exposed
    as:
        void testNotEqualMemory4(void);
  - This file provides a minimal wrapper to execute the focal test within a
    Unity-driven harness from C++11, keeping the test semantics faithful
    to the original (i.e., the test is expected to abort and be captured by
    Unity's abort-handling mechanism).

  Notes:
  - The harness uses the standard Unity entry points (UnityBegin, RUN_TEST, UnityEnd).
  - The testNotEqualMemory4 function already contains the EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
    blocks to capture the abort; running it through RUN_TEST ensures the abort is handled
    by the Unity harness and does not crash the entire test suite.
  - Explanations are provided alongside the test invocation to document intent and coverage goals.

  Candidate Keywords (from Step 1 analysis):
  - TEST_ASSERT_EQUAL_MEMORY, NULL, "fool", 4, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END,
    testNotEqualMemory4, testNotEqualMemory1/2/3 (reference variants), memory comparison,
    UnityBegin, RUN_TEST, UnityEnd, memory pointers, abort-capture, non-terminating assertions
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal test under test (located in testunity.c)
void testNotEqualMemory4(void);
#ifdef __cplusplus
}
#endif

// Domain knowledge note:
// - We expose and run only the focal testNotEqualMemory4 to validate the exact
//   code path where a memory comparison against NULL should abort.
// - We rely on Unity's abort-capture machinery (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
//   which are embedded inside testNotEqualMemory4.

int main(void)
{
    // Begin Unity test suite for NotEqualMemory4
    UnityBegin("NotEqualMemory4_Suite");

    // Execute the focal test that asserts a failing memory comparison produces an abort
    // and is properly captured by the Unity harness.
    // This corresponds to the Exact scenario: TEST_ASSERT_EQUAL_MEMORY("fool", NULL, 4);
    RUN_TEST(testNotEqualMemory4);

    // End and report results
    return UnityEnd();
}