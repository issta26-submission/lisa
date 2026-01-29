// Test suite for ComponentSize in threaded_split.c
// This file provides a lightweight, self-contained test harness (no GTest) 
// that can be compiled with C++11. It exercises the focal function by validating
// its behavior against the T_BYTES macro and the sizeof(cmsUInt64Number) fallback.
//
// Notes:
// - The tests rely on the macro T_BYTES(format) defined in threaded_internal.h.
// - We assume at least two test formats: one for which T_BYTES(format) != 0 (non-zero branch)
//   and one for which T_BYTES(format) == 0 (zero branch). We use format values that are
//   simple constants to drive the decision branches.
// - We use a non-terminating test approach: tests report failures but do not abort or throw,
//   allowing all tests to run for maximum coverage.
//
// Candidate Keywords (Step 1): T_BYTES, cmsUInt64Number, BytesPerComponent, ComponentSize,
// cmsUInt32Number, sizeof, zero-branch, non-zero-branch, inline function, memory size.

#include <threaded_internal.h>
#include <iostream>


// Lightweight test framework (non-terminating assertions)
static int g_total_tests  = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASS()  do { ++g_passed_tests; } while(0)
#define TEST_FAIL(msg) do { \
    ++g_failed_tests; \
    std::cerr << "Test failed: " << (msg) << std::endl; \
} while(0)
#define TEST_CHECK(cond, msg) do { \
    ++g_total_tests; \
    if (cond) TEST_PASS(); else TEST_FAIL(msg); \
} while(0)

// Step 2: Unit Test Generation for ComponentSize
// We craft two tests:
//  - NonZeroBytesTest: uses a format value where T_BYTES(format) != 0 and expects ComponentSize(format)
//    to return exactly T_BYTES(format).
//  - ZeroBytesTest: uses a format value where T_BYTES(format) == 0 and expects ComponentSize(format)
//    to return sizeof(cmsUInt64Number).
//
// Explanatory test comments are provided above each test function.


// Test 1: Non-zero BytesPerComponent path: T_BYTES(format) != 0
// This test asserts that when T_BYTES(format) yields a non-zero value, ComponentSize
// returns that same value (the non-zero branch of the code path).
bool NonZeroBytesTest()
{
    // Chosen format to drive non-zero T_BYTES(format) value.
    // If the environment defines T_BYTES(1) == 0, this test will still compare against
    // the computed expected value, ensuring correctness for the used macro.
    cmsUInt32Number format = 1;

    cmsUInt32Number expected = T_BYTES(format);
    // If the macro resolves to 0 (unexpected for this test), fall back to the known size
    // as the function would in the zero-branch case. This keeps the test robust across
    // different internal macro definitions.
    if (expected == 0) {
        expected = sizeof(cmsUInt64Number);
    }

    cmsUInt32Number actual = ComponentSize(format);

    TEST_CHECK(actual == expected,
               "NonZeroBytesTest: ComponentSize(1) should equal T_BYTES(1) when non-zero");
    return actual == expected;
}

// Test 2: Zero BytesPerComponent path: T_BYTES(format) == 0
// This test asserts that when T_BYTES(format) yields zero, ComponentSize
// returns the size of cmsUInt64Number (the fallback path).
bool ZeroBytesTest()
{
    cmsUInt32Number format = 0;

    cmsUInt32Number expected = T_BYTES(format);
    if (expected == 0) {
        expected = sizeof(cmsUInt64Number);
    }

    cmsUInt32Number actual = ComponentSize(format);

    TEST_CHECK(actual == expected,
               "ZeroBytesTest: ComponentSize(0) should equal sizeof(cmsUInt64Number) when T_BYTES is zero");
    return actual == expected;
}

// Entry point
int main()
{
    std::cout << "Starting ComponentSize unit tests (non-GTest, non-terminating assertions)" << std::endl;

    // Run tests
    NonZeroBytesTest();
    ZeroBytesTest();

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Passed: " << g_passed_tests
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}