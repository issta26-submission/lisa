// Test suite for ApplyTransforms in icctrans.c
// This suite uses MATLAB MEX-like API (mxArray, mxCreateDoubleMatrix, etc.)
// and calls the focal function: ApplyTransforms(const mxArray *In, mxArray *Out)
// NOTE: This test relies on the surrounding icctrans.c / LittleCMS
// environment being available at link time. It avoids GTest and uses
// a lightweight, non-terminating assertion approach to maximize coverage.

#include <cstring>
#include <cstdio>
#include <limits>
#include <mex.h>
#include <vector>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <lcms2.h>
#include <cmath>
#include <string.h>


// MATLAB MEX API headers (assumed available in the build environment)

// The focal function to test. It has C linkage in the original code.
extern "C" void ApplyTransforms(const mxArray *In, mxArray *Out);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total; \
        if (!(cond)) { \
            ++g_failed; \
            std::cerr << "[FAIL] " << msg << std::endl; \
        } \
    } while (0)

#define EXPECT_EQ(actual, expected, msg) \
    do { \
        ++g_total; \
        if (!((actual) == (expected))) { \
            ++g_failed; \
            std::cerr << "[FAIL] " << msg << " | expected: " << (expected) \
                      << " actual: " << (actual) << std::endl; \
        } \
    } while (0)

// Helper to create a double mxArray and fill with given values (row-major for ease)
static mxArray* MakeDoubleArrayFromVector(const std::vector<double>& values, mwSize rows, mwSize cols) {
    // Create a real double matrix with the requested dimensions
    mxArray* arr = mxCreateDoubleMatrix(rows, cols, mxREAL);
    if (!arr) return nullptr;
    double* data = mxGetPr(arr);
    // Copy values in row-major order to match our vector assumption
    size_t n = values.size();
    for (size_t i = 0; i < n; ++i) {
        data[i] = values[i];
    }
    return arr;
}

// Test 1: Basic single-pixel transformation should not crash and should produce a valid Out buffer.
// We cannot rely on exact transform results without knowing the current transform setup,
// but we can verify that ApplyTransforms executes and writes something to Output
// when there is a positive nPixels. We also ensure no crash occurs.
static void Test_ApplyTransforms_BasicChange() {
    // Prepare input: 1x4 matrix with known values
    std::vector<double> inVals = { 1.0, 2.0, 3.0, 4.0 };
    mxArray* In  = MakeDoubleArrayFromVector(inVals, 1, 4);
    if (!In) {
        std::cerr << "Failed to allocate In matrix in Test_ApplyTransforms_BasicChange." << std::endl;
        ++g_failed; return;
    }

    // Prepare output: 1x4 matrix, initialized to a sentinel value
    std::vector<double> outInit(4, std::numeric_limits<double>::quiet_NaN());
    mxArray* Out = MakeDoubleArrayFromVector(outInit, 1, 4);
    if (!Out) {
        std::cerr << "Failed to allocate Out matrix in Test_ApplyTransforms_BasicChange." << std::endl;
        mxDestroyArray(In);
        ++g_failed; return;
    }

    // The focal function under test
    ApplyTransforms(In, Out);

    // Verify that At least one Output element has been written (not NaN) or changed from initial NaN.
    double* outPtr = mxGetPr(Out);
    bool anyWritten = false;
    for (size_t i = 0; i < 4; ++i) {
        if (!std::isnan(outPtr[i])) { anyWritten = true; break; }
    }

    EXPECT_TRUE(anyWritten, "ApplyTransforms did not write any values to Output for 1x4 input.");

    // Cleanup
    mxDestroyArray(In);
    mxDestroyArray(Out);
}

// Test 2: Multiple pixels with varied input. Ensure function executes and Output buffer is touched.
// This increases coverage on memory access and loop behavior inside ApplyTransforms.
static void Test_ApplyTransforms_MultiplePixels() {
    // Prepare input: 2x2 matrix (4 pixels)
    std::vector<double> inVals = { 10.0, -5.0, 7.5, 0.0 };
    mxArray* In  = MakeDoubleArrayFromVector(inVals, 2, 2);
    if (!In) { std::cerr << "Failed to allocate In matrix in Test_ApplyTransforms_MultiplePixels." << std::endl; ++g_failed; return; }

    // Prepare output: 2x2 matrix initialized with a fixed value to detect changes
    std::vector<double> outInit(4, -7.0);
    mxArray* Out = MakeDoubleArrayFromVector(outInit, 2, 2);
    if (!Out) { std::cerr << "Failed to allocate Out matrix in Test_ApplyTransforms_MultiplePixels." << std::endl; mxDestroyArray(In); ++g_failed; return; }

    // Call the function
    ApplyTransforms(In, Out);

    // Verify that Output has been touched (not equal to the initial value on all elements)
    double* outPtr = mxGetPr(Out);
    bool touched = false;
    for (size_t i = 0; i < 4; ++i) {
        if (outPtr[i] != -7.0) { touched = true; break; }
    }
    EXPECT_TRUE(touched, "Output remained unchanged for 2x2 input; ApplyTransforms may not have operated on data.");

    // Cleanup
    mxDestroyArray(In);
    mxDestroyArray(Out);
}

// Test 3: Re-run with different input to increase coverage of repeat usage.
// Ensures that subsequent calls do not crash and that Out buffer can be overwritten.
static void Test_ApplyTransforms_RepeatUsage() {
    // First input: 1x3
    std::vector<double> inVals1 = { 0.0, 1.0, 2.0 };
    mxArray* In1 = MakeDoubleArrayFromVector(inVals1, 1, 3);
    if (!In1) { std::cerr << "Failed to allocate In1 in Test_ApplyTransforms_RepeatUsage." << std::endl; ++g_failed; return; }

    std::vector<double> outInit1(3, 3.14);
    mxArray* Out1 = MakeDoubleArrayFromVector(outInit1, 1, 3);
    if (!Out1) { std::cerr << "Failed to allocate Out1 in Test_ApplyTransforms_RepeatUsage." << std::endl; mxDestroyArray(In1); ++g_failed; return; }

    ApplyTransforms(In1, Out1);
    double* o1 = mxGetPr(Out1);
    bool anyWritten1 = false;
    for (size_t i = 0; i < 3; ++i) if (o1[i] != 3.14) { anyWritten1 = true; break; }

    // Second input: 1x2
    std::vector<double> inVals2 = { -1.0, 4.5 };
    mxArray* In2 = MakeDoubleArrayFromVector(inVals2, 1, 2);
    if (!In2) { std::cerr << "Failed to allocate In2 in Test_ApplyTransforms_RepeatUsage." << std::endl; mxDestroyArray(In1); mxDestroyArray(Out1); ++g_failed; return; }

    std::vector<double> outInit2(2, -2.2);
    mxArray* Out2 = MakeDoubleArrayFromVector(outInit2, 1, 2);
    if (!Out2) { std::cerr << "Failed to allocate Out2 in Test_ApplyTransforms_RepeatUsage." << std::endl; mxDestroyArray(In1); mxDestroyArray(Out1); mxDestroyArray(In2); ++g_failed; return; }

    ApplyTransforms(In2, Out2);
    double* o2 = mxGetPr(Out2);
    bool anyWritten2 = false;
    for (size_t i = 0; i < 2; ++i) if (o2[i] != -2.2) { anyWritten2 = true; break; }

    EXPECT_TRUE(anyWritten1 || anyWritten2, "Repeat usage did not write to Output buffers in any of the two calls.");

    // Cleanup
    mxDestroyArray(In1); mxDestroyArray(Out1);
    mxDestroyArray(In2); mxDestroyArray(Out2);
}

// Main entry: run all tests and report results
int main() {
    std::cout << "Starting unit tests for ApplyTransforms (icctrans.c) with non-GTest harness." << std::endl;

    // Run tests
    Test_ApplyTransforms_BasicChange();
    Test_ApplyTransforms_MultiplePixels();
    Test_ApplyTransforms_RepeatUsage();

    // Summary
    std::cout << "Test Summary: " << g_total << " tests run, " << g_failed << " failures." << std::endl;
    if (g_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
}