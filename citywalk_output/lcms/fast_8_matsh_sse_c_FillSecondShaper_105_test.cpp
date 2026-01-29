/*
Unit Test Suite for FillSecondShaper (fast_8_matsh_sse.c)

Notes:
- This test suite avoids any external testing framework (no GTest/GMock).
- It uses a small C linkage mock of cmsEvalToneCurveFloat to deterministically drive FillSecondShaper.
- We declare minimal type aliases to satisfy the signatures and provide a controlled cmsEvalToneCurveFloat.
- We perform multiple test scenarios to exercise true/false branches of the transformation logic
  (identity curve, square curve, negative/constant curve triggering clamping, and high-value clamp).
- Each test includes explanatory comments describing intent and expected outcomes.

How to compile (example):
g++ -std=c++11 -Isrc -c test_fill_second_shaper.cpp
g++ -std=c++11 -Isrc -c fast_8_matsh_sse.c
g++ -std=c++11 test_fill_second_shaper.o fast_8_matsh_sse.o -o test_fill_second_shaper

Running:
./test_fill_second_shaper
*/

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cpuid.h>
#include <cstdint>


// Since we are compiling alongside the production C source, we provide
// C linkage for the touched functions to ensure proper symbol resolution.
extern "C" {

// Minimal forward declarations to match the production signatures.
// These typedefs are intentionally lightweight and compatible with the
// usage within FillSecondShaper (they mirror the common LittleCMS types).
typedef unsigned char cmsUInt8Number;
typedef signed char   cmsInt8Number;
typedef int           cmsInt32Number;
typedef float         cmsFloat32Number;
typedef struct _cmsToneCurve cmsToneCurve;

// Prototype of the function under test (from fast_8_matsh_sse.c)
void FillSecondShaper(cmsUInt8Number* Table, cmsToneCurve* Curve);

// Prototype for the CMS tone-curve evaluation function, which we will mock.
cmsFloat32Number cmsEvalToneCurveFloat(cmsToneCurve* Curve, cmsFloat32Number R);

} // extern "C"

// -----------------------------------------------------------------------------
// Mock and test harness to drive deterministic behavior for cmsEvalToneCurveFloat
// -----------------------------------------------------------------------------

// Global control for the mock to switch curve behavior
static int g_curveMode = 0;

// A lightweight dummy curve object to pass to the function; the mock ignores its content.
struct _cmsToneCurve { int dummy; };

// Setter to control mock behavior per test
static inline void SetCurrentCurveMode(int mode)
{
    g_curveMode = mode;
}

// Mock implementation of cmsEvalToneCurveFloat with C linkage so it overrides
// any real implementation linked from the library during tests.
extern "C" cmsFloat32Number cmsEvalToneCurveFloat(cmsToneCurve* Curve, cmsFloat32Number R)
{
    (void)Curve; // Curve is unused by the mock per test design; we drive behavior via mode.

    switch (g_curveMode)
    {
        // Mode 0: Identity curve. Val = R
        case 0:
            return R;

        // Mode 1: Square curve. Val = R^2
        case 1:
            return R * R;

        // Mode 2: Negative constant to trigger clamping on all samples.
        // Val will be negative, leading to w = 0 after clamping.
        case 2:
            return -0.5f;

        // Mode 3: High constant (> 1) to force clamping to 255 for all samples.
        case 3:
            return 1.1f; // > 1.0 ensures w clamps to 255
    }

    // Default fall-back identical to identity
    return R;
}

// -----------------------------------------------------------------------------
// Test helpers and test cases
// -----------------------------------------------------------------------------

// Simple error counter
static int g_failures = 0;

// Helper to print per-test header
static inline void PrintHeader(const char* name)
{
    std::cout << "=== Test: " << name << " ===" << std::endl;
}

// Type alias to match the production code's expected prototype (for test clarity)
typedef unsigned char cmsUInt8Number;

// Reference implementation of the expected output for a given mode and index.
// This is derived from the algorithm inside FillSecondShaper but using the mock.
static int ExpectedW_ForModeAtIndex(int i, int totalI, int mode)
{
    // totalI is not strictly required, but kept for clarity.
    // Compute R based on i
    cmsFloat32Number R = static_cast<cmsFloat32Number>(i) / 16384.0f;
    cmsFloat32Number Val;
    switch (mode)
    {
        case 0: Val = R; break;     // Identity
        case 1: Val = R * R; break; // Square
        case 2: Val = -0.5f; break;  // Negative constant
        case 3: Val = 1.1f; break;   // Constant > 1 to force max clamp
        default: Val = R; break;
    }
    int w = static_cast<int>(Val * 255.0f + 0.5f);
    if (w < 0) w = 0;
    if (w > 255) w = 255;
    return w;
}

// Test 1: Identity curve
static int test_FillSecondShaper_identity()
{
    PrintHeader("FillSecondShaper - Identity Curve");
    // Prepare
    const int N = 0x4001;
    std::vector<cmsUInt8Number> table(N, 0);

    // Use a dummy curve object
    _cmsToneCurve curveDummyIdentity;
    SetCurrentCurveMode(0); // Identity

    FillSecondShaper(table.data(), reinterpret_cast<cmsToneCurve*>(&curveDummyIdentity));

    // Validate a few representative samples including boundaries
    int failures = 0;
    for (int i : {0, 1, 100, 8192, 16384, 16383, 0x4000})
    {
        int expected = ExpectedW_ForModeAtIndex(i, N-1, 0);
        int actual   = static_cast<int>(table[i]);
        if (actual != expected)
        {
            std::cerr << "  Mismatch at i=" << i << ": expected " << expected
                      << ", got " << actual << std::endl;
            ++failures;
        }
    }
    if (failures == 0)
        std::cout << "  PASSED identity checks" << std::endl;
    else
        g_failures += failures;
    return failures;
}

// Test 2: Square curve
static int test_FillSecondShaper_square()
{
    PrintHeader("FillSecondShaper - Square Curve");
    const int N = 0x4001;
    std::vector<cmsUInt8Number> table(N, 0);

    _cmsToneCurve curveDummySquare;
    SetCurrentCurveMode(1); // Square

    FillSecondShaper(table.data(), reinterpret_cast<cmsToneCurve*>(&curveDummySquare));

    int failures = 0;
    // Validate a few representative samples
    for (int i : {0, 1, 100, 8192, 16384})
    {
        int expected = ExpectedW_ForModeAtIndex(i, N-1, 1);
        int actual   = static_cast<int>(table[i]);
        if (actual != expected)
        {
            std::cerr << "  Mismatch at i=" << i << ": expected " << expected
                      << ", got " << actual << std::endl;
            ++failures;
        }
    }

    if (failures == 0)
        std::cout << "  PASSED square curve checks" << std::endl;
    else
        g_failures += failures;
    return failures;
}

// Test 3: Negative constant curve to exercise clamping at lower bound
static int test_FillSecondShaper_negativeClamp()
{
    PrintHeader("FillSecondShaper - Negative Constant (clamp to 0)");
    const int N = 0x4001;
    std::vector<cmsUInt8Number> table(N, 0);

    _cmsToneCurve curveDummyNeg;
    SetCurrentCurveMode(2); // Negative constant

    FillSecondShaper(table.data(), reinterpret_cast<cmsToneCurve*>(&curveDummyNeg));

    int failures = 0;
    // Expect all values clamped to 0
    for (int i = 0; i < N; ++i)
    {
        int actual = static_cast<int>(table[i]);
        if (actual != 0)
        {
            std::cerr << "  Mismatch at i=" << i << ": expected 0, got " << actual << std::endl;
            ++failures;
            // Do not break; continue to collect all failures
        }
    }

    if (failures == 0)
        std::cout << "  PASSED negative clamp case" << std::endl;
    else
        g_failures += failures;
    return failures;
}

// Test 4: High constant curve to exercise clamping at upper bound (255)
static int test_FillSecondShaper_upperClamp()
{
    PrintHeader("FillSecondShaper - High Constant (clamp to 255)");
    const int N = 0x4001;
    std::vector<cmsUInt8Number> table(N, 0);

    _cmsToneCurve curveDummyHigh;
    SetCurrentCurveMode(3); // Value > 1.0 to force max clamping

    FillSecondShaper(table.data(), reinterpret_cast<cmsToneCurve*>(&curveDummyHigh));

    int failures = 0;
    // Expect all values clamped to 255
    for (int i = 0; i < N; ++i)
    {
        int actual = static_cast<int>(table[i]);
        if (actual != 255)
        {
            std::cerr << "  Mismatch at i=" << i << ": expected 255, got " << actual << std::endl;
            ++failures;
        }
    }

    if (failures == 0)
        std::cout << "  PASSED upper clamp case" << std::endl;
    else
        g_failures += failures;
    return failures;
}

// -----------------------------------------------------------------------------
// Main: run all tests
// -----------------------------------------------------------------------------

int main()
{
    // Run tests
    int totalFailures = 0;

    totalFailures += test_FillSecondShaper_identity();
    totalFailures += test_FillSecondShaper_square();
    totalFailures += test_FillSecondShaper_negativeClamp();
    totalFailures += test_FillSecondShaper_upperClamp();

    if (g_failures > 0) {
        std::cerr << "\nTotal failures across tests: " << g_failures << "\n";
    }

    if (totalFailures == 0 && g_failures == 0) {
        std::cout << "\nAll FillSecondShaper tests PASSED." << std::endl;
    } else {
        std::cout << "\nSome tests FAILED. Total (per-test counts): " << totalFailures
                  << " failures in test(s); global failures: " << g_failures << std::endl;
    }

    return (g_failures == 0 && totalFailures == 0) ? 0 : 1;
}