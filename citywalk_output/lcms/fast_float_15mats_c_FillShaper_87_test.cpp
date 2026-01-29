// Test suite for FillShaper in fast_float_15mats.c
// - Uses LittleCMS tone curves to create deterministic inputs
// - Avoids GTest; uses a lightweight test harness with non-terminating assertions
// - Two test cases: identity curve (gamma = 1.0) and gamma-2.0 curve
// - Tests ensure FillShaper fills the Table with expected 1.15 fixed-point values

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <lcms2.h>
#include <cstdint>


// Include domain headers

// Ensure we can call the C function FillShaper from C++
extern "C" void FillShaper(cmsUInt16Number* Table, cmsToneCurve* Curve);

// Lightweight test macro: non-terminating, prints details on failure
#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ FAILED: " << msg << " | " #a " = " << (a) << " , " #b " = " << (b) << "\n"; \
        test_ok = false; \
    } \
} while(0)

#define TEST_PASSED() do { if (test_ok) std::cout << "[PASS] " << test_name << "\n"; else std::cout << "[FAIL] " << test_name << "\n"; } while(0)

// Forward declarations of tests
void test_FillShaper_identity();
void test_FillShaper_gamma2();

// Helper: Build an identity tone curve (Points[i] = i/(n-1))
cmsToneCurve* BuildIdentityCurve(int nPoints) {
    std::vector<cmsFloat32Number> pts(nPoints);
    for (int i = 0; i < nPoints; ++i) {
        pts[i] = static_cast<cmsFloat32Number>(i) / static_cast<cmsFloat32Number>(nPoints - 1);
    }
    // cmsBuildToneCurveFloat signature: cmsToneCurve* cmsBuildToneCurveFloat(int nPoints, cmsFloat32Number* Points);
    return cmsBuildToneCurveFloat(nPoints, pts.data());
}

// Helper: Build a gamma curve: y = (t)^(gamma)
cmsToneCurve* BuildGammaCurve(int nPoints, cmsFloat32Number gamma) {
    std::vector<cmsFloat32Number> pts(nPoints);
    for (int i = 0; i < nPoints; ++i) {
        cmsFloat32Number t = static_cast<cmsFloat32Number>(i) / static_cast<cmsFloat32Number>(nPoints - 1);
        // Clamp t to [0,1] just in case
        if (t < 0) t = 0;
        if (t > 1) t = 1;
        pts[i] = std::pow(static_cast<cmsFloat32Number>(t), gamma);
    }
    return cmsBuildToneCurveFloat(nPoints, pts.data());
}

// Test 1: Identity curve (gamma = 1.0) should yield y = R for each sample point
void test_FillShaper_identity() {
    const int nPoints = MAX_NODES_IN_CURVE; // Align with FillShaper loop
    cmsToneCurve* curve = BuildIdentityCurve(nPoints);
    if (!curve) {
        std::cerr << "Failed to build identity tone curve\n";
        std::cout << "[FAIL] test_FillShaper_identity\n";
        return;
    }

    // Prepare output table
    std::vector<cmsUInt16Number> Table(nPoints, 0);

    // Call the function under test
    FillShaper(Table.data(), curve);

    // Verify: for each i, value should be DOUBLE_TO_1FIXED15(R) where R = i/(nPoints-1)
    bool test_ok = true;
    const std::string test_name = "FillShaper_identity";

    for (int i = 0; i < nPoints; ++i) {
        cmsFloat32Number R = static_cast<cmsFloat32Number>(i) / static_cast<cmsFloat32Number>(nPoints - 1);
        cmsFloat32Number expectedY = R; // Identity
        cmsUInt16Number expectedVal = (cmsUInt16Number)DOUBLE_TO_1FIXED15(expectedY);
        EXPECT_EQ(Table[i], expectedVal, "identity curve, index " << i);
    }

    cmsFreeToneCurve(curve);
    TEST_PASSED();
}

// Test 2: Gamma-2 curve should yield y = R^2 for each sample point
void test_FillShaper_gamma2() {
    const int nPoints = MAX_NODES_IN_CURVE; // Align with FillShaper loop
    cmsToneCurve* curve = BuildGammaCurve(nPoints, 2.0f);
    if (!curve) {
        std::cerr << "Failed to build gamma-2 tone curve\n";
        std::cout << "[FAIL] test_FillShaper_gamma2\n";
        return;
    }

    // Prepare output table
    std::vector< cmsUInt16Number > Table(nPoints, 0);

    // Call the function under test
    FillShaper(Table.data(), curve);

    // Verify: for each i, value should be DOUBLE_TO_1FIXED15(R^2) where R = i/(nPoints-1)
    bool test_ok = true;
    const std::string test_name = "FillShaper_gamma2";

    for (int i = 0; i < nPoints; ++i) {
        cmsFloat32Number R = static_cast<cmsFloat32Number>(i) / static_cast<cmsFloat32Number>(nPoints - 1);
        cmsFloat32Number expectedY = R * R; // Gamma^2
        cmsUInt16Number expectedVal = (cmsUInt16Number)DOUBLE_TO_1FIXED15(expectedY);
        EXPECT_EQ(Table[i], expectedVal, "gamma2 curve, index " << i);
    }

    cmsFreeToneCurve(curve);
    TEST_PASSED();
}

// Main: run all tests
int main() {
    std::cout << "Running FillShaper unit tests (no GTest):\n";

    test_FillShaper_identity();
    test_FillShaper_gamma2();

    return 0;
}