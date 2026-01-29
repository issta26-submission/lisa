// Candidate Keywords extracted from the focal method and its dependencies
// - TEST_ASSERT_EQUAL_HEX8_ARRAY
// - p0, p1, p2, p3 (unsigned char arrays)
// - 1, 4, 3, 1 (lengths used in tests)
// - testEqualHEX8Arrays (the focal test method to mirror/extend)
// - Equality semantics over hex8 arrays
// - Boundary testing across different lengths
// - Not-equal verification via expected failures

// This test suite targets the focal behavior of testEqualHEX8Arrays by
// recreating and extending its scenarios in a C++11 compatible Unity-based test.
// The tests are written to be executable without GTest, invoking the Unity runner
// from main and using non-terminating assertions where appropriate.

#include <cstddef>
#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declarations for Unity setup/teardown (no special setup required here)
void setUp(void);
void tearDown(void);

// Basic coverage of the original focal test logic: equal hex8 arrays across various lengths
static void testEqualHEX8Arrays_BasicCases(void)
{
    // Mirror of the original test's core scenarios
    unsigned char p0[] = {1, 8, 254u, 123};
    unsigned char p1[] = {1, 8, 254u, 123};
    unsigned char p2[] = {1, 8, 254u, 2};
    unsigned char p3[] = {1, 23, 25, 26};

    // Equal when compared to itself with minimal and full length
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p0, 1);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p0, 4);

    // Equal when contents match across arrays with identical content
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p1, 4);

    // Not all elements match; compare up to length 3 to ensure early elements match
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p2, 3);

    // Different content at some position within length 1
    TEST_ASSERT_EQUAL_HEX8_ARRAY(p0, p3, 1);
}

// Extended coverage: verify that a mismatch within the specified length triggers a failure
static void testEqualHEX8Arrays_MismatchWithinLength(void)
{
    unsigned char a[] = {1, 8, 254u, 253u};
    unsigned char b[] = {1, 8, 254u, 252u};

    // Expect the assertion to fail when all 4 elements are compared and the 4th differs
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8_ARRAY(a, b, 4);
    VERIFY_FAILS_END
}

// Extended coverage: ensure that a mismatch beyond the specified length does not cause a failure
static void testEqualHEX8Arrays_MismatchBeyondLength_Pass(void)
{
    unsigned char a[] = {1, 2, 3, 4};
    unsigned char b[] = {1, 2, 3, 9};

    // Mismatch occurs at index 3, which is outside the compared length (3)
    // This should pass since only the first 3 elements are checked
    TEST_ASSERT_EQUAL_HEX8_ARRAY(a, b, 3);
}

// Comprehensive test that uses the focal behavior to confirm pass on identical arrays
// but uses a different arrangement to ensure cross-checking across multiple lengths.
static void testEqualHEX8Arrays_ExtendedCrossChecks(void)
{
    unsigned char a[] = {1, 2, 3, 4};
    unsigned char b[] = {1, 2, 3, 4};

    // All comparisons should pass
    TEST_ASSERT_EQUAL_HEX8_ARRAY(a, a, 1);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(a, b, 2);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(a, b, 3);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(a, b, 4);
}

// Main function to run the focused test suite
int main(void)
{
    // Unity initialization for this test file
    UNITY_BEGIN();

    // Run focused tests for testEqualHEX8Arrays behavior
    RUN_TEST(testEqualHEX8Arrays_BasicCases);
    RUN_TEST(testEqualHEX8Arrays_MismatchWithinLength);
    RUN_TEST(testEqualHEX8Arrays_MismatchBeyondLength_Pass);
    RUN_TEST(testEqualHEX8Arrays_ExtendedCrossChecks);

    // End Unity tests and return result
    return UNITY_END();
}

// Minimal setUp/tearDown implementations required by Unity
void setUp(void)
{
    // No special per-test setup required
}

void tearDown(void)
{
    // No per-test teardown required
}