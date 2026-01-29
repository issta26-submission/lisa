// Test suite for EvaluateCurves in cmslut.c
// Note: This test uses a lightweight, self-contained harness (no GTest) and a minimal
// mock for cmsEvalToneCurveFloat. It is designed to be compiled together with cmslut.c
// in a single build unit. The goal is to exercise true/false branches and key logic
// paths of EvaluateCurves.

// Compile example (typical):
// g++ -std=c++11 -O2 -c cmslut.c this_test_file.cpp
// gcc -shared -o liblcms_mock.so this_test_file.o cmslut.o
// Or simply compile and link as one translation unit if your build system supports it.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstddef>


// Minimal type definitions to compile against cmslut.c
typedef float cmsFloat32Number;
typedef unsigned int cmsUInt32Number;

// Forward declaration of cmsStage to match the CMS API expected by cmslut.c
struct cmsStage {
    void* Data; // We only need this member for tests
};

// Forward declare tone curve type (actual layout is opaque to test)
struct cmsToneCurve;

// Forward declare EvaluateCurves from cmslut.c (C linkage)
extern "C" void EvaluateCurves(const cmsFloat32Number In[],
                               cmsFloat32Number Out[],
                               const cmsStage *mpe);

// Forward declare cmsEvalToneCurveFloat (we will provide a mock in test)
extern "C" cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve* curve, cmsFloat32Number In);

// Since cmsToneCurve is an opaque type in the real library, declare it as a forward-declared struct
struct cmsToneCurve;

// Globals used by the mock function to determine which curve is being evaluated
static const cmsToneCurve* gIdentityCurve = nullptr;
static const cmsToneCurve* gHalfCurve = nullptr;

// Simple markers to obtain stable addresses for our mock curves
static int gIdentityMarker;
static int gHalfMarker;

// Mock implementation of cmsEvalToneCurveFloat.
// This overrides the real curve evaluation to enable deterministic testing.
extern "C" cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve* curve, cmsFloat32Number In) {
    // If the curve pointer matches our identity curve, return input unmodified.
    if (curve == gIdentityCurve) {
        return In;
    }
    // If the curve pointer matches our half-curve, return half the input.
    if (curve == gHalfCurve) {
        return In * 0.5f;
    }
    // Default behavior (safe fallback): identity
    return In;
}

// Lightweight test harness (non-terminating, prints failures but continues)
static int gTestsRun = 0;
static int gFailures = 0;

#define EXPECT(cond, msg) do {                                   \
    ++gTestsRun;                                                    \
    if (!(cond)) {                                                  \
        ++gFailures;                                               \
        std::cerr << "EXPECT FAILED: " << (msg)                    \
                  << " in " << __FILE__ << ":" << __LINE__ << "\n"; \
    }                                                             \
} while(0)

static inline bool almost_equal(float a, float b, float tol = 1e-5f) {
    return std::fabs(a - b) <= tol;
}

// Helper: create a stage with given _cmsStageToneCurvesData-like layout
// We re-create a minimal _cmsStageToneCurvesData-compatible struct here
struct _cmsStageToneCurvesData {
    cmsUInt32Number nCurves;
    void** TheCurves;
};

// Test 1: Data == NULL should cause early return and leave Out untouched
void test_EvaluateCurves_DataNull() {
    // Prepare stage pointing to NULL Data (simulate early exit)
    cmsStage stage;
    stage.Data = nullptr;

    cmsFloat32Number In[3] = {0.1f, 0.2f, 0.3f};
    cmsFloat32Number Out[3] = { 7.0f, -1.0f, 0.0f }; // initial values to detect changes

    EvaluateCurves(In, Out, &stage);

    // Out must be unchanged
    EXPECT(almost_equal(Out[0], 7.0f), "Out[0] unchanged when Data is NULL");
    EXPECT(almost_equal(Out[1], -1.0f), "Out[1] unchanged when Data is NULL");
    EXPECT(almost_equal(Out[2], 0.0f), "Out[2] unchanged when Data is NULL");
}

// Test 2: Data non-NULL but Data->TheCurves is NULL should cause early return
void test_EvaluateCurves_DataTheCurvesNull() {
    cmsStage stage;

    // Build Data with TheCurves = NULL
    _cmsStageToneCurvesData data;
    data.nCurves = 2;
    data.TheCurves = nullptr;

    stage.Data = &data;

    cmsFloat32Number In[2] = {0.25f, 0.75f};
    cmsFloat32Number Out[2] = {0.0f, 0.0f}; // initial

    EvaluateCurves(In, Out, &stage);

    // Out must be unchanged (still zeros)
    EXPECT(almost_equal(Out[0], 0.0f), "Out[0] unchanged when TheCurves is NULL");
    EXPECT(almost_equal(Out[1], 0.0f), "Out[1] unchanged when TheCurves is NULL");
}

// Test 3: Data with two curves: Identity and Half (1st curve identity, 2nd curve halves input)
void test_EvaluateCurves_IdentityAndHalf() {
    // Prepare two curves: one identity, one halving
    // Point our mock curves to stable addresses using markers
    // (we'll reuse gIdentityMarker and gHalfMarker as stable addresses)
    gIdentityCurve = (const cmsToneCurve*) &gIdentityMarker;
    gHalfCurve     = (const cmsToneCurve*) &gHalfMarker;

    // Allocate curves array
    void** curves = new void*[2];
    curves[0] = (void*) gIdentityCurve;
    curves[1] = (void*) gHalfCurve;

    // Prepare _cmsStageToneCurvesData-like structure
    _cmsStageToneCurvesData data;
    data.nCurves = 2;
    data.TheCurves = curves;

    cmsStage stage;
    stage.Data = &data;

    // Input/output
    cmsFloat32Number In[2] = {0.2f, 0.8f};
    cmsFloat32Number Out[2] = {0.0f, 0.0f};

    EvaluateCurves(In, Out, &stage);

    // Expected: Out[0] = 0.2 (identity), Out[1] = 0.4 (half)
    EXPECT(almost_equal(Out[0], 0.2f), "Out[0] should equal input (identity curve)");
    EXPECT(almost_equal(Out[1], 0.4f), "Out[1] should equal input * 0.5 (half curve)");

    delete[] curves;
}

// Test 4: Single curve (Identity) should produce identical output
void test_EvaluateCurves_SingleIdentityCurve() {
    gIdentityCurve = (const cmsToneCurve*) &gIdentityMarker;
    gHalfCurve = nullptr; // not used in this test

    void** curves = new void*[1];
    curves[0] = (void*) gIdentityCurve;

    _cmsStageToneCurvesData data;
    data.nCurves = 1;
    data.TheCurves = curves;

    cmsStage stage;
    stage.Data = &data;

    cmsFloat32Number In[1] = {0.66f};
    cmsFloat32Number Out[1] = {0.0f};

    EvaluateCurves(In, Out, &stage);

    // Expected: Out[0] == In[0]
    EXPECT(almost_equal(Out[0], 0.66f), "Out[0] should equal input for single identity curve");

    delete[] curves;
}

// Main: runs all tests and prints a short summary
int main() {
    // Run tests
    test_EvaluateCurves_DataNull();
    test_EvaluateCurves_DataTheCurvesNull();
    test_EvaluateCurves_IdentityAndHalf();
    test_EvaluateCurves_SingleIdentityCurve();

    // Summary
    std::cout << "Tests run: " << gTestsRun << ", Failures: " << gFailures << std::endl;
    return (gFailures == 0) ? 0 : 1;
}