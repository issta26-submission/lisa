/*
Unit test suite for the focal method _LUTevalFloat located in cmslut.c

Notes:
- This test suite uses lightweight, hand-rolled stubs for the essential CMS types
  (cmsPipeline, cmsStage) and the function prototype for _LUTevalFloat.
- It does not rely on Google Test (GTest) or any external testing framework.
- Tests are designed to exercise key behavior of _LUTevalFloat:
  1) Identity/No-op behavior with a single stage
  2) Composition of two single-channel stages (x2 then +1)
  3) Composition of three single-channel stages (x2, *0.5, +1)
  4) Multi-channel (2-channel) pipeline path
  5) No stages (NULL Elements) path
- All assertions are non-terminating (they print results and continue), suitable for manual inspection or CI capture.
- The code assumes cmsFloat32Number is a 32-bit float (cmsFloat32Number = float) and that
  _LUTevalFloat has the signature exactly as shown in the focal method.
  extern "C" linkage is used to call the C function from C++.

This file should be compiled and linked together with cmslut.c (and any required headers)
in a C++11-compatible environment.
*/

#include <limits>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cmath>


// Domain knowledge hints: avoid relying on private members or GMock. Use only provided APIs.
// We provide minimal stand-ins for the CMS types used by the focal method's interface.

using cmsFloat32Number = float;

// Lightweight mock definitions to match the minimal usage in _LUTevalFloat
struct cmsStage {
    cmsStage* Next;
    void (*EvalPtr)(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe);
    // In real CMS, there are more fields; our test only relies on Next and EvalPtr.
};

struct cmsPipeline {
    int InputChannels;
    int OutputChannels;
    cmsStage* Elements; // Head of linked list of stages
};

// Declaration of the focal function under test (C linkage)
extern "C" void _LUTevalFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], const void* D);

////////////////////////////////////////////////////////////////////////////////
// Helper: floating-point comparison with tolerance
static bool almost_equal(float a, float b, float tol = 1e-6f) {
    if (std::isnan(a) || std::isnan(b)) return false;
    return std::fabs(a - b) <= tol;
}

////////////////////////////////////////////////////////////////////////////////
// Test 1: Identity (single-channel, one-stage)
static void test_LUTevalFloat_identity_single_channel(bool &hadFailures) {
    // #1 Explain: Verifies a simple identity stage passes input to output unchanged.
    static cmsStage stage_identity = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe; // unused
        Out[0] = In[0];
    } };

    cmsPipeline lut;
    lut.InputChannels  = 1;
    lut.OutputChannels = 1;
    lut.Elements = &stage_identity;
    stage_identity.Next = nullptr;

    cmsFloat32Number In[1]  = { 0.123f };
    cmsFloat32Number Out[1] = { 0.0f };

    _LUTevalFloat(In, Out, (void*)(&lut));

    bool ok = almost_equal(Out[0], In[0]);
    std::cout << "Test 1 - Identity single-channel: " << (ok ? "PASS" : "FAIL")
              << " (Out=" << Out[0] << ", In=" << In[0] << ")\n";
    if (!ok) hadFailures = true;
}

////////////////////////////////////////////////////////////////////////////////
// Test 2: Two-stage single-channel pipeline: Out = In * 2, then +1
static void test_LUTevalFloat_two_stage_double_then_plus_one(bool &hadFailures) {
    // Stage 1: multiply by 2
    static cmsStage stage_double = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe;
        Out[0] = In[0] * 2.0f;
    } };

    // Stage 2: add 1
    static cmsStage stage_plus1 = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe;
        Out[0] = In[0] + 1.0f;
    } };

    // Link stages: stage_double -> stage_plus1
    stage_double.Next = &stage_plus1;
    stage_plus1.Next  = nullptr;

    cmsPipeline lut;
    lut.InputChannels  = 1;
    lut.OutputChannels = 1;
    lut.Elements = &stage_double;

    cmsFloat32Number In[1]  = { 1.5f };
    cmsFloat32Number Out[1] = { 0.0f };

    _LUTevalFloat(In, Out, (void*)(&lut));

    float expected = In[0] * 2.0f + 1.0f;
    bool ok = almost_equal(Out[0], expected);
    std::cout << "Test 2 - Two-stage (x2 then +1) single-channel: " 
              << (ok ? "PASS" : "FAIL") << " (Out=" << Out[0] << ", Expected=" << expected << ")\n";
    if (!ok) hadFailures = true;
}

////////////////////////////////////////////////////////////////////////////////
// Test 3: Three-stage single-channel pipeline: Out = (((In * 2) * 0.5) + 1)
static void test_LUTevalFloat_three_stage_sequence(bool &hadFailures) {
    // Stage 1: multiply by 2
    static cmsStage s1 = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe;
        Out[0] = In[0] * 2.0f;
    } };

    // Stage 2: multiply by 0.5
    static cmsStage s2 = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe;
        Out[0] = In[0] * 0.5f;
    } };

    // Stage 3: add 1
    static cmsStage s3 = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe;
        Out[0] = In[0] + 1.0f;
    } };

    s1.Next = &s2;
    s2.Next = &s3;
    s3.Next = nullptr;

    cmsPipeline lut;
    lut.InputChannels  = 1;
    lut.OutputChannels = 1;
    lut.Elements = &s1;

    cmsFloat32Number In[1]  = { 3.0f };
    cmsFloat32Number Out[1] = { 0.0f };

    _LUTevalFloat(In, Out, (void*)(&lut));

    // Expected: ((3 * 2) * 0.5) + 1 = (6 * 0.5) + 1 = 3 + 1 = 4
    float expected = (In[0] * 2.0f) * 0.5f + 1.0f;
    bool ok = almost_equal(Out[0], expected);
    std::cout << "Test 3 - Three-stage single-channel sequence: "
              << (ok ? "PASS" : "FAIL") << " (Out=" << Out[0] << ", Expected=" << expected << ")\n";
    if (!ok) hadFailures = true;
}

////////////////////////////////////////////////////////////////////////////////
// Test 4: Multi-channel (2-channel) stage: diag transform per channel
static void test_LUTevalFloat_two_channel_diag_transform(bool &hadFailures) {
    // Stage: For channel 0: *2, channel 1: *3
    static cmsStage s = { nullptr, [](const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
        (void)mpe;
        Out[0] = In[0] * 2.0f;
        Out[1] = In[1] * 3.0f;
    } };

    s.Next = nullptr;

    cmsPipeline lut;
    lut.InputChannels  = 2;
    lut.OutputChannels = 2;
    lut.Elements = &s;

    cmsFloat32Number In[2]  = { 1.0f, 2.0f };
    cmsFloat32Number Out[2] = { 0.0f, 0.0f };

    _LUTevalFloat(In, Out, (void*)(&lut));

    bool ok0 = almost_equal(Out[0], In[0] * 2.0f);
    bool ok1 = almost_equal(Out[1], In[1] * 3.0f);
    std::cout << "Test 4 - Two-channel diag transform: "
              << ((ok0 && ok1) ? "PASS" : "FAIL")
              << " (Out[0]=" << Out[0] << ", Expected0=" << In[0]*2.0f
              << "; Out[1]=" << Out[1] << ", Expected1=" << In[1]*3.0f << ")\n";
    if (!(ok0 && ok1)) hadFailures = true;
}

////////////////////////////////////////////////////////////////////////////////
// Test 5: No stages (Elements == NULL) should pass input to output
static void test_LUTevalFloat_no_stages_pass_through(bool &hadFailures) {
    cmsPipeline lut;
    lut.InputChannels  = 2;
    lut.OutputChannels = 2;
    lut.Elements = nullptr; // No stages

    cmsFloat32Number In[2]  = { 0.25f, -1.75f };
    cmsFloat32Number Out[2] = { 0.0f, 0.0f };

    _LUTevalFloat(In, Out, (void*)(&lut));

    bool ok0 = almost_equal(Out[0], In[0]);
    bool ok1 = almost_equal(Out[1], In[1]);
    std::cout << "Test 5 - No stages: "
              << ((ok0 && ok1) ? "PASS" : "FAIL")
              << " (Out[0]=" << Out[0] << ", Expected0=" << In[0]
              << "; Out[1]=" << Out[1] << ", Expected1=" << In[1] << ")\n";
    if (!(ok0 && ok1)) hadFailures = true;
}

////////////////////////////////////////////////////////////////////////////////
// Main: Run all tests
int main() {
    bool hadFailures = false;

    std::cout << "Running test suite for _LUTevalFloat (cmslut.c) - C++11 harness\n";

    test_LUTevalFloat_identity_single_channel(hadFailures);
    test_LUTevalFloat_two_stage_double_then_plus_one(hadFailures);
    test_LUTevalFloat_three_stage_sequence(hadFailures);
    test_LUTevalFloat_two_channel_diag_transform(hadFailures);
    test_LUTevalFloat_no_stages_pass_through(hadFailures);

    if (hadFailures) {
        std::cout << "Some tests FAILED. See outputs above for details.\n";
        return EXIT_FAILURE;
    } else {
        std::cout << "All tests PASSED.\n";
        return EXIT_SUCCESS;
    }
}