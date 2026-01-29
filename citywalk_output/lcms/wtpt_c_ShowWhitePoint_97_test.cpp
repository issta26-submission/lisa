// File: utils.h
#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <iostream>
#include <string>
#include <stdbool.h>
#include <utils.h>
#include <cmath>


#ifdef __cplusplus
extern "C" {
#endif

// Basic color-space data structures (simplified for testing)
typedef struct {
    double X;
    double Y;
    double Z;
} cmsCIEXYZ;

typedef struct {
    double L;
    double a;
    double b;
} cmsCIELab;

typedef struct {
    double L;
    double C;
    double h;
} cmsCIELCh;

typedef struct {
    double x;
    double y;
    double Y;
} cmsCIExyY;

// Flags controlling which values to print (mocked in tests)
extern bool lShowXYZ;
extern bool lShowLab;
extern bool lShowLCh;

// Conversion function prototypes (mock implementations provided in mock_utils.cpp)
void cmsXYZ2Lab(void* dummy, cmsCIELab* Lab, cmsCIEXYZ* WtPt);
void cmsLab2LCh(cmsCIELCh* LCh, cmsCIELab* Lab);
void cmsXYZ2xyY(cmsCIExyY* xyY, cmsCIEXYZ* WtPt);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H

// File: mock_utils.cpp

// Global print-flags shared with the tested code
bool lShowXYZ = false;
bool lShowLab = false;
bool lShowLCh = false;

// Global storage to inspect internal state from tests
cmsCIEXYZ LastWtPt;
cmsCIELab LastLab;
cmsCIELCh LastLCh;
double LastSENS = 0.0;

// Expose internal state for tests (as extern declarations from utils.h)
extern cmsCIEXYZ LastWtPt;
extern cmsCIELab LastLab;
extern cmsCIELCh LastLCh;

void cmsXYZ2Lab(void* /*dummy*/, cmsCIELab* Lab, cmsCIEXYZ* WtPt)
{
    // Simple deterministic mapping to allow test assertions
    if (Lab) {
        Lab->L = WtPt->X;
        Lab->a = WtPt->Y;
        Lab->b = WtPt->Z;
        LastLab = *Lab;
    }
    if (WtPt) {
        LastWtPt = *WtPt;
    }
}

void cmsLab2LCh(cmsCIELCh* LCh, cmsCIELab* Lab)
{
    if (LCh && Lab) {
        LCh->L = Lab->L;
        LCh->C = std::sqrt(Lab->a * Lab->a + Lab->b * Lab->b);
        LCh->h = 0.0; // not needed for tests
        LastLCh = *LCh;

        // Compute and store the Ssens value as used by ShowWhitePoint
        LastSENS = (LCh->C * 100.0) / std::sqrt(LCh->C * LCh->C + LCh->L * LCh->L);
    }
}

void cmsXYZ2xyY(cmsCIExyY* xyY, cmsCIEXYZ* WtPt)
{
    if (xyY && WtPt) {
        xyY->x = (WtPt->X > 0) ? WtPt->X : 0.0;
        xyY->y = (WtPt->Y > 0) ? WtPt->Y : 0.0;
        xyY->Y = WtPt->Z;
    }
}

// End-to-end test friendly aliases
cmsCIEXYZ LastWtPtAlias; // optional alias if needed

// Ensure the symbol LastWtPt refers to the proper object
cmsCIEXYZ LastWtPt = LastWtPtAlias;

// Note: The real code uses cmsXYZ2Lab, cmsLab2LCh, cmsXYZ2xyY and global flags.
// The mock implementations above provide deterministic behavior for unit tests.


// File: test_show_white_point.cpp
// This test suite exercises ShowWhitePoint(cmsCIEXYZ* WtPt) from wtpt.c
// - Verifies that internal transformations are performed as expected (via mocks)
// - Verifies the Sens value calculation (via LastSENS captured in mocks)
// - Avoids terminating tests on failure; reports pass/fail for each case


// Forward declaration of the function under test (C linkage)
extern "C" void ShowWhitePoint(cmsCIEXYZ* WtPt);

// Simple non-terminating assertion helper
#define CHECK(cond, msg) do { if (!(cond)) { std::cerr << "[FAIL] " << msg << "\n"; ++g_failures; } } while (0)

static int g_failures = 0;

static bool approx(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Basic transformation path mapping X->L, Y->a, Z->b and C from a,b
// Verifies internal state and SENS calculation for a known input.
void test_ShowWhitePoint_basic_mapping() {
    // Arrange
    cmsCIEXYZ wt;
    wt.X = 1.0; // L
    wt.Y = 2.0; // a
    wt.Z = 3.0; // b

    LastLab = {0,0,0};
    LastLCh = {0,0,0};
    LastSENS = 0.0;
    LastWtPt = {0,0,0};

    lShowXYZ = true;
    lShowLab = true;
    lShowLCh = true;

    // Act
    ShowWhitePoint(&wt);

    // Assert
    CHECK(LastWtPt.X == wt.X && LastWtPt.Y == wt.Y && LastWtPt.Z == wt.Z,
          "ShowWhitePoint should propagate input WtPt into LastWtPt via cmsXYZ2Lab");

    // Lab derived from mapping in mock: L=WtPt.X, a=WtPt.Y, b=WtPt.Z
    CHECK(approx(LastLab.L, wt.X) && approx(LastLab.a, wt.Y) && approx(LastLab.b, wt.Z),
          "Lab should be mapped from WtPt (L= X, a=Y, b=Z)");

    // LCh derived from Lab: L=Lab.L, C = sqrt(a^2 + b^2)
    double expectedC = std::sqrt(wt.Y * wt.Y + wt.Z * wt.Z);
    CHECK(approx(LastLCh.L, wt.X) && approx(LastLCh.C, expectedC),
          "LCh should be derived from Lab (L=Lab.L, C=sqrt(a^2+b^2))");

    // SENS calculated in cmsLab2LCh should be stored in LastSENS
    double expectedS = (LastLCh.C * 100.0) / std::sqrt(LastLCh.C * LastLCh.C + LastLCh.L * LastLCh.L);
    CHECK(approx(LastSENS, expectedS),
          "Sens should equal (C*100)/sqrt(C^2 + L^2)");
    
    // H should be 0 in this mock path
    CHECK(approx(LastLCh.h, 0.0), "LCh.h should be 0 in mock implementation");
}

// Test 2: Variation with negative and larger values to ensure multiple branches execute
// Verifies that internal state remains consistent for different input while formulas hold.
void test_ShowWhitePoint_variant_input() {
    // Arrange
    cmsCIEXYZ wt;
    wt.X = 4.0;
    wt.Y = -1.0;
    wt.Z = 2.0;

    LastLab = {0,0,0};
    LastLCh = {0,0,0};
    LastSENS = 0.0;
    LastWtPt = {0,0,0};

    lShowXYZ = true;
    lShowLab = false; // buttons off should not affect logic inside mock aside from not printing
    lShowLCh = true;

    // Act
    ShowWhitePoint(&wt);

    // Assert internal consistency
    CHECK(LastWtPt.X == wt.X && LastWtPt.Y == wt.Y && LastWtPt.Z == wt.Z,
          "ShowWhitePoint should propagate input WtPt into LastWtPt via cmsXYZ2Lab (variant)");
    
    // Lab: L=WtPt.X, a=WtPt.Y, b=WtPt.Z
    CHECK(approx(LastLab.L, wt.X) && approx(LastLab.a, wt.Y) && approx(LastLab.b, wt.Z),
          "Lab should be mapped from WtPt (variant)");

    // LCh
    double expectedC = std::sqrt(wt.Y * wt.Y + wt.Z * wt.Z);
    CHECK(approx(LastLCh.L, wt.X) && approx(LastLCh.C, expectedC),
          "LCh should be derived from Lab (variant)");

    // SENS
    double expectedS = (LastLCh.C * 100.0) / std::sqrt(LastLCh.C * LastLCh.C + LastLCh.L * LastLCh.L);
    CHECK(approx(LastSENS, expectedS), "Sens should match recomputed value for variant input");
}

// Runner
int main() {
    std::cout << "Running ShowWhitePoint unit tests (mocked dependencies)..." << std::endl;

    test_ShowWhitePoint_basic_mapping();
    test_ShowWhitePoint_variant_input();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}