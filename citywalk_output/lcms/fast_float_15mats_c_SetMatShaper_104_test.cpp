// C++11 unit test suite for the focal method SetMatShaper in fast_float_15mats.c
// This test harness does not use GoogleTest. It uses lightweight, non-terminating EXPECT_* style checks
// that print failures but allow all tests to run.
//
// Notes:
// - The tests rely on the presence of LittleCMS-like types (cmsContext, cmsBool, cmsToneCurve, cmsMAT3, cmsVEC3, etc.)
//   and the function prototype for SetMatShaper as declared below. The test compiles against the
//   project header(s) that declare these types.
// - Tests intentionally avoid deep validation of the FillShaper portion (which depends on ToneCurve internals).
//   They focus on allocation, basic field population, and OFF handling branches to improve coverage.
// - Static helpers (e.g., FreeMatShaper) are invoked to avoid leaks.
// - Ensure fast_float_15mats.c is built and linked with this test binary.
//
// If any of the assumptions about the external API differ in your environment, adjust the includes and
// type references accordingly.

#include <cstdint>
#include <fast_float_internal.h>
#include <iostream>
#include <iomanip>
#include <cmath>


// Include the internal header that defines the types used by SetMatShaper, if available.
// If your environment uses a different header path, adjust accordingly.

// Declare the focal function and its destructor/free helper for C++ linkage.
// The implementation of SetMatShaper is in the C file and uses C linkage.
extern "C" {
    XMatShaperData* SetMatShaper(cmsContext ContextID,
                                cmsToneCurve* Curve1[3],
                                cmsMAT3* Mat,
                                cmsVEC3* Off,
                                cmsToneCurve* Curve2[3],
                                cmsBool IdentityMat);

    void FreeMatShaper(cmsContext ContextID, void* Data);
}

// Lightweight test harness
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if( (static_cast<typeof(a)>(a)) != (static_cast<typeof(b)>(b)) ) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NEAR(a, b, tol) do { \
    if (std::abs(static_cast<long double>(a) - static_cast<long double>(b)) > (tol)) { \
        std::cerr << "EXPECT_NEAR failed: " #a " (" << (a) << ") != " #b " (" << (b) \
                  << ") within tol " << (tol) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

static inline int to_fixed15(double x) {
    // Mimic expected 1FIXED15 conversion: 15 fractional bits, rounding to nearest.
    // This mirrors DOUBLE_TO_1FIXED15 usage in the source.
    return static_cast<int>(llround(x * 0x4000)); // 0x4000 == 2^14
}

// Test 1: Basic success path with Off == NULL
static void test_SetMatShaper_OffNull_BasicMatrixIdentity() {
    cmsContext ctx = 12345;          // arbitrary non-zero context id
    cmsBool idMat = 1;               // IdentityMat true
    cmsMAT3 Mat;
    cmsVEC3 Off; // Will be NULL in this test

    // Initialize Mat with a simple identity matrix values to verify conversion
    // Mat.v[i].n[j] is used in SetMatShaper
    Mat.v[0].n[0] = 1.0; Mat.v[0].n[1] = 0.0; Mat.v[0].n[2] = 0.0;
    Mat.v[1].n[0] = 0.0; Mat.v[1].n[1] = 1.0; Mat.v[1].n[2] = 0.0;
    Mat.v[2].n[0] = 0.0; Mat.v[2].n[1] = 0.0; Mat.v[2].n[2] = 1.0;

    cmsToneCurve* Curve1[3] = { nullptr, nullptr, nullptr };
    cmsToneCurve* Curve2[3] = { nullptr, nullptr, nullptr };

    // Off is NULL to exercise the default offset branch (0x4000)
    XMatShaperData* p = SetMatShaper(ctx, Curve1, &Mat, NULL, Curve2, idMat);

    EXPECT_TRUE(p != nullptr);
    if (p != nullptr) {
        EXPECT_EQ(p->ContextID, ctx);
        EXPECT_EQ(p->IdentityMat, idMat);

        // Validate Mat conversion: should map 1.0 -> 0x4000 on diagonal, 0.0 -> 0
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int val = static_cast<int>(p->Mat[i][j]);
                if (i == j) {
                    EXPECT_EQ(val, 0x4000);
                } else {
                    EXPECT_EQ(val, 0);
                }
            }
        }

        // Off should be defaulted to 0x4000 when Off == NULL
        EXPECT_EQ(static_cast<int>(p->Off[0]), 0x4000);
        EXPECT_EQ(static_cast<int>(p->Off[1]), 0x4000);
        EXPECT_EQ(static_cast<int>(p->Off[2]), 0x4000);
    }

    // Free resources
    if (p) {
        FreeMatShaper(ctx, p);
    }
}

// Test 2: Off non-NULL but with zero offsets (Off.n[i] == 0). Expect Off[i] == 0x4000 still.
static void test_SetMatShaper_OffNonNullZeros() {
    cmsContext ctx = 54321;
    cmsBool idMat = 0; // false
    cmsMAT3 Mat;
    cmsVEC3 Off;
    Off.n[0] = 0.0; Off.n[1] = 0.0; Off.n[2] = 0.0;

    // Identity matrix for simplicity
    Mat.v[0].n[0] = 0.0; Mat.v[0].n[1] = 0.0; Mat.v[0].n[2] = 0.0;
    Mat.v[1].n[0] = 0.0; Mat.v[1].n[1] = 0.0; Mat.v[1].n[2] = 0.0;
    Mat.v[2].n[0] = 0.0; Mat.v[2].n[1] = 0.0; Mat.v[2].n[2] = 0.0;

    cmsToneCurve* Curve1[3] = { nullptr, nullptr, nullptr };
    cmsToneCurve* Curve2[3] = { nullptr, nullptr, nullptr };

    XMatShaperData* p = SetMatShaper(ctx, Curve1, &Mat, &Off, Curve2, idMat);

    EXPECT_TRUE(p != nullptr);
    if (p != nullptr) {
        EXPECT_EQ(p->ContextID, ctx);
        EXPECT_EQ(p->IdentityMat, idMat);
        // Since Mat is zero, all should be zero
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                EXPECT_EQ(static_cast<int>(p->Mat[i][j]), 0);
            }
        }
        // Off should be Off.n[i] + 0x4000. Here Off.n[i] = 0, so Off[i] == 0x4000
        EXPECT_EQ(static_cast<int>(p->Off[0]), 0x4000);
        EXPECT_EQ(static_cast<int>(p->Off[1]), 0x4000);
        EXPECT_EQ(static_cast<int>(p->Off[2]), 0x4000);
    }

    if (p) FreeMatShaper(ctx, p);
}

// Test 3: Mat non-trivial values with Off NULL to verify that Mat conversion is applied
static void test_SetMatShaper_NonTrivialMat_OffNull() {
    cmsContext ctx = 9999;
    cmsBool idMat = 1;
    cmsMAT3 Mat;
    Mat.v[0].n[0] = 2.0; Mat.v[0].n[1] = -1.0; Mat.v[0].n[2] = 0.5;
    Mat.v[1].n[0] = -0.75; Mat.v[1].n[1] = 1.25; Mat.v[1].n[2] = -0.25;
    Mat.v[2].n[0] = 0.0; Mat.v[2].n[1] = 0.0; Mat.v[2].n[2] = 1.5;

    cmsToneCurve* Curve1[3] = { nullptr, nullptr, nullptr };
    cmsToneCurve* Curve2[3] = { nullptr, nullptr, nullptr };

    XMatShaperData* p = SetMatShaper(ctx, Curve1, &Mat, NULL, Curve2, idMat);

    EXPECT_TRUE(p != nullptr);
    if (p != nullptr) {
        EXPECT_EQ(p->ContextID, ctx);
        EXPECT_EQ(p->IdentityMat, idMat);

        // Check only the diagonal elements since non-trivial off-diagonals are not asserted here.
        // Diagonal values should equal DOUBLE_TO_1FIXED15 of the corresponding inputs.
        // We'll compute expected with the llround-based helper.
        int a00 = to_fixed15(Mat.v[0].n[0]); // 2.0
        int a11 = to_fixed15(Mat.v[1].n[1]); // 1.25
        int a22 = to_fixed15(Mat.v[2].n[2]); // 1.5

        EXPECT_EQ(static_cast<int>(p->Mat[0][0]), a00);
        EXPECT_EQ(static_cast<int>(p->Mat[1][1]), a11);
        EXPECT_EQ(static_cast<int>(p->Mat[2][2]), a22);

        // Off should be defaulted to 0x4000
        EXPECT_EQ(static_cast<int>(p->Off[0]), 0x4000);
        EXPECT_EQ(static_cast<int>(p->Off[1]), 0x4000);
        EXPECT_EQ(static_cast<int>(p->Off[2]), 0x4000);
    }

    if (p) FreeMatShaper(ctx, p);
}

// Run all tests and report summary
static void run_all_tests() {
    test_SetMatShaper_OffNull_BasicMatrixIdentity();
    test_SetMatShaper_OffNonNullZeros();
    test_SetMatShaper_NonTrivialMat_OffNull();
}

int main() {
    run_all_tests();
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 2;
    }
}