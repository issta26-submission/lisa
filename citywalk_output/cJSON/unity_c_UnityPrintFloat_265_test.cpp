// C++11 test suite for UnityPrintFloat in unity.c without GTest
// This test harness hooks Unity's output via UnityOutputChar to capture printed characters.
// It then asserts expected output strings for a variety of inputs, covering true/false branches.
// The tests are self-contained and invoked from main().

#include <stddef.h>
#include <cmath>
#include <unity.h>
#include <iostream>
#include <string>


// Declaration of the focal function from the C Unity library.
// We assume UNITY_DOUBLE is typedef'd to double in the actual library.
extern "C" void UnityPrintFloat(const double input_number);

// Hook for Unity's output character function.
// The Unity code uses UnityOutputChar via the UNITY_OUTPUT_CHAR macro.
// We provide a C-compatible implementation to capture output in a C++ test.
extern "C" void UnityOutputChar(char c);

// Buffer to collect all characters emitted by UnityPrintFloat via UnityOutputChar.
static std::string UnityOutputBuffer;

// Implementation of the output hook to capture printed characters.
extern "C" void UnityOutputChar(char c)
{
    UnityOutputBuffer.push_back(c);
}

// Helper to reset the captured output buffer before each test.
static void reset_output_buffer() {
    UnityOutputBuffer.clear();
}

// Simple test harness assertion with explanatory comment.
// Prints PASS/FAIL for each test with a short description.
static void assert_equal(const std::string& test_name,
                         const std::string& expected,
                         const std::string& actual)
{
    if (actual == expected) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name
                  << " | Expected: \"" << expected
                  << "\" | Actual: \"" << actual << "\"\n";
    }
}

// Individual unit tests with comments describing the scenario and expected outcomes.

// Test 1: Negative zero should print a minus sign followed by zero ("-0").
// This exercises the sign handling branch for zero values.
static void test_negative_zero_prints_minus_zero()
{
    reset_output_buffer();
    UnityPrintFloat(-0.0);
    assert_equal("Negative zero prints -0", "-0", UnityOutputBuffer);
}

// Test 2: Positive zero should print just "0" without a sign.
// Validates the zero handling path when number == 0.
static void test_zero_prints_zero()
{
    reset_output_buffer();
    UnityPrintFloat(0.0);
    assert_equal("Zero prints 0", "0", UnityOutputBuffer);
}

// Test 3: NaN should print "nan" exactly.
// Branch for isnan(number) -> "nan".
static void test_nan_prints_nan()
{
    reset_output_buffer();
    UnityPrintFloat(std::numeric_limits<double>::quiet_NaN());
    assert_equal("NaN prints nan", "nan", UnityOutputBuffer);
}

// Test 4: Positive infinity should print "inf".
// Branch for isinf(number) when number is +inf.
static void test_infinity_prints_inf()
{
    reset_output_buffer();
    UnityPrintFloat(std::numeric_limits<double>::infinity());
    assert_equal("Infinity prints inf", "inf", UnityOutputBuffer);
}

// Test 4b: Negative infinity should print "-inf".
// Sign handling followed by "inf".
static void test_negative_infinity_prints_minus_inf()
{
    reset_output_buffer();
    UnityPrintFloat(-std::numeric_limits<double>::infinity());
    assert_equal("Negative Infinity prints -inf", "-inf", UnityOutputBuffer);
}

// Test 5: Positive integer should print without decimal point (e.g., 12 -> "12").
// Exercises path where exponent results in 0 decimals and integer output.
static void test_positive_integer_prints_without_decimal()
{
    reset_output_buffer();
    UnityPrintFloat(12.0);
    assert_equal("Positive integer prints 12", "12", UnityOutputBuffer);
}

// Test 6: Large number prints in scientific notation, e.g., 1e6 -> "1e+06".
// This checks the exponent formatting branch.
static void test_large_number_prints_scientific_notation()
{
    reset_output_buffer();
    UnityPrintFloat(1000000.0); // 1e6
    assert_equal("Large number prints 1e+06", "1e+06", UnityOutputBuffer);
}

// Test 7: Negative large number prints in scientific notation with minus sign, e.g., -1e6 -> "-1e+06".
static void test_negative_large_number_prints_negative_scientific()
{
    reset_output_buffer();
    UnityPrintFloat(-1000000.0); // -1e6
    assert_equal("Negative large number prints -1e+06", "-1e+06", UnityOutputBuffer);
}

// Test 8: Small positive number that should print as a fixed-point integer part when feasible, e.g., 1.0 -> "1".
static void test_one_prints_one_without_decimal()
{
    reset_output_buffer();
    UnityPrintFloat(1.0);
    assert_equal("One prints 1", "1", UnityOutputBuffer);
}

// Entry point to run all tests.
int main()
{
    // Run all tests in a sequence; the test harness prints PASS/FAIL per test.
    // Each test includes comments describing the scenario and expected behavior.

    // Test 1: Negative zero
    test_negative_zero_prints_minus_zero();

    // Test 2: Zero
    test_zero_prints_zero();

    // Test 3: NaN
    test_nan_prints_nan();

    // Test 4: +Infinity
    test_infinity_prints_inf();

    // Test 4b: -Infinity
    test_negative_infinity_prints_minus_inf();

    // Test 5: Positive integer
    test_positive_integer_prints_without_decimal();

    // Test 6: Large number in scientific notation
    test_large_number_prints_scientific_notation();

    // Test 7: Negative large number in scientific notation
    test_negative_large_number_prints_negative_scientific();

    // Test 8: Small integer-like value
    test_one_prints_one_without_decimal();

    return 0;
}