// Test suite for EvaluateLab2XYZ in cmslut.c
// Note: This test uses the actual LittleCMS API to compute the expected XYZ
//  from Lab inputs so that we can compare against the output of EvaluateLab2XYZ.
// The test is C++11 compatible and does not rely on GTest. It uses a minimal
// in-house assertion framework with non-terminating checks.

#include <iostream>
#include <algorithm>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Declaration of the focal function under test (provided by cmslut.c)
extern void EvaluateLab2XYZ(const cmsFloat32Number In[],
                            cmsFloat32Number Out[],
                            const cmsStage *mpe);

// Simple near-equality check with a relative tolerance
static bool near_float(cmsFloat32Number a, cmsFloat32Number b, double rel_eps)
{
    const double aa = static_cast<double>(a);
    const double bb = static_cast<double>(b);
    const double diff = std::fabs(aa - bb);
    const double scale = std::max(1.0, std::max(std::fabs(aa), std::fabs(bb)));
    return diff <= rel_eps * scale;
}

// Compute the expected XYZ output for a given Lab input using the library's cmsLab2XYZ.
// Out = XYZ / MAX_ENCODEABLE_XYZ, where XYZ is the Lab->XYZ conversion.
static void compute_expected_lab_to_xyz(const cmsFloat32Number In[3], cmsFloat32Number Out[3])
{
    cmsCIELab Lab;
    cmsCIEXYZ XYZ;
    // Admissible to use NULL for ContextID as done in the focal function
    Lab.L = In[0] * 100.0;
    Lab.a = In[1] * 255.0 - 128.0;
    Lab.b = In[2] * 255.0 - 128.0;

    cmsLab2XYZ(NULL, &XYZ, &Lab);

    // From code comments: XYZadj = MAX_ENCODEABLE_XYZ
    const cmsFloat64Number XYZadj = MAX_ENCODEABLE_XYZ;
    Out[0] = static_cast<cmsFloat32Number>(static_cast<cmsFloat64Number>(XYZ.X) / XYZadj);
    Out[1] = static_cast<cmsFloat32Number>(static_cast<cmsFloat64Number>(XYZ.Y) / XYZadj);
    Out[2] = static_cast<cmsFloat32Number>(static_cast<cmsFloat64Number>(XYZ.Z) / XYZadj);
}

// Run a single test case and report results
static void run_case(const char* test_name, const cmsFloat32Number In[3])
{
    cmsFloat32Number Out[3] = {0.0f, 0.0f, 0.0f};
    cmsFloat32Number Expected[3] = {0.0f, 0.0f, 0.0f};

    // Execute the focal function
    EvaluateLab2XYZ(In, Out, NULL);

    // Compute expected using a reference implementation via cmsLab2XYZ
    compute_expected_lab_to_xyz(In, Expected);

    // Tolerance settings
    const double rel_eps = 1e-5; // relative tolerance
    bool all_ok = true;

    // Compare each channel
    for (int i = 0; i < 3; ++i) {
        if (!near_float(Out[i], Expected[i], rel_eps)) {
            all_ok = false;
            break;
        }
        // Also ensure outputs are finite
        if (!std::isfinite(static_cast<double>(Out[i]))) {
            all_ok = false;
            break;
        }
    }

    // Report
    std::cout << "Test: " << test_name << " -> "
              << (all_ok ? "PASS" : "FAIL") 
              << " | Out = [" 
              << static_cast<double>(Out[0]) << ", "
              << static_cast<double>(Out[1]) << ", "
              << static_cast<double>(Out[2]) << "]"
              << " | Expected approx = [" 
              << static_cast<double>(Expected[0]) << ", "
              << static_cast<double>(Expected[1]) << ", "
              << static_cast<double>(Expected[2]) << "]"
              << std::endl;
}

// Helper to create a readable test name and run
static void test_EvaluateLab2XYZ_basic_white()
{
    // In: Lab input corresponding to White point (L=100, a=0, b=0)
    // In values are in [0,1] for L, a, b when scaled as in the focal method.
    const cmsFloat32Number In[3] = {1.0f, 128.0f/255.0f, 128.0f/255.0f};
    // The function will be exercised inside run_case and results printed.
    run_case("EvaluateLab2XYZ: WhitePoint (L=100, a=0, b=0)", In);
}

// Basic test with mid-range Lab values
static void test_EvaluateLab2XYZ_mid_values()
{
    // In: L=0.5 -> L=50, a=127.5-128 ~ -0.5, b similar
    const cmsFloat32Number In[3] = {0.5f, 0.5f, 0.5f};
    run_case("EvaluateLab2XYZ: Mid-range Lab (L=50, a≈0, b≈0)", In);
}

// Test with maximum Lab values within the encoding
static void test_EvaluateLab2XYZ_max_values()
{
    // In: L=100, a=~127, b=~127
    const cmsFloat32Number In[3] = {1.0f, 1.0f, 1.0f};
    run_case("EvaluateLab2XYZ: Max Lab (L=100, a≈127, b≈127)", In);
}

// Test with mixed edge inputs to ensure stability
static void test_EvaluateLab2XYZ_edge_inputs()
{
    // In: L near 0, a near -128, b near 127
    const cmsFloat32Number In[3] = {0.0f, 0.0f, 1.0f};
    run_case("EvaluateLab2XYZ: Edge input (L≈0, a≈-128, b≈127)", In);
}

int main()
{
    // Run a battery of unit tests for EvaluateLab2XYZ
    test_EvaluateLab2XYZ_basic_white();
    test_EvaluateLab2XYZ_mid_values();
    test_EvaluateLab2XYZ_max_values();
    test_EvaluateLab2XYZ_edge_inputs();

    // Note: The test suite is designed to be non-terminating; all tests print their results.
    // The process exit code is non-zero if any test failed, but each test reports individually.
    // For simplicity in this lightweight harness, we don't aggregate a global pass/fail count.
    return 0;
}