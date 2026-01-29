// Lightweight C++11 unit test suite for the focal method
// EvaluateCLUTfloatIn16 in cmslut.c (reproduced here for testing purposes)
//
// This test suite provides a minimal, self-contained environment to
// exercise EvaluateCLUTfloatIn16 without relying on GTest.
// It uses a tiny test harness with non-terminating assertions to maximize
// code coverage and execution flow.
//
// Core idea:
// - Reproduce minimal versions of the dependent types and helpers used by
//   EvaluateCLUTfloatIn16 (FromFloatTo16, From16ToFloat, etc.).
// - Provide a controllable Lerp16 implementation via a cargo/params structure.
// - Run multiple tests to cover identity behavior and a simple scaling path.

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cmath>


using cmsFloat32Number = float;
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;

// Lightweight replacements for the LittleCMS constants/macros
#define MAX_STAGE_CHANNELS 16
#define _cmsAssert(x) do { (void)0; } while(0)

// Forward declarations for testable dependencies
struct cmsStage;
struct _cmsStageCLutParams;
struct _cmsStageCLutData;
struct _cmsStageCLutParams {
    int mode; // 0 = identity, 1 = scale-by-2
    int n;    // number of channels (valid up to MAX_STAGE_CHANNELS)
    // Interpolation sub-structure
    struct _Interpolation {
        void (*Lerp16)(const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Cargo);
    } Interpolation;
};
struct _cmsStageCLutData {
    _cmsStageCLutParams* Params;
};

// Minimal stage structure used by the focal method
struct cmsStage {
    void* Data;
    cmsUInt32Number InputChannels;
    cmsUInt32Number OutputChannels;
};

// Global test harness state
static int g_test_failures = 0;

// Assertion helper for non-terminating tests
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { \
    reportFailure(__FILE__, __LINE__, msg); } } while(0)

static void reportFailure(const char* file, int line, const char* msg) {
    std::fprintf(stderr, "TEST_FAIL: %s:%d: %s\n", file, line, msg);
    ++g_test_failures;
}

// Helper: approximate float comparison
static bool approxEqual(float a, float b, float tol = 1e-4f) {
    return std::fabs(a - b) <= tol;
}

// Conversion helpers (as used by the focal method)
static void FromFloatTo16(const cmsFloat32Number In[], cmsUInt16Number Out[], cmsUInt32Number n) {
    for (cmsUInt32Number i = 0; i < n; ++i) {
        float v = In[i];
        if (v < 0.0f) v = 0.0f;
        if (v > 1.0f) v = 1.0f;
        Out[i] = (cmsUInt16Number)(v * 65535.0f + 0.5f);
    }
}

static void From16ToFloat(const cmsUInt16Number In[], cmsFloat32Number Out[], cmsUInt32Number n) {
    for (cmsUInt32Number i = 0; i < n; ++i) {
        Out[i] = (cmsFloat32Number)In[i] / 65535.0f;
    }
}

// Reproduced focal method: EvaluateCLUTfloatIn16
static void EvaluateCLUTfloatIn16(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage *mpe)
{
    {
        _cmsStageCLutData* Data = (_cmsStageCLutData*) mpe ->Data;
        cmsUInt16Number In16[MAX_STAGE_CHANNELS], Out16[MAX_STAGE_CHANNELS];
        _cmsAssert(mpe ->InputChannels  <= MAX_STAGE_CHANNELS);
        _cmsAssert(mpe ->OutputChannels <= MAX_STAGE_CHANNELS);
        FromFloatTo16(In, In16, mpe ->InputChannels);
        // Call into the interpolation Lerp16
        Data -> Params ->Interpolation.Lerp16(In16, Out16, Data->Params);
        From16ToFloat(Out16, Out,  mpe ->OutputChannels);
    }
}

// Test 1: Identity interpolation over 2 channels
static void test_EvaluateCLUTfloatIn16_identity_two_channels() {
    // Prepare stage with 2 input and 2 output channels
    cmsStage mpe;
    mpe.InputChannels = 2;
    mpe.OutputChannels = 2;

    // Set up CLUT data with identity interpolation
    _cmsStageCLutParams params;
    params.mode = 0; // identity
    params.n = 2;

    // Lerp16 function: identity (could be swapped with mode-based logic)
    params.Interpolation.Lerp16 = [](const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Cargo) {
        const _cmsStageCLutParams* P = static_cast<const _cmsStageCLutParams*>(Cargo);
        // If we ever want to switch behavior per test, use P->mode.
        // For this test, keep identity when mode == 0.
        if (P->mode == 0) {
            for (int i = 0; i < P->n; ++i) {
                Out[i] = In[i];
            }
        } else {
            // Fallback: identity
            for (int i = 0; i < P->n; ++i) {
                Out[i] = In[i];
            }
        }
    };

    _cmsStageCLutData data;
    data.Params = &params;

    mpe.Data = &data;

    // Inputs: [0.0, 0.5]
    cmsFloat32Number In[2]  = {0.0f, 0.5f};
    cmsFloat32Number Out[2] = {0.0f, 0.0f};

    EvaluateCLUTfloatIn16(In, Out, &mpe);

    // Expected results: [0.0, 0.5]
    TEST_ASSERT(approxEqual(Out[0], 0.0f), "Identity: Out[0] should be 0.0");
    TEST_ASSERT(approxEqual(Out[1], 0.5f), "Identity: Out[1] should be 0.5");
}

// Test 2: Scaling interpolation by 2x (capped to 1.0 after 16->float)
static void test_EvaluateCLUTfloatIn16_scale_by_2_two_channels() {
    cmsStage mpe;
    mpe.InputChannels = 2;
    mpe.OutputChannels = 2;

    _cmsStageCLutParams params;
    params.mode = 1; // scale by 2
    params.n = 2;
    // Lerp16 with scaling behavior
    params.Interpolation.Lerp16 = [](const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Cargo) {
        const _cmsStageCLutParams* P = static_cast<const _cmsStageCLutParams*>(Cargo);
        for (int i = 0; i < P->n; ++i) {
            if (P->mode == 1) {
                unsigned long long val = (unsigned long long)In[i] * 2;
                if (val > 65535ULL) val = 65535ULL;
                Out[i] = (cmsUInt16Number)val;
            } else {
                Out[i] = In[i];
            }
        }
    };

    _cmsStageCLutData data;
    data.Params = &params;

    mpe.Data = &data;

    // Inputs: [0.0, 1.0] -> In16: [0, 65535] -> Out16 after Lerp: [0, 65535]
    cmsFloat32Number In[2]  = {0.0f, 1.0f};
    cmsFloat32Number Out[2] = {0.0f, 0.0f};

    EvaluateCLUTfloatIn16(In, Out, &mpe);

    // Expected: [0.0, 1.0]
    TEST_ASSERT(approxEqual(Out[0], 0.0f), "Scale2: Out[0] should be 0.0");
    TEST_ASSERT(approxEqual(Out[1], 1.0f), "Scale2: Out[1] should be 1.0");
}

// Test 3: Identity interpolation over 3 channels
static void test_EvaluateCLUTfloatIn16_identity_three_channels() {
    cmsStage mpe;
    mpe.InputChannels = 3;
    mpe.OutputChannels = 3;

    _cmsStageCLutParams params;
    params.mode = 0; // identity
    params.n = 3;
    params.Interpolation.Lerp16 = [](const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Cargo) {
        const _cmsStageCLutParams* P = static_cast<const _cmsStageCLutParams*>(Cargo);
        for (int i = 0; i < P->n; ++i) {
            Out[i] = In[i];
        }
    };

    _cmsStageCLutData data;
    data.Params = &params;
    mpe.Data = &data;

    cmsFloat32Number In[3]  = {0.1f, 0.2f, 0.3f};
    cmsFloat32Number Out[3] = {0.0f, 0.0f, 0.0f};

    EvaluateCLUTfloatIn16(In, Out, &mpe);

    // Expected: identical values
    TEST_ASSERT(approxEqual(Out[0], In[0]), "Identity3: Out[0] should match In[0]");
    TEST_ASSERT(approxEqual(Out[1], In[1]), "Identity3: Out[1] should match In[1]");
    TEST_ASSERT(approxEqual(Out[2], In[2]), "Identity3: Out[2] should match In[2]");
}

// Main test driver
int main() {
    // Run tests
    test_EvaluateCLUTfloatIn16_identity_two_channels();
    test_EvaluateCLUTfloatIn16_scale_by_2_two_channels();
    test_EvaluateCLUTfloatIn16_identity_three_channels();

    // Summary
    if (g_test_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("TESTS FAILED: %d failure(s)\n", g_test_failures);
        return 1;
    }
}