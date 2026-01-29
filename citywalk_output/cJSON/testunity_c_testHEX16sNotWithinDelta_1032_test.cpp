/*
  Test suite: Hex16 within Delta logic (C++11)
  Purpose: Complement the focal Unity test testHEX16sNotWithinDelta by validating
           the underlying hex16WithinDelta-like behavior used by TEST_ASSERT_HEX16_WITHIN.
           This standalone suite does not rely on GTest and uses a lightweight C++11 test harness.

  Notes:
  - We implement a small helper hex16WithinDelta(delta, a, b) that emulates the core
    comparison used by TEST_ASSERT_HEX16_WITHIN: absolute difference <= delta.
  - Each test case below is self-contained and includes comments describing the scenario.
  - The main function runs all tests and prints a concise pass/fail summary.

  This file is intended to compile with a C++11 compiler in a project that also contains
  the Unity-based C tests (e.g., testunity.c). It does not depend on Unity headers.
*/

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Core logic replicated for test purposes: check if two 16-bit values are within a delta.
static bool hex16WithinDelta(uint16_t delta, uint16_t a, uint16_t b)
{
    // Compute absolute difference safely using 32-bit intermediate
    uint32_t diff = (a > b) ? static_cast<uint32_t>(a) - static_cast<uint32_t>(b)
                            : static_cast<uint32_t>(b) - static_cast<uint32_t>(a);
    return diff <= static_cast<uint32_t>(delta);
}

// Lightweight test harness
typedef bool (*TestFn)();

struct TestCase {
    std::string name;
    TestFn function;
};

// Test 1: Within delta true for a simple small-range pair
// Verifies that 5000 and 5001 are within delta 2.
static bool test_hex16_within_delta_true_basic() {
    return hex16WithinDelta(2, 5000, 5001);
}

// Test 2: Not within delta for focal case from the original problem
// Verifies that 65535 and 0 are NOT within delta 2 (wrap-around boundary not considered).
static bool test_hex16_within_delta_false_case_focal() {
    return !hex16WithinDelta(2, 65535, 0);
}

// Test 3: Zero delta with identical values should be within
// Verifies that delta 0 accepts identical numbers (edge case).
static bool test_hex16_within_delta_zero_delta_same() {
    return hex16WithinDelta(0, 12345, 12345);
}

// Test 4: Wrap-around boundary false
// Verifies that a and b at the ends of the 16-bit range are not within a small delta.
static bool test_hex16_within_delta_wraparound_false() {
    return !hex16WithinDelta(1, 0xFFFF, 0x0000);
}

// Test 5: Large delta allows wrap-around true
// Verifies that a and b across the boundary are within a large delta.
static bool test_hex16_within_delta_large_delta_true() {
    return hex16WithinDelta(65535, 0x0000, 0xFFFF);
}

// Test 6: Additional true case in mid-range
// Verifies a straightforward within-delta scenario in mid-range values.
static bool test_hex16_within_delta_midrange_true() {
    return hex16WithinDelta(10, 60000, 60007);
}

// Execute all tests
int main() {
    std::vector<TestCase> tests = {
        { "test_hex16_within_delta_true_basic",      test_hex16_within_delta_true_basic },
        { "test_hex16_within_delta_false_case_focal",test_hex16_within_delta_false_case_focal },
        { "test_hex16_within_delta_zero_delta_same",  test_hex16_within_delta_zero_delta_same },
        { "test_hex16_within_delta_wraparound_false", test_hex16_within_delta_wraparound_false },
        { "test_hex16_within_delta_large_delta_true", test_hex16_within_delta_large_delta_true },
        { "test_hex16_within_delta_midrange_true",    test_hex16_within_delta_midrange_true },
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running " << total << " hex16WithinDelta tests...\n";

    for (const auto& t : tests) {
        bool ok = t.function();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << std::setw(55) << std::left << t.name << "\n";
        } else {
            std::cout << "[FAIL] " << std::setw(55) << std::left << t.name << "\n";
        }
    }

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";
    if (passed == total) {
        std::cout << "ALL HEX16 WITHIN DELTA tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED. See above for details.\n";
        return 1;
    }
}