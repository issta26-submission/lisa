// Lightweight unit tests for Word2Byte (cmsUInt8Number Word2Byte(cmsUInt16Number w))
// This test harness is C++11 compatible and does not rely on GTest.
// It uses a small set of non-terminating EXPECT_* macros to gather coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cmath>


// Minimal type definitions to match the focal function interface.
// In the real project these would come from cms types, but for testing we mirror them here.
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef double cmsFloat64Number;

// Declare the focal function from cmsps2.c with C linkage so we can link against it.
// We assume the test binary will be linked with the cmsps2.c object file.
extern "C" cmsUInt8Number Word2Byte(cmsUInt16Number w);

// Simple non-terminating assertion macros.
// They record failures but do not abort the test run, increasing coverage.
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_EQ(actual, expected) do { \
    ++g_totalTests; \
    auto _a = (actual); \
    auto _e = (expected); \
    if (_a != _e) { \
        ++g_failedTests; \
        std::cerr << "FAIL: " << __FUNCTION__ << " at line " << __LINE__ \
                  << ": expected " << +(_e) << ", got " << +(_a) << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if (!(cond)) { \
        ++g_failedTests; \
        std::cerr << "FAIL: " << __FUNCTION__ << " at line " << __LINE__ \
                  << ": condition is false" << std::endl; \
    } \
} while (0)

#define TEST_FOCUS(message) \
    std::cout << "Running test: " << message << std::endl;

// Helper to compute the expected Word2Byte value using the same mathematical formula.
// This is used to validate the actual function's output without peeking into its implementation.
static cmsUInt8Number ExpectedWord2Byte(cmsUInt16Number w)
{
    double val = static_cast<double>(w) / 257.0 + 0.5;
    // floor of val, cast to 8-bit unsigned
    return static_cast<cmsUInt8Number>(static_cast<int>(std::floor(val)));
}

// Test 1: Boundary values around the first threshold (0 -> 0; 129 -> 1)
static void test_Word2Byte_BoundaryFirstThreshold()
{
    TEST_FOCUS("Word2Byte boundary around first threshold (0/128 vs 129)");
    // w = 0 -> 0
    EXPECT_EQ(Word2Byte(0), ExpectedWord2Byte(0));

    // w = 128 -> 0
    EXPECT_EQ(Word2Byte(128), ExpectedWord2Byte(128));

    // w = 129 -> 1
    EXPECT_EQ(Word2Byte(129), ExpectedWord2Byte(129));
}

// Test 2: Small range samples to exercise rounding behavior
static void test_Word2Byte_SmallRangeSamples()
{
    TEST_FOCUS("Word2Byte small range samples to cover rounding behavior");
    // 256, 385 -> both map to 1
    EXPECT_EQ(Word2Byte(256), ExpectedWord2Byte(256));
    EXPECT_EQ(Word2Byte(385), ExpectedWord2Byte(385));

    // 386 -> 2 (boundary to next bucket)
    EXPECT_EQ(Word2Byte(386), ExpectedWord2Byte(386));

    // 642 -> 2 (still bucket 2)
    EXPECT_EQ(Word2Byte(642), ExpectedWord2Byte(642));

    // 643 -> 3 (boundary to next bucket)
    EXPECT_EQ(Word2Byte(643), ExpectedWord2Byte(643));
}

// Test 3: Upper-bound boundary and a few additional samples to ensure non-decreasing mapping
static void test_Word2Byte_BoundsAndMonotonic()
{
    TEST_FOCUS("Word2Byte upper-bound and monotonicity checks");
    // Maximum w should map to 255
    EXPECT_EQ(Word2Byte(65535), ExpectedWord2Byte(65535));

    // A mid range value
    EXPECT_EQ(Word2Byte(5000), ExpectedWord2Byte(5000));
    EXPECT_EQ(Word2Byte(10000), ExpectedWord2Byte(10000));
    EXPECT_EQ(Word2Byte(60000), ExpectedWord2Byte(60000));

    // Monotonicity check across a small sequence
    cmsUInt8Number prev = Word2Byte(0);
    for (cmsUInt16Number w = 1; w <= 200; ++w) {
        cmsUInt8Number cur = Word2Byte(w);
        EXPECT_TRUE(cur >= prev); // non-decreasing
        prev = cur;
    }
}

// Test 4: Random-ish samples to increase coverage
static void test_Word2Byte_RandomishSamples()
{
    TEST_FOCUS("Word2Byte random-ish samples for broader coverage");
    EXPECT_EQ(Word2Byte(123), ExpectedWord2Byte(123));
    EXPECT_EQ(Word2Byte(1000), ExpectedWord2Byte(1000));
    EXPECT_EQ(Word2Byte(2000), ExpectedWord2Byte(2000));
    EXPECT_EQ(Word2Byte(8000), ExpectedWord2Byte(8000));
}

// Main driver to run all tests
int main()
{
    std::cout << "Starting Word2Byte unit tests (cmsUInt8Number Word2Byte(cmsUInt16Number))" << std::endl;

    test_Word2Byte_BoundaryFirstThreshold();
    test_Word2Byte_SmallRangeSamples();
    test_Word2Byte_BoundsAndMonotonic();
    test_Word2Byte_RandomishSamples();

    std::cout << "Tests completed. Total: " << g_totalTests
              << ", Passed: " << (g_totalTests - g_failedTests)
              << ", Failed: " << g_failedTests << std::endl;

    // Return non-zero if any test failed to aid automation in CI environments.
    return (g_failedTests == 0) ? 0 : 1;
}