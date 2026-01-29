/*******************************************************
 * Lightweight C++11 test suite for the focal method
 * testINT8sWithinDelta (as found in testunity.c)
 *
 * Notes:
 * - This test suite is written to be self-contained
 *   and does not depend on Unity or GTest.
 * - It provides non-terminating assertions so that
 *   all tests can run to completion.
 * - The tests focus on the core behavior of "within delta"
 *   checks for int8 values, mirroring the essence of
 *   testINT8sWithinDelta from the provided focal method.
 *
 * Step 1 (Program Understanding):
 * Candidate Keywords extracted from the focal method:
 * - INT8, WITHIN, DELTA, testINT8sWithinDelta
 * - 1 and 127, 126
 * - -2, 2
 * Step 2 (Unit Test Generation) and Step 3 (Refinement) are
 * implemented in this file as a concise, executable suite.
 *******************************************************/

// Candidate Keywords (for traceability/documentation)
 // Keywords: testINT8sWithinDelta, INT8, WITHIN, DELTA, 1, 127, 126, -2, 2

#include <cmath>
#include <cstdint>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight test framework (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_totalTests; \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_failedTests; \
    } \
} while (0)

#define ASSERT_FALSE(cond, msg) do { \
    ++g_totalTests; \
    if (cond) { \
        std::cerr << "[FAIL] " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_failedTests; \
    } \
} while (0)

#define RUN_TEST(func) do { \
    std::cout << "RUNNING: " #func << std::endl; \
    func(); \
} while (0)

inline bool withinInt8Delta(int delta, int8_t a, int8_t b)
{
    // Core logic: absolute difference <= delta
    int diff = static_cast<int>(a) - static_cast<int>(b);
    if (diff < 0) diff = -diff;
    return diff <= delta;
}

// Forward declarations of tests
static void testINT8sWithinDelta_BasicPasses();
static void testINT8sWithinDelta_BoundaryEquality();
static void testINT8sWithinDelta_NotWithinDelta();
static void testINT8sWithinDelta_MaxBoundaryNotWithin();
static void testINT8sWithinDelta_EdgeCases();

// Test registry entry
struct TestCase {
    const char* name;
    std::function<void()> func;
};

static void testINT8sWithinDelta_BasicPasses()
{
    // This test mirrors the focal method's core assertions:
    // TEST_ASSERT_INT8_WITHIN(1, 127, 126);
    // TEST_ASSERT_INT8_WITHIN(5, -2, 2);
    // TEST_ASSERT_INT8_WITHIN(5, 2, -2);
    // All three should pass (no abort)
    bool r1 = withinInt8Delta(1, static_cast<int8_t>(127), static_cast<int8_t>(126));
    bool r2 = withinInt8Delta(5, static_cast<int8_t>(-2), static_cast<int8_t>(2));
    bool r3 = withinInt8Delta(5, static_cast<int8_t>(2), static_cast<int8_t>(-2));

    ASSERT_TRUE(r1, "INT8 within delta failed for (127,126) with delta=1");
    ASSERT_TRUE(r2, "INT8 within delta failed for (-2,2) with delta=5");
    ASSERT_TRUE(r3, "INT8 within delta failed for (2,-2) with delta=5");
}

static void testINT8sWithinDelta_BoundaryEquality()
{
    // Additional coverage: boundary equality cases
    // within delta 4 for (0,4) -> false; but (0,4) with delta 4 would be true
    bool r1 = withinInt8Delta(4, static_cast<int8_t>(0), static_cast<int8_t>(4)); // diff=4
    bool r2 = withinInt8Delta(4, static_cast<int8_t>(-4), static_cast<int8_t>(0)); // diff=4
    ASSERT_TRUE(r1, "INT8 within delta boundary equality failed for (0,4) delta=4");
    ASSERT_TRUE(r2, "INT8 within delta boundary equality failed for (-4,0) delta=4");
}

static void testINT8sWithinDelta_NotWithinDelta()
{
    // Not within delta cases should fail (non-aborting in this harness)
    bool r = withinInt8Delta(1, static_cast<int8_t>(127), static_cast<int8_t>(-126)); // diff ~ 253
    ASSERT_FALSE(r, "INT8 within delta incorrectly true for (127,-126) delta=1");
}

static void testINT8sWithinDelta_MaxBoundaryNotWithin()
{
    // Extreme boundary test: 127 vs -128 with any reasonable delta
    bool r = withinInt8Delta(254, static_cast<int8_t>(127), static_cast<int8_t>(-128)); // diff=255
    // 255 > 254 -> false
    ASSERT_FALSE(r, "INT8 within delta incorrectly true for (127,-128) delta=254");
}

static void testINT8sWithinDelta_EdgeCases()
{
    // Edge case: delta 0 (only identical values pass)
    bool r1 = withinInt8Delta(0, static_cast<int8_t>(10), static_cast<int8_t>(10));
    bool r2 = withinInt8Delta(0, static_cast<int8_t>(-5), static_cast<int8_t>(0)); // diff=5
    ASSERT_TRUE(r1, "INT8 within delta edge case failed for (10,10) delta=0");
    ASSERT_FALSE(r2, "INT8 within delta edge case incorrectly true for (-5,0) delta=0");
}

// Main runs all tests
int main()
{
    // Register tests
    std::vector<TestCase> tests = {
        {"testINT8sWithinDelta_BasicPasses", testINT8sWithinDelta_BasicPasses},
        {"testINT8sWithinDelta_BoundaryEquality", testINT8sWithinDelta_BoundaryEquality},
        {"testINT8sWithinDelta_NotWithinDelta", testINT8sWithinDelta_NotWithinDelta},
        {"testINT8sWithinDelta_MaxBoundaryNotWithin", testINT8sWithinDelta_MaxBoundaryNotWithin},
        {"testINT8sWithinDelta_EdgeCases", testINT8sWithinDelta_EdgeCases}
    };

    // Run all tests
    for (auto &tc : tests) {
        // Reset per-test metrics if needed (not strictly required for this simple harness)
        tc.func();
    }

    // Summary
    std::cout << "Total tests run: " << g_totalTests << std::endl;
    std::cout << "Total tests failed: " << g_failedTests << std::endl;
    if (g_failedTests == 0)
        std::cout << "ALL TESTS PASSED" << std::endl;
    else
        std::cout << "SOME TESTS FAILED" << std::endl;

    return g_failedTests;
}

// Definitions after main (to satisfy the static forward declarations)
static void testINT8sWithinDelta_BasicPasses();
static void testINT8sWithinDelta_BoundaryEquality();
static void testINT8sWithinDelta_NotWithinDelta();
static void testINT8sWithinDelta_MaxBoundaryNotWithin();
static void testINT8sWithinDelta_EdgeCases();

```