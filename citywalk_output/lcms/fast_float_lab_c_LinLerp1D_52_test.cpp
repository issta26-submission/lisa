// Lightweight C++11 unit test suite for the focal method LinLerp1D
// This test avoids GoogleTest and uses simple, non-terminating
// assertions to maximize code coverage.
// The tests assume the project provides: cmsFloat32Number type,
// the SIGMOID_POINTS macro, and the LinLerp1D function with C linkage.
// We include the library header to obtain these definitions.

#include <vector>
#include <iostream>
#include <cmath>
#include <fast_float_internal.h>


// Import necessary project definitions (types, macros, and function prototype).
// If your build system places these headers in a different path, adjust the include path accordingly.

extern "C" {
    // Declaration of the focal function under test (C linkage).
    // The actual signature in the project is: cmsInline cmsFloat32Number LinLerp1D(cmsFloat32Number Value, const cmsFloat32Number* LutTable);
    cmsFloat32Number LinLerp1D(cmsFloat32Number Value, const cmsFloat32Number* LutTable);
}

// Simple non-terminating test logger and assertion helpers
static int g_failCount = 0;

static void logFailure(const char* testName, const char* message, cmsFloat32Number actual, cmsFloat32Number expected)
{
    ++g_failCount;
    std::cerr << "[FAILED] " << testName << ": " << message
              << " | actual: " << actual << " | expected: " << expected << std::endl;
}

// Floating-point comparison with tolerance
static void expectNear(const char* testName, cmsFloat32Number a, cmsFloat32Number b, cmsFloat32Number tol = 1e-6f)
{
    if (std::fabs(static_cast<double>(a) - static_cast<double>(b)) > static_cast<double>(tol))
    {
        logFailure(testName, "values are not within tolerance", a, b);
    }
}

// Expose test cases with descriptive comments
// Step 2: Unit Test Generation
// - Cover true and false branches of each predicate in LinLerp1D
// - Use a small, monotonic LUT with known values to compute expected interpolations
// - Validate boundary conditions: Value >= 1.0f and Value <= 0.0f
// - Validate a mid-range case to exercise the interpolation path

static void test_ValueGe1_ReturnsLastLutEntry()
{
    // Focal method branch: Value >= 1.0f
    // Prepare a LUT with known entries: 0, 1, 2, ..., SIGMOID_POINTS-1
    const int points = SIGMOID_POINTS;
    std::vector<cmsFloat32Number> lut(points);
    for (int i = 0; i < points; ++i)
        lut[i] = static_cast<cmsFloat32Number>(i);

    cmsFloat32Number value = 2.0f; // any value >= 1.0f
    cmsFloat32Number actual = LinLerp1D(value, lut.data());
    cmsFloat32Number expected = lut[points - 1];

    expectNear("ValueGe1_ReturnsLastLutEntry", actual, expected);
    // Explanatory comment: The function should clamp to the last LUT entry when Value >= 1.
}

static void test_ValueLe0_ReturnsFirstLutEntry()
{
    // Focal method branch: Value <= 0
    const int points = SIGMOID_POINTS;
    std::vector<cmsFloat32Number> lut(points);
    for (int i = 0; i < points; ++i)
        lut[i] = static_cast<cmsFloat32Number>(i);

    cmsFloat32Number value = -0.75f; // Value <= 0 should clamp to first LUT entry
    cmsFloat32Number actual = LinLerp1D(value, lut.data());
    cmsFloat32Number expected = lut[0];

    expectNear("ValueLe0_ReturnsFirstLutEntry", actual, expected);
    // Explanatory comment: The function should return the first LUT entry when Value <= 0.
}

static void test_ValueMidInterpolation()
{
    // Focal method path: 0 < Value < 1 (interpolation between two LUT entries)
    const int points = SIGMOID_POINTS;
    std::vector<cmsFloat32Number> lut(points);
    for (int i = 0; i < points; ++i)
        lut[i] = static_cast<cmsFloat32Number>(i); // simple ascending LUT: y = i

    // Choose a mid-range value that yields a non-trivial interpolation
    cmsFloat32Number value = 0.37f; // between 0 and 1
    cmsFloat32Number actual = LinLerp1D(value, lut.data());

    // Compute expected using the exact same interpolation formula as LinLerp1D
    // to establish a reference result.
    cmsFloat32Number scaled = value * (static_cast<cmsFloat32Number>(points - 1));
    int cell0 = static_cast<int>(std::floor(static_cast<double>(scaled)));
    int cell1 = cell0 + 1;
    cmsFloat32Number rest = scaled - static_cast<cmsFloat32Number>(cell0);
    cmsFloat32Number y0 = lut[cell0];
    cmsFloat32Number y1 = lut[cell1];
    cmsFloat32Number expected = y0 + (y1 - y0) * rest;

    expectNear("ValueMidInterpolation", actual, expected);
    // Explanatory comment: This checks the interpolation path between two LUT entries.
}

static void test_ValueAtBoundaryJustBelowOne()
{
    // Additional boundary test: ensure Value just below 1 interpolates correctly
    const int points = SIGMOID_POINTS;
    std::vector<cmsFloat32Number> lut(points);
    for (int i = 0; i < points; ++i)
        lut[i] = static_cast<cmsFloat32Number>(i);

    cmsFloat32Number value = 0.999f; // just below 1
    cmsFloat32Number actual = LinLerp1D(value, lut.data());

    cmsFloat32Number scaled = value * (static_cast<cmsFloat32Number>(points - 1));
    int cell0 = static_cast<int>(std::floor(static_cast<double>(scaled)));
    int cell1 = cell0 + 1;
    cmsFloat32Number rest = scaled - static_cast<cmsFloat32Number>(cell0);
    cmsFloat32Number y0 = lut[cell0];
    cmsFloat32Number y1 = lut[cell1];
    cmsFloat32Number expected = y0 + (y1 - y0) * rest;

    expectNear("ValueAtBoundaryJustBelowOne", actual, expected);
}

static void runAllTests()
{
    // Step 3: Test Case Refinement
    // Run all defined test cases; logs will indicate failures without stopping on first error.
    test_ValueGe1_ReturnsLastLutEntry();
    test_ValueLe0_ReturnsFirstLutEntry();
    test_ValueMidInterpolation();
    test_ValueAtBoundaryJustBelowOne();
}

// Main entry point to execute tests
int main()
{
    // Explain what we are testing for clarity in logs
    std::cout << "Starting LinLerp1D unit tests (C++11, no GTest dependency)" << std::endl;

    runAllTests();

    if (g_failCount == 0)
    {
        std::cout << "[OK] All LinLerp1D tests passed." << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "[SUMMARY] LinLerp1D tests completed with " << g_failCount << " failure(s)." << std::endl;
        return 1;
    }
}