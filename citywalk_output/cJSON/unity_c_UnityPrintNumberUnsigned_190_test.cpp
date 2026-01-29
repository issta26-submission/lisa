// A lightweight C++11 test suite for UnityPrintNumberUnsigned from Unity's unity.h
// - Captures printed output via a custom UNITY_OUTPUT_CHAR macro
// - Uses a small, non-terminating assertion mechanism (does not abort on failure)
// - Runs a few representative test cases to achieve high coverage of the function

#include <stddef.h>
#include <string>
#include <iostream>
#include <unity.h>


// Buffer to capture characters printed by UnityPrintNumberUnsigned
static std::string UnityPrintedOutput;

// Forward declaration to satisfy the macro before including unity.h
extern "C" void capture_output_char(char c);

// Redirect Unity's output to our buffer by redefining the output macro
#define UNITY_OUTPUT_CHAR(c) capture_output_char((char)(c))

// Implementation of the capture function that appends characters to the buffer
extern "C" void capture_output_char(char c) {
    UnityPrintedOutput.push_back(static_cast<char>(c));
}

// Include Unity's header (contains UnityPrintNumberUnsigned and related declarations)

// Minimal test harness (non-terminating assertions)
static int total_tests = 0;
static int failed_tests = 0;

static void fail(const std::string& msg) {
    ++total_tests;
    ++failed_tests;
    std::cerr << "TEST FAILED: " << msg << std::endl;
}

static void pass(const std::string& msg) {
    ++total_tests;
    std::cout << "TEST PASSED: " << msg << std::endl;
}

// Generic assertion for strings
static void assert_equal_string(const std::string& expected,
                                const std::string& actual,
                                const std::string& test_desc) {
    if (expected == actual) {
        pass(test_desc);
    } else {
        fail(test_desc + " - expected: \"" + expected + "\", actual: \"" + actual + "\"");
    }
}

// Test 1: UnityPrintNumberUnsigned(0) should print "0" (edge case)
static void test_UnityPrintNumberUnsigned_Zero() {
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)0);
    assert_equal_string("0", UnityPrintedOutput, "UnityPrintNumberUnsigned(0) prints '0'");
}

// Test 2: UnityPrintNumberUnsigned with a single digit (e.g., 5) prints "5"
static void test_UnityPrintNumberUnsigned_SingleDigit() {
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)5);
    assert_equal_string("5", UnityPrintedOutput, "UnityPrintNumberUnsigned(5) prints '5'");
}

// Test 3: UnityPrintNumberUnsigned with multiple digits (e.g., 1234) prints "1234"
static void test_UnityPrintNumberUnsigned_MultiDigits() {
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)1234);
    assert_equal_string("1234", UnityPrintedOutput, "UnityPrintNumberUnsigned(1234) prints '1234'");
}

// Test 4: UnityPrintNumberUnsigned with a larger value (e.g., 98765) prints correctly
static void test_UnityPrintNumberUnsigned_Large() {
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)98765);
    assert_equal_string("98765", UnityPrintedOutput, "UnityPrintNumberUnsigned(98765) prints '98765'");
}

// Test 5: Verify that consecutive calls do not produce interleaved output
static void test_UnityPrintNumberUnsigned_ConsecutiveCalls() {
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)42);
    UnityPrintNumberUnsigned((UNITY_UINT)7);
    // Expect only the second call's output since we reset between calls; simulate isolation
    // Here we simulate isolation by resetting before each call and validating individually
    // First part
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)42);
    assert_equal_string("42", UnityPrintedOutput, "UnityPrintNumberUnsigned(42) prints '42' in isolation");
    // Second part
    UnityPrintedOutput.clear();
    UnityPrintNumberUnsigned((UNITY_UINT)7);
    assert_equal_string("7", UnityPrintedOutput, "UnityPrintNumberUnsigned(7) prints '7' in isolation");
}

// Run all tests
static void run_all_tests() {
    test_UnityPrintNumberUnsigned_Zero();
    test_UnityPrintNumberUnsigned_SingleDigit();
    test_UnityPrintNumberUnsigned_MultiDigits();
    test_UnityPrintNumberUnsigned_Large();
    test_UnityPrintNumberUnsigned_ConsecutiveCalls();
}

// Main entry point for the test binary
int main() {
    // Reset counters
    total_tests = 0;
    failed_tests = 0;

    // Execute tests
    run_all_tests();

    // Summary
    std::cout << "Total tests: " << total_tests
              << ", Passed: " << (total_tests - failed_tests)
              << ", Failed: " << failed_tests << std::endl;

    // Return non-zero if any test failed
    return (failed_tests == 0) ? 0 : 1;
}