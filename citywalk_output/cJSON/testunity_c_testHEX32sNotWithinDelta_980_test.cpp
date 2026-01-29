// Test suite for the focal behavior around testHEX32sNotWithinDelta
// This standalone C++11 test harness mirrors the essential logic of the
// Unity-based test that asserts HEX32 values are NOT within a given delta.
// It provides explanations for each test, and avoids depending on GTest.
// Note: The original Unity tests rely on aborting behavior (EXPECT_ABORT_BEGIN),
// here we emulate equivalent logical checks to ensure the core functionality
// (within-delta vs. not-within-delta) is exercised in a portable way.

#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Candidate Keywords mapping (for Step 1 understanding):
// - HEX32_WITHIN / HEX32 difference checks
// - delta (allowed difference)
// - NOT_WITHIN (expect not within delta)
// - 32-bit unsigned arithmetic
// - edge cases with large values (e.g., 2147483647, 2147483649)

// Underlying helper: checks if two 32-bit unsigned values are within a given delta.
static bool hex32_within(uint32_t delta, uint32_t a, uint32_t b) {
    // Compute absolute difference in unsigned space without overflow by using a and b as uint32_t
    uint32_t diff = (a > b) ? (a - b) : (b - a);
    return diff <= delta;
}

// Test 1: Basic not-within-delta scenario (mirrors original testHEX32sNotWithinDelta)
static bool testHEX32sNotWithinDelta_basic_case(void) {
    // delta = 1, values are 2147483647 and 2147483649
    // Difference = 2, which is greater than delta -> NOT within
    bool within = hex32_within(1u, 2147483647u, 2147483649u);
    return !within; // expected: true (i.e., test passes)
}

// Test 2: Not-within-delta with a custom message path (conceptual; exact Unity message path not reproduced)
static bool testHEX32sNotWithinDelta_andCustomMessage_case(void) {
    // Using the same inputs as Test 1 to illustrate handling of a "custom message" scenario
    bool within = hex32_within(1u, 2147483647u, 2147483649u);
    return !within; // expected: true
}

// Test 3: Not-within-delta where an unsigned value corresponds to a signed edge case
// In unsigned space, -1 becomes 0xFFFFFFFF; difference with 1 is large and not within delta.
static bool testHEX32sNotWithinDelta_EvenThoughASignedIntWouldPass_case(void) {
    // delta = 5, a = 1, b = -1 (as uint32_t => 0xFFFFFFFF)
    bool within = hex32_within(5u, 1u, static_cast<uint32_t>(-1));
    // In unsigned arithmetic this is not within delta
    return !within; // expected: true
}

// Test 4: Not-within-delta with a signed edge case and custom message path
static bool testHEX32sNotWithinDelta_EvenThoughASignedIntWouldPassAndCustomMessage_case(void) {
    // Same inputs as Test 3 to illustrate a "custom message" path
    bool within = hex32_within(5u, 1u, static_cast<uint32_t>(-1));
    return !within; // expected: true
}

// Simple harness to run tests and report results without GTest or Unity
int main(void) {
    using TestFn = bool (*)();

    struct TestCase {
        std::string name;
        TestFn      func;
    };

    TestCase tests[] = {
        { "testHEX32sNotWithinDelta_basic_case",                          testHEX32sNotWithinDelta_basic_case },
        { "testHEX32sNotWithinDelta_andCustomMessage_case",                 testHEX32sNotWithinDelta_andCustomMessage_case },
        { "testHEX32sNotWithinDelta_EvenThoughASignedIntWouldPass_case",   testHEX32sNotWithinDelta_EvenThoughASignedIntWouldPass_case },
        { "testHEX32sNotWithinDelta_EvenThoughASignedIntWouldPassAndCustomMessage_case", testHEX32sNotWithinDelta_EvenThoughASignedIntWouldPassAndCustomMessage_case }
    };

    int failures = 0;
    std::cout << "Running HEX32 not-within-delta test suite (standalone harness)" << std::endl;

    for (const auto& t : tests) {
        bool result = false;
        // Guard against potential runtime exceptions in future expansions
        try {
            result = t.func();
        } catch (...) {
            result = false;
        }
        if (result) {
            std::cout << "[PASS] " << t.name << std::endl;
        } else {
            std::cout << "[FAIL] " << t.name << std::endl;
            ++failures;
        }
    }

    if (failures) {
        std::cerr << "Some tests failed. Failures: " << failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}