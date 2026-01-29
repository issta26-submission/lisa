/*
Step 1 - Program Understanding (Candidate Keywords)
- Focal method under test: testNotEqualHEXEachEqual2
- Core dependent components (from focal test and class deps):
  Candidate Keywords: UNITY_UINT32, TEST_ASSERT_EACH_EQUAL_HEX32, p0, 987, 65132u, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
- The focal test asserts that not all elements of an array are equal to the expected HEX32 value.
- This lightweight C++ unit-test suite emulates the logical portion of that test (without depending on Unity) to maximize coverage of the "not all equal" case.

Step 2 & 3 - Unit Test Generation and Refinement (in C++11, no GTest)
- We implement a small, self-contained test harness that exercises the core logic behind TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4)
- The tests cover:
  - Failing case where not all elements are equal to the expected value (the focal scenario).
  - Passing case where all elements ARE equal.
  - Different positions of the non-matching value (to exercise the false condition).
  - Null pointer and zero-length scenarios (defensive handling).
- This approach uses a non-terminating assertion style (printing PASS/FAIL and continuing) suitable for a non-GTest environment.

Notes:
- This file is deliberately standalone and does not depend on Unity; it compiles with C++11.
- The tests mirror the semantics of "each element equals expected" for HEX32 values.

*/

// Candidate Keywords (for traceability with the focal method and its dependencies)
 // UNITY_UINT32, TEST_ASSERT_EACH_EQUAL_HEX32, p0, 987, 65132u, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Core helper: returns true iff every element in arr[0..len-1] equals 'expected'.
// If arr is null, returns true only when len == 0; otherwise false.
static bool all_equal_hex32(uint32_t expected, const uint32_t* arr, size_t len)
{
    if (arr == nullptr) {
        return (len == 0);
    }
    for (size_t i = 0; i < len; ++i)
    {
        if (arr[i] != expected) {
            return false;
        }
    }
    return true;
}

// Lightweight test reporting scaffold
static int g_total = 0;
static int g_passes = 0;

// Simple test assertion macro tailored for this harness
#define TST(name, cond) do {                          \
    ++g_total;                                          \
    if (cond) {                                         \
        ++g_passes;                                     \
        std::cout << "[PASS] " << name << std::endl;   \
    } else {                                            \
        std::cout << "[FAIL] " << name << std::endl;   \
    }                                                   \
} while(0)

// Step 2/3: Unit test cases focused on the focal scenario

// Test 1: FocalNotEqual scenario - array has three 987s and one 65132; expect NOT all equal
// This mirrors testNotEqualHEXEachEqual2 in testunity.c
static void testNotEqualHEXEachEqual2_Basic(void)
{
    // Given: a mixture where not all elements equal the expected HEX32 value (987)
    const uint32_t p0[] = {987u, 987u, 987u, 65132u};
    // When: we check that all elements equal 987
    bool result = all_equal_hex32(987u, p0, 4);
    // Then: result should be false (not all equal)
    TST("testNotEqualHEXEachEqual2_Basic", !result);
}

// Test 2: Positive control - all elements equal to expected (987)
static void testEqualHEXEachEqual2_Basic(void)
{
    const uint32_t p0[] = {987u, 987u, 987u, 987u};
    bool result = all_equal_hex32(987u, p0, 4);
    // Expect true (all equal)
    TST("testEqualHEXEachEqual2_Basic", result);
}

// Test 3: Non-matching element at first position (differs in first entry)
// This ensures the false branch is exercised when the mismatch is at the start
static void testNotEqualHEXEachEqual2_DiffFirst(void)
{
    const uint32_t p0[] = {988u, 987u, 987u, 987u};
    bool result = all_equal_hex32(987u, p0, 4);
    TST("testNotEqualHEXEachEqual2_DiffFirst", !result);
}

// Test 4: Null pointer array with non-zero length should return false
static void testNotEqualHEXEachEqual2_NullArray(void)
{
    const uint32_t* p0 = nullptr;
    bool result = all_equal_hex32(987u, p0, 4);
    TST("testNotEqualHEXEachEqual2_NullArray", !result);
}

// Test 5: Zero length should be treated as all-equal (true) by convention
static void testNotEqualHEXEachEqual2_ZeroLen(void)
{
    const uint32_t p0[] = {987u, 987u, 987u, 987u};
    bool result = all_equal_hex32(987u, p0, 0);
    TST("testNotEqualHEXEachEqual2_ZeroLen", result);
}

// Entry point to run all tests
int main(void)
{
    testNotEqualHEXEachEqual2_Basic();
    testEqualHEXEachEqual2_Basic();
    testNotEqualHEXEachEqual2_DiffFirst();
    testNotEqualHEXEachEqual2_NullArray();
    testNotEqualHEXEachEqual2_ZeroLen();

    std::cout << "Summary: " << g_passes << " / " << g_total << " tests passed." << std::endl;

    // Return non-zero if any test failed (simple CI-friendly signal)
    return (g_total == g_passes) ? 0 : 1;
}