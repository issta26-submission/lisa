// A lightweight C++11 test harness to validate the focal Unity test
// function: testDoublesNotEqualPlusMinusInf, without requiring GTest.
// This harness mirrors the behavior of Unity's EXPECT_ABORT_BEGIN / TEST_ASSERT_EQUAL_DOUBLE
// by throwing exceptions when an assertion would fail, and catching them in the test runner
// so that test execution can continue for other tests.
//
// The goal is to exercise the focal method under both scenarios: when UNITY_EXCLUDE_DOUBLE is not
// defined (the test runs and is expected to abort) and (optionally) when UNITY_EXCLUDE_DOUBLE is defined
// (the test is effectively ignored). To achieve this, we implement two test variants guarded by
// <UNITY_EXCLUDE_DOUBLE> macro checks. The tests utilize infinity generation via division by zero
// and via std::numeric_limits<double>::infinity() to verify non-equality of plus/minus infinity.
//
// This file is self-contained and does not rely on Unity. It is intended to be compiled with C++11.
//
// Notes aligned to the instruction steps:
// - Step 1 (Program Understanding): The core behavior centers on comparing +Infinity and -Infinity,
//   which should not be equal. We model this via a tiny assertion that aborts on inequality.
// - Step 2 (Unit Test Generation): We provide test coverage for two approaches to generate Inf:
//   division by zero (1.0 / 0.0) and explicit infinity constants. We guard these with UNITY_EXCLUDE_DOUBLE.
// - Step 3 (Test Case Refinement): We keep tests deterministic, use standard library facilities, and
//   ensure tests either abort (as the original Unity test would) or are ignored as requested.
//

#include <cmath>
#include <exception>
#include <unity.h>
#include <limits>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple exception types to simulate Unity-like test outcomes
class AbortException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Test aborted due to assertion failure";
    }
};

class TestFailureException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Test failed: expected abort did not occur";
    }
};

// Lightweight test harness
struct TestCase {
    std::string name;
    std::function<void()> func;
    // In this harness, we treat tests that expect an abort as "expected to Abort"
    // via the AbortException. If that exception is thrown, the test passes.
    // If a TestFailureException is thrown, the test fails.
    bool expectAbort;
};

// Global storage of tests
static std::vector<TestCase> g_tests;

// Register a test into the harness
static void registerTest(const std::string& name, const std::function<void()>& f, bool expectAbort)
{
    g_tests.push_back({name, f, expectAbort});
}

// Macro-like helper to simulate TEST_ASSERT_EQUAL_DOUBLE
// It throws AbortException if the two doubles are not exactly equal.
static void test_assert_equal_double(double a, double b)
{
    if (!(a == b)) {
        throw AbortException();
    }
}

// D_ZERO and related infinite-value generation
#ifndef UNITY_EXCLUDE_DOUBLE
static const double d_zero = 0.0;
#endif

// Implementation of the focal test logic (two variants to cover different ways to produce Inf)
#ifndef UNITY_EXCLUDE_DOUBLE
// Variant 1: Generate +Inf and -Inf via division by zero
static void testDoublesNotEqualPlusMinusInf_DivByZero_impl()
{
    // This test expects the equality assertion to fail (i.e., abort)
    // because +Infinity and -Infinity are not equal.
    double a = 1.0 / d_zero;   // expected: +Inf
    double b = -1.0 / d_zero;  // expected: -Inf

    // If this assertion passes (which it shouldn't), raise a test failure to indicate the
    // test did not abort as expected.
    test_assert_equal_double(a, b); // Will throw AbortException if not equal
    throw TestFailureException();   // If no exception occurred, abort did not happen
}
#else
// If UNITY_EXCLUDE_DOUBLE is defined, this body is excluded.
static void testDoublesNotEqualPlusMinusInf_DivByZero_impl() {}
#endif

#ifndef UNITY_EXCLUDE_DOUBLE
// Variant 2: Generate Inf via explicit constants (no division by zero)
static void testDoublesNotEqualPlusMinusInf_InfConstants_impl()
{
    double a = std::numeric_limits<double>::infinity();
    double b = -std::numeric_limits<double>::infinity();

    test_assert_equal_double(a, b); // Expect abort due to inequality
    throw TestFailureException();   // If no exception occurred, abort did not happen
}
#else
static void testDoublesNotEqualPlusMinusInf_InfConstants_impl() {}
#endif

// Public wrappers to expose test scenarios to the harness, respecting UNITY_EXCLUDE_DOUBLE
#ifndef UNITY_EXCLUDE_DOUBLE
static void testDoublesNotEqualPlusMinusInf_DivByZero_wrapper()
{
    testDoublesNotEqualPlusMinusInf_DivByZero_impl();
}

static void testDoublesNotEqualPlusMinusInf_InfConstants_wrapper()
{
    testDoublesNotEqualPlusMinusInf_InfConstants_impl();
}
#else
static void testDoublesNotEqualPlusMinusInf_DivByZero_wrapper() { /* ignored when UNITY_EXCLUDE_DOUBLE defined */ }
static void testDoublesNotEqualPlusMinusInf_InfConstants_wrapper() { /* ignored when UNITY_EXCLUDE_DOUBLE defined */ }
#endif

// Main entry: register the tests
static void register_tests()
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // Both variants exercise the core predicate (division by zero and explicit infinity)
    // They should abort (success for the test) as per the focal method's intent.
    registerTest(
        "testDoublesNotEqualPlusMinusInf_DivByZero",
        []() { testDoublesNotEqualPlusMinusInf_DivByZero_wrapper(); },
        true  // expecting abort
    );
    registerTest(
        "testDoublesNotEqualPlusMinusInf_InfConstants",
        []() { testDoublesNotEqualPlusMinusInf_InfConstants_wrapper(); },
        true  // expecting abort
    );
#else
    // When doubles are excluded, we treat tests as ignored (no-op)
    registerTest(
        "testDoublesNotEqualPlusMinusInf_DivByZero (ignored)",
        []() { /* test ignored due to UNITY_EXCLUDE_DOUBLE */ },
        false
    );
    registerTest(
        "testDoublesNotEqualPlusMinusInf_InfConstants (ignored)",
        []() { /* test ignored due to UNITY_EXCLUDE_DOUBLE */ },
        false
    );
#endif
}

// Runner: executes all registered tests and reports outcomes
int main()
{
    register_tests();

    int total = static_cast<int>(g_tests.size());
    int passed = 0;
    int failed = 0;

    for (const auto& t : g_tests)
    {
        bool t_passed = true;
        bool exceptionCaught = false;
        try
        {
            t.func();
            // If the test function returns normally, success depends on whether
            // an abort was expected. If we expected an abort, this is a failure.
            if (t.expectAbort)
            {
                t_passed = false;
            }
        }
        catch (const AbortException&)
        {
            exceptionCaught = true;
            // Expected abort
            t_passed = t.expectAbort; // should be true
        }
        catch (const TestFailureException&)
        {
            exceptionCaught = true;
            // Test failed because abort did not occur (we threw explicit failure)
            t_passed = false;
        }
        catch (...)
        {
            exceptionCaught = true;
            // Unexpected exception: treat as failure to be strict
            t_passed = false;
        }

        if (t_passed && t.expectAbort)
        {
            ++passed;
            std::cout << "[PASS] " << t.name << " (expected abort occurred)\n";
        }
        else if (t_passed && !t.expectAbort)
        {
            ++passed;
            std::cout << "[PASS] " << t.name << "\n";
        }
        else
        {
            ++failed;
            std::cout << "[FAIL] " << t.name << "\n";
            if (exceptionCaught)
            {
                std::cout << "       Reason: exception captured during test execution.\n";
            }
            else
            {
                std::cout << "       Reason: test did not abort as expected.\n";
            }
        }
    }

    std::cout << "\nTest results: " << passed << " passed, " << failed << " failed, out of " << total << " tests.\n";

    return (failed == 0) ? 0 : 1;
}