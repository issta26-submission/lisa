// Minimal C++11 test harness to validate the focal behavior of testNotEqualMemoryArrays1
// without using GTest or Unity's test framework.
// This focuses on the core idea: TEST_ASSERT_EQUAL_MEMORY_ARRAY should abort on mismatch.
// We replicate the abort-when-different semantics via setjmp/longjmp.

// Candidate Keywords reflected from the focal method analysis:
// - memory arrays, memory comparison, element size, element count, equality check
// - abort-on-mismatch (assertion failure), not-equal scenario
// - two int arrays with a single differing element (-2 vs 2)

#include <unity.h>
#include <cstring>
#include <csetjmp>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Global jump buffer used to simulate Unity's abort-on-failure behavior.
static jmp_buf g_testAbortBuf;

// Under-test: a lightweight assertion that emulates Unity's TEST_ASSERT_EQUAL_MEMORY_ARRAY
// It compares 'count' elements of size 'size' starting at pointers 'a' and 'b'.
// If any corresponding bytes differ, it aborts via longjmp to the test harness.
static void test_assert_equal_memory_array(const void* a, const void* b, size_t size, size_t count)
{
    const unsigned char* pa = static_cast<const unsigned char*>(a);
    const unsigned char* pb = static_cast<const unsigned char*>(b);
    for (size_t i = 0; i < count; ++i)
    {
        if (std::memcmp(pa + i * size, pb + i * size, size) != 0)
        {
            // Simulate Unity-like abort on failure
            longjmp(g_testAbortBuf, 1); // non-zero return indicates abort/event
        }
    }
    // If loop completes, memory blocks are equal (no abort).
}

// Focal test bodies (as scenarios). They mirror the original testunity.c focal method(s),
// but are implemented as plain C++ functions to be invoked by the test harness.

static void focal_testNotEqualMemoryArrays1_body(void)
{
    // Two int arrays that differ in the last element: -2 vs 2
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, 2};
    // This should abort, since the memory arrays are not equal.
    test_assert_equal_memory_array(p0, p1, sizeof(int), 4);
}

static void focal_testEqualMemoryArrays_noAbort_body(void)
{
    // Equal arrays should not abort
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    test_assert_equal_memory_array(p0, p1, sizeof(int), 4);
}

// Lightweight test runner
// - runTest invokes the given test function.
// - It expects an abort (via longjmp) if 'expectAbort' is true.
// - It reports PASS/FAIL with a short message.
// - Returns true if the test outcome matches the expectation.
static bool runTest(void (*testFunc)(void), bool expectAbort, const std::string& testName)
{
    int jumpVal = setjmp(g_testAbortBuf);
    if (jumpVal == 0)
    {
        // No abort has occurred yet; run the test function.
        testFunc();
        // If we reach here, no abort happened.
        if (expectAbort)
        {
            std::cout << "[FAIL] " << testName << " - expected abort but did not abort.\n";
            return false;
        }
        else
        {
            std::cout << "[PASS] " << testName << " - completed without abort as expected.\n";
            return true;
        }
    }
    else
    {
        // An abort occurred (longjmp returned here).
        if (expectAbort)
        {
            std::cout << "[PASS] " << testName << " - aborted as expected.\n";
            return true;
        }
        else
        {
            std::cout << "[FAIL] " << testName << " - aborted unexpectedly.\n";
            return false;
        }
    }
}

int main()
{
    // Step 2 / Step 3: Build a small, focused test suite around the focal method.
    // Test 1: NotEqualMemoryArrays1 should abort when arrays differ (the original focal test).
    // Explanatory comment: mirrors testNotEqualMemoryArrays1 in testunity.c where p0 != p1.
    bool t1 = runTest(&focal_testNotEqualMemoryArrays1_body, true, "testNotEqualMemoryArrays1_body_abort_expected");

    // Test 2: EqualMemoryArrays should NOT abort when arrays are equal.
    // Explanatory comment: ensures non-abort path is exercised (true branch).
    bool t2 = runTest(&focal_testEqualMemoryArrays_noAbort_body, false, "testEqualMemoryArrays_noAbort");

    // Return overall status: 0 if all tests passed, non-zero otherwise.
    int failures = 0;
    if (!t1) ++failures;
    if (!t2) ++failures;

    return failures;
}