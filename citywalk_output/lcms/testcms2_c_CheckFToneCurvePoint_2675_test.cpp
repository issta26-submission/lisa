// Test suite for the focal method: CheckFToneCurvePoint
// This test is written for a C++11 environment without using GTest.
// It relies on the test harness and LittleCMS-style APIs provided in testcms2.h,
// including CreateSegmentedCurve() and cmsEvalToneCurve16().
// The tests exercise true/false branches of the predicate:
//    abs(Value - cmsEvalToneCurve16(c, Point)) < 2

#include <cstdint>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <cmath>


// Step 1: Candidate Keywords (for understanding and coverage planning)
// - cmsInt32Number: 32-bit signed integer type used by CMS
// - cmsUInt16Number: 16-bit unsigned input for tone curves
// - cmsToneCurve: structure representing a tone curve
// - cmsEvalToneCurve16: evaluates the 16-bit tone curve at a given input
// - CreateSegmentedCurve(): helper to build a representative test tone curve
// - CheckFToneCurvePoint(c, Point, Value): focal function under test
// - cmsFreeToneCurve(): free allocated tone curve
// These symbols are provided by the test harness (testcms2.h) and the LittleCMS-like API.


static int g_total = 0;
static int g_passed = 0;

// Simple non-terminating assertion helper
#define TEST_ASSERT(cond, msg) do { \
    ++g_total; \
    if (cond) { ++g_passed; } else { \
        std::cerr << "TEST FAIL: " << (msg) << " (test #" << g_total << ")" << std::endl; \
    } \
} while (0)


// Test 1: True branch with exact equality
// Use Point = 0 and Value = Result to ensure perfect match (difference 0 < 2)
bool Test_CheckFToneCurvePoint_True_EqualsAtZero() {
    cmsToneCurve* c = CreateSegmentedCurve();
    if (c == nullptr) {
        std::cerr << "Failed to create tone curve for Test_CheckFToneCurvePoint_True_EqualsAtZero" << std::endl;
        return false;
    }

    cmsUInt16Number point = 0;
    cmsInt32Number result = cmsEvalToneCurve16(c, point);
    bool test_ok = CheckFToneCurvePoint(c, point, result) != 0;

    cmsFreeToneCurve(c);

    TEST_ASSERT(test_ok, "CheckFToneCurvePoint should return true when Value equals real Result at Point=0");
    return test_ok;
}


// Test 2: True branch with difference of 1 (Value = Result +/- 1)
// Ensure the function treats small difference (<2) as true
bool Test_CheckFToneCurvePoint_True_DifferenceOne() {
    cmsToneCurve* c = CreateSegmentedCurve();
    if (c == nullptr) {
        std::cerr << "Failed to create tone curve for Test_CheckFToneCurvePoint_True_DifferenceOne" << std::endl;
        return false;
    }

    // Pick a mid-range point to avoid boundary issues
    cmsUInt16Number point = 12345;
    cmsInt32Number realRes = cmsEvalToneCurve16(c, point);

    // Choose a Value that differs by exactly 1 (if possible)
    cmsInt32Number desired = realRes;
    cmsUInt16Number testValue;
    if (realRes < 0xFFFF) {
        testValue = static_cast<cmsUInt16Number>(realRes + 1);
    } else {
        testValue = static_cast<cmsUInt16Number>(realRes - 1);
    }

    bool test_ok = CheckFToneCurvePoint(c, point, static_cast<cmsInt32Number>(testValue)) != 0;

    cmsFreeToneCurve(c);

    TEST_ASSERT(test_ok, "CheckFToneCurvePoint should return true when Value differs by 1 from real Result");
    return test_ok;
}


// Test 3: False branch with difference >= 2
// Use a point and a Value that are at least 3 apart from the real Result
bool Test_CheckFToneCurvePoint_False_DifferenceThree() {
    cmsToneCurve* c = CreateSegmentedCurve();
    if (c == nullptr) {
        std::cerr << "Failed to create tone curve for Test_CheckFToneCurvePoint_False_DifferenceThree" << std::endl;
        return false;
    }

    cmsUInt16Number point = 54321;
    cmsInt32Number realRes = cmsEvalToneCurve16(c, point);

    // Choose a Value that differs by at least 3
    cmsInt32Number testValue;
    if (realRes > 3) testValue = realRes - 3;
    else testValue = realRes + 3;

    bool test_ok = CheckFToneCurvePoint(c, point, testValue) == 0;

    cmsFreeToneCurve(c);

    TEST_ASSERT(test_ok, "CheckFToneCurvePoint should return false when Value differs by >= 3");
    return test_ok;
}


// Test 4: True branch at high boundary with equal Value
// Use a boundary point 65535 and ensure equality yields true
bool Test_CheckFToneCurvePoint_True_EqualsAtMax() {
    cmsToneCurve* c = CreateSegmentedCurve();
    if (c == nullptr) {
        std::cerr << "Failed to create tone curve for Test_CheckFToneCurvePoint_True_EqualsAtMax" << std::endl;
        return false;
    }

    cmsUInt16Number point = 65535;
    cmsInt32Number realRes = cmsEvalToneCurve16(c, point);

    bool test_ok = CheckFToneCurvePoint(c, point, realRes) != 0;

    cmsFreeToneCurve(c);

    TEST_ASSERT(test_ok, "CheckFToneCurvePoint should return true when Value equals real Result at Point=65535");
    return test_ok;
}


// Test 5: False branch with negative Value (out-of-range for unsigned interpretation)
bool Test_CheckFToneCurvePoint_False_NegativeValue() {
    cmsToneCurve* c = CreateSegmentedCurve();
    if (c == nullptr) {
        std::cerr << "Failed to create tone curve for Test_CheckFToneCurvePoint_False_NegativeValue" << std::endl;
        return false;
    }

    cmsUInt16Number point = 42;
    cmsInt32Number realRes = cmsEvalToneCurve16(c, point);

    // Use a clearly negative Value to force large difference
    cmsInt32Number testValue = -5;

    bool test_ok = CheckFToneCurvePoint(c, point, testValue) == 0;

    cmsFreeToneCurve(c);

    TEST_ASSERT(test_ok, "CheckFToneCurvePoint should return false when Value is negative (out-of-range)");
    return test_ok;
}


// Main: run all tests and report summary
int main() {
    bool t1 = Test_CheckFToneCurvePoint_True_EqualsAtZero();
    bool t2 = Test_CheckFToneCurvePoint_True_DifferenceOne();
    bool t3 = Test_CheckFToneCurvePoint_False_DifferenceThree();
    bool t4 = Test_CheckFToneCurvePoint_True_EqualsAtMax();
    bool t5 = Test_CheckFToneCurvePoint_False_NegativeValue();

    // Aggregate results
    // Note: Tests themselves perform per-test assertions and printing.
    // Here we print a concise summary.
    std::cout << "CheckFToneCurvePoint tests completed. "
              << g_passed << " passed, "
              << g_total << " total." << std::endl;

    // Exit code 0 if all tests passed; otherwise non-zero.
    if (g_passed == g_total) return 0;
    return 2;
}