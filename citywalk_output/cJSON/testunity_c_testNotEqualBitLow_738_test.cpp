// Lightweight C++11 test harness to exercise the focal testNotEqualBitLow behavior
// This harness mimics the intent of the Unity-based testNotEqualBitLow
// by using exception-driven abort semantics to verify expected aborts.
// Note: This is a self-contained stand-alone test suite and does not rely on GTest.
// It focuses on the core dependent behavior around "bit low" checks as implied by the focal test.
//
// The goal is to cover the two branches of the predicate "bit is low" for a 32-bit value:
// - When the targeted bit (30) is not low (i.e., is 1): abort is expected (testNotEqualBitLow_Fails...).
// - When the targeted bit (30) is low (i.e., is 0): abort should NOT occur (testNotEqualBitLow_Passes...).
//
// This approach provides a compact, portable unit test suite suitable for C++11 environments.

#include <unity.h>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Core utility: determine if a specific bit in a 32-bit value is clear (0)
static bool isBitLow(uint32_t value, unsigned int bit)
{
    // Assuming bit indexing: 0 = LSB, 31 = MSB
    return ((value >> bit) & 0x1u) == 0u;
}

// Focal logic reproduction (simplified): simulate the Unity test expectation
// In the original Unity test, TEST_ASSERT_BIT_LOW(bit, value) would abort
// if the bit is not low. We replicate that with an exception to model "abort".
static void test_NotEqualBitLow_Fails_Impl()
{
    // Based on the focal method:
    // v0 = 0xFF55AA00; check bit 30
    // Bit 30 in 0xFF55AA00 is 1, so the bit is not low and the assertion should abort.
    uint32_t v0 = 0xFF55AA00u;
    if (!isBitLow(v0, 30)) {
        // Simulate Unity abort by throwing an exception
        throw std::runtime_error("Abort as TEST_ASSERT_BIT_LOW(30, v0) should fail");
    }
    // If the bit were low, we would fall through without abort (not the expected path for this test).
}

// Auxiliary test: a case where the bit is indeed low, so no abort should occur
static void test_NotEqualBitLow_Passes_Impl()
{
    // Choose a value where bit 30 is 0. For example, 0x80000000 has bit 31 = 1, bit 30 = 0.
    uint32_t v = 0x80000000u;
    if (!isBitLow(v, 30)) {
        // This path should not be taken because bit 30 is low (assert should pass)
        throw std::runtime_error("Abort unexpectedly when bit 30 is low");
    }
    // If no abort occurs, this test passes.
}

// Lightweight test framework (no external libraries)
struct TestCase {
    std::string name;
    std::function<void()> testFunc;
    bool expectThrow; // true if we expect the test to abort (throw)
};

// Execute a single test case, returning whether it passed
static bool runTestCase(const TestCase& tc)
{
    bool threw = false;
    try {
        tc.testFunc();
    } catch (...) {
        threw = true;
    }
    // A test passes if the actual abort-ness matches expectation
    return (threw == tc.expectThrow);
}

int main()
{
    // Register test cases inspired by the focal testNotEqualBitLow behavior.
    // We intentionally keep the suite small but representative of both branches.
    std::vector<TestCase> tests;

    // Test 1: NotEqualBitLow should abort when bit 30 is not low (i.e., high)
    // This corresponds to the original Unity testNotEqualBitLow in testunity.c.
    tests.push_back({
        "NotEqualBitLow_Fails_When_Bit30_High",
        []() { test_NotEqualBitLow_Fails_Impl(); },
        true // Expect an abort (exception)
    });

    // Test 2: NotEqualBitLow should not abort when bit 30 is low
    tests.push_back({
        "NotEqualBitLow_Passes_When_Bit30_Low",
        []() { test_NotEqualBitLow_Passes_Impl(); },
        false // Do not expect an abort
    });

    // Execute tests and report
    int total = static_cast<int>(tests.size());
    int passed = 0;
    for (const auto& t : tests) {
        bool ok = runTestCase(t);
        std::cout
            << "[Test] " << t.name << " -> " << (ok ? "PASS" : "FAIL") << std::endl;
        if (ok) ++passed;
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}