// C++11 based lightweight unit test suite for the focal logic of UINT32 within-delta checks.
// This replaces the original Unity-based test harness for the specific focus on UINT32 within delta.
//
// Note: This test suite is self-contained and does not depend on GTest or Unity.
// It mirrors the essential behavior of the focal test UINT32sWithinDelta by validating
// the within-delta predicate across representative scenarios, including edge cases.

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight predicate used by the focal test.
// Returns true if |actual - expected| <= delta, using 64-bit arithmetic to avoid overflow.
static bool within_uint32_within_delta(uint32_t delta, uint32_t expected, uint32_t actual) {
    uint64_t diff = (actual >= expected) ? static_cast<uint64_t>(actual) - static_cast<uint64_t>(expected)
                                       : static_cast<uint64_t>(expected) - static_cast<uint64_t>(actual);
    return diff <= static_cast<uint64_t>(delta);
}

// Global test registry
static std::vector<std::pair<std::string, std::function<bool()>>> g_tests;

// Helper to register a test
static void register_test(const std::string& name, std::function<bool()> func) {
    g_tests.emplace_back(name, func);
}

// Focal test: recreates the essence of testUINT32sWithinDelta from testunity.c
// This test verifies three within-delta checks using the exact values from the focal method.
static bool testUINT32sWithinDelta_focal() {
    // True cases: diff <= delta
    bool ok = within_uint32_within_delta(1, 5000, 5001) &&  // diff = 1
              within_uint32_within_delta(5, 5000, 4996) &&  // diff = 4
              within_uint32_within_delta(5, 5000, 5005);    // diff = 5
    return ok;
}

// Test 1: Fail when delta is too small for the given pair
// This exercises the false branch of the predicate.
static bool testUINT32sWithinDelta_Fail1() {
    // diff = 2, delta = 1 -> should fail
    return !within_uint32_within_delta(1, 5000, 5002);
}

// Test 2: Boundary condition where diff equals delta (should pass)
static bool testUINT32sWithinDelta_BoundaryPass() {
    // diff = 5, delta = 5 -> should pass
    return within_uint32_within_delta(5, 100, 105);
}

// Test 3: Zero delta with non-equal values should fail
static bool testUINT32sWithinDelta_ZeroDeltaFail() {
    // diff = 1, delta = 0 -> should fail
    return !within_uint32_within_delta(0, 10, 11);
}

// Test 4: Large-number boundary where only a single-unit difference is tested
static bool testUINT32sWithinDelta_MaxEdgePass() {
    // Use near-UINT32_MAX values to ensure no overflow in diff computation
    uint32_t expected = 0xFFFFFFFEu; // 4294967294
    uint32_t actual   = 0xFFFFFFFFu; // 4294967295
    return within_uint32_within_delta(1, expected, actual); // diff = 1 -> pass
}

// Main driver
int main() {
    // Register tests (name, test-function)
    register_test("testUINT32sWithinDelta_focal", testUINT32sWithinDelta_focal);
    register_test("testUINT32sWithinDelta_Fail1", testUINT32sWithinDelta_Fail1);
    register_test("testUINT32sWithinDelta_BoundaryPass", testUINT32sWithinDelta_BoundaryPass);
    register_test("testUINT32sWithinDelta_ZeroDeltaFail", testUINT32sWithinDelta_ZeroDeltaFail);
    register_test("testUINT32sWithinDelta_MaxEdgePass", testUINT32sWithinDelta_MaxEdgePass);

    // Run tests
    int failures = 0;
    for (const auto& t : g_tests) {
        bool result = t.second();
        std::cout << t.first << ": " << (result ? "PASS" : "FAIL") << std::endl;
        if (!result) ++failures;
    }

    if (failures != 0) {
        std::cout << "Total FAILURES: " << failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    }
}