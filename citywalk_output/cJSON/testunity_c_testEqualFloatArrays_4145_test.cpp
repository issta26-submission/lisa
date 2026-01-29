// C++11 test suite for the focal method: testEqualFloatArrays
// This suite provides a lightweight, non-terminating test harness
// to exercise equality checks on float arrays as exercised by the
// original Unity-based test in testunity.c. The tests are designed
// to run without GTest and rely only on the C++ standard library.

// Notes:
// - We implement a small equal_float_arrays helper that mirrors the
//   semantics observed in the focal Unity tests: two NULL pointers are
//   considered equal; a NULL vs non-NULL is not equal; when both are
//   non-NULL, compare the first `len` elements exactly.
// - We use non-terminating assertions to maximize code coverage.
// - The main function runs a fixed set of tests and reports a summary.

#include <unity.h>
#include <iostream>
#include <functional>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


namespace FloatArrayTest {

    // Current test name for logging
    static std::string g_currentTestName;

    // Track test results
    static int g_totalTests = 0;
    static int g_failedTests = 0;
    static int g_passedTests = 0;

    // Flag set when an assertion fails within a test (non-terminating)
    static bool g_testFailed = false;

    // Utility: Determine if two float arrays are equal for the first `len` elements.
    // Semantics:
    // - If both pointers are NULL, considered equal.
    // - If one is NULL and the other is not, considered not equal.
    // - If both non-NULL, compare element-wise up to `len` with exact equality.
    bool equal_float_arrays(const float* a, const float* b, std::size_t len) {
        if (a == nullptr && b == nullptr) {
            return true;
        }
        if (a == nullptr || b == nullptr) {
            return false;
        }
        for (std::size_t i = 0; i < len; ++i) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    // Non-terminating assertion helper
    void expect(bool condition, const std::string& message) {
        if (!condition) {
            g_testFailed = true;
            // Print diagnostic immediately but do not abort the test run
            std::cerr << "Failure in " << g_currentTestName << ": " << message << std::endl;
        }
    }

    // Public entry point to enable per-test result logging
    // Test 1: Basic equality checks
    void testEqualFloatArrays_BasicEqualityCases(void) {
        // Purpose: verify exact equality across various length checks
        // p0 is the baseline array
        float p0[] = {1.0f, -8.0f,  25.4f, -0.123f};
        float p1[] = {1.0f, -8.0f,  25.4f, -0.123f}; // identical copy
        float p2[] = {1.0f, -8.0f,  25.4f, -0.2f};   // last element differs
        float p3[] = {1.0f, -23.0f, 25.0f, -0.26f};  // different second element

        // True: identical on length 1
        expect(equal_float_arrays(p0, p0, 1), "p0 vs p0 with len=1 should be equal");
        // True: identical on full length
        expect(equal_float_arrays(p0, p0, 4), "p0 vs p0 with len=4 should be equal");
        // True: p0 and p1 are identical for full length
        expect(equal_float_arrays(p0, p1, 4), "p0 vs p1 with len=4 should be equal");
        // True: first 3 elements identical
        expect(equal_float_arrays(p0, p2, 3), "p0 vs p2 with len=3 should be equal (first 3 elements)");
        // True: first element identical
        expect(equal_float_arrays(p0, p3, 1), "p0 vs p3 with len=1 should be equal (first element)");
        // True: NULL vs NULL should be considered equal
        expect(equal_float_arrays(nullptr, nullptr, 1), "NULL vs NULL with len=1 should be equal");
        // No explicit false-case assertions here; the complementary tests cover false paths.
    }

    // Test 2: Mismatch should fail when full length is considered
    void testEqualFloatArrays_FailsOnFullMismatch(void) {
        // p0 baseline
        float p0[] = {1.0f, -8.0f,  25.4f, -0.123f};
        // p2 differs in the last element
        float p2[] = {1.0f, -8.0f,  25.4f, -0.2f};
        // Expect failure when comparing full length=4
        expect(!equal_float_arrays(p0, p2, 4), "p0 vs p2 with len=4 should not be equal (last element differs)");
    }

    // Test 3: NULL vs non-NULL should fail
    void testEqualFloatArrays_NullVsNonNull(void) {
        float p0[] = {1.0f, -8.0f, 25.4f, -0.123f};
        // NULL vs non-NULL should not be equal
        expect(!equal_float_arrays(p0, nullptr, 4), "Non-NULL vs NULL should not be equal");
        expect(!equal_float_arrays(nullptr, p0, 4), "NULL vs Non-NULL should not be equal");
    }

    // Simple test runner wrapper (non-terminating)
    void runTest(const std::string& name, void (*testFunc)(void)) {
        g_currentTestName = name;
        g_testFailed = false;
        testFunc();
        ++g_totalTests;
        if (g_testFailed) {
            ++g_failedTests;
        } else {
            ++g_passedTests;
        }
    }

    // Bridge to C-style function pointers for tests
    void testEqualFloatArrays_BasicEqualityCases_Wrapper(void) { testEqualFloatArrays_BasicEqualityCases(); }
    void testEqualFloatArrays_FailsOnFullMismatch_Wrapper(void) { testEqualFloatArrays_FailsOnFullMismatch(); }
    void testEqualFloatArrays_NullVsNonNull_Wrapper(void) { testEqualFloatArrays_NullVsNonNull(); }

    // Initialize all tests in a single suite function to keep output tidy
    void run_all_tests() {
        // The test names follow the Unity-style naming for traceability
        runTest("testEqualFloatArrays_BasicEqualityCases", testEqualFloatArrays_BasicEqualityCases_Wrapper);
        runTest("testEqualFloatArrays_FailsOnFullMismatch", testEqualFloatArrays_FailsOnFullMismatch_Wrapper);
        runTest("testEqualFloatArrays_NullVsNonNull", testEqualFloatArrays_NullVsNonNull_Wrapper);
    }

} // namespace FloatArrayTest

int main() {
    // Run the tests
    FloatArrayTest::run_all_tests();

    // Report summary
    std::cout << "Test Summary: Total=" << FloatArrayTest::g_totalTests
              << ", Passed=" << FloatArrayTest::g_passedTests
              << ", Failed=" << FloatArrayTest::g_failedTests << std::endl;

    // Return non-zero if any test failed
    return FloatArrayTest::g_failedTests;
}