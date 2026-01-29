/*
  Lightweight C++11 test harness to exercise Not Equal HEX Each Equal scenarios
  inspired by the Unity-based testNotEqualHEXEachEqual1 from testunity.c.

  This file provides a small, self-contained test framework (no GTest) and
  implements three unit tests that mirror the intended behavior of the
  Unity macros TEST_ASSERT_EACH_EQUAL_HEX32 and the abort-on-failure pattern.

  Step 1 (Program Understanding) and Step 2 (Unit Test Generation) guided the
  creation of a concise, focused suite around the focal concept:
  - Candidate Keywords: TEST_ASSERT_EACH_EQUAL_HEX32, UNITY_UINT32 (uint32_t),
    expected-mismatch abort behavior, NOT_EQUAL HEX32 scenario.

  Step 3 (Test Refinement) is addressed by adding clear comments to each test
  and ensuring deterministic pass/fail reporting without terminating the entire
  test run on a single failure.

  Notes:
  - The tests are written to run in a C++11 environment.
  - No external test framework is used; a minimal runner is provided.
  - The tests simulate the Unity-style abort-on-failure behavior by throwing
    a lightweight exception when a test would abort.
*/

#include <unity.h>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


namespace TinyTestHarness {

    // Lightweight exception to simulate an assertion abort in Unity-like tests
    struct TestAbort : public std::exception {
        const char* what() const noexcept override { return "Test aborted due to assertion failure"; }
    };

    // Minimal assertion helper that replicates the core behavior:
    // If any element != expected, abort (throw).
    void TEST_ASSERT_EACH_EQUAL_HEX32(uint32_t expected, const uint32_t* arr, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            if (arr[i] != expected) {
                throw TestAbort();
            }
        }
    }

    // Convenience alias for readability in tests
    using TestFunc = void(*)();

    // Simple test registry
    struct TestCase {
        std::string name;
        TestFunc    func;
    };

    // Storage for registered tests
    static TestCase g_tests[3];
    static size_t   g_testCount = 0;

    void registerTest(const std::string& name, TestFunc f) {
        if (g_testCount < sizeof(g_tests)/sizeof(g_tests[0])) {
            g_tests[g_testCount++] = { name, f };
        }
    }

    // Forward declarations of focal tests (mirror Unity Not Equal HEX Easy cases)
    void testNotEqualHEXEachEqual1();
    void testNotEqualHEXEachEqual2();
    void testNotEqualHEXEachEqual3();

    // Helper to run all tests and report results
    int runAllTests() {
        int failed = 0;
        for (size_t i = 0; i < g_testCount; ++i) {
            const auto& t = g_tests[i];
            try {
                t.func();
                std::cout << "[PASS] " << t.name << "\n";
            } catch (const TestAbort&) {
                // Expected abort by design of NOT EQUAL assertions
                std::cout << "[ABORTED] " << t.name << " (abort occurred as expected)\n";
            } catch (...) {
                std::cout << "[FAIL] " << t.name << " (unexpected exception)\n";
                ++failed;
            }
        }
        return failed;
    }

} // namespace TinyTestHarness

// Place tests in the global namespace but register them within main via TinyTestHarness
using namespace TinyTestHarness;

// Implement the three test variants mirroring Unity's testNotEqualHEXEachEqual1/2/3
// Test 1: The array has a first element that differs from the expected HEX32 value.
// Expected result: aborts when mismatch is detected.
void testNotEqualHEXEachEqual1() {
    // Focal scenario from Unity:
    //  UNITY_UINT32 p0[] = {1, 65132u, 65132u, 65132u};
    //  TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
    // Expect abort because p0[0] != 65132
    uint32_t p0[] = {1, 65132u, 65132u, 65132u};
    try {
        TEST_ASSERT_EACH_EQUAL_HEX32(65132u, p0, 4);
        // If no abort occurred, this test should fail
        throw std::runtime_error("TestNotEqualHEXEachEqual1 failed: no abort on mismatch");
    } catch (const TestAbort&) {
        // Expected abort, test passes
    } catch (...) {
        // Any other exception means a test failure
        throw;
    }
}

// Test 2: The array has a different value in the last element (mismatch not at index 0).
// Expected result: aborts when mismatch is detected.
void testNotEqualHEXEachEqual2() {
    // Focal scenario:
    //  UNITY_UINT32 p0[] = {987, 987, 987, 65132u};
    //  TEST_ASSERT_EACH_EQUAL_HEX32(987, p0, 4);
    uint32_t p0[] = {987, 987, 987, 65132u};
    try {
        TEST_ASSERT_EACH_EQUAL_HEX32(987u, p0, 4);
        throw std::runtime_error("TestNotEqualHEXEachEqual2 failed: no abort on mismatch");
    } catch (const TestAbort&) {
        // Expected abort
    } catch (...) {
        throw;
    }
}

// Test 3: The array has a middle element that differs from the expected value.
// Expected result: aborts when mismatch is detected.
void testNotEqualHEXEachEqual3() {
    // Focal scenario:
    //  UNITY_UINT32 p0[] = {8, 8, 987, 8};
    //  TEST_ASSERT_EACH_EQUAL_HEX32(8, p0, 4);
    uint32_t p0[] = {8, 8, 987, 8};
    try {
        TEST_ASSERT_EACH_EQUAL_HEX32(8u, p0, 4);
        throw std::runtime_error("TestNotEqualHEXEachEqual3 failed: no abort on mismatch");
    } catch (const TestAbort&) {
        // Expected abort
    } catch (...) {
        throw;
    }
}

// Register tests
static void registerAllTests() {
    registerTest("testNotEqualHEXEachEqual1", testNotEqualHEXEachEqual1);
    registerTest("testNotEqualHEXEachEqual2", testNotEqualHEXEachEqual2);
    registerTest("testNotEqualHEXEachEqual3", testNotEqualHEXEachEqual3);
}

int main() {
    // Initialize and register tests
    registerAllTests();

    // Run and report
    int failures = runAllTests();

    // Return non-zero if failures occurred
    return failures > 0 ? 1 : 0;
}

/* Notes on test coverage and reasoning:
   - The three tests exercise the negative/abort path when not all elements equal
     the expected HEX32 value, aligning with the Unity macro behavior shown in
     testNotEqualHEXEachEqual1.
   - We intentionally use exceptions to simulate abort-like behavior so that the
     test runner can continue executing other tests (non-terminating behavior).
   - The tests use only standard C++11 facilities, and do not rely on GTest or
     other external frameworks, satisfying the project constraints.
*/