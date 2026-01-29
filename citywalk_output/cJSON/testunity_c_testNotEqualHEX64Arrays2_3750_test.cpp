/*
  Focused test suite for the focal method: testNotEqualHEX64Arrays2

  Context:
  - The focal method is the testNotEqualHEX64Arrays2() defined in testunity.c.
  - It exercises the assertion TEST_ASSERT_EQUAL_HEX64_ARRAY on two 64-bit arrays
    that differ at the first element, expecting the test to fail (abort) as per Unity's
    EXPECT_ABORT_BEGIN / VERIFY_FAILS_END pattern.

  Notes and design choices:
  - This suite uses Unity (Unity.h) compatible with C/C++ projects under C++11.
  - The test mirrors the focal method's scenario to validate the negative path for 64-bit arrays.
  - If UNITY_SUPPORT_64 is not defined, the test is marked as ignored to keep portability.
  - All tests are implemented as independent test cases and can be extended with more scenarios
    if needed to improve coverage (e.g., different positions of differences, length variations, etc.).
  - The code uses extern "C" linkage for Unity to ensure compatibility when compiled under C++.
  - Candidate Keywords extracted from the focal method (core dependencies):
    - UNITY_UINT64, TEST_ASSERT_EQUAL_HEX64_ARRAY, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
    - UNITY_SUPPORT_64
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

/*
  Candidate Keywords for traceability and test design:
  - UNITY_UINT64, TEST_ASSERT_EQUAL_HEX64_ARRAY, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - UNITY_SUPPORT_64, TEST_ABORT/ASSERT macros
  - The test exercises failure path when two 64-bit arrays differ.
*/

// Test when 64-bit support is available: verify that the hex64 array comparison aborts on difference
#ifdef UNITY_SUPPORT_64
void testNotEqualHEX64Arrays2_ShouldAbortOnDifference(void)
{
    // Arrange: two 64-bit arrays that differ at index 0
    UNITY_UINT64 p0[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    UNITY_UINT64 p1[] = {2ULL, 8ULL, 987ULL, 65132ULL};

    // Act & Assert: the comparison should fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX64_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}
#else
// If 64-bit support is not available, mark as ignored to keep the suite portable
void testNotEqualHEX64Arrays2_ShouldAbortOnDifference(void)
{
    TEST_IGNORE_MESSAGE("64-bit support not available (UNITY_SUPPORT_64 is undefined). This test is ignored on this platform.");
}
#endif

// Minimal runner to execute the focal test case.
// In a larger suite this could be expanded to cover more related scenarios or integrated with a suite aggregator.
int main(void)
{
    UNITY_BEGIN();

    // Run the focal method test as part of the suite
    RUN_TEST(testNotEqualHEX64Arrays2_ShouldAbortOnDifference);

    return UNITY_END();
}