// A standalone C++11 test-suite that targets the core logic exercised by the focal
// testEqualInt16Arrays in testunity.c, adapted to run with a minimal, self-contained
// harness (no GTest). It reuses the concept of Unity-like array comparison (16-bit)
// but implemented here with plain C++ for portability in a C++11 build.
//
// Notes:
// - This suite focuses on the key dependent components and behaviors (int16 arrays,
//   the length parameter, and boundary conditions) that the focal method exercises.
// - It also adds a few extra scenarios to improve coverage beyond the original focal test.
// - The tests are designed to be non-terminal on failure for broader coverage (exceptions
//   are caught in the runner and reported as failures).

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Lightweight replicated core: compare two int16_t arrays up to a given length.
// This mirrors the essential behavior of Unity's TEST_ASSERT_EQUAL_INT16_ARRAY for a subset.
static bool equalInt16Arrays(const int16_t* a, const int16_t* b, size_t len)
{
    if (len == 0) {
        // By convention, zero-length comparisons are considered equal.
        return true;
    }
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Very small assertion helper that throws on failure to simplify a standalone harness.
static void expectTrue(bool cond, const std::string& msg)
{
    if (!cond)
        throw std::runtime_error("ASSERTION FAILED: " + msg);
}

// Type representing a unit test case.
struct TestCase {
    std::string name;
    std::function<void()> func;
};

// FOCAL: testEqualInt16Arrays
// We recreate tests that exercise the same scenarios using a standalone harness.
// Explanatory comment per test describing what behavior is validated.
static void testEqualInt16Arrays_Basics()
{
    // Given arrays
    int16_t p0[] = {1, 8, 117, 3};
    int16_t p1[] = {1, 8, 117, 3};
    int16_t p2[] = {1, 8, 117, 2};
    int16_t p3[] = {1, 50, 60, 70};

    // Case 1: compare p0 with itself for length 1 -> should be equal
    expectTrue(equalInt16Arrays(p0, p0, 1),
               "p0 vs p0 with len=1 should be equal");

    // Case 2: compare p0 with itself for full length 4 -> should be equal
    expectTrue(equalInt16Arrays(p0, p0, 4),
               "p0 vs p0 with len=4 should be equal");

    // Case 3: compare p0 with p1 (identical contents) for full length -> equal
    expectTrue(equalInt16Arrays(p0, p1, 4),
               "p0 vs p1 (identical) with len=4 should be equal");

    // Case 4: compare p0 with p2 for length 3 -> first 3 elements equal -> equal
    expectTrue(equalInt16Arrays(p0, p2, 3),
               "First 3 elements equal between p0 and p2 for len=3");

    // Case 5: compare p0 with p3 for length 1 -> first element equal -> equal
    expectTrue(equalInt16Arrays(p0, p3, 1),
               "First element equal between p0 and p3 for len=1");
}

// Additional positive test: different first element but length=1 should fail
static void testEqualInt16Arrays_DifferentFirstWithinLenShouldFail()
{
    int16_t a[] = { -1 };
    int16_t b[] = {  1 };
    // Should be not equal when len=1
    if (equalInt16Arrays(a, b, 1))
        throw std::runtime_error("Expected inequality for len=1 when first elements differ");
}

// Additional positive test: zero-length comparison should always be equal
static void testEqualInt16Arrays_ZeroLengthAlwaysEqual()
{
    int16_t a[] = { 5, -7, 1024 };
    int16_t b[] = { -5, 7, -1024 };
    // len=0 should yield equality
    expectTrue(equalInt16Arrays(a, b, 0),
               "Zero-length comparison should be treated as equal");
}

// Additional positive test: identical content but larger arrays, ensure boundary
static void testEqualInt16Arrays_MidsizeBoundary()
{
    int16_t a[] = { 10, 20, 30, 40, 50 };
    int16_t b[] = { 10, 20, 30, 99, 50 }; // differs at index 3 (0-based), but we compare only len=3
    expectTrue(equalInt16Arrays(a, b, 3),
               "First three elements are equal; len=3 should pass even if later elements differ");
}

// Runner that executes all tests and reports results.
int main()
{
    std::vector<TestCase> tests = {
        { "testEqualInt16Arrays_Basics", testEqualInt16Arrays_Basics },
        { "testEqualInt16Arrays_DifferentFirstWithinLenShouldFail", testEqualInt16Arrays_DifferentFirstWithinLenShouldFail },
        { "testEqualInt16Arrays_ZeroLengthAlwaysEqual", testEqualInt16Arrays_ZeroLengthAlwaysEqual },
        { "testEqualInt16Arrays_MidsizeBoundary", testEqualInt16Arrays_MidsizeBoundary }
    };

    size_t total = tests.size();
    size_t passed = 0;

    std::cout << "Running " << total << " test(s) for focal method: testEqualInt16Arrays (standalone harness)\n";

    for (const auto& t : tests)
    {
        try
        {
            t.func();
            std::cout << "[PASS] " << t.name << "\n";
            ++passed;
        }
        catch (const std::exception& ex)
        {
            std::cout << "[FAIL] " << t.name << " - " << ex.what() << "\n";
        }
        catch (...)
        {
            std::cout << "[FAIL] " << t.name << " - unknown exception\n";
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}