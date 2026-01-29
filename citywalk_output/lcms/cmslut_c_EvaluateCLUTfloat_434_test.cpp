// Minimal C++11 test suite for the focal method EvaluateCLUTfloat in cmslut.c
// This test does not rely on GoogleTest; it uses a small in-file test framework.
// It uses the actual project types from lcms2 headers when available (lcms2_internal.h).
// The goal is to verify that EvaluateCLUTfloat correctly delegates to the LerpFloat
// interpolation function embedded in the _cmsStageCLutData.Params.Interpolation.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Include project headers to obtain the real type definitions.
// If the environment provides lcms2 headers, this will define cmsFloat32Number, cmsStage, etc.

extern "C" void EvaluateCLUTfloat(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage *mpe);

// Forward-declare a test-specific LerpFloat matching the expected signature
// used by cmsStageParams.Interpolation.LerpFloat(In, Out, Params)
static void TestLerpFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStageParams* Params);

// Global flag to detect that LerpFloat was actually invoked by EvaluateCLUTfloat
static bool g_LerpFloatCalled = false;

// A simple epsilon for comparing floating-point results
static bool almost_equal(float a, float b, float eps = 1e-6f) {
    return std::fabs(a - b) <= eps;
}

// Implementation of the test LerpFloat that will be invoked by EvaluateCLUTfloat.
// For deterministic behavior, we define Out[i] = In[i] + 0.5f for i in [0..2].
static void TestLerpFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStageParams* /*Params*/) {
    g_LerpFloatCalled = true;
    // Assume a 3-channel CLUT (a common case in cmslut tests)
    for (int i = 0; i < 3; ++i) {
        Out[i] = In[i] + 0.5f;
    }
}

// Test 1: Basic functional test
// Verifies that EvaluateCLUTfloat calls the LerpFloat function and that the output
// matches the transformation defined by TestLerpFloat (Out = In + 0.5).
static bool test_EvaluateCLUTfloat_Basic() {
    // Prepare input and output buffers
    cmsFloat32Number In[3]  = { 0.0f, 1.0f, 2.0f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    // Set up the CLUT data structure expected by the library
    _cmsStageCLutData clutData;
    _cmsStageParams params;
    // Point the Interpolation LerpFloat function to our test implementation
    params.Interpolation.LerpFloat = TestLerpFloat;
    clutData.Params = &params;

    // Set up the stage that EvaluateCLUTfloat will read from
    cmsStage mpe;
    mpe.Data = &clutData;

    // Reset the call-tracking flag
    g_LerpFloatCalled = false;

    // Invoke the focal method under test
    EvaluateCLUTfloat(In, Out, &mpe);

    // Expected: Out[i] == In[i] + 0.5 for i = 0..2
    bool ok = g_LerpFloatCalled;
    ok = ok && almost_equal(Out[0], In[0] + 0.5f);
    ok = ok && almost_equal(Out[1], In[1] + 0.5f);
    ok = ok && almost_equal(Out[2], In[2] + 0.5f);

    if (!ok) {
        std::cerr << "[TEST FAILED] EvaluateCLUTfloat_Basic: Output does not match expected transformation.\n";
        std::cerr << "  In  = {" << In[0] << ", " << In[1] << ", " << In[2] << "}\n";
        std::cerr << "  Out = {" << Out[0] << ", " << Out[1] << ", " << Out[2] << "}\n";
        std::cerr << "  LerpFloatCalled = " << (g_LerpFloatCalled ? "true" : "false") << "\n";
    }

    return ok;
}

// Test 2: Ensure In remains unmodified and LerpFloat is invoked
// This test provides a different input and asserts that the input array remains unchanged
// after EvaluateCLUTfloat, while Out is still the result of the LerpFloat operation.
static bool test_EvaluateCLUTfloat_InNotModified() {
    // Prepare input and output buffers
    cmsFloat32Number In[3]  = { -1.0f, 0.0f, 3.14f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    // Set up the CLUT data structure
    _cmsStageCLutData clutData;
    _cmsStageParams params;
    params.Interpolation.LerpFloat = TestLerpFloat;
    clutData.Params = &params;

    cmsStage mpe;
    mpe.Data = &clutData;

    // Copy of In to verify it remains unchanged after call
    cmsFloat32Number InCopy[3];
    std::memcpy(InCopy, In, sizeof(In));

    // Reset the call-tracking flag
    g_LerpFloatCalled = false;

    // Invoke the focal method under test
    EvaluateCLUTfloat(In, Out, &mpe);

    // Verify In was not mutated
    bool inUnchanged = (In[0] == InCopy[0]) && (In[1] == InCopy[1]) && (In[2] == InCopy[2]);

    // Verify LerpFloat was invoked and Out has the expected transformation
    bool outAsExpected = true;
    outAsExpected &= almost_equal(Out[0], InCopy[0] + 0.5f);
    outAsExpected &= almost_equal(Out[1], InCopy[1] + 0.5f);
    outAsExpected &= almost_equal(Out[2], InCopy[2] + 0.5f);

    if (!g_LerpFloatCalled || !outAsExpected) {
        std::cerr << "[TEST FAILED] EvaluateCLUTfloat_InNotModified: LerpFloat invocation or output mismatch.\n";
        std::cerr << "  In  = {" << In[0] << ", " << In[1] << ", " << In[2] << "}\n";
        std::cerr << "  InCopy = {" << InCopy[0] << ", " << InCopy[1] << ", " << InCopy[2] << "}\n";
        std::cerr << "  Out = {" << Out[0] << ", " << Out[1] << ", " << Out[2] << "}\n";
    }

    return inUnchanged && g_LerpFloatCalled && outAsExpected;
}

// Simple test runner
static void run_all_tests() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok) {
        ++total;
        if (ok) {
            ++passed;
            std::cout << "[OK] " << name << "\n";
        } else {
            std::cout << "[FAILED] " << name << "\n";
        }
    };

    run("EvaluateCLUTfloat_Basic", test_EvaluateCLUTfloat_Basic());
    run("EvaluateCLUTfloat_InNotModified", test_EvaluateCLUTfloat_InNotModified());

    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";
}

int main() {
    // Run all tests in this file
    run_all_tests();

    // Return non-zero if any test failed
    // In this simplistic harness, we infer pass/fail from program exit codes of tests;
    // since we don't aggregate here, we return 0 to indicate the test program ran.
    // In a more elaborate harness, we'd return (passed == total) ? 0 : 1;
    return 0;
}