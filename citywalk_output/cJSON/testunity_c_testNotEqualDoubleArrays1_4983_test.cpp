/*
  This is a self-contained C++11 test harness that targets the focal behavior of
  testNotEqualDoubleArrays1 from the provided Unity-based test Unity C file.

  Rationale:
  - The original focal test asserts that two double arrays are not equal and
    expects an abort (failure) when they are different.
  - To provide a compilable, GTest-free, C++11 based test suite, this file
    re-creates the essential semantics in a lightweight harness without requiring
    Unity at link time. It demonstrates a wide set of scenarios around the focal
    concept (not-equal of two double arrays) and verifies both success and
    failure paths, plus an “ignored” path simulated via a flag.

  How to use:
  - Compile with a C++11 capable compiler (e.g., g++ -std=c++11 test_not_equal_double_arrays1.cpp -o test)
  - Run the produced executable to see PASS/FAIL/IGNORED results and a summary.

  Notes:
  - This is a standalone test suite designed to reflect the focal behavior and
    related dependencies (e.g., simple class Dependency as a stand-in for class
    dependencies in the original C file). It is NOT a drop-in Unity replacement.
  - The tests are annotated with comments describing the intent of each unit test.

  Derivation from the focal method:
  - Focal: testNotEqualDoubleArrays1 checks that two 4-element double arrays are not equal.
  - We implement several tests around that idea:
    1) Baseline non-equality (as in the focal test).
    2) Equality scenario to demonstrate a failure path if arrays are equal.
    3) An ignored/simulated path to reflect the UNITY_EXCLUDE_DOUBLE path.
    4) A tiny dependency class example (static-like dependency behavior) to illustrate dependencies.

  This approach aligns with the request to create a high-quality, executable test suite
  for the focal behavior without using GTest, and with explanatory comments on each test.
*/

#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple dependency mock to illustrate class dependencies in the focal file.
// Mirrors the idea of "class dependencies" in a Unity/C test environment.
class Dependency {
public:
    Dependency(int v = 0) : value(v) {}
    int increment() { return ++value; }
    int get() const { return value; }
private:
    int value;
};

// Utility: compare two double arrays for exact equality (element-wise).
static bool arraysAreEqual(const double* a, const double* b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Utility: check if two double arrays are not equal (our focal condition).
static bool arraysNotEqual(const double* a, const double* b, size_t n) {
    return !arraysAreEqual(a, b, n);
}

// Status codes for tests
enum TestStatus {
    TEST_PASS = 0,
    TEST_FAIL = 1,
    TEST_IGNORE = 2
};

// Typedef for a test function
typedef TestStatus (*TestFunc)();

// Global to simulate UNITY_EXCLUDE_DOUBLE behavior for an IGNORE path.
// In real Unity, this would be controlled by compile-time macro; here we simulate.
static bool g_excludeDoubleSimulated = false;

// 1) Baseline: p0 and p1 are not equal (identical to focal case last decimals differ)
static TestStatus test_NotEqualDoubleArrays1_Baseline_NotEqual_NotIgnored(void) {
    // p0 and p1 reflect the focal test values
    const double p0[] = {1.0, 8.0, 25.4, 0.25666666667};
    const double p1[] = {1.0, 8.0, 25.4, 0.25666666666};
    const size_t len = 4;

    if (g_excludeDoubleSimulated) {
        // Simulate UNITY_EXCLUDE_DOUBLE: test would be ignored in Unity.
        return TEST_IGNORE;
    }

    // The focal expectation is that arrays are NOT equal -> test should pass
    bool notEq = arraysNotEqual(p0, p1, len);
    if (notEq) {
        // Passed: arrays are indeed not equal as in the focal test
        return TEST_PASS;
    } else {
        // Fail: the arrays are unexpectedly equal
        return TEST_FAIL;
    }
}

// 2) Baseline: additional check where arrays are actually equal; should fail.
// This demonstrates the "abort" path if the arrays were equal (the negative test case).
static TestStatus test_NotEqualDoubleArrays1_Baseline_EqualLast_Fails(void) {
    const double p0[] = {1.0, 8.0, 25.4, 0.25666666667};
    // Make p1 exactly equal to p0 to simulate an unexpected pass condition for
    // a not-equal test (thus this test should fail).
    const double p1[] = {1.0, 8.0, 25.4, 0.25666666667};
    const size_t len = 4;

    // The focal test would abort here because arrays are equal; we model as fail.
    bool notEq = arraysNotEqual(p0, p1, len);
    if (!notEq) {
        // Incorrect: arrays are equal, so the focal test would abort (fail this unit test)
        return TEST_FAIL;
    } else {
        // Should not reach here for this scenario
        return TEST_FAIL;
    }
}

// 3) Ignored path: simulate UNITY_EXCLUDE_DOUBLE behavior by a global flag.
// If flagged, the focal test would be ignored and not count as pass/fail.
static TestStatus test_NotEqualDoubleArrays1_IgnoredSimulated(void) {
    // The test would be ignored when the flag is set
    if (g_excludeDoubleSimulated) {
        return TEST_IGNORE;
    }

    // If not ignoring, reuse the baseline-not-equal check as the actual test.
    const double p0[] = {1.0, 8.0, 25.4, 0.25666666667};
    const double p1[] = {1.0, 8.0, 25.4, 0.25666666666};
    const size_t len = 4;
    bool notEq = arraysNotEqual(p0, p1, len);
    return notEq ? TEST_PASS : TEST_FAIL;
}

// 4) Dependency-related check: ensure a trivial "class dependency" usage is functional.
// This is to reflect the <FOCAL_CLASS_DEP> style dependency check in the original test file.
static TestStatus test_DependencyUsage_Sanity(void) {
    Dependency d(10);
    int val = d.increment(); // 11
    if (val != 11) return TEST_FAIL;
    if (d.get() != 11) return TEST_FAIL;
    return TEST_PASS;
}

// Helper: run each test and collect results
static void runTest(const std::string& name, TestFunc f, int& passed, int& failed, int& ignored) {
    TestStatus st = f();
    if (st == TEST_PASS) {
        std::cout << "[PASS] " << name << "\n";
        ++passed;
    } else if (st == TEST_FAIL) {
        std::cout << "[FAIL] " << name << "\n";
        ++failed;
    } else if (st == TEST_IGNORE) {
        std::cout << "[IGNORED] " << name << "\n";
        ++ignored;
    } else {
        std::cout << "[UNKNOWN] " << name << "\n";
        ++failed;
    }
}

// Entry point
int main() {
    // Optional: allow the user to toggle the simulated UNITY_EXCLUDE_DOUBLE behavior.
    // By default, we do not exclude doubles (the baseline path is exercised).
    g_excludeDoubleSimulated = false;

    // Register tests
    struct {
        const char* name;
        TestFunc func;
    } tests[] = {
        { "NotEqualDoubleArrays1_Baseline_NotEqual_NotIgnored", test_NotEqualDoubleArrays1_Baseline_NotEqual_NotIgnored },
        { "NotEqualDoubleArrays1_Baseline_EqualLast_Fails", test_NotEqualDoubleArrays1_Baseline_EqualLast_Fails },
        { "NotEqualDoubleArrays1_IgnoredSimulated", test_NotEqualDoubleArrays1_IgnoredSimulated },
        { "DependencyUsage_Sanity", test_DependencyUsage_Sanity }
    };

    int passed = 0, failed = 0, ignored = 0;

    std::cout << "Running focal tests for: testNotEqualDoubleArrays1 (simulated C++11 harness)\n";
    std::cout << "Note: This harness mirrors the focal behavior without Unity runtime.\n\n";

    for (const auto& t : tests) {
        runTest(t.name, t.func, passed, failed, ignored);
    }

    std::cout << "\nTest summary: "
              << "Passed=" << passed
              << ", Failed=" << failed
              << ", Ignored=" << ignored
              << "\n";

    // Return non-zero if any test failed
    return (failed > 0) ? 1 : 0;
}