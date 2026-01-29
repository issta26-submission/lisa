// Test suite for the focal method SetIdentity in cmstypes.c
// This test suite is designed for C++11 without using GTest.
// It focuses on the SetIdentity routine which fills a 3x4 matrix to an identity-like form.
// The tests cover zero-initialized, random-initialized, and already-identity inputs.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Step 1: Domain-Centric setup
// We declare the expected signature of the focal function for linkage against the C/C library.
// The actual library uses cmsFloat64Number (likely a typedef for double). We mirror this here
// to ensure compatibility with the linked object file cmstypes.c.
typedef double cmsFloat64Number;

// Declare the focal function with C linkage to avoid name mangling during linking.
extern "C" void SetIdentity(cmsFloat64Number XYZ2XYZmatrix[3][4]);

// Small helper for approximate floating-point comparisons
static const double EPS = 1e-12;
static inline bool approx_eq(double a, double b, double eps = EPS) {
    return std::fabs(a - b) <= eps;
}

// Helper to log per-test failures without terminating execution
static void CheckEq(double expected, double actual, const char* label, bool& failed) {
    if (!approx_eq(expected, actual)) {
        std::cerr << "Test failure: " << label
                  << " | expected: " << expected
                  << " , actual: " << actual << "\n";
        failed = true;
    }
}

// Candidate Keywords extracted from focal method understanding
// - XYZ2XYZmatrix[3][4]
// - Identity pattern: diag 1.0 and zeros elsewhere
// - Data types: cmsFloat64Number (double)
// - Operation: overwrite all elements to identity configuration
// - Safety: ensure full 3x4 matrix coverage (12 elements)


// Test 1: Ensure SetIdentity turns a zero-initialized 3x4 matrix into a proper identity matrix.
// Expected: [ [1,0,0,0], [0,1,0,0], [0,0,1,0] ]
void Test_SetIdentity_ZeroMatrix() {
    // Explanatory: Start with a matrix filled with zeros and verify full identity pattern after call.
    bool failed = false;
    cmsFloat64Number m[3][4] = { {0.0, 0.0, 0.0, 0.0},
                                {0.0, 0.0, 0.0, 0.0},
                                {0.0, 0.0, 0.0, 0.0} };

    SetIdentity(m);

    CheckEq(1.0, m[0][0], "m[0][0] == 1.0", failed);
    CheckEq(0.0, m[0][1], "m[0][1] == 0.0", failed);
    CheckEq(0.0, m[0][2], "m[0][2] == 0.0", failed);
    CheckEq(0.0, m[0][3], "m[0][3] == 0.0", failed);

    CheckEq(0.0, m[1][0], "m[1][0] == 0.0", failed);
    CheckEq(1.0, m[1][1], "m[1][1] == 1.0", failed);
    CheckEq(0.0, m[1][2], "m[1][2] == 0.0", failed);
    CheckEq(0.0, m[1][3], "m[1][3] == 0.0", failed);

    CheckEq(0.0, m[2][0], "m[2][0] == 0.0", failed);
    CheckEq(0.0, m[2][1], "m[2][1] == 0.0", failed);
    CheckEq(1.0, m[2][2], "m[2][2] == 1.0", failed);
    CheckEq(0.0, m[2][3], "m[2][3] == 0.0", failed);

    if (!failed) {
        std::cout << "[PASS] Test_SetIdentity_ZeroMatrix\n";
    } else {
        std::cout << "[FAIL] Test_SetIdentity_ZeroMatrix\n";
    }
}

// Test 2: Ensure SetIdentity overwrites arbitrary values to the identity pattern.
// We initialize with non-identity values and verify the resulting matrix is identity.
void Test_SetIdentity_RandomMatrix() {
    bool failed = false;
    cmsFloat64Number m[3][4] = {
        {2.5, -3.1, 7.0, 9.9},
        {-4.2, 5.5, 0.0, -1.1},
        {3.14, -2.71, 6.02, 123.0}
    };

    SetIdentity(m);

    CheckEq(1.0, m[0][0], "m[0][0] == 1.0", failed);
    CheckEq(0.0, m[0][1], "m[0][1] == 0.0", failed);
    CheckEq(0.0, m[0][2], "m[0][2] == 0.0", failed);
    CheckEq(0.0, m[0][3], "m[0][3] == 0.0", failed);

    CheckEq(0.0, m[1][0], "m[1][0] == 0.0", failed);
    CheckEq(1.0, m[1][1], "m[1][1] == 1.0", failed);
    CheckEq(0.0, m[1][2], "m[1][2] == 0.0", failed);
    CheckEq(0.0, m[1][3], "m[1][3] == 0.0", failed);

    CheckEq(0.0, m[2][0], "m[2][0] == 0.0", failed);
    CheckEq(0.0, m[2][1], "m[2][1] == 0.0", failed);
    CheckEq(1.0, m[2][2], "m[2][2] == 1.0", failed);
    CheckEq(0.0, m[2][3], "m[2][3] == 0.0", failed);

    if (!failed) {
        std::cout << "[PASS] Test_SetIdentity_RandomMatrix\n";
    } else {
        std::cout << "[FAIL] Test_SetIdentity_RandomMatrix\n";
    }
}

// Test 3: Ensure SetIdentity leaves a matrix that is already identity unchanged.
// This validates idempotence for the identity pattern.
void Test_SetIdentity_AlreadyIdentity() {
    bool failed = false;
    cmsFloat64Number m[3][4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0}
    };

    SetIdentity(m);

    CheckEq(1.0, m[0][0], "m[0][0] == 1.0 (already identity)", failed);
    CheckEq(0.0, m[0][1], "m[0][1] == 0.0 (already identity)", failed);
    CheckEq(0.0, m[0][2], "m[0][2] == 0.0 (already identity)", failed);
    CheckEq(0.0, m[0][3], "m[0][3] == 0.0 (already identity)", failed);

    CheckEq(0.0, m[1][0], "m[1][0] == 0.0 (already identity)", failed);
    CheckEq(1.0, m[1][1], "m[1][1] == 1.0 (already identity)", failed);
    CheckEq(0.0, m[1][2], "m[1][2] == 0.0 (already identity)", failed);
    CheckEq(0.0, m[1][3], "m[1][3] == 0.0 (already identity)", failed);

    CheckEq(0.0, m[2][0], "m[2][0] == 0.0 (already identity)", failed);
    CheckEq(0.0, m[2][1], "m[2][1] == 0.0 (already identity)", failed);
    CheckEq(1.0, m[2][2], "m[2][2] == 1.0 (already identity)", failed);
    CheckEq(0.0, m[2][3], "m[2][3] == 0.0 (already identity)", failed);

    if (!failed) {
        std::cout << "[PASS] Test_SetIdentity_AlreadyIdentity\n";
    } else {
        std::cout << "[FAIL] Test_SetIdentity_AlreadyIdentity\n";
    }
}

// Entry point to run all tests
int main() {
    // Step 2/3: Run the generated test suite
    Test_SetIdentity_ZeroMatrix();
    Test_SetIdentity_RandomMatrix();
    Test_SetIdentity_AlreadyIdentity();

    // Summary
    std::cout << "SetIdentity test suite completed.\n";
    return 0;
}