// Test harness for the focal method CHAD2Temp (from cmscnvrt.c)
// This file provides a self-contained, mock environment to exercise the
// CHAD2Temp function without requiring the full Little CMS codebase.
// It implements just enough of the dependency surface to drive true/false
// branches and a deterministic output for verification.
//
// Notes:
// - We re-create the minimal data structures used by CHAD2Temp.
// - We provide mock implementations for _cmsMAT3inverse, _cmsMAT3eval,
//   cmsD50_XYZ, cmsXYZ2xyY, and cmsTempFromWhitePoint to control behavior.
// - We test three scenarios: inverse failure, white point temp-fail, and success.
// - All code is self-contained and compiles under C++11. No GTest is used.

#include <lcms2_internal.h>
#include <bits/stdc++.h>


// Basic typedefs and structures to mirror the focal function's usage.
typedef double cmsFloat64Number;
typedef int    cmsBool;
typedef unsigned int cmsUInt32Number;

#define FALSE 0

// Indices used by CHAD2Temp
#define VX 0
#define VY 1
#define VZ 2

// Mat and vector/quasi-structures
struct cmsMAT3 {
    double m[3][3];
};

struct cmsVEC3 {
    double n[3];
};

struct cmsCIEXYZ {
    double X;
    double Y;
    double Z;
};

struct cmsCIExyY {
    double x;
    double y;
    double Y; // luminance
};

// Forward declarations of the internal dependencies (to be provided below)
extern "C" cmsBool _cmsMAT3inverse(const cmsMAT3* src, cmsMAT3* dst);
extern "C" void      _cmsMAT3eval(cmsVEC3* out, const cmsMAT3* m, const cmsVEC3* v);
extern "C" const cmsCIEXYZ* cmsD50_XYZ();
extern "C" void      cmsXYZ2xyY(struct cmsCIExyY* xyY, const cmsCIEXYZ* xyz);
extern "C" cmsBool   cmsTempFromWhitePoint(cmsFloat64Number* TempK, const cmsCIExyY* xyY);

// The focal function to test (copied/adapted from the prompt)
cmsFloat64Number CHAD2Temp(const cmsMAT3* Chad)
{
{
    cmsVEC3 d, s;
    cmsCIEXYZ Dest;
    cmsCIExyY DestChromaticity;
    cmsFloat64Number TempK;
    cmsMAT3 m1, m2;
    m1 = *Chad;
    if (!_cmsMAT3inverse(&m1, &m2)) return FALSE;
    s.n[VX] = cmsD50_XYZ() -> X;
    s.n[VY] = cmsD50_XYZ() -> Y;
    s.n[VZ] = cmsD50_XYZ() -> Z;
    _cmsMAT3eval(&d, &m2, &s);
    Dest.X = d.n[VX];
    Dest.Y = d.n[VY];
    Dest.Z = d.n[VZ];
    cmsXYZ2xyY(&DestChromaticity, &Dest);
    if (!cmsTempFromWhitePoint(&TempK, &DestChromaticity))
        return -1.0;
    return TempK;
}
}

// ---------------- Mocked dependency implementations ----------------

static int g_forceInverseFailure = 0;
static int g_tempFromWhitePointFail = 0;
static cmsFloat64Number g_tempFromWhitePointValue = 0.0;

// Inverse of a 3x3 matrix. For test simplicity, we implement a robust solver
// for general matrices. If g_forceInverseFailure is set, we return 0 to
// simulate a singular/inverse failure.
extern "C" cmsBool _cmsMAT3inverse(const cmsMAT3* src, cmsMAT3* dst)
{
    if (g_forceInverseFailure) {
        return 0; // simulate failure
    }

    // Compute the inverse of a 3x3 matrix
    const double (*a)[3] = src->m;
    double det =
        a[0][0]*(a[1][1]*a[2][2] - a[1][2]*a[2][1]) -
        a[0][1]*(a[1][0]*a[2][2] - a[1][2]*a[2][0]) +
        a[0][2]*(a[1][0]*a[2][1] - a[1][1]*a[2][0]);

    if (det == 0.0) {
        return 0;
    }
    double invdet = 1.0 / det;

    // Cofactor matrix (transposed) for the adjugate
    dst->m[0][0] =  (a[1][1]*a[2][2] - a[1][2]*a[2][1]) * invdet;
    dst->m[0][1] = -(a[0][1]*a[2][2] - a[0][2]*a[2][1]) * invdet;
    dst->m[0][2] =  (a[0][1]*a[1][2] - a[0][2]*a[1][1]) * invdet;

    dst->m[1][0] = -(a[1][0]*a[2][2] - a[1][2]*a[2][0]) * invdet;
    dst->m[1][1] =  (a[0][0]*a[2][2] - a[0][2]*a[2][0]) * invdet;
    dst->m[1][2] = -(a[0][0]*a[1][2] - a[0][2]*a[1][0]) * invdet;

    dst->m[2][0] =  (a[1][0]*a[2][1] - a[1][1]*a[2][0]) * invdet;
    dst->m[2][1] = -(a[0][0]*a[2][1] - a[0][1]*a[2][0]) * invdet;
    dst->m[2][2] =  (a[0][0]*a[1][1] - a[0][1]*a[1][0]) * invdet;

    return 1;
}

// Multiply matrix m (3x3) by vector v (3x1) and store in out
extern "C" void _cmsMAT3eval(cmsVEC3* out, const cmsMAT3* m, const cmsVEC3* v)
{
    const double* x = v->n;
    for (int i = 0; i < 3; ++i) {
        out->n[i] = m->m[i][0]*x[0] + m->m[i][1]*x[1] + m->m[i][2]*x[2];
    }
}

// D50 XYZ reference
extern "C" const cmsCIEXYZ* cmsD50_XYZ()
{
    // Provide a pointer to a static XYZ with D50 characteristics
    static cmsCIEXYZ d50 = {0.9642, 1.0000, 0.8249};
    // The original API uses pointer semantics with "->" to extract X/Y/Z
    return &d50;
}

// Convert XYZ to xyY chromaticity
extern "C" void cmsXYZ2xyY(struct cmsCIExyY* xyY, const cmsCIEXYZ* xyz)
{
    double X = xyz->X, Y = xyz->Y, Z = xyz->Z;
    double denom = X + Y + Z;
    if (denom > 0.0) {
        xyY->x = X / denom;
        xyY->y = Y / denom;
    } else {
        xyY->x = 0.0;
        xyY->y = 0.0;
    }
    xyY->Y = Y;
}

// cmsTempFromWhitePoint: returns true and a fixed temperature unless flagged to fail
extern "C" cmsBool cmsTempFromWhitePoint(cmsFloat64Number* TempK, const cmsCIExyY* xyY)
{
    (void)xyY; // not used in mock
    if (g_tempFromWhitePointFail) {
        return 0;
    }
    *TempK = g_tempFromWhitePointValue;
    return 1;
}

// ----------------- Test harness -------------------------------------------------

// Utility to compare doubles with tolerance
static bool nearlyEqual(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// TRUE test: Inverse fails, CHAD2Temp should return 0 (FALSE macro)
bool test_inverse_failure()
{
    g_forceInverseFailure = 1;
    g_tempFromWhitePointFail = 0;
    g_tempFromWhitePointValue = 12345.0;

    cmsMAT3 Chad = { { {1.0,0.0,0.0},
                       {0.0,1.0,0.0},
                       {0.0,0.0,1.0} } };

    cmsFloat64Number res = CHAD2Temp(&Chad);
    bool pass = (res == static_cast<cmsFloat64Number>(FALSE));
    // Explanation: CHAD2Temp should early-exit due to inverse failure.
    if (pass) {
        std::cout << "[PASS] test_inverse_failure: CHAD2Temp() returned FALSE as expected.\n";
    } else {
        std::cout << "[FAIL] test_inverse_failure: CHAD2Temp() did not return FALSE. Got " << res << "\n";
    }
    // cleanup
    g_forceInverseFailure = 0;
    return pass;
}

// WPT failure: Inverse succeeds but cmsTempFromWhitePoint fails -> -1.0 path
bool test_whitepoint_failure()
{
    g_forceInverseFailure = 0;
    g_tempFromWhitePointFail = 1;
    g_tempFromWhitePointValue = 999.0; // Should be ignored due to failure

    cmsMAT3 Chad = { { {1.0,0.0,0.0},
                       {0.0,1.0,0.0},
                       {0.0,0.0,1.0} } };

    cmsFloat64Number res = CHAD2Temp(&Chad);
    bool pass = nearlyEqual(res, -1.0);
    if (pass) {
        std::cout << "[PASS] test_whitepoint_failure: CHAD2Temp() returned -1.0 when TempFromWhitePoint failed.\n";
    } else {
        std::cout << "[FAIL] test_whitepoint_failure: Expected -1.0, got " << res << "\n";
    }
    g_tempFromWhitePointFail = 0;
    return pass;
}

// Success path: inverse ok and cmsTempFromWhitePoint ok -> should return the set TempK value
bool test_success_path()
{
    g_forceInverseFailure = 0;
    g_tempFromWhitePointFail = 0;
    g_tempFromWhitePointValue = 77.5;

    cmsMAT3 Chad = { { {1.0,0.0,0.0},
                       {0.0,1.0,0.0},
                       {0.0,0.0,1.0} } };

    cmsFloat64Number res = CHAD2Temp(&Chad);
    bool pass = nearlyEqual(res, 77.5);
    if (pass) {
        std::cout << "[PASS] test_success_path: CHAD2Temp() returned the configured TempK value 77.5.\n";
    } else {
        std::cout << "[FAIL] test_success_path: Expected 77.5, got " << res << "\n";
    }
    return pass;
}

// Entry point to run all tests
int main() {
    bool all_passed = true;
    all_passed &= test_inverse_failure();
    all_passed &= test_whitepoint_failure();
    all_passed &= test_success_path();
    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}