/*
Step 1 (Program Understanding) - Candidate Keywords
- test function: testEqualUint16sWhenThereAreDifferencesOutside16Bits
- Core dependent components:
  - Unity assertion: TEST_ASSERT_EQUAL_UINT16
  - Values illustrating 16-bit truncation: 0x54321 vs 0x64321
  - Values illustrating truncation with upper bits: 0xFFFF4321 vs 0x00004321
  - 16-bit boundary behavior (lower 16 bits comparison)
  - Unity framework macros: TEST_ASSERT_EQUAL_UINT16, UNITY_BEGIN/END, RUN_TEST
- Focus: verify that two integers differing only outside the lower 16 bits are considered equal when compared as 16-bit unsigned values
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
void setUp(void);
void tearDown(void);
#ifdef __cplusplus
}
#endif

/* 
   Test 1: Verifies that two values differing only outside the 16-bit boundary are treated as equal
   by TEST_ASSERT_EQUAL_UINT16 when the lower 16 bits match.
*/
static void testEqualUint16sWhenThereAreDifferencesOutside16Bits_PassFirst(void)
{
    // Core dependent scenario: lower 16 bits of both values are 0x4321
    // 0x54321 truncated to 16 bits -> 0x4321
    // 0x64321 truncated to 16 bits -> 0x4321
    // Expectation: the assertion should pass
    TEST_ASSERT_EQUAL_UINT16(0x54321, 0x64321);
}

/* 
   Test 2: Verifies another pair where the lower 16 bits match after truncation
   - 0xFFFF4321 truncated to 0x4321
   - 0x00004321 truncated to 0x4321
*/
static void testEqualUint16sWhenThereAreDifferencesOutside16Bits_PassSecond(void)
{
    // Core dependent scenario: ensure that significant upper bits do not affect 16-bit comparison
    TEST_ASSERT_EQUAL_UINT16(0xFFFF4321, 0x00004321);
}

/*
   Note:
   - The focal method under test is testEqualUint16sWhenThereAreDifferencesOutside16Bits.
   - The above two tests exercise the true-branch behavior of the 16-bit comparison by ensuring
     that values differing only in bits above 16 are considered equal when truncated to 16 bits.
   - To preserve a green test run in CI, we avoid adding a failing test case here to prevent
     unintended test-suite aborts. If needed, a separate test could be added that intentionally
     fails (wrapped with EXPECT_ABORT_BEGIN/VERIFY_FAILS_END from the Unity extras) to verify
     failure quoting, but that requires the full macro support from testunity.c.
*/

int main(void)
{
    // Step 3: Test Case Refinement - Run selected tests
    UNITY_BEGIN();
    RUN_TEST(testEqualUint16sWhenThereAreDifferencesOutside16Bits_PassFirst);
    RUN_TEST(testEqualUint16sWhenThereAreDifferencesOutside16Bits_PassSecond);
    return UNITY_END();
}