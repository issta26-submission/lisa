// test_NotEqualBitsLow_suite.cpp
// Purpose: Provide a focused test runner for the focal method
//          testNotEqualBitsLow located in testunity.c (Unity-based tests).
// Notes:
// - This file acts as a small test harness that invokes the focal test function.
// - It assumes the Unity framework is available and that testunity.c provides
//   the implementation of testNotEqualBitsLow() wrapped with Unity's test macros.
// - Compile and link with the Unity sources (unity.c and any project test files).
// - This file uses C++11, and avoids GoogleTest (as requested).

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Forward declaration of the focal test function implemented in testunity.c.
// The function is defined with C linkage in that file; hence the extern "C".
// If your build system uses C linkage differently, adjust accordingly.
#ifdef __cplusplus
extern "C" {
#endif
void testNotEqualBitsLow(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the focal test for "Not Equal Bits Low" scenario.
    // This test is designed to verify that the TEST_ASSERT_BITS_LOW macro
    // correctly detects a mismatch in the lower bits and aborts as expected.
    // A passing test means the abort behavior was caught and handled as intended
    // by the test harness (e.g., via EXPECT_ABORT_BEGIN / VERIFY_FAILS_END in
    // the focal test).
    RUN_TEST(testNotEqualBitsLow);

    // Finalize Unity and return its status
    return UNITY_END();
}

/*
Explanatory notes for the unit test:
- testNotEqualBitsLow (in testunity.c) sets up two 32-bit values:
    uint32_t v0 = 0xFF55AA00;
    uint32_t v1 = 0x55550000;
  and then executes:
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_BITS_LOW(v0, v1);
    VERIFY_FAILS_END
  The intent is to ensure that the lower-bits comparison fails (i.e., bits_low do not match),
  and that Unity properly handles the expected abort. The test is considered a valid
  negative/abort-test and should pass when the abort is correctly triggered and captured.

- To run this suite:
  1) Ensure Unity sources (unity.c and unity.h) are compiled and linked.
  2) Compile this file as part of the test project.
  3) Link with the translation unit that provides testNotEqualBitsLow (i.e., testunity.c)
     so that the symbol is resolvable.

- This approach isolates the focal test and keeps the test harness lightweight while
  preserving the intended behavior validation of the NotEqualBitsLow scenario. 
*/