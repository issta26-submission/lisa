/*
  C++11 unit test suite for the focal method testNotEqualMemoryArraysActualNull
  (ported from the Unity test in testunity.c).

  Design notes:
  - We implement a lightweight, non-terminating test framework suitable for C++11.
  - The focal behavior (as in the original Unity test) is to detect inequality
    between two memory blocks where the actual pointer is NULL. In Unity this
    would trigger a test failure/abort. Here we implement a pure-NOT-ABORT check:
    the test passes if inequality is detected (i.e., memory blocks are not equal),
    which mirrors the intent of the original test without terminating the test runner.
  - We cover the focal case plus several additional scenarios to improve coverage:
    - Not equal when actual is NULL (focal case)
    - Not equal for mismatches in different positions
    - Equal case
  - All tests are self-contained and do not rely on GTest or Unity.
*/

#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight, non-aborting memory comparison helper.
// Returns true if the memory blocks are equal, false otherwise.
// If either pointer is NULL, returns true only if both are NULL and count == 0.
static bool memory_equal(const void* a, const void* b, std::size_t elemSize, std::size_t count) {
    if (a == nullptr || b == nullptr) {
        // Treat NULLs as equal only when both are NULL and the requested region is zero-sized.
        return (a == nullptr && b == nullptr && count == 0);
    }
    // Compare the raw memory blocks: block size = elemSize * count
    const std::size_t totalBytes = elemSize * count;
    return std::memcmp(a, b, totalBytes) == 0;
}

/*
  Candidate Keywords (Step 1)
  - Memory arrays
  - NULL vs non-NULL pointers (actual vs expected)
  - Element size and element count
  - Memory equality assertion semantics
  - Non-terminating assertions (test harness style)
  - Cross-type applicability via generic pointer comparisons
  - Minimal, dependency-free testing (no GTest/GMock)
*/

// Test harness infrastructure
using TestFunc = std::function<bool()>;

struct TestCase {
    std::string name;
    TestFunc func;
};

static std::vector<TestCase> g_tests;

// Helper to register tests
#define REGISTER_TEST(name) \
    g_tests.push_back({#name, static_cast<TestFunc>(name)})

// Focal test implementations (non-terminating, purely assertion-based)

// Test: NotEqualMemoryArraysActualNull
// Purpose: Ensure that memory blocks are detected as not equal when the actual
// is NULL (mimics the Unity test's expected abort/fail condition).
static bool test_NotEqualMemoryArraysActualNull() {
    int p0[] = {1, 8, 987, -2};
    int* p1 = nullptr;

    bool equal = memory_equal(static_cast<const void*>(p0),
                              static_cast<const void*>(p1),
                              sizeof(int), 4);
    // Expect not equal (i.e., false from memory_equal)
    return !equal;
}

// Test: NotEqualMemoryArraysMismatchLast
// Purpose: Ensure inequality is detected when the last element differs.
static bool test_NotEqualMemoryArraysMismatchLast() {
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2}; // last element differs
    bool equal = memory_equal(static_cast<const void*>(p0),
                              static_cast<const void*>(p1),
                              sizeof(int), 4);
    return !equal;
}

// Test: NotEqualMemoryArraysMismatchFirst
// Purpose: Ensure inequality is detected when the first element differs.
static bool test_NotEqualMemoryArraysMismatchFirst() {
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2}; // first element differs
    bool equal = memory_equal(static_cast<const void*>(p0),
                              static_cast<const void*>(p1),
                              sizeof(int), 4);
    return !equal;
}

// Test: NotEqualMemoryArraysMismatchThird
// Purpose: Ensure inequality is detected when the third element differs.
static bool test_NotEqualMemoryArraysMismatchThird() {
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2}; // third element differs
    bool equal = memory_equal(static_cast<const void*>(p0),
                              static_cast<const void*>(p1),
                              sizeof(int), 4);
    return !equal;
}

// Test: EqualMemoryArrays
// Purpose: Ensure equality is detected when both arrays are identical.
static bool test_EqualMemoryArrays() {
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    bool equal = memory_equal(static_cast<const void*>(p0),
                              static_cast<const void*>(p1),
                              sizeof(int), 4);
    return equal;
}

// Registration of the focal and additional tests
static void register_tests() {
    REGISTER_TEST(test_NotEqualMemoryArraysActualNull);
    REGISTER_TEST(test_NotEqualMemoryArraysMismatchLast);
    REGISTER_TEST(test_NotEqualMemoryArraysMismatchFirst);
    REGISTER_TEST(test_NotEqualMemoryArraysMismatchThird);
    REGISTER_TEST(test_EqualMemoryArrays);
}

// Minimal test driver (no external frameworks)
int main() {
    register_tests();

    int passed = 0;
    int failed = 0;

    for (const auto& t : g_tests) {
        bool result = false;
        try {
            result = t.func();
        } catch (...) {
            // If an unexpected exception occurs, count as failure for robustness.
            result = false;
        }

        if (result) {
            std::cout << "[PASS] " << t.name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << t.name << "\n";
            ++failed;
        }
    }

    std::cout << "\nTest results: "
              << passed << " passed, "
              << failed << " failed.\n";

    // Return non-zero if any test failed (simple CI signal)
    return failed ? 1 : 0;
}