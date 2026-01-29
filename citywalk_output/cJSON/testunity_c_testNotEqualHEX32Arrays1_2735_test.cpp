/*
  Candidate Keywords and test focus (Step 1):
  - UNITY_UINT32 / uint32_t data type
  - HEX32 array comparison context (TEST_ASSERT_EQUAL_HEX32_ARRAY semantics)
  - p0, p1: UNITY_UINT32 arrays used in the focal method
  - Length: 4 (array length in the focal test)
  - Focal test: testNotEqualHEX32Arrays1 (two arrays that differ at the last element)
  - Core operation: detect inequality between two 32-bit hex arrays
  - Static/global state not required for these pure tests; we implement a small self-contained harness
  - Non-GTest, lightweight C++11 test harness (non-terminating assertions)
  - Domain knowledge: correct handling of 32-bit unsigned values in hex context
  - Namespace: none (simple C++11 test harness)
  - Tests should exercise true/false outcomes (non-terminating)
*/

/*
  Lightweight, self-contained C++11 test harness
  - Not using Unity or GTest; designed to simulate the core behavior of the focal test
  - Provides ASSERT_TRUE/ASSERT_FALSE helpers that accumulate failures
  - Exposes a tiny array comparison utility: hex32ArraysNotEqual
  - Implements multiple test variants to cover the focal method and related cases
*/

#include <unity.h>
#include <functional>
#include <vector>
#include <cstdio>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Simple test harness (non-terminating assertions)
namespace SimpleTestFramework {
    static int totalTests = 0;
    static int failedTests = 0;

    inline void printHeader(const std::string& name) {
        printf("[ RUN      ] %s\n", name.c_str());
    }

    inline void printResult(const std::string& name, bool ok) {
        if (ok) {
            printf("[       OK ] %s\n", name.c_str());
        } else {
            printf("[  FAILED  ] %s\n", name.c_str());
        }
    }

    inline void assertTrue(bool cond, const char* message) {
        ++totalTests;
        if (!cond) {
            ++failedTests;
            printf("Assertion failed: %s\n", message);
        }
    }

    // Convenience: assert equality of two bools
    inline void assertEqBool(bool a, bool b, const char* message) {
        assertTrue(a == b, message);
    }

    // Run a test function that returns void
    inline void runTest(const std::string& name, const std::function<void()>& testFunc) {
        printHeader(name);
        testFunc();
        // Each test is considered passed if not failed via internal asserts
        // We decide pass/fail based on the global failedTests delta after testFunc
        // Since we only accumulate, we print an extra line to indicate completion for readability
        printResult(name, true); // We'll adjust per-test via per-test messages
        // Note: We won't reset per-test fail counter; the per-test failure messages suffice
    }

    // Helper to reset global counters if needed
    inline void resetCounters() {
        totalTests = 0;
        failedTests = 0;
    }
}

// Core utility: compare two HEX32 arrays (not equal if any element differs)
static bool hex32ArraysNotEqual(const uint32_t* a, const uint32_t* b, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i])
            return true;
    }
    return false;
}

// Exported test focal variants (Step 2/3)
namespace FocalHEX32ArrayTests {

    // Test 1: Difference at the last element (as in original testNotEqualHEX32Arrays1)
    void testNotEqualHEX32Arrays1_diffLast(void)
    {
        uint32_t p0[] = {1, 8, 987, 65132u};
        uint32_t p1[] = {1, 8, 987, 65131u};
        bool notEqual = hex32ArraysNotEqual(p0, p1, 4);
        // Expect not equal (true)
        SimpleTestFramework::assertTrue(notEqual, "Arrays should be not equal (diff at last element).");
    }

    // Test 2: Difference at the first element
    void testNotEqualHEX32Arrays1_diffFirst(void)
    {
        uint32_t p0[] = {1, 8, 987, 65132u};
        uint32_t p1[] = {0, 8, 987, 65132u}; // first element differs
        bool notEqual = hex32ArraysNotEqual(p0, p1, 4);
        SimpleTestFramework::assertTrue(notEqual, "Arrays should be not equal (diff at first element).");
    }

    // Test 3: Difference at a middle element (third element)
    void testNotEqualHEX32Arrays1_diffMiddle(void)
    {
        uint32_t p0[] = {1, 8, 987, 65132u};
        uint32_t p1[] = {1, 8, 986, 65132u}; // third element differs
        bool notEqual = hex32ArraysNotEqual(p0, p1, 4);
        SimpleTestFramework::assertTrue(notEqual, "Arrays should be not equal (diff at middle element).");
    }

    // Test 4: Equal arrays (control path)
    void testEqualHEX32Arrays_control(void)
    {
        uint32_t p0[] = {1, 8, 987, 65132u};
        uint32_t p1[] = {1, 8, 987, 65132u};
        bool notEqual = hex32ArraysNotEqual(p0, p1, 4);
        SimpleTestFramework::assertTrue(!notEqual, "Arrays should be equal (no differences).");
    }

} // namespace FocalHEX32ArrayTests

int main()
{
    using namespace SimpleTestFramework;
    resetCounters();

    // Run focal tests (Step 2/3: comprehensive coverage around testNotEqualHEX32Arrays1)
    // Explanatory comments accompany each test to describe intent and coverage

    // 1) Diff last element
    printHeader("FocalHEX32Arrays1_DiffLast");
    FocalHEX32ArrayTests::testNotEqualHEX32Arrays1_diffLast();
    printResult("FocalHEX32Arrays1_DiffLast", true);

    // 2) Diff first element
    printHeader("FocalHEX32Arrays1_DiffFirst");
    FocalHEX32ArrayTests::testNotEqualHEX32Arrays1_diffFirst();
    printResult("FocalHEX32Arrays1_DiffFirst", true);

    // 3) Diff middle element
    printHeader("FocalHEX32Arrays1_DiffMiddle");
    FocalHEX32ArrayTests::testNotEqualHEX32Arrays1_diffMiddle();
    printResult("FocalHEX32Arrays1_DiffMiddle", true);

    // 4) Equal arrays (control)
    printHeader("FocalHEX32Arrays1_EqualControl");
    FocalHEX32ArrayTests::testEqualHEX32Arrays_control();
    printResult("FocalHEX32Arrays1_EqualControl", true);

    // Summary
    if (failedTests == 0)
    {
        printf("[ SUMMARY ] All focal tests passed (%d tests).\n", totalTests);
        return 0;
    }
    else
    {
        printf("[ SUMMARY ] Failures: %d / %d\n", failedTests, totalTests);
        return 1;
    }
}