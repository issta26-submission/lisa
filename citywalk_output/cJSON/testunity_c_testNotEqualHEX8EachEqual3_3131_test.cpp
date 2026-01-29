// Minimal C++11 based test harness (no GTest) focused on the focal scenario:
// Replicates the essence of the Unity test "testNotEqualHEX8EachEqual3" by
// providing pure C++ tests that model whether the "TEST_ASSERT_EACH_EQUAL_HEX8"
// style check would fail for a given input.
// The goal is to achieve high-coverage for the not-equal path without depending on Unity.

#include <cstddef>
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


// Candidate keyword mapping (Step 1): core components for the focal method
// - HEX8 comparison across an array
// - The notion of "each element equals" vs. not equal
// - The concept of an expected failure when a mismatch exists

// Helper: simulate whether an "each equal" hex8 assertion would fail given a target
// value and an array of pck values (unsigned 8-bit).
// In Unity, TEST_ASSERT_EACH_EQUAL_HEX8 would abort on the first mismatch.
// Here, we return true if a mismatch exists (i.e., the assertion would fail),
// and false if all elements are equal to the target.
static bool wouldFailIfEachEqualHEX8(uint8_t target, const uint8_t* arr, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        if (arr[i] != target)
        {
            // Found a mismatch -> would fail the assertion in Unity
            return true;
        }
    }
    // All elements match -> assertion would pass
    return false;
}

// Focal test impls (Step 2 / Step 3): create isolated tests that exercise
// the not-equal path for TEST_ASSERT_EACH_EQUAL_HEX8(8, ...).
// Each test is annotated with comments describing intent and coverage.

static void testNotEqualHEX8EachEqual3_ShouldFail_impl()
{
    // Arrange: input array with a mismatch (first element is 1, not 8)
    const uint8_t p0[] = {1, 8, 8, 8};
    // Act: determine whether the assertion would fail
    bool wouldFail = wouldFailIfEachEqualHEX8(8, p0, 4);
    // Assert: For a negative test, we expect a failure (wouldFail == true)
    if (!wouldFail)
    {
        throw std::runtime_error("Expected assertion to fail for testNotEqualHEX8EachEqual3, but it did not.");
    }
}

// Explanation: This is a negative test: the array contains a non-matching element,
// so the "each equal to 8" condition should fail (abort in Unity terms).
// Covering the not-equal path ensures the false branch of the core predicate is hit.

static void testNotEqualHEX8EachEqual3_AllMatchPass_impl()
{
    // Arrange: input array with all elements equal to 8
    const uint8_t p1[] = {8, 8, 8, 8};
    // Act: determine whether the assertion would fail
    bool wouldFail = wouldFailIfEachEqualHEX8(8, p1, 4);
    // Assert: For this positive test, the assertion should pass (wouldFail == false)
    if (wouldFail)
    {
        throw std::runtime_error("Did not expect failure for testNotEqualHEX8EachEqual3_AllMatchPass.");
    }
}

// Explanation: Verifies that the all-matching case does not trigger a failure path,
// thus ensuring the true-branch (pass path) of the predicate is exercised.

static void testNotEqualHEX8EachEqual3_EmptyArray_impl()
{
    // Arrange: empty array should not trigger a mismatch as there are no elements
    const uint8_t* pEmpty = nullptr;
    // Act: wouldFail on zero-length input should be false (no elements to contradict)
    bool wouldFail = wouldFailIfEachEqualHEX8(8, pEmpty, 0);
    // Assert: Empty input should not cause a failure
    if (wouldFail)
    {
        throw std::runtime_error("Empty array should not cause a failure in TEST_ASSERT_EACH_EQUAL_HEX8.");
    }
}

// Simple test harness to run the above pure-C++ tests without relying on Unity.
struct SimpleTestRunner
{
    using TestFn = std::function<void()>;

    void addTest(const std::string& name, TestFn fn)
    {
        tests.emplace_back(Test{name, fn, false, ""});
    }

    int run()
    {
        int failures = 0;
        for (auto& t : tests)
        {
            try
            {
                t.fn();
                t.passed = true;
            }
            catch (const std::exception& e)
            {
                t.passed = false;
                t.message = e.what();
                ++failures;
            }
            // Report per-test result
            std::cout << (t.passed ? "[PASS] " : "[FAIL] ") << t.name;
            if (!t.passed && !t.message.empty())
            {
                std::cout << " - " << t.message;
            }
            std::cout << "\n";
        }
        return failures;
    }

private:
    struct Test
    {
        std::string name;
        TestFn      fn;
        bool        passed;
        std::string message;
    };

    std::vector<Test> tests;
};

// Test entry points (wrapper bindings for the harness)
static void runAllTests()
{
    SimpleTestRunner runner;

    // Register tests with explanatory comments
    // Test 1: negative path for Not Equal HEX8 Each Equal 3 (expected to fail)
    runner.addTest("testNotEqualHEX8EachEqual3_ShouldFail_impl",
                   testNotEqualHEX8EachEqual3_ShouldFail_impl);

    // Test 2: positive path where all elements are equal -> should pass
    runner.addTest("testNotEqualHEX8EachEqual3_AllMatchPass_impl",
                   testNotEqualHEX8EachEqual3_AllMatchPass_impl);

    // Test 3: empty array path -> should not fail
    runner.addTest("testNotEqualHEX8EachEqual3_EmptyArray_impl",
                   testNotEqualHEX8EachEqual3_EmptyArray_impl);

    // Execute and report
    int failures = runner.run();

    // Finalize based on outcomes
    if (failures > 0)
    {
        std::cerr << "Some tests FAILED. Total failures: " << failures << std::endl;
    }
    else
    {
        std::cout << "All tests PASSED." << std::endl;
    }
}

// Main: drive the test suite (no external test framework required)
int main()
{
    runAllTests();
    return 0;
}