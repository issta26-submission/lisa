// Lightweight C++11 unit test harness for FillFirstShaper (fast_8_matsh.c)
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cmath>


// -----------------------------------------------------------------------------
// Minimal type stubs and prototypes to build a self-contained test harness.
// These reproduce just enough of the environment for FillFirstShaper to compile
// and be exercised in unit tests without relying on the full external library.
// -----------------------------------------------------------------------------

// The focal function is declared with C linkage to match the C source file.
extern "C" {
    // Function under test
    void FillFirstShaper(int16_t* Table, void* Curve); // cmsS1Fixed14Number* -> int16_t*, cmsToneCurve* -> void*
    
    // Curve evaluation function used by FillFirstShaper
    // We provide our own stub to control behavior in tests.
    float cmsEvalToneCurveFloat(const void* Curve, float R);
}

// Typedefs to mirror the real types in a minimal way.
// We keep them simple since the tests only rely on memory layout and pointer semantics.
typedef int16_t cmsS1Fixed14Number; // 1 fixed-point with 14 fractional bits
typedef void cmsToneCurve;           // opaque in test; actual structure is not needed here
typedef float cmsFloat32Number;

// Forward declare the actual curve structure used by tests.
// Our cmsEvalToneCurveFloat will cast the Curve pointer to this type.
struct TestCurve {
    float a;
    float b;
};

// Our test curve instance will be passed as cmsToneCurve* but actually points to TestCurve.
static inline const TestCurve* toTestCurve(const cmsToneCurve* Curve) {
    return static_cast<const TestCurve*>(Curve);
}

// Implementation of cmsEvalToneCurveFloat used by FillFirstShaper in tests.
// This function is purposely simplistic: y = a + b * R, then clamped to [0,1].
extern "C" float cmsEvalToneCurveFloat(const void* Curve, float R) {
    const TestCurve* c = toTestCurve(static_cast<const cmsToneCurve*>(Curve));
    float y = c->a + c->b * R;
    if (y < 0.0f) y = 0.0f;
    if (y > 1.0f) y = 1.0f;
    return y;
}

// Provide a compatible round-to-fixed-14 conversion used by the code under test.
static inline cmsS1Fixed14Number DOUBLE_TO_1FIXED14(float y) {
    // 1 FIXED14 scale is 2^14 = 16384
    constexpr int FIX14_SCALE = 1 << 14;
    return static_cast<cmsS1Fixed14Number>(std::round(y * FIX14_SCALE));
}

// Helper to cast a TestCurve instance to cmsToneCurve* for passing into FillFirstShaper
static inline cmsToneCurve* makeCurve(float a, float b) {
    // Allocate on stack in tests; FillFirstShaper will read via cmsEvalToneCurveFloat.
    static TestCurve curve_storage; // reused across calls in this test file
    curve_storage.a = a;
    curve_storage.b = b;
    // Return as opaque cmsToneCurve* to mimic library usage
    return reinterpret_cast<cmsToneCurve*>(&curve_storage);
}

// -----------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions). No GTest/GMock used.
// -----------------------------------------------------------------------------

static int g_failures = 0;

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cout << "EXPECT_EQ FAILED: " << (msg) << " | expected: " << (b) << " actual: " << (a) << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cout << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while(0)

// -----------------------------------------------------------------------------
// Test Suite for FillFirstShaper
// -----------------------------------------------------------------------------

// Test 1: Identity curve y = R (a = 0, b = 1). Each Table[i] should be round(R * 16384).
// Purpose: Validate correct linear mapping and rounding behavior.
void test_FillFirstShaper_identity_curve() {
    const int N = 256;
    cmsS1Fixed14Number Table[N];

    // Curve: y = R
    cmsToneCurve* Curve = makeCurve(0.0f, 1.0f);

    FillFirstShaper(Table, Curve);

    for (int i = 0; i < N; ++i) {
        float R = static_cast<float>(i) / 255.0f;
        // clamped to [0,1], then converted
        float y = R;
        if (y < 0.0f) y = 0.0f;
        if (y > 1.0f) y = 1.0f;
        cmsS1Fixed14Number expected = DOUBLE_TO_1FIXED14(y);
        EXPECT_EQ(Table[i], expected, "Identity curve: mismatch at index " << i);
    }
}

// Test 2: Clamp to zero (y always 0). a = -5.0, b = 0.0. Expect all zeros.
// Purpose: Ensure non-positive outputs are clamped correctly to 0.
void test_FillFirstShaper_negative_curve_clamp_zero() {
    const int N = 256;
    cmsS1Fixed14Number Table[N];

    // Curve: y = -5.0 (then clamped to 0)
    cmsToneCurve* Curve = makeCurve(-5.0f, 0.0f);

    FillFirstShaper(Table, Curve);

    for (int i = 0; i < N; ++i) {
        cmsS1Fixed14Number expected = 0;
        EXPECT_EQ(Table[i], expected, "Negative curve should clamp to zero at index " << i);
    }
}

// Test 3: Saturation (y = min(1, a + b*R)). a = 0, b = 2. Values saturate at 1 for larger R.
// Purpose: Verify clamping near upper bound across multiple points.
// We check a few representative indices for thoroughness.
void test_FillFirstShaper_saturation_curve() {
    const int N = 256;
    cmsS1Fixed14Number Table[N];

    // Curve: y = 0 + 2*R; clamp to 1
    cmsToneCurve* Curve = makeCurve(0.0f, 2.0f);

    FillFirstShaper(Table, Curve);

    // i = 0
    {
        float R = 0.0f;
        float y = R; // 0
        if (y < 0.0f) y = 0.0f;
        if (y > 1.0f) y = 1.0f;
        cmsS1Fixed14Number expected = DOUBLE_TO_1FIXED14(y);
        EXPECT_EQ(Table[0], expected, "Saturation curve: index 0");
    }

    // i = 128 (R ~ 0.502)
    {
        int i = 128;
        float R = static_cast<float>(i) / 255.0f;
        float y = 2.0f * R;
        if (y < 0.0f) y = 0.0f;
        if (y > 1.0f) y = 1.0f;
        cmsS1Fixed14Number expected = DOUBLE_TO_1FIXED14(y);
        EXPECT_EQ(Table[i], expected, "Saturation curve: index 128");
    }

    // i = 255 (R = 1.0)
    {
        int i = 255;
        float R = 1.0f;
        float y = 2.0f * R;
        if (y < 0.0f) y = 0.0f;
        if (y > 1.0f) y = 1.0f;
        cmsS1Fixed14Number expected = DOUBLE_TO_1FIXED14(y);
        EXPECT_EQ(Table[i], expected, "Saturation curve: index 255");
    }
}

// -----------------------------------------------------------------------------
// Main harness
// -----------------------------------------------------------------------------

int main() {
    std::cout << "Running FillFirstShaper unit tests (C++11 harness, no GTest)...\n";

    test_FillFirstShaper_identity_curve();
    test_FillFirstShaper_negative_curve_clamp_zero();
    test_FillFirstShaper_saturation_curve();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Number of failed tests: " << g_failures << "\n";
        return 1;
    }
}