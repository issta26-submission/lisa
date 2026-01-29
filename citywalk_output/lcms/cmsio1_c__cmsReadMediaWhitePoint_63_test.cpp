// cmsio1_test.cpp
// Light-weight unit tests for the focal method _cmsReadMediaWhitePoint
// Using a small custom test harness (no GTest) with simple mock overrides.
// This test suite focuses on exercising the key branches of the focal method
// by providing controlled mock values for dependent functions.
// Notes:
// - We avoid modifying the original source; instead we override a subset of
//   dependent library functions via weak/link-time override technique.
// - The approach is designed to work when linking against the real Little CMS
//   library (lcms2). It assumes _cmsReadMediaWhitePoint is linkable from the
//   library (not a static/internal function).
// - The tests are non-terminating: they log failures but continue execution
//   to maximize coverage.

#include <cstring>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Define minimal types compatible with the focal function's usage.
// We do not include the actual lcms headers to keep this test self-contained.
typedef void* cmsHPROFILE;
typedef struct { double x; double y; double z; } cmsCIEXYZ;
typedef int cmsBool;
typedef uint32_t cmsTagSignature;

// The library typically defines these. We declare them here to enable linking.
// We rely on the library's internal definition for cmsSigDisplayClass.
extern "C" cmsBool _cmsReadMediaWhitePoint(cmsCIEXYZ* Dest, cmsHPROFILE hProfile);

// Mockable dependencies (to override the library's behavior for testing)
extern "C" cmsCIEXYZ* cmsD50_XYZ();
extern "C" cmsCIEXYZ* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature sig);
extern "C" uint32_t cmsGetEncodedICCversion(cmsHPROFILE hProfile);
extern "C" uint32_t cmsGetDeviceClass(cmsHPROFILE hProfile);
extern "C" const uint32_t cmsSigDisplayClass; // symbol provided by library in real build

// Test-specific mock data and overrides
static cmsCIEXYZ D50XYZ = { 0.9642, 1.0000, 0.8249 }; // Typical D50 white point
static cmsCIEXYZ TagXYZ  = { 0.345, 0.355, 0.2 };     // Arbitrary test tag value
static cmsCIEXYZ* MockTag = nullptr;
static uint32_t MockICCVersion = 0;      // ICC Version returned by cmsGetEncodedICCversion
static uint32_t MockDeviceClass = 0;       // Device class returned by cmsGetDeviceClass

// Helpers for test assertions
static bool xyzEquals(const cmsCIEXYZ& a, const cmsCIEXYZ& b, double tol = 1e-6) {
    return (std::fabs(a.x - b.x) <= tol) &&
           (std::fabs(a.y - b.y) <= tol) &&
           (std::fabs(a.z - b.z) <= tol);
}

// Override cmsD50_XYZ() to return our D50XYZ
extern "C" cmsCIEXYZ* cmsD50_XYZ() {
    return &D50XYZ;
}

// Override cmsReadTag to return test-controlled Tag
extern "C" cmsCIEXYZ* cmsReadTag(cmsHPROFILE /*hProfile*/, cmsTagSignature /*sig*/) {
    return MockTag; // May be nullptr to simulate missing tag
}

// Override cmsGetEncodedICCversion to return test-controlled version
extern "C" uint32_t cmsGetEncodedICCversion(cmsHPROFILE /*hProfile*/) {
    return MockICCVersion;
}

// Override cmsGetDeviceClass to return test-controlled device class
extern "C" uint32_t cmsGetDeviceClass(cmsHPROFILE /*hProfile*/) {
    return MockDeviceClass;
}

// Use the library's cmsSigDisplayClass constant if available at link time.
// If not, the value is resolved from the library; tests that rely on it should
// not depend on its exact numeric value (we avoid changing this in tests).
extern "C" const uint32_t cmsSigDisplayClass;

// Small test harness helpers
static void logTest(const char* name, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << "\n";
}

static void test_TagNull_ReturnsD50() {
    // Setup: No Media White Point tag in the profile (Tag == NULL)
    MockTag = nullptr;
    MockICCVersion = 0x4000000;  // Some value; not critical for this path
    MockDeviceClass = 0xDEADBEEF; // Non-Display (or non-matching) to avoid inner path

    cmsCIEXYZ Dest;
    cmsHPROFILE fakeProfile = nullptr;

    bool ok = (_cmsReadMediaWhitePoint(&Dest, fakeProfile) == 1);

    bool destIsD50 = xyzEquals(Dest, *cmsD50_XYZ());

    logTest("TagNull -> Dest should be D50", ok && destIsD50);
}

static void test_TagNonNull_AllOkPath() {
    // Setup: Tag exists, but we ensure the outer condition leads to using Tag
    MockTag = &TagXYZ;
    MockICCVersion = 0x5000000; // >= 0x4000000 to skip inner D50 path
    MockDeviceClass = 0x0;      // Not equal to cmsSigDisplayClass; or simple mismatch

    cmsCIEXYZ Dest;
    cmsHPROFILE fakeProfile = nullptr;

    bool ok = (_cmsReadMediaWhitePoint(&Dest, fakeProfile) == 1);

    bool destEqualsTag = xyzEquals(Dest, TagXYZ);

    logTest("TagNonNull + version >= 0x4000000 (or non-Display) -> Dest equals Tag", ok && destEqualsTag);
}

static void runAllTests() {
    std::cout << "Running tests for _cmsReadMediaWhitePoint (cmsio1.c focal method)\n";
    test_TagNull_ReturnsD50();
    test_TagNonNull_AllOkPath();
}

int main() {
    runAllTests();
    return 0;
}

// Explanation of test cases and design notes:
//
// - Test_TagNull_ReturnsD50
//   Sets MockTag to NULL to simulate a profile that carries no Media White Point
//   tag. According to the focal method, when Tag == NULL, the function should
//   set Dest to cmsD50_XYZ() and return TRUE. The test asserts that the result
//   Dest matches the D50 XYZ coordinates.
//
// - Test_TagNonNull_AllOkPath
//   Sets MockTag to a non-null XYZ value and forces the version check to fail the
//   inner branch or to bypass the DisplayClass check by returning a non-Display
//   device class. This ensures the function follows the "All seems ok" path and
//   copies Tag into Dest. The test asserts Dest equals the mock TagXYZ.
//
// - Notes about the mocking approach
//   The test harness overrides dependent functions used by the focal method
//   (cmsReadTag, cmsGetEncodedICCversion, cmsGetDeviceClass, cmsD50_XYZ) by
//   providing non-library definitions with C linkage. This technique allows
//   deterministic control of the focal method's dependencies without modifying
//   the library source. In real builds, linking order and symbol resolution must
//   ensure these overrides take precedence or be linked with a compatible mock
//   library. The approach is intentionally lightweight to maximize portability
//   across environments that support C/link-time overrides.
//
// - Important caveat
//   If cmsD50_XYZ(), cmsSigDisplayClass, or other internal constants differ between
//   library versions, the tests relying on exact numeric values may need to be
//   adjusted. The tests above are designed to be robust by focusing on the
//   observable Dest values rather than the internal numerics, where possible.