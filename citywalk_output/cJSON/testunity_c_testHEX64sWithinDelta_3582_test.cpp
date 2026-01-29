/*
  Lightweight C++ test harness for the focal Unity-based testHEX64sWithinDelta
  This file provides a self-contained test suite that mimics the behavior of
  the Unity 64-bit within-delta assertion without depending on GTest.
  It demonstrates:
  - Core 64-bit within-delta checks (via hex64_within)
  - A simple static dependency class to reflect class dependencies
  - Conditional compilation paths simulating UNITY_SUPPORT_64
  - A small test runner invoked from main
  - "Non-terminating" assertions: tests collect failures but do not abort
  Notes:
  - By default UNITY_SUPPORT_64 is not defined, so the focal test is reported as IGNORE.
  - Define UNITY_SUPPORT_64 at compile time to exercise the 64-bit test cases.
  - This code compiles under C++11 and does not rely on GTest.
*/

#include <cmath>
#include <unity.h>
#include <cstdio>
#include <stdint.h>
#include <string.h>
#include <cstdlib>
#include <cstdint>
#include <stdio.h>


// Global counters to reflect Unity-like test reporting
static int g_totalTests = 0;
static int g_totalFailures = 0;

// A simple class to reflect "class dependencies" mentioned in the prompt.
// Static members are accessed to demonstrate correct usage via the class name.
class DependencyClass {
public:
    static int staticCounter;
    static void setCounter(int v) { staticCounter = v; }
    static int getCounter() { return staticCounter; }
};

// Definition of the static member
int DependencyClass::staticCounter = 0;

// Internal helper: compute absolute difference for 64-bit values
static inline uint64_t diff64(uint64_t a, uint64_t b) {
    return (a > b) ? (a - b) : (b - a);
}

// Core logic under test: whether two 64-bit values are within a given delta
static inline bool hex64_within(uint64_t delta, uint64_t expected, uint64_t actual) {
    // Guard against underflow/overflow is not necessary for unsigned diffs here
    uint64_t d = diff64(expected, actual);
    return d <= delta;
}

// Lightweight non-terminating assertion macro for 64-bit within-delta.
// Mimics Unity's TEST_ASSERT_HEX64_WITHIN behavior but in a standalone harness.
#ifndef UNITY_KERNEL_64BIT_MOCK
#define TEST_ASSERT_HEX64_WITHIN(delta, expected, actual) do { \
    ++g_totalTests; \
    if (!hex64_within((delta), (uint64_t)(expected), (uint64_t)(actual))) { \
        ++g_totalFailures; \
        std::fprintf(stderr, "HEX64_WITHIN_FAILED: delta=%llu, expected=0x%016llX, actual=0x%016llX (line=%d)\n", \
                     (unsigned long long)(delta), (unsigned long long)(expected), (unsigned long long)(actual), __LINE__); \
    } \
} while(0)
#else
// If a UNITY kernel is present in the environment, you could replace with the real macro.
// For this standalone harness we simply fall back to the same behavior above.
#define TEST_ASSERT_HEX64_WITHIN(delta, expected, actual) do { \
    ++g_totalTests; \
    if (!hex64_within((delta), (uint64_t)(expected), (uint64_t)(actual))) { \
        ++g_totalFailures; \
        std::fprintf(stderr, "HEX64_WITHIN_FAILED (UNITY_KERNEL_MOCK): delta=%llu, expected=0x%016llX, actual=0x%016llX (line=%d)\n", \
                     (unsigned long long)(delta), (unsigned long long)(expected), (unsigned long long)(actual), __LINE__); \
    } \
} while(0)
#endif

/*
  Step 2, and reflection of Step 3 Domain Knowledge:
  - We provide a focal test: testHEX64sWithinDelta mirroring the Unity test.
  - We include a small dependency (DependencyClass) to show static member access via class name.
  - We include an extra edge-case test and a not-within-delta test to improve coverage.
  - We simulate the UNITY_SUPPORT_64 path via a compile-time macro.
*/

// Focal test: mirrors the original testHEX64sWithinDelta in testunity.c
// Behavior depends on UNITY_SUPPORT_64 macro.
static void testHEX64sWithinDelta(void)
{
    // This block represents the branch where 64-bit support is available.
#ifdef UNITY_SUPPORT_64
    // True branch: 1-unit delta around the two adjacent 64-bit literals should pass
    TEST_ASSERT_HEX64_WITHIN(1, 0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFEULL);
    // Additional within-delta checks using regular 64-bit integers
    TEST_ASSERT_HEX64_WITHIN(5, 5000ULL, 4996ULL);
    TEST_ASSERT_HEX64_WITHIN(5, 5000ULL, 5005ULL);

    // Demonstrate access to a static member via its class name (class dependency)
    DependencyClass::setCounter(42);
    if (DependencyClass::getCounter() != 42) {
        ++g_totalFailures;
        std::fprintf(stderr, "DEPENDENCY_STATIC_ACCESS_FAILED: DependencyClass::getCounter() != 42 (line=%d)\n", __LINE__);
    }
#else
    // If 64-bit support is not available, mimic Unity's TEST_IGNORE behavior.
    // Here we simply note the ignore in the output and do not count as a failure.
    std::printf("TEST_IGNORE: testHEX64sWithinDelta (UNITY_SUPPORT_64 not defined)\n");
#endif
}

// Extra test: additional 64-bit edge-case coverage to improve branch coverage
static void testHEX64sWithinDelta_EdgeCases(void)
{
#ifdef UNITY_SUPPORT_64
    // Edge: exact match with delta 0 should pass
    TEST_ASSERT_HEX64_WITHIN(0, 0x123456789ABCDEF0ULL, 0x123456789ABCDEF0ULL);
    // Edge: near upper range with small delta
    TEST_ASSERT_HEX64_WITHIN(2, 0xFFFFFFFFFFFFFFF0ULL, 0xFFFFFFFFFFFFFFF2ULL);
#else
    // No-op when 64-bit support is not enabled
#endif
}

// Negative/Not-Within test: ensures a failure is recorded when delta is insufficient
static void testHEX64sWithinDelta_NotWithinDelta(void)
{
#ifdef UNITY_SUPPORT_64
    // This should fail since delta is 0 but values differ by 1
    TEST_ASSERT_HEX64_WITHIN(0, 0x1ULL, 0x2ULL);
#else
#endif
}

// Simple test runner: calls focal test and extra tests.
// Exposed from main to satisfy the requirement to call tests from main when gtest is not used.
static void runAllTests(void)
{
#ifdef UNITY_SUPPORT_64
    testHEX64sWithinDelta();
    testHEX64sWithinDelta_EdgeCases();
    testHEX64sWithinDelta_NotWithinDelta();
#else
    testHEX64sWithinDelta();          // Will print ignore message
    testHEX64sWithinDelta_EdgeCases(); // Will be no-op
    testHEX64sWithinDelta_NotWithinDelta(); // Will be no-op
#endif

    // Summary output
    std::printf("Total tests executed: %d\n", g_totalTests);
    std::printf("Total failures detected: %d\n", g_totalFailures);
}

// Expose a C-style main as required by the domain knowledge (call tests from main when not using GTest)
int main(void)
{
    runAllTests();
    // Return non-zero if there were any failures to aid automation
    return (g_totalFailures > 0) ? 1 : 0;
}