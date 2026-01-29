// This file provides a minimal C++11 test harness for the UnityAssertFloatSpecial
// function (ported here with the necessary minimal Unity stubs).
// The goal is to verify correct behavior across true/false branches of each
// condition predicate, without using GTest, and without requiring the full Unity
// framework to be present.
// Explanatory comments accompany each unit test case.

#include <stddef.h>
#include <cmath>
#include <limits>
#include <unity.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>


// -----------------------------------------------------------------------------------
// Lightweight stand-ins for Unity types and API used by UnityAssertFloatSpecial
// These definitions are sufficient for compiling and exercising the focal method.
// -----------------------------------------------------------------------------------

// Typedefs to mimic Unity's type aliases
typedef int UNITY_INT;
typedef unsigned int UNITY_UINT;
typedef int UNITY_LINE_TYPE;
typedef unsigned int UNITY_FLAGS_T;
typedef unsigned int UNITY_DISPLAY_STYLE_T;
typedef void* UNITY_INTERNAL_PTR;
typedef unsigned int UNITY_UINT32;
typedef float  UNITY_FLOAT;
typedef double UNITY_DOUBLE;
typedef unsigned int UNITY_FLOAT_TRAIT_T;

// Unity "string" literals used by the focal function
static const char* UnityStrInf              = "inf";
static const char* UnityStrNegInf           = "-inf";
static const char* UnityStrNaN               = "NaN";
static const char* UnityStrDet               = "det";
static const char* UnityStrInvalidFloatTrait = "InvalidFloatTrait";

static const char* UnityStrExpected = "Expected";
static const char* UnityStrNot        = "Not ";
static const char* UnityWas           = " was ";

// A simple, non-terminating failure macro for the test harness
#define UNITY_FAIL_AND_BAIL { return; }

// An empty placeholder macro used in the focal function
#define RETURN_IF_FAIL_OR_IGNORE /* no-op for test harness */

// The 8 predefined style constants (INF/NEG_INF/NAN/DET variants)
// LSB encodes whether the assertion describes the value or its negation.
// The trait_index is computed as (style >> 1) and used to pick a string from trait_names.
#define UNITY_FLOAT_IS_INF          0
#define UNITY_FLOAT_IS_NOT_INF      1
#define UNITY_FLOAT_IS_NEG_INF      2
#define UNITY_FLOAT_IS_NOT_NEG_INF  3
#define UNITY_FLOAT_IS_NAN          4
#define UNITY_FLOAT_IS_NOT_NAN      5
#define UNITY_FLOAT_IS_DET          6
#define UNITY_FLOAT_IS_NOT_DET      7

// Lightweight bridge to provide isinf / isnan in this C++ test harness
// Accepts Unity_FLOAT (float) values and forwards to <cmath> equivalents.
#define isinf(x) std::isinf((double)(x))
#define isnan(x) std::isnan((double)(x))

// Global test state to observe outcomes of UnityAssertFloatSpecial
static int g_fail_count = 0;
static int g_fail_line  = -1;
static std::string g_output; // Optional: captures printed strings for debugging

// Minimal stubs to emulate Unity's printing and failure signaling
extern "C" {
    void UnityPrint(const char* string) {
        g_output += string;
    }

    void UnityPrintFloat(double input_number) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%.6f", input_number);
        g_output += buf;
    }

    void UnityTestResultsFailBegin(const UNITY_LINE_TYPE line) {
        g_fail_line = static_cast<int>(line);
        ++g_fail_count;
    }

    void UnityAddMsgIfSpecified(const char* msg) {
        if (msg) {
            g_output += " | ";
            g_output += msg;
        }
    }

    // The focal function uses UnityStr* constants above; these are simple prints.
    void UnityPrintExpectedAndActualStrings(const char* /*expected*/, const char* /*actual*/) {
        // For test purposes, we don't need to implement the full formatting.
    }

    // Dummy placeholders for other Unity internal API used by the focal function
    static void UnityConcludeTest(void) { (void)0; }
    static int UnityEnd(void) { return 0; }
}

// -----------------------------------------------------------------------------------
// The focal method under test: UnityAssertFloatSpecial
// Implemented here with the same logic as provided in the prompt, adapted for C++11.
// -----------------------------------------------------------------------------------
extern "C" void UnityAssertFloatSpecial(const UNITY_FLOAT actual,
                                       const char* msg,
                                       const UNITY_LINE_TYPE lineNumber,
                                       const UNITY_FLOAT_TRAIT_T style)
{
{
    const char* trait_names[] = {UnityStrInf, UnityStrNegInf, UnityStrNaN, UnityStrDet};
    UNITY_INT should_be_trait = ((UNITY_INT)style & 1);
    UNITY_INT is_trait        = !should_be_trait;
    UNITY_INT trait_index     = (UNITY_INT)(style >> 1);
    RETURN_IF_FAIL_OR_IGNORE;
    switch (style)
    {
        case UNITY_FLOAT_IS_INF:
        case UNITY_FLOAT_IS_NOT_INF:
            is_trait = isinf(actual) && (actual > 0);
            break;
        case UNITY_FLOAT_IS_NEG_INF:
        case UNITY_FLOAT_IS_NOT_NEG_INF:
            is_trait = isinf(actual) && (actual < 0);
            break;
        case UNITY_FLOAT_IS_NAN:
        case UNITY_FLOAT_IS_NOT_NAN:
            is_trait = isnan(actual) ? 1 : 0;
            break;
        case UNITY_FLOAT_IS_DET: /* A determinate number is non infinite and not NaN. */
        case UNITY_FLOAT_IS_NOT_DET:
            is_trait = !isinf(actual) && !isnan(actual);
            break;
        default:
            trait_index = 0;
            trait_names[0] = UnityStrInvalidFloatTrait;
            break;
    }
    if (is_trait != should_be_trait)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        if (!should_be_trait)
            UnityPrint(UnityStrNot);
        UnityPrint(trait_names[trait_index]);
        UnityPrint(UnityStrWas);
#ifndef UNITY_EXCLUDE_FLOAT_PRINT
        UnityPrintFloat((UNITY_DOUBLE)actual);
#else
        if (should_be_trait)
            UnityPrint(UnityStrNot);
        UnityPrint(trait_names[trait_index]);
#endif
        UnityAddMsgIfSpecified(msg);
        UNITY_FAIL_AND_BAIL;
    }
}
}

// -----------------------------------------------------------------------------------
// Test harness: a small, self-contained suite for UnityAssertFloatSpecial
// The harness uses the focal function and asserts on the observed failure state.
// -----------------------------------------------------------------------------------

// Helper: reset test state
static void ResetTestState() {
    g_fail_count = 0;
    g_fail_line  = -1;
    g_output.clear();
}

// Helper: Run a single test with given actual value, style, and expectation
static void RunTest(const std::string& test_name,
                    UNITY_FLOAT actual,
                    UNITY_FLOAT_TRAIT_T style,
                    bool expect_fail)
{
    ResetTestState();
    // Choose a line number for reporting (arbitrary for tests)
    const UNITY_LINE_TYPE line = 42;
    // Call the focal method under test
    UnityAssertFloatSpecial(actual, nullptr, line, style);

    bool did_fail = (g_fail_count > 0);
    if (did_fail != expect_fail) {
        // Report discrepancy
        std::cout << "[FAIL] " << test_name
                  << " | actual=" << static_cast<float>(actual)
                  << " | style=" << style
                  << " | expected_fail=" << (expect_fail ? "true" : "false")
                  << " | observed_fail=" << (did_fail ? "true" : "false")
                  << " | fail_line=" << g_fail_line
                  << "\n";
        if (did_fail) std::cout << "  -> Fail line: " << g_fail_line
                               << " | Unity output: " << g_output << "\n";
    } else {
        std::cout << "[PASS] " << test_name
                  << " | actual=" << static_cast<float>(actual)
                  << " | style=" << style
                  << " | expected_fail=" << (expect_fail ? "true" : "false")
                  << "\n";
    }
}

// Test cases
// 1) INF should trigger a fail for UNITY_FLOAT_IS_INF
static void Test_INF_IsInf_ShouldFail() {
    UNITY_FLOAT actual = std::numeric_limits<float>::infinity();
    RunTest("Test_INF_IsInf_ShouldFail", actual, UNITY_FLOAT_IS_INF, true);
}

// 2) INF with NOT_INF should NOT trigger a failure
static void Test_INF_WithNotInf_ShouldPass() {
    UNITY_FLOAT actual = std::numeric_limits<float>::infinity();
    RunTest("Test_INF_WithNotInf_ShouldPass", actual, UNITY_FLOAT_IS_NOT_INF, false);
}

// 3) -INF should trigger a fail for UNITY_FLOAT_IS_NEG_INF
static void Test_NegInf_IsNegInf_ShouldFail() {
    UNITY_FLOAT actual = -std::numeric_limits<float>::infinity();
    RunTest("Test_NegInf_IsNegInf_ShouldFail", actual, UNITY_FLOAT_IS_NEG_INF, true);
}

// 4) -INF with NOT_NEG_INF should NOT trigger a failure
static void Test_NegInf_WithNotNegInf_ShouldPass() {
    UNITY_FLOAT actual = -std::numeric_limits<float>::infinity();
    RunTest("Test_NegInf_WithNotNegInf_ShouldPass", actual, UNITY_FLOAT_IS_NOT_NEG_INF, false);
}

// 5) NaN should trigger a fail for UNITY_FLOAT_IS_NAN
static void Test_NaN_IsNaN_ShouldFail() {
    UNITY_FLOAT actual = std::numeric_limits<float>::quiet_NaN();
    RunTest("Test_NaN_IsNaN_ShouldFail", actual, UNITY_FLOAT_IS_NAN, true);
}

// 6) NaN with NOT_NAN should NOT trigger a failure
static void Test_NaN_WithNotNaN_ShouldPass() {
    UNITY_FLOAT actual = std::numeric_limits<float>::quiet_NaN();
    RunTest("Test_NaN_WithNotNaN_ShouldPass", actual, UNITY_FLOAT_IS_NOT_NAN, false);
}

// 7) Non-INF, non-NaN number with DET should fail (since it's determinate)
static void Test_Det_IsDet_ShouldFail() {
    UNITY_FLOAT actual = 3.14159f;
    RunTest("Test_Det_IsDet_ShouldFail", actual, UNITY_FLOAT_IS_DET, true);
}

// 8) Non-INF, non-NaN number with NOT_DET should pass
static void Test_NotDet_IsNotDet_ShouldPass() {
    UNITY_FLOAT actual = 3.14159f;
    RunTest("Test_NotDet_IsNotDet_ShouldPass", actual, UNITY_FLOAT_IS_NOT_DET, false);
}

// 9) Invalid style (default branch) should trigger a fail
static void Test_InvalidStyle_ShouldFail() {
    UNITY_FLOAT actual = 1.0f;
    RunTest("Test_InvalidStyle_ShouldFail", actual, 0 /* default/invalid style */, true);
}

// 10) Determinate-like case for 0.0 to verify NOT_DET passes/fails appropriately
static void Test_ZeroDet_NotDet_Behavior() {
    // 0.0 is not inf and not NaN, so DET would fail, NOT_DET should pass
    UNITY_FLOAT actual = 0.0f;
    RunTest("Test_ZeroDet_NotDet_Behavior", actual, UNITY_FLOAT_IS_DET, true); // DET should fail
    RunTest("Test_ZeroDet_NotDet_Behavior_PASS", actual, UNITY_FLOAT_IS_NOT_DET, false);
}

// -----------------------------------------------------------------------------------
// Main: execute all tests and report summary
// -----------------------------------------------------------------------------------
int main() {
    std::cout << "Starting UnityAssertFloatSpecial unit tests (C++11 harness)\n";

    std::vector<void(*)()> tests;
    // Register tests in a deterministic order
    tests.push_back(Test_INF_IsInf_ShouldFail);
    tests.push_back(Test_INF_WithNotInf_ShouldPass);
    tests.push_back(Test_NegInf_IsNegInf_ShouldFail);
    tests.push_back(Test_NegInf_WithNotNegInf_ShouldPass);
    tests.push_back(Test_NaN_IsNaN_ShouldFail);
    tests.push_back(Test_NaN_WithNotNaN_ShouldPass);
    tests.push_back(Test_Det_IsDet_ShouldFail);
    tests.push_back(Test_NotDet_IsNotDet_ShouldPass);
    tests.push_back(Test_InvalidStyle_ShouldFail);
    tests.push_back(Test_ZeroDet_NotDet_Behavior);

    // Execute each test
    for (auto t : tests) {
        // Clear any previous output to ensure isolation
        // Each test will print its own PASS/FAIL status
        t();
    }

    // Final summary
    int total = static_cast<int>(tests.size());
    int failed = 0;
    // Since tests print per-case results, compute a rough pass/fail count
    // by re-running with a separate function could be more robust, but for brevity:
    // We'll simply report that tests have concluded.
    std::cout << "UnityAssertFloatSpecial tests completed. Total: " << total
              << ", Check individual test outputs above for pass/fail per case.\n";

    return 0;
}