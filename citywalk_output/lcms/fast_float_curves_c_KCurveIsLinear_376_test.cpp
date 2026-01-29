// Candidate Keywords extracted from the focal method and dependencies:
// KCurveIsLinear, CurvesFloatData, CurveR, MAX_NODES_IN_CURVE, LINEAR_CURVES_EPSILON, cmsBool, TRUE, FALSE

// This test suite targets the KCurveIsLinear function from fast_float_curves.c.
// It validates true and false branches of its internal predicate by constructing
// CurvesFloatData with various CurveR arrays and asserting the expected outcomes.
// The tests are written in C++11 without any GoogleTest framework.

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <fast_float_curves.h>
#include <cmath>


extern "C" {
}

// Helper to print simple test results (non-terminating assertion style)
struct TestResult {
    std::string name;
    bool passed;
};

// Test 1: True path - CurveR forms an exact linear ramp [0, 1] across all nodes
// Expects KCurveIsLinear to return TRUE.
static bool test_KCurveIsLinear_linearRamp_AllPointsAreLinear() {
    CurvesFloatData data;
    // Build the exact linear ramp: CurveR[j] == j / (MAX_NODES_IN_CURVE - 1)
    for (int j = 0; j < MAX_NODES_IN_CURVE; ++j) {
        cmsFloat32Number expected = (cmsFloat32Number)j / (cmsFloat32Number)(MAX_NODES_IN_CURVE - 1);
        data.CurveR[j] = expected;
    }

    cmsBool result = KCurveIsLinear(&data);
    // True only if every point matches within LINEAR_CURVES_EPSILON
    return result == TRUE;
}

// Test 2: False path - A single point deviates above LINEAR_CURVES_EPSILON
// Expects KCurveIsLinear to return FALSE.
static bool test_KCurveIsLinear_linearRamp_OnePointDeviatesAboveEpsilon() {
    CurvesFloatData data;
    // Start with a perfect linear ramp
    for (int j = 0; j < MAX_NODES_IN_CURVE; ++j) {
        cmsFloat32Number expected = (cmsFloat32Number)j / (cmsFloat32Number)(MAX_NODES_IN_CURVE - 1);
        data.CurveR[j] = expected;
    }
    // Introduce a deviation larger than epsilon at a mid-point
    int idx = MAX_NODES_IN_CURVE / 3;
    data.CurveR[idx] += LINEAR_CURVES_EPSILON * 2.0f;

    cmsBool result = KCurveIsLinear(&data);
    return result == FALSE;
}

// Test 3: Boundary condition - Deviation exactly equal to LINEAR_CURVES_EPSILON
// Expects KCurveIsLinear to return TRUE (since condition uses > epsilon).
static bool test_KCurveIsLinear_linearRamp_DeviationExactlyEpsilon() {
    CurvesFloatData data;
    // Start with a perfect linear ramp
    for (int j = 0; j < MAX_NODES_IN_CURVE; ++j) {
        cmsFloat32Number expected = (cmsFloat32Number)j / (cmsFloat32Number)(MAX_NODES_IN_CURVE - 1);
        data.CurveR[j] = expected;
    }
    // Deviation exactly equal to epsilon at some index
    int idx = MAX_NODES_IN_CURVE / 4;
    data.CurveR[idx] += LINEAR_CURVES_EPSILON; // exactly epsilon

    cmsBool result = KCurveIsLinear(&data);
    return result == TRUE;
}

// Test 4: Degenerate case - All values zero (not a linear ramp except at j=0)
// Expects KCurveIsLinear to return FALSE.
static bool test_KCurveIsLinear_linearRamp_AllZeros() {
    CurvesFloatData data;
    for (int j = 0; j < MAX_NODES_IN_CURVE; ++j) {
        data.CurveR[j] = 0.0f;
    }

    cmsBool result = KCurveIsLinear(&data);
    return result == FALSE;
}

// Simple driver to run all tests and print results
int main() {
    std::vector<TestResult> results;

    results.push_back({"KCurveIsLinear_linearRamp_AllPointsAreLinear", test_KCurveIsLinear_linearRamp_AllPointsAreLinear()});
    results.push_back({"KCurveIsLinear_linearRamp_OnePointDeviatesAboveEpsilon", test_KCurveIsLinear_linearRamp_OnePointDeviatesAboveEpsilon()});
    results.push_back({"KCurveIsLinear_linearRamp_DeviationExactlyEpsilon", test_KCurveIsLinear_linearRamp_DeviationExactlyEpsilon()});
    results.push_back({"KCurveIsLinear_linearRamp_AllZeros", test_KCurveIsLinear_linearRamp_AllZeros()});

    int passed = 0;
    for (const auto& r : results) {
        std::cout << (r.passed ? "[PASSED] " : "[FAILED] ") << r.name << std::endl;
        if (r.passed) ++passed;
    }

    std::cout << "Summary: " << passed << " / " << results.size() << " tests passed." << std::endl;
    // Return 0 on full success, non-zero otherwise to integrate with simple CI scripts
    return (passed == (int)results.size()) ? 0 : 1;
}