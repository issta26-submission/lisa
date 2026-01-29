// This is a lightweight, self-contained C++11 unit-test harness
// to exercise the focal concept represented by testINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits
// from the Unity-based test file (testunity.c). It mirrors the core idea of
// TEST_ASSERT_INT16_WITHIN(delta, expected, actual) by performing
// 16-bit truncation (as would happen in C on a 16-bit signed int) and
// comparing the absolute difference against the given delta.
// The test suite is designed to be executable without GTest and without
// requiring the full Unity runtime. It focuses on the core semantics
// demonstrated by the focal test and related edge cases.
//
// Candidate Keywords (core concepts tested):
// - 16-bit signed truncation (int16_t) of 32-bit values
// - Delta-based within comparison (abs(diff) <= delta)
// - Wrapping behavior across 0xFFFF boundary (e.g., 0x54321 vs 0x44321)
// - Edge cases with negative representations via signed 16-bit wrap
// - Non-overflowing, non-terminating test execution (all tests run)
//
// Notes:
// - This harness is intentionally self-contained and does not depend on GTest or
//   the Unity test runner. It provides clear PASS/FAIL output per test and a final summary.

#include <cmath>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Core helper that mimics the behavior of TEST_ASSERT_INT16_WITHIN(delta, expected, actual)
// by truncating both numbers to 16-bit signed values and comparing their absolute difference.
static bool withinInt16Delta(int delta, uint32_t expected, uint32_t actual)
{
    // Simulate 16-bit signed truncation as C would do for int16_t casting
    int16_t e = static_cast<int16_t>(static_cast<uint16_t>(expected & 0xFFFFu));
    int16_t a = static_cast<int16_t>(static_cast<uint16_t>(actual & 0xFFFFu));

    int diff = static_cast<int>(e) - static_cast<int>(a);
    if (diff < 0) diff = -diff;
    return diff <= delta;
}

// Lightweight test harness infrastructure
typedef bool (*TestFn)(void);

struct TestCase {
    const char* name;
    TestFn function;
};

// Focal test variants (standalone, self-contained without Unity)


// Test 1: Replicates the focal test case where the two 32-bit values differ outside 16 bits
// but end up identical after 16-bit truncation, so the within-delta should pass.
static bool test_INT16sWithinDelta_WrapsPass(void)
{
    // delta = 5
    // 0x54321 truncated to 0x4321 (17185)
    // 0x44321 truncated to 0x4321 (17185)
    // |17185 - 17185| = 0 <= 5 -> PASS
    return withinInt16Delta(5, 0x54321, 0x44321);
}

// Test 2: A case where the truncation leads to a larger difference that exceeds delta.
// Use values that wrap to 0x7FFF and 0x8000 respectively, giving an int16 difference of 65535.
static bool test_INT16sWithinDelta_WrapsLargeDifferenceShouldFail(void)
{
    // delta = 5
    // 0x00007FFF -> 0x7FFF -> 32767
    // 0x00008000 -> 0x8000 -> -32768
    // |32767 - (-32768)| = 65535 > 5 -> FAIL
    return withinInt16Delta(5, 0x00007FFF, 0x00008000);
}

// Test 3: Negative wrap case that should pass for a small delta
// 0xFFFF -> -1, 0x0001 -> 1, difference is 2 which should be within delta = 2
static bool test_INT16sWithinDelta_NegativeWrapWithinDelta(void)
{
    return withinInt16Delta(2, 0x0000FFFF, 0x00000001);
}

// Test 4: Negative wrap case that should fail for a small delta
// 0xFFFF -> -1, 0x0001 -> 1, difference is 2 which should exceed delta = 1
static bool test_INT16sWithinDelta_NegativeWrapNotWithinDelta(void)
{
    return withinInt16Delta(1, 0x0000FFFF, 0x00000001);
}

// Test 5: Boundary case where both values truncate to same 0 value -> should pass with delta 0
static bool test_INT16sWithinDelta_BothZeroWithinDelta(void)
{
    // 0x00000000 and 0x00000000 both truncate to 0
    return withinInt16Delta(0, 0x00000000, 0x00000000);
}

// Test 6: Edge case where both values truncate to same large positive value -> should pass
static bool test_INT16sWithinDelta_SamePositiveWithinDelta(void)
{
    // 0x00007FFF and 0x00007FFF truncate to 0x7FFF
    return withinInt16Delta(0, 0x00007FFF, 0x00007FFF);
}

static TestCase g_TestSuite[] = {
    { "test_INT16sWithinDelta_WrapsPass", test_INT16sWithinDelta_WrapsPass },
    { "test_INT16sWithinDelta_WrapsLargeDifferenceShouldFail", test_INT16sWithinDelta_WrapsLargeDifferenceShouldFail },
    { "test_INT16sWithinDelta_NegativeWrapWithinDelta", test_INT16sWithinDelta_NegativeWrapWithinDelta },
    { "test_INT16sWithinDelta_NegativeWrapNotWithinDelta", test_INT16sWithinDelta_NegativeWrapNotWithinDelta },
    { "test_INT16sWithinDelta_BothZeroWithinDelta", test_INT16sWithinDelta_BothZeroWithinDelta },
    { "test_INT16sWithinDelta_SamePositiveWithinDelta", test_INT16sWithinDelta_SamePositiveWithinDelta },
    { nullptr, nullptr } // sentinel
};

// Helper to run all tests and print a summary
int main()
{
    std::cout << std::boolalpha;
    std::cout << "Running focal testINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits-inspired tests (self-contained harness)\n";
    int total = 0;
    int passed = 0;

    for (const TestCase* tc = g_TestSuite; tc && tc->name != nullptr; ++tc)
    {
        bool result = tc->function();
        ++total;
        if (result) {
            ++passed;
            std::cout << "[PASS] " << tc->name << "\n";
        } else {
            std::cout << "[FAIL] " << tc->name << "\n";
        }
    }

    std::cout << "\nTest results: " << passed << " / " << total << " passed.\n";
    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}