// This file provides a lightweight, self-contained C++11 test harness
// for the focal behavior represented by the Unity-based testNotEqualHEXArrays2
// function found in testunity.c. The goal is to exercise the core comparison
// logic (HEX32 array equality) in a standalone environment without requiring
// the entire Unity test framework. This helps verify the intent of the original
// test and improve coverage around the not-equal condition for HEX32 arrays.
//
// Key concepts mirrored from the focal test:
// - Comparison of two HEX32 arrays (4 elements in the example)
// - Not-equal scenario should be detected when any corresponding elements differ
// - Coverage aims to validate "not equal" detection and a contrasting "equal" case

#include <unity.h>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Candidate Keywords derived from the focal method:
// - UNITY_UINT32 / UINT32 array data type
// - Test arrays: p0, p1
// - HEX32 array equality check
// - Not-equal condition detection

// Lightweight helper: determine if two HEX32 arrays are not equal (element-wise)
static bool HEX32_Arrays_Not_Equal(const uint32_t* a, const uint32_t* b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return true; // Not equal at this position
        }
    }
    return false; // All elements equal
}

// Test 1: Verifies NOT_EQUAL behavior for two HEX32 arrays that differ in the first element
static void test_NotEqualHEXArrays2_Case1(void) {
    // Source arrays from the original focal test (p0 and p1)
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {2, 8, 987, 65132u};

    // Expected outcome: not equal
    bool notEqual = HEX32_Arrays_Not_Equal(p0, p1, 4);

    // Explanatory comment:
    // - We expect notEqual to be true because p0[0] != p1[0].
    // - If notEqual is false, the test would fail.
    if (!notEqual) {
        std::cerr << "test_NotEqualHEXArrays2_Case1: FAIL - arrays are unexpectedly equal\n";
        std::abort();
    } else {
        std::cout << "test_NotEqualHEXArrays2_Case1: PASS\n";
    }
}

// Test 2: Verifies NOT_EQUAL behavior for two HEX32 arrays that differ in a non-first element
// This confirms detection of not-equality beyond the first element (coverage breadth)
static void test_NotEqualHEXArrays2_Case2(void) {
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {1, 8, 987, 65131u}; // last element differs

    bool notEqual = HEX32_Arrays_Not_Equal(p0, p1, 4);

    if (!notEqual) {
        std::cerr << "test_NotEqualHEXArrays2_Case2: FAIL - arrays are unexpectedly equal\n";
        std::abort();
    } else {
        std::cout << "test_NotEqualHEXArrays2_Case2: PASS\n";
    }
}

// Test 3: Verifies NOT_EQUAL behavior when arrays are actually equal (sanity check)
static void test_NotEqualHEXArrays2_Case3_EqualArrays(void) {
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {1, 8, 987, 65132u};

    bool notEqual = HEX32_Arrays_Not_Equal(p0, p1, 4);

    if (notEqual) {
        std::cerr << "test_NotEqualHEXArrays2_Case3_EqualArrays: FAIL - arrays are not equal but should be\n";
        std::abort();
    } else {
        std::cout << "test_NotEqualHEXArrays2_Case3_EqualArrays: PASS\n";
    }
}

// Simple test dispatcher to run all tests and report summary
int main(void) {
    std::cout << "Starting lightweight HEX32-Array NOT_EQUAL test suite (C++11)\n";

    // Run tests
    test_NotEqualHEXArrays2_Case1();
    test_NotEqualHEXArrays2_Case2();
    test_NotEqualHEXArrays2_Case3_EqualArrays();

    std::cout << "HEX32 NOT_EQUAL test suite completed.\n";

    return 0;
}