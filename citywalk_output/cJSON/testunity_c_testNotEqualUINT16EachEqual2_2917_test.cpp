// New test suite for the focal method testNotEqualUINT16EachEqual2
// This suite adds additional scenarios to exercise the NOT-Equal path
// for TEST_ASSERT_EACH_EQUAL_UINT16 with 16-bit values.
// The tests are written to be compatible with the existing Unity-based test harness
// used in testunity.c, and are intended to be compiled in a C++11 project without GTest.

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

// Forward declarations for Unity spy functions to satisfy the EXPECT_ABORT_BEGIN macro usage
// These are provided by the existing class dependencies in testunity.c in the actual project.
// If the build environment provides implementations, you can remove these stubs.
void startPutcharSpy(void) {}
void endPutcharSpy(void) {}
char* getBufferPutcharSpy(void) { return NULL; }

// Empty setup/teardown to satisfy Unity's expectations when linking with unity.c
void setUp(void) {}
void tearDown(void) {}

// Test 1: Mismatch occurs in the last element
// Rationale: verifies that a single out-of-place value anywhere in the array triggers
// the expected failure when asserting all elements equal to 65132U.
void testNotEqualUINT16EachEqual2_Variant1(void)
{
    unsigned short p0[] = {65132u, 65132u, 65132u, 0};
    // We expect TEST_ASSERT_EACH_EQUAL_UINT16 to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// Test 2: Mismatch occurs in the second element
// Rationale: ensures that a mismatch in a middle position is still detected correctly.
void testNotEqualUINT16EachEqual2_Variant2(void)
{
    unsigned short p0[] = {65132u, 0, 65132u, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// Test 3: Mismatch occurs in the first element
// Rationale: ensures detection of a mismatch at the array's starting index.
void testNotEqualUINT16EachEqual2_Variant3(void)
{
    unsigned short p0[] = {0, 65132u, 65132u, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
    VERIFY_FAILS_END
}

// Main runner for the suite
// This invokes the three additional variant tests to augment existing coverage
int main(void)
{
    UNITY_BEGIN();

    // Run the added tests to ensure coverage of NOT-EQUAL path across positions
    RUN_TEST(testNotEqualUINT16EachEqual2_Variant1);
    RUN_TEST(testNotEqualUINT16EachEqual2_Variant2);
    RUN_TEST(testNotEqualUINT16EachEqual2_Variant3);

    // Note: Existing focal test testNotEqualUINT16EachEqual2 in testunity.c is left intact.
    // This suite provides independent tests that exercise the same assertion macro
    // with different input arrays to improve coverage of the NOT-EQUAL branch.

    return UNITY_END();
}