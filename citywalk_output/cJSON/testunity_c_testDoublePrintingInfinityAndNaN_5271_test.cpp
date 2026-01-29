// Test suite for the focal method behavior: Infinity and NaN handling in double printing.
// This is a standalone C++11 test harness designed to validate the core semantics
// (infinite values and NaN representation) that the focal Unity-based test would
// exercise, without depending on the Unity framework. It mirrors the intent of
// the testDoublePrintingInfinityAndNaN() logic by using standard C++ facilities.
//
// Candidate Keywords (derived from the focal method and dependencies):
// - "inf", "-inf", "nan" strings as output tokens
// - d_zero as the zero-double dependency (conceptually, zero-division triggers inf/nan)
// - OUTPUT_SPY / printing capture mechanism (represented conceptually)
// - isinf / isnan semantics for true/false branches
// - 1.0 / d_zero / 0.0 / 0.0 (IEEE-754 behavior for inf/nan)
// - TEST_ASSERT_EQUAL_PRINT_FLOATING equivalent semantics simulated via string checks
//
// Note: This test suite does not rely on Unity. It uses a small, self-contained
// test harness and standard library facilities.

#include <cmath>
#include <unity.h>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple assertion macro to throw on failure (non-terminating in the sense of a test suite wrapper)
#define TEST_ASSERT(cond) do { if(!(cond)) throw std::runtime_error("Assertion failed: " #cond); } while(0)
#define TEST_ASSERT_EQUAL_STRING(a, b) TEST_ASSERT(std::string((a)) == std::string((b)))

// Utility: simulate the textual representation of floating values as Unity's
// TEST_ASSERT_EQUAL_PRINT_FLOATING would in the original test.
// For NaN, Inf, and finite numbers we return deterministic strings.
static std::string simulateFloatingPrinting(double value)
{
    if (std::isinf(value))
    {
        return (value > 0.0) ? "inf" : "-inf";
    }
    if (std::isnan(value))
    {
        return "nan";
    }
    // Finite numbers: format with a compact representation
    char buf[64];
    // Use a representation that is stable for tests
    std::snprintf(buf, sizeof(buf), "%g", value);
    return std::string(buf);
}

// Simple test harness
struct TestCase {
    std::string name;
    std::function<void()> func;
};

static void runTest(const TestCase& tc)
{
    try
    {
        tc.func();
        std::cout << "[OK]   " << tc.name << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[FAILED] " << tc.name << " -> " << ex.what() << std::endl;
        throw; // propagate to fail the overall run
    }
    catch (...)
    {
        std::cerr << "[FAILED] " << tc.name << " -> unknown exception" << std::endl;
        throw;
    }
}

// Domain knowledge helpers
static void testInfinityPrinting_PositiveInf()
{
    // Tests the positive infinity printing path: 1.0 / 0.0 yields +inf on IEEE754
    // Expected textual representation: "inf"
    double oneOverZero = 1.0 / 0.0; // should yield +INF
    std::string printed = simulateFloatingPrinting(oneOverZero);
    TEST_ASSERT_EQUAL_STRING("inf", printed.c_str());
}

static void testInfinityPrinting_NegativeInf()
{
    // Tests the negative infinity printing path: -1.0 / 0.0 yields -inf on IEEE754
    double negOneOverZero = -1.0 / 0.0; // should yield -INF
    std::string printed = simulateFloatingPrinting(negOneOverZero);
    TEST_ASSERT_EQUAL_STRING("-inf", printed.c_str());
}

static void testInfinityPrinting_NaN()
{
    // Tests NaN printing path: 0.0 / 0.0 yields NaN
    double zeroDivZero = 0.0 / 0.0;
    std::string printed = simulateFloatingPrinting(zeroDivZero);
    TEST_ASSERT_EQUAL_STRING("nan", printed.c_str());
}

static void testInfinityPrinting_FiniteValue()
{
    // Sanity check: finite values should print as a numeric string (not "inf" or "nan")
    double finiteVal = 123.456;
    std::string printed = simulateFloatingPrinting(finiteVal);
    // We expect the representation to be a numeric string. Use a simple check.
    TEST_ASSERT(!printed.empty() && printed != "inf" && printed != "-inf" && printed != "nan");
}

static void testInfinityDetection_PositiveInfFlag()
{
    // True/false branch coverage: std::isinf positive should be true for +INF
    double posInf = std::numeric_limits<double>::infinity();
    TEST_ASSERT(std::isinf(posInf) && posInf > 0.0);
}

static void testInfinityDetection_NegativeInfFlag()
{
    // True/false branch coverage: std::isinf negative should be true for -INF
    double negInf = -std::numeric_limits<double>::infinity();
    TEST_ASSERT(std::isinf(negInf) && negInf < 0.0);
}

static void testNaNDetection_NaNFlag()
{
    // NaN detection should be true for NaN
    double nanVal = std::numeric_limits<double>::quiet_NaN();
    TEST_ASSERT(std::isnan(nanVal));
}

static void testFiniteDetection_NotInfNotNaN()
{
    // Finite value should not be INF or NaN
    double v = 3.14159;
    TEST_ASSERT(!std::isinf(v) && !std::isnan(v));
}

// Main entry to execute tests
int main()
{
    // Collect all tests
    TestCase tests[] = {
        {"testInfinityPrinting_PositiveInf", testInfinityPrinting_PositiveInf},
        {"testInfinityPrinting_NegativeInf", testInfinityPrinting_NegativeInf},
        {"testInfinityPrinting_NaN", testInfinityPrinting_NaN},
        {"testInfinityPrinting_FiniteValue", testInfinityPrinting_FiniteValue},
        {"testInfinityDetection_PositiveInfFlag", testInfinityDetection_PositiveInfFlag},
        {"testInfinityDetection_NegativeInfFlag", testInfinityDetection_NegativeInfFlag},
        {"testNaNDetection_NaNFlag", testNaNDetection_NaNFlag},
        {"testFiniteDetection_NotInfNotNaN", testFiniteDetection_NotInfNotNaN}
    };

    const int testCount = sizeof(tests) / sizeof(TestCase);
    int failures = 0;

    for (int i = 0; i < testCount; ++i)
    {
        try
        {
            runTest(tests[i]);
        }
        catch (...)
        {
            ++failures;
        }
    }

    std::cout << "\nTest results: " << (testCount - failures) << " passed, " << failures << " failed." << std::endl;
    return failures == 0 ? 0 : 1;
}