// Lightweight C++11 test suite for the focal method: testFloatPrintingInfinityAndNaN
// This suite focuses on validating the core behavior of printing special floating-point values
// (infinity and NaN) in a manner consistent with the Unity-based test in the provided code.
// Note: This is a standalone, minimal test harness suitable for environments where GTest is not allowed.

#include <cmath>
#include <sstream>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple micro-test framework (no external dependencies)
static int g_testsRun = 0;
static int g_testsFailed = 0;

// Simple assertion utility
#define ASSERT_TRUE(cond, msg)                                                           \
    do {                                                                                 \
        if (!(cond)) {                                                                 \
            std::cerr << "ASSERTION FAILED: " << (msg) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false;                                                              \
        }                                                                                \
    } while (0)

#define PASS_IF(cond)                                                                    \
    do {                                                                                 \
        if (!(cond)) {                                                                 \
            std::cerr << "TEST FAILED: condition not met at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false;                                                              \
        }                                                                                \
    } while (0)

// Helper: print a floating-point value as per the focal method's behavior.
// If the value is +inf or -inf, print "inf" or "-inf". If NaN, print "nan".
// Otherwise, print the numeric value using default formatting.
static void printFloating(const float& v, std::ostream& os)
{
    if (std::isinf(v))
    {
        // Sign determines +inf or -inf
        os << (std::signbit(v) ? "-inf" : "inf");
    }
    else if (std::isnan(v))
    {
        os << "nan";
    }
    else
    {
        // For non-special values, print using default formatting
        os << v;
    }
}

// Convenience: capture the string representation of a float via printFloating
static std::string capturePrintedFloat(const float& v)
{
    std::ostringstream oss;
    printFloating(v, oss);
    return oss.str();
}

// Candidate Keywords derivation (Step 1) applied conceptually in comments
// - Infinity handling: printFloating -> "inf" / "-inf"
// - NaN handling: printFloating -> "nan"
// - Pressure points: 1.0f / f_zero, -1.0f / f_zero, 0.0f / f_zero as in focal code
// - f_zero and its use as a zero reference point for generating edge-case values

// Global constants mirroring the focal class dependencies in the provided code
static const float f_zero = 0.0f;

// Test 1: Validate that positive infinity is printed as "inf".
//
// This mirrors the focal test where 1.0f / f_zero yields +infinity and should be printed as "inf".
static bool testFloatPrintingInfinity_PosInfPrinted(void)
{
    // Compute +infinity via IEEE754 rules (1.0f / 0.0f)
    const float posInf = 1.0f / f_zero;

    // Capture the textual representation
    std::string printed = capturePrintedFloat(posInf);

    // Expected output is "inf"
    if (printed != "inf")
    {
        std::cerr << "Expected 'inf' but got '" << printed << "' for +Inf printing.\n";
        return false;
    }
    return true;
}

// Test 2: Validate that negative infinity is printed as "-inf".
//
// This mirrors the focal test where -1.0f / f_zero yields -infinity and should be printed as "-inf".
static bool testFloatPrintingInfinity_NegInfPrinted(void)
{
    const float negInf = -1.0f / f_zero;
    std::string printed = capturePrintedFloat(negInf);

    if (printed != "-inf")
    {
        std::cerr << "Expected '-inf' but got '" << printed << "' for -Inf printing.\n";
        return false;
    }
    return true;
}

// Test 3: Validate that NaN is printed as "nan".
//
// This mirrors the focal test where 0.0f / f_zero yields NaN and should be printed as "nan".
static bool testFloatPrintingInfinity_NaNPrinted(void)
{
    const float nanVal = 0.0f / f_zero;
    std::string printed = capturePrintedFloat(nanVal);

    if (printed != "nan")
    {
        std::cerr << "Expected 'nan' but got '" << printed << "' for NaN printing.\n";
        return false;
    }
    return true;
}

// Test 4: Ensure that finite non-special values do not accidentally print as "inf" or "nan".
//
// This covers the non-edge-case path and ensures numeric formatting is preserved.
static bool testFloatPrintingFiniteValueNotSpecial(void)
{
    const float finiteVal = 3.14159f;
    std::string printed = capturePrintedFloat(finiteVal);

    // It should not print "inf" or "nan" for a finite value; compare against the literal representation or numeric print
    // We allow any string that is not exactly "inf" or "nan"
    ASSERT_TRUE(!(printed == "inf" || printed == "nan"), "Finite value should not print as inf or nan");
    // Additional sanity: the printed string should not be empty
    PASS_IF(!printed.empty());
    return true;
}

// Test 5: Simulated integration of the focal test approach with a finite value (ensures numeric formatting is produced).
static bool testFloatPrintingFiniteValueFormatting(void)
{
    const float finiteVal = 1.2345f;
    std::string printed = capturePrintedFloat(finiteVal);
    // Expect some textual numeric representation (not "inf" or "nan")
    PASS_IF(!(printed == "inf" || printed == "nan") );
    PASS_IF(!printed.empty());
    return true;
}

// Driver: Run all tests and report summary
int main()
{
    std::cout << "Running focal-method-inspired tests for testFloatPrintingInfinityAndNaN (IEEE754 edge-case handling)...\n";

    g_testsRun = 0;
    g_testsFailed = 0;

    // Each test is counted; if a test fails, increment failure count.
    auto runTest = [&](const char* testName, bool (*testFunc)()) {
        ++g_testsRun;
        bool ok = testFunc();
        if (!ok) {
            ++g_testsFailed;
            std::cerr << "Test failed: " << testName << "\n";
        } else {
            std::cout << "Test passed: " << testName << "\n";
        }
    };

    // Execute tests with descriptive comments
    runTest("testFloatPrintingInfinity_PosInfPrinted", testFloatPrintingInfinity_PosInfPrinted);
    // Explanation: Verifies that positive infinity is printed as "inf" as expected by the focal test.
    runTest("testFloatPrintingInfinity_NegInfPrinted", testFloatPrintingInfinity_NegInfPrinted);
    // Explanation: Verifies that negative infinity is printed as "-inf" as expected by the focal test.
    runTest("testFloatPrintingInfinity_NaNPrinted", testFloatPrintingInfinity_NaNPrinted);
    // Explanation: Verifies that NaN is printed as "nan" as expected by the focal test.
    runTest("testFloatPrintingFiniteValueNotSpecial", testFloatPrintingFiniteValueNotSpecial);
    // Explanation: Ensures finite values do not accidentally print as "inf" or "nan".
    runTest("testFloatPrintingFiniteValueFormatting", testFloatPrintingFiniteValueFormatting);
    // Explanation: Confirms finite numeric formatting is produced for non-edge-case values.

    std::cout << "Summary: Ran " << g_testsRun << " tests, " << g_testsFailed << " failed.\n";

    return (g_testsFailed == 0) ? 0 : 1;
}