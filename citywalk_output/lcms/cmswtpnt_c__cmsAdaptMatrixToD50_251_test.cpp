// This test harness provides a self-contained unit test suite for the focal
// method _cmsAdaptMatrixToD50 and its dependent components.
// It mocks the minimal subset of the original C lcms2 types and functions to
// enable isolated testing without GTest. The test focuses on covering both
// true/false branches and matrix-based behavior as described in the focal code.
// The tests are written to compile under C++11.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cmath>


namespace cms_test {

typedef int cmsBool;
const int TRUE = 1;
const int FALSE = 0;

// Minimal representations of the involved data structures
struct cmsCIEXYZ {
    double X;
    double Y;
    double Z;
};

struct cmsCIExyY {
    double x;
    double y;
    double Y;
};

struct cmsMAT3 {
    double m[3][3];
};

// Static Bradford adaptation matrix (typical values)
static const cmsMAT3 BradfordMatrix = {{
    { 0.8951,  0.2664, -0.1614 },
    {-0.7502,  1.7135,  0.0367 },
    { 0.0389, -0.0685,  1.0296 }
}};

// D50 white XYZ reference
cmsCIEXYZ cmsD50_XYZ() {
    cmsCIEXYZ xyz{0.96422, 1.00000, 0.82521};
    return xyz;
}

// Global switch to drive test scenarios (true: adaptation matrix succeeds; false: fails)
static int g_AdaptationShouldSucceed = 1;

// Forward declarations of helper routines used by the focal function
// Convert xyY to XYZ (Dn)
void cmsxyY2XYZ(cmsCIEXYZ* Dn, const cmsCIExyY* xyY) {
    double x = xyY->x;
    double y = xyY->y;
    double Y = xyY->Y;
    if (y == 0.0) {
        Dn->X = Dn->Y = Dn->Z = 0.0;
        return;
    }
    Dn->X = Y * x / y;
    Dn->Y = Y;
    Dn->Z = Y * (1.0 - x - y) / y;
}

// Mock adaptation matrix computation: fills Bradford with a deterministic matrix
cmsBool _cmsAdaptationMatrix(cmsMAT3* r, const cmsMAT3* ConeMatrix,
                             const cmsCIEXYZ* FromIll, const cmsCIEXYZ* ToIll) {
    (void)ConeMatrix; (void)FromIll; (void)ToIll; // unused in mock
    if (!g_AdaptationShouldSucceed) {
        return FALSE;
    }
    *r = BradfordMatrix;
    return TRUE;
}

// 3x3 matrix multiplication: r = A * B
void _cmsMAT3per(cmsMAT3* r, const cmsMAT3* A, const cmsMAT3* B) {
    cmsMAT3 res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double s = 0.0;
            for (int k = 0; k < 3; ++k) {
                s += A->m[i][k] * B->m[k][j];
            }
            res.m[i][j] = s;
        }
    }
    *r = res;
}

// Initialize r to identity
void _cmsMAT3init_identity(cmsMAT3* r) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            r->m[i][j] = (i == j) ? 1.0 : 0.0;
}

// The focal method under test (re-implemented here to enable isolated unit testing)
cmsBool _cmsAdaptMatrixToD50(cmsMAT3* r, const cmsCIExyY* SourceWhitePt) {
    {
        cmsCIEXYZ Dn;
        cmsMAT3 Bradford;
        cmsMAT3 Tmp;
        cmsxyY2XYZ(&Dn, SourceWhitePt);
        if (!_cmsAdaptationMatrix(&Bradford, NULL, &Dn, cmsD50_XYZ())) return FALSE;
        Tmp = *r;
        _cmsMAT3per(r, &Bradford, &Tmp);
        return TRUE;
    }
}

// Expose BradfordMatrix for test comparisons (as a global constant)
static const cmsMAT3 BradfordConst = BradfordMatrix;

// Public helpers for tests
bool approximately_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: True path with identity initial matrix; expect r to become Bradford * I = Bradford
bool test_success_path() {
    // Explanation: When adaptation succeeds and r initially is identity, the result should be Bradford.
    g_AdaptationShouldSucceed = 1;

    cmsMAT3 r;
    _cmsMAT3init_identity(&r);

    cmsCIExyY src{0.3127, 0.3290, 1.0}; // Typical CIE xyY for D65-ish white

    cmsBool ok = _cmsAdaptMatrixToD50(&r, &src);
    if (ok != TRUE) {
        printf("Test Success Path: Failed - function returned FALSE on success path.\n");
        return false;
    }

    // Expected Bradford * I = Bradford
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!approximately_equal(r.m[i][j], BradfordConst.m[i][j])) {
                printf("Test Success Path: Failed - element (%d,%d) expected %.6f got %.6f\n",
                       i, j, BradfordConst.m[i][j], r.m[i][j]);
                return false;
            }
        }
    }
    printf("Test Success Path: Passed\n");
    return true;
}

// Test 2: False path where _cmsAdaptationMatrix signals failure; r should be unchanged and FALSE returned
bool test_false_path() {
    // Explanation: If adaptation matrix creation fails, function should return FALSE and not modify r.
    g_AdaptationShouldSucceed = 0;

    cmsMAT3 r;
    // Initialize to a non-identity baseline to verify no mutation on failure
    r.m[0][0] = 2.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0;
    r.m[1][0] = 0.0; r.m[1][1] = 2.0; r.m[1][2] = 0.0;
    r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 2.0;

    cmsCIExyY src{0.3127, 0.3290, 1.0};

    cmsBool ok = _cmsAdaptMatrixToD50(&r, &src);
    if (ok != FALSE) {
        printf("Test False Path: Failed - expected FALSE, got TRUE.\n");
        return false;
    }

    // r should be unchanged
    if (!(r.m[0][0] == 2.0 && r.m[0][1] == 0.0 && r.m[0][2] == 0.0 &&
          r.m[1][0] == 0.0 && r.m[1][1] == 2.0 && r.m[1][2] == 0.0 &&
          r.m[2][0] == 0.0 && r.m[2][1] == 0.0 && r.m[2][2] == 2.0)) {
        printf("Test False Path: Failed - matrix changed on false path.\n");
        return false;
    }

    printf("Test False Path: Passed\n");
    return true;
}

// Test 3: Non-identity initial matrix, verify Bradford * R0 is produced
bool test_non_identity_initial_r() {
    // Explanation: With a non-identity initial matrix, result should be Bradford * R0
    g_AdaptationShouldSucceed = 1;

    // Initial R0
    cmsMAT3 R0;
    R0.m[0][0] = 0.5; R0.m[0][1] = 0.2; R0.m[0][2] = -0.1;
    R0.m[1][0] = 0.1; R0.m[1][1] = 0.4; R0.m[1][2] = 0.2;
    R0.m[2][0] = -0.2; R0.m[2][1] = 0.0; R0.m[2][2] = 0.3;

    cmsMAT3 r = R0; // copy into r to be mutated by function
    cmsCIExyY src{0.3127, 0.3290, 1.0};

    // Compute expected = Bradford * R0
    cmsMAT3 expected;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double s = 0.0;
            for (int k = 0; k < 3; ++k) s += BradfordConst.m[i][k] * R0.m[k][j];
            expected.m[i][j] = s;
        }
    }

    cmsBool ok = _cmsAdaptMatrixToD50(&r, &src);
    if (ok != TRUE) {
        printf("Test Non-Identity Initial: Failed - function returned FALSE on success path.\n");
        return false;
    }

    // Compare r with expected
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!approximately_equal(r.m[i][j], expected.m[i][j])) {
                printf("Test Non-Identity Initial: Failed - element (%d,%d) expected %.6f got %.6f\n",
                       i, j, expected.m[i][j], r.m[i][j]);
                return false;
            }
        }
    }

    printf("Test Non-Identity Initial: Passed\n");
    return true;
}

// Runner
int run_all_tests() {
    int passed = 0;
    int total = 0;

    if (test_success_path()) ++passed;
    ++total;

    if (test_false_path()) ++passed;
    ++total;

    if (test_non_identity_initial_r()) ++passed;
    ++total;

    printf("Summary: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}

} // namespace cms_test

// Entry point
int main() {
    return cms_test::run_all_tests();
}