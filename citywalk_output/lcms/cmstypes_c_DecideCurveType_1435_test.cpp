/*
   Lightweight C++11 test suite for cmsTagTypeSignature DecideCurveType
   - No Google Test or external testing framework used
   - Uses a small custom EXPECT-like assertions that continue on failure
   - Covers all branches of DecideCurveType as implemented in cmsTagTypeSignature DecideCurveType(cmsFloat64Number ICCVersion, const void *Data)
   - Relies on LittleCMS structures from lcms2.h (extern "C" linkage)
*/

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <lcms2.h>


// Bring in C headers with C linkage
extern "C" {
}

// Simple non-terminating test harness
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total; \
        if (cond) { ++g_passed; } \
        else { ++g_failed; std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")\n"; } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++g_total; \
        if ((a) == (b)) { ++g_passed; } \
        else { ++g_failed; std::cerr << "[FAILED] " << msg << " Expected: " << (b) << " Actual: " << (a) << " (line " << __LINE__ << ")\n"; } \
    } while (0)


// Helper: create a cmsToneCurve with n segments, all segments having the same Type
static cmsToneCurve* create_curve(int nSegments, int type) {
    cmsToneCurve* Curve = new cmsToneCurve;
    Curve->nSegments = static_cast<cmsUInt32Number>(nSegments);
    Curve->Segments = new cmsToneCurveSegment[nSegments];
    for (int i = 0; i < nSegments; ++i) {
        Curve->Segments[i].Type = type;
    }
    return Curve;
}

// Helper: clean up created curve
static void destroy_curve(cmsToneCurve* Curve) {
    if (Curve) {
        delete[] Curve->Segments;
        delete Curve;
    }
}

// Test 1: ICCVersion < 4.0 should yield CurveType
static void test_ICCVersion_below_4_yields_CurveType() {
    cmsToneCurve* Curve = create_curve(1, 0); // Type=0 is valid, but ICCVersion < 4 triggers first branch
    cmsTagTypeSignature res = DecideCurveType(3.0, Curve);
    EXPECT_EQ(res, cmsSigCurveType, "ICCVersion < 4.0 must return cmsSigCurveType");
    destroy_curve(Curve);
}

// Test 2: nSegments != 1 should yield CurveType even if ICCVersion >= 4
static void test_nSegments_not_one_yields_CurveType() {
    cmsToneCurve* Curve = create_curve(2, 0); // 2 segments -> should not be parametric
    cmsTagTypeSignature res = DecideCurveType(4.0, Curve);
    EXPECT_EQ(res, cmsSigCurveType, "nSegments != 1 must return cmsSigCurveType");
    destroy_curve(Curve);
}

// Test 3: nSegments == 1 but first segment Type < 0 should yield CurveType
static void test_Type_negative_yields_CurveType() {
    cmsToneCurve* Curve = create_curve(1, -1); // Negative type -> not a valid parametric
    cmsTagTypeSignature res = DecideCurveType(4.0, Curve);
    EXPECT_EQ(res, cmsSigCurveType, "First segment Type < 0 must return cmsSigCurveType");
    destroy_curve(Curve);
}

// Test 4: nSegments == 1, Type > 5 should yield CurveType
static void test_Type_above_5_yields_CurveType() {
    cmsToneCurve* Curve = create_curve(1, 6); // Type > 5 -> not ICC parametric
    cmsTagTypeSignature res = DecideCurveType(4.5, Curve);
    EXPECT_EQ(res, cmsSigCurveType, "First segment Type > 5 must return cmsSigCurveType");
    destroy_curve(Curve);
}

// Test 5: nSegments == 1, 0 <= Type <= 5 and ICCVersion >= 4 should yield ParametricCurveType
static void test_Parametric_yields_ParametricCurveType() {
    cmsToneCurve* Curve = create_curve(1, 3); // Type in valid range
    cmsTagTypeSignature res = DecideCurveType(4.0, Curve);
    EXPECT_EQ(res, cmsSigParametricCurveType, "ICCVersion >= 4 with valid Type should return cmsSigParametricCurveType");
    destroy_curve(Curve);
}

// Test 6: Boundary case: ICCVersion exactly 4.0 with Type 0
static void test_boundary_ICCVersion4_Type0_yields_ParametricCurveType() {
    cmsToneCurve* Curve = create_curve(1, 0);
    cmsTagTypeSignature res = DecideCurveType(4.0, Curve);
    EXPECT_EQ(res, cmsSigParametricCurveType, "ICCVersion == 4.0 with Type in [0..5] should return cmsSigParametricCurveType");
    destroy_curve(Curve);
}

// Entry point
int main() {
    std::cout << "Starting DecideCurveType unit tests (CMS/C++)...\n";

    test_ICCVersion_below_4_yields_CurveType();
    test_nSegments_not_one_yields_CurveType();
    test_Type_negative_yields_CurveType();
    test_Type_above_5_yields_CurveType();
    test_Parametric_yields_ParametricCurveType();
    test_boundary_ICCVersion4_Type0_yields_ParametricCurveType();

    std::cout << "Tests run: " << g_total << ", Passed: " << g_passed << ", Failed: " << g_failed << "\n";

    if (g_failed == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}