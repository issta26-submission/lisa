// Minimal C++11 test harness for cmsInt32Number TestSingleFixed15_16(cmsFloat64Number d)
// This file provides a small, non-terminating test framework (no GoogleTest) and
// exercises the focal method TestSingleFixed15_16 with a set of inputs to cover
// true and false branches of its internal predicate.
//
// How to use:
// 1) Ensure testcms2.c (which defines TestSingleFixed15_16) is compiled and linked
//    with this test file.
// 2) Build with a C++11 compiler: e.g., g++ -std=c++11 test_fixed15_16.cpp testcms2.c -o test_fixed15_16
// 3) Run ./test_fixed15_16\n
//
// Notes:
// - We use a tiny test harness that reports failures but continues execution to maximize coverage.
// - We deliberately test both a normal value (true path) and values expected to overflow/flag false (false path).
// - The test uses only standard library facilities.

#include <cstdio>
#include <cmath>
#include <testcms2.h>


// Minimal type aliases matching the focal code expectations.
// We avoid including the full testcms2.h to keep the test self-contained and compatible
// with C++ compilation. The actual implementation in testcms2.c uses these types.
typedef int cmsInt32Number;
typedef double cmsFloat64Number;

// The focal function is defined in testcms2.c.
// We declare it with C linkage to ensure proper linking from C++.
extern "C" cmsInt32Number TestSingleFixed15_16(cmsFloat64Number d);

int main() {
    // Simple non-terminating test harness
    int totalTests = 0;
    int passedTests = 0;

    auto run = [&](const char* testName, bool condition) {
        ++totalTests;
        if (condition) {
            ++passedTests;
        } else {
            // Non-fatal failure: print diagnostic but continue
            std::fprintf(stderr, "Test failed: %s\n", testName);
        }
    };

    // Test 1: Input 0.0 should yield a precise round-trip with zero error
    // Expect true (non-zero return)
    {
        cmsFloat64Number input = 0.0;
        cmsInt32Number res = TestSingleFixed15_16(input);
        run("TestSingleFixed15_16(0.0) should be true (exact with zero)", res != 0);
    }

    // Test 2: Input 0.5 (exact binary fraction) should also round-trip exactly
    // Expect true
    {
        cmsFloat64Number input = 0.5;
        cmsInt32Number res = TestSingleFixed15_16(input);
        run("TestSingleFixed15_16(0.5) should be true (exact representable)", res != 0);
    }

    // Test 3: A typical non-exact value within representable range
    // Expect true (error should be within fixed precision)
    {
        cmsFloat64Number input = 1.234567;
        cmsInt32Number res = TestSingleFixed15_16(input);
        run("TestSingleFixed15_16(1.234567) should be true (within fixed precision)", res != 0);
    }

    // Test 4: Overflow/Out-of-range positive input
    // This is expected to fail (false) due to saturation or large error.
    {
        cmsFloat64Number input = 32768.0; // Just above the max representable 15.16 value (~32767.99998)
        cmsInt32Number res = TestSingleFixed15_16(input);
        run("TestSingleFixed15_16(32768.0) should be false (overflow)", res == 0);
    }

    // Test 5: Overflow/Out-of-range negative input
    // This is also expected to fail (false) due to saturation or large error.
    {
        cmsFloat64Number input = -32769.0; // Just below the min representable value (~-32768)
        cmsInt32Number res = TestSingleFixed15_16(input);
        run("TestSingleFixed15_16(-32769.0) should be false (underflow)", res == 0);
    }

    // Summary
    std::printf("Summary: %d/%d tests passed.\n", passedTests, totalTests);

    // Exit code: 0 if all tests passed, non-zero otherwise
    return (passedTests == totalTests) ? 0 : 1;
}