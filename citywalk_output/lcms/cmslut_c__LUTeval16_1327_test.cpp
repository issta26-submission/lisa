// cmslut_test.cpp
// Lightweight unit tests for the focal method _LUTeval16 in cmslut.c
// - No GTest: uses simple bool-returning tests and a minimal harness in main()
// - Tests focus on _LUTeval16 behavior using a small synthetic pipeline
// - Uses only C++11 standard library and cms library primitives
// - Includes explanatory comments for each test

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include Little CMS core headers to access basic types.

// Ensure C linkage for the focal function when linking against the C library.
extern "C" {
    // Prototype for the focal function. Signature may be macro-expanded in the library.
    // We declare a C-compatible prototype to enable linking.
    void _LUTeval16(const cmsUInt16Number In[], cmsUInt16Number Out[], const void* D);
}

// Local helper: a lightweight "stage" used to create a tiny pipeline for tests.
// We rely on the real cmsStage structure layout: the TestStage struct's first member
// is the cmsStage base, so a pointer to base is safely reinterpretable as TestStage*.
struct TestStage;

// TestStage wraps a cmsStage and stores an id used by the EvalPtr implementation.
struct TestStage {
    cmsStage base;  // must be first member to allow casting(from base pointer)
    int id;          // 0-based stage id for behavior customization
};

// Stage evaluator that manipulates the float domain used inside the LUT pipeline.
// It mirrors the pattern seen in _LUTeval16: In[] is float domain, Out[] is written by the stage.
static void TestStageEval(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe)
{
    // Recover the test stage object from the cmsStage pointer
    const TestStage* ts = reinterpret_cast<const TestStage*>(mpe);
    int stage_id = ts->id;

    // Simple deterministic behavior:
    // - Stage with id == 0 acts as an identity (Out = In)
    // - Stage with id == 1 adds 1.0f to each channel
    // - Other ids also act deterministically (e.g., identity) to keep tests robust
    const int channels = 2; // We'll use 2-channel pipelines for tests
    if (stage_id == 0) {
        Out[0] = In[0];
        Out[1] = In[1];
    } else if (stage_id == 1) {
        Out[0] = In[0] + 1.0f;
        Out[1] = In[1] + 1.0f;
    } else {
        // Fallback: identity
        Out[0] = In[0];
        Out[1] = In[1];
    }
}

// Helper: compute exact equality of two uint16 arrays
static bool equal16(const cmsUInt16Number* a, const cmsUInt16Number* b, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Test 1: Zero-stage LUT should yield a true round-trip for 2 channels.
// We verify that With no elements, _LUTeval16 performs a 16-bit <-> float round-trip
// consistent with From16ToFloat / FromFloatTo16 (no stage processing).
bool test_LUTeval16_no_stages_identity()
{
    // Prepare a tiny LUT with 0 elements
    cmsPipeline lut;
    lut.InputChannels  = 2;
    lut.OutputChannels = 2;
    lut.Elements = NULL;

    // Input data (two channels)
    cmsUInt16Number In[2]  = { 0, 65535 };
    cmsUInt16Number Out[2] = { 0, 0 };

    // Call focal function
    _LUTeval16(In, Out, &lut);

    // Compute expected via explicit round-trip conversions (no stages)
    cmsFloat32Number tmp[2];
    cmsUInt16Number expected[2];
    From16ToFloat(In, tmp, lut.InputChannels);
    FromFloatTo16(tmp, expected, lut.OutputChannels);

    bool pass = equal16(Out, expected, 2);
    if (pass) {
        std::cout << "[PASS] test_LUTeval16_no_stages_identity\n";
    } else {
        std::cerr << "[FAIL] test_LUTeval16_no_stages_identity: output differs from expected\n";
        std::cerr << "  Out:      [" << (int)Out[0] << ", " << (int)Out[1] << "]\n";
        std::cerr << "  Expected: [" << (int)expected[0] << ", " << (int)expected[1] << "]\n";
    }
    return pass;
}

// Test 2: Two-stage pipeline: Stage 0 (identity) followed by Stage 1 (add 1)
// Expect final Out to be InFloat + 1 (after conversion sequences).
bool test_LUTeval16_two_stages_identity_then_add1()
{
    // Allocate two test stages
    TestStage s1; // id 0: identity
    TestStage s2; // id 1: add 1
    s1.id = 0;
    s2.id = 1;
    s1.base.EvalPtr = TestStageEval;
    s2.base.EvalPtr = TestStageEval;
    // Link stages: s1 -> s2
    s1.base.Next = &s2.base;
    s2.base.Next = NULL;

    // Build LUT with 2 stages
    cmsPipeline lut;
    lut.InputChannels  = 2;
    lut.OutputChannels = 2;
    lut.Elements = &s1.base;

    // Prepare input
    cmsUInt16Number In[2]  = { 1000, 4000 };
    cmsUInt16Number Out[2] = { 0, 0 };

    // Run focal function
    _LUTeval16(In, Out, &lut);

    // Compute expected manually:
    // 1) Convert In to float
    cmsFloat32Number tmp0[2];
    From16ToFloat(In, tmp0, lut.InputChannels);
    // 2) Stage 0: identity (tmp1 == tmp0)
    cmsFloat32Number tmp1[2];
    tmp1[0] = tmp0[0];
    tmp1[1] = tmp0[1];
    // 3) Stage 1: add 1
    cmsFloat32Number tmp2[2];
    tmp2[0] = tmp1[0] + 1.0f;
    tmp2[1] = tmp1[1] + 1.0f;
    // 4) Convert to 16-bit
    cmsUInt16Number expected[2];
    FromFloatTo16(tmp2, expected, lut.OutputChannels);

    bool pass = equal16(Out, expected, 2);
    if (pass) {
        std::cout << "[PASS] test_LUTeval16_two_stages_identity_then_add1\n";
    } else {
        std::cerr << "[FAIL] test_LUTeval16_two_stages_identity_then_add1: output differs from expected\n";
        std::cerr << "  Out:      [" << (int)Out[0] << ", " << (int)Out[1] << "]\n";
        std::cerr << "  Expected: [" << (int)expected[0] << ", " << (int)expected[1] << "]\n";
    }
    return pass;
}

// Test 3: Ensure that a non-empty LUT with two stages yields a deterministic difference.
// We'll use a different two-stage configuration: Stage 0 (add 0) followed by Stage 1 (add 2).
bool test_LUTeval16_two_stages_add2_variant()
{
    // Two stages, but different behavior
    TestStage s1; // id 0: add 0
    TestStage s2; // id 2: add 2 (we'll keep behavior: +id)
    s1.id = 0;
    s2.id = 2;
    s1.base.EvalPtr = TestStageEval;
    s2.base.EvalPtr = TestStageEval;
    s1.base.Next = &s2.base;
    s2.base.Next = NULL;

    cmsPipeline lut;
    lut.InputChannels  = 2;
    lut.OutputChannels = 2;
    lut.Elements = &s1.base;

    cmsUInt16Number In[2]  = { 12345, 54321 };
    cmsUInt16Number Out[2] = { 0, 0 };

    _LUTeval16(In, Out, &lut);

    // Expected: InFloat + (0 + 2) since Stage 1 adds 2; identity for Stage 0
    cmsFloat32Number tmp0[2];
    From16ToFloat(In, tmp0, lut.InputChannels);

    cmsFloat32Number tmp1[2];
    // Stage 0: +0
    tmp1[0] = tmp0[0] + 0.0f;
    tmp1[1] = tmp0[1] + 0.0f;
    // Stage 1: +2
    cmsFloat32Number tmp2[2];
    tmp2[0] = tmp1[0] + 2.0f;
    tmp2[1] = tmp1[1] + 2.0f;

    cmsUInt16Number expected[2];
    FromFloatTo16(tmp2, expected, lut.OutputChannels);

    bool pass = equal16(Out, expected, 2);
    if (pass) {
        std::cout << "[PASS] test_LUTeval16_two_stages_add2_variant\n";
    } else {
        std::cerr << "[FAIL] test_LUTeval16_two_stages_add2_variant: output differs from expected\n";
        std::cerr << "  Out:      [" << (int)Out[0] << ", " << (int)Out[1] << "]\n";
        std::cerr << "  Expected: [" << (int)expected[0] << ", " << (int)expected[1] << "]\n";
    }
    return pass;
}

// Main entry: run all tests and report summary
int main()
{
    bool all_passed = true;
    std::vector<bool> results;

    results.push_back(test_LUTeval16_no_stages_identity());
    results.push_back(test_LUTeval16_two_stages_identity_then_add1());
    results.push_back(test_LUTeval16_two_stages_add2_variant());

    // Aggregate results
    for (size_t i = 0; i < results.size(); ++i) {
        if (!results[i]) all_passed = false;
    }

    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}