// Code: Lightweight C++11 unit test for the focal behavior
// Focus: testEqualInt8sWhenThereAreDifferencesOutside8Bits
// This test suite is designed to verify that two 16-bit values compare equal
// when cast to int8_t (i.e., ignoring differences outside the least-significant 8 bits).

#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Candidate Keywords (Step 1):
// - INT8 / int8_t
// - 0x321, 0x421, 0xFF21, 0x0021 (values used to demonstrate differences outside 8 bits)
// - Casting to 8-bit to ignore high bits
// - High-bit differences should be ignored for equality checks

// Global test harness state (non-terminating assertions)
static bool g_testFailed = false;
static std::string g_failureMessage;
static std::string g_currentTestName;

// Helper to signal a test failure with a message
void assertionFailure(const std::string& msg) {
    g_testFailed = true;
    g_failureMessage = msg;
}

// Core assertion used by the focal test: compare two values as int8_t
// and ignore any differences in the upper bits.
void assert_equal_int8_ignore_ext_bits(uint16_t a, uint16_t b, const std::string& detail) {
    // Compare lower 8 bits by casting to int8_t
    int8_t a8 = static_cast<int8_t>(a);
    int8_t b8 = static_cast<int8_t>(b);
    if (a8 != b8) {
        // Build a descriptive message including the detail and actual values
        std::ostringstream oss;
        oss << "Assertion failed in " << g_currentTestName
            << ": expected int8 equality ignoring high bits. "
            << detail << " | actual (a8=" << static_cast<int>(a8)
            << ", b8=" << static_cast<int>(b8) << ")";
        assertionFailure(oss.str());
    }
}

// Test runner: executes a test function, captures pass/fail, prints result
static int s_testsRun = 0;
static int s_testsFailed = 0;

void runTest(const std::string& name, const std::function<void()>& test) {
    g_currentTestName = name;
    g_testFailed = false;
    g_failureMessage.clear();
    test();
    ++s_testsRun;
    if (g_testFailed) {
        ++s_testsFailed;
        std::cout << "[FAIL] " << name << " - " << g_failureMessage << std::endl;
    } else {
        std::cout << "[PASS] " << name << std::endl;
    }
}

// Test 1: Pass case using 0x321 vs 0x421 (lower 8 bits are both 0x21)
void testEqualInt8sWhenThereAreDifferencesOutside8Bits_case1() {
    // The two values differ in high bits, but their int8_cast is equal (0x21)
    assert_equal_int8_ignore_ext_bits(0x321, 0x421, "0x321 vs 0x421 should be equal after masking to int8");
}

// Test 2: Pass case using 0xFF21 vs 0x0021 (lower 8 bits 0x21)
void testEqualInt8sWhenThereAreDifferencesOutside8Bits_case2() {
    // Again, differences outside the low 8 bits should be ignored
    assert_equal_int8_ignore_ext_bits(0xFF21, 0x0021, "0xFF21 vs 0x0021 should be equal after masking to int8");
}

// Test 3: Fail case where lower 8 bits differ (0x321 -> 0x320). This ensures
// that differences in the retained 8 bits are detected.
void testNotEqualInt8sWhenThereAreDifferencesOutside8Bits_lowerBitsDiffer() {
    // 0x321 has lower 8 bits 0x21, 0x320 has lower 8 bits 0x20 -> should fail
    assert_equal_int8_ignore_ext_bits(0x321, 0x320, "0x321 vs 0x320 should NOT be equal after masking to int8");
}

// Main: run all tests and print a summary
int main() {
    std::cout << "Running focal tests for: testEqualInt8sWhenThereAreDifferencesOutside8Bits\n";

    runTest("testEqualInt8sWhenThereAreDifferencesOutside8Bits_case1", testEqualInt8sWhenThereAreDifferencesOutside8Bits_case1);
    runTest("testEqualInt8sWhenThereAreDifferencesOutside8Bits_case2", testEqualInt8sWhenThereAreDifferencesOutside8Bits_case2);
    runTest("testNotEqualInt8sWhenThereAreDifferencesOutside8Bits_lowerBitsDiffer", testNotEqualInt8sWhenThereAreDifferencesOutside8Bits_lowerBitsDiffer);

    // Summary
    std::cout << "\nTest Summary: " << s_testsRun << " run, "
              << s_testsFailed << " failed, "
              << (s_testsRun - s_testsFailed) << " passed." << std::endl;

    // Return non-zero if any test failed
    return (s_testsFailed == 0) ? 0 : 1;
}