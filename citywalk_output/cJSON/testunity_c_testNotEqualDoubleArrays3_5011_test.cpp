// This C++11 test suite is handcrafted to exercise the focal method
// testNotEqualDoubleArrays3 from the provided testunity.c context.
// It does not rely on GTest or Unity at runtime. Instead, it provides a
// lightweight, self-contained testing harness to verify the core logic
// illustrated by the focal method: comparing two double arrays for equality
// and handling the "not equal" case as the focal test expects.
//
// Step 1 (Conceptual): Candidate Keywords derived from the focal method and its
// dependencies include:
// - testNotEqualDoubleArrays3, p0 and p1 double arrays, length 4
// - UNITY_EXCLUDE_DOUBLE macro controlling test execution (ignore path)
// - TEST_ASSERT_EQUAL_DOUBLE_ARRAY macro (abstracted here as a direct comparison)
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (simulated via non-aborting assertion)
// - The class dependency context (Unity framework) is not required for this
//   lightweight C++ test harness, but the logic is emulated to validate the idea.
//
// Step 2 & 3 (Implementation): Provide a minimal test harness and focused tests
// to validate the core behavior of the focal scenario. Explanatory comments
// accompany each unit test describing intent and coverage.

#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight assertion/notification helpers (non-terminating)
static bool g_currentTestFailed = false;
static std::string g_currentTestName;
static int g_totalFailures = 0;
static int g_totalTests = 0;

// Begin a test; initialize its context
void beginTest(const char* name) {
    g_currentTestFailed = false;
    g_currentTestName = name;
    ++g_totalTests;
    std::cout << "Running test: " << name << std::endl;
}

// End a test; report result and update global counters
void endTest() {
    if (g_currentTestFailed) {
        ++g_totalFailures;
        std::cout << "Result: FAIL\n" << std::endl;
    } else {
        std::cout << "Result: PASS\n" << std::endl;
    }
}

// Non-terminating assertion: report failure but continue
void expectBool(bool condition, const std::string& message) {
    if (!condition) {
        g_currentTestFailed = true;
        std::cerr << "  Assertion failed: " << message
                  << " [in " << g_currentTestName << "]" << std::endl;
    }
}

// Helper macros (non-terminating)
#define EXPECT_TRUE(cond)  expectBool((cond), "Expected true but was false")
#define EXPECT_FALSE(cond) expectBool(!(cond), "Expected false but was true")

// Core comparison utility (mimics the essence of TEST_ASSERT_EQUAL_DOUBLE_ARRAY)
bool equalDoubleArrays(const double* a, const double* b, std::size_t n) {
    if (a == nullptr || b == nullptr) {
        // If either is null, consider them not equal (except both null is handled by tests)
        return (a == b) && (n == 0);
    }
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Test 1: Validate that the focal scenario detects not-equality for the given p0 and p1.
// This mirrors the intended behavior of testNotEqualDoubleArrays3 where the arrays differ
// at index 2 (25.4 vs 25.5).
void testNotEqualDoubleArrays3_NotEqualScenario(void)
{
#if defined(UNITY_EXCLUDE_DOUBLE)
    // Simulate UNITY macro behavior: when double support is excluded, the test is ignored.
    std::cout << "Test testNotEqualDoubleArrays3_NotEqualScenario is ignored due to UNITY_EXCLUDE_DOUBLE.\n\n";
    return;
#else
    beginTest("testNotEqualDoubleArrays3_NotEqualScenario");
    double p0[] = {1.0, 8.0, 25.4, 0.253};
    double p1[] = {1.0, 8.0, 25.5, 0.253}; // differs at index 2
    bool areEqual = equalDoubleArrays(p0, p1, 4);
    // The focal test expects not-equal; thus we assert that they are not equal.
    EXPECT_FALSE(areEqual); // non-equality should be detected
    endTest();
#endif
}

// Test 2: Validate the opposite path: when the arrays are actually equal, the equality
// check should yield true. This covers the "true" branch for the underlying comparison
// logic illustrated by the focal method's context.
void testNotEqualDoubleArrays3_EqualCase(void)
{
#if defined(UNITY_EXCLUDE_DOUBLE)
    // If double support is excluded, skip the test as per macro semantics.
    std::cout << "Test testNotEqualDoubleArrays3_EqualCase is ignored due to UNITY_EXCLUDE_DOUBLE.\n\n";
    return;
#else
    beginTest("testNotEqualDoubleArrays3_EqualCase");
    double p0[] = {1.0, 8.0, 25.4, 0.253};
    double p1[] = {1.0, 8.0, 25.4, 0.253}; // identical to p0
    bool areEqual = equalDoubleArrays(p0, p1, 4);
    // The arrays are equal; thus the non-abort assertion would fail in Unity.
    // Here we verify that the equality check correctly reports equality.
    EXPECT_TRUE(areEqual);
    endTest();
#endif
}

// Optional: additional coverage by simulating the "exclude" path and ensuring no crash.
void testNotEqualDoubleArrays3_ExcludePath(void)
{
#if defined(UNITY_EXCLUDE_DOUBLE)
    // This test will be a no-op to simulate exclusion behavior.
    beginTest("testNotEqualDoubleArrays3_ExcludePath (skipped)");
    endTest();
#else
    // If not excluded, run the Not Equal scenario as in Test 1.
    testNotEqualDoubleArrays3_NotEqualScenario();
#endif
}

// Main entry to run the lightweight test suite
int main() {
    std::cout << "Starting lightweight test suite for focal method: testNotEqualDoubleArrays3\n" << std::endl;

    // Run tests (covering not-equal path and equal path)
    testNotEqualDoubleArrays3_NotEqualScenario();
    testNotEqualDoubleArrays3_EqualCase();
    testNotEqualDoubleArrays3_ExcludePath();

    // Summary
    std::cout << "Test run complete." << std::endl;
    std::cout << "Total tests: " << g_totalTests
              << ", Failures: " << g_totalFailures << std::endl;

    // Return non-zero if any test failed
    return (g_totalFailures > 0) ? 1 : 0;
}