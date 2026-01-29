/*
 * Lightweight Unity-based test suite focusing on the focal method
 * testEqualHex16s from testunity.c (ported for a C/C++11 environment).
 *
 * This suite provides:
 * - A passing test that mirrors the focal testEqualHex16s scenario.
 * - A couple of failure-path tests using the Unity "EXPECT_ABORT_BEGIN" pattern
 *   to validate that mismatches abort as expected (without terminating the
 *   entire test run).
 * - Pointer-based variant to exercise addressing/pointer dereference paths.
 *
 * Notes:
 * - This file is designed to compile under C++11 (no GTest).
 * - Unity is treated as a C library; the header is included with extern "C" when
 *   compiled in C++.
 * - Minimal spy functions are provided to satisfy EXPECT_ABORT_BEGIN macro usage.
 */

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

// Lightweight stubs to satisfy Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END usage.
// The real project provides a more complete in-file spy; these are minimal for compilation.
static char g_putchar_spy_buffer[256];
static int g_putchar_spy_index = 0;

void startPutcharSpy(void) {
    g_putchar_spy_index = 0;
}

void endPutcharSpy(void) {
    // no-op for the lightweight harness
}

char* getBufferPutcharSpy(void) {
    g_putchar_spy_buffer[g_putchar_spy_index] = '\0';
    return g_putchar_spy_buffer;
}

// Static control flags used by some test harnesses in Unity-based suites.
// Kept to mirror the pattern in the provided dependency header, to ensure compatibility.
static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

void setUp(void) {
    // Reset per-test flags (mimic behavior from FOCAL_CLASS_DEP)
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void) {
    // Lightweight tear-down: ensure no stray failures leak into subsequent tests.
    if (SetToOneToFailInTearDown == 1) {
        // In a full Unity environment we might call TEST_FAIL_MESSAGE here.
        // For this lightweight harness, nothing additional is required.
    }
    // No additional global failure handling needed for these focused tests.
}

// Focal test: replicate the exact sequence in testEqualHex16s to verify correct behavior.
void testEqualHex16s_Basics(void)
{
    // Purpose: Validate that TEST_ASSERT_EQUAL_HEX16 works across
    //            value, variable, and pointer-based inputs.
    unsigned short v0, v1;
    unsigned short *p0, *p1;

    v0 = 0x9876;
    v1 = 0x9876;
    p0 = &v0;
    p1 = &v1;

    // All of these should pass (equal values)
    TEST_ASSERT_EQUAL_HEX16(0x9876, 0x9876);
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    TEST_ASSERT_EQUAL_HEX16(0x9876, v1);
    TEST_ASSERT_EQUAL_HEX16(v0, 0x9876);
    TEST_ASSERT_EQUAL_HEX16(*p0, v1);
    TEST_ASSERT_EQUAL_HEX16(*p0, *p1);
    TEST_ASSERT_EQUAL_HEX16(*p0, 0x9876);
}

// Failure-path test: ensure that a mismatch aborts as expected.
// This mirrors the pattern seen in the focal test suite (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END).
void testNotEqualHex16s_ShouldAbort(void)
{
    unsigned short v0 = 0x1234;
    unsigned short v1 = 0x1235;

    // Expect the following to abort (since 0x1234 != 0x1235)
    // The test harness captures the abort without failing the entire suite.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(v0, v1);
    VERIFY_FAILS_END
}

// Variant: test equality via pointers (addresses), ensuring addressing paths are covered.
void testNotEqualHex16s_Pointer_ShouldAbort(void)
{
    unsigned short v0 = 0x1234;
    unsigned short v1 = 0x1235;
    unsigned short *p0 = &v0;
    unsigned short *p1 = &v1;

    // Should abort due to differing contents at the pointer targets
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX16(*p0, *p1);
    VERIFY_FAILS_END
}

// Optional additional passing variant to exercise a slightly different input shape.
void testEqualHex16s_PointerAndConst_ShouldPass(void)
{
    const unsigned short c0 = 0xABCD;
    unsigned short v = 0xABCD;
    unsigned short *p = &v;

    TEST_ASSERT_EQUAL_HEX16(c0, v);
    TEST_ASSERT_EQUAL_HEX16(v, *p);
}

// Prototypes to satisfy the linker in C++ environments
void testEqualHex16s_Basics(void);
void testNotEqualHex16s_ShouldAbort(void);
void testNotEqualHex16s_Pointer_ShouldAbort(void);
void testEqualHex16s_PointerAndConst_ShouldPass(void);

// Main runner: invoke the test suite
int main(void)
{
    UNITY_BEGIN();

    // Execute focal tests (passing scenarios)
    RUN_TEST(testEqualHex16s_Basics);
    RUN_TEST(testEqualHex16s_PointerAndConst_ShouldPass);

    // Execute focal tests (failure scenarios)
    RUN_TEST(testNotEqualHex16s_ShouldAbort);
    RUN_TEST(testNotEqualHex16s_Pointer_ShouldAbort);

    return UNITY_END();
}