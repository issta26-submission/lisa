// Test suite for the focal method chromaticity_x(CIE_color c) from pngvalid.c
// This file provides a lightweight, framework-free set of unit tests (no GTest) 
// suitable for C++11 compilation. It relies on the real project headers to expose
// CIE_color and chromaticity_x with C linkage when included from C++.
// Important: Ensure the build environment provides png.h (or the proper header path)
// so that CIE_color and chromaticity_x are declared correctly.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <math.h>


// Bring C API into C++ translation unit without name mangling issues
#if defined(__cplusplus)
extern "C" {
#endif

// Declaration of the focal function from the C source
// The actual type CIE_color is defined in the project's headers (png.h)
typedef struct CIE_color_tag {
    double X;
    double Y;
    double Z;
} CIE_color;

// In real project, this would be provided by the headers, e.g.:
// double chromaticity_x(CIE_color c);
double chromaticity_x(CIE_color c);

#if defined(__cplusplus)
}
#endif

// Lightweight test harness (no external test framework)
// Provides a simple assertion helper that reports failures but continues execution.

/*
Test plan (mapped to Candidate Keywords and core logic):
- Core computation: X / (X + Y + Z)
  Keywords: X, Y, Z, sum = X+Y+Z, result = X / sum
- True branch coverage:
  - Typical positive numbers (regular division)
  - Negative numbers (negative numerator)
  - Sum equals zero with nonzero X (division by zero -> Inf)
  - Sum equals zero with X == Y == Z == 0 (0/0 -> NaN)
- Boundary checks:
  - Validate result within [0,1] when sum > 0
  - NaN and Inf handling via std::isnan / std::isinf
- Type safety and accessibility:
  - Use the project-provided CIE_color type directly
*/

static bool almost_equal(double a, double b, double tol = 1e-12) {
    // Handle NaN comparison safely: NaN == NaN is false, so use isnan checks
    if (std::isnan(a) && std::isnan(b))
        return true;
    // Handle infinities as a normal floating check (not expected for both, but safe)
    if (std::isinf(a) || std::isinf(b))
        return a == b;
    return std::fabs(a - b) <= tol;
}

// Test 1: Basic positive case
// X=0.1, Y=0.2, Z=0.3 => sum=0.6 => chromaticity_x = 0.1/0.6 = 0.166666...
static bool test_chromaticity_x_basic() {
    CIE_color c;
    c.X = 0.1;
    c.Y = 0.2;
    c.Z = 0.3;

    double res = chromaticity_x(c);
    double expected = 0.1 / (0.1 + 0.2 + 0.3);

    bool ok = almost_equal(res, expected);
    if (!ok) {
        std::cerr << "test_chromaticity_x_basic FAILED: res=" << res
                  << " expected=" << expected << "\n";
    }
    return ok;
}

// Test 2: Negative numerator
// X=-0.1, Y=0.2, Z=0.3 => sum=0.4 => chromaticity_x = -0.1/0.4 = -0.25
static bool test_chromaticity_x_negative_numerator() {
    CIE_color c;
    c.X = -0.1;
    c.Y = 0.2;
    c.Z = 0.3;

    double res = chromaticity_x(c);
    double expected = -0.1 / (0.1 - 0.2 - 0.3 + 0.0); // placeholder to ensure sum calc used below
    // Correct sum: X+Y+Z = -0.1 + 0.2 + 0.3 = 0.4
    expected = -0.1 / (0.1 * 0.0 + 0.4); // keep straightforward calculation
    // Simpler and correct:
    expected = -0.1 / (0.4);

    bool ok = almost_equal(res, expected);
    if (!ok) {
        std::cerr << "test_chromaticity_x_negative_numerator FAILED: res=" << res
                  << " expected=" << expected << "\n";
    }
    return ok;
}

// Test 3: Sum equals zero with nonzero X -> division by zero => Inf
// X=1.0, Y=-1.0, Z=0.0 => sum=0 => chromaticity_x = 1.0/0.0 -> +Inf
static bool test_chromaticity_x_div_by_zero_pos_inf() {
    CIE_color c;
    c.X = 1.0;
    c.Y = -1.0;
    c.Z = 0.0;

    double res = chromaticity_x(c);

    bool ok = std::isinf(res) && (res > 0.0);
    if (!ok) {
        std::cerr << "test_chromaticity_x_div_by_zero_pos_inf FAILED: res=" << res
                  << " expected +Inf\n";
    }
    return ok;
}

// Test 4: Sum zero with all zeros -> 0/0 -> NaN
// X=0, Y=0, Z=0 => sum=0 => 0/0
static bool test_chromaticity_x_nan_case() {
    CIE_color c;
    c.X = 0.0;
    c.Y = 0.0;
    c.Z = 0.0;

    double res = chromaticity_x(c);

    bool ok = std::isnan(res);
    if (!ok) {
        std::cerr << "test_chromaticity_x_nan_case FAILED: res=" << res
                  << " expected NaN\n";
    }
    return ok;
}

// Test 5: Boundary: sum > 0 ensures result within [0,1]
// X=0.6, Y=0.2, Z=0.2 => sum=1.0 => chromaticity_x = 0.6/1.0 = 0.6
static bool test_chromaticity_x_within_bounds() {
    CIE_color c;
    c.X = 0.6;
    c.Y = 0.2;
    c.Z = 0.2;

    double res = chromaticity_x(c);
    double expected = 0.6; // since sum is 1.0
    bool ok = almost_equal(res, expected) && res >= 0.0 && res <= 1.0;
    if (!ok) {
        std::cerr << "test_chromaticity_x_within_bounds FAILED: res=" << res
                  << " expected=" << expected << ", bounds [0,1]\n";
    }
    return ok;
}

int main() {
    // Run all tests, collect results
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok) {
        ++total;
        if (ok) {
            ++passed;
        } else {
            std::cerr << "FAILED: " << name << "\n";
        }
        return ok;
    };

    bool a = test_chromaticity_x_basic();
    run("test_chromaticity_x_basic", a);

    bool b = test_chromaticity_x_negative_numerator();
    run("test_chromaticity_x_negative_numerator", b);

    bool c = test_chromaticity_x_div_by_zero_pos_inf();
    run("test_chromaticity_x_div_by_zero_pos_inf", c);

    bool d = test_chromaticity_x_nan_case();
    run("test_chromaticity_x_nan_case", d);

    bool e = test_chromaticity_x_within_bounds();
    run("test_chromaticity_x_within_bounds", e);

    if (passed == total) {
        std::cout << "ALL CHROMATICITY_X TESTS PASSED (" << passed << "/" << total << ")\n";
        return 0;
    } else {
        std::cout << "CHROMATICITY_X TESTS PASSED: " << passed << " / " << total << "\n";
        return 1;
    }
}