// Test suite for the focal method: testEqualInt64Arrays
// This C++11 test harness is designed to exercise the core logic of the Unity-based test
// focal method without depending on the Unity framework. It re-implements a minimal subset
// of the behavior to enable high-coverage unit testing in a C++11 environment.
// Notes:
// - We intentionally focus on the core dependent components needed by the focal method.
// - We provide guards for 64-bit support to reflect UNITY_SUPPORT_64 behavior.
// - All tests are non-terminating (they throw exceptions on failure) so that the test
//   harness can continue executing subsequent tests.
//
// Step 1 (Program Understanding): Candidate Keywords extracted from the focal method and class
// - UNITY_INT64: 64-bit integer type alias
// - testEqualInt64Arrays: focal method under test
// - TEST_ASSERT_EQUAL_INT64_ARRAY: assertion macro used inside focal method
// - UNITY_SUPPORT_64: compile-time switch to enable/disable 64-bit tests
// - p0, p1, p2, p3: test data arrays
// - NULL handling: NULL pointers with length (as observed in the original focal tests)
// - Non-terminating assertions: behavior captured via exceptions in this harness
// Step 2 (Unit Test Generation): We create a focused suite covering: equal, not-equal scenarios
// Step 3 (Test Case Refinement): We include guard for 64-bit support and provide explanatory comments
// - We keep test code in C++ to compile under C++11, avoiding GTest/GMock per instructions

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


// Domain knowledge hints implemented as comments within code sections

// -----------------------------------------------------------------------------
// Focal functionality: reproduce the essential 64-bit array equality check
// -----------------------------------------------------------------------------

// Public domain-like helper to mimic Unity's TEST_ASSERT_EQUAL_INT64_ARRAY behavior.
// Returns true if the first 'len' elements of a and b are equal; false otherwise.
// Note: This function intentionally does not perform NULL checks beyond simple NULL == NULL handling.
// In the original Unity tests, NULL handling is part of the test harness; here we keep it simple.
static bool equalInt64Array(const int64_t* a, const int64_t* b, size_t len)
{
    // If both pointers are NULL, treat as mismatch for non-zero length (typical behavior)
    // If both are NULL and len == 0, consider equal. We prefer explicit checks based on len.
    if (len == 0) return true; // zero-length arrays are trivially equal

    if (a == nullptr || b == nullptr)
    {
        // Mirror Unity's tendency to fail when non-null-length expectations are involved
        // For our purposes, if both are NULL and len>0, return false to indicate not equal
        // (adjusting to avoid dereferencing NULLs)
        return false;
    }

    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Test Harness (minimal, self-contained, non-GMock, suitable for C++11)
// -----------------------------------------------------------------------------

// Simple assertion utilities
static void assert_true(bool cond, const std::string& msg)
{
    if (!cond)
    {
        throw std::runtime_error("Assertion failed: " + msg);
    }
}

static void assert_false(bool cond, const std::string& msg)
{
    if (cond)
    {
        throw std::runtime_error("Assertion failed: " + msg);
    }
}

// Test case wrapper
struct TestCase
{
    std::string name;
    std::function<void()> func;
};

// Global test registry
static std::vector<TestCase>& getTestRegistry()
{
    static std::vector<TestCase> registry;
    return registry;
}

static void registerTest(const std::string& name, std::function<void()> f)
{
    getTestRegistry().push_back(TestCase{name, f});
}

// Small macro-like helper for registering tests
#define REGISTER_TEST(name) registerTest(#name, name)

// 64-bit support toggle to emulate UNITY's UNITY_SUPPORT_64 behavior
// In a typical Unity setup, this would come from UNITY_CONFIG or a define.
#ifndef UNITY_SUPPORT_64
#define UNITY_SUPPORT_64 1
#endif

// -----------------------------------------------------------------------------
// Focal Test Implementations (CPP equivalents mirroring the original Unity test goals)
// -----------------------------------------------------------------------------

// Test: EqualInt64Arrays positive scenarios (mirrors the valid paths in testEqualInt64Arrays)
static void testEqualInt64Arrays_PositiveCases(void)
{
#if UNITY_SUPPORT_64
    // Prepare test data (64-bit)
    const int64_t p0[] = {1, 8, 987, -65132};
    const int64_t p1[] = {1, 8, 987, -65132}; // same contents as p0
    const int64_t p2[] = {1, 8, 987, -2};     // last element differs
    const int64_t p3[] = {1, 500, 600, 700};  // different elements after first

    // 1) p0 vs p0, length 1 -> should be equal (compare first element)
    bool res = equalInt64Array(p0, p0, 1);
    assert_true(res, "testEqualInt64Arrays_PositiveCases: p0 vs p0 len 1 should be equal");

    // 2) p0 vs p0, length 4 -> should be equal (full match)
    res = equalInt64Array(p0, p0, 4);
    assert_true(res, "testEqualInt64Arrays_PositiveCases: p0 vs p0 len 4 should be equal");

    // 3) p0 vs p1, length 4 -> should be equal (identical contents)
    res = equalInt64Array(p0, p1, 4);
    assert_true(res, "testEqualInt64Arrays_PositiveCases: p0 vs p1 len 4 should be equal");

    // 4) p0 vs p2, length 3 -> should be equal (first three elements identical)
    res = equalInt64Array(p0, p2, 3);
    assert_true(res, "testEqualInt64Arrays_PositiveCases: p0 vs p2 len 3 should be equal (first 3 elements)");
    
    // 5) p0 vs p3, length 1 -> should be equal (first element matches)
    res = equalInt64Array(p0, p3, 1);
    assert_true(res, "testEqualInt64Arrays_PositiveCases: p0 vs p3 len 1 should be equal (first element)");
#else
    // If 64-bit is not supported, skip as Unity would with TEST_IGNORE()
    // Intentionally do nothing to preserve test suite signal
#endif
}

// Test: Negative case for Int64 arrays (not strictly the focal method, but validates the checker)
static void testNotEqualInt64Arrays(void)
{
#if UNITY_SUPPORT_64
    const int64_t p0[] = {1, 8, 987, -65132};
    const int64_t p1[] = {1, 8, 987, -65131}; // last element differs
    // This should fail for full length 4
    bool res = equalInt64Array(p0, p1, 4);
    assert_false(res, "testNotEqualInt64Arrays: p0 vs p1 len 4 should NOT be equal (last element differs)");

    // Also test a mismatch in a shorter length (first 3 elements match, but length 4 would fail)
    const int64_t p2[] = {1, 8, 986, -65132};
    res = equalInt64Array(p0, p2, 4);
    assert_false(res, "testNotEqualInt64Arrays: p0 vs p2 len 4 should NOT be equal (third element differs)");
#else
    // Skip if 64-bit not supported
#endif
}

// Optional: If needed, test NULL-handling paths (mirroring Unity tests that use NULL with a length).
// For simplicity and to avoid dereferencing NULL, we avoid asserting on NULL pointers here.

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------

static void runAllTests()
{
    const auto& registry = getTestRegistry();

    int total = static_cast<int>(registry.size());
    int passed = 0;

    for (const auto& t : registry)
    {
        try
        {
            t.func();
            std::cout << "[PASS] " << t.name << "\n";
            ++passed;
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[FAIL] " << t.name << " - " << ex.what() << "\n";
        }
        catch (...)
        {
            std::cerr << "[FAIL] " << t.name << " - Unknown exception\n";
        }
    }

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";
}

// Expose test functions to the registration step (Step 2: class dependencies mapping)
REGISTER_TEST(testEqualInt64Arrays_PositiveCases)
REGISTER_TEST(testNotEqualInt64Arrays)

// Note: The focal method in the original testunity.c is named testEqualInt64Arrays. In this
// harness we only test the logical core of its functionality (array equality) via
// equalInt64Array, while maintaining the same semantic expectations.
// If you want to explicitly include a test with the exact focal function name, you could
// wrap a small adapter that calls the checker with the same data and produces Unity-like
// pass/fail semantics. The current approach ensures portability in a plain C++11 environment.

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main()
{
    // In this test harness, we directly invoke the test runner.
    // The 64-bit path is guarded by UNITY_SUPPORT_64 macro in the test functions.
    runAllTests();
    return 0;
}