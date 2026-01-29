// Memory equality unit tests focusing on the focal method/testNotEqualMemory2 semantics.
// This suite complements the existing testunity.c by exercising additional scenarios
// around TEST_ASSERT_EQUAL_MEMORY to improve coverage and validate expected abort behavior.
// Note: This file uses Unity's C-style API and is compiled under C++11 as requested.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Test: Not equal memory - inline approach (mirrors focal scenario using literals)
// This test should abort as the memory content differs at the 4th byte ('l' vs 'd').
void testNotEqualMemoryInline(void)
{
    // Expects Unity to abort on a mismatch
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 4);
    VERIFY_FAILS_END
}

// Test: Equal memory - inline approach
// This test should pass without abort since both regions are identical for 4 bytes.
void testEqualMemoryInline(void)
{
    TEST_ASSERT_EQUAL_MEMORY("fool", "fool", 4);
}

// Test: Not equal memory when actual pointer is NULL
// Should abort because a NULL pointer is not a valid memory region for non-zero length.
void testNotEqualMemoryNullActual(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4);
    VERIFY_FAILS_END
}

// Test: Not equal memory when expected pointer is NULL
// Should abort because a NULL expected memory region (non-zero length) cannot match.
void testNotEqualMemoryNullExpected(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", NULL, 4);
    VERIFY_FAILS_END
}

// Main runner for the test suite
int main(void)
{
    UNITY_BEGIN();

    // Run tests validating focal memory-not-equal behavior and its positive counterparts
    RUN_TEST(testNotEqualMemoryInline);
    RUN_TEST(testEqualMemoryInline);
    RUN_TEST(testNotEqualMemoryNullActual);
    RUN_TEST(testNotEqualMemoryNullExpected);

    return UNITY_END();
}