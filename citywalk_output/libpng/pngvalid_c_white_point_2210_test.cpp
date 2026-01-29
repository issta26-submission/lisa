// This test suite targets the focal method:
//   white_point(const color_encoding *encoding)
// which computes the CIE white point by summing the XYZ components
// of the red, green, and blue encodings.
//
// The tests are designed to be self-contained C++11 code that can be
// linked against the C implementation provided in pngvalid.c.
// We avoid any external testing framework (GTest, etc.) per the task.
// Explanatory comments accompany each unit test to describe its intent.
//
// Domain knowledge assumptions (aligned with provided info):
// - color_encoding struct contains three components: red, green, blue.
// - Each component has fields X, Y, Z (floating point).
// - The function returns a CIE_color with fields X, Y, Z representing the summed white point.
//
// Notes:
// - The test declares matching lightweight C-style structs to mirror the
//   layout expected by the focal function. The function is declared with C linkage
//   via extern "C" to enable linking with the C implementation.
// - We perform non-terminating (non-aborting) assertions by returning
//   boolean results and printing detailed mismatch information.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Domain-specific type definitions to mirror the types used by white_point.
// These definitions must be layout-compatible with the actual pngvalid.c types.
typedef struct {
    double X;
    double Y;
    double Z;
} XYZ;

typedef struct {
    XYZ red;
    XYZ green;
    XYZ blue;
} color_encoding;

typedef struct {
    double X;
    double Y;
    double Z;
} CIE_color;

// Declaration of the focal function from the C source file (pngvalid.c).
// We declare it with C linkage to ensure correct name mangling during linking.
extern "C" CIE_color white_point(const color_encoding *encoding);

// Utility: compare doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: All color components are zero -> white_point should be (0,0,0)
bool test_white_point_zero_components() {
    color_encoding enc;
    // red
    enc.red.X = 0.0; enc.red.Y = 0.0; enc.red.Z = 0.0;
    // green
    enc.green.X = 0.0; enc.green.Y = 0.0; enc.green.Z = 0.0;
    // blue
    enc.blue.X = 0.0; enc.blue.Y = 0.0; enc.blue.Z = 0.0;

    CIE_color w = white_point(&enc);

    bool ok = almost_equal(w.X, 0.0) && almost_equal(w.Y, 0.0) && almost_equal(w.Z, 0.0);
    if (!ok) {
        std::cerr << "[FAIL] test_white_point_zero_components: got ("
                  << w.X << ", " << w.Y << ", " << w.Z << ") expected (0,0,0)\n";
    }
    return ok;
}

// Test 2: Basic orthonormal inputs to verify simple sum behavior
// red = (1,0,0), green = (0,1,0), blue = (0,0,1) -> white should be (1,1,1)
bool test_white_point_basic_sum() {
    color_encoding enc;
    // red
    enc.red.X = 1.0; enc.red.Y = 0.0; enc.red.Z = 0.0;
    // green
    enc.green.X = 0.0; enc.green.Y = 1.0; enc.green.Z = 0.0;
    // blue
    enc.blue.X = 0.0; enc.blue.Y = 0.0; enc.blue.Z = 1.0;

    CIE_color w = white_point(&enc);

    bool ok = almost_equal(w.X, 1.0) && almost_equal(w.Y, 1.0) && almost_equal(w.Z, 1.0);
    if (!ok) {
        std::cerr << "[FAIL] test_white_point_basic_sum: got ("
                  << w.X << ", " << w.Y << ", " << w.Z << ") expected (1,1,1)\n";
    }
    return ok;
}

// Test 3: Random non-integer values to ensure proper summation with decimals
// red = (0.1,0.2,0.3), green = (0.4,0.5,0.6), blue = (0.7,0.8,0.9)
// expected white = (1.2, 1.5, 1.8)
bool test_white_point_random_values() {
    color_encoding enc;
    // red
    enc.red.X = 0.1; enc.red.Y = 0.2; enc.red.Z = 0.3;
    // green
    enc.green.X = 0.4; enc.green.Y = 0.5; enc.green.Z = 0.6;
    // blue
    enc.blue.X = 0.7; enc.blue.Y = 0.8; enc.blue.Z = 0.9;

    CIE_color w = white_point(&enc);

    bool ok = almost_equal(w.X, 1.2) && almost_equal(w.Y, 1.5) && almost_equal(w.Z, 1.8);
    if (!ok) {
        std::cerr << "[FAIL] test_white_point_random_values: got ("
                  << w.X << ", " << w.Y << ", " << w.Z << ") expected (1.2, 1.5, 1.8)\n";
    }
    return ok;
}

// Test 4: Negative components to ensure the function handles negative values correctly
// red = (-1,-2,-3), green = (-0.5,-0.4,-0.3), blue = (-0.1,-0.2,-0.3)
// expected white = (-1.6, -2.6, -3.6)
bool test_white_point_negative_values() {
    color_encoding enc;
    // red
    enc.red.X = -1.0; enc.red.Y = -2.0; enc.red.Z = -3.0;
    // green
    enc.green.X = -0.5; enc.green.Y = -0.4; enc.green.Z = -0.3;
    // blue
    enc.blue.X = -0.1; enc.blue.Y = -0.2; enc.blue.Z = -0.3;

    CIE_color w = white_point(&enc);

    bool ok = almost_equal(w.X, -1.6) && almost_equal(w.Y, -2.6) && almost_equal(w.Z, -3.6);
    if (!ok) {
        std::cerr << "[FAIL] test_white_point_negative_values: got ("
                  << w.X << ", " << w.Y << ", " << w.Z << ") expected (-1.6, -2.6, -3.6)\n";
    }
    return ok;
}

// Main test runner
int main() {
    std::cout << "Running unit tests for white_point(const color_encoding *encoding)...\n";

    int failures = 0;

    auto log = [&](const char* name, bool result) {
        if (result) {
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
            ++failures;
        }
    };

    log("test_white_point_zero_components", test_white_point_zero_components());
    log("test_white_point_basic_sum",      test_white_point_basic_sum());
    log("test_white_point_random_values",  test_white_point_random_values());
    log("test_white_point_negative_values",test_white_point_negative_values());

    if (failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Total failures: " << failures << "\n";
    }

    // Return non-zero if any test failed to indicate failure in automation frameworks if used.
    return failures ? 1 : 0;
}