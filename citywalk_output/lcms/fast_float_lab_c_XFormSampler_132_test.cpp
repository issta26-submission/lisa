// Minimal C++11 test suite for the focal method XFormSampler and its dependencies.
// This test suite does not rely on GTest and uses a lightweight harness implemented in main().

#include <cstdio>
#include <cmath>
#include <fast_float_internal.h>
#include <cstdlib>


// Domain-agnostic typedefs and macros to mirror the focal environment.
typedef float cmsFloat32Number;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0
#define CMSREGISTER

// Forward declarations (to mimic the real header usage in the focal project)
struct Data;
struct ResamplingContainer;

// Lightweight implementations to enable unit testing without the full downstream library.
// These are test doubles to drive and verify the focal behavior deterministically.

// The Data structure used by the container (as seen in the focal code)
struct Data {
    const cmsFloat32Number* sigmoidOut; // Lookup table for the sigmoid mapping
};

// The ResamplingContainer holding references required by XFormSampler
struct ResamplingContainer {
    Data* data;
    void* original;
};

// Local mock implementation of LinLerp1D used by XFormSampler
// For testability, we implement a simple behavior:
//   If LutTable is provided, return Value + LutTable[0]
//   Otherwise, return Value
cmsFloat32Number LinLerp1D(cmsFloat32Number Value, const cmsFloat32Number* LutTable)
{
    if (LutTable) {
        return Value + LutTable[0];
    }
    return Value;
}

// Mock implementation of the pipeline evaluation routine used by XFormSampler
// We choose a deterministic transformation that is easy to reason about in tests.
void cmsPipelineEvalFloat(const cmsFloat32Number in[3], cmsFloat32Number out[3], void* /*orig*/)
{
    // Deterministic behavior: simply double each component
    out[0] = in[0] * 2.0f;
    out[1] = in[1] * 2.0f;
    out[2] = in[2] * 2.0f;
}

// The focal method under test copied from the provided source.
// Note: The braces mirror the snippet present in the prompt.
int XFormSampler(CMSREGISTER const cmsFloat32Number In[], CMSREGISTER cmsFloat32Number Out[], CMSREGISTER void* Cargo)
{
{
    ResamplingContainer* container = (ResamplingContainer*)Cargo;
    cmsFloat32Number linearized[3];
    // Apply inverse sigmoid
    linearized[0] = In[0];
    linearized[1] = LinLerp1D(In[1], container->data->sigmoidOut);
    linearized[2] = LinLerp1D(In[2], container->data->sigmoidOut);
    cmsPipelineEvalFloat(linearized, Out, container->original);
    return TRUE;
}
}

// Simple helper for floating-point comparisons
const float EPS = 1e-6f;
bool almostEqual(float a, float b, float eps = EPS) {
    return std::fabs(a - b) <= eps;
}

// Test 1: XFormSampler with non-null sigmoidOut LUT
// This validates that linearized[1] and linearized[2] incorporate the LUT value
// and that the downstream pipeline evaluation doubles the final result.
void test_XFormSampler_withNonNullLut() {
    // Prepare input
    cmsFloat32Number In[3] = { 1.0f, 0.5f, 0.8f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    // Setup LUT and container
    cmsFloat32Number sigmoid[] = { 0.1f }; // LUT with a single value
    Data d;
    d.sigmoidOut = sigmoid;
    ResamplingContainer rc;
    rc.data = &d;
    rc.original = nullptr;

    int r = XFormSampler(In, Out, &rc);

    // Expected:
    // linearized[0] = 1.0
    // linearized[1] = 0.5 + 0.1 = 0.6
    // linearized[2] = 0.8 + 0.1 = 0.9
    // cmsPipelineEvalFloat doubles: Out = {2.0, 1.2, 1.8}
    bool pass = (r == TRUE) &&
                almostEqual(Out[0], 2.0f) &&
                almostEqual(Out[1], 1.2f) &&
                almostEqual(Out[2], 1.8f);

    if (pass) {
        std::printf("[Test 1] XFormSampler_withNonNullLut: PASS\n");
    } else {
        std::printf("[Test 1] XFormSampler_withNonNullLut: FAIL (Out = { %g, %g, %g }, r = %d)\n",
                    (double)Out[0], (double)Out[1], (double)Out[2], r);
    }
}

// Test 2: XFormSampler with null sigmoidOut LUT
// This ensures the code handles a missing LUT gracefully by keeping In[1] and In[2] unchanged
// before the pipeline evaluation.
void test_XFormSampler_withNullLut() {
    // Prepare input
    cmsFloat32Number In[3] = { 1.0f, 0.5f, 0.8f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    // Setup container with NULL LUT
    Data d;
    d.sigmoidOut = nullptr;
    ResamplingContainer rc;
    rc.data = &d;
    rc.original = nullptr;

    int r = XFormSampler(In, Out, &rc);

    // Expected:
    // linearized[0] = 1.0
    // linearized[1] = 0.5 (no LUT)
    // linearized[2] = 0.8 (no LUT)
    // cmsPipelineEvalFloat doubles: Out = {2.0, 1.0, 1.6}
    bool pass = (r == TRUE) &&
                almostEqual(Out[0], 2.0f) &&
                almostEqual(Out[1], 1.0f) &&
                almostEqual(Out[2], 1.6f);

    if (pass) {
        std::printf("[Test 2] XFormSampler_withNullLut: PASS\n");
    } else {
        std::printf("[Test 2] XFormSampler_withNullLut: FAIL (Out = { %g, %g, %g }, r = %d)\n",
                    (double)Out[0], (double)Out[1], (double)Out[2], r);
    }
}

// Test 3: Direct test of LinLerp1D behavior with a non-null LUT
// Ensures the LUT contribution is correctly added to the value.
void test_LinLerp1D_basic() {
    cmsFloat32Number val = 0.3f;
    cmsFloat32Number lut[] = { 0.2f };
    cmsFloat32Number res = LinLerp1D(val, lut);
    bool pass = almostEqual(res, 0.5f);
    if (pass) {
        std::printf("[Test 3] LinLerp1D_basic: PASS\n");
    } else {
        std::printf("[Test 3] LinLerp1D_basic: FAIL (res = %g)\n", (double)res);
    }
}

// Entry point: run all tests and summarize
int main() {
    std::printf("Starting unit tests for XFormSampler and dependencies...\n");
    test_XFormSampler_withNonNullLut();
    test_XFormSampler_withNullLut();
    test_LinLerp1D_basic();
    std::printf("Unit tests completed.\n");
    return 0;
}