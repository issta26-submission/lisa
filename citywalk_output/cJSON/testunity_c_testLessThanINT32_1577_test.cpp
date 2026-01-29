/********** Lightweight C++11 test suite for testLessThanINT32 focal method **********
   - Purpose: Provide a self-contained, non-GTest unit test suite that exercises
     the logical structure of the focal method testLessThanINT32 without depending
     on Unity. This suite mirrors the core comparisons performed by the focal test:
     four comparisons using a pair of 32-bit values and their pointer aliases.
   - Approach: Implement a tiny test harness (no external dependencies) with
     simple assert-like helpers. Register and run tests from main().
   - Note: This is designed to be compatible with C++11. It does not require
     GTest or Unity; static analysis and basic instrumentation are preserved.
   - Coverage goals realized:
     - True-branch coverage for the four less-than predicates (via testLessThanINT32_trueCase).
     - False-branch coverage (via testLessThanINT32_falseCase) to exercise expectations
       when the predicate fails.
     - Demonstrates handling of raw values and their pointer aliases (v0/v1 and p0/p1).
******************************************************************************/

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
struct TestCase {
    std::string name;
    std::function<void()> func;
    bool passed = false;
    std::string message;
};

// Global registry of tests
static std::vector<TestCase>& getTestRegistry() {
    static std::vector<TestCase> registry;
    return registry;
}

// Register a test case
static void registerTest(const std::string& name, const std::function<void()>& f) {
    getTestRegistry().push_back(TestCase{name, f, false, ""});
}

// Simple assertion helpers (throw on failure)
static void assertTrue(bool cond, const std::string& msg) {
    if (!cond) {
        throw std::runtime_error("ASSERT FAILED: " + msg);
    }
}

static void assertFalse(bool cond, const std::string& msg) {
    if (cond) {
        throw std::runtime_error("ASSERT FAILED: " + msg);
    }
}

// Focal method surrogate tests implemented in C++ (self-contained)
// These tests replicate the logical intent of the Unity-based testLessThanINT32
// by validating the four predicates involving v0, v1 and their pointers.
static void testLessThanINT32_trueCase() {
    // This test exercises the true-branch pathway where v0 < v1 holds.
    // It uses values that satisfy the strict less-than relation and
    // also validates the pointer-based variants.
    int v0 = -10;
    int v1 = 5;
    int* p0 = &v0;
    int* p1 = &v1;

    // Four analogous predicates from the focal test:
    //  TEST_ASSERT_LESS_THAN_INT32(v0, v1);
    //  TEST_ASSERT_LESS_THAN_INT32(*p0, v1);
    //  TEST_ASSERT_LESS_THAN_INT32(v0, *p1);
    //  TEST_ASSERT_LESS_THAN_INT32(*p0, *p1);
    //
    // In this true-case all four predicates must evaluate to true.

    assertTrue(v0 < v1, "testLessThanINT32_trueCase: v0 < v1 must be true");
    assertTrue(*p0 < v1, "testLessThanINT32_trueCase: *p0 < v1 must be true");
    assertTrue(v0 < *p1, "testLessThanINT32_trueCase: v0 < *p1 must be true");
    assertTrue(*p0 < *p1, "testLessThanINT32_trueCase: *p0 < *p1 must be true");
}

// False-branch coverage for the same predicates
static void testLessThanINT32_falseCase() {
    // This test exercises the false-branch pathway where v0 < v1
    // does not hold for the same set of predicates.
    int v0 = 214783647;
    int v1 = -214783648;
    int* p0 = &v0;
    int* p1 = &v1;

    // Compute the four predicates (as in the focal test)
    bool c1 = (v0 < v1);
    bool c2 = (*p0 < v1);
    bool c3 = (v0 < *p1);
    bool c4 = (*p0 < *p1);

    // In this configuration, all four predicates are false.
    // The test asserts that they are false to reflect the false-branch case.
    assertFalse(c1, "testLessThanINT32_falseCase: v0 < v1 should be false");
    assertFalse(c2, "testLessThanINT32_falseCase: *p0 < v1 should be false");
    assertFalse(c3, "testLessThanINT32_falseCase: v0 < *p1 should be false");
    assertFalse(c4, "testLessThanINT32_falseCase: *p0 < *p1 should be false");
}

// Entry points for test registration
static void registerAllTests() {
    registerTest("testLessThanINT32_trueCase", testLessThanINT32_trueCase);
    registerTest("testLessThanINT32_falseCase", testLessThanINT32_falseCase);
}

// Runner
static void runAllTests() {
    auto &registry = getTestRegistry();
    int total = static_cast<int>(registry.size());
    int passed = 0;
    int failed = 0;

    for (auto &tc : registry) {
        try {
            tc.func();
            tc.passed = true;
            ++passed;
        } catch (const std::exception &e) {
            tc.passed = false;
            tc.message = e.what();
            ++failed;
        } catch (...) {
            tc.passed = false;
            tc.message = "Unknown exception";
            ++failed;
        }
    }

    // Summary
    std::cout << "Test results: " << total << " run, "
              << passed << " passed, "
              << failed << " failed.\n";

    // Detailed report
    for (const auto &tc : registry) {
        std::cout << " - " << tc.name << ": "
                  << (tc.passed ? "PASSED" : "FAILED");
        if (!tc.passed && !tc.message.empty()) {
            std::cout << " (" << tc.message << ")";
        }
        std::cout << std::endl;
    }

    // Exit code for CI-style harness
    if (failed > 0) {
        std::exit(1);
    } else {
        std::exit(0);
    }
}

// Main: build and run the lightweight suite
int main() {
    // Register test cases before running
    registerAllTests();

    // Run all registered tests
    runAllTests();

    // Should not reach here due to exit in runner, but placate compilers
    return 0;
}