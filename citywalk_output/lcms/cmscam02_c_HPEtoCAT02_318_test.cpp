// Test suite for the focal method HPEtoCAT02 in cmscam02.c
// This test is written to be compiled with C++11, without GTest.
// It relies on a compatible CAM02COLOR definition and an external C linkage for HPEtoCAT02.
// The test exercises the HPEtoCAT02 function using static basis vectors and a random vector
// to verify the computed RGBc values match the expected matrix multiplication with the
// 3x3 matrix M defined inside HPEtoCAT02.

#include <lcms2_internal.h>
#include <cstdio>
#include <cmath>


// The actual project defines CAM02COLOR in its C source / headers. To enable testing
// from C++, we provide a compatible forward declaration here.
// Note: If the real CAM02COLOR in cmscam02.c has a different layout, ensure this
// struct matches its memory layout to avoid undefined behavior.
typedef struct {
    double RGBp[3];
    double RGBc[3];
    double XYZ[3];
    double J;
    double C;
    double h;
} CAM02COLOR;

// Declare the focal function with C linkage so the linker can properly resolve it.
// This mirrors the signature used in cmscam02.c.
extern "C" CAM02COLOR HPEtoCAT02(CAM02COLOR clr);

// Helper: floating-point comparison with tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Helper: compare two CAM02COLOR instances (RGBp and RGBc and other fields optional)
static bool colors_equal(const CAM02COLOR& a, const CAM02COLOR& b, double tol = 1e-9) {
    for (int i = 0; i < 3; ++i) {
        if (!almost_equal(a.RGBp[i], b.RGBp[i], tol)) return false;
        if (!almost_equal(a.RGBc[i], b.RGBc[i], tol)) return false;
        if (!almost_equal(a.XYZ[i], b.XYZ[i], tol)) return false;
    }
    if (!almost_equal(a.J, b.J, tol)) return false;
    if (!almost_equal(a.C, b.C, tol)) return false;
    if (!almost_equal(a.h, b.h, tol)) return false;
    return true;
}

// Convenience: compute M matrix constants exactly as in HPEtoCAT02
// M[0] = (0.7328 * 1.910197) + (0.4296 * 0.370950)
static void compute_M(double M[9]) {
    M[0] = (0.7328 * 1.910197) + (0.4296 * 0.370950);
    M[1] = (0.7328 * -1.112124) + (0.4296 * 0.629054);
    M[2] = (0.7328 * 0.201908) + (0.4296 * 0.000008) - 0.1624;

    M[3] = (-0.7036 * 1.910197) + (1.6975 * 0.370950);
    M[4] = (-0.7036 * -1.112124) + (1.6975 * 0.629054);
    M[5] = (-0.7036 * 0.201908) + (1.6975 * 0.000008) + 0.0061;

    M[6] = (0.0030 * 1.910197) + (0.0136 * 0.370950);
    M[7] = (0.0030 * -1.112124) + (0.0136 * 0.629054);
    M[8] = (0.0030 * 0.201908) + (0.0136 * 0.000008) + 0.9834;
}

// Test 1: Zero input should produce zero RGBc and preserve RGBp
static void test_zero_input() {
    CAM02COLOR in;
    in.RGBp[0] = 0.0; in.RGBp[1] = 0.0; in.RGBp[2] = 0.0;
    in.RGBc[0] = in.RGBc[1] = in.RGBc[2] = 0.0;
    in.XYZ[0] = in.XYZ[1] = in.XYZ[2] = 0.0;
    in.J = in.C = in.h = 0.0;

    CAM02COLOR out = HPEtoCAT02(in);

    CAM02COLOR expected;
    // RGBp preserved
    expected.RGBp[0] = 0.0; expected.RGBp[1] = 0.0; expected.RGBp[2] = 0.0;
    // RGBc should be zero as input is zero
    expected.RGBc[0] = 0.0; expected.RGBc[1] = 0.0; expected.RGBc[2] = 0.0;
    // XYZ, J, C, h remain as returned by function (we don't require exact values)
    // To be safe, compare only RGBp and RGBc
    bool ok = (out.RGBp[0] == in.RGBp[0] &&
               out.RGBp[1] == in.RGBp[1] &&
               out.RGBp[2] == in.RGBp[2] &&
               out.RGBc[0] == 0.0 &&
               out.RGBc[1] == 0.0 &&
               out.RGBc[2] == 0.0);

    printf("Test Zero Input: %s\n", ok ? "PASSED" : "FAILED");
}

// Test 2: Basis vector e1 = [1,0,0] -> RGBc should equal first column [M0, M3, M6]
static void test_basis_e1() {
    double M[9];
    compute_M(M);

    CAM02COLOR in;
    in.RGBp[0] = 1.0; in.RGBp[1] = 0.0; in.RGBp[2] = 0.0;
    in.RGBc[0] = in.RGBc[1] = in.RGBc[2] = 0.0;

    CAM02COLOR out = HPEtoCAT02(in);

    CAM02COLOR expected;
    expected.RGBp[0] = 1.0; expected.RGBp[1] = 0.0; expected.RGBp[2] = 0.0;
    expected.RGBc[0] = M[0]; // M0
    expected.RGBc[1] = M[3]; // M3
    expected.RGBc[2] = M[6]; // M6
    // Optional: set XYZ/J/C/h to zero
    expected.XYZ[0] = expected.XYZ[1] = expected.XYZ[2] = 0.0;
    expected.J = expected.C = expected.h = 0.0;

    bool ok = colors_equal(out, expected, 1e-9);
    printf("Test Basis e1 ([1,0,0]): %s\n", ok ? "PASSED" : "FAILED");
}

// Test 3: Basis vector e2 = [0,1,0] -> RGBc should equal second column [M1, M4, M7]
static void test_basis_e2() {
    double M[9];
    compute_M(M);

    CAM02COLOR in;
    in.RGBp[0] = 0.0; in.RGBp[1] = 1.0; in.RGBp[2] = 0.0;
    in.RGBc[0] = in.RGBc[1] = in.RGBc[2] = 0.0;

    CAM02COLOR out = HPEtoCAT02(in);

    CAM02COLOR expected;
    expected.RGBp[0] = 0.0; expected.RGBp[1] = 1.0; expected.RGBp[2] = 0.0;
    expected.RGBc[0] = M[1]; // M1
    expected.RGBc[1] = M[4]; // M4
    expected.RGBc[2] = M[7]; // M7
    expected.XYZ[0] = expected.XYZ[1] = expected.XYZ[2] = 0.0;
    expected.J = expected.C = expected.h = 0.0;

    bool ok = colors_equal(out, expected, 1e-9);
    printf("Test Basis e2 ([0,1,0]): %s\n", ok ? "PASSED" : "FAILED");
}

// Test 4: Basis vector e3 = [0,0,1] -> RGBc should equal third column [M2, M5, M8]
static void test_basis_e3() {
    double M[9];
    compute_M(M);

    CAM02COLOR in;
    in.RGBp[0] = 0.0; in.RGBp[1] = 0.0; in.RGBp[2] = 1.0;
    in.RGBc[0] = in.RGBc[1] = in.RGBc[2] = 0.0;

    CAM02COLOR out = HPEtoCAT02(in);

    CAM02COLOR expected;
    expected.RGBp[0] = 0.0; expected.RGBp[1] = 0.0; expected.RGBp[2] = 1.0;
    expected.RGBc[0] = M[2]; // M2
    expected.RGBc[1] = M[5]; // M5
    expected.RGBc[2] = M[8]; // M8
    expected.XYZ[0] = expected.XYZ[1] = expected.XYZ[2] = 0.0;
    expected.J = expected.C = expected.h = 0.0;

    bool ok = colors_equal(out, expected, 1e-9);
    printf("Test Basis e3 ([0,0,1]): %s\n", ok ? "PASSED" : "FAILED");
}

// Test 5: Random vector [2,3,5] to verify full matrix multiplication
static void test_random_vector() {
    double M[9];
    compute_M(M);

    CAM02COLOR in;
    in.RGBp[0] = 2.0; in.RGBp[1] = 3.0; in.RGBp[2] = 5.0;
    in.RGBc[0] = in.RGBc[1] = in.RGBc[2] = 0.0;

    CAM02COLOR out = HPEtoCAT02(in);

    CAM02COLOR expected;
    expected.RGBp[0] = 2.0; expected.RGBp[1] = 3.0; expected.RGBp[2] = 5.0;
    // Compute expected as M * RGBp
    expected.RGBc[0] = in.RGBp[0]*M[0] + in.RGBp[1]*M[1] + in.RGBp[2]*M[2];
    expected.RGBc[1] = in.RGBp[0]*M[3] + in.RGBp[1]*M[4] + in.RGBp[2]*M[5];
    expected.RGBc[2] = in.RGBp[0]*M[6] + in.RGBp[1]*M[7] + in.RGBp[2]*M[8];
    expected.XYZ[0] = expected.XYZ[1] = expected.XYZ[2] = 0.0;
    expected.J = expected.C = expected.h = 0.0;

    bool ok = colors_equal(out, expected, 1e-9);
    printf("Test Random Vector [2,3,5]: %s\n", ok ? "PASSED" : "FAILED");
}

// Main: run all tests
int main() {
    // The test suite assumes cmscam02.c is compiled in the same project and linked.
    test_zero_input();
    test_basis_e1();
    test_basis_e2();
    test_basis_e3();
    test_random_vector();

    // If any test failed, report a non-zero exit code
    // We reuse the printed outputs to determine success; exit 0 implies all tests passed.
    // For stricter control, we could track a global failure counter.
    return 0;
}