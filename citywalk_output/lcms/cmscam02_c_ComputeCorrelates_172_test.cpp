// High-quality C++11 unit tests for the focal method: ComputeCorrelates
// This test suite is designed to run without Google Test (GTest).
// It uses a lightweight, self-contained test harness with non-terminating
// assertions (EXPECT_* style) implemented as macros and helper functions.
// The test focuses on the CAM02COLOR ComputeCorrelates(...) function from cmscam02.c.

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cmath>


// Lightweight, self-contained test harness (no GTest).
// Expectation helpers
#define TEST_ASSERT(condition, msg) do { \
    if (!(condition)) { \
        fprintf(stderr, "TEST FAILED: %s\n", msg); \
        exit(1); \
    } \
} while(0)

static inline bool almost_equal(double a, double b, double tol) {
    return std::fabs(a - b) <= tol;
}

#define EXPECT_NEAR(actual, expected, tol, msg) do { \
    if (!almost_equal((actual), (expected), (tol))) { \
        fprintf(stderr, "EXPECT_NEAR FAILED: %s | actual: % .10f, expected: % .10f, tol: % .10f\n", (msg), (actual), (expected), (tol)); \
        fail_count++; \
    } \
} while(0)

#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        fprintf(stderr, "EXPECT_EQ FAILED: %s | actual: %ld, expected: %ld\n", (msg), (long)(actual), (long)(expected)); \
        fail_count++; \
    } \
} while(0)

// Forward declarations matching the external CMS function interfaces.
// We declare the types with layout consistent with the library in cmscam02.c.
// This is required to craft CAM02COLOR and cmsCIECAM02 instances for tests.

extern "C" {

// Minimal type definitions mirroring the library's public layout used by ComputeCorrelates.
// These definitions are designed to be compatible with cmscam02.c in the test environment.

typedef double cmsFloat64Number;

// CAM02COLOR: contains color representations and correlates
typedef struct {
    double XYZ[3];
    double RGBpa[3];
    double A;
    double h;
    double H;
    double J;
    double Q;
    double C;
    double M;
    double s;
} CAM02COLOR;

// cmsCIECAM02: model with required fields used by ComputeCorrelates
typedef struct {
    CAM02COLOR adoptedWhite;
    double Nc;
    double Ncb;
    double Nbb;
    double FL;
    double c;
    double z;
    double n;
    double D;
    int ContextID;
} cmsCIECAM02;

// Focal function under test
CAM02COLOR ComputeCorrelates(CAM02COLOR clr, cmsCIECAM02* pMod);

} // extern "C"

// Helper to compute expected H from a given h (replicates the piecewise mapping in the source)
static double computeExpectedHFromH(double h) {
    const double r2d = 180.0 / 3.141592654;
    double temp;
    if (h < 20.14) {
        // temp = ((h + 122.47)/1.2) + ((20.14 - h)/0.8);
        temp = ((h + 122.47) / 1.2) + ((20.14 - h) / 0.8);
        return 300.0 + (100.0 * ((h + 122.47) / 1.2)) / temp;
    } else if (h < 90.0) {
        // temp = ((h - 20.14)/0.8) + ((90.00 - h)/0.7);
        temp = ((h - 20.14) / 0.8) + ((90.00 - h) / 0.7);
        return (100.0 * ((h - 20.14) / 0.8)) / temp;
    } else if (h < 164.25) {
        // temp = ((h - 90.00)/0.7) + ((164.25 - h)/1.0);
        temp = ((h - 90.00) / 0.7) + ((164.25 - h) / 1.0);
        return 100.0 + ((100.0 * ((h - 90.00) / 0.7)) / temp);
    } else if (h < 237.53) {
        // temp = ((h - 164.25)/1.0) + ((237.53 - h)/1.2);
        temp = ((h - 164.25) / 1.0) + ((237.53 - h) / 1.2);
        return 200.0 + ((100.0 * ((h - 164.25) / 1.0)) / temp);
    } else {
        // temp = ((h - 237.53)/1.2) + ((360 - h + 20.14)/0.8);
        temp = ((h - 237.53) / 1.2) + ((360.0 - h + 20.14) / 0.8);
        return 300.0 + ((100.0 * ((h - 237.53) / 1.2)) / temp);
    }
}

// Global failure counter for the test harness
static int fail_count = 0;

// Test Case 1: a == 0, b == 0 -> h == 0, J == 100, and C == M == s == 0
void test_computeCorrelates_caseA() {
    // Initialize input color such that a == 0 and b == 0
    CAM02COLOR clr;
    std::memset(&clr, 0, sizeof(CAM02COLOR));
    clr.RGBpa[0] = 10.0;
    clr.RGBpa[1] = 10.0;
    clr.RGBpa[2] = 10.0;
    clr.A = 100.0; // choose A equal to adoptedWhite.A to get J == 100
    // Model parameters (pMod)
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(cmsCIECAM02));
    mod.Nc = 1.0;
    mod.Ncb = 1.0;
    mod.adoptedWhite.A = 100.0;
    mod.FL = 1.0;
    mod.c = 0.69;
    mod.z = 1.0;
    mod.n = 1.0;
    // Call ComputeCorrelates
    CAM02COLOR out = ComputeCorrelates(clr, &mod);
    // Expected h and other values
    double expected_h = 0.0;
    double expected_H = computeExpectedHFromH(expected_h); // using the same logic as in code
    double tol = 1e-6;

    TEST_ASSERT(almost_equal(out.h, expected_h, tol), "Case A: h should be 0 when a == 0 and b == 0");
    EXPECT_NEAR(out.H, expected_H, 1e-3, "Case A: H computed from h=0");
    EXPECT_NEAR(out.J, 100.0, 1e-9, "Case A: J should be exactly 100 when A matches adoptedWhite.A");
    EXPECT_NEAR(out.C, 0.0, 1e-9, "Case A: C should be 0 when a==0 and b==0");
    EXPECT_NEAR(out.M, 0.0, 1e-9, "Case A: M should be 0 when C==0");
    EXPECT_NEAR(out.s, 0.0, 1e-9, "Case A: s should be 0 when M==0");
    printf("Case A passed\n");
}

// Test Case 2: a == 0, b > 0 -> h == 90
void test_computeCorrelates_caseB() {
    // Set RGBpa to satisfy a == 0 and b > 0
    // Solve: a = x - (12/11)*y + z/11 = 0 and b = (x + y - 2z)/9 > 0
    // Use x=2, y=1, z = 12*y - 11*x = 12 - 22 = -10 -> a = 0, b = (2+1 - 2*(-10))/9 = 23/9 > 0
    CAM02COLOR clr;
    std::memset(&clr, 0, sizeof(CAM02COLOR));
    clr.RGBpa[0] = 2.0;
    clr.RGBpa[1] = 1.0;
    clr.RGBpa[2] = -10.0;
    clr.A = 100.0; // J should be 100
    // pMod
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(cmsCIECAM02));
    mod.Nc = 1.0;
    mod.Ncb = 1.0;
    mod.adoptedWhite.A = 100.0;
    mod.FL = 1.0;
    mod.c = 0.69;
    mod.z = 1.0;
    mod.n = 1.0;

    CAM02COLOR out = ComputeCorrelates(clr, &mod);
    double expected_h = 90.0; // by construction
    double tol = 1e-6;

    // Validate h and J
    EXPECT_NEAR(out.h, expected_h, tol, "Case B: h should be 90 when a==0 and b>0");
    EXPECT_NEAR(out.J, 100.0, 1e-9, "Case B: J should be exactly 100 when A matches adoptedWhite.A");
    // Basic sanity on C/M/S non-negative and non-zero due to t > 0
    TEST_ASSERT(out.C > 0.0, "Case B: C should be > 0 for a==0, b>0");
    TEST_ASSERT(out.M > 0.0, "Case B: M should be > 0 if C > 0");
    TEST_ASSERT(out.s >= 0.0, "Case B: s should be non-negative");
    printf("Case B passed\n");
}

// Test Case 3: a < 0 (else branch) -> h computed with +180 offset
void test_computeCorrelates_caseC() {
    // Choose RGBpa to have a < 0:
    // x=5, y=9, z=0 -> a = 5 - (12/11)*9 + 0/11 = 5 - 9.818... ≈ -4.818 (<0)
    // b = (5+9 - 0)/9 = 14/9 ≈ 1.555
    CAM02COLOR clr;
    std::memset(&clr, 0, sizeof(CAM02COLOR));
    clr.RGBpa[0] = 5.0;
    clr.RGBpa[1] = 9.0;
    clr.RGBpa[2] = 0.0;
    clr.A = 100.0;
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(cmsCIECAM02));
    mod.Nc = 1.0;
    mod.Ncb = 1.0;
    mod.adoptedWhite.A = 100.0;
    mod.FL = 1.0;
    mod.c = 0.69;
    mod.z = 1.0;
    mod.n = 1.0;

    CAM02COLOR out = ComputeCorrelates(clr, &mod);

    // Compute expected h using the same branch formula as in code
    double a = clr.RGBpa[0] - (12.0 * clr.RGBpa[1] / 11.0) + (clr.RGBpa[2] / 11.0);
    double b = (clr.RGBpa[0] + clr.RGBpa[1] - (2.0 * clr.RGBpa[2])) / 9.0;
    double r2d = 180.0 / 3.141592654;
    double expected_h;
    if (a == 0) {
        if (b == 0) expected_h = 0;
        else if (b > 0) expected_h = 90;
        else expected_h = 270;
    } else if (a > 0) {
        double temp = b / a;
        if (b > 0) expected_h = r2d * atan(temp);
        else if (b == 0) expected_h = 0;
        else expected_h = (r2d * atan(temp)) + 360;
    } else {
        double temp = b / a;
        expected_h = (r2d * atan(temp)) + 180;
    }

    double tol = 1e-6;
    EXPECT_NEAR(out.h, expected_h, tol, "Case C: h should match computed value for a<0 branch");
    EXPECT_NEAR(out.J, 100.0, 1e-9, "Case C: J should be exactly 100 when A matches adoptedWhite.A");
    printf("Case C passed\n");
}

int main() {
    // Run tests in a sequence to cover distinct branches
    test_computeCorrelates_caseA();
    test_computeCorrelates_caseB();
    test_computeCorrelates_caseC();

    if (fail_count == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILED TESTS: %d\n", fail_count);
        return 2;
    }
}