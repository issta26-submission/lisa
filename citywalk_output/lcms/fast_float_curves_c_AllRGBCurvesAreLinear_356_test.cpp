// Test suite for CMS AllRGBCurvesAreLinear function
// Target: fast_float_curves.c - AllRGBCurvesAreLinear(CurvesFloatData* data)
// This test is written for a C++11 environment without GoogleTest.
// It relies on the project's public header to expose CurvesFloatData, MAX_NODES_IN_CURVE,
// LINEAR_CURVES_EPSILON and the prototype for AllRGBCurvesAreLinear.

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <fast_float_curves.h>
#include <cmath>


// Include the project's header that declares the types and function under test.
// Adjust the path if your environment uses a different include layout.

// Ensure C linkage for the function we are testing, in case the header is not already extern "C".
extern "C" cmsBool AllRGBCurvesAreLinear(CurvesFloatData* data);

// Simple non-terminating test reporting helpers
static int g_totalTests = 0;
static int g_failedTests = 0;

static void ReportResult(const char* testName, bool passed) {
    ++g_totalTests;
    if (passed) {
        std::cout << "[OK]    " << testName << "\n";
    } else {
        ++g_failedTests;
        std::cerr << "[FAILED] " << testName << "\n";
    }
}

// Helper: fill data with perfectly linear RGB curves: R = G = B = t for t in [0,1]
static void FillLinearCurves(CurvesFloatData* data) {
    // We rely on MAX_NODES_IN_CURVE and the linear spacing
    const int N = MAX_NODES_IN_CURVE;
    const cmsFloat32Number step = 1.0f / static_cast<cmsFloat32Number>(N - 1);
    for (int i = 0; i < N; ++i) {
        cmsFloat32Number v = static_cast<cmsFloat32Number>(i) * step;
        data->CurveR[i] = v;
        data->CurveG[i] = v;
        data->CurveB[i] = v;
    }
}

// Helper: create a fresh CurvesFloatData with zero-initialized memory
static CurvesFloatData CreateEmptyCurvesData() {
    CurvesFloatData data;
    // Zero initialize to avoid any undefined values
    std::memset(&data, 0, sizeof(CurvesFloatData));
    return data;
}

// Test 1: All curves are perfectly linear => AllRGBCurvesAreLinear should return TRUE
static bool Test_AllLinearReturnsTrue() {
    // Arrange
    CurvesFloatData data = CreateEmptyCurvesData();
    FillLinearCurves(&data);

    // Act
    cmsBool result = AllRGBCurvesAreLinear(&data);

    // Assert
    bool passed = (result == TRUE);
    if (!passed) {
        std::cerr << "Expected TRUE for perfectly linear curves, got FALSE.\n";
    }
    ReportResult("Test_AllLinearReturnsTrue", passed);
    return passed;
}

// Test 2: A single channel deviation exceeding epsilon should cause FALSE
static bool Test_DeviationInOneChannelReturnsFalse_AboveEpsilon() {
    // Arrange
    CurvesFloatData data = CreateEmptyCurvesData();
    FillLinearCurves(&data);

    // Choose a mid index to introduce deviation
    const int idx = MAX_NODES_IN_CURVE / 2;
    // Compute the expected linear value at idx
    cmsFloat32Number step = 1.0f / static_cast<cmsFloat32Number>(MAX_NODES_IN_CURVE - 1);
    cmsFloat32Number expected = static_cast<cmsFloat32Number>(idx) * step;
    // Introduce a deviation larger than epsilon for CurveR
    data.CurveR[idx] = expected + LINEAR_CURVES_EPSILON * 1.1f;

    // Act
    cmsBool result = AllRGBCurvesAreLinear(&data);

    // Assert
    bool passed = (result == FALSE);
    if (!passed) {
        std::cerr << "Expected FALSE when CurveR deviates beyond epsilon at idx " << idx << ".\n";
    }
    ReportResult("Test_DeviationInOneChannelReturnsFalse_AboveEpsilon", passed);
    return passed;
}

// Test 3: A deviation in CurveG below epsilon should still be considered linear
// (This ensures the epsilon threshold is respected per channel and per node)
static bool Test_DeviationInOneChannelAtOrBelowEpsilonKeepsTrue() {
    // Arrange
    CurvesFloatData data = CreateEmptyCurvesData();
    FillLinearCurves(&data);

    // Choose a mid index to introduce deviation
    const int idx = MAX_NODES_IN_CURVE / 3;
    cmsFloat32Number step = 1.0f / static_cast<cmsFloat32Number>(MAX_NODES_IN_CURVE - 1);
    cmsFloat32Number expected = static_cast<cmsFloat32Number>(idx) * step;

    // Introduce a deviation equal to epsilon (not greater)
    data.CurveG[idx] = expected + LINEAR_CURVES_EPSILON;

    // Act
    cmsBool result = AllRGBCurvesAreLinear(&data);

    // Assert
    bool passed = (result == TRUE);
    if (!passed) {
        std::cerr << "Expected TRUE when CurveG deviation is exactly epsilon at idx " << idx << ".\n";
    }
    ReportResult("Test_DeviationInOneChannelAtOrBelowEpsilonKeepsTrue", passed);
    return passed;
}

// Test 4: Multiple channels deviating but only within epsilon should still be treated as linear
// to verify multi-channel parallel checks behave correctly with threshold
static bool Test_MultipleDeviationsAtOrBelowEpsilonStillTrue() {
    // Arrange
    CurvesFloatData data = CreateEmptyCurvesData();
    FillLinearCurves(&data);

    // Introduce deviations for multiple channels at two different indices
    const int idx1 = MAX_NODES_IN_CURVE / 4;
    const int idx2 = 3 * MAX_NODES_IN_CURVE / 4;
    cmsFloat32Number step = 1.0f / static_cast<cmsFloat32Number>(MAX_NODES_IN_CURVE - 1);

    cmsFloat32Number exp1 = static_cast<cmsFloat32Number>(idx1) * step;
    cmsFloat32Number exp2 = static_cast<cmsFloat32Number>(idx2) * step;

    data.CurveR[idx1] = exp1 + LINEAR_CURVES_EPSILON; // at threshold
    data.CurveG[idx2] = exp2 + LINEAR_CURVES_EPSILON; // at threshold

    // Act
    cmsBool result = AllRGBCurvesAreLinear(&data);

    // Assert
    bool passed = (result == TRUE);
    if (!passed) {
        std::cerr << "Expected TRUE when multiple channels deviate exactly at epsilon, indices " 
                  << idx1 << " and " << idx2 << ".\n";
    }
    ReportResult("Test_MultipleDeviationsAtOrBelowEpsilonStillTrue", passed);
    return passed;
}

// Main routine to run all tests
int main() {
    // Run tests
    bool t1 = Test_AllLinearReturnsTrue();
    bool t2 = Test_DeviationInOneChannelReturnsFalse_AboveEpsilon();
    bool t3 = Test_DeviationInOneChannelAtOrBelowEpsilonKeepsTrue();
    bool t4 = Test_MultipleDeviationsAtOrBelowEpsilonStillTrue();

    // Summary
    std::cout << "\nTest Summary: " << g_totalTests << " tests run, "
              << g_failedTests << " failures.\n";

    // Return non-zero if any test failed
    return g_failedTests > 0 ? 1 : 0;
}