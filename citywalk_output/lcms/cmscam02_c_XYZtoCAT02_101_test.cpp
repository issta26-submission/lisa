/*
Unit Test Suite for CAM02COLOR XYZtoCAT02(CAM02COLOR clr)
Target: Validate the transformation XYZ -> CAT02 color space via the focal method
XYZtoCAT02 defined in cmscam02.c

Notes:
- This test uses a lightweight, non-terminating assertion framework (custom EXPECT_*)
  suitable for C++11, without GTest or Google Mock.
- We rely on the library being built alongside (lcms2 headers and cmscam02.c present).
- The tests cover:
  - Basis vectors (X, Y, Z) mapping to expected RGB values using the provided linear coefficients.
  - A few mixed inputs to confirm linearity and correctness.
  - Preservation of input XYZ values (the function does not modify input XYZ in-place).
  - Edge case with zero input vector.
- Static functions in cmscam02.c are not mocked or accessed; we only test the external XYZtoCAT02.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Candidate Keywords observed from the focal method and dependencies:
// - CAM02COLOR, XYZ, RGB, XYZtoCAT02, 1:1 mapping, linear transform

// Forward declaration of the focal function as a C-linkage for C++ tests.
// The actual type CAM02COLOR is defined in cmscam02.c / lcms2 headers.
extern "C" CAM02COLOR XYZtoCAT02(CAM02COLOR clr);

// Include library header that defines CAM02COLOR and related types if available.
// If the environment provides these definitions via cms headers, this include is correct.
// Otherwise, rely on the external linkage (the test must be linked with the library).

// Simple, non-terminating assertion framework
static int g_failures = 0;
static const double TOLERANCE = 1e-6;

#define EXPECT_NEAR(actual, expected, tol) do {                                     \
    double _a = static_cast<double>(actual);                                         \
    double _b = static_cast<double>(expected);                                       \
    if (std::fabs(_a - _b) > (tol)) {                                              \
        std::cerr << "FAILED: " << __FILE__ << ":" << __LINE__                 \
                  << " - Expected near: " << _b                                        \
                  << ", Actual: " << _a << ", Tolerance: " << (tol) << "\n";       \
        ++g_failures;                                                               \
    }                                                                               \
} while(0)

#define EXPECT_TRUE(condition) do {                                                    \
    if (!(condition)) {                                                                \
        std::cerr << "FAILED: " << __FILE__ << ":" << __LINE__                       \
                  << " - Condition is false: " #condition << "\n";                   \
        ++g_failures;                                                                   \
    }                                                                                    \
} while(0)

#define EXPECT_EQUAL_XYZ_INPUT preserved_xyz_check

// Test 1: Verify XYZ basis vectors map to expected RGB according to CAM02COLOR XYZtoCAT02
void test_XYZtoCAT02_basis_vectors() {
    CAM02COLOR clr;
    CAM02COLOR res;
    // Basis X: XYZ = (1,0,0)
    clr.XYZ[0] = 1.0; clr.XYZ[1] = 0.0; clr.XYZ[2] = 0.0;
    res = XYZtoCAT02(clr);
    EXPECT_NEAR(res.RGB[0], 0.7328, TOLERANCE);
    EXPECT_NEAR(res.RGB[1], -0.7036, TOLERANCE);
    EXPECT_NEAR(res.RGB[2], 0.0030, TOLERANCE);
    // XYZ should be preserved in the input (function returns by value)
    EXPECT_NEAR(res.XYZ[0], clr.XYZ[0], TOLERANCE);
    EXPECT_NEAR(res.XYZ[1], clr.XYZ[1], TOLERANCE);
    EXPECT_NEAR(res.XYZ[2], clr.XYZ[2], TOLERANCE);

    // Basis Y: XYZ = (0,1,0)
    clr.XYZ[0] = 0.0; clr.XYZ[1] = 1.0; clr.XYZ[2] = 0.0;
    res = XYZtoCAT02(clr);
    EXPECT_NEAR(res.RGB[0], 0.4296, TOLERANCE);
    EXPECT_NEAR(res.RGB[1], 1.6975, TOLERANCE);
    EXPECT_NEAR(res.RGB[2], 0.0136, TOLERANCE);

    // Basis Z: XYZ = (0,0,1)
    clr.XYZ[0] = 0.0; clr.XYZ[1] = 0.0; clr.XYZ[2] = 1.0;
    res = XYZtoCAT02(clr);
    EXPECT_NEAR(res.RGB[0], -0.1624, TOLERANCE);
    EXPECT_NEAR(res.RGB[1], 0.0061, TOLERANCE);
    EXPECT_NEAR(res.RGB[2], 0.9834, TOLERANCE);
}

// Test 2: Mixed XYZ input to verify linearity and correctness
void test_XYZtoCAT02_mixed_inputs() {
    CAM02COLOR clr;
    CAM02COLOR res;

    // Mixed: XYZ = (0.5, 0.5, 0)
    clr.XYZ[0] = 0.5; clr.XYZ[1] = 0.5; clr.XYZ[2] = 0.0;
    res = XYZtoCAT02(clr);
    // R = 0.5*0.7328 + 0.5*0.4296 = 0.3664 + 0.2148 = 0.5812
    // G = 0.5*(-0.7036) + 0.5*1.6975 = -0.3518 + 0.84875 = 0.49695
    // B = 0.5*0.0030  + 0.5*0.0136  = 0.0015 + 0.0068  = 0.0083
    EXPECT_NEAR(res.RGB[0], 0.5812, TOLERANCE);
    EXPECT_NEAR(res.RGB[1], 0.49695, TOLERANCE);
    EXPECT_NEAR(res.RGB[2], 0.0083, TOLERANCE);
    EXPECT_NEAR(res.XYZ[0], 0.5, TOLERANCE);
    EXPECT_NEAR(res.XYZ[1], 0.5, TOLERANCE);
    EXPECT_NEAR(res.XYZ[2], 0.0, TOLERANCE);
}

// Test 3: Edge case with negative XYZ values (scalar test for robustness)
void test_XYZtoCAT02_negative_inputs() {
    CAM02COLOR clr;
    CAM02COLOR res;

    clr.XYZ[0] = -1.0; clr.XYZ[1] = -1.0; clr.XYZ[2] = -1.0;
    res = XYZtoCAT02(clr);
    // Based on linear transform properties:
    // R = -1*(0.7328) + -1*(0.4296) + -1*(-0.1624) = -1.0
    // G = -1*(-0.7036) + -1*(1.6975) + -1*(0.0061) = -1.0
    // B = -1*(0.0030)  + -1*(0.0136)  + -1*(0.9834) = -1.0
    EXPECT_NEAR(res.RGB[0], -1.0, TOLERANCE);
    EXPECT_NEAR(res.RGB[1], -1.0, TOLERANCE);
    EXPECT_NEAR(res.RGB[2], -1.0, TOLERANCE);
    // Input XYZ should be preserved
    EXPECT_NEAR(res.XYZ[0], -1.0, TOLERANCE);
    EXPECT_NEAR(res.XYZ[1], -1.0, TOLERANCE);
    EXPECT_NEAR(res.XYZ[2], -1.0, TOLERANCE);
}

// Test 4: Zero input vector (edge case)
void test_XYZtoCAT02_zero_input() {
    CAM02COLOR clr;
    CAM02COLOR res;

    clr.XYZ[0] = 0.0; clr.XYZ[1] = 0.0; clr.XYZ[2] = 0.0;
    res = XYZtoCAT02(clr);
    EXPECT_NEAR(res.RGB[0], 0.0, TOLERANCE);
    EXPECT_NEAR(res.RGB[1], 0.0, TOLERANCE);
    EXPECT_NEAR(res.RGB[2], 0.0, TOLERANCE);
    EXPECT_NEAR(res.XYZ[0], 0.0, TOLERANCE);
    EXPECT_NEAR(res.XYZ[1], 0.0, TOLERANCE);
    EXPECT_NEAR(res.XYZ[2], 0.0, TOLERANCE);
}

// Optional: Run all tests
int main() {
    std::cout << "Running XYZtoCAT02 unit tests for CAM02COLOR...\n";

    test_XYZtoCAT02_basis_vectors();
    test_XYZtoCAT02_mixed_inputs();
    test_XYZtoCAT02_negative_inputs();
    test_XYZtoCAT02_zero_input();

    if (g_failures == 0) {
        std::cout << "All XYZtoCAT02 tests PASSED.\n";
    } else {
        std::cout << "XYZtoCAT02 tests completed with " << g_failures << " failure(s).\n";
    }

    // Return non-zero if failures occurred
    return g_failures ? 1 : 0;
}