// A small, self-contained C++11 test harness focusing on the behavior of UINT16 within-delta
// tests, particularly the scenario where differences occur outside of the 16-bit range.
// This does not rely on GTest and is designed to be easily portable with minimal dependencies.

#include <cmath>
#include <cstdint>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight test harness
namespace TinyTestHarness {
    static int totalTests = 0;
    static int passedTests = 0;

    inline void fail(const std::string& msg) {
        throw std::runtime_error(msg);
    }

    // Simple assertion wrapper
    inline void assert_true(bool condition, const std::string& msg) {
        if (!condition) {
            fail(msg);
        }
    }

    // Run a single test function and report
    inline void runTest(const std::string& name, const std::function<void()>& test) {
        ++totalTests;
        try {
            test();
            ++passedTests;
            std::cout << "[PASSED] " << name << std::endl;
        } catch (const std::exception& e) {
            std::cout << "[FAILED] " << name << " - " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[FAILED] " << name << " - unknown exception" << std::endl;
        }
    }

    inline void printSummary() {
        std::cout << "\nTest Summary: " << passedTests << " passed, " << (totalTests - passedTests)
                  << " failed, out of " << totalTests << " tests." << std::endl;
    }

    // Helper: emulate Unity's UINT16 within-delta predicate with 16-bit truncation semantics
    // Behavior: compare lower 16 bits of both numbers; compute absolute difference; compare to delta.
    inline bool test_UINT16_WITHIN(uint32_t delta, uint32_t expected, uint32_t actual) {
        uint16_t exp16 = static_cast<uint16_t>(expected);
        uint16_t act16 = static_cast<uint16_t>(actual);
        int diff = static_cast<int>(static_cast<uint32_t>(act16)) - static_cast<int>(static_cast<uint32_t>(exp16));
        if (diff < 0) diff = -diff;
        return diff <= static_cast<int>(delta);
    }
}

// Namespace alias for brevity
using namespace TinyTestHarness;

// Focal tests for testUINT16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits
// These tests exercise the 16-bit truncation behavior observed in the focal method.
// Each test includes comments describing intent and expected outcomes.

void test_UINT16sWithinDeltaOutside16Bits_Pass(void) {
    // This corresponds to: TEST_ASSERT_UINT16_WITHIN(5, 0x54321, 0x44321);
    // After truncation: 0x4321 vs 0x4321 -> difference = 0, which is within delta (5)
    bool result = test_UINT16_WITHIN(5, 0x54321, 0x44321);
    assert_true(result, "Expected within-delta to pass for values differing outside 16 bits (wrap).");
}

// Variant with a descriptive message (simulated, as we are not using Unity's _MESSAGE macro here)
void test_UINT16sWithinDeltaOutside16Bits_PassWithMessage(void) {
    // Same scenario as the previous test; ensure the predicate passes.
    bool result = test_UINT16_WITHIN(5, 0x54321, 0x44321);
    if (!result) {
        // Simulated custom message behavior
        fail("Custom Message: UINT16 within-delta test failed for outside-16-bit difference (wrap).");
    }
}

// Negative/Fail case: ensure that a delta too small detects the effective difference after 16-bit truncation
void test_UINT16sWithinDeltaOutside16Bits_Fail(void) {
    // Construct a case where the lower 16 bits clearly differ beyond delta after truncation.
    // exp16 = static_cast<uint16_t>(0x44321) = 0x4321
    // act16 = static_cast<uint16_t>(0x41121) = 0x1121
    // diff 0x4321 - 0x1121 = 0x3200 = 12800, which is > delta (5)
    bool result = test_UINT16_WITHIN(5, 0x44321, 0x41121);
    if (result) {
        fail("Test unexpectedly passed when difference after 16-bit truncation should exceed delta.");
    }
}

// Additional test to verify behavior when delta is large enough to cover wrap difference
void test_UINT16sWithinDeltaOutside16Bits_PassWithBigDelta(void) {
    // Even though lower 16 bits would differ if chosen, choose a large delta to ensure pass
    // For example: expected 0x54321 -> 0x4321, actual 0x41121 -> 0x1121; diff = 0x3200
    bool result = test_UINT16_WITHIN(0x4000, 0x54321, 0x41121); // delta = 16384
    // 0x3200 (12800) <= 16384, so this should pass
    assert_true(result, "Expected pass when delta is sufficiently large to cover 16-bit wrap difference.");
}

int main() {
    // Register and run tests
    runTest("test_UINT16sWithinDeltaOutside16Bits_Pass", test_UINT16sWithinDeltaOutside16Bits_Pass);
    runTest("test_UINT16sWithinDeltaOutside16Bits_PassWithMessage", test_UINT16sWithinDeltaOutside16Bits_PassWithMessage);
    runTest("test_UINT16sWithinDeltaOutside16Bits_Fail", test_UINT16sWithinDeltaOutside16Bits_Fail);
    runTest("test_UINT16sWithinDeltaOutside16Bits_PassWithBigDelta", test_UINT16sWithinDeltaOutside16Bits_PassWithBigDelta);

    printSummary();
    return (totalTests == passedTests) ? 0 : 1;
}