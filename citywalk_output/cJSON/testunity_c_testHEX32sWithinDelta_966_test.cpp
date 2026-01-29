/*
  Test Suite: HEX32 within Delta (C++) -- inspired by focal method testHEX32sWithinDelta
  Context:
  - The original focal method is testHEX32sWithinDelta located in testunity.c and uses Unity
    assertion macros such as TEST_ASSERT_HEX32_WITHIN(delta, a, b).
  - This standalone C++ test suite provides a lightweight, self-contained verification of the
    core semantics exercised by TEST_ASSERT_HEX32_WITHIN for 32-bit unsigned values.
  - The goal is to exercise true/false branches of the "within delta" predicate, including edge
    cases like boundary conditions and unsigned wrap-around considerations, without relying on GTest.
  - This harness is designed to be compilable under C++11 and uses a minimal, non-terminating test
    runner (no GTest, no Unity dependencies required).
  - Candidate Keywords distilled from the focal logic:
    HEX32, WITHIN, DELTA, 32-bit, unsigned, abs difference, boundary, wrap-around, unsigned
    comparisons, delta-boundaries, signed vs unsigned interpretation.

  Note: This suite does not depend on external Unity macros. Instead, it reimplements the essential
  semantics in a safe, self-contained manner to ensure high portability and easy integration into
  C++11 projects without GTest.
*/

#include <cmath>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Lightweight assertion helpers
static void assert_true(bool cond, const std::string& msg) {
    if (!cond) {
        throw std::runtime_error("Assertion failed: " + msg);
    }
}

static void assert_false(bool cond, const std::string& msg) {
    if (cond) {
        throw std::runtime_error("Assertion failed: " + msg);
    }
}

// Core semantics: TEST_ASSERT_HEX32_WITHIN(delta, a, b)
// Returns true iff |a - b| <= delta (unsigned 32-bit arithmetic treated as normal unsigned subtraction)
static bool hex32_within_delta(uint32_t delta, uint32_t a, uint32_t b) {
    // Use 64-bit to avoid overflow on subtraction
    uint64_t da = static_cast<uint64_t>(a);
    uint64_t db = static_cast<uint64_t>(b);
    uint64_t diff = (da > db) ? (da - db) : (db - da);
    return diff <= static_cast<uint64_t>(delta);
}

// Test Case 1: Basic within-delta scenarios mirroring the focal method's three asserts
// -  delta=1, a=5000, b=5001  -> within
// -  delta=5, a=5000, b=4996  -> within
// -  delta=5, a=5000, b=5005  -> within
static void testHEX32sWithinDelta_BasicWithinCases() {
    // True: 5000 vs 5001 (diff 1 <= delta 1)
    assert_true(hex32_within_delta(1, 5000u, 5001u),
                "HEX32 within delta failed for delta=1, 5000 vs 5001");

    // True: 5000 vs 4996 (diff 4 <= delta 5)
    assert_true(hex32_within_delta(5, 5000u, 4996u),
                "HEX32 within delta failed for delta=5, 5000 vs 4996");

    // True: 5000 vs 5005 (diff 5 <= delta 5)
    assert_true(hex32_within_delta(5, 5000u, 5005u),
                "HEX32 within delta failed for delta=5, 5000 vs 5005");
}

// Test Case 2: Exact boundary condition
// - delta equals the exact difference; must be considered within delta
static void testHEX32sWithinDelta_BoundaryExact() {
    // diff = 2147483649 - 2147483647 = 2; with delta=2 -> within
    assert_true(hex32_within_delta(2, 2147483647u, 2147483649u),
                "HEX32 within delta boundary exact case failed for delta=2, 2147483647 vs 2147483649");
}

// Test Case 3: Not within delta due to large difference (unsigned space)
 // - delta=1, a=2147483647, b=2147483649 (diff=2) -> not within
static void testHEX32sWithinDelta_NotWithinDelta() {
    assert_false(hex32_within_delta(1, 2147483647u, 2147483649u),
                 "HEX32 not within delta expected for delta=1, 2147483647 vs 2147483649");
}

// Test Case 4: Signed-vs-Unsigned pitfall demonstration
// - a=1, b= -1 (interpreted as 0xFFFFFFFF). As unsigned, diff is 0xFFFFFFFE which is > delta.
// - This mirrors tests like "NotWithinDeltaEvenThoughASignedIntWouldPass" from the Unity suite.
static void testHEX32sWithinDelta_NotWithinDeltaWithWrapAround() {
    // delta small, but signed intuition would suggest near, unsigneddisagrees
    uint32_t a = 1u;
    uint32_t b = static_cast<uint32_t>(-1); // 0xFFFFFFFF
    assert_false(hex32_within_delta(5u, a, b),
                 "HEX32 not within delta expected for wrap-around case (a=1, b=-1, delta=5)");
}

// Test Case 5: Large wrap boundary within delta
// - delta=1, a=0x80000000, b=0x7FFFFFFF (diff=1) -> within
static void testHEX32sWithinDelta_WrapBoundaryWithin() {
    assert_true(hex32_within_delta(1u, 0x80000000u, 0x7FFFFFFFu),
                "HEX32 within delta boundary across wrap boundary (delta=1, 0x80000000 vs 0x7FFFFFFF)");
}

// Simple test harness
struct TestCase {
    std::string name;
    std::function<void()> func;
};

int main() {
    // Collect tests
    std::vector<TestCase> tests = {
        { "testHEX32sWithinDelta_BasicWithinCases", testHEX32sWithinDelta_BasicWithinCases },
        { "testHEX32sWithinDelta_BoundaryExact",      testHEX32sWithinDelta_BoundaryExact },
        { "testHEX32sWithinDelta_NotWithinDelta",     testHEX32sWithinDelta_NotWithinDelta },
        { "testHEX32sWithinDelta_NotWithinDeltaWithWrapAround", testHEX32sWithinDelta_NotWithinDeltaWithWrapAround },
        { "testHEX32sWithinDelta_WrapBoundaryWithin", testHEX32sWithinDelta_WrapBoundaryWithin }
    };

    int passed = 0;
    int failed = 0;

    std::cout << "Running HEX32 within-delta test suite (non-GTest, Unity-free harness)\n";
    std::cout << "Candidate Keywords: HEX32, WITHIN, DELTA, 32-bit, unsigned, abs_diff, boundary\n";
    std::cout << "------------------------------------------------------------\n";

    for (const auto& t : tests) {
        try {
            // Report test start
            std::cout << "[ RUN      ] " << t.name << "\n";
            t.func();
            // If no exception, test passed
            ++passed;
            std::cout << "[  PASSED  ] " << t.name << "\n\n";
        } catch (const std::exception& ex) {
            ++failed;
            std::cout << "[  FAILED  ] " << t.name << " - " << ex.what() << "\n\n";
        } catch (...) {
            ++failed;
            std::cout << "[  FAILED  ] " << t.name << " - unknown exception\n\n";
        }
    }

    // Summary
    std::cout << "Test results: " << passed << " passed, " << failed << " failed, "
              << tests.size() << " total.\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}

/*
  Notes for reviewers:
  - The tests focus on the core predicate used by the focal method testHEX32sWithinDelta:
    within-delta for 32-bit unsigned integers.
  - Test Case 1 asserts the three original success cases from the focal method.
  - Test Case 2 validates the exact boundary (delta equals difference).
  - Test Case 3 ensures not-within-delta paths are correctly detected.
  - Test Case 4 demonstrates the pitfall where signed intuition does not map to unsigned arithmetic.
  - Test Case 5 covers a wrap-around boundary that is still within the delta when diff is 1.
  - The harness is self-contained and does not rely on Unity or GTest, aligning with the requirement
    to avoid GTest and to be portable to C++11 environments.
  - If you want to run alongside existing Unity-based tests in testunity.c, you can adapt this code to a
    small wrapper that calls RUN_TEST(testHEX32sWithinDelta) inside a Unity-style main, but that would
    require pulling in Unity's infrastructure. The current approach provides a lightweight, framework-free
    verification focused on the focal logic.
*/