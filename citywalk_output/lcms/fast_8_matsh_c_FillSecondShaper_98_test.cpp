/*
Unit test suite for FillSecondShaper (function defined in fast_8_matsh.c)

Step 1 - Program Understanding and Candidate Keywords
- Core loop indices: i in [0, 0x4000] with 0x4001 iterations
- Dependent elements: R (input domain), Val (curve output), w (intermediate result), Table (output buffer)
- Curve interaction: Val = cmsEvalToneCurveFloat(Curve, R)
- Output processing: w = (Val * 255.0f) + 0.5f; clamp to [0, 255]; Table[i] = (cmsInt8Number) w
- Key dependencies (Candidates): R, Val, w, Curve, cmsEvalToneCurveFloat, Table, 0x4001 loop bound

Step 2 - Unit Test Generation (no GTest, standalone harness)
- We mock cmsEvalToneCurveFloat to drive Curve behavior without requiring a full external library.
- We provide a minimal cmsToneCurve type compatible with the existing API usage.
- We declare FillSecondShaper externally (as it would be in fast_8_matsh.c).
- We cover true/false branches of clamping (w < 0 and w > 255) by using different curve modes.

Step 3 - Test Case Refinement
- Test gamma-like curves to exercise typical path and boundary:
  - Kind 1: gamma-like curve Val = 0.8*R + 0.1
  - Kind 2: identity-like curve Val = clamp(R, 0, 1)
  - Kind 3: force negative Val to exercise w < 0 path (then clamp to 0)
  - Kind 4: force Val > 1 to exercise w > 255 path (then clamp to 255)
- Validate deterministic output at representative indices: i = 0, 8192, 16384
- Validate signed 8-bit interpretation of produced values (Table stores cmsInt8Number)

Note: This test suite assumes the ability to override cmsEvalToneCurveFloat via a test-scoped implementation. It also treats FillSecondShaper as an external C function. The test harness uses a minimal cmsToneCurve structure and opaque Curve fields since cmsEvalToneCurveFloat in tests does not dereference Curve.

Code below (single file, C++11, no GTest, includes its own light test runner and explanatory comments)
*/

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdint>


// Domain-specific typedefs (compatible with typical LittleCMS-style names)
typedef unsigned char cmsUInt8Number;
typedef signed char   cmsInt8Number;
typedef int             cmsInt32Number;
typedef float           cmsFloat32Number;
typedef void*           cmsContext;
typedef struct cmsToneCurve cmsToneCurve;

// Declaration of the function under test (C linkage)
extern "C" void FillSecondShaper(cmsUInt8Number* Table, cmsToneCurve* Curve);

// Global test hook to drive cmsEvalToneCurveFloat behavior
static int g_curve_kind = 0;

// Mock implementation of cmsEvalToneCurveFloat used by the focal function FillSecondShaper
// The actual implementation would come from the color-management library; here we simulate
// different curve behaviors to exercise the code paths in FillSecondShaper.
extern "C" cmsFloat32Number cmsEvalToneCurveFloat(cmsToneCurve* Curve, cmsFloat32Number Input)
{
    (void)Curve; // Curve layout is opaque in tests; we drive behavior via g_curve_kind

    switch (g_curve_kind)
    {
        case 1: // gamma-like: Val = 0.8*R + 0.1
            return Input * 0.8f + 0.1f;

        case 2: // identity-like with clamping to [0,1]
        {
            float v = Input;
            if (v < 0.0f) v = 0.0f;
            if (v > 1.0f) v = 1.0f;
            return v;
        }

        case 3: // force negative Val to exercise w < 0 path
            return -0.1f;

        case 4: // force Val > 1 to exercise w > 255 path
            return 2.0f;

        default: // identity
            return Input;
    }
}

// Minimal cmsToneCurve type (kept opaque for tests)
struct cmsToneCurve { int dummy; };

// Helper: compute expected signed 8-bit value for a given index i and curve kind.
// This replicates the logic inside FillSecondShaper for validation.
int compute_expected_signed(int i, int kind)
{
    const float INV_DEN = 16384.0f;
    float R = static_cast<float>(i) / INV_DEN; // i/16384.0f

    float Val;
    switch (kind)
    {
        case 1: Val = R * 0.8f + 0.1f; break;
        case 2: Val = R; break;
        case 3: Val = -0.1f; break;
        case 4: Val = 2.0f; break;
        default: Val = R; break;
    }

    int w = static_cast<int>(Val * 255.0f + 0.5f);
    if (w < 0) w = 0;
    if (w > 255) w = 255;

    // Cast to cmsInt8Number (signed 8-bit) as FillSecondShaper does
    cmsInt8Number signedW = static_cast<cmsInt8Number>(w);
    return static_cast<int>(signedW);
}

// Simple test assertion helper
static bool assert_equal(int expected, int actual, const char* msg)
{
    if (expected == actual) {
        std::cout << "[PASS] " << msg << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << msg << " | Expected: " << expected << " Actual: " << actual << "\n";
        return false;
    }
}

// Test 1: Gamma-like curve (kind=1) at representative indices
bool test_gamma_kind1_indices()
{
    g_curve_kind = 1;
    cmsToneCurve curve;
    unsigned char Table[0x4001] = {0}; // Output buffer (unsigned 8-bit storage)

    FillSecondShaper(Table, &curve);

    // Validate at i = 0, 8192, 16384
    int i0 = 0;
    int i1 = 8192;
    int i2 = 16384;

    int actual0 = static_cast<int>(static_cast<cmsInt8Number>(Table[i0]));
    int expected0 = compute_expected_signed(i0, 1);

    int actual1 = static_cast<int>(static_cast<cmsInt8Number>(Table[i1]));
    int expected1 = compute_expected_signed(i1, 1);

    int actual2 = static_cast<int>(static_cast<cmsInt8Number>(Table[i2]));
    int expected2 = compute_expected_signed(i2, 1);

    bool ok = true;
        ok &= assert_equal(expected0, actual0, "gamma kind1: i=0");
        ok &= assert_equal(expected1, actual1, "gamma kind1: i=8192");
        ok &= assert_equal(expected2, actual2, "gamma kind1: i=16384");

    return ok;
}

// Test 2: Identity-like curve with clamping (kind=2)
bool test_gamma_kind2_indices()
{
    g_curve_kind = 2;
    cmsToneCurve curve;
    unsigned char Table[0x4001] = {0};

    FillSecondShaper(Table, &curve);

    int i0 = 0;
    int i1 = 8192;
    int i2 = 16384;

    int actual0 = static_cast<int>(static_cast<cmsInt8Number>(Table[i0]));
    int expected0 = compute_expected_signed(i0, 2);

    int actual1 = static_cast<int>(static_cast<cmsInt8Number>(Table[i1]));
    int expected1 = compute_expected_signed(i1, 2);

    int actual2 = static_cast<int>(static_cast<cmsInt8Number>(Table[i2]));
    int expected2 = compute_expected_signed(i2, 2);

    bool ok = true;
        ok &= assert_equal(expected0, actual0, "gamma2: i=0");
        ok &= assert_equal(expected1, actual1, "gamma2: i=8192");
        ok &= assert_equal(expected2, actual2, "gamma2: i=16384");

    return ok;
}

// Test 3: Trigger w < 0 clamp path (kind=3)
bool test_gamma_kind3_negative_clamp()
{
    g_curve_kind = 3;
    cmsToneCurve curve;
    unsigned char Table[0x4001] = {0};

    FillSecondShaper(Table, &curve);

    int actual0 = static_cast<int>(static_cast<cmsInt8Number>(Table[0]));
    // Expected: w would be negative, then clamped to 0
    int expected0 = 0;

    bool ok = assert_equal(expected0, actual0, "gamma3: i=0 clamps to 0 due to negative Val");
    return ok;
}

// Test 4: Trigger w > 255 clamp path (kind=4)
bool test_gamma_kind4_overflow_clamp()
{
    g_curve_kind = 4;
    cmsToneCurve curve;
    unsigned char Table[0x4001] = {0};

    FillSecondShaper(Table, &curve);

    int actual0 = static_cast<int>(static_cast<cmsInt8Number>(Table[0]));
    // Val = 2.0 -> w = 510 -> clamp to 255 -> int8(255) == -1
    int expected0 = static_cast<int>(static_cast<cmsInt8Number>(255)); // -1 as signed char

    // To compare correctly, reinterpret as signed
    if (actual0 != expected0) {
        std::cout << "[INFO] gamma4: i=0 actual as signed = " << actual0 << ", expected signed = " << expected0 << "\n";
    }

    bool ok = assert_equal(expected0, actual0, "gamma4: i=0 clamps to -1 (255) in int8 space");
    return ok;
}

// Simple test runner
int main()
{
    std::cout << "Running FillSecondShaper unit tests (C++11, standalone harness)\n";

    int total = 0;
    int passes = 0;

    auto accumulate = [&](bool t) { total++; if (t) passes++; return t; };

    accumulate(test_gamma_kind1_indices());
    accumulate(test_gamma_kind2_indices());
    accumulate(test_gamma_kind3_negative_clamp());
    accumulate(test_gamma_kind4_overflow_clamp());

    std::cout << "Summary: " << passes << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed (for CI convenience)
    return (passes == total) ? 0 : 1;
}