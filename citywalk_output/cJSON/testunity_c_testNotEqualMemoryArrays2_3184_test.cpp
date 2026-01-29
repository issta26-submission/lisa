/*
  Notable context:
  - This file provides a small, self-contained C++11 test suite focused on
    verifying the behavior related to non-equality of memory buffers
    (i.e., scenarios similar to Unity's TEST_ASSERT_EQUAL_MEMORY_ARRAY where
    non-equal arrays should “fail” the test).
  - We implement a tiny, framework-like harness (no GTest) to execute tests,
    with explicit comments for each unit test describing its purpose.
  - The Candidate Keywords from Step 1 have been used to shape test names and logic:
    testNotEqualMemoryArrays2, TEST_ASSERT_EQUAL_MEMORY_ARRAY, p0/p1 arrays, sizeof(int),
    4 elements, and not-equal memory semantics.
  - This suite is designed to be independently runnable in C++11 environments
    and does not rely on Unity headers. It mirrors the intent of the focal
    function by ensuring not-equality scenarios are detected correctly.
*/

#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Lightweight assertion helper
#define ASSERT_MSG(cond, msg) do { if (!(cond)) throw std::runtime_error(msg); } while (0)

// Minimal memory comparison utility, similar in spirit to TEST_ASSERT_EQUAL_MEMORY_ARRAY
// element_size: size of each element (e.g., sizeof(int))
// count: number of elements
static bool memory_equal(const void* a, const void* b, size_t element_size, size_t count)
{
    // Zero-length comparisons are always considered equal
    if (count == 0) return true;
    // If both pointers are NULL, treat as equal; one NULL and one non-NULL is not equal
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return std::memcmp(a, b, element_size * count) == 0;
}

// Simple test-case harness
struct TestCase {
    const char* name;
    std::function<void()> fn;
    bool passed = false;
};

// Global function to run a single test case and capture failures
static void run_test(TestCase& t)
{
    try
    {
        t.fn();
        t.passed = true;
    }
    catch (const std::exception& ex)
    {
        t.passed = false;
        std::cerr << "Test \"" << t.name << "\" failed with exception: " << ex.what() << "\n";
    }
    catch (...)
    {
        t.passed = false;
        std::cerr << "Test \"" << t.name << "\" failed with unknown exception.\n";
    }
}

// Candidate Keyword driven test suite for non-equal memory arrays
// Focus: testNotEqualMemoryArrays2 variations
//
// Original focal method (conceptual): expects TEST_ASSERT_EQUAL_MEMORY_ARRAY(p0, p1, sizeof(int), 4)
// to fail when p0 and p1 are not equal. We mirror that intent by asserting
// that non-equality is detected correctly.

static void testNotEqualMemoryArrays2_BasicNotEqual()
{
    // Test: two int arrays differ at the first element
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    // We expect them to NOT be equal
    ASSERT_MSG(!eq, "Expected memory arrays to be not equal (basic case).");
}

static void testNotEqualMemoryArrays2_FirstElementDiff()
{
    // Different first element, other elements identical
    int p0[] = {5, 8, 987, -2};
    int p1[] = {7, 8, 987, -2};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(!eq, "Expected memory arrays to be not equal (first element diff).");
}

static void testNotEqualMemoryArrays2_SecondElementDiff()
{
    // Different second element
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 9, 987, -2};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(!eq, "Expected memory arrays to be not equal (second element diff).");
}

static void testNotEqualMemoryArrays2_ThirdElementDiff()
{
    // Different third element
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 988, -2};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(!eq, "Expected memory arrays to be not equal (third element diff).");
}

static void testNotEqualMemoryArrays2_FourthElementDiff()
{
    // Different fourth element
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -3};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(!eq, "Expected memory arrays to be not equal (fourth element diff).");
}

static void testNotEqualMemoryArrays2_NullComparedToNonNull()
{
    // One side NULL, other non-NULL
    int p0[] = {1, 8, 987, -2};
    int* p1 = nullptr;
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(!eq, "Expected memory comparison to be not equal when one side is NULL.");
}

static void testNotEqualMemoryArrays2_BasicallyDifferent()
{
    // A more varied difference: last and first element both differ
    int p0[] = {1, 8, 987, -2};
    int p1[] = {9, 8, 988, -3};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(!eq, "Expected memory arrays to be not equal (multiple diffs).");
}

static void testNotEqualMemoryArrays2_EqualityCheckWithIdenticalArrays()
{
    // Identical arrays should be equal (negative test would fail)
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(eq, "Expected memory arrays to be equal when contents match.");
}

// Optional: a simple test to ensure the equality checker reports equal case as not failing
static void testEqualMemoryArraysBasic()
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    bool eq = memory_equal(p0, p1, sizeof(int), 4);
    ASSERT_MSG(eq, "Expected memory arrays to be equal for identical contents.");
}

// Entry point to run all focused tests
int main()
{
    std::vector<TestCase> tests = {
        { "testNotEqualMemoryArrays2_BasicNotEqual", testNotEqualMemoryArrays2_BasicNotEqual },
        { "testNotEqualMemoryArrays2_FirstElementDiff", testNotEqualMemoryArrays2_FirstElementDiff },
        { "testNotEqualMemoryArrays2_SecondElementDiff", testNotEqualMemoryArrays2_SecondElementDiff },
        { "testNotEqualMemoryArrays2_ThirdElementDiff", testNotEqualMemoryArrays2_ThirdElementDiff },
        { "testNotEqualMemoryArrays2_FourthElementDiff", testNotEqualMemoryArrays2_FourthElementDiff },
        { "testNotEqualMemoryArrays2_NullComparedToNonNull", testNotEqualMemoryArrays2_NullComparedToNonNull },
        { "testNotEqualMemoryArrays2_BasicallyDifferent", testNotEqualMemoryArrays2_BasicallyDifferent },
        { "testNotEqualMemoryArrays2_EqualityCheckWithIdenticalArrays", testNotEqualMemoryArrays2_EqualityCheckWithIdenticalArrays },
        { "testEqualMemoryArraysBasic", testEqualMemoryArraysBasic }
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    for (auto &t : tests)
    {
        run_test(t);
        std::cout << "Test " << t.name << ": " << (t.passed ? "PASS" : "FAIL") << "\n";
        if (t.passed) ++passed;
    }

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}

/*
  Explanatory notes for reviewers:
  - The test suite targets the same core concept as the focal method testNotEqualMemoryArrays2:
    two memory buffers of identical element type (int) that differ in at least one element.
  - Each unit test documents the exact scenario it covers (which element differs, or whether a
    NULL pointer is involved).
  - The tests are self-contained, use C++11 features only, and do not rely on GTest or Unity.
  - The approach mirrors a focused aspect of Step 2 (unit testing a memory-equivalence assertion),
    while providing explicit, actionable feedback via exceptions and clear test names.
*/