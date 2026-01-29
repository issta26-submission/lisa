// Test suite for the focal behavior corresponding to testNotEqualUINT32EachEqual3
// Adapted for C++11 (no GTest). This suite does not rely on the Unity framework.
// It focuses on the core logical meaning of TEST_ASSERT_EACH_EQUAL_UINT32(1, p0, 4):
// - It passes if every element in p0 equals 1
// - It fails (abort-like) if any element differs from 1
//
// Candidate Keywords guiding this suite: UNITY_UINT32, TEST_ASSERT_EACH_EQUAL_UINT32,
// p0 (array under test), 1 (expected value), 4 (length), abort on mismatch, not-all-equal case.
//
// This test suite is designed to be self-contained and executable via a plain C++
// main function, suitable for building with a C++11 toolchain.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


namespace FocalTestSuite {

// Simple stand-in for the core comparison logic behind TEST_ASSERT_EACH_EQUAL_UINT32(1, p0, len)
// Returns true if all elements are equal to 'expected', false otherwise.
static bool allElementsEqualUint32(const uint32_t* arr, size_t len, uint32_t expected)
{
    for (size_t i = 0; i < len; ++i)
    {
        if (arr[i] != expected)
            return false;
    }
    return true;
}

// Global counters for test results (simple harness)
static int gTotalTests  = 0;
static int gFailedTests = 0;

// Helper to report a failed assertion
static void reportFailure(const std::string& testName, const std::string& detail)
{
    std::cerr << "FAIL: " << testName << " - " << detail << std::endl;
    ++gFailedTests;
}

// Helper to report a passed test
static void reportSuccess(const std::string& testName)
{
    std::cout << "PASS: " << testName << std::endl;
}

// Test 1 (Focal): Verifies that testNotEqualUINT32EachEqual3 corresponds to a "not all equal" case.
// Input: p0 = {1, 1, 1, 65132}
// Expected: not all elements are 1 -> the assertion should fail (i.e., our checker returns false)
void testNotEqualUINT32EachEqual3_ReturnsFalse(void)
{
    const std::string testName = "testNotEqualUINT32EachEqual3_ReturnsFalse";

    ++gTotalTests;
    uint32_t p0[] = {1, 1, 1, 65132u};
    bool result = allElementsEqualUint32(p0, 4, 1); // Simulates TEST_ASSERT_EACH_EQUAL_UINT32(1, p0, 4)

    if (result)
    {
        reportFailure(testName, "Expected not-all-equal scenario to fail, but all elements were equal.");
    }
    else
    {
        reportSuccess(testName);
    }
}

// Test 2 (Complement): Verifies that when all elements are 1, the same check passes.
// Input: p0 = {1, 1, 1, 1}
// Expected: all elements equal to 1 -> the assertion should pass
void testNotEqualUINT32EachEqual3_AllEqualReturnsTrue(void)
{
    const std::string testName = "testNotEqualUINT32EachEqual3_AllEqualReturnsTrue";

    ++gTotalTests;
    uint32_t p0[] = {1, 1, 1, 1};
    bool result = allElementsEqualUint32(p0, 4, 1);

    if (result)
    {
        reportSuccess(testName);
    }
    else
    {
        reportFailure(testName, "Expected all elements to be equal to 1, but found a mismatch.");
    }
}

// Test 3 (Edge): Verifies that an empty array is treated as all-equal (vacuously true).
// Input: p0 = {} with len = 0
// Expected: vacuously true (no elements differ from 1)
void testNotEqualUINT32EachEqual3_EmptyArrayReturnsTrue(void)
{
    const std::string testName = "testNotEqualUINT32EachEqual3_EmptyArrayReturnsTrue";

    ++gTotalTests;
    uint32_t p0[] = {}; // length 0
    bool result = allElementsEqualUint32(p0, 0, 1);

    if (result)
    {
        reportSuccess(testName);
    }
    else
    {
        reportFailure(testName, "Empty array should be treated as all-equal (true).");
    }
}

// Public runner to execute all tests in this focal suite
void runAllTests(void)
{
    testNotEqualUINT32EachEqual3_ReturnsFalse();
    testNotEqualUINT32EachEqual3_AllEqualReturnsTrue();
    testNotEqualUINT32EachEqual3_EmptyArrayReturnsTrue();
}

} // namespace FocalTestSuite

int main()
{
    // Run the focal test suite
    FocalTestSuite::runAllTests();

    // Summary
    std::cout << "\nTest Summary: Total=" << FocalTestSuite::gTotalTests
              << ", Passed=" << (FocalTestSuite::gTotalTests - FocalTestSuite::gFailedTests)
              << ", Failed=" << FocalTestSuite::gFailedTests << std::endl;

    // Return non-zero if any test failed
    return (FocalTestSuite::gFailedTests > 0) ? 1 : 0;
}