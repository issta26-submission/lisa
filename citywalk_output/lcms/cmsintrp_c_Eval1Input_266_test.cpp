// Unit test suite for Eval1Input in cmsintrp.c
// No Google Test used. A small in-process test harness is provided.
// The tests exercise the Eval1Input function from LittleCMS-like interpolation code.
// The test relies on the actual library types and function implementations
// (e.g., cmsUInt16Number, cmsInterpParams, _cmsToFixedDomain, LinearInterp, etc.).
// To compile and run, include the library's headers (lcms2_internal.h) and link the cmsintrp.c
// into the test build.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>


// Include internal LittleCMS headers to obtain necessary types and function prototypes.
// If your environment uses different include guards or paths, adjust accordingly.

// Declare the function under test with C linkage for compatibility when linking with cmsintrp.c
extern "C" void Eval1Input(const cmsUInt16Number Input[],
                           cmsUInt16Number Output[],
                           const cmsInterpParams* p16);

// Simple test harness state
static int g_TestFailures = 0;

// Helper to report a test failure (non-terminating, logs and continues)
static void report_failure(const char* test_name, const char* msg)
{
    std::cerr << "[FAIL] " << test_name << ": " << msg << std::endl;
    ++g_TestFailures;
}

// Helper to run all tests and report summary
static void summarize_tests()
{
    if (g_TestFailures == 0) {
        std::cout << "[OK] All tests passed.\n";
    } else {
        std::cout << "[DONE] " << g_TestFailures << " test(s) failed.\n";
    }
}

// Test 1: Last-value path when Input[0] == 0xffff
// Expected behavior: Output[OutChan] should equal LutTable[y0 + OutChan]
// where y0 = Domain[0] * opta[0].
static void test_Eval1Input_LastValue_InputFFFF()
{
    // Domain and interpolation table setup
    // Use small values to keep the expected indices within lut bounds.
    // Domain[0] = 2, opta[0] = 4 => y0 = 8
    // We'll provide 2 outputs to keep expectations simple: lut[8], lut[9]
    const size_t nOutputs = 2;
    std::vector cmsUInt16Number> lut(12);
    for (size_t i = 0; i < lut.size(); ++i) lut[i] = static_cast<cmsUInt16Number>(i); // simple 0..11

    cmsInterpParams p;
    // Initialize p with real library fields (as expected by Eval1Input)
    p.Table = lut.data();
    p.Domain[0] = 2;        // Domain factor
    p.opta[0] = 4;           // Fixed-point-like scalar
    p.nOutputs = static_cast<cmsUInt32Number>(nOutputs);

    cmsUInt16Number Input[1] = { 0xffff }; // last-value flag
    cmsUInt16Number Output[2] = { 0 };

    // Call method under test
    Eval1Input(Input, Output, &p);

    // Expected values
    cmsUInt32Number y0 = p.Domain[0] * p.opta[0]; // 2 * 4 = 8
    unsigned expected0 = lut[y0 + 0];
    unsigned expected1 = lut[y0 + 1];

    // Verify results (non-terminating, log on mismatch)
    if (Output[0] != expected0) {
        report_failure("Eval1Input_LastValue_InputFFFF", "Output[0] does not match expected LutTable[y0]");
    }
    if (Output[1] != expected1) {
        report_failure("Eval1Input_LastValue_InputFFFF", "Output[1] does not match expected LutTable[y0+1]");
    }

    // Explanatory: This test validates the true true-branch when the "last value"
    // condition is met. It checks that proper LUT indexing occurs for the last value case.
}

// Test 2: Last-value path when Domain[0] == 0 (regardless of Input not equal to 0xffff)
// Expected Output should be the first nOutputs elements of LutTable (y0 = 0)
static void test_Eval1Input_LastValue_DomainZero()
{
    // Setup a LUT with known values
    const size_t nOutputs = 2;
    std::vector cmsUInt16Number> lut = { 10, 11, 12, 13, 14, 15 }; // simple progressive values

    cmsInterpParams p;
    p.Table = lut.data();
    p.Domain[0] = 0; // triggers the if-branch
    p.opta[0] = 7;  // arbitrary
    p.nOutputs = static_cast<cmsUInt32Number>(nOutputs);

    cmsUInt16Number Input[1] = { 5 }; // not 0xffff, but Domain[0] == 0 will force the first branch
    cmsUInt16Number Output[2] = { 0 };

    Eval1Input(Input, Output, &p);

    unsigned expected0 = lut[0]; // y0 = 0 * opta[0] = 0
    unsigned expected1 = lut[1];

    if (Output[0] != expected0) {
        report_failure("Eval1Input_LastValue_DomainZero", "Output[0] does not match LutTable[0]");
    }
    if (Output[1] != expected1) {
        report_failure("Eval1Input_LastValue_DomainZero", "Output[1] does not match LutTable[1]");
    }

    // Explanatory: Verifies the domain-zero condition yields the initial LUT values.
}

// Test 3: Else-path (typical interpolation path) with Input[0] = 0 and non-zero Domain[0].
// We expect rk = 0 (since v = Input[0] * Domain[0] = 0, and _cmsToFixedDomain(0) yields 0.0...)
// Consequently, LinearInterp(0, Lut[K0+OutChan], Lut[K1+OutChan]) should resolve to Lut[K0+OutChan].
// Choose lut values so that expected outputs are Lut[0] and Lut[1] for nOutputs=2.
static void test_Eval1Input_ElsePath_InputZero()
{
    const size_t nOutputs = 2;
    // lut values chosen so that expected = lut[0] and lut[1]
    std::vector cmsUInt16Number> lut = { 0, 10, 20, 30, 40, 50 };

    cmsInterpParams p;
    p.Table = lut.data();
    p.Domain[0] = 1; // non-zero to enter else-path
    p.opta[0] = 3;  // some small fixed-point multiplier
    p.nOutputs = static_cast<cmsUInt32Number>(nOutputs);

    cmsUInt16Number Input[1] = { 0 }; // else-path with v = 0
    cmsUInt16Number Output[2] = { 0 };

    Eval1Input(Input, Output, &p);

    // Expectation based on rk = 0 => Output[OutChan] == Lut[K0 + OutChan], with K0 = opta[0] * k0 and k0=0
    // So K0 = 0, hence Output should be lut[0], lut[1]
    unsigned expected0 = lut[0];
    unsigned expected1 = lut[1];

    if (Output[0] != expected0) {
        report_failure("Eval1Input_ElsePath_InputZero", "Output[0] did not match expected lut[0]");
    }
    if (Output[1] != expected1) {
        report_failure("Eval1Input_ElsePath_InputZero", "Output[1] did not match expected lut[1]");
    }

    // Explanatory: Ensures the else-path with zero input yields left-end LUT interpolation value.
}

// Main function to run all tests
int main()
{
    std::cout << "Running Eval1Input unit tests...\n";

    test_Eval1Input_LastValue_InputFFFF();
    test_Eval1Input_LastValue_DomainZero();
    test_Eval1Input_ElsePath_InputZero();

    summarize_tests();
    return g_TestFailures == 0 ? 0 : 1;
}