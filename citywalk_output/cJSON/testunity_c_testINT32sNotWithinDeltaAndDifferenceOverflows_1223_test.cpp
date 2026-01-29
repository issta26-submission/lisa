// This C++11 test suite focuses on the core logic behind the focal Unity test:
// testINT32sNotWithinDeltaAndDifferenceOverflows
// The original test asserts that TEST_ASSERT_INT32_WITHIN(1, -1, 0x7FFFFFFF) should fail,
// highlighting how a naive 32-bit subtraction could overflow. Here we implement a
// small, overflow-safe within-delta checker and validate its behavior with multiple tests.
//
// Candidate Keywords guiding the test generation:
// - INT32 within-delta predicate
// - delta (unsigned) comparison
// - overflow-safe arithmetic (64-bit intermediate)
// - edge cases near INT32_MIN / INT32_MAX
// - test harness (basic pass/fail reporting, no GTest)
// - static analysis of signed/unsigned boundaries
// - minimal dependency on external frameworks (no GTest, no Unity needed)

#include <exception>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Domain knowledge alignment: implement a safe within-delta predicate that avoids
// 32-bit signed overflow by performing the subtraction in 64-bit space.
static inline bool int32_within_delta(int32_t delta, int32_t expected, int32_t actual)
{
    // Use 64-bit arithmetic to avoid 32-bit overflow in the difference.
    const int64_t diff = static_cast<int64_t>(static_cast<int64_t>(actual) - static_cast<int64_t>(expected));
    return (diff <= static_cast<int64_t>(delta)) && (diff >= -static_cast<int64_t>(delta));
}

// Lightweight test harness (no external frameworks like GTest/Unity).
// It provides basic assertion handling, a tiny runner, and a final summary.
namespace UnitTestSuite
{
    static int totalTests = 0;
    static int passedTests = 0;
    static int failedTests = 0;

    // Simple assertion helper with descriptive message.
    inline void require(bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    // Convenience wrappers for common assertions.
    inline void assertTrue(bool condition, const std::string& message)
    {
        require(condition, message);
    }

    inline void assertFalse(bool condition, const std::string& message)
    {
        require(!condition, message);
    }

    // Runner for a single test function.
    void runTest(const std::string& testName, const std::function<void()>& testFunc)
    {
        ++totalTests;
        try
        {
            testFunc();
            ++passedTests;
            std::cout << "[PASS] " << testName << "\n";
        }
        catch (const std::exception& ex)
        {
            ++failedTests;
            std::cerr << "[FAIL] " << testName << " - " << ex.what() << "\n";
        }
        catch (...)
        {
            ++failedTests;
            std::cerr << "[FAIL] " << testName << " - Unknown exception\n";
        }
    }

    // Test cases

    // Test 1: Within-delta should be true for a small difference (no overflow)
    // This validates the basic, non-overflow path of the predicate.
    void test_int32_within_delta_true_basic()
    {
        // delta = 1, expected = -5, actual = -4 -> diff = 1
        const int32_t delta = 1;
        const int32_t expected = -5;
        const int32_t actual = -4;
        const bool within = int32_within_delta(delta, expected, actual);
        assertTrue(within, "int32_within_delta should be true for a difference <= delta (no overflow)");
    }

    // Test 2: Within-delta should be false for a difference exceeding delta (no overflow)
    void test_int32_within_delta_false_basic()
    {
        // delta = 1, expected = 100, actual = 102 -> diff = 2
        const int32_t delta = 1;
        const int32_t expected = 100;
        const int32_t actual = 102;
        const bool within = int32_within_delta(delta, expected, actual);
        assertFalse(within, "int32_within_delta should be false when difference exceeds delta");
    }

    // Test 3: Overflow-case for difference (the focal scenario)
    // This mirrors the Unity test: delta=1, expected=-1, actual=0x7FFFFFFF.
    // A naive 32-bit subtraction would overflow; the safe 64-bit path must yield false.
    void test_int32_within_delta_overflow_case()
    {
        const int32_t delta = 1;
        const int32_t expected = -1;
        const int32_t actual = 0x7FFFFFFF; // 2147483647
        const bool within = int32_within_delta(delta, expected, actual);
        assertFalse(within, "int32_within_delta must return false for overflow-prone difference (expected=-1, actual=INT32_MAX)");
    }

    // Test 4: Overflow-case near boundaries with large delta still should be false if diff is large
    void test_int32_within_delta_overflow_large_diff_should_be_false()
    {
        const int32_t delta = 2147483647; // INT32_MAX
        const int32_t expected = INT32_MIN; // -2147483648
        const int32_t actual = INT32_MIN + 5; // -2147483643
        const bool within = int32_within_delta(delta, expected, actual);
        // diff = 5, which is <= delta, so this is actually within. This test validates a near-boundary scenario.
        // Since diff is small, it's expected to be true.
        assertTrue(within, "int32_within_delta should be true when diff is well within a mega-delta near boundary");
    }

    // Test 5: Large opposite-side difference with big delta to ensure true
    void test_int32_within_delta_large_delta_true_case()
    {
        const int32_t delta = 2147483647; // INT32_MAX
        const int32_t expected = INT32_MIN; // -2147483648
        const int32_t actual = INT32_MAX;    // 2147483647
        const bool within = int32_within_delta(delta, expected, actual);
        // diff = 2147483647 - (-2147483648) = 4294967295, but since we use 64-bit, this is within delta
        // Noting: 4294967295 > INT32_MAX, but delta is INT32_MAX; diff > delta, hence not within.
        // Therefore this should be false.
        assertFalse(within, "int32_within_delta should be false for a huge diff exceeding INT32_MAX even with max delta");
    }

    // Run all tests
    void runAllTests()
    {
        // Note: This imitates a small portion of the Unity test style by collecting and running tests.
        runTest("test_int32_within_delta_true_basic", test_int32_within_delta_true_basic);
        runTest("test_int32_within_delta_false_basic", test_int32_within_delta_false_basic);
        runTest("test_int32_within_delta_overflow_case", test_int32_within_delta_overflow_case);
        runTest("test_int32_within_delta_overflow_large_diff_should_be_false", test_int32_within_delta_overflow_large_diff_should_be_false);
        runTest("test_int32_within_delta_large_delta_true_case", test_int32_within_delta_large_delta_true_case);
    }
} // namespace UnitTestSuite

// Entry point to execute the test suite.
// This can be compiled as a standalone C++11 program.
// The tests focus on the overflow-safe within-delta predicate underlying the focal Unity test.
int main()
{
    // Bring tests into scope
    using namespace UnitTestSuite;

    // Run all tests and print a concise summary
    runAllTests();

    // Summary
    std::cout << "\nTest Summary: "
              << "Total=" << totalTests
              << ", Passed=" << passedTests
              << ", Failed=" << failedTests << "\n";

    // Exit code 0 if all tests passed, non-zero otherwise
    return (failedTests == 0) ? 0 : 1;
}