/*
  C++11 compatible Unity test suite for the focal testUINT8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage concept.
  - This test suite is crafted to exercise the behavior where 8-bit comparisons ignore high-order bits
    (differences outside the 8 LSBs) by truncating to 8 bits before computing the delta.
  - It uses Unity's C macros (via extern "C" linkage) to remain consistent with the original test suite
    located in testunity.c, but provides a separate translation unit so it can be compiled within a C++11 project.
  - The tests are designed to be non-terminating (non-fatal) for coverage purposes where appropriate,
    while still asserting the intended outcomes.
  - Two tests are included:
    1) A passing case mirroring the focal test (LSB-equal after truncation, delta satisfied with a custom message).
    2) A failing case to confirm that when delta is too small, even with truncation, the comparison fails
       and the custom message is reported.
*/

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Unity is a C framework. Include with C linkage in a C++ translation unit.
extern "C" {
}

// Optional: forward declare test setup/teardown if Unity expects them.
// These are kept minimal to avoid side effects on global state.
static void setUp(void);
static void tearDown(void);

// Focal test cases (non-original function names to avoid symbol clash with testunity.c).
// 1) Focal pass-case: Demonstrates that 0x123 and 0xF23 truncate to 0x23, difference 0, within delta, with custom message.
static void test_UINT8_WithInDelta_DiffOutside8Bits_WithCustomMessage_Pass_FocalVariant(void)
{
    // This mirrors the focal test:
    // TEST_ASSERT_UINT8_WITHIN_MESSAGE(5, 0x123, 0xF23, "Custom Message.");
    // After truncation to 8 bits: 0x23 vs 0x23 -> diff = 0 <= 5 -> PASS
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(5, 0x123, 0xF23, "Custom Message.");
}

// 2) Focal fail-case: Delta too small even after truncation; ensures failure is reported with custom message.
static void test_UINT8_WithInDelta_DiffOutside8Bits_WithCustomMessage_FailSmallDelta(void)
{
    // This mirrors the idea: 0x120 (LSB 0x20) vs 0x121 (LSB 0x21) would have a delta of 1 after truncation.
    // With delta = 0, this should fail, and Unity will report the custom message string.
    // Expecting failure:
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT8_WITHIN_MESSAGE(0, 0x120, 0x121, "Custom Message.");
    VERIFY_FAILS_END
}

// The test runner entry point
int main(void)
{
    UnityBegin("test_UINT8_within_delta_diff_outside8bits_custom_message_cpp");

    // Register tests
    RUN_TEST(test_UINT8_WithInDelta_DiffOutside8Bits_WithCustomMessage_Pass_FocalVariant);
    RUN_TEST(test_UINT8_WithInDelta_DiffOutside8Bits_WithCustomMessage_FailSmallDelta);

    return UnityEnd();
}

// Minimal SetUp/TearDown implementations to satisfy Unity's expectations.
// They intentionally perform no operations to avoid altering test behavior.
static void setUp(void)
{
    // No per-test setup required for these tests
}

static void tearDown(void)
{
    // No per-test teardown required for these tests
}