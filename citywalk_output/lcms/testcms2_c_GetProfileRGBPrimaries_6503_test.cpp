// Lightweight C++11 test suite for GetProfileRGBPrimaries from testcms2.c
// This test uses LittleCMS (lcms2) to create a known FPL profile (sRGB) and
// verifies that GetProfileRGBPrimaries returns the expected RGB->XYZ matrix.
// It also tests the negative path by passing a NULL profile handle.
// Notes:
// - Compile with: g++ -std=c++11 -I<path-to-lcms2-headers> -lcms2 -o test_getprof test_getprof.cpp
// - Link against LittleCMS (liblcms2). Ensure headers are accessible.
// - The test does not use GoogleTest; it runs in main() and uses simple
//   non-terminating assertions to maximize code coverage.

#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>
#include <cmath>


// Include LittleCMS header

// Declaration of the focal method from testcms2.c
extern "C" {
    // The function under test: GetProfileRGBPrimaries
    cmsBool GetProfileRGBPrimaries(cmsHPROFILE hProfile,
                                   cmsCIEXYZTRIPLE *result,
                                   cmsUInt32Number intent);
}

// Simple non-terminating test harness
static int g_failures = 0;

static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << std::endl;
        ++g_failures;
    } else {
        std::cout << "PASS: " << msg << std::endl;
    }
}

static void expect_false(bool cond, const char* msg) {
    expect_true(!cond, msg);
}

static void expect_close(double a, double b, double tol, const char* label) {
    if (std::fabs(a - b) > tol) {
        std::cerr << "FAIL: " << label << " expected close to " << b
                  << " but got " << a << " (tol " << tol << ")" << std::endl;
        ++g_failures;
    } else {
        std::cout << "PASS: " << label << " (" << a << " ~ " << b << ")" << std::endl;
    }
}

// Test 1: Positive path using a standard sRGB profile.
// Expect the resulting RGB primaries in XYZ space to match the known sRGB matrix.
static void test_GetProfileRGBPrimaries_sRGB(void) {
    // Create a standard sRGB profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    expect_true(hProfile != NULL, "cmsCreate_sRGBProfile should succeed");

    cmsCIEXYZTRIPLE result;
    // Use RELATIVE_COLORIMETRIC as the colorimetric intent (common choice)
    cmsUInt32Number intent = INTENT_RELATIVE_COLORIMETRIC;

    cmsBool ok = GetProfileRGBPrimaries(hProfile, &result, intent);
    cmsCloseProfile(hProfile);

    expect_true(ok == TRUE, "GetProfileRGBPrimaries should return TRUE for a valid sRGB profile");

    // Known sRGB primaries to XYZ (D65)
    // Red:  X=0.4124, Y=0.2126, Z=0.0193
    // Green:X=0.3576, Y=0.7152, Z=0.1192
    // Blue: X=0.1805, Y=0.0722, Z=0.9505
    const double tol = 1e-4;

    // Access results
    double Rx = static_cast<double>(result.Red.X);
    double Ry = static_cast<double>(result.Red.Y);
    double Rz = static_cast<double>(result.Red.Z);

    double Gx = static_cast<double>(result.Green.X);
    double Gy = static_cast<double>(result.Green.Y);
    double Gz = static_cast<double>(result.Green.Z);

    double Bx = static_cast<double>(result.Blue.X);
    double By = static_cast<double>(result.Blue.Y);
    double Bz = static_cast<double>(result.Blue.Z);

    expect_close(Rx, 0.4124, tol, "Red.X == 0.4124");
    expect_close(Ry, 0.2126, tol, "Red.Y == 0.2126");
    expect_close(Rz, 0.0193, tol, "Red.Z == 0.0193");

    expect_close(Gx, 0.3576, tol, "Green.X == 0.3576");
    expect_close(Gy, 0.7152, tol, "Green.Y == 0.7152");
    expect_close(Gz, 0.1192, tol, "Green.Z == 0.1192");

    expect_close(Bx, 0.1805, tol, "Blue.X == 0.1805");
    expect_close(By, 0.0722, tol, "Blue.Y == 0.0722");
    expect_close(Bz, 0.9505, tol, "Blue.Z == 0.9505");
}

// Test 2: Negative path - passing NULL hProfile should fail gracefully.
static void test_GetProfileRGBPrimaries_null_profile(void) {
    cmsCIEXYZTRIPLE result;
    cmsBool ok = GetProfileRGBPrimaries(NULL, &result, INTENT_RELATIVE_COLORIMETRIC);
    expect_true(ok == FALSE, "GetProfileRGBPrimaries should return FALSE when hProfile is NULL");
}

// Main runner
int main(void) {
    std::cout << "Starting GetProfileRGBPrimaries test suite (C++11, no GTest)" << std::endl;

    test_GetProfileRGBPrimaries_sRGB();
    test_GetProfileRGBPrimaries_null_profile();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}