/*
  Lightweight C++11 test harness focused on the focal behavior of testEqualHEX8EachEqual.
  This reproduces the essence of Unity's TEST_ASSERT_EACH_EQUAL_HEX8 without depending on
  the Unity framework. It validates that an 8-bit hex value is equal across multiple
  bytes in an array, and also verifies failure cases where a mismatch occurs.

  Step 1: Program Understanding
  - Focal method: testEqualHEX8EachEqual (from testunity.c)
  - Core dependent component: TEST_ASSERT_EACH_EQUAL_HEX8 equivalent semantics
  - Core data type: unsigned char (HEX8 values)
  - Key arrays involved: p0, p1, p2, p3 with various lengths (1, 4, 3, 1)

  Step 2: Unit Test Generation
  - Build a small test harness (no GTest, no Unity dependency)
  - Implement assertEachEqualHEX8 to emulate the macro's success path
  - Create positive tests (all elements equal to the expected value)
  - Create negative tests (at least one element differs) and consider them as expected failures

  Step 3: Test Case Refinement
  - Provide clear per-test comments
  - Ensure tests are self-contained and do not rely on static/global Unity state
  - Use simple, deterministic outputs for pass/fail and a final summary

  Candidate Keywords:
  - TEST_ASSERT_EACH_EQUAL_HEX8 semantics, HEX8, unsigned char, arrays, length, p0/p1/p2/p3
*/

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <functional>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Helper to format a number as a small hex string (0xNN)
static std::string toHex(unsigned int v) {
    const char* hex = "0123456789ABCDEF";
    char buf[5] = {'0','x','0','0','\0'};
    buf[2] = hex[(v >> 4) & 0xF];
    buf[3] = hex[v & 0xF];
    return std::string(buf);
}

// Lightweight assertion that checks if every element in arr equals 'expected'.
// Returns true on success, false on first mismatch and prints a diagnostic message.
static bool assertEachEqualHEX8(unsigned char expected, const unsigned char* arr, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (arr[i] != expected) {
            std::cerr << "Mismatch at index " << i
                      << ": expected " << toHex((unsigned int)expected)
                      << ", got " << toHex((unsigned int)arr[i]) << " (actual: 0x"
                      << std::hex << (unsigned int)arr[i] << std::dec << ")\n";
            return false;
        }
    }
    return true;
}

// Positive test: all scenarios in the focal method should pass
// - p0: {254, 254, 254, 254}, check 1st element
// - p0: {254, 254, 254, 254}, check all 4
// - p1: {123, 123, 123, 123}, check all 4
// - p2: {8, 8, 8, 2}, check first 3 (all 8s)
// - p3: {1, 23, 25, 26}, check 1st element (1)
static bool testEqualHEX8EachEqual_Positive(void) {
    unsigned char p0[] = {254u, 254u, 254u, 254u};
    unsigned char p1[] = {123, 123, 123, 123};
    unsigned char p2[] = {8, 8, 8, 2};
    unsigned char p3[] = {1, 23, 25, 26};

    bool r1 = assertEachEqualHEX8(254u, p0, 1); // first element matches
    bool r2 = assertEachEqualHEX8(254u, p0, 4); // all 4 elements match
    bool r3 = assertEachEqualHEX8(123u, p1, 4); // all 4 elements match
    bool r4 = assertEachEqualHEX8(8u, p2, 3);   // first 3 elements match (8)
    bool r5 = assertEachEqualHEX8(1u, p3, 1);    // first element matches

    return r1 && r2 && r3 && r4 && r5;
}

// Negative test 1: a mismatch occurs where an element differs from expected (253 in this case)
static bool testNotEqualHEX8EachEqual1(void) {
    unsigned char p0[] = {253u, 253u, 254u, 253u};
    // Expect failure (mismatch at index 2)
    bool ok = assertEachEqualHEX8(253u, p0, 4);
    return !ok; // test passes if assertion fails (as expected)
}

// Negative test 2: mismatch at the last element
static bool testNotEqualHEX8EachEqual2(void) {
    unsigned char p0[] = {254u, 254u, 254u, 253u};
    bool ok = assertEachEqualHEX8(254u, p0, 4);
    return !ok;
}

// Negative test 3: full array mismatch when expecting 8 but array has 1 at start
static bool testNotEqualHEX8EachEqual3(void) {
    unsigned char p0[] = {1u, 8u, 8u, 8u};
    bool ok = assertEachEqualHEX8(8u, p0, 4);
    return !ok;
}

// Simple test harness
int main() {
    int testsRun = 0;
    int testsPassed = 0;

    auto run = [&](const char* name, const std::function<bool(void)>& fn) {
        ++testsRun;
        bool result = fn();
        if (result) {
            std::cout << "[PASS] " << name << "\n";
            ++testsPassed;
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Run tests
    run("testEqualHEX8EachEqual_Positive", testEqualHEX8EachEqual_Positive);
    run("testNotEqualHEX8EachEqual1", testNotEqualHEX8EachEqual1);
    run("testNotEqualHEX8EachEqual2", testNotEqualHEX8EachEqual2);
    run("testNotEqualHEX8EachEqual3", testNotEqualHEX8EachEqual3);

    // Summary
    std::cout << "Total tests run: " << testsRun
              << " | Passed: " << testsPassed
              << " | Failed: " << (testsRun - testsPassed) << "\n";

    return (testsRun == testsPassed) ? 0 : 1;
}