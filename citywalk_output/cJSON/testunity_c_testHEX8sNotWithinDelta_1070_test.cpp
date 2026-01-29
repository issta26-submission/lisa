// test_hex8_within_delta_suite.cpp
// This file adds a focused test suite for the HEX8 within-delta assertion
// around the focal method testHEX8sNotWithinDelta (Unity test harness).
// The tests are designed to be compiled with a C++11 capable compiler,
// using Unity (no GTest). Each test includes explanatory comments.

// Import Unity in a C-friendly way for C++ compilation.
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

// Candidate Keywords from Step 1 (extracted from the focal method):
// - TEST_ASSERT_HEX8_WITHIN
// - TEST_ASSERT_HEX8_WITHIN_MESSAGE
// - EXPECT_ABORT_BEGIN
// - VERIFY_FAILS_END
// - HEX8 comparisons (within delta semantics)


// Test helpers do not rely on private or static internals; they exercise
// the public-API-like macros provided by Unity for hex8 within-delta checks.


// Test 1: Basic pass path - within delta should succeed without abort
// This exercises a typical scenario where actual is within the given delta of expected.
void testHEX8WithinDelta_Pass_Basic(void)
{
    // delta = 2, expected = 0xFE (254), actual = 0xFF (255) -> difference 1
    // Expect no abort; test should pass normally.
    TEST_ASSERT_HEX8_WITHIN(2, 0xFE, 0xFF);
}

// Test 2: Boundary pass path - exact delta boundary should pass
// This ensures boundary conditions are handled correctly.
void testHEX8WithinDelta_Pass_BoundaryExact(void)
{
    // delta = 2, expected = 0xFF (255), actual = 0xFD (253) -> difference 2
    // Exactly at the delta boundary; should pass.
    TEST_ASSERT_HEX8_WITHIN(2, 0xFF, 0xFD);
}

// Test 3: Boundary fail path - just outside delta should abort
// This confirms that values just outside the allowed delta cause a failure.
void testHEX8NotWithinDelta_Edge_Fail(void)
{
    // delta = 1, expected = 0xFF (255), actual = 0xFC (252) -> difference 3
    // Outside delta; test should abort.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(1, 0xFF, 0xFC);
    VERIFY_FAILS_END
}

// Test 4: Pass path with custom message - within delta with message should pass
// Ensures the variant that attaches a message does not affect success paths.
void testHEX8WithinDelta_WithMessage_Pass(void)
{
    // delta = 2, expected = 0xFE (254), actual = 0xFF (255) -> difference 1
    // Should pass even when a custom message is supplied (on success, message unused).
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 0xFE, 0xFF, "HEX8 within delta should pass with message.");
}

// Test 5: Fail path with custom message - within delta fails and reports message
// Confirms that the custom message is recorded when assertion fails.
void testHEX8WithinDelta_WithMessage_Fail(void)
{
    // delta = 1, expected = 0xFF (255), actual = 0xFC (252) -> difference 3
    // Should abort and include the custom message.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(1, 0xFF, 0xFC, "HEX8 within delta failed with custom message.");
    VERIFY_FAILS_END
}


// Main entry point to execute the added tests along with a minimal runner.
// This allows integration as a standalone test unit when compiling with C++11.
int main(void)
{
    UNITY_BEGIN();

    // Run the extended HEX8 within-delta tests
    RUN_TEST(testHEX8WithinDelta_Pass_Basic);
    RUN_TEST(testHEX8WithinDelta_Pass_BoundaryExact);
    RUN_TEST(testHEX8NotWithinDelta_Edge_Fail);
    RUN_TEST(testHEX8WithinDelta_WithMessage_Pass);
    RUN_TEST(testHEX8WithinDelta_WithMessage_Fail);

    return UNITY_END();
}