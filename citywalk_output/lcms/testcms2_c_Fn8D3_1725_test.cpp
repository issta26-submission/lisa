// Test suite for Fn8D3 function found in testcms2.c
// This C++11 test harness avoids GoogleTest and uses a lightweight, non-terminating assertion approach.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Forward declaration of the focal function Fn8D3 with C linkage.
// We assume cmsUInt16Number maps to 16-bit unsigned and cmsUInt32Number to 32-bit unsigned.
// If your build environment uses different typedefs, this signature should still be valid
// due to C linkage and 16/32-bit widths remaining consistent across platforms.
extern "C" unsigned short Fn8D3(unsigned short a1, unsigned short a2, unsigned short a3, unsigned short a4,
                             unsigned short a5, unsigned short a6, unsigned short a7, unsigned short a8,
                             unsigned int m);

// Lightweight assertion bookkeeping
static int g_failures = 0;

// Non-terminating assertion helper: reports mismatch but continues execution
static void ExpectEq(const char* testName, uint16_t expected, uint16_t actual) {
    if (expected != actual) {
        std::cerr << "[FAIL] " << testName
                  << " - expected: " << static_cast<uint32_t>(expected)
                  << ", actual: " << static_cast<uint32_t>(actual) << "\n";
        ++g_failures;
    } else {
        std::cout << "[PASS] " << testName << "\n";
    }
}

// Helper to compute the exact expected value following the focal function logic.
// Mirrors the exact arithmetic, including 32-bit unsigned wraparound for the denominator.
static uint16_t ComputeExpected(uint16_t a1, uint16_t a2, uint16_t a3, uint16_t a4,
                                uint16_t a5, uint16_t a6, uint16_t a7, uint16_t a8,
                                uint32_t m)
{
    // Numerator uses 32-bit arithmetic to avoid intermediate overflow in C.
    uint32_t numerator = 3u * static_cast<uint32_t>(a1)
                       + 2u * static_cast<uint32_t>(a2)
                       + 3u * static_cast<uint32_t>(a3)
                       + static_cast<uint32_t>(a4)
                       + static_cast<uint32_t>(a5)
                       + static_cast<uint32_t>(a6)
                       + static_cast<uint32_t>(a7)
                       + static_cast<uint32_t>(a8);

    // Denominator with 32-bit unsigned wraparound behavior (m + 5)
    uint32_t denom = m + 5u; // wraps modulo 2^32 automatically

    uint32_t quotient = numerator / denom; // unsigned division

    return static_cast<uint16_t>(quotient); // final cast to 16-bit as in Fn8D3
}

/*
Step 2 - Test Suite for Fn8D3

Tests cover:
- Basic arithmetic correctness with small operands.
- Denominator wraparound via max unsigned m (0xFFFFFFFF).
- Various m values to exercise division results and final 16-bit cast behavior.
- Overflow handling of the 16-bit return type via large numerators.

Note: Fn8D3 has no branching; thus, branch coverage is not applicable here.
The tests focus on functional correctness and edge-case behaviors of the arithmetic.
*/

// Test 1: Basic arithmetic correctness with small operands
// a1..a8 = 1,2,3,4,5,6,7,8, m = 5
static void Test_Fn8D3_Basic(void) {
    // Domain: small, non-edge values
    uint16_t a1 = 1, a2 = 2, a3 = 3, a4 = 4;
    uint16_t a5 = 5, a6 = 6, a7 = 7, a8 = 8;
    uint32_t m  = 5;

    uint16_t expected = ComputeExpected(a1, a2, a3, a4, a5, a6, a7, a8, m);
    uint16_t actual   = Fn8D3(a1, a2, a3, a4, a5, a6, a7, a8, m);

    ExpectEq("Fn8D3_Basic", expected, actual);
}

// Test 2: Denominator = m + 5 with m = 0
// a1..a8 = 1..8
static void Test_Fn8D3_MZero(void) {
    uint16_t a1 = 1, a2 = 2, a3 = 3, a4 = 4;
    uint16_t a5 = 5, a6 = 6, a7 = 7, a8 = 8;
    uint32_t m  = 0;

    uint16_t expected = ComputeExpected(a1, a2, a3, a4, a5, a6, a7, a8, m);
    uint16_t actual   = Fn8D3(a1, a2, a3, a4, a5, a6, a7, a8, m);

    ExpectEq("Fn8D3_MZero", expected, actual);
}

// Test 3: Overflow of 16-bit return due to large numerator with small denominator
// a1..a8 = 65535, m = 0
static void Test_Fn8D3_MaxNumerator(void) {
    uint16_t a1 = 65535, a2 = 65535, a3 = 65535, a4 = 65535;
    uint16_t a5 = 65535, a6 = 65535, a7 = 65535, a8 = 65535;
    uint32_t m  = 0;

    uint16_t expected = ComputeExpected(a1, a2, a3, a4, a5, a6, a7, a8, m);
    uint16_t actual   = Fn8D3(a1, a2, a3, a4, a5, a6, a7, a8, m);

    ExpectEq("Fn8D3_MaxNumerator", expected, actual);
}

// Test 4: Typical large denominator values (m = 1000)
static void Test_Fn8D3_LargeDenom(void) {
    uint16_t a1 = 1, a2 = 2, a3 = 3, a4 = 4;
    uint16_t a5 = 5, a6 = 6, a7 = 7, a8 = 8;
    uint32_t m  = 1000;

    uint16_t expected = ComputeExpected(a1, a2, a3, a4, a5, a6, a7, a8, m);
    uint16_t actual   = Fn8D3(a1, a2, a3, a4, a5, a6, a7, a8, m);

    ExpectEq("Fn8D3_LargeDenom", expected, actual);
}

// Test 5: Denominator wraparound using maximum 32-bit unsigned m (0xFFFFFFFF)
// This tests C-style wraparound behavior of m + 5
static void Test_Fn8D3_DenomWrapAround(void) {
    uint16_t a1 = 1, a2 = 2, a3 = 3, a4 = 4;
    uint16_t a5 = 5, a6 = 6, a7 = 7, a8 = 8;
    uint32_t m  = 0xFFFFFFFFu;

    uint16_t expected = ComputeExpected(a1, a2, a3, a4, a5, a6, a7, a8, m);
    uint16_t actual   = Fn8D3(a1, a2, a3, a4, a5, a6, a7, a8, m);

    ExpectEq("Fn8D3_DenomWrapAround", expected, actual);
}

int main() {
    // Run all tests
    Test_Fn8D3_Basic();
    Test_Fn8D3_MZero();
    Test_Fn8D3_MaxNumerator();
    Test_Fn8D3_LargeDenom();
    Test_Fn8D3_DenomWrapAround();

    // Summary
    if (g_failures == 0) {
        std::cout << "[SUMMARY] All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "[SUMMARY] " << g_failures << " test(s) FAILED.\n";
        return 1;
    }
}