// Test Suite: C++11 harness for Unity-based focal test testNotEqualHEX32Arrays2
// Purpose: Execute the focal Unity test (testNotEqualHEX32Arrays2) within a C++11
//          environment without GTest. This harness also invokes the related
//          NotEqual HEX32 Array tests (1 and 3) to ensure broader coverage
//          of the same dependency path and failure behavior.
//
// How this works:
// - The focal test is defined in testunity.c (as a C function). We declare it with
//   C linkage (extern "C") for compatibility with the C-based Unity framework.
// - We use Unity's RUN_TEST macro inside a small C++ main to drive the tests.
// - We rely on the existing Unity setup/teardown and the expected-abort wrappers
//   (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END) already present in testunity.c.
//
// Notes:
// - This harness assumes Unity's implementation (unity.c) is compiled and linked
//   with this test binary along with testunity.c.
// - The static/static-like behavior inside testunity.c (e.g., static arrays) is
//   exercised via the focal test calls themselves; no private details are accessed
//   here beyond what the focal tests exercise.
// - The tests are non-GMock / non-GTest and rely solely on the Unity framework.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Focal method under test (defined in testunity.c)
void testNotEqualHEX32Arrays2(void);

// Related tests (defined in testunity.c)
void testNotEqualHEX32Arrays1(void);
void testNotEqualHEX32Arrays3(void);

#ifdef __cplusplus
}
#endif

int main(void)
{
    // Initialize Unity Test Framework
    UNITY_BEGIN();

    // Test 1: Run the focal testNotEqualHEX32Arrays2
    // Rationale: Verifies that two equal-length HEX32 arrays with a single-different
    //            element cause the TEST_ASSERT_EQUAL_HEX32_ARRAY to fail as expected.
    RUN_TEST(testNotEqualHEX32Arrays2);

    // Test 2: Run additional related tests to exercise similar failure paths
    //         (NotEqual HEX32 Arrays with different elements in other positions).
    // Rationale: Increases coverage of the HEX32 array comparison logic.
    RUN_TEST(testNotEqualHEX32Arrays1);
    RUN_TEST(testNotEqualHEX32Arrays3);

    // Finalize Unity and return status
    return UNITY_END();
}