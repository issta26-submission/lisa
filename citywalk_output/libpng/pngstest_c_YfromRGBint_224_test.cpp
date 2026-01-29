/*
Unit Test Suite for YfromRGBint (PNG STRESS/ISOC90 style test)

Step 1 - Program Understanding (Candidate Keywords)
- Focal method: YfromRGBint(int ir, int ig, int ib)
- Core dependency: YfromRGB(double r, double g, double b) is invoked within YfromRGBint
- Core inputs: ir, ig, ib (integers, presumably in 0..255)
- Core output: int (the return value of YfromRGBint), which reflects a Y (luma) value derived from RGB
- Candidate Keywords (core components to reflect in tests): YfromRGBint, YfromRGB, r, g, b, ir, ig, ib, int, double, RGB, luma, sRGB, PNG
- The test will exercise the relationship of YfromRGBint outputs across representative color samples and basic boundaries.

Step 2 - Unit Test Generation (Test Plan)
- Since we do not rely on GTest, we implement a lightweight C++11 test harness that calls the C function YfromRGBint from pngstest.c.
- Tests focus on robust, non-terminating assertions (we implement simple PASS/FAIL reporting).
- Tests cover:
  1) Boundary behavior: (0,0,0) yields the minimum output (expected near 0) and (255,255,255) yields a maximum output (expected > 0)
  2) Color channel ordering consistency: YfromRGBint(255,0,0) vs YfromRGBint(0,255,0) vs YfromRGBint(0,0,255) to reflect typical luma weighting (Green typically yields the highest luma, Blue the lowest)
  3) Monotonicity/relative brightness: YfromRGBint(255,255,0) and YfromRGBint(255,255,255) should be non-decreasing relative to (0,0,0)
  4) Rounding stability for grayscale-like inputs: YfromRGBint(128,128,128) vs YfromRGBint(127,127,127) should be close (difference of at most 1 if rounding occurs)

Step 3 - Test Case Refinement (Domain knowledge)
- We rely on standard luma intuition (R/G/B contribute differently to Y). While exact coefficients depend on implementation (601 vs 709, etc.), the relative ordering among distinct primary colors (R, G, B) is stable: Green typically yields the largest Y, Blue the smallest, Red in-between.
- We keep assertions non-terminating and provide detailed messages for each case.
- We avoid private or static internals; we only rely on the external YfromRGBint symbol.
- We use only standard library facilities in C++, and provide explanations in comments for each test.

Compilation note:
- The test code declares extern "C" int YfromRGBint(int ir, int ig, int ib); It assumes the symbol is available from the PNG test code (pngstest.c) when linked.
- If the external YfromRGBint is defined differently (e.g., returns int with different scaling), the tests may need adaptation. The relative ordering and boundary checks are chosen to be robust to typical luma implementations.

// Test file: test_YfromRGBint.cpp
// Compile command example (assuming pngstest.c is available and built as a library or object):
//   g++ -std=c++11 -I<path-to-pngheaders> test_YfromRGBint.cpp pngstest.o -o test_YfromRGBint
// or: g++ -std=c++11 test_YfromRGBint.cpp -o test_YfromRGBint -L. -lpngstest

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <functional>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <ctype.h>


// Step 1: External C function under test
extern "C" int YfromRGBint(int ir, int ig, int ib); // Provided by pngstest.c

// Simple test framework (non-GTest)
static int g_passed = 0;
static int g_failed = 0;

// Helper: run a test and report
static void RunTest(const char* testName, std::function<bool()> testFunc) {
    bool ok = false;
    try {
        ok = testFunc();
    } catch (...) {
        ok = false;
    }
    if (ok) {
        ++g_passed;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        ++g_failed;
        std::cout << "[FAIL] " << testName << "\n";
    }
}

// Helper: normalize int output to [0,1] as double
static double NormalizeOut(int yInt) {
    // Guard against potential out-of-range values due to implementation differences
    if (yInt <= 0) return 0.0;
    // If output can exceed 255 due to some internal scaling, clamp to 1.0 after normalization
    if (yInt >= 255) return 1.0;
    return static_cast<double>(yInt) / 255.0;
}

// Test 1: Boundary behavior - black should map to minimum, white should map to maximum
bool test_boundary_zero_and_white() {
    int yBlack = YfromRGBint(0, 0, 0);
    int yWhite = YfromRGBint(255, 255, 255);
    // Expect black to be at or near 0
    if (yBlack < 0) {
        std::cerr << "YfromRGBint(0,0,0) produced negative value: " << yBlack << "\n";
        return false;
    }
    // Expect white to be greater than black
    if (!(yWhite > yBlack)) {
        std::cerr << "YfromRGBint(255,255,255) is not greater than YfromRGBint(0,0,0) (" << yWhite << " <= " << yBlack << ")\n";
        return false;
    }
    // Optional: ensure white is reasonably high (>= 200 would be too strict for some implementations)
    if (yWhite <= 0) {
        std::cerr << "YfromRGBint(255,255,255) unexpectedly low: " << yWhite << "\n";
        return false;
    }
    return true;
}

// Test 2: Color channel ordering consistency
// For most luma implementations, Green carries the largest weight, Blue the smallest.
bool test_red_green_blue_ordering() {
    int yR = YfromRGBint(255, 0, 0);
    int yG = YfromRGBint(0, 255, 0);
    int yB = YfromRGBint(0, 0, 255);

    double dR = NormalizeOut(yR);
    double dG = NormalizeOut(yG);
    double dB = NormalizeOut(yB);

    // Expect Green to be largest, Blue to be smallest, Red in-between
    if (!(dG > dR + 0.01 && dR > dB + 0.01)) {
        std::cerr << "Unexpected channel ordering: G=" << dG << ", R=" << dR << ", B=" << dB
                  << " (expected G > R > B with margin)\n";
        return false;
    }
    return true;
}

// Test 3: Monotonicity with brightness (0,0,0) -> (255,255,255)
bool test_monotonic_brightness() {
    int yBlack = YfromRGBint(0, 0, 0);
    int yYellow = YfromRGBint(255, 255, 0);
    int yWhite = YfromRGBint(255, 255, 255);

    // Expect non-decreasing luminance with increasing color channels
    if (!(yBlack <= yYellow && yYellow <= yWhite)) {
        std::cerr << "Brightness monotonicity violation: B=" << yBlack
                  << " Y=" << yYellow << " W=" << yWhite << "\n";
        return false;
    }
    return true;
}

// Test 4: Grayscale rounding stability
// For grayscale inputs, consecutive integer steps should yield outputs that are close (due to casting)
bool test_grayscale_rounding_stability() {
    int y128 = YfromRGBint(128, 128, 128);
    int y127 = YfromRGBint(127, 127, 127);
    if (std::abs(y128 - y127) > 1) {
        std::cerr << "Rounding instability for grayscale inputs: 128->" << y128
                  << ", 127->" << y127 << "\n";
        return false;
    }
    return true;
}

int main() {
    // Step 3: Run all tests
    RunTest("Boundary: black and white mapping", test_boundary_zero_and_white);
    RunTest("Color channel ordering (G > R > B)", test_red_green_blue_ordering);
    RunTest("Monotonic brightness from black to white", test_monotonic_brightness);
    RunTest("Grayscale rounding stability (128 vs 127)", test_grayscale_rounding_stability);

    // Summary
    std::cout << "\nTest Summary: " << g_passed << " passed, " << g_failed << " failed\n";
    // Non-terminating tests: return non-zero if any failed to indicate test suite failure
    return (g_failed == 0) ? 0 : 1;
}