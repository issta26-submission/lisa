// Test suite for TrilinearInterpFloat in cmsintrp.c
// Note: This test relies on the project-provided cmsFloat32Number, cmsInterpParams, and
// the function signature void TrilinearInterpFloat(const cmsFloat32Number Input[],
// cmsFloat32Number Output[], const cmsInterpParams* p).
// The tests are designed to be compiled with C++11 and without GoogleTest.
// Each test is self-contained and uses the public interface only.

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Include the library headers that define cmsFloat32Number, cmsInterpParams, and
// the TrilinearInterpFloat function. Adjust include paths if necessary for your build.
extern "C" {
void TrilinearInterpFloat(const cmsFloat32Number Input[],
                          cmsFloat32Number Output[],
                          const cmsInterpParams* p);
}

// Simple helper to compare floats with a tolerance
static bool almost_equal_float(float a, float b, float tol = 1e-5f) {
    return std::fabs(a - b) <= tol;
}

// Global test result accumulator
static int g_failures = 0;

// Utility to print test result
static void report_result(const std::string& test_name, bool passed, const std::string& details = "") {
    if (passed) {
        std::cout << "[PASS] " << test_name;
        if (!details.empty()) std::cout << " - " << details;
        std::cout << std::endl;
    } else {
        std::cout << "[FAIL] " << test_name;
        if (!details.empty()) std::cout << " - " << details;
        std::cout << std::endl;
        ++g_failures;
    }
}

// Test 1: Interior point interpolation with simple 2x2x2 LUT and single output channel
// Expected result (with LutTable = {0,1,2,3,4}, Domain all 1.0, opta all 1.0, nOutputs=1):
// Output should be 1.5 at Input = {0.5, 0.5, 0.5}
static void test_trilinear_float_interior_single_output() {
    // Domain knowledge reference:
    // 8 corner samples mapped by DENS(i,j,k) = LutTable[(i)+(j)+(k)+OutChan]
    // For (x0,y0,z0)=(0,0,0) and (x1,y1,z1)=(1,1,1) with fx=fy=fz=0.5, result is 1.5
    cmsFloat32Number Input[3] = {0.5f, 0.5f, 0.5f};
    cmsFloat32Number Output[1] = {0.0f};
    cmsFloat32Number lut[5] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f}; // 5 entries to satisfy DENS with OutChan=0
    cmsInterpParams p;
    // Initialize the required fields
    p.Table = lut;
    p.nOutputs = 1;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.Domain[2] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;
    p.opta[2] = 1.0f;

    TrilinearInterpFloat(Input, Output, &p);

    float expected = 1.5f;
    bool pass = almost_equal_float(Output[0], expected);
    std::string details = "Output=" + std::to_string(Output[0]) + ", expected=" + std::to_string(expected);
    report_result("test_trilinear_float_interior_single_output", pass, details);
}

// Test 2: Corner case at origin (Input = {0,0,0}) for a single output channel
// Expect d000 which is LutTable[0] (here 0.0)
static void test_trilinear_float_origin_corner_single_output() {
    cmsFloat32Number Input[3] = {0.0f, 0.0f, 0.0f};
    cmsFloat32Number Output[1] = {0.0f};
    cmsFloat32Number lut[5] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    cmsInterpParams p;
    p.Table = lut;
    p.nOutputs = 1;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.Domain[2] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;
    p.opta[2] = 1.0f;

    TrilinearInterpFloat(Input, Output, &p);

    float expected = 0.0f;
    bool pass = almost_equal_float(Output[0], expected);
    std::string details = "Output=" + std::to_string(Output[0]) + ", expected=" + std::to_string(expected);
    report_result("test_trilinear_float_origin_corner_single_output", pass, details);
}

// Test 3: Multi-output (nOutputs=2) to exercise per-output channel LUT indexing.
// Input = {0.5,0.5,0.5} with LUT {0,1,2,3,4} as before.
// Expected outputs: 1.5 for channel 0 and 2.5 for channel 1 (based on shifting LUT index by OutChan)
static void test_trilinear_float_interior_two_outputs() {
    cmsFloat32Number Input[3] = {0.5f, 0.5f, 0.5f};
    cmsFloat32Number Output[2] = {0.0f, 0.0f};
    cmsFloat32Number lut[5] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    cmsInterpParams p;
    p.Table = lut;
    p.nOutputs = 2;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.Domain[2] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;
    p.opta[2] = 1.0f;

    TrilinearInterpFloat(Input, Output, &p);

    float expected0 = 1.5f; // as in single-output case
    float expected1 = 2.5f; // shifted by 1 in LUT indexing
    bool pass0 = almost_equal_float(Output[0], expected0);
    bool pass1 = almost_equal_float(Output[1], expected1);
    std::string details = "Output0=" + std::to_string(Output[0]) + ", expected0=" + std::to_string(expected0) +
                          "; Output1=" + std::to_string(Output[1]) + ", expected1=" + std::to_string(expected1);
    report_result("test_trilinear_float_interior_two_outputs", pass0 && pass1, details);
}

// Test 4: Clipping behavior for negative inputs (ensures inputs are clamped to 0 before interpolation)
static void test_trilinear_float_negative_input_clipping() {
    cmsFloat32Number Input[3] = {-1.0f, -0.5f, 0.0f}; // all inputs should clamp to 0
    cmsFloat32Number Output[1] = {0.0f};
    cmsFloat32Number lut[5] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    cmsInterpParams p;
    p.Table = lut;
    p.nOutputs = 1;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.Domain[2] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;
    p.opta[2] = 1.0f;

    TrilinearInterpFloat(Input, Output, &p);

    float expected = 0.0f; // since all clamps lead to x0=y0=z0=0 and d000=lut[0]
    bool pass = almost_equal_float(Output[0], expected);
    std::string details = "Output=" + std::to_string(Output[0]) + ", expected=" + std::to_string(expected);
    report_result("test_trilinear_float_negative_input_clipping", pass, details);
}

// Main function to run all tests
int main() {
    std::cout << "Running TrilinearInterpFloat test suite (C++11, no GTest)..." << std::endl;
    test_trilinear_float_interior_single_output();
    test_trilinear_float_origin_corner_single_output();
    test_trilinear_float_interior_two_outputs();
    test_trilinear_float_negative_input_clipping();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}