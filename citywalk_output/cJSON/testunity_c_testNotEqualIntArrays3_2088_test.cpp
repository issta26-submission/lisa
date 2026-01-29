/*
 * Lightweight C++11 test suite for the focal method scenario:
 * testNotEqualIntArrays3 (as described in the provided Unity test file).
 *
 * Intent:
 * - Provide a self-contained, non-GTest test harness in C++11 that exercises
 *   the core logic behind the focal test: comparing two int arrays for equality.
 * - Reproduce the scenario where two int arrays differ (the "Not Equal" case)
 *   and validate the test behavior without depending on the Unity framework.
 *
 * Notes:
 * - The real Unity test in testunity.c uses Unity’s macros (EXPECT_ABORT_BEGIN, etc.)
 *   to catch aborts on inequality. In this standalone suite we implement a small
 *   equivalence checker and expect certain outcomes. We emulate the spirit of
 *   the tests by writing explicit assertions and reporting PASS/FAIL.
 * - We also include a few additional tests to cover:
 *   - Equal arrays
 *   - Difference at a different index
 *   - NULL pointer handling (both NULL, one NULL)
 *
 * This file is self-contained and compiles under C++11. No external testing
 * frameworks are required.
 */

#include <cstdint>
#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords derived from the focal method:
// - p0, p1 (input arrays), 4 (length in the given test), equality check
// - Not Equal scenario (differs at an element)
// - Test harness: PASS/FAIL reporting
// - NULL handling (both NULL vs one NULL scenarios)

// Utility: determine whether two int arrays are element-wise equal for n elements.
// Semantics inferred from Unity tests:
 // - If both pointers are NULL, they are considered equal.
 // - If one is NULL and the other is not, they are not equal.
 // - Otherwise, compare element by element.
static bool IntArraysAreEqual(const int* a, const int* b, std::size_t n)
{
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;

    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Simple test framework (minimal, self-contained)
struct TestCase {
    const char* name;
    std::function<void()> func;
};

static void runAllTests(const std::vector<TestCase>& tests)
{
    using namespace std;
    size_t passed = 0;
    for (const auto& t : tests) {
        try {
            t.func();
            std::cout << "[PASS]  " << t.name << "\n";
            ++passed;
        } catch (const std::exception& e) {
            std::cout << "[FAIL]  " << t.name << " -> Exception: " << e.what() << "\n";
        } catch (...) {
            std::cout << "[FAIL]  " << t.name << " -> Unknown exception\n";
        }
    }
    std::cout << "\nTest Summary: " << passed << " / " << tests.size() << " tests passed.\n";
    if (passed != tests.size()) {
        throw std::runtime_error("Some tests failed.");
    }
}

/*  Test 1: Basic Not Equal case from Step 2 focal method
 *  p0 = {1, 8, 987, -2}
 *  p1 = {1, 8, 986, -2}
 *  Expectation: Not equal
 */
void testNotEqualIntArrays3_basic(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 986, -2};
    const std::size_t len = 4;

    bool eq = IntArraysAreEqual(p0, p1, len);
    if (eq) {
        throw std::runtime_error("Arrays unexpectedly equal in testNotEqualIntArrays3_basic");
    }
    // If we reach here, the test passes (arrays are not equal as expected)
}

/*  Test 2: Not Equal case where a different element is at index 0
 *  p0 = {1, 8, 987, -2}
 *  p1 = {2, 8, 987, -2}
 *  Expectation: Not equal
 */
void testNotEqualIntArrays3_diffFirst(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};
    const std::size_t len = 4;

    bool eq = IntArraysAreEqual(p0, p1, len);
    if (eq) {
        throw std::runtime_error("Arrays unexpectedly equal in testNotEqualIntArrays3_diffFirst");
    }
}

/*  Test 3: Equal case to ensure the equality path works
 *  p0 = {1, 8, 987, -2}
 *  p1 = {1, 8, 987, -2}
 *  Expectation: Equal
 */
void testEqualIntArrays3_equal(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    const std::size_t len = 4;

    bool eq = IntArraysAreEqual(p0, p1, len);
    if (!eq) {
        throw std::runtime_error("Arrays unexpectedly not equal in testEqualIntArrays3_equal");
    }
}

/*  Test 4: Both NULL inputs should be considered equal (per Unity test pattern)
 *  p0 = NULL
 *  p1 = NULL
 *  len = 1 (as used in tests)
 */
void testNotEqualIntArrays3_bothNull(void)
{
    const int* p0 = nullptr;
    const int* p1 = nullptr;
    const std::size_t len = 1;

    bool eq = IntArraysAreEqual(p0, p1, len);
    if (!eq) {
        throw std::runtime_error("Both NULL arrays should be considered equal in testNotEqualIntArrays3_bothNull");
    }
}

/*  Test 5: One NULL and one non-NULL should be Not Equal
 *  p0 = non-NULL
 *  p1 = NULL
 *  len = 4
 */
void testNotEqualIntArrays3_oneNull(void)
{
    int p0[] = {1, 2, 3, 4};
    const int* p1 = nullptr;
    const std::size_t len = 4;

    bool eq = IntArraysAreEqual(p0, p1, len);
    if (eq) {
        throw std::runtime_error("Arrays should not be equal when one side is NULL in testNotEqualIntArrays3_oneNull");
    }
}

// Main entry: gather and run all tests
int main()
{
    std::vector<TestCase> tests = {
        {"testNotEqualIntArrays3_basic", testNotEqualIntArrays3_basic},
        {"testNotEqualIntArrays3_diffFirst", testNotEqualIntArrays3_diffFirst},
        {"testEqualIntArrays3_equal", testEqualIntArrays3_equal},
        {"testNotEqualIntArrays3_bothNull", testNotEqualIntArrays3_bothNull},
        {"testNotEqualIntArrays3_oneNull", testNotEqualIntArrays3_oneNull}
    };

    try {
        runAllTests(tests);
    } catch (const std::exception& ex) {
        std::cerr << "Test suite failed: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}